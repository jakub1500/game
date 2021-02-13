#include <gtest/gtest.h>
#include "protocol_msg.h"
#include <cstring>

#define PROTOCOL_MSG_TESTSUITE protocol_msg_testsuite

TEST(PROTOCOL_MSG_TESTSUITE, basic_msg_creation)
{
    Msg msg;
    uint32_t type = msg.get_type();
    uint32_t size = msg.get_size();
    uint32_t size_in_bytes = msg.get_size_in_bytes();

    ASSERT_EQ(type, 0x41414141);
    ASSERT_EQ(size, 0);
    ASSERT_EQ(size_in_bytes, sizeof(Header));
}

TEST(PROTOCOL_MSG_TESTSUITE, msg_creation_with_initial_type)
{
    uint32_t msg_init_type = 0xdeadbabe;
    Msg msg(msg_init_type);
    uint32_t type = msg.get_type();
    uint32_t size = msg.get_size();
    uint32_t size_in_bytes = msg.get_size_in_bytes();

    ASSERT_EQ(type, msg_init_type);
    ASSERT_EQ(size, 0);
    ASSERT_EQ(size_in_bytes, sizeof(Header));
}

TEST(PROTOCOL_MSG_TESTSUITE, appending_msg_body)
{
    Msg msg;
    uint32_t p1 = 0x55;
    msg << p1;
    uint32_t size = msg.get_size();
    uint32_t size_in_bytes = msg.get_size_in_bytes();

    ASSERT_EQ(size, 1);
    ASSERT_EQ(size_in_bytes, sizeof(Header) + sizeof(uint32_t));
}

TEST(PROTOCOL_MSG_TESTSUITE, check_msg_body_pointer)
{
    Msg msg;
    uint32_t p1 = 0x55;
    uint32_t p2 = 0x44;
    msg << p1 << p2;

    uint32_t* ptr_to_body = msg.ptr_to_body();

    ASSERT_EQ(*ptr_to_body, p1);
    ASSERT_EQ(*(ptr_to_body+1), p2);
}

TEST(PROTOCOL_MSG_TESTSUITE, parse_raw_data_to_msg__empty_body)
{
    uint8_t raw_data[] = {0x41, 0x41, 0x41, 0x41,
                        0x00, 0x00, 0x00, 0x00};
    Msg msg = Msg::raw_to_msg_parser(raw_data);

    uint32_t type = msg.get_type();
    uint32_t size = msg.get_size();
    uint32_t size_in_bytes = msg.get_size_in_bytes();

    ASSERT_EQ(type, 0x41414141);
    ASSERT_EQ(size, 0);
    ASSERT_EQ(size_in_bytes, sizeof(Header));
}

TEST(PROTOCOL_MSG_TESTSUITE, parse_raw_data_to_msg__one_item_body)
{
    uint8_t raw_data[] = {0x41, 0x41, 0x41, 0x41,
                        0x01, 0x00, 0x00, 0x00,
                        0xbe, 0xba, 0xad, 0xde};
    Msg msg = Msg::raw_to_msg_parser(raw_data);

    uint32_t type = msg.get_type();
    uint32_t size = msg.get_size();
    uint32_t size_in_bytes = msg.get_size_in_bytes();
    uint32_t* ptr_to_body = msg.ptr_to_body();

    ASSERT_EQ(type, 0x41414141);
    ASSERT_EQ(size, 1);
    ASSERT_EQ(size_in_bytes, sizeof(Header) + sizeof(uint32_t));
    ASSERT_EQ(*ptr_to_body, 0xdeadbabe);
}

TEST(PROTOCOL_MSG_TESTSUITE, parse_msg_to_raw_data__empty_body)
{
    const std::size_t buffer_size = 8;
    uint8_t raw_data[] = {0x41, 0x41, 0x41, 0x41,
                        0x00, 0x00, 0x00, 0x00};
    Msg msg;
    uint8_t buffer[buffer_size];
    Msg::msg_to_raw_parser(buffer, msg);

    ASSERT_EQ(memcmp(raw_data, buffer, buffer_size), 0);
}

TEST(PROTOCOL_MSG_TESTSUITE, parse_msg_to_raw_data__one_item_body)
{
    const std::size_t buffer_size = 12;
    uint8_t raw_data[] = {0x41, 0x41, 0x41, 0x41,
                        0x01, 0x00, 0x00, 0x00,
                        0xbe, 0xba, 0xad, 0xde};
    Msg msg;
    msg << 0xdeadbabe;
    uint8_t buffer[buffer_size];
    Msg::msg_to_raw_parser(buffer, msg);

    ASSERT_EQ(memcmp(raw_data, buffer, buffer_size), 0);
}

TEST(PROTOCOL_MSG_TESTSUITE, check_if_full_msg_obtained__no_full_header)
{
    const std::size_t buffer_size = 2;
    uint8_t* raw_data = nullptr;
    bool result = Msg::check_if_full_msg_obtained(raw_data, buffer_size);

    ASSERT_FALSE(result);
}

TEST(PROTOCOL_MSG_TESTSUITE, check_if_full_msg_obtained__no_full_body_only_header)
{
    const std::size_t buffer_size = 8;
    uint8_t raw_data[] = {0x41, 0x41, 0x41, 0x41,
                        0x01, 0x00, 0x00, 0x00};
    bool result = Msg::check_if_full_msg_obtained(raw_data, buffer_size);

    ASSERT_FALSE(result);
}

TEST(PROTOCOL_MSG_TESTSUITE, check_if_full_msg_obtained__no_full_body_partial_body)
{
    const std::size_t buffer_size = 10;
    uint8_t raw_data[] = {0x41, 0x41, 0x41, 0x41,
                        0x01, 0x00, 0x00, 0x00};
    bool result = Msg::check_if_full_msg_obtained(raw_data, buffer_size);

    ASSERT_FALSE(result);
}

TEST(PROTOCOL_MSG_TESTSUITE, check_if_full_msg_obtained__full_header_empty_body)
{
    const std::size_t buffer_size = 8;
    uint8_t raw_data[] = {0x41, 0x41, 0x41, 0x41,
                        0x00, 0x00, 0x00, 0x00};
    bool result = Msg::check_if_full_msg_obtained(raw_data, buffer_size);

    ASSERT_TRUE(result);
}

TEST(PROTOCOL_MSG_TESTSUITE, check_if_full_msg_obtained__full_message)
{
    const std::size_t buffer_size = 12;
    uint8_t raw_data[] = {0x41, 0x41, 0x41, 0x41,
                        0x01, 0x00, 0x00, 0x00,
                        0xbe, 0xba, 0xad, 0xde};
    bool result = Msg::check_if_full_msg_obtained(raw_data, buffer_size);

    ASSERT_TRUE(result);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}