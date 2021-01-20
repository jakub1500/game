cmake_minimum_required(VERSION 3.11)
project(MyProject VERSION 1.0.0)

add_library(
    net
    net/msg_processer.cpp
    net/msg_processer.h
    net/net_agent.cpp
    net/net_agent.h
    net/net_common.cpp
    net/net_common.h
    net/net_session.cpp
    net/net_session.h
)

add_library(
    world
    world/field.cpp
    world/field.h
    world/player.cpp
    world/player.h
    world/world.cpp
    world/world.h
)

add_executable(
    game
    game.cpp
    screen.cpp
    screen.h
    action.cpp
    action.h
)

add_executable(
    server
    server.cpp
    screen.cpp
    screen.h
)

find_package (Threads)

target_link_libraries(game PRIVATE net world ${CMAKE_THREAD_LIBS_INIT} ncurses)
target_link_libraries(server PRIVATE net world ${CMAKE_THREAD_LIBS_INIT} ncurses)
target_include_directories(net PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(world PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
