#ifndef SEQUENTIAL_FILE_H
#define SEQUENTIAL_FILE_H

#include <vector>
#include <string>
#include <fstream>

template <typename RecordType>
class SequentialFile {

public:
    typedef typename RecordType::KeyType KeyType;

private:

    std::string inputFileName;
    std::string sequentialFileName;
    long totalOrderedRecords;
    long totalUnorderedRecords;

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

    void initializeFreeList() {
        std::fstream header("data/header.bin", std::ios::out);
        long headerPointer = -1;
        header.seekp(0);
        header.write((char *) &headerPointer, sizeof(headerPointer));
        header.close();
    }

    RecordType getPrevRecord(RecordType record) {
        std::fstream sequentialFile(this->sequentialFileName, std::ios::in);
        if (record.next == -2) {
            sequentialFile.seekg((totalOrderedRecords - 2) * sizeof(RecordType));
            sequentialFile.read((char *) &record, sizeof(RecordType));
        } else {
            sequentialFile.seekg(record.next * sizeof(RecordType));
            sequentialFile.read((char *) &record, sizeof(RecordType));
            sequentialFile.seekg((record.prev - 1) * sizeof(RecordType));
            sequentialFile.read((char *) &record, sizeof(RecordType));
        }
        return record;
    }

    void simpleInsert(RecordType baseRecord, RecordType record) {
        std::fstream sequentialFileIn(this->sequentialFileName);

        RecordType baseRecordNext;
        sequentialFileIn.seekg(baseRecord.next * sizeof(RecordType));
        sequentialFileIn.read((char *) &baseRecordNext, sizeof(RecordType));
        sequentialFileIn.close();

        long currentRecordLogPos = this->findWhereToInsert();
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
        long toInsertLogPos = this->findWhereToInsert();

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

        long toInsertLogPos = this->findWhereToInsert();

        record.next = toInsertLogPos;
        firstRecordNext.prev = toInsertLogPos;
        record.prev = -1;

        firstRecord.prev = 0;

        std::fstream sequentialFileOut(this->sequentialFileName);

        sequentialFileOut.seekp(0);
        sequentialFileOut.write((char *) &record, sizeof(RecordType));

        sequentialFileOut.seekp((toInsertLogPos) * sizeof(RecordType));
        sequentialFileOut.write((char *) &firstRecord, sizeof(RecordType));

        sequentialFileOut.seekp(firstRecord.next * sizeof(RecordType));
        sequentialFileOut.write((char *) &firstRecordNext, sizeof(RecordType));

        sequentialFileOut.close();
    }

    void insertAtLastPosition(RecordType record) {
        std::fstream sequentialFileIn(this->sequentialFileName);

        RecordType lastRecord;
        sequentialFileIn.seekg((totalOrderedRecords - 1) * sizeof(RecordType));
        sequentialFileIn.read((char *) &lastRecord, sizeof(RecordType));
        sequentialFileIn.close();

        record.next = -2;
        record.prev = totalOrderedRecords - 1;

        long toInsertLogPos = this->findWhereToInsert();

        lastRecord.next = toInsertLogPos;

        std::fstream sequentialFileOut(this->sequentialFileName);

        sequentialFileOut.seekp((totalOrderedRecords - 1) * sizeof(RecordType));
        sequentialFileOut.write((char *) &lastRecord, sizeof(RecordType));

        sequentialFileOut.seekp((toInsertLogPos) * sizeof(RecordType));
        sequentialFileOut.write((char *) &record, sizeof(RecordType));

        sequentialFileOut.close();
    }

    void insertAfterNull(RecordType current, RecordType record) {
        std::fstream sequentialFileIn(this->sequentialFileName);

        RecordType currentRecordPrev;
        sequentialFileIn.seekg(current.prev * sizeof(RecordType));
        sequentialFileIn.read((char *) &currentRecordPrev, sizeof(RecordType));
        sequentialFileIn.close();

        record.next = -2;
        record.prev = currentRecordPrev.next;

        long toInsertLogPos = this->findWhereToInsert();

        current.next = toInsertLogPos;

        std::fstream sequentialFileOut(this->sequentialFileName);

        sequentialFileOut.seekp(currentRecordPrev.next * sizeof(RecordType));
        sequentialFileOut.write((char *) &current, sizeof(RecordType));

        sequentialFileOut.seekp((toInsertLogPos) * sizeof(RecordType));
        sequentialFileOut.write((char *) &record, sizeof(RecordType));

        sequentialFileOut.close();
    }

    void rebuildAfterInsert() {
        unsigned long totalLines = getFileSize(this->sequentialFileName) / sizeof(RecordType);

        this->rebuild(totalLines);

        totalOrderedRecords += 5;
        totalUnorderedRecords = 0;
    }

    RecordType searchInOrderedRecords(KeyType ID) {
        long low = 0, high = totalOrderedRecords - 1, mid;

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

        return currentRecord;
    }

    void updatePointersDelete(RecordType toDelete, long toDeleteLogPos) {
        std::fstream sequentialFileIn(this->sequentialFileName);
        std::fstream sequentialFileOut(this->sequentialFileName);

        if (toDelete.prev == -1) { // first register
            RecordType toDeleteNext;
            sequentialFileIn.seekg(toDelete.next * sizeof(RecordType));
            sequentialFileIn.read((char *) &toDeleteNext, sizeof(RecordType));

            toDeleteNext.prev = -1;
            
            sequentialFileOut.seekp(toDelete.next * sizeof(RecordType));
            sequentialFileOut.write((char *) &toDeleteNext, sizeof(RecordType));
        } else if (toDelete.next == - 2) { // register whose next is null
            RecordType toDeletePrev;
            sequentialFileIn.seekg(toDelete.prev * sizeof(RecordType));
            sequentialFileIn.read((char *) &toDeletePrev, sizeof(RecordType));

            toDeletePrev.next = -2;

            sequentialFileOut.seekp(toDelete.prev * sizeof(RecordType));
            sequentialFileOut.write((char *) &toDeletePrev, sizeof(RecordType));
        } else { // normal case
            RecordType toDeletePrev, toDeleteNext;

            sequentialFileIn.seekg(toDelete.prev * sizeof(RecordType));
            sequentialFileIn.read((char *) &toDeletePrev, sizeof(RecordType));
            sequentialFileIn.seekg(toDelete.next * sizeof(RecordType));
            sequentialFileIn.read((char *) &toDeleteNext, sizeof(RecordType));

            toDeletePrev.next = toDelete.next;
            toDeleteNext.prev = toDelete.prev;

            sequentialFileOut.seekp(toDelete.prev * sizeof(RecordType));
            sequentialFileOut.write((char *) &toDeletePrev, sizeof(RecordType));
            sequentialFileOut.seekp(toDelete.next * sizeof(RecordType));
            sequentialFileOut.write((char *) &toDeleteNext, sizeof(RecordType));
        }

        sequentialFileIn.close();
        sequentialFileOut.close();
    }

    void deleteOrderedRecord(long toDeleteLogPos) {
        unsigned long totalLines = getFileSize(this->sequentialFileName) / sizeof(RecordType) - 1;

        this->rebuild(totalLines);

        totalOrderedRecords = totalOrderedRecords + totalUnorderedRecords;
        totalUnorderedRecords = 0;
    }

    long readHeader() {
        std::fstream header("data/header.bin");
        long headerValue;
        header.seekg(0);
        header.read((char *) &headerValue, sizeof(headerValue));
        header.close();
        return headerValue;
    }

    void writeHeader(long toDeleteLogPos) {
        std::fstream header("data/header.bin");
        header.seekp(0);
        header.write((char *) &toDeleteLogPos, sizeof(toDeleteLogPos));
        header.close();
    }

    void deleteUnorderedRecord(long toDeleteLogPos) {
        long headerTemp = readHeader();
        writeHeader(toDeleteLogPos);
        RecordType toDelete(-1, headerTemp); // mark as deleted with ID -1
        
        std::fstream sequentialFileOut(this->sequentialFileName);
        sequentialFileOut.seekp(toDeleteLogPos * sizeof(RecordType));
        sequentialFileOut.write((char *) &toDelete, sizeof(RecordType));
        sequentialFileOut.close();
    }

    long getFirstRecordLogPos() {
        std::fstream sequentialFile(this->sequentialFileName);

        long currentRecordLogPos = 0;

        RecordType currentRecord;
        sequentialFile.seekg(currentRecordLogPos * sizeof(RecordType));
        sequentialFile.read((char *) &currentRecord, sizeof(RecordType));
        while (currentRecord.prev == -1) {
            sequentialFile.seekg(++currentRecordLogPos * sizeof(RecordType));
            sequentialFile.read((char *) &currentRecord, sizeof(RecordType));
        }

        return currentRecordLogPos - 1;
    }

    void rebuild(unsigned long totalLines) {
        std::fstream sequentialFileIn(this->sequentialFileName);
        std::fstream auxFile("data/auxFile.bin", std::ios::out);

        RecordType record;
        sequentialFileIn.seekg(this->getFirstRecordLogPos() * sizeof(RecordType));
        sequentialFileIn.read((char *) &record, sizeof(RecordType));
        while (record.next != -2) {
            auxFile.write((char *) &record, sizeof(RecordType));
            sequentialFileIn.seekg(record.next * sizeof(RecordType));
            sequentialFileIn.read((char *) &record, sizeof(record));
        }
        auxFile.write((char *) &record, sizeof(RecordType));

        sequentialFileIn.close();
        auxFile.close();

        std::fstream sequentialFileOut(this->sequentialFileName, std::ios::out);
        auxFile.open("data/auxFile.bin");

        long currentNext = 1;
        long currentPrev = -1;

        while (auxFile.read((char *) &record, sizeof(RecordType))) {
            record.next = totalLines == currentNext ? -2 : currentNext++;
            record.prev = currentPrev++;
            sequentialFileOut.write((char *) &record, sizeof(RecordType));
        }

        sequentialFileOut.close();
        auxFile.close();
    }

    long getLogicalPosition(RecordType record) {
        if (record.prev == -1) {
            return 0;
        } else {
            std::fstream sequentialFile(this->sequentialFileName);
            RecordType prevRecord;
            sequentialFile.seekg(record.prev * sizeof(RecordType));
            sequentialFile.read((char *) &prevRecord, sizeof(RecordType));
            return prevRecord.next;
        }
    }

    // finds where to insert in unordered records, considering free list
    long findWhereToInsert() {
        long currentHeader = readHeader();
        if (currentHeader == -1) {
            return totalOrderedRecords + totalUnorderedRecords;
        } else {
            RecordType deleted = read(this->sequentialFileName, currentHeader);
            writeHeader(deleted.next);
            return currentHeader;
        }
    }

    RecordType read(std::string fileName, long position) {
        std::fstream file(fileName);
        RecordType record;
        file.seekg(position * sizeof(RecordType));
        file.read((char *) &record, sizeof(RecordType));
        file.close();
        return record;
    }

    void write(RecordType record, std::string fileName, long position) {
        std::fstream file(fileName);
        file.seekp(position * sizeof(RecordType));
        file.write((char *) &record, sizeof(RecordType));
        file.close();
    }

public:

    SequentialFile(std::string inputFileName, std::string sequentialFileName) {
        this->inputFileName = inputFileName;
        this->sequentialFileName = sequentialFileName;

        this->initializeSequentialFile();
        this->initializeFreeList();
    }

    SequentialFile() {}

    std::vector<RecordType> load() {
        std::vector<RecordType> records;
        std::fstream sequentialFile(this->sequentialFileName, std::ios::in);

        RecordType record = RecordType();
        while (sequentialFile.read((char *) &record, sizeof(record))) {
            records.push_back(record);
        }

        return records;
    }

    RecordType search(KeyType ID) {
        RecordType baseRecord = this->searchInOrderedRecords(ID);

        if (baseRecord.ID == ID) {
            return baseRecord;
        }

        if ((baseRecord.prev == -1 && ID < baseRecord.ID) || (baseRecord.next == -2 && ID > baseRecord.ID)) {
            throw std::out_of_range("Search out of range. ID: " + std::to_string(ID));
        }

        if (baseRecord.ID > ID) {
            baseRecord = this->getPrevRecord(baseRecord);
        }

        RecordType current = baseRecord;

        std::fstream sequentialFile(this->sequentialFileName);
        sequentialFile.seekg(current.next * sizeof(RecordType));
        sequentialFile.read((char *) &current, sizeof(RecordType));
        while (current.ID <= ID) {
            if (current.ID == ID) {
                return current;
            } else {
                sequentialFile.seekg(current.next * sizeof(RecordType));
                sequentialFile.read((char *) &current, sizeof(RecordType));
            }
        }

        throw std::out_of_range("Search out of range. ID: " + std::to_string(ID));
    }

    void deleteRecord(KeyType ID) {
        RecordType toDelete = this->search(ID);
        long toDeleteLogPos = this->getLogicalPosition(toDelete);

        this->updatePointersDelete(toDelete, toDeleteLogPos);

        if (toDeleteLogPos < totalOrderedRecords) {
            this->deleteOrderedRecord(toDeleteLogPos);
            --totalOrderedRecords;
        } else {
            this->deleteUnorderedRecord(toDeleteLogPos);
            --totalUnorderedRecords;
        }
    }

    void insert(RecordType record) {
        RecordType baseRecord = this->searchInOrderedRecords(record.ID);

        if (baseRecord.ID == record.ID) {
            throw std::out_of_range("User attempted to insert an already existing ID");
        }

        if (baseRecord.prev == -1 && record.ID < baseRecord.ID) { // insert at the beginning
            this->insertAtFirstPosition(record);
        } else if (baseRecord.next == -2) {
            this->insertAtLastPosition(record);
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
                RecordType baseRecordNext;
                sequentialFileIn.seekg(baseRecord.next * sizeof(RecordType));
                sequentialFileIn.read((char *) &baseRecordNext, sizeof(RecordType));
                baseRecordLogPos = baseRecordNext.prev;

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
                        if (current.next == -2) {
                            this->insertAfterNull(current, record);
                        } else {
                            long currentNextLogPos = current.next;
                            sequentialFileIn.seekg(currentNextLogPos * sizeof(RecordType));
                            RecordType currentNext;
                            sequentialFileIn.read((char *) &currentNext, sizeof(RecordType));
                            this->insertUpdatingPointers(record, currentNext);
                        }
                    }
                } else {
                    this->insertUpdatingPointers(record, current);
                }
            }
        }
        if (++totalUnorderedRecords == 5) {
            this->rebuildAfterInsert();
        }
    }

    long getTotalOrderedRecords() {
        return this->totalOrderedRecords;
    }

};

#endif