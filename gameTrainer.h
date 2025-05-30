/*
// Created by samsam on 3/14/25.
*/

#ifndef GAMETRAINER_H
#define GAMETRAINER_H

#include "AIBehaviour.h"
#include <omp.h>
//https://softwareengineering.stackexchange.com/questions/338611/how-do-we-produce-the-next-generation
//the sum of the four following must equal 1 (so the numbers of individuals in the next generation is the same as the one in the current generation)
// constexpr float keepBestPercentage=0.1; //REMOVED,found to be ineffective //we only keep the best now //in case mutated and crossover perform worst (unlikely)
// constexpr float generateNewPercentage=0.1; //REMOVED,found to be ineffective //the chance of getting a good performer is infinitesimal because of the colossal number of elements of the "brain" array

//probability of crossover to happen, then probability of mutation to happen(both can stack in this order)
constexpr float crossoverProbability=0.6;
constexpr float mutateProbability=0.9;

//percentage of the decision array to be mutated
constexpr float mutationRate=0.1;

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
            this->thisGeneration[i].generateBullshitPlayer();
        }
    };

    void addTrainingCircuit(std::string filePath);
    void train(int nbGeneration);
    void sortDescendingOrder();
    int printAndSaveBestIndiviual(int generation);
    void SaveGenerationToFile();
};

#endif //GAMETRAINER_H
