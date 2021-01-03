#include "field.h"
#include <vector>
#include "player.h"

#pragma once

class Map {
    Field** field_map;
    const char* map_file_name = "world.map";
    const unsigned int map_x_size = 1000;
    const unsigned int map_y_size = 1000;
    const unsigned int randomize_perc = 5;
    void randomize_map(void);

    public:
    Map(void);
    ~Map();
    void print_map_to_file(void);
    Field& get_field_by_cords(unsigned int, unsigned int);
    void update_player_on_map(Player*);
    bool is_field_accessible(uint32_t, uint32_t);
};

class World {

    public:
    Map* map;
    std::vector<Player*> players;
    World(void);
};