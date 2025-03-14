cmake_minimum_required(VERSION 3.29)
project(carsRaceGameAI C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(carsRaceGameAI
        main.c
        circuitFunctions.c
        circuitFunctions.h
)
