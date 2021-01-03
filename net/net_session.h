#include "net_common.h"
#include <boost/asio.hpp>
#include "msg_processer.h"

#pragma once

class Session {
    boost::asio::ip::tcp::socket socket;
    std::string text;
    char data[1024];
    void write(void);
    void read(void);
    bool kill_session_trigger;

    public:
    Fifo in_fifo;
    Fifo out_fifo;
    Session(boost::asio::ip::tcp::socket);
    void run(Msg_Processer_Base&);
    void kill_session(void);
    bool is_alive(void);

};