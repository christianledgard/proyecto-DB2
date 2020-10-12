template <typename KeyType>
struct Record{
    KeyType ID;
    long prevDelete;
};

template <typename KeyType>
struct Player: public Record<KeyType>{
    char surname[20];
    char team[20];
    char position[20];
    long minutes, shots, passes, tackles, saves;
};


template <typename KeyType>
struct Team: public Record<KeyType>{
    char team[20];
    long ranking, games, wins, draws, losses, goalsFor,
    goalsAgainst, yellowCards, redCards;
};
