#pragma once

#include "processers/processer_base.h"
#include "world/world.h"
#include "net/net_common.h"
#include "screen.h"
#include <vector>
#include "net/net_session.h"


class Processer_Client : public Processer_Base {
    Screen* screen;

    public:
    Processer_Client(std::vector<Session*>*, Screen*);
    void process_msg(Msg&) override;
    Msg create_msg(void) override;
    int loop(void) override;
};