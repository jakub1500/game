#include "processers/processer_server.h"
#include <iostream>

using namespace std::chrono_literals;

Processer_Server::Processer_Server(std::vector<Session*>* vec_sessions, World& _world) : Processer_Base(vec_sessions), world(&_world) {}

void Processer_Server::move_player(Move_Direction direction) {
    Player* p = world->players.back();
    uint32_t pos_x = p->get_pos_x();
    uint32_t pos_y = p->get_pos_y();

    switch (direction){
        case Move_Direction::Up:
            pos_y--;
            break;
        case Move_Direction::Down:
            pos_y++;
            break;
        case Move_Direction::Left:
            pos_x--;
            break;
        case Move_Direction::Right:
            pos_x++;
            break;
        default:
            return;
    }
    if (world->map->is_field_accessible(pos_x, pos_y)) {
        p->set_pos_xy(pos_x, pos_y);
        world->map->update_player_on_map(p);
    }
}

void Processer_Server::process_msg(Msg& msg) {
    uint32_t type = msg.get_type();
    switch (type){
        case Msg_Type::MOVE_PLAYER:
            move_player((Move_Direction)*(msg.ptr_to_body()));
            break;
        default:
            return;
    }
}

int Processer_Server::loop(void) {
    while(1) {
        std::this_thread::sleep_for(1ms);
        if (vec_sessions->size() == 0) {
            continue;
        }
        for (Session* ses : *vec_sessions) {
            Msg msgx = world->make_board_for_player();
            ses->send_msg(std::move(msgx));
            std::cout << "sending new map\n";
            ses->run();
            if (!ses->new_message_available()) {
                continue;
            }
            Msg msg = ses->get_next_msg();
            std::cout << msg;
            process_msg(msg);
        }   
    }
    return 0;
}

Msg Processer_Server::create_msg(void) {Msg msg; return msg;}