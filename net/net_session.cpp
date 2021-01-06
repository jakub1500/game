#include "net_session.h"
#include "msg_processer.h"
#include "net_common.h"
#include <iostream>

uint32_t* msg_to_raw_parser(Msg& msg_to_parse){
    uint32_t* holder;
    uint32_t size = msg_to_parse.get_size();
    holder = new uint32_t[size+2];

    *holder = msg_to_parse.get_type();
    *(holder+1) = size;
    std::memcpy(holder+2, msg_to_parse.ptr_to_body(), size*sizeof(uint32_t));
    return holder;
}

Msg raw_to_msg_parser(uint32_t* raw_msg) {
    uint32_t type = *(raw_msg);
    uint32_t size = *(raw_msg+1);
    Msg msg(type);
    for (int i = 2; i < size+2; i++) {
        msg << *(raw_msg + i);
    }
    return msg;
} 

Session::Session(boost::asio::ip::tcp::socket _socket)
        : socket(std::move(_socket)), kill_session_trigger(false) {
    std::memset(data, '\0', SESSION_READ_BUFFER_SIZE);
    
}

void Session::run(void) {
    if (!out_fifo.is_fifo_empty()) {
            Msg outMsg = out_fifo.get_element();
            write(outMsg);
    }
    read();
}

void Session::write(Msg& outMsg) {
    uint32_t* out = msg_to_raw_parser(outMsg);
    uint32_t raw_size = outMsg.get_size()*sizeof(uint32_t)+2*sizeof(uint32_t);
    std::cout << "trying to send" << std::endl;

    boost::asio::async_write(socket, boost::asio::buffer(out, raw_size),
        [&](boost::system::error_code ec, std::size_t) {
        // delete out;
        if (!ec) {
            // write();
        }
        if (ec) {
            std::cout << "there is a problem " << ec.message() << " | " << ec.value() << std::endl;
        }
        std::cout << "new msg was sent" << std::endl;

    });
    delete out;
}

void Session::read(void) {
            std::cout << "new trying to read\n";

    socket.async_read_some(boost::asio::buffer(data, SESSION_READ_BUFFER_SIZE),
        [&](boost::system::error_code ec, std::size_t length) {
            std::cout << "new msg12412412412\n";
        if (!ec) {
            Msg msg = raw_to_msg_parser((uint32_t*)data);
            in_fifo.push_element(std::move(msg));
            //write();
        }
        (length < SESSION_READ_BUFFER_SIZE)? std::memset(data, '\0', length) : std::memset(data, '\0', SESSION_READ_BUFFER_SIZE);

    });
}

void Session::kill_session(void) {
    kill_session_trigger = true;
}

bool Session::is_alive(void) {
    return socket.is_open();
}