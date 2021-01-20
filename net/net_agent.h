#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include "net_common.h"
#include "net_session.h"

class Net_Agent_Base {
    protected:
    uint16_t port;
    boost::asio::io_context context;
    boost::system::error_code error_code;
    boost::asio::ip::tcp::endpoint* endpoint;
    boost::asio::ip::tcp::socket socket;
    std::thread thr_context;

    Net_Agent_Base(void);
    Net_Agent_Base(uint16_t);
    ~Net_Agent_Base();

};

class Net_Agent_Client : public Net_Agent_Base {
    std::string ip;

    void connect(void);

    public:
    Session** session;
    Net_Agent_Client(Session**);
    Net_Agent_Client(Session**, std::string);
    Net_Agent_Client(Session**, uint16_t);
    Net_Agent_Client(Session**, std::string, uint16_t);
    ~Net_Agent_Client();

};

class Net_Agent_Server : public Net_Agent_Base {
    boost::asio::ip::tcp::acceptor* acceptor;

    void accept_connection(void);

    public:
    std::vector<Session*>* sessions;
    Net_Agent_Server(std::vector<Session*>*);
    Net_Agent_Server(std::vector<Session*>*, uint16_t);
    ~Net_Agent_Server();
};