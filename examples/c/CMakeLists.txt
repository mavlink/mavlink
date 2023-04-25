cmake_minimum_required(VERSION 3.13)

project(udp_example C)

set(CMAKE_C_STANDARD 11)

find_package(MAVLink REQUIRED)

add_executable(udp_example udp_example.c)

target_link_libraries(udp_example PRIVATE MAVLink::mavlink)
