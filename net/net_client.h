#include <cstdint>
#include <string>
#include <boost/asio.hpp>
#include "net_common.h"
#include "net_session.h"

class NetClient {
    std::string ip;
    uint16_t port;
    boost::asio::io_context context;
    boost::system::error_code error_code;
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::ip::tcp::socket socket;
    std::thread thr_context;

    void connect(void);

    public:
    Session* sessions_holder;
    NetClient(void);
    NetClient(std::string);
    NetClient(uint16_t);
    NetClient(std::string, uint16_t);
    ~NetClient();

};