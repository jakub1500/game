#pragma once

#include "net_common.h"
#include <boost/asio.hpp>
#include "msg_processer.h"

#define SESSION_READ_BUFFER_SIZE 8196

class Session {
    boost::asio::ip::tcp::socket socket;
    std::string text;
    char data[SESSION_READ_BUFFER_SIZE];
    void write(Msg&);
    void read(void);
    bool kill_session_trigger;
    bool readyToRead;
    unsigned int read_offset;
    void process_incomming_data(std::size_t);

    public:
    Fifo in_fifo;
    Fifo out_fifo;
    Session(boost::asio::ip::tcp::socket);
    void run(void);
    void kill_session(void);
    bool is_alive(void);

};

Msg raw_to_msg_parser(uint32_t*, std::size_t);
uint32_t* msg_to_raw_parser(Msg&);