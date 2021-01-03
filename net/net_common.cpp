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

Msg::Msg(void) : Msg(0x41414141){}

Msg::Msg(uint32_t _type) {
    head.type = _type;
    update_size();
}

void Msg::update_size(void) {
    head.size = body.size(); //* sizeof(uint32_t) + sizeof(Header);
}

Msg& operator<<(Msg& in, const uint32_t part) {
    // std::cout << "Adding " << part << std::endl;
    in.body.push_back(part);
    in.update_size();
    return in;
}

std::ostream& operator<< (std::ostream& stream, Msg& msg) {
    uint32_t type = msg.get_type();
    uint32_t size = msg.get_size();
    uint32_t* ptr_body = msg.ptr_to_body();
    stream << "Msg type: " << std::hex << type << " size: " << std::hex << size << std::endl;
    stream << "Msg body: " << std::endl;
    for (uint32_t* ptr = ptr_body; ptr != ptr_body+size; ptr++) {
        stream << std::hex << *ptr << std::endl;
    }
    return stream;
}

uint32_t Msg::get_size(void) const {
    return head.size;
}

uint32_t Msg::get_type(void) const {
    return head.type;
}
uint32_t* Msg::ptr_to_body(void) {
    return body.data();
}

void msg_to_raw_parser(uint32_t* holder, Msg& msg_to_parse){
    *holder = msg_to_parse.get_type();
    *(holder+1) = msg_to_parse.get_size();
    std::memcpy(holder+2, msg_to_parse.ptr_to_body(), msg_to_parse.get_size()*sizeof(uint32_t));
}

Msg raw_to_msg_parser(uint32_t* raw_msg) {
    uint32_t type = *(raw_msg);
    uint32_t size = *(raw_msg+1);
    Msg msg(type);
    for (int i = 2; i < size+2; i++) {
        msg << *(raw_msg + i);
    }
    return msg;
} 

