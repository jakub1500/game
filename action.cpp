#include "action.h"
#include <ncurses.h>
#include <iostream>

Action::Action(void) {}

Msg Action::move(Move_Direction direction) {
    Msg msg(Msg_Type::MOVE_PLAYER);
    msg << direction;
    return msg; 
}

KeyStrokeEngine::KeyStrokeEngine(Processer_Client* _processer_client) : processer_client(_processer_client){}

void KeyStrokeEngine::run() {
    char c;

    while(1)
    {
        c = getch();
        Msg msg;
        if (c == 'w') {
            msg = action.move(Move_Direction::Up);
        } else if (c == 's') {
            msg = action.move(Move_Direction::Down);
        } else if (c == 'a') {
            msg = action.move(Move_Direction::Left);
        } else if (c == 'd') {
            msg = action.move(Move_Direction::Right);
        } else {
            break;
        }

        // let's assume that client side can have only one session
        Session* ses = processer_client->vec_sessions->back();
        ses->send_msg(std::move(msg));
    }
}