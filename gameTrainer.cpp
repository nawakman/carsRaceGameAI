/*
// Created by samsam on 3/14/25.
*/

#include "gameTrainer.h"
#include <fstream>
#include <iostream>
#include <random>

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
    int min=thisGeneration[0].meanScore;
    for (auto& aiplayer : thisGeneration) {
        if (aiplayer.meanScore<min) {
            min = aiplayer.meanScore;
            best=index;
        }
        index++;
    }
    std::cout<<"MASTERCLASSSS TROP FORRRT : "<<min<<" index : "<<best<<std::endl;
    thisGeneration[best].savePositionsToFile(-1,true);//we save the best individual as a generation with -1
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
        std::vector<AIPlayer> newGeneration;
        for(AIPlayer &ai:thisGeneration) {
            ai.playGames();
        }
        /*
        for(int i=0;i<thisGeneration.size();i++) {
            int i1=rand()%thisGeneration.size();
            int i2=rand()%thisGeneration.size();
            AIPlayer &ai1=thisGeneration[i1];
            AIPlayer &ai2=thisGeneration[i2];
            AIPlayer bestPlayer;
            AIPlayer worstPlayer;
            if (ai1.meanScore<ai2.meanScore) {
                bestPlayer=ai1;
                worstPlayer=ai2;
            }
            else {
                bestPlayer=ai2;
                worstPlayer=ai1;
            }
            newGeneration.push_back(bestPlayer);
        }

        for (int i=0;i<newGeneration.size();i++) {
            // All this shit to generate a random number between 0 and 1
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> dist(0.0, 1.0);

            int ai1 = rand() % newGeneration.size();
            int ai2 = rand() % newGeneration.size();
            // Possibly crossover

            if (dist(gen)<0.6) {
                newGeneration[ai1].crossover(newGeneration[ai2]);
            }// Possibly mutate
        }
        thisGeneration=newGeneration;
    }
    */
    }
}
