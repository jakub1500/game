#pragma once

#include <vector>
#include <deque>
#include <mutex>

template<typename itemType>
class Fifo {
    std::deque<itemType> _fifo;
    std::mutex lock;

    public:
    Fifo(void);
    ~Fifo();
    itemType get_element(void);
    void push_element(itemType&&);
    bool is_fifo_empty(void);
};
