/*
// Created by samsam on 3/14/25.
*/

#ifndef GAMETRAINER_H
#define GAMETRAINER_H

#include "AIBehaviour.h"
#include <omp.h>
//https://softwareengineering.stackexchange.com/questions/338611/how-do-we-produce-the-next-generation
//the sum of the four following must equal 1 (so the numbers of individuals in the next generation is the same as the one in the current generation)
// constexpr float keepBestPercentage=0.1;
constexpr float mutatePercentage=0.9;
constexpr float crossoverPercentage=0.6;
// constexpr float generateNewPercentage=0.1;
// 0.025 works well
constexpr float mutationRate=0.2;

class gameTrainer {

    int nbAIPerGeneration;
    int generationIndex=0;//which generation is this one
public:
    // Must be public or else it will error out in gameTrainer.cpp
    std::vector<AIPlayer> thisGeneration;
    std::vector<Circuit> trainingCircuits;
    gameTrainer(const int _nbAIPerGeneration) {
        nbAIPerGeneration=_nbAIPerGeneration;
        this->thisGeneration = std::vector<AIPlayer>(nbAIPerGeneration);
        #pragma omp parallel for
        for (int i=0; i<nbAIPerGeneration; i++) {
            AIPlayer ai = AIPlayer();
            ai.generateBullshitPlayer();
            this->thisGeneration[i]=ai;
        }
    };

    void addTrainingCircuit(std::string filePath);
    void train(int nbGeneration);
    void sortDescendingOrder();
    int printAndSaveBestIndiviual(int generation);
    void SaveGenerationToFile();
};



#endif //GAMETRAINER_H
