#pragma once

#include "processers/processer_base.h"
#include "world/world.h"
#include "net/net_common.h"
#include <vector>
#include "net/net_session.h"


class Processer_Server : public Processer_Base {

    void move_player(Move_Direction);

    public:
    World* world;
    Processer_Server(std::vector<Session*>*, World&);
    void process_msg(Msg&) override;
    Msg create_msg(void) override;
    int loop(void) override;
};