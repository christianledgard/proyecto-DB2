#ifndef SEQUENTIAL_FILE_H
#define SEQUENTIAL_FILE_H

#include <vector>
#include <string>
#include <fstream>

#include "functions.h"

template <typename RecordType>
class SequentialFile {

public:
    typedef typename RecordType::KeyType KeyType;

private:

    std::string inputFileName;
    std::string sequentialFileName;
    unsigned long totalOrderedRecords;
    unsigned long totalUnorderedRecords;

    unsigned long getFileSize(std::string newFileName) {
        std::ifstream file(newFileName, std::ios::ate | std::ios::binary);
        unsigned long size = file.tellg();
        file.close();
        return size;
    }

    void initializeSequentialFile() {
        std::ifstream inputFile(this->inputFileName, std::ios::in);
        std::ofstream sequentialFile(this->sequentialFileName, std::ios::out);

        RecordType record;
        long currentNext = 1;
        long currentPrev = -1;

        unsigned long totalLines = getFileSize(this->inputFileName) / (sizeof(RecordType) - 2 * sizeof(long));
        while (inputFile.read((char *) &record, sizeof(RecordType) - 2 * sizeof(long))) {
            record.next = totalLines == currentNext ? -2 : currentNext++;
            record.prev = currentPrev++;
            sequentialFile.write((char *) &record, sizeof(RecordType));
        }

        totalOrderedRecords = totalLines;
        totalUnorderedRecords = 0;
    }

    RecordType getPrevRecord(RecordType record) {
        std::fstream sequentialFile(this->sequentialFileName, std::ios::in);
        while (record.prev > totalOrderedRecords - 2) {
            sequentialFile.seekg(record.prev * sizeof(RecordType));
            sequentialFile.read((char *) &record, sizeof(record));
        }
        return record;
    }

    void simpleInsert(RecordType baseRecord, RecordType record) {
        std::fstream sequentialFileIn(this->sequentialFileName);

        RecordType baseRecordNext;
        sequentialFileIn.seekg(baseRecord.next * sizeof(RecordType));
        sequentialFileIn.read((char *) &baseRecordNext, sizeof(RecordType));
        sequentialFileIn.close();

        long currentRecordLogPos = totalOrderedRecords + totalUnorderedRecords;
        long baseRecordLogPos = baseRecordNext.prev;
        long baseRecordNextLogPos = baseRecord.next;

        baseRecord.next = currentRecordLogPos;
        record.prev = baseRecordLogPos;
        record.next = baseRecordNextLogPos;
        baseRecordNext.prev = currentRecordLogPos;

        std::fstream sequentialFileOut(this->sequentialFileName);

        sequentialFileOut.seekp(baseRecordLogPos * sizeof(RecordType));
        sequentialFileOut.write((char *) &baseRecord, sizeof(RecordType));

        sequentialFileOut.seekp(currentRecordLogPos * sizeof(RecordType));
        sequentialFileOut.write((char *) &record, sizeof(RecordType));

        sequentialFileOut.seekp(baseRecordNextLogPos * sizeof(RecordType));
        sequentialFileOut.write((char *) &baseRecordNext, sizeof(RecordType));
    }

    // insert "toInsert" to the left of "currentRecord"
    void insertUpdatingPointers(RecordType toInsert, RecordType currentRecord) {
        std::fstream sequentialFileIn(this->sequentialFileName);
        std::fstream sequentialFileOut(this->sequentialFileName);

        // get previous record
        long prevRecordLogPos = currentRecord.prev;
        sequentialFileIn.seekg(prevRecordLogPos * sizeof(RecordType));
        RecordType prevRecord;
        sequentialFileIn.read((char *) &prevRecord, sizeof(RecordType));

        // get logical position of currentRecord
        long currentRecLogPos = prevRecord.next;
        // calculate logical position of toInsert
        long toInsertLogPos = totalOrderedRecords + totalUnorderedRecords;

        // update prevRecord.next
        prevRecord.next = toInsertLogPos;
        sequentialFileOut.seekp(prevRecordLogPos * sizeof(RecordType));
        sequentialFileOut.write((char *) &prevRecord, sizeof(RecordType));

        // set toInsert pointers
        toInsert.next = currentRecLogPos;
        toInsert.prev = prevRecordLogPos;
        sequentialFileOut.seekp(toInsertLogPos * sizeof(RecordType));
        sequentialFileOut.write((char *) &toInsert, sizeof(RecordType));

        // update currentRecord.prev
        currentRecord.prev = toInsertLogPos;
        sequentialFileOut.seekp(currentRecLogPos * sizeof(RecordType));
        sequentialFileOut.write((char *) &currentRecord, sizeof(RecordType));

        sequentialFileIn.close();
        sequentialFileOut.close();
    }

    void insertAtFirstPosition(RecordType record) {
        std::fstream sequentialFileIn(this->sequentialFileName);

        RecordType firstRecord;
        sequentialFileIn.seekg(0);
        sequentialFileIn.read((char *) &firstRecord, sizeof(RecordType));

        RecordType firstRecordNext;
        sequentialFileIn.seekg(firstRecord.next * sizeof(RecordType));
        sequentialFileIn.read((char *) &firstRecordNext, sizeof(RecordType));
        sequentialFileIn.close();

        record.next = totalOrderedRecords + totalUnorderedRecords;
        firstRecordNext.prev = totalOrderedRecords + totalUnorderedRecords;
        record.prev = -1;

        firstRecord.prev = 0;

        std::fstream sequentialFileOut(this->sequentialFileName);

        sequentialFileOut.seekp(0);
        sequentialFileOut.write((char *) &record, sizeof(RecordType));

        sequentialFileOut.seekp((totalOrderedRecords + totalUnorderedRecords) * sizeof(RecordType));
        sequentialFileOut.write((char *) &firstRecord, sizeof(RecordType));

        sequentialFileOut.seekp(firstRecord.next * sizeof(RecordType));
        sequentialFileOut.write((char *) &firstRecordNext, sizeof(RecordType));

        sequentialFileOut.close();
    }

public:

    SequentialFile(std::string inputFileName, std::string sequentialFileName) {
        this->inputFileName = inputFileName;
        this->sequentialFileName = sequentialFileName;

        this->initializeSequentialFile();
    }

    RecordType searchInOrderedRecords(KeyType ID) {
        unsigned long low = 0, high = totalOrderedRecords - 1, mid;

        std::fstream sequentialFile(this->sequentialFileName, std::ios::in);
        RecordType currentRecord;

        while (low <= high) {
            mid = (low + high) / 2;
            sequentialFile.seekg(mid * sizeof(RecordType));
            sequentialFile.read((char *) &currentRecord, sizeof(RecordType));
            KeyType currentID = currentRecord.ID;

            if (currentID < ID) {
                low = mid + 1;
            } else if (currentID > ID) {
                high = mid - 1;
            } else {
                return currentRecord;
            }
        }

        throw std::out_of_range("Record with ID" + std::to_string(ID) + "not found in ordered registers");
    }

    void insertAll(std::vector<RecordType> registers) {
        std::sort(registers.begin(), registers.end());
        std::fstream file(this->fileName, std::ios::out);

        unsigned long nextPointer = 0;
        // el puntero prev a null es -1
        unsigned long prevPointer = -1;

        if (getFileSize(this->fileName) == 0) {
            for (int i = 0; i < registers.size(); ++i) {
                file.seekg(nextPointer * sizeof(RecordType));
                if (i == registers.size() - 1) {
                    // el puntero next a null es -2
                    registers[i].next = -2;
                } else {
                    registers[i].next = ++nextPointer;
                }
                registers[i].prev = prevPointer++;
                file.write((char *) &registers[i], sizeof(RecordType));
            }
            totalOrderedRecords += registers.size();
        }
    }

    std::vector<RecordType> load() {
        std::vector<RecordType> records;
        std::fstream sequentialFile(this->sequentialFileName, std::ios::in);

        RecordType record = RecordType();
        while (sequentialFile.read((char *) &record, sizeof(record))) {
            records.push_back(record);
        }

        return records;
    }

    void insert(RecordType record) {
        RecordType baseRecord = this->searchInOrderedRecords(record.ID);

        if (baseRecord.ID == record.ID) {
            throw std::out_of_range("User attempted to insert an already existing ID");
        }

        if (baseRecord.prev == -1 && record.ID < baseRecord.ID) { // insert at the beginning
            this->insertAtFirstPosition(record);
        } else { 
            if (baseRecord.ID > record.ID) {
                baseRecord = this->getPrevRecord(baseRecord);
            }
            if (baseRecord.next < totalOrderedRecords) {
                this->simpleInsert(baseRecord, record);
            } else {
                std::fstream sequentialFileIn(this->sequentialFileName);

                RecordType current;

                long baseRecordLogPos;
                if (baseRecord.next == -2) {
                    baseRecordLogPos = totalOrderedRecords - 1;
                } else {
                    RecordType baseRecordNext;
                    sequentialFileIn.seekg(baseRecord.next * sizeof(RecordType));
                    sequentialFileIn.read((char *) &baseRecordNext, sizeof(RecordType));
                    baseRecordLogPos = baseRecordNext.prev;
                }

                sequentialFileIn.seekg(baseRecordLogPos * sizeof(RecordType));
                sequentialFileIn.read((char *) &current, sizeof(RecordType));
                while (current.next > totalOrderedRecords - 1 && current.next != -2 && current.ID < record.ID) {
                    if (current.ID == record.ID) {
                        throw std::out_of_range("User attempted to insert an already existing ID");
                    }
                    sequentialFileIn.seekg(current.next * sizeof(RecordType));
                    sequentialFileIn.read((char *) &current, sizeof(RecordType));
                }
                if (current.next < totalOrderedRecords) {
                    if (current.ID > record.ID) {
                        this->insertUpdatingPointers(record, current);
                    } else {
                        long currentNextLogPos = current.next;
                        sequentialFileIn.seekg(currentNextLogPos * sizeof(RecordType));
                        RecordType currentNext;
                        sequentialFileIn.read((char *) &currentNext, sizeof(RecordType));
                        this->insertUpdatingPointers(record, currentNext);
                    }
                } else {
                    this->insertUpdatingPointers(record, current);
                }
            }
        }
        ++totalUnorderedRecords;
    }

    unsigned long getTotalOrderedRecords() {
        return this->totalOrderedRecords;
    }

};

#endif