#include <iostream>

#include "SequentialFile.h"
#include "Record.h"
#include "functions.h"

int main() {

    SequentialFile<Team<long> > teamsSequentialFile = SequentialFile<Team<long> >("data/Teams.bin", "data/TeamsSequentialFile.bin");

    teamsSequentialFile.insert(Team<long>(99, "Jesus", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    teamsSequentialFile.insert(Team<long>(97, "Christian", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    teamsSequentialFile.insert(Team<long>(98, "Carlos", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    printMessage("Se ha agregado a: Jesus, Christian y Carlos");
    std::vector<Team<long> > teams = teamsSequentialFile.load();
    shortPrintTeams(teams, teamsSequentialFile.getTotalOrderedRecords());

    teamsSequentialFile.insert(Team<long>(100, "Fabrizio", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    teamsSequentialFile.insert(Team<long>(10, "Juan", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    printMessage("Se ha agregado a: Fabrizio y Juan. Se dió la primera reconstrucción");
    teams = teamsSequentialFile.load();
    shortPrintTeams(teams, teamsSequentialFile.getTotalOrderedRecords());

    teamsSequentialFile.insert(Team<long>(998, "Pedro", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    teamsSequentialFile.insert(Team<long>(996, "Heider", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    teamsSequentialFile.insert(Team<long>(997, "YouTube", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    teamsSequentialFile.insert(Team<long>(999, "Vimeo", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    printMessage("Se ha agregado a: Pedro, Heider, YouTuber y Vimeo");
    teams = teamsSequentialFile.load();
    shortPrintTeams(teams, teamsSequentialFile.getTotalOrderedRecords());

    printMessage("Buscando a Pedro:");
    searchAndPrintTeam(998, teamsSequentialFile);
    printMessage("Buscando a Heider:");
    searchAndPrintTeam(996, teamsSequentialFile);
    printMessage("Buscando a YouTube:");
    searchAndPrintTeam(997, teamsSequentialFile);
    printMessage("Buscando a Vimeo:");
    searchAndPrintTeam(999, teamsSequentialFile);

    teamsSequentialFile.insert(Team<long>(777, "Hercules", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    printMessage("Se ha agregado a Hercules. Segunda reconstrucción");
    teams = teamsSequentialFile.load();
    shortPrintTeams(teams, teamsSequentialFile.getTotalOrderedRecords());

    printMessage("Buscando e imprimiendo todos los registros");
    searchAndPrintTeams(teams, teamsSequentialFile);

    printMessage("Borrando a Juan (primer registro)");
    deleteAndPrint(10, teamsSequentialFile);

    printMessage("Borrando a Vimeo (último registro)");
    deleteAndPrint(999, teamsSequentialFile);

    printMessage("Borrando a New Zealand");
    deleteAndPrint(551, teamsSequentialFile);

    printMessage("Borrando a Greece");
    deleteAndPrint(949, teamsSequentialFile);

    teamsSequentialFile.insert(Team<long>(453, "Peru", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    teamsSequentialFile.insert(Team<long>(202, "Ciudad del Vaticano", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    teamsSequentialFile.insert(Team<long>(750, "San Marino", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    teamsSequentialFile.insert(Team<long>(600, "Monaco", 1, 1, 1, 1, 1, 1, 1, 1, 1));
    printMessage("Se ha agregado a: Peru, Ciudad del Vaticano, San Marino y Monaco");
    teams = teamsSequentialFile.load();
    shortPrintTeams(teams, teamsSequentialFile.getTotalOrderedRecords());

    return 0;
}