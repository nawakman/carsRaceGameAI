cmake_minimum_required(VERSION 3.29)
project(carsRaceGameAI CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(carsRaceGameAI
    main.cpp circuitFunctions.cpp AIPlayer.cpp
)
target_compile_options(carsRaceGameAI PRIVATE -Wall)
