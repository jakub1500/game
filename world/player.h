#pragma once

#include <string>
#include <cstdint>

class Player {
    std::string name;
    uint32_t pos_x=500;
    uint32_t pos_y=500;
    char layout;

    public:
    Player(void);
    Player(std::string);
    uint32_t get_pos_x(void);
    uint32_t get_pos_y(void);
    void set_pos_x(uint32_t);
    void set_pos_y(uint32_t);
    void set_pos_xy(uint32_t, uint32_t);
    char get_layout(void);
};