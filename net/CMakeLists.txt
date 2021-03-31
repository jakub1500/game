
add_library(
    net
    net_tcpip.cpp
    net_session.cpp
)

target_compile_definitions(
    net
        PUBLIC
            IP=${IP}
            PORT=${PORT}
)