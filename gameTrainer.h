/*
// Created by samsam on 3/14/25.
*/

#ifndef GAMETRAINER_H
#define GAMETRAINER_H

#include "AIBehaviour.h"

class gameTrainer {
    std::vector<AIPlayer> thisGeneration;
    int generationIndex=0;//which generation is this one
public:
    void SortIndividualsByScore();
    void SaveGenerationToFile();
};



#endif //GAMETRAINER_H
