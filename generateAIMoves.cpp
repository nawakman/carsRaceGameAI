//
// Created by theo on 3/28/25.
//

#include "generateAIMoves.h"
#include "AIBehaviour.h"

#include <iostream>
#include <filesystem>

int main(int argc, char const *argv[])
{
    std::cout<<"cpp working directory: "<<std::filesystem::current_path().string()<<std::endl;
    if (argc!=3) {//program name counts as argument, in reality we only have 2 useful arguments
        std::cout<<"wrong args, please bigBrain file path and mapPoints file path "<<std::endl;
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    AIPlayer ai=AIPlayer();
    ai.loadDecisionGridFromFile(argv[1]);//load brain from given filepath
    Circuit circuit=Circuit(argv[2]);//play on given circuit
    ai.addGame(&circuit);
    ai.getGame(0)->playGame();
    ai.savePositionsToFile(-1,true);
    return 0;
}
