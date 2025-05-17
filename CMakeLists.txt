cmake_minimum_required(VERSION 3.29)
project(carsRaceGameAI CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(carsRaceGameAI
        main.cpp
        circuitFunctions.cpp
        AIBehaviour.cpp
        gameTrainer.cpp
)

add_executable(generateAIMoves
        AIBehaviour.cpp
        generateAIMoves.cpp
        circuitFunctions.cpp
)

find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    target_link_libraries(carsRaceGameAI PRIVATE OpenMP::OpenMP_CXX)
endif()
target_compile_options(carsRaceGameAI PRIVATE -Wall)
target_compile_options(generateAIMoves PRIVATE -Wall)
