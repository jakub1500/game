#include "net_client.h"
#include <iostream>

#include "net_session.h"
#include "msg_processer.h"

NetClient::NetClient(void) : NetClient(std::string(DEFAULT_IP), DEFAULT_PORT) {}
NetClient::NetClient(std::string _ip) : NetClient(_ip, DEFAULT_PORT){}
NetClient::NetClient(uint16_t _port) : NetClient(std::string(DEFAULT_IP), _port) {}

NetClient::NetClient(std::string _ip, uint16_t _port) :
        ip(_ip),
        port(_port),
        endpoint(boost::asio::ip::make_address(ip, error_code), port), 
        socket(context),
        sessions_holder(nullptr){
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> p = boost::asio::make_work_guard(context);
    thr_context = std::thread([&]() {context.run();});
    connect();
}

NetClient::~NetClient() {
    context.stop();
    thr_context.join();
}

void NetClient::connect(void) {
    socket.connect(endpoint, error_code);
    if (!error_code) {
        std::cout << "Connected" << std::endl;
        sessions_holder = new Session(std::move(socket));
    } else {
        std::cout << "Failed to connect to address:\n" << error_code.message() << std::endl;
    }
}