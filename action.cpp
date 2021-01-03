#include "action.h"
#include <ncurses.h>
#include <iostream>

Action::Action(Session* _session) : session(_session) {}

void Action::move(Move_Direction direction) {
    Msg msg(Msg_type::MOVE_PLAYER);
    msg << direction;
    std::cout << msg;
    session->out_fifo.push_element(std::move(msg));
}

KeyStrokeEngine::KeyStrokeEngine(Session* _session) : action(_session){}

void KeyStrokeEngine::run() {
    char c;

    while(1)
    {
        c = getch();
        std::cout << c << std::endl;
        if (c == 'w') {
            action.move(Move_Direction::Up);
        } else if (c == 's') {
            action.move(Move_Direction::Down);
        } else if (c == 'a') {
            action.move(Move_Direction::Left);
        } else if (c == 'd') {
            action.move(Move_Direction::Right);
        } else {
            break;
        }

    }
}