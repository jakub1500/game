#include <iostream>
#include <string>
#include <vector>
#include "net/net_agent.h"
#include <chrono>
#include <thread>
#include "world/world.h"
#include "net/msg_processer.h"
#include "net/net_session.h"
#include "net/msg_processer.h"


using namespace std::chrono_literals;

void manage_session_loop(std::vector<Session*>* sessions, Msg_Processer_Server* processer) {
    while(1){
        std::this_thread::sleep_for(200ms);
        if (!sessions || sessions->size() == 0) {
            continue;
        }
        for (Session* ses : *sessions) {
            Msg msgx = processer->world->make_board_for_player();
            ses->out_fifo.push_element(std::move(msgx));
            std::cout << "sending new map\n";
            ses->run();
            if (ses->in_fifo.is_fifo_empty()) {
                continue;
            }
            Msg msg = ses->in_fifo.get_element();
            std::cout << msg;
            processer->process_msg(msg);
        }   
    }
}

int main(int agrc, char** argv) {
    std::cout << "start" << std::endl;

    std::vector<Session*>* sessions = new std::vector<Session*>();
    Net_Agent_Server ns(sessions);
    World world;
    world.map->print_map_to_file();

    Msg_Processer_Server msg_proc_server(world);

    std::thread check_session_loop = std::thread(manage_session_loop, sessions, &msg_proc_server);
    check_session_loop.join();
    if (sessions) {
        delete sessions;
    }
    return 0;
}