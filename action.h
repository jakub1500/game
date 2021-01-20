#pragma once

#include "net/net_session.h"

class Action {
    public:
    Action(void);
    Msg move(Move_Direction);
};

class KeyStrokeEngine {
    Action action;
    Session* session;
    public:
    KeyStrokeEngine(Session*);
    void run(void);
};


