#include "screen.h"
#include "action.h"
#include "net/net_agent.h"
#include <chrono>
#include <iostream>
#include "net/msg_processer.h"

using namespace std::chrono_literals;

void manage_session_loop(Session* session, Msg_Processer_Client* processer) {
    std::this_thread::sleep_for(1000ms);
    while(1){
        std::this_thread::sleep_for(1000ms);
        if (!session) {
            continue;
        }
        Msg msx(Msg_type::MOVE_PLAYER);
        msx << 1;
        session->out_fifo.push_element(std::move(msx));
        session->run();
        if (!session->in_fifo.is_fifo_empty()) {
            std::cout << "there is new msg" << std::endl;
            Msg msg = session->in_fifo.get_element();
            std::cout << msg;
            processer->process_msg(msg);
        }

    }
}

int main() {
    Session* session = nullptr;
    Net_Agent_Client nc(&session);
    Screen s;
    Msg_Processer_Client msg_processer_client(&s);
    std::thread check_session_loop = std::thread(manage_session_loop, session, &msg_processer_client);
    KeyStrokeEngine kse(session);
    kse.run();

    check_session_loop.join();

    if(session) {
        delete session;
    }
    return 0;
}