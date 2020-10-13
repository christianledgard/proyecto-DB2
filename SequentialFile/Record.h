#ifndef RECORD_H
#define RECORD_H

#include <string>
#include <iostream>
#include <iomanip>

template <typename Key>
struct Record {
    typedef Key KeyType;
    KeyType ID;
};

template <typename Key>
struct Player : public Record<Key> {
    char surname[20];
    char team[20];
    char position[20];
    long minutes, shots, passes, tackles, saves;
    long next, prev;

    Player(const std::string& surname, const std::string& team, const std::string& position, long minutes,
        long shots, long passes, long tackles, long saves) : minutes(minutes), shots(shots), 
        passes(passes), tackles(tackles), saves(saves) {
        std::strcpy(this->surname,surname.c_str());
        std::strcpy(this->team,team.c_str());
        std::strcpy(this->position,position.c_str());
    }


    Player(Key ID, long next) {
        this->ID = ID;
        std::strcpy(this->surname, "deleted");
        std::strcpy(this->team, "deleted");
        std::strcpy(this->position, "deleted");
        this->next = next;
        minutes = shots = passes = tackles = saves = next = prev = 0;
    }

    Player() {}

    void print() {
        std::cout << this->ID << " | " << surname << " | " << team << " | "  << position << " | "
            << minutes << " | " << shots << " | " << passes << " | " << tackles << " | " << saves <<
            " | " << next << " | " << prev << std::endl;
    }

    void shortPrint() {
        std::cout << std::setw(4) << this->ID << " | " << std::setw(20) << surname << " | " << std::setw(4) << next 
        << " | " << std::setw(4) << prev << std::endl;
    }

};

template <typename Key>
struct Team : public Record<Key> {
    char team[20];
    long ranking, games, wins, draws, losses, goalsFor,
    goalsAgainst, yellowCards, redCards;
    long next, prev;

    Team(Key ID, const std::string& team, long ranking, long games, long wins, long draws, long losses, 
        long goalsFor, long goalsAgainst, long yellowCards, long redCards) : ranking(ranking), games(games), 
        wins(wins), draws(draws), losses(losses), goalsFor(goalsFor), goalsAgainst(goalsAgainst), 
        yellowCards(yellowCards), redCards(redCards) {
            this->ID = ID;
            std::strcpy(this->team,team.c_str());
    }

    Team(Key ID, long next) {
        this->ID = ID;
        std::strcpy(this->team, "deleted");
        this->next = next;
        prev = ranking = games = wins = draws = losses = goalsFor = goalsAgainst = yellowCards = redCards = 0;
    }

    Team() {}

    void print() {
        std::cout << this->ID << " | " << team << " | " << ranking << " | "  << games << " | "
            << wins << " | " << draws << " | " << losses << " | " << goalsFor << " | " << goalsAgainst <<
            " | " << yellowCards << " | " << redCards << " | " << next << " | " << prev << std::endl;
    }

    void shortPrint() {
        std::cout << std::setw(4) << this->ID << " | " << std::setw(20) << team << " | " << std::setw(4) << next 
        << " | " << std::setw(4) << prev << " | ";
    }
    
};

#endif
