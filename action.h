enum Move_Direction {
    Up=0,
    Down=1,
    Left=2,
    Right=3
};

class Action {
    public:
    void move(Move_Direction);
};

class KeyStrokeEngine {
    Action a;
    public:
    KeyStrokeEngine(void);
    void run(void);
};


