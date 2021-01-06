#include "net_agent.h"

#include <iostream>

#include "net_session.h"

Net_Agent_Base::Net_Agent_Base(void) : Net_Agent_Base(DEFAULT_PORT) {}
Net_Agent_Base::Net_Agent_Base(uint16_t _port) :
        port(_port),
        socket(context),
        endpoint(nullptr) {

}
Net_Agent_Base::~Net_Agent_Base(){}

Net_Agent_Client::Net_Agent_Client(Session** _session) : Net_Agent_Client(_session, std::string(DEFAULT_IP), DEFAULT_PORT) {}
Net_Agent_Client::Net_Agent_Client(Session** _session, std::string _ip) : Net_Agent_Client(_session, _ip, DEFAULT_PORT){}
Net_Agent_Client::Net_Agent_Client(Session** _session, uint16_t _port) : Net_Agent_Client(_session, std::string(DEFAULT_IP), _port) {}

Net_Agent_Client::Net_Agent_Client(Session** _session, std::string _ip, uint16_t port) :
        ip(_ip),
        session(_session) {
    endpoint = new boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(ip, error_code), port);
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> p = boost::asio::make_work_guard(context);
    thr_context = std::thread([&]() {context.run();});
    connect();
}

Net_Agent_Client::~Net_Agent_Client() {
    context.stop();
    thr_context.join();
}

void Net_Agent_Client::connect(void) {
    socket.connect(*endpoint, error_code);
    if (!error_code) {
        std::cout << "Connected" << std::endl;
        // Session* _session = new Session(std::move(socket));
        // session = &_session;
        *session = new Session(std::move(socket));
        
    } else {
        std::cout << "Failed to connect to address:\n" << error_code.message() << std::endl;
    }
}
Net_Agent_Server::Net_Agent_Server(std::vector<Session*>* _sessions) : Net_Agent_Server(_sessions, DEFAULT_PORT) {
}

Net_Agent_Server::Net_Agent_Server(std::vector<Session*>* _sessions, uint16_t _port) :
        acceptor(nullptr),
        sessions(_sessions) {
    endpoint = new boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
    acceptor = new boost::asio::ip::tcp::acceptor(context, *endpoint);
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> p = boost::asio::make_work_guard(context);
    thr_context = std::thread([&]() {context.run();});
    accept_connection();
}

Net_Agent_Server::~Net_Agent_Server() {
    context.stop();
    thr_context.join();
}

void Net_Agent_Server::accept_connection(void) {
    acceptor->async_accept(socket, [&](boost::system::error_code ec)
        {
            if (!ec)
            {
                std::cout << std::hex << sessions << std::endl;
                Session* session = new Session(std::move(socket));
                std::cout << "new connection\n";
                // session->run();
                sessions->push_back(session);
                // delete session;

                // std::make_shared<session>(std::move(socket_))->start();
            }
        
            accept_connection();
        });
}