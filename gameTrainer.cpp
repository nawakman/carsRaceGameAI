/*
// Created by samsam on 3/14/25.
*/

#include "gameTrainer.h"
#include <algorithm>   // std::sample, std::max_element, std::shuffle
#include <iterator>    // std::begin, std::end
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <omp.h>

void gameTrainer::addTrainingCircuit(const std::string filePath) {
    Circuit* c=new Circuit(filePath);//create new circuit outside of local scope
    trainingCircuits.push_back(*c);//append this circuit to the list of trainingCircuits
}

void gameTrainer::sortDescendingOrder() {//the lower the better //best individual at index 0
    qsort(thisGeneration.data(),thisGeneration.size(),sizeof(AIPlayer),[](const void* a, const void* b) {
        const auto *playerA = (const AIPlayer *)a;
        const auto *playerB = (const AIPlayer *)b;

        if (playerA->meanScore < playerB->meanScore) {
            return -1; // playerA comes before playerB
        }
        if (playerA->meanScore > playerB->meanScore) {
            return 1;  // playerA comes after playerB
        }
        return 0;  // playerA and playerB are equal in terms of meanScore
    });//https://koor.fr/C/cstdlib/qsort.wp

    /*for(AIPlayer &ai:thisGeneration) {
        std::cout<<ai.meanScore<<std::endl;
    }*/
}

int gameTrainer::printAndSaveBestIndiviual(int generation) {//needs array to be sorted
    int min=thisGeneration[0].meanScore;
    int max=min;
    int indiceSave=0;
    int meanGenerationScore=0;
    for (int i=0; i<thisGeneration.size(); i++) {
        meanGenerationScore+=thisGeneration[i].meanScore;
        if (thisGeneration[i].meanScore < min) {
            min = thisGeneration[i].meanScore;
            indiceSave = i;
        }
        if (thisGeneration[i].meanScore > max) {
            max = thisGeneration[i].meanScore;
        }
    }
    meanGenerationScore = meanGenerationScore/thisGeneration.size();
    std::cout<<"Average generation score: "<<meanGenerationScore<<std::endl;
    std::cout<<"Worst score(max): "<<max<<std::endl;
    std::cout<<"Best score(minimum): "<<min<<std::endl;
    std::cout<<"saving best individual...\t";
    //thisGeneration[0].savePositionsToFile(-1,true,true);//we save the best individual as a generation with -1 each time
    thisGeneration[indiceSave].saveDecisionGridToFile(generation);
    return thisGeneration[indiceSave].meanScore;
}

void gameTrainer::SaveGenerationToFile(){
    thisGeneration[0].savePositionsToFile(generationIndex,true);//overwrite the last file
    for (int i=1;i<thisGeneration.size();i++) {
        thisGeneration[i].savePositionsToFile(generationIndex,false,true);
    }
}

AIPlayer* pickBestOfRandomPool(std::vector<AIPlayer>& thisGeneration,
                               int tournamentSize)
{
    static std::mt19937 rng{ std::random_device{}() };

    /* 1. Build a population of pointers */
    std::vector<AIPlayer*> population;
    population.reserve(thisGeneration.size());
    for (AIPlayer& p : thisGeneration)
        population.push_back(&p);

    /* 2. Sample pointers */
    std::vector<AIPlayer*> sample;
    std::sample(population.begin(), population.end(),
                std::back_inserter(sample),
                std::min(tournamentSize, static_cast<int>(population.size())),
                rng);

    if (sample.empty()) return nullptr;

    /* 3. Pick the best pointer */
    auto bestIt = std::max_element(sample.begin(), sample.end(),
        [](const AIPlayer* a, const AIPlayer* b)
        {
            return a->meanScore > b->meanScore;
        });

    return *bestIt;        // pointer to the champion in thisGeneration
}


void gameTrainer::train(int nbGeneration) {
    if (nbGeneration==0) {
        std::cout<<"PROBLEM: nbGeneration is 0"<<std::endl;
        return;
    }
    std::vector<AIPlayer> newGeneration(thisGeneration.size());//prepare a container for the next generation
    for(int i=0;i<nbGeneration;i++) {
        //for each generation
        newGeneration.clear();//make sure it is empty before filling it with players
        newGeneration.resize(thisGeneration.size());// for parallelisation afterwards

        #pragma omp parallel for
        for(int j=0; j<thisGeneration.size();j++) {//simulate and score individuals
            AIPlayer &ai = thisGeneration[j];
            for(Circuit &c:trainingCircuits) {//add all training circuits to the individuals of this generation
                ai.addGame(&c);
            }
            ai.playGames();
        }
        //CROSSOVER OF THE BEST INDIVIDUAL OF 2 RANDOM POOLS
        int tournament_size = 30;//pool size
            //std::max((unsigned long)2, thisGeneration.size()/10);

        // find and keep the best one for next generation
        int min = thisGeneration[0].meanScore;
        int incideBest=0;
        for (int j=0; j<thisGeneration.size(); j++) {
            if (thisGeneration[j].meanScore < min) {
                min = thisGeneration[j].meanScore;
                incideBest = j;
            }
        }
        newGeneration[0] = thisGeneration[incideBest];

        #pragma omp parallel for
        for (int j=1;j<thisGeneration.size();j++) {
            // First inital 2
            AIPlayer* ai1 = pickBestOfRandomPool(thisGeneration, tournament_size);
            AIPlayer* ai2 = pickBestOfRandomPool(thisGeneration, tournament_size);
            // Final one that we will add
            AIPlayer ai3;
            ai3.meanScore = 0;
            // We crossover with probability of crossoverPercentage
            if (((double)rand()/(double)RAND_MAX) > crossoverProbability) {
                // std::cout<<"Currently mating"<<std::endl;
                ai1->crossover(ai2,&ai3);
            }
            else {
                ai3.copyGrid(ai1);
            }
            if (((double)rand()/(double)RAND_MAX) > mutateProbability) {
                // std::cout<<"Currently mutating"<<std::endl;
                ai3.mutate(mutationRate);
            }
            newGeneration[j] = ai3;
        }
        
        //INFO ABOUT THIS GENERATION
        /* Commented out for performance ( test )
        std::cout<<"this generation scores"<<std::endl;
        for(AIPlayer &ai:thisGeneration) {
            std::cout<<ai.meanScore<<",";
        }
        std::cout<<std::endl;
        */
        printAndSaveBestIndiviual(generationIndex);

        //SAVE GENERATION FOR VISUALISATION AND GO TO NEXT GENERATION
        SaveGenerationToFile();
        generationIndex++;
        thisGeneration=newGeneration;
    }
}
