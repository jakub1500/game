#include <iostream>
#include <string>
#include <ncurses.h>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

enum Direction {
    up=0,
    down=1,
    left=2,
    right=3
};

class Field {

    public:

    char x;
    Field() {
        x = ' ';
    }
    char get_char(void) {
        return x;
    }
};
class Board {
    unsigned int x_size, y_size;
    unsigned int x,y;
    Field** fields;

    Field& get_field_by_cords(unsigned int x, unsigned int y) {
        Field* ptr = ((*(fields+x) + y));
        return *ptr;
    }

    public:
    Board(unsigned int _x_size, unsigned int _y_size)
    : x_size(_x_size), y_size(_y_size) {
        fields = new Field*[x_size];
        for (unsigned int i=0; i < x_size; i++) {
            *(fields+i) = new Field[y_size];
        }
        x = 0;
        y = 0;
    }
    ~Board() {
        for (int i=0; i < x_size; i++) {
            delete *(fields+i);
        }
        delete fields;
    }
    
    /*
        +----------------> x(j)
        |
        |
        |
        |
        |
        V

        y(i)
    */
    void print(void) {
        for (unsigned int i = 0; i < y_size + 2; i++) {
            for (unsigned int j = 0; j < x_size + 2; j++) {
                if ((i == 0 && (j == 0 || j == x_size + 1)) ||
                    ((i == y_size + 1 && (j == 0 || j == x_size + 1)))) {
                    _print("+");
                } else if (i == 0 || i == y_size + 1) {
                    _print("-");
                } else if (j == 0 || j == x_size + 1) {
                     _print("|");
                } else {
                    Field t_field = get_field_by_cords(j-1, i-1);
                    _print(std::string(1, t_field.x));
                }
            }
            _print("\n");
        }
    }
    void _print(std::string text) {
        printw(text.c_str());
    }

    void move(Direction direction) {
        Field& a = get_field_by_cords(x,y);
        a.x = ' ';
        if (direction == Direction::up) {
            y--;
        } else if (direction == Direction::down) {
            y++;
        } else if (direction == Direction::left) {
            x--;
        } else if (direction == Direction::right) {
            x++;
        }
        Field& b = get_field_by_cords(x,y);
        b.x = 'X';
    }

};

int main() {
    Board b(70, 30);
    bool exit = false;
    char c;
    initscr();
    cbreak();
    noecho();

    std::thread windows_thread = std::thread([&]{
        while(!exit) {
            clear();
            b.print();
            std::this_thread::sleep_for(30ms);
        }
    });

    while(1)
    {

        c = getch();
        if (c == 'w') {
            b.move(Direction::up);
        } else if (c == 's') {
            b.move(Direction::down);
        } else if (c == 'a') {
            b.move(Direction::left);
        } else if (c == 'd') {
            b.move(Direction::right);
        } else {
            break;
        }

    }
    endwin();

    exit=true;
    windows_thread.join();
    return 0;
}