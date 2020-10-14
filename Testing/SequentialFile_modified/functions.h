#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <fstream>

#include "Record.h"
#include "SequentialFile.h"

void shortPrintTeams(std::vector<Team<long> > teams, unsigned long totalOrderedRegisters) {
    std::cout << std::setw(4) << "ID" << " | " << std::setw(20) << "Equipo" << " | " << std::setw(4) << "next" << " | " 
        << std::setw(4) << "prev" << " | " << std::setw(4) << "pos" << std::endl;
    for (int i = 0; i < teams.size(); ++i) {
        if (i == totalOrderedRegisters) {
            std::cout << "------------------------------------------------" << std::endl;
        }
        teams[i].shortPrint();
        std::cout << std::setw(4) << i << std::endl;
    }
}

void shortPrintPlayers(std::vector<Player<long> > players, unsigned long totalOrderedRegisters) {
    for (int i = 0; i < players.size(); ++i) {
        if (i == totalOrderedRegisters) {
            std::cout << "------------------------------------------------" << std::endl;
        }
        players[i].shortPrint();
        std::cout << " | " << std::setw(4) << i << std::endl;
    }
}

void printMessage(std::string message) {
    std::cout << "\n" << message << std::endl;
}

void searchAndPrintTeams(std::vector<Team<long> > teams, SequentialFile<Team<long> > &teamsSequentialFile) {
    for (Team<long> team : teams) {
        Team<long> foundTeam = teamsSequentialFile.search(team.ID);
        foundTeam.shortPrint();
        std::cout << std::endl;
    }
}

void searchAndPrintTeam(long ID, SequentialFile<Team<long> > &teamsSequentialFile) {
    Team<long> foundTeam = teamsSequentialFile.search(ID);
    foundTeam.shortPrint();
    std::cout << std::endl;
}

void searchAndPrintTeamsRange(long begin, long end, SequentialFile<Team<long> > &teamsSequentialFile) {
    std::vector<Team<long> > searchResult = teamsSequentialFile.searchByRanges(begin, end);
    for (Team<long> team : searchResult) {
        team.shortPrint();
        putchar('\n');
    }
}

void deleteAndPrint(long IDtoDelete, SequentialFile<Team<long> > &teamsSequentialFile) {
    teamsSequentialFile.deleteRecord(IDtoDelete);
    std::vector<Team<long> > teams = teamsSequentialFile.load();
    shortPrintTeams(teams, teamsSequentialFile.getTotalOrderedRecords());
}

void writeCSV(std::string filename, std::vector<long> v){
    std::ofstream myFile(filename);
    for(long i = 0; i < v.size(); ++i)
        myFile << v.at(i) << "\n";
    myFile.close();
}

#endif