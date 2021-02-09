#include "net_session.h"
#include "net_common.h"
#include <iostream>

Session::Session(boost::asio::ip::tcp::socket _socket)
        : socket(std::move(_socket)), read_offset(0), dispose_next_read(true) {
    std::memset(data, '\0', SESSION_READ_BUFFER_SIZE);
    
}

void Session::run(void) {
    if (!out_fifo.is_fifo_empty()) {
            Msg outMsg = out_fifo.get_element();
            write(outMsg);
    }

    std::lock_guard<std::mutex> l(dispose_next_read_locker);
    if (dispose_next_read) {
        read();
        dispose_next_read = false;
    }
}

void Session::write(Msg& outMsg) {
    uint32_t* out = Msg::msg_to_raw_parser(outMsg);
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
    socket.async_read_some(boost::asio::buffer(data+read_offset, SESSION_READ_BUFFER_SIZE-read_offset),
        [&](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            process_incomming_data(length);
        }
        std::lock_guard<std::mutex> l(dispose_next_read_locker);
        dispose_next_read = true;
    });
}

void Session::process_incomming_data(std::size_t length) {
    read_offset += length;
    while(1) {
        if (!Msg::check_if_full_msg_obtained(data, read_offset)) {
            break;
        }

        // when there is full message in buffer we can process it
        Msg msg = Msg::raw_to_msg_parser((uint32_t*)data);
        std::size_t bytes_msg_size = msg.get_size_in_bytes();

        in_fifo.push_element(std::move(msg));

        // if there are bytes left from next message we have to write them at the beggining of the buffer
        std::size_t bytes_from_next_msg = read_offset - bytes_msg_size;

        std::memcpy(data, data+(bytes_msg_size), bytes_from_next_msg);
        std::memset(data+bytes_from_next_msg, '\0', SESSION_READ_BUFFER_SIZE-bytes_from_next_msg);
        read_offset = bytes_from_next_msg;
        length = 0;
    }
    
};
