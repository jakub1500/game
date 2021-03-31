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

set(BOOST_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/asio/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/core/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/system/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/config/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/utility/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/assert/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/static_assert/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/throw_exception/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/type_traits/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/numeric_conversion/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/mpl/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/preprocessor/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bind/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/regex/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/predef/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/date_time/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/smart_ptr/include")

add_subdirectory(common)
add_subdirectory(net)
add_subdirectory(processers)
add_subdirectory(protocol)
add_subdirectory(actions)

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

target_link_libraries(game PRIVATE net common protocol world processer_game -pthread ncurses)
target_link_libraries(server PRIVATE net common protocol world processer_server -pthread)
target_include_directories(action_game PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(action_server PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(processer_game PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(
    processer_server 
        PUBLIC 
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${BOOST_INCLUDES}"
)
target_include_directories(common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(
    net 
        PUBLIC 
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${BOOST_INCLUDES}"
)
target_include_directories(world PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
