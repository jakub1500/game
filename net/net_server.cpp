#include "net_server.h"
#include <string>
#include <iostream>


NetServer::NetServer(void) : NetServer(DEFAULT_PORT) {
}

NetServer::NetServer(uint16_t _port)
        : port(_port),
          endpoint(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
          acceptor(boost::asio::ip::tcp::acceptor(context, endpoint)),
          socket(context) {
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> p = boost::asio::make_work_guard(context);
    thr_context = std::thread([&]() {context.run();});
    accept_connection();
}

NetServer::~NetServer() {
    context.stop();
    thr_context.join();
}

void NetServer::accept_connection(void) {

    // boost::asio::ip::tcp::socket socket(context);
    acceptor.async_accept(socket, [&](boost::system::error_code ec)
        {
            if (!ec)
            {
                Session* session = new Session(std::move(socket));
                // session->run();
                sessions_holder.push_back(session);
                // delete session;

                // std::make_shared<session>(std::move(socket_))->start();
            }
        
            accept_connection();
        });
}