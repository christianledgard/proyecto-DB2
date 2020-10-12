#ifndef RECORD_HASH_H
#define RECORD_HASH_H

template <typename Key>
struct RecordHash{
    typedef Key KeyType;
    KeyType ID;
    long prevDelete=0;
    RecordHash(KeyType id) : ID(id) {}
    RecordHash()=default;
};

template <typename Key>
struct PlayerHash: public RecordHash<Key>{
    char surname[20];
    char team[20];
    char position[20];
    long minutes, shots, passes, tackles, saves;
};

template <typename Key>
struct TeamHash: public RecordHash<Key>{
    char team[20];
    long ranking, games, wins, draws, losses, goalsFor,
    goalsAgainst, yellowCards, redCards;
};

#endif
