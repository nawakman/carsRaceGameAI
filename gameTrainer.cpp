/*
// Created by samsam on 3/14/25.
*/

#include "gameTrainer.h"
#include <fstream>
#include <iostream>

void gameTrainer::addTrainingCircuit(const std::string filePath) {
    Circuit* newCircuit=new Circuit(filePath);//store Circuit outside of local variable
    for(AIPlayer &ai:thisGeneration) {
        ai.addGame(newCircuit);
    }
}

int gameTrainer::printBestScore() {
    int index=0, best=0;
    if (thisGeneration.empty() || !thisGeneration[0].getGame(0)) {
        std::cerr << "Error: thisGeneration is empty or game pointer is null." << std::endl;
        return -1;
    }
    int min=thisGeneration[0].getGame(0)->getDistanceToFinish();
    for (auto& aiplayer : thisGeneration) {
        auto* game=aiplayer.getGame(0);
        // std::cout<<"Distance to finish : "<<game->getDistanceToFinish()<<std::endl;
        if (game->getDistanceToFinish()<min) {
            min = game->getDistanceToFinish();
            best=index;
        }
        index++;
    }
    std::cout<<"MASTERCLASSSS TROP FORRRT : "<<min<<" index : "<<best<<std::endl;
    thisGeneration[best].savePositionsToFile(2,true);
    return min;
}


void gameTrainer::SaveGenerationToFile(){
    thisGeneration[0].savePositionsToFile(generationIndex,true);//overwrite the last file
    for (int i=1;i<thisGeneration.size();i++) {
        thisGeneration[i].savePositionsToFile(generationIndex,false);
    }
}

void gameTrainer::train(int nbGereration) {
    if (nbGereration==0) {
        std::cout<<"AAALLLLLLLOOOOOOOOOOOOOOOO"<<std::endl;
        return;
    }
    for(int i=0;i<nbGereration;i++) {
        for(AIPlayer &ai:thisGeneration) {
            ai.playGames();
            //GATHER THE BEST AND KILL THE OTHERS
        }
    }
}

