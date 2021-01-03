#include <cstdint>
#include <vector>
#include <deque>
#include <mutex>

class Msg;

class Fifo {
    std::deque<Msg> _fifo;
    std::mutex lock;

    public:
    Fifo(void);
    ~Fifo();
    Msg get_element(void);
    void push_element(Msg&&);
    bool is_fifo_empty(void);
};

struct Header {
    uint32_t type;
    uint32_t size;
};
class Msg {
    Header head;
    std::vector<uint32_t> body;
    void update_size(void);

    public:
    Msg();
    friend Msg& operator<<(Msg&, const uint32_t);
    uint32_t get_size(void);
    uint32_t get_type(void);
    uint32_t* ptr_to_body(void);
};

Msg raw_to_msg_parser();
void msg_to_raw_parser(uint32_t*, Msg&);