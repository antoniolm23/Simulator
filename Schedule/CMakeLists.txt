cmake_minimum_required(VERSION 2.6)
project(schedule)

add_executable(schedule main.cpp)

install(TARGETS schedule RUNTIME DESTINATION bin)
