#include <iostream>
#include <string>
#include <vector>
#include "net/net_server.h"
#include <chrono>


using namespace std::chrono_literals;


int main(int agrc, char** argv) {
    std::cout << "start" << std::endl;

    NetServer ns;
    std::thread filler = std::thread([&]{
        while(1) {
            if (ns.sessions_holder.size() > 0) {
                Session* ses = ns.sessions_holder.back();
                Msg msg;
                msg << 0x42424242 << 0x43434343 << 0x44444400;
                ses->out_fifo.push_element(std::move(msg));
            }
            std::this_thread::sleep_for(2000ms);
        }
    });

    while(1){
    }
    filler.join();
    return 0;
}