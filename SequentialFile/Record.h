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

    bool operator < (const Player<Key> &r1) const {
        std::string r1Surname(r1.surname);
        return this->surname < r1Surname;
    }
};

template <typename Key>
struct Team : public Record<Key> {
    char team[20];
    long ranking, games, wins, draws, losses, goalsFor,
    goalsAgainst, yellowCards, redCards;
    long next, prev;

    bool operator < (const Team<Key> &r1) const {
        std::string r1Team(r1.team);
        return this->team < r1Team;
    }

    void print() {
        std::cout << this->ID << " | " << team << " | " << ranking << " | "  << games << " | "
            << wins << " | " << draws << " | " << losses << " | " << goalsFor << " | " << goalsAgainst <<
            " | " << yellowCards << " | " << redCards << " | " << next << " | " << prev << std::endl;
    }

    void shortPrint() {
        std::cout << std::setw(4) << this->ID << " | " << std::setw(20) << team << " | " << std::setw(4) << next 
        << " | " << std::setw(4) << prev << std::endl;
    }
    
};

#endif
