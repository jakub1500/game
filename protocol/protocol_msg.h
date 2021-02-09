#pragma once

#include <iostream>
#include <vector>
#include <cstdint>

enum Msg_type : uint32_t {
    MOVE_PLAYER = 44,
    NEW_BOARD
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
    std::size_t get_size_in_bytes(void) const;

    static Msg raw_to_msg_parser(uint32_t*);
    static bool check_if_full_msg_obtained(char*, std::size_t);
    static uint32_t* msg_to_raw_parser(Msg&);
};