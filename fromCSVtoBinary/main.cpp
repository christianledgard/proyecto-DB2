#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include "Record.h"
using namespace std;

void Teams() {
    fstream file("Teams.csv",ios::in);
    std::string line;

    std::vector<Team<long>> vec;

    Team<long> team;

    while (getline(file, line)) {
        vector<string> result;
        stringstream s_stream(line);
        while(s_stream.good()) {
            string substr;
            getline(s_stream, substr, ',');
            result.push_back(substr);
        }
        team.ID = stol(result.at(0));
        strcpy(team.team, result.at(1).c_str());
        team.ranking = stol(result.at(2));
        team.games = stol(result.at(3));
        team.wins = stol(result.at(4));
        team.draws = stol(result.at(5));
        team.losses = stol(result.at(6));
        team.goalsFor = stol(result.at(7));
        team.goalsAgainst = stol(result.at(8));
        team.yellowCards = stol(result.at(9));
        team.redCards = stol(result.at(10));

        vec.push_back(team);
    }

    file.close();
    fstream fileOut("Teams.bin",ios::out);


    for (auto& it : vec) {
        fileOut.write((char *) &it, sizeof(Team<long>));
    }

    fileOut.close();

}


void Players() {
    fstream file("Players.csv",ios::in);
    std::string line;

    std::vector<Player<long>> vec;

    Player<long> player;

    int i = 0;
    while (getline(file, line)) {
        vector<string> result;
        stringstream s_stream(line); //create string stream from the string
        while(s_stream.good()) {
            string substr;
            getline(s_stream, substr, ','); //get first string delimited by comma
            result.push_back(substr);
        }
        player.ID = stol(result.at(0));
        strcpy(player.surname, result.at(1).c_str());
        strcpy(player.team, result.at(2).c_str());
        strcpy(player.position, result.at(3).c_str());
        player.minutes = stol(result.at(4));
        player.shots = stol(result.at(5));
        player.passes = stol(result.at(6));
        player.tackles = stol(result.at(7));
        player.saves = stol(result.at(8));

        vec.push_back(player);
    }

    file.close();
    fstream fileOut("Players.bin",ios::out);


    for (auto& it : vec) {
        fileOut.write((char *) &it, sizeof(Player<long>));
    }

    fileOut.close();

}

int main(){
    Teams(); Players();
}
