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

Msg raw_to_msg_parser(uint32_t* raw_msg, std::size_t length) {
    uint32_t type = *(raw_msg);
    uint32_t size = *(raw_msg+1);
    Msg msg(type);
    for (int i = 2; i < size+2; i++) {
        msg << *(raw_msg + i);
    }
    return msg;
} 

Session::Session(boost::asio::ip::tcp::socket _socket)
        : socket(std::move(_socket)), kill_session_trigger(false), readyToRead(true), read_offset(0){
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

    boost::asio::async_write(socket, boost::asio::buffer(out, raw_size),
        [&](boost::system::error_code ec, std::size_t) {
        if (!ec) {
        } else {
            std::cout << "there is a problem " << ec.message() << " | " << ec.value() << std::endl;
        }

    });
    delete out;
}

void Session::read(void) {
    if (!readyToRead) {
        return;
    }
    readyToRead = false;

    socket.async_read_some(boost::asio::buffer(data+read_offset, SESSION_READ_BUFFER_SIZE-read_offset),
        [&](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            process_incomming_data(length);
        }
        readyToRead = true;

    });
}

void Session::process_incomming_data(std::size_t length) {
    while(1) {
        // wait for full header
        if (read_offset + length < 8) {
            read_offset += length;
            break;
        }
        // now we know that we have full header
        uint32_t msg_size = *((uint32_t*)(data+4));
        unsigned int full_message_size_in_bytes = msg_size*4+8;

        // wait for full message
        if ((read_offset + length) < (full_message_size_in_bytes)) {
            read_offset += length;
            break;
        }

        // when there is full message in buffer we can process it
        Msg msg = raw_to_msg_parser((uint32_t*)data, length);
        in_fifo.push_element(std::move(msg));

        // if there are bytes left from next message we have to write them at the beggining of the buffer
        unsigned int bytes_from_next_msg;
        bytes_from_next_msg = (read_offset + length) - (full_message_size_in_bytes);

        std::memcpy(data, data+(full_message_size_in_bytes), bytes_from_next_msg);
        std::memset(data+bytes_from_next_msg, '\0', SESSION_READ_BUFFER_SIZE-bytes_from_next_msg);
        read_offset = bytes_from_next_msg;
        length = 0;
    }
    
};

void Session::kill_session(void) {
    kill_session_trigger = true;
}

bool Session::is_alive(void) {
    return socket.is_open();
}