#pragma once

#include "net_common.h"
#include <boost/asio.hpp>
#include <mutex>
#include "protocol/protocol_msg.h"

#define SESSION_READ_BUFFER_SIZE 8196

class Session {
    boost::asio::ip::tcp::socket socket;
    uint8_t data[SESSION_READ_BUFFER_SIZE];
    void write(Msg&);
    void read(void);
    unsigned int read_offset;
    std::mutex dispose_next_read_locker;
    bool dispose_next_read;
    void process_incomming_data(std::size_t);

    public:
    Fifo in_fifo;
    Fifo out_fifo;
    Session(boost::asio::ip::tcp::socket);
    void run(void);

};
