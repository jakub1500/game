#include "net_server.h"
#include <string>
#include <iostream>

Session::Session(boost::asio::ip::tcp::socket _socket)
        : socket(std::move(_socket)), kill_session_trigger(false) {
    std::memcpy(data, "witaj wedrowcze\n", 16);
    
}

void Session::run(void) {
    write();
    // while(!kill_session_trigger) {
    //     if (!out_fifo.is_fifo_empty()) {
    //         write();
    //     }
    // }
}

void Session::write(void) {
    uint32_t* out;
    uint32_t size = 0;
    if (!out_fifo.is_fifo_empty()) {
            Msg outMsg = out_fifo.get_element();
            size = outMsg.get_size();
            out = new uint32_t[size/sizeof(uint32_t)];
            msg_to_raw_parser(out, outMsg);
    }
    // if (size != 0) {
    //     std::cout << "sending " << size << " bytes\n";
    //     for (int i = 0; i < size; i++) {
    //         Msg* x = &outMsg;
    //         char* ptr = ((char*)x)+i;

    //         std::cout << *ptr;
    //     }
    //     std::cout << std::endl;
    // }
        
    // Msg outMsg = out_fifo.get_element();
    boost::asio::async_write(socket, boost::asio::buffer(out, size),
        [&](boost::system::error_code ec, std::size_t) {
        if (!ec) {
            write();
        }

    });
}

void Session::read(void) {
    std::memset(data, '\0', 1024);
    socket.async_read_some(boost::asio::buffer(data, 1024),
        [&](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            data[length] = '\n';
            //write();
        }

    });
}

void Session::kill_session(void) {
    kill_session_trigger = true;
}

NetServer::NetServer(void) : NetServer(4444) {
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
                session->run();
                sessions_holder.push_back(session);
                // delete session;

                // std::make_shared<session>(std::move(socket_))->start();
            }
        
            accept_connection();
        });
}