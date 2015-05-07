cmake_minimum_required(VERSION 2.6)
project(tsch_trials)

add_executable(tsch_trials main.cpp)

install(TARGETS tsch_trials RUNTIME DESTINATION bin)
