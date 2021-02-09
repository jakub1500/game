#pragma once

#include "world/world.h"
#include "net/net_common.h"
#include <vector>
#include "net/net_session.h"

class Processer_Base {
    public:
    std::vector<Session*>* vec_sessions;

    public:
    Processer_Base(std::vector<Session*>*);
    virtual void process_msg(Msg&) = 0;
    virtual Msg create_msg(void) = 0;
    virtual int loop(void) = 0;
};