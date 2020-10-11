template <typename Key>
struct Record{
    typedef Key KeyType;
    KeyType ID;
    long prevDelete;
};

template <typename Key>
struct Player: public Record<Key>{
    char surname[20];
    char team[20];
    char position[20];
    long minutes, shots, passes, tackles, saves;
};

template <typename Key>
struct Team: public Record<Key>{
    char team[20];
    long ranking, games, wins, draws, losses, goalsFor,
    goalsAgainst, yellowCards, redCards;
};