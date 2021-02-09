#include <iostream>
#include <string>
#include <vector>
#include "net/net_tcpip.h"
#include <thread>
#include "world/world.h"
#include "net/net_session.h"
#include "processers/processer_server.h"

int main(int agrc, char** argv) {
    std::cout << "start" << std::endl;

    std::vector<Session*>* sessions = new std::vector<Session*>();
    Net_TcpIp_Server ns(sessions);
    World world;

    Processer_Server proc_server(sessions, world);
    std::thread check_session_loop = std::thread([&]{proc_server.loop();});
    check_session_loop.join();
    if (sessions) {
        delete sessions;
    }
    return 0;
}