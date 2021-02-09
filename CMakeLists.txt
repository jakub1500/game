cmake_minimum_required(VERSION 3.12)

# Setup only C++ compiler for this project.
project(MyProject
        VERSION 1.0.0
        LANGUAGES CXX)

# Just for testing purpose
variable_watch(DEBUG)
set(DEBUG 1)
message(WARNING "The value of debug is: ${DEBUG}")
set(DEBUG 2)
# --------------------------------

set(IP \"127.0.0.1\")
set(PORT 4444)

add_subdirectory(net)
add_subdirectory(processers)
add_subdirectory(protocol)

add_library(
    world
    world/field.cpp
    world/player.cpp
    world/world.cpp
)

add_executable(
    game
    game.cpp
    screen.cpp
    action.cpp
)

add_executable(
    server
    server.cpp
)

# find_package (Threads)

# target_link_libraries(game PRIVATE net world ${CMAKE_THREAD_LIBS_INIT} ncurses)
# target_link_libraries(server PRIVATE net world ${CMAKE_THREAD_LIBS_INIT} ncurses)
target_link_libraries(game PRIVATE net protocol world processer_game -pthread ncurses)
target_link_libraries(server PRIVATE net protocol world processer_server -pthread)
target_include_directories(processer_game PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(processer_server PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(net PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(world PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
