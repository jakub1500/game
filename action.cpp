#include "action.h"
#include <ncurses.h>

void Action::move(Move_Direction direction) {

}

KeyStrokeEngine::KeyStrokeEngine() {

}

void KeyStrokeEngine::run() {
    char c;

    while(1)
    {
        c = getch();
        if (c == 'w') {
            a.move(Move_Direction::Up);
        } else if (c == 's') {
            a.move(Move_Direction::Down);
        } else if (c == 'a') {
            a.move(Move_Direction::Left);
        } else if (c == 'd') {
            a.move(Move_Direction::Right);
        } else {
            break;
        }

    }
}