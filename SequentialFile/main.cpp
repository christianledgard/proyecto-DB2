#include <iostream>

#include "SequentialFile.h"
#include "Record.h"
#include "functions.h"

int main() {

    SequentialFile<Team<long> > teamsSequentialFile = SequentialFile<Team<long> >("data/Teams.bin", "data/TeamsSequentialFile.bin");


    //shortPrintTeams(teams);

    teamsSequentialFile.insert(Team<long>(99, "Jesus", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    teamsSequentialFile.insert(Team<long>(97, "Christian", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    teamsSequentialFile.insert(Team<long>(98, "Carlos", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    teamsSequentialFile.insert(Team<long>(100, "Fabrizio", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    teamsSequentialFile.insert(Team<long>(10, "Fabrizio", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    // teamsSequentialFile.insert(Team<long>(998, "Fabrizio", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    // teamsSequentialFile.insert(Team<long>(996, "Heider", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    // teamsSequentialFile.insert(Team<long>(997, "YouTube", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    // teamsSequentialFile.insert(Team<long>(999, "Vimeo", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    std::vector<Team<long> > teams = teamsSequentialFile.load();

    shortPrintTeams(teams, teamsSequentialFile.getTotalOrderedRecords());
    
    return 0;
}