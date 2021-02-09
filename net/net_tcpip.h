#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include "net_session.h"

class Net_TcpIp_Base {
    protected:
    uint16_t port;
    boost::asio::io_context context;
    boost::system::error_code error_code;
    boost::asio::ip::tcp::endpoint* endpoint;
    boost::asio::ip::tcp::socket socket;
    std::thread thr_context;

    Net_TcpIp_Base(void);
    Net_TcpIp_Base(uint16_t);
    virtual ~Net_TcpIp_Base();

};

class Net_TcpIp_Client : public Net_TcpIp_Base {
    std::string ip;

    void connect(void);

    public:
    std::vector<Session*>* sessions;
    Net_TcpIp_Client(std::vector<Session*>*);
    Net_TcpIp_Client(std::vector<Session*>*, std::string);
    Net_TcpIp_Client(std::vector<Session*>*, uint16_t);
    Net_TcpIp_Client(std::vector<Session*>*, std::string, uint16_t);
    ~Net_TcpIp_Client();

};

class Net_TcpIp_Server : public Net_TcpIp_Base {
    boost::asio::ip::tcp::acceptor* acceptor;

    void accept_connection(void);

    public:
    std::vector<Session*>* sessions;
    Net_TcpIp_Server(std::vector<Session*>*);
    Net_TcpIp_Server(std::vector<Session*>*, uint16_t);
    ~Net_TcpIp_Server();
};