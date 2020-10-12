#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <fstream>

#include "Record.h"
#include "SequentialFile.h"

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

#endif