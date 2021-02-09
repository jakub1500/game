#include "screen.h"
#include "action.h"
#include "net/net_tcpip.h"
#include <iostream>
#include "processers/processer_game.h"
#include <vector>
#include "net/net_session.h"

int main() {
    std::vector<Session*>* sessions = new std::vector<Session*>();
    Net_TcpIp_Client nc(sessions);
    Screen s;
    Processer_Client processer_client(sessions, &s);
    std::thread check_session_loop = std::thread([&]{processer_client.loop();});
    Session* ses = sessions->back();
    KeyStrokeEngine kse(ses);
    kse.run();

    check_session_loop.join();

    return 0;
}