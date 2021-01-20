#pragma once

#include <cstdint>
#include <vector>
#include <deque>
#include <mutex>

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 4444

enum Move_Direction : uint32_t {
    Up=0,
    Down=1,
    Left=2,
    Right=3
};

enum Msg_type : uint32_t {
    MOVE_PLAYER = 44,
    NEW_BOARD
};

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
    Msg(void);
    Msg(uint32_t);
    friend Msg& operator<<(Msg&, const uint32_t);
    friend std::ostream& operator<< (std::ostream&, Msg&);
    uint32_t get_size(void) const;
    uint32_t get_type(void) const;
    uint32_t* ptr_to_body(void);
};
