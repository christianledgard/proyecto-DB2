#ifndef SEQUENTIAL_FILE_H
#define SEQUENTIAL_FILE_H

#include <vector>
#include <string>
#include <fstream>

int totalDiskAccess = 0;

template <typename RecordType>
class SequentialFile {

public:
    typedef typename RecordType::KeyType KeyType;

private:

    std::string inputFileName;
    std::string sequentialFileName;
    std::string headerFileName;
    std::string auxFileName;

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
            totalDiskAccess += 2;
        }

        totalOrderedRecords = totalLines;
        totalUnorderedRecords = 0;
    }

    void initializeFreeList() {
        std::fstream header(this->headerFileName, std::ios::out);
        long headerPointer = -1;
        header.seekp(0);
        header.write((char *) &headerPointer, sizeof(headerPointer));
        ++totalDiskAccess;
        header.close();
    }

    RecordType getPrevRecord(RecordType record) {
        if (record.next == -2) {
            record = this->read(this->sequentialFileName, totalOrderedRecords - 2);
        } else {
            record = this->read(this->sequentialFileName, record.next);
            record = this->read(this->sequentialFileName, record.prev - 1);
        }
        return record;
    }

    void simpleInsert(RecordType baseRecord, RecordType record) {
        RecordType baseRecordNext = this->read(this->sequentialFileName, baseRecord.next);

        long currentRecordLogPos = this->findWhereToInsert();
        long baseRecordLogPos = baseRecordNext.prev;
        long baseRecordNextLogPos = baseRecord.next;

        baseRecord.next = currentRecordLogPos;
        record.prev = baseRecordLogPos;
        record.next = baseRecordNextLogPos;
        baseRecordNext.prev = currentRecordLogPos;

        this->write(baseRecord, this->sequentialFileName, baseRecordLogPos);
        this->write(record, this->sequentialFileName, currentRecordLogPos);
        this->write(baseRecordNext, this->sequentialFileName, baseRecordNextLogPos);
    }

    // insert "toInsert" to the left of "currentRecord"
    void insertUpdatingPointers(RecordType toInsert, RecordType currentRecord) {
        // get previous record
        long prevRecordLogPos = currentRecord.prev;
        RecordType prevRecord = this->read(this->sequentialFileName, prevRecordLogPos);

        // get logical position of currentRecord
        long currentRecLogPos = prevRecord.next;
        // calculate logical position of toInsert
        long toInsertLogPos = this->findWhereToInsert();

        // update prevRecord.next
        prevRecord.next = toInsertLogPos;
        this->write(prevRecord, this->sequentialFileName, prevRecordLogPos);

        // set toInsert pointers
        toInsert.next = currentRecLogPos;
        toInsert.prev = prevRecordLogPos;
        this->write(toInsert, this->sequentialFileName, toInsertLogPos);

        // update currentRecord.prev
        currentRecord.prev = toInsertLogPos;
        this->write(currentRecord, this->sequentialFileName, currentRecLogPos);
    }

    void insertAtFirstPosition(RecordType record) {
        RecordType firstRecord = this->read(this->sequentialFileName, this->getFirstRecordLogPos());
        RecordType firstRecordNext = this->read(this->sequentialFileName, firstRecord.next);

        long toInsertLogPos = this->findWhereToInsert();

        record.next = toInsertLogPos;
        firstRecordNext.prev = toInsertLogPos;
        record.prev = -1;

        firstRecord.prev = 0;

        this->write(record, this->sequentialFileName, this->getFirstRecordLogPos());
        this->write(firstRecord, this->sequentialFileName, toInsertLogPos);
        this->write(firstRecordNext, this->sequentialFileName, firstRecord.next);
    }

    void insertAtLastPosition(RecordType record) {
        RecordType lastRecord = this->read(this->sequentialFileName, totalOrderedRecords - 1);

        record.next = -2;
        record.prev = totalOrderedRecords - 1;

        long toInsertLogPos = this->findWhereToInsert();

        lastRecord.next = toInsertLogPos;

        this->write(lastRecord, this->sequentialFileName, totalOrderedRecords - 1);
        this->write(record, this->sequentialFileName, toInsertLogPos);
    }

    void insertAfterNull(RecordType current, RecordType record) {
        RecordType currentRecordPrev = this->read(this->sequentialFileName, current.prev);

        record.next = -2;
        record.prev = currentRecordPrev.next;

        long toInsertLogPos = this->findWhereToInsert();

        current.next = toInsertLogPos;

        this->write(current, this->sequentialFileName, currentRecordPrev.next);
        this->write(record, this->sequentialFileName, toInsertLogPos);
    }

    void rebuildAfterInsert() {
        unsigned long totalLines = getFileSize(this->sequentialFileName) / sizeof(RecordType);

        this->rebuild(totalLines);

        totalOrderedRecords += 5;
        totalUnorderedRecords = 0;
    }

    RecordType searchInOrderedRecords(KeyType ID) {
        long low = 0, high = totalOrderedRecords - 1, mid;

        RecordType currentRecord;

        while (low <= high) {
            mid = (low + high) / 2;
            currentRecord = this->read(this->sequentialFileName, mid);
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
        if (toDelete.prev == -1) { // first register
            RecordType toDeleteNext = this->read(this->sequentialFileName, toDelete.next);
            toDeleteNext.prev = -1;
            this->write(toDeleteNext, this->sequentialFileName, toDelete.next);
        } else if (toDelete.next == - 2) { // register whose next is null
            RecordType toDeletePrev;
            toDeletePrev = this->read(this->sequentialFileName, toDelete.prev);
            toDeletePrev.next = -2;
            this->write(toDeletePrev, this->sequentialFileName, toDelete.prev);
        } else { // normal case
            RecordType toDeletePrev, toDeleteNext;
            toDeletePrev = this->read(this->sequentialFileName, toDelete.prev);
            toDeleteNext = this->read(this->sequentialFileName, toDelete.next);
            toDeletePrev.next = toDelete.next;
            toDeleteNext.prev = toDelete.prev;
            this->write(toDeletePrev, this->sequentialFileName, toDelete.prev);
            this->write(toDeleteNext, this->sequentialFileName, toDelete.next);
        }
    }

    void deleteOrderedRecord(long toDeleteLogPos) {
        unsigned long totalLines = getFileSize(this->sequentialFileName) / sizeof(RecordType) - 1;

        this->rebuild(totalLines);

        totalOrderedRecords = totalOrderedRecords + totalUnorderedRecords;
        totalUnorderedRecords = 0;
    }

    long readHeader() {
        std::fstream header(this->headerFileName);
        long headerValue;
        header.seekg(0);
        header.read((char *) &headerValue, sizeof(headerValue));
        header.close();
        ++totalDiskAccess;
        return headerValue;
    }

    void writeHeader(long toDeleteLogPos) {
        std::fstream header(this->headerFileName);
        header.seekp(0);
        header.write((char *) &toDeleteLogPos, sizeof(toDeleteLogPos));
        ++totalDiskAccess;
        header.close();
    }

    void deleteUnorderedRecord(long toDeleteLogPos) {
        long headerTemp = readHeader();
        writeHeader(toDeleteLogPos);
        RecordType toDelete(-1, headerTemp); // mark as deleted with ID -1
        this->write(toDelete, this->sequentialFileName, toDeleteLogPos);
    }

    long getFirstRecordLogPos() {
        long currentRecordLogPos = 0;
        RecordType currentRecord = this->read(this->sequentialFileName, currentRecordLogPos);
        while (currentRecord.prev == -1) {
            currentRecord = this->read(this->sequentialFileName, ++currentRecordLogPos);
        }
        return currentRecordLogPos - 1;
    }

    void rebuild(unsigned long totalLines) {
        std::fstream sequentialFile(this->sequentialFileName);
        std::fstream auxFile(this->auxFileName, std::ios::out);

        RecordType record = this->read(this->sequentialFileName, this->getFirstRecordLogPos());
        while (record.next != -2) {
            auxFile.write((char *) &record, sizeof(RecordType));
            record = this->read(this->sequentialFileName, record.next);
            ++totalDiskAccess;
        }
        auxFile.write((char *) &record, sizeof(RecordType));
        ++totalDiskAccess;

        sequentialFile.close();
        auxFile.close();

        sequentialFile.open(this->sequentialFileName, std::ios::out);
        auxFile.open(this->auxFileName);

        long currentNext = 1;
        long currentPrev = -1;

        while (auxFile.read((char *) &record, sizeof(RecordType))) {
            record.next = totalLines == currentNext ? -2 : currentNext++;
            record.prev = currentPrev++;
            sequentialFile.write((char *) &record, sizeof(RecordType));
            totalDiskAccess += 2;
        }

        sequentialFile.close();
        auxFile.close();
    }

    long getLogicalPosition(RecordType record) {
        if (record.prev == -1) {
            return 0;
        } else {
            RecordType prevRecord = this->read(this->sequentialFileName, record.prev);
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
        ++totalDiskAccess;
        file.close();
        return record;
    }

    void write(RecordType record, std::string fileName, long position) {
        std::fstream file(fileName);
        file.seekp(position * sizeof(RecordType));
        file.write((char *) &record, sizeof(RecordType));
        ++totalDiskAccess;
        file.close();
    }

    void insertBetweenUnorderedRecords(RecordType baseRecord, RecordType toInsert) {
        RecordType current;

        long baseRecordLogPos;
        RecordType baseRecordNext = this->read(this->sequentialFileName, baseRecord.next);
        baseRecordLogPos = baseRecordNext.prev;

        current = this->read(this->sequentialFileName, baseRecordLogPos);
        while (current.next > totalOrderedRecords - 1 && current.next != -2 && current.ID < toInsert.ID) { // find where to insert
            if (current.ID == toInsert.ID) {
                --totalUnorderedRecords;
                return;
            }
            current = this->read(this->sequentialFileName, current.next);
        }
        if (current.ID == toInsert.ID) { // check last register ID
            --totalUnorderedRecords;
            return;
        }
        if (current.next < totalOrderedRecords) { // if current points to ordered records
            if (current.ID > toInsert.ID) {
                this->insertUpdatingPointers(toInsert, current);
            } else {
                if (current.next == -2) {
                    this->insertAfterNull(current, toInsert); // special case
                } else {
                    long currentNextLogPos = current.next;
                    RecordType currentNext = this->read(this->sequentialFileName, currentNextLogPos);
                    this->insertUpdatingPointers(toInsert, currentNext); // insert record to the left of currentNext
                }
            }
        } else { // if current points to an unordered record
            this->insertUpdatingPointers(toInsert, current); // insert record to the left of current
        }
    }

public:

    SequentialFile(std::string inputFileName, std::string sequentialFileName) {
        this->inputFileName = inputFileName;
        this->sequentialFileName = sequentialFileName;
        this->headerFileName = inputFileName.substr(0, inputFileName.size() - 4) + "Header.bin";
        this->auxFileName = inputFileName.substr(0, inputFileName.size() - 4) + "AuxFile.bin";

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
            return baseRecord;
        }

        if (baseRecord.ID > ID) {
            baseRecord = this->getPrevRecord(baseRecord);
        }

        RecordType current = baseRecord;

        current = this->read(this->sequentialFileName, current.next);
        while (current.ID <= ID) {
            if (current.ID == ID) {
                return current;
            } else {
                current = this->read(this->sequentialFileName, current.next);
            }
        }

        return current;
    }

    std::vector<RecordType> searchByRanges(KeyType begin, KeyType end) {
        if (begin > end) {
            std::swap(begin, end);
        }

        RecordType current = this->searchInOrderedRecords(begin);

        if (current.prev != -1) {
            if (current.ID > begin) {
                current = this->getPrevRecord(current);
            }
        }

        std::vector<RecordType> searchResult;

        while (true) {
            if (current.ID >= begin && current.ID <= end) {
                searchResult.push_back(current);
            }
            if (current.ID > end || current.next == -2) {
                return searchResult;
            }
            current = this->read(this->sequentialFileName, current.next);
        }
    }

    void deleteRecord(KeyType ID) {
        RecordType toDelete = this->search(ID);
        if (toDelete.ID != ID) {
            return;
        }
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

    void insert(RecordType toInsert) {
        RecordType baseRecord = this->searchInOrderedRecords(toInsert.ID);

        if (baseRecord.ID == toInsert.ID) {
            return;
        }

        if (baseRecord.prev == -1 && toInsert.ID < baseRecord.ID) { // insert at the beginning
            this->insertAtFirstPosition(toInsert);
        } else if (baseRecord.next == -2) { // insert at last position
            this->insertAtLastPosition(toInsert);
        } else { 
            if (baseRecord.ID > toInsert.ID) {
                baseRecord = this->getPrevRecord(baseRecord);
            }
            if (baseRecord.next < totalOrderedRecords) {
                this->simpleInsert(baseRecord, toInsert); // when it's not necessary to insert "between" unordered registers
            } else { // when baseRecord points to unordered records
                this->insertBetweenUnorderedRecords(baseRecord, toInsert);
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