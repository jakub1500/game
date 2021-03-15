#pragma once

#include "processers/processer_game.h"

class Action {
    public:
    Action(void);
    Msg move(Move_Direction);
};

class KeyStrokeEngine {
    Action action;
    Processer_Client* processer_client;
    public:
    KeyStrokeEngine(Processer_Client*);
    void run(void);
};


