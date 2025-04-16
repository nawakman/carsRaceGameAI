/*
// Created by samsam on 3/14/25.
*/

#ifndef GAMETRAINER_H
#define GAMETRAINER_H

#include "AIBehaviour.h"

//gemini.google.com/app?hl=fr
//the sum of the three following must equal 1 (so the numbers of individuals in the next generation is the same as the one in the current generation)
constexpr float mutateBestPercentage=0.2;
constexpr float crossoverPercentage=0.7;
constexpr float generateNewPercentage=0.1;

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
    void addTrainingCircuit(std::string filePath);
    void train(int nbGeneration);
    void sortDescendingOrder();
    int printBestScore();
    void SaveGenerationToFile();
};



#endif //GAMETRAINER_H
