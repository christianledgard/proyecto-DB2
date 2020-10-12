#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <fstream>

#include "Record.h"
#include "SequentialFile.h"

unsigned long getFileSize(std::string newFileName) {
    std::ifstream file(newFileName, std::ios::ate | std::ios::binary);
    unsigned long size = file.tellg();
    file.close();
    return size;
}

void shortPrintTeams(std::vector<Team<long> > teams, unsigned long totalOrderedRegisters) {
    std::cout << std::setw(4) << "ID" << " | " << std::setw(20) << "Equipo" << " | " << std::setw(4) << "next" << " | " 
        << std::setw(4) << "prev" << std::endl;
    for (int i = 0; i < teams.size(); ++i) {
        if (i == totalOrderedRegisters) {
            std::cout << "-----------------------------------------" << std::endl;
        }
        teams[i].shortPrint();
    }
}

// void searchAndPrint(SequentialFile<Team<long> > &sf, std::vector<Team<long> > records) {
//     for (Team<long> record : records) {
//         Team<long> foundRecord = sf.searchInOrderedRegisters(record.ID);
//         foundRecord.shortPrint();
//     }
// }

#endif