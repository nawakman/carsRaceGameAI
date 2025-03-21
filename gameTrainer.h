/*
// Created by samsam on 3/14/25.
*/

#ifndef GAMETRAINER_H
#define GAMETRAINER_H

#include "AIBehaviour.h"

constexpr int nbPlayersEachGen=10000;

class gameTrainer {
    std::vector<AIPlayer> thisGeneration;
    int generationIndex=0;//which generation is this one
public:
    gameTrainer() {
        Circuit test1 = Circuit("../circuits/test1Points.txt");
        for (int i=0; i<nbPlayersEachGen; i++) {
            AIPlayer ai = AIPlayer();
            ai.generateBullshitPlayer();
            ai.addGame(&test1);
            ai.playGames();
            thisGeneration.push_back(ai);
        }
    };
    void train(int nbGens);
    int printBestScore();
    void SortIndividualsByScore();
    void SaveGenerationToFile();
};



#endif //GAMETRAINER_H
