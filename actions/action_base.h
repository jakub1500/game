#pragma once

#include "protocol/protocol_msg.h"

class Action_Base {
    Msg_Type msg_type;

    public:
    virtual void send_action(void) = 0;
    virtual void process_action(void) = 0;
};

class Single_Action {

};