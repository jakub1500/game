#include "../world/world.h"
#include "net_common.h"
#include "../screen.h"

#pragma once

class Msg_Processer_Base {
    public:
    virtual void process_msg(Msg&) = 0;
    virtual Msg create_msg(void) = 0;
};

class Msg_Processer_Client : public Msg_Processer_Base {
    Screen* screen;

    public:
    Msg_Processer_Client(Screen*);
    void process_msg(Msg&) override;
    Msg create_msg(void) override;
};

class Msg_Processer_Server : public Msg_Processer_Base {

    void move_player(Move_Direction);

    public:
    World* world;
    Msg_Processer_Server(World&);
    void process_msg(Msg&) override;
    Msg create_msg(void) override;
};