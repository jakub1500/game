#include "world.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

Map::Map(void) {
    field_map = new Field*[map_x_size];
    for (unsigned int i=0; i < map_x_size; i++) {
        *(field_map+i) = new Field[map_y_size];
    }
    randomize_map();
}

Map::~Map() {
    for (int i=0; i < map_x_size; i++) {
        delete *(field_map+i);
    }
    delete field_map;
}
void Map::print_map_to_file(void) {
    std::ofstream map_file;
    Field t_field;

    map_file.open(map_file_name);
    for (unsigned int i = 0; i < map_y_size; i++) {
        for (unsigned int j = 0; j < map_x_size; j++) {
            if ((i == 0 && (j == 0 || j == map_x_size - 1)) ||
                ((i == map_y_size + 1 && (j == 0 || j == map_x_size - 1)))) {
                map_file << "#";
            } else if (i == 0 || i == map_y_size - 1) {
                map_file << "*";
            } else if (j == 0 || j == map_x_size - 1) {
                map_file << "*";
            } else {
                t_field = get_field_by_cords(j, i);
                map_file << std::string(1, t_field.get_image());
            }
        }
        map_file << "\n";
    }
    map_file.close();
}

Field& Map::get_field_by_cords(unsigned int x, unsigned int y) {
    Field* ptr = ((*(field_map+x) + y));
    return *ptr;
}

void Map::randomize_map(void) {
    int rand_x, rand_y;
    for (int i = 0; i < (randomize_perc*map_x_size*map_y_size/100); i++) {
        rand_x = rand() % map_x_size;
        rand_y = rand() % map_y_size;
        Field& rand_field = get_field_by_cords(rand_x, rand_y);
        rand_field.set_image('$');
    }
}

void Map::update_player_on_map(Player* player){
    uint32_t x = player->get_pos_x();
    uint32_t y = player->get_pos_y();
    Field field = get_field_by_cords(x, y);
    field.set_image(player->get_layout());
}

bool Map::is_field_accessible(uint32_t x, uint32_t y){
    Field field = get_field_by_cords(x, y);
    return field.get_image() == ' ';
}

World::World(void) : map(new Map) {
    players.push_back(new Player);
}

World::~World() {
    for (Player* player : players) {
        delete player;
    }
}

int lay_counter = 0;
bool direction = true;

Msg World::make_board_for_player(void) {
    Player* player = players.back();
    uint32_t pos_x = player->get_pos_x();
    uint32_t pos_y = player->get_pos_y();
    char layout = player->get_layout();

    uint32_t board_x_size = 41;
    uint32_t board_y_size = 21;
    uint32_t board_size = board_x_size*board_y_size;

    char board[board_size];

    Field field;
    int i = 0;
    for (int y = pos_y-(board_y_size/2); y <= pos_y+(board_y_size/2); y++) {
        for (int x = pos_x-(board_x_size/2); x <= pos_x+(board_x_size/2); x++) {
            field = map->get_field_by_cords(x,y);
            board[i] = field.get_image();
            i++;
        }
    }
    layout += lay_counter;
    if (direction) {
        lay_counter++;
        if (lay_counter > 5) {
            direction = false;
        }
    } else {
        lay_counter--;
        if (lay_counter < 0) {
            direction = true;
        }
    }
    board[i/2] = layout;

    Msg msg(Msg_Type::NEW_BOARD);
    int fulluint32 = (board_size)/sizeof(uint32_t);
    int modulorest = (board_size)%sizeof(uint32_t);

    for (int i = 0; i< fulluint32; i++) {
        uint32_t new_val=0;
        for (int j = 0; j<sizeof(uint32_t); j++) {
            *(((char*)(&new_val))+j) = board[i*sizeof(uint32_t)+j];
        }
        msg << new_val;
    }
    uint32_t last=0;
    for (int i = 0; i < modulorest; i++) {
        *(((char*)(&last))+i) = board[board_size-modulorest+i];
    }
    if (modulorest > 0) {
        msg << last;
    } 

    return msg; 

}