#include <iostream>

#include "SequentialFile.h"
#include "Record.h"
#include "functions.h"

int main() {

    SequentialFile<Team<long> > teamsSequentialFile = SequentialFile<Team<long> >("data/Teams.bin", "data/TeamsSequentialFile.bin");


    //shortPrintTeams(teams);

    teamsSequentialFile.insert(Team<long>(100, "Jesus", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    std::vector<Team<long> > teams = teamsSequentialFile.load();

    shortPrintTeams(teams, teamsSequentialFile.getTotalOrderedRecords());

    
    return 0;
}