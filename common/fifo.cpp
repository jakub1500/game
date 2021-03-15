#include "fifo.h"

template<typename itemType>
Fifo<itemType>::Fifo(void) {}

template<typename itemType>
Fifo<itemType>::~Fifo() {}

template<typename itemType>
itemType Fifo<itemType>::get_element(void){
    std::lock_guard<std::mutex> l(lock);
    itemType retMsg = _fifo.front();
    _fifo.pop_front();
    return retMsg;
}

template<typename itemType>
void Fifo<itemType>::push_element(itemType&& item){
    std::lock_guard<std::mutex> l(lock);
    _fifo.push_back(item);
}

template<typename itemType>
bool Fifo<itemType>::is_fifo_empty(void){
    return _fifo.size() == 0;
}

// needed for keeping fifo.cpp as separate compile unit
#include "protocol/protocol_msg.h"
template class Fifo<Msg>;
