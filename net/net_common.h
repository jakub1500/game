#pragma once

#include <cstdint>
#include <vector>
#include <deque>
#include <mutex>

enum Move_Direction : uint32_t {
    Up=0,
    Down=1,
    Left=2,
    Right=3
};



// class Fifo {
//     std::deque<Msg> _fifo;
//     std::mutex lock;

//     public:
//     Fifo(void);
//     ~Fifo();
//     Msg get_element(void);
//     void push_element(Msg&&);
//     bool is_fifo_empty(void);
// };

