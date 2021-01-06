#include <thread>
#include <ncurses.h>
#include <chrono>
#include <string>
#include "screen.h"

using namespace std::chrono_literals;



Board::Board(unsigned int _x_size, unsigned int _y_size) : x_size(_x_size), y_size(_y_size) {
    fields = new Field*[x_size];
    for (unsigned int i=0; i < x_size; i++) {
        *(fields+i) = new Field[y_size];
    }
    x = 0;
    y = 0;
}
Board::~Board() {
    for (int i=0; i < x_size; i++) {
        delete *(fields+i);
    }
    delete fields;
}
Field& Board::get_field_by_cords(unsigned int x, unsigned int y) {
    Field* ptr = ((*(fields+x) + y));
    return *ptr;
}

Screen::Screen(void) : exit(false), board(71,31) {
    x_size = 100;
    y_size = 31;
    // initscr();
    // cbreak();
    // noecho();
    thr_screen = std::thread([&]{
        while(!exit) {
            // clear();
            // print();
            std::this_thread::sleep_for(30ms);
        }
    });
};
Screen::~Screen() {
    endwin();
    exit = true;
    thr_screen.join();
};
void Screen::print() {
    for (unsigned int i = 0; i < board.y_size + 2; i++) {
        for (unsigned int j = 0; j < board.x_size + 2; j++) {
            if ((i == 0 && (j == 0 || j == board.x_size + 1)) ||
                ((i == board.y_size + 1 && (j == 0 || j == board.x_size + 1)))) {
                print_simple("+");
            } else if (i == 0 || i == board.y_size + 1) {
                print_simple("-");
            } else if (j == 0 || j == board.x_size + 1) {
                 print_simple("|");
            } else {
                Field t_field = board.get_field_by_cords(j-1, i-1);
                print_simple(std::string(1, t_field.get_image()));
            }
        }
        print_simple("\n");
    }
}
void Screen::print_simple(std::string text) {
    printw(text.c_str());
}

void Screen::show_board(void) {
    
}
