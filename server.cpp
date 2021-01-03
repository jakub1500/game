#include <iostream>
#include <string>
#include <vector>
#include "net/net_server.h"
#include <chrono>
#include <thread>
#include "world/world.h"
#include "net/msg_processer.h"
#include "net/net_session.h"
#include "net/msg_processer.h"


using namespace std::chrono_literals;

void manage_session_loop(NetServer* ns, Msg_Processer_Server* processor) {
    while(1){
        if (ns->sessions_holder.size() > 0) {
                Session* ses = ns->sessions_holder.back();
                // std::cout << "new session\n";
                ses->run(*processor);
                // std::cout << "is session alive? " << ses->is_alive() << std::endl;
            }
        std::this_thread::sleep_for(1ms);
    }
}

void temp_session_filler(NetServer* ns) {
    while(1) {
        if (ns->sessions_holder.size() > 0) {
            Session* ses = ns->sessions_holder.back();
            Msg msg;
            msg << 0x42424242 << 0x43434343 << 0x44444400;
            ses->out_fifo.push_element(std::move(msg));
        }
        std::this_thread::sleep_for(2000ms);
    }
}

int main(int agrc, char** argv) {
    std::cout << "start" << std::endl;

    NetServer ns;
    World world;
    world.map->print_map_to_file();

    Msg_Processer_Server msg_proc_server(world);

    std::thread check_session_loop = std::thread(manage_session_loop, &ns, &msg_proc_server);
    // std::thread filler = std::thread(temp_session_filler, &ns);

    // filler.join();
    check_session_loop.join();
    return 0;
}