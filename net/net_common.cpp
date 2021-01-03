#include "net_common.h"
#include <iostream>
#include <cstring>

Fifo::Fifo(void) {

}
Fifo::~Fifo() {

}
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

Msg::Msg(void) {
    head.type = 0x41414141;
    update_size();
}

void Msg::update_size(void) {
    head.size = body.size() * sizeof(uint32_t) + sizeof(Header);
}

Msg& operator<<(Msg& in, const uint32_t part) {
    std::cout << "Adding " << part << std::endl;
    in.body.push_back(part);
    in.update_size();
    return in;
}

uint32_t Msg::get_size(void) {
    return head.size;
}

uint32_t Msg::get_type(void) {
    return head.type;
}
uint32_t* Msg::ptr_to_body(void){
    return body.data();
}

void msg_to_raw_parser(uint32_t* holder, Msg& msg_to_parse){
    *holder = msg_to_parse.get_type();
    *(holder+1) = msg_to_parse.get_size();
    std::memcpy(holder+2, msg_to_parse.ptr_to_body(), msg_to_parse.get_size()-8);
}