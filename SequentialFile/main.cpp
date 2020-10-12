#include <iostream>

#include "SequentialFile.h"
#include "Record.h"
#include "functions.h"

int main() {

    SequentialFile<Team<long> > teamsSequentialFile = SequentialFile<Team<long> >("data/Teams.bin", "data/TeamsSequentialFile.bin");

    std::vector<Team<long> > teams = teamsSequentialFile.load();

    //shortPrintTeams(teams);

    for (Team<long> team : teams) {
        Team<long> foundTeam = teamsSequentialFile.searchInOrderedRegisters(team.ID);
        foundTeam.shortPrint();
    }
    
    return 0;
}