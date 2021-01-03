#include "net/net_session.h"

class Action {
    Session* session;
    public:
    Action(Session*);
    void move(Move_Direction);
};

class KeyStrokeEngine {
    Action action;
    public:
    KeyStrokeEngine(Session*);
    void run(void);
};


