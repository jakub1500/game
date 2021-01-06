#include "msg_processer.h"
#include "net_common.h"
#include "../world/world.h"
#include <iostream>

// void Msg_Processer_Base::process_msg(Msg& msg) {}
// Msg& Msg_Processer_base::create_msg(void) {Msg msg; return msg;}

Msg_Processer_Server::Msg_Processer_Server(World& _world) : world(&_world) {}

void Msg_Processer_Server::move_player(Move_Direction direction) {
    Player* p = world->players.back();
    uint32_t pos_x = p->get_pos_x();
    uint32_t pos_y = p->get_pos_y();

    switch (direction){
        case Move_Direction::Up:
            pos_x++;
            break;
        case Move_Direction::Down:
            pos_x--;
            break;
        case Move_Direction::Left:
            pos_y--;
            break;
        case Move_Direction::Right:
            pos_y++;
            break;
        default:
            return;
    }
    if (world->map->is_field_accessible(pos_x, pos_y)) {
        p->set_pos_xy(pos_x, pos_y);
        world->map->update_player_on_map(p);
    }
}

void Msg_Processer_Server::process_msg(Msg& msg) {
    uint32_t type = msg.get_type();
    switch (type){
        case Msg_type::MOVE_PLAYER:
            move_player((Move_Direction)*(msg.ptr_to_body()));
            break;
        default:
            return;
    }
}

Msg Msg_Processer_Server::create_msg(void) {Msg msg; return msg;}

Msg_Processer_Client::Msg_Processer_Client(Screen* _screen) : screen(_screen) {}
void Msg_Processer_Client::process_msg(Msg& msg) {
    uint32_t type = msg.get_type();
    
    switch (type) {
        case Msg_type::NEW_BOARD:

            break;
        default:
            std::cout << "cannot handle message\n";
            break;
    }
}
Msg Msg_Processer_Client::create_msg(void) {Msg msg; return msg;}