#include "processers/processer_game.h"
#include <iostream>

Processer_Client::Processer_Client(std::vector<Session*>* vec_sessions, Screen* _screen) : Processer_Base(vec_sessions), screen(_screen) {}
void Processer_Client::process_msg(Msg& msg) {
    uint32_t type = msg.get_type();
    
    switch (type) {
        case Msg_Type::NEW_BOARD:
                screen->show_board(msg);
            break;
        default:
            std::cout << "cannot handle message due to msg type "<< std::hex << type << "\n";
            break;
    }
}

Msg Processer_Client::create_msg(void) {Msg msg; return msg;}

int Processer_Client::loop(void) {
    while(1) {
       for (Session* ses : *vec_sessions) {     
            ses->run();
            if (ses->new_message_available()) {
                Msg msg = ses->get_next_msg();
                process_msg(msg);
            }
        }   
    }
    return 0;
}