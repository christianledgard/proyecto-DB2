template <typename Key>
struct Record{
    typedef Key KeyType;
    KeyType ID;
    long prevDelete=-2;
    Record(KeyType id) : ID(id) {}
    Record()=default;
};

template <typename Key>
struct Player: public Record<Key>{
    char surname[20];
    char team[20];
    char position[20];
    long minutes, shots, passes, tackles,saves;

    Player()=default;

    Player(Key id,  string surnamex,
           string teamx,
           string positionx,
    long minutes, long shots, long passes,
    long tackles, long saves){
        this->ID=id;

        strcpy(surname,surnamex.c_str());
        strcpy(team,teamx.c_str());
        strcpy(position,positionx.c_str());

        this->minutes=minutes;
        this->shots=shots;
        this->passes=passes;
        this->tackles=shots;
        this->saves=saves;
    }
};

template <typename Key>
struct Team: public Record<Key>{
    char team[20];
    long ranking, games, wins, draws, losses, goalsFor,
    goalsAgainst, yellowCards, redCards;
};