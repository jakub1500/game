#include "protocol_msg.h"
#include "cstring"

uint32_t Net_Byte_Order_Conv::convert_to_network(uint32_t in_val) {
    uint32_t ret_val = 0;
    uint8_t* ptr_ret_val = (uint8_t*)&ret_val;

    for (int i=0; i<sizeof(in_val); i++) {
        uint8_t temp_val = in_val << (sizeof(in_val)-1-i)*8 >> (sizeof(in_val)-1) * 8;
        ptr_ret_val[sizeof(in_val)-1-i] = temp_val;
    }

    return ret_val;
}
uint32_t Net_Byte_Order_Conv::convert_from_network(uint32_t in_val) {
    uint32_t ret_val = 0;
    uint8_t* ptr_in_val = (uint8_t*)&in_val;

    for (int i=0; i<sizeof(in_val); i++) {
        uint32_t temp_val = ((uint32_t)ptr_in_val[sizeof(in_val)-1-i]) << i * 8;
        ret_val |= temp_val;
    }

    return ret_val;
}

Msg::Msg(void) : Msg(0x41414141){}

Msg::Msg(uint32_t _type) {
    head.type = _type;
    update_size();
}

void Msg::update_size(void) {
    head.size = body.size();
}

Msg& operator<<(Msg& in, const uint32_t part) {
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

std::size_t Msg::get_size_in_bytes(void) const {
    return get_size()*sizeof(uint32_t)+sizeof(Header);
}

void Msg::msg_to_raw_parser(uint8_t* buffer, Msg& msg_to_parse){
    uint32_t* holder = (uint32_t*)buffer;
    uint32_t size = msg_to_parse.get_size();
    uint32_t* ptr_to_body = msg_to_parse.ptr_to_body();

    *holder = Net_Byte_Order_Conv::convert_to_network(msg_to_parse.get_type());
    *(++holder) = Net_Byte_Order_Conv::convert_to_network(size);

    for (int i = 0; i < size; i++) {
        *(++holder) = Net_Byte_Order_Conv::convert_to_network(*(ptr_to_body+i));
    }
}

Msg Msg::raw_to_msg_parser(uint8_t* raw_msg) {
    uint32_t type = Net_Byte_Order_Conv::convert_from_network(*(uint32_t*)(raw_msg));
    uint32_t size = Net_Byte_Order_Conv::convert_from_network(*(((uint32_t*)raw_msg)+1));
    Msg msg(type);
    for (int i = 2; i < size+2; i++) {
        msg << Net_Byte_Order_Conv::convert_from_network(*(((uint32_t*)raw_msg) + i));
    }
    return msg;
}

bool Msg::check_if_full_msg_obtained(uint8_t* data, std::size_t read_bytes) {
    // wait for full header
    if (read_bytes < sizeof(Header)) {
        return false;
    }

    // now we know that we have full header
    uint32_t msg_size = Net_Byte_Order_Conv::convert_from_network(*(uint32_t*)(data+sizeof(Header::type)));
    std::size_t full_message_size_in_bytes = msg_size*sizeof(uint32_t)+sizeof(Header);
    if (read_bytes < full_message_size_in_bytes) {
        return false;
    }

    return true;
}