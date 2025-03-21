/*
// Created by samsam on 3/14/25.
*/

#ifndef GAMETRAINER_H
#define GAMETRAINER_H

#include "AIBehaviour.h"

class gameTrainer {
    std::vector<AIPlayer> thisGeneration;
    int nbAIPerGeneration;
    int generationIndex=0;//which generation is this one
public:
    gameTrainer(const int _nbAIPerGeneration) {
        nbAIPerGeneration=_nbAIPerGeneration;
        for (int i=0; i<nbAIPerGeneration; i++) {
            AIPlayer ai = AIPlayer();
            ai.generateBullshitPlayer();
            thisGeneration.push_back(ai);
        }
    };
    void addTrainingCircuit(const std::string filePath);
    void train(int nbGeneration);
    int printBestScore();
    void SaveGenerationToFile();
};



#endif //GAMETRAINER_H
