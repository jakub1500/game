#include "net_common.h"
#include <iostream>
#include <cstring>

Fifo::Fifo(void) {}
Fifo::~Fifo() {}
Msg Fifo::get_element(void){
    std::lock_guard<std::mutex> l(lock);
    Msg retMsg = _fifo.front();
    _fifo.pop_front();
    return retMsg;
}
void Fifo::push_element(Msg&& msg){
    std::lock_guard<std::mutex> l(lock);
    _fifo.push_back(msg);
}
bool Fifo::is_fifo_empty(void){
    return _fifo.size() == 0;
}
