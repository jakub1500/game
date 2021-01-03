#include "net_session.h"
#include "msg_processer.h"
#include "net_common.h"
#include <iostream>


Session::Session(boost::asio::ip::tcp::socket _socket)
        : socket(std::move(_socket)), kill_session_trigger(false) {
    // std::memcpy(data, "witaj wedrowcze\n", 16);
    
}

void Session::run(Msg_Processer_Base& msg_proc) {
    if (!out_fifo.is_fifo_empty()) {
            write();
    }
    if (!in_fifo.is_fifo_empty()) {
        Msg msg = in_fifo.get_element();
        std::cout << msg;
        msg_proc.process_msg(msg);
    }
    read();
    // write();
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
            out = new uint32_t[size+2];
            msg_to_raw_parser(out, outMsg);
    }

    boost::asio::async_write(socket, boost::asio::buffer(out, size*sizeof(uint32_t)+2*sizeof(uint32_t)),
        [&](boost::system::error_code ec, std::size_t) {
        if (!ec) {
            // write();
        }
        if (ec) {
            std::cout << "there is a problem " << ec.message() << " | " << ec.value() << std::endl;
        }
        delete out;

    });
}

void Session::read(void) {
    std::memset(data, '\0', 1024);
    socket.async_read_some(boost::asio::buffer(data, 1024),
        [&](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            Msg msg = raw_to_msg_parser((uint32_t*)data);
            in_fifo.push_element(std::move(msg));
            //write();
        }

    });
}

void Session::kill_session(void) {
    kill_session_trigger = true;
}

bool Session::is_alive(void) {
    return socket.is_open();
}