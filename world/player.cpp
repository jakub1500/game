#include "player.h"
#include <iostream>

Player::Player(void) : Player("Unknown") {}

Player::Player(std::string _name) : name(_name), layout('X') {}

uint32_t Player::get_pos_x(void) {
    return pos_x;
}

uint32_t Player::get_pos_y(void) {
    return pos_y;
}

void Player::set_pos_x(uint32_t x) {
    std::cout << "Updating player position from pos_x " << pos_x << " to " << x << std::endl;
    pos_x = x;
}

void Player::set_pos_y(uint32_t y) {
    std::cout << "Updating player position from pos_y " << pos_y << " to " << y << std::endl;
    pos_y = y;
}

void Player::set_pos_xy(uint32_t x, uint32_t y) {
    set_pos_x(x);
    set_pos_y(y);
}

char Player::get_layout(void) {
    return layout;
}