#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include <string>
#include "net_common.h"

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
    void run(void);
    void kill_session(void);

};

class NetServer {
    uint16_t port;
    boost::asio::io_context context;
    boost::system::error_code error_code;
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::acceptor acceptor;
    std::thread thr_context;

    void accept_connection(void);

    public:
    std::vector<Session*> sessions_holder;
    NetServer(void);
    NetServer(uint16_t port);
    ~NetServer();
};