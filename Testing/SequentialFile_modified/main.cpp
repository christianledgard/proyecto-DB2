#include <iostream>
#include <chrono>

#include "SequentialFile.h"
#include "Record.h"
#include "functions.h"

#define MAX_OPERATIONS 10000

int main() {

    SequentialFile<Player<long> > playersSequentialFile = SequentialFile<Player<long> >("data/Players.bin", "data/PlayersSequentialFile.bin");

    std::vector<long> insertTimes, searchTimes, insertDiskAccess, searchDiskAccess, deleteTimes, deleteDiskAccess;

    for (long i = 0; i < MAX_OPERATIONS; ++i) {
        auto t1 = std::chrono::high_resolution_clock::now();

        playersSequentialFile.insert(Player<long>(i/2,"APELLIDO" ,"EQUIPO", "POSICION", i, i, i, i, i));
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        insertTimes.push_back(duration);
        insertDiskAccess.push_back(totalDiskAccess);
        totalDiskAccess = 0;
        std::cout << "insert" << i / 2 << std::endl;
    }

    writeCSV("test_results/time_insert_sq.csv", insertTimes);
    writeCSV("test_results/disk_access_insert_sq.csv", insertDiskAccess);

    for (long i = 0; i < MAX_OPERATIONS; ++i) {
        auto t1 = std::chrono::high_resolution_clock::now();
        playersSequentialFile.search(i/2);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        searchTimes.push_back(duration);
        searchDiskAccess.push_back(totalDiskAccess);
        totalDiskAccess = 0;
        std::cout << "search" << i / 2 << std::endl;
    }

    writeCSV("test_results/time_search_sq.csv", searchTimes);
    writeCSV("test_results/disk_access_search_sq.csv", searchDiskAccess);

    for (long i = 0; i < MAX_OPERATIONS; ++i) {
        auto t1 = std::chrono::high_resolution_clock::now();
        playersSequentialFile.deleteRecord(i/2);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        deleteTimes.push_back(duration);
        deleteDiskAccess.push_back(totalDiskAccess);
        totalDiskAccess = 0;
        std::cout << "delete: " << i / 2 << " size: " << playersSequentialFile.getTotalOrderedRecords() <<std::endl;
    }

    writeCSV("test_results/time_delete_sq.csv", deleteTimes);
    writeCSV("test_results/disk_access_delete_sq.csv", deleteDiskAccess);

    return 0;
}