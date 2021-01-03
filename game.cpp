#include "screen.h"
#include "action.h"
#include "net/net_client.h"
#include <chrono>
#include <iostream>
#include "net/msg_processer.h"

using namespace std::chrono_literals;

void manage_session_loop(NetClient* nc, Msg_Processer_Client* processer) {
    std::this_thread::sleep_for(1000ms);
    while(1){
        Session* ses = nc->sessions_holder;
        if (!ses) {
            continue;
        }
        std::cout << "there is new session" << std::endl;
        ses->run(*processer);

        std::this_thread::sleep_for(1000ms);
    }
}

int main() {
    NetClient nc;
    Screen s;
    Msg_Processer_Client msg_processer_client;
    std::thread check_session_loop = std::thread(manage_session_loop, &nc, &msg_processer_client);

    KeyStrokeEngine kse(nc.sessions_holder);
    kse.run();

    check_session_loop.join();

    return 0;
}