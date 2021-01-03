#include "../world/world.h"
#include "net_common.h"

#pragma once

class Msg_Processer_Base {
    public:
    virtual void process_msg(Msg&);
};

class Msg_Processer_Client : public Msg_Processer_Base {

    public:
    Msg_Processer_Client(void);
    void process_msg(Msg&) override;
};

class Msg_Processer_Server : public Msg_Processer_Base {
    World* world;

    void move_player(Move_Direction);

    public:
    Msg_Processer_Server(World&);
    void process_msg(Msg&) override;
};