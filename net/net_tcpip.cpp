#include "net_tcpip.h"

#include <iostream>


#ifndef IP
    #error "IP undefined!"
#endif
#ifndef PORT
    #error "PORT undefined"
#endif


Net_TcpIp_Base::Net_TcpIp_Base(void) : Net_TcpIp_Base(PORT) {}
Net_TcpIp_Base::Net_TcpIp_Base(uint16_t _port) :
        port(_port),
        socket(context),
        endpoint(nullptr) {

}
Net_TcpIp_Base::~Net_TcpIp_Base(){}

Net_TcpIp_Client::Net_TcpIp_Client(std::vector<Session*>* _sessions) : Net_TcpIp_Client(_sessions, std::string(IP), PORT) {}
Net_TcpIp_Client::Net_TcpIp_Client(std::vector<Session*>* _sessions, std::string _ip) : Net_TcpIp_Client(_sessions, _ip, PORT){}
Net_TcpIp_Client::Net_TcpIp_Client(std::vector<Session*>* _sessions, uint16_t _port) : Net_TcpIp_Client(_sessions, std::string(IP), _port) {}

Net_TcpIp_Client::Net_TcpIp_Client(std::vector<Session*>* _sessions, std::string _ip, uint16_t port) :
        ip(_ip),
        sessions(_sessions) {
    endpoint = new boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(ip, error_code), port);
    thr_context = std::thread([&]() {
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> p = boost::asio::make_work_guard(context);
        context.run();
    });
    connect();
}

Net_TcpIp_Client::~Net_TcpIp_Client() {
    context.stop();
    thr_context.join();
}

void Net_TcpIp_Client::connect(void) {
    socket.connect(*endpoint, error_code);
    if (!error_code) {
        std::cout << "Connected" << std::endl;
        Session* session = new Session(std::move(socket));
        sessions->push_back(session);
        
    } else {
        std::cout << "Failed to connect to address:\n" << error_code.message() << std::endl;
    }
}
Net_TcpIp_Server::Net_TcpIp_Server(std::vector<Session*>* _sessions) : Net_TcpIp_Server(_sessions, PORT) {
}

Net_TcpIp_Server::Net_TcpIp_Server(std::vector<Session*>* _sessions, uint16_t _port) :
        acceptor(nullptr),
        sessions(_sessions) {
    endpoint = new boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
    acceptor = new boost::asio::ip::tcp::acceptor(context, *endpoint);
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> p = boost::asio::make_work_guard(context);
    thr_context = std::thread([&]() {context.run();});
    accept_connection();
}

Net_TcpIp_Server::~Net_TcpIp_Server() {
    context.stop();
    thr_context.join();
}

void Net_TcpIp_Server::accept_connection(void) {
    std::cout << "accept connection called\n";
    acceptor->async_accept(socket, [&](boost::system::error_code ec)
        {
            if (!ec)
            {
                std::cout << std::hex << sessions << std::endl;
                Session* session = new Session(std::move(socket));
                std::cout << "new connection\n";
                sessions->push_back(session);
            }
        
            accept_connection();
        });
}