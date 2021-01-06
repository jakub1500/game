#include <thread>
#include "world/field.h"

#pragma once

class Board {
    unsigned int x,y;
    Field** fields;

    

    public:
    Field& get_field_by_cords(unsigned int x, unsigned int y);
    unsigned int x_size, y_size;
    Board(unsigned int _x_size, unsigned int _y_size);
    ~Board();

};

class Screen {
    unsigned int x_size;
    unsigned int y_size;
    std::thread thr_screen;
    bool exit;
    char c;
    void print_simple(std::string text);

    public:
    Board board;
    Screen(void);
    ~Screen();

    void print();
    void show_board();

};