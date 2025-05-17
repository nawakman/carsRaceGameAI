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
        std::cout<<"AAALLLLLLLOOOOOOOOOOOOOOOO"<<std::endl;
        return;
    }
    std::vector<AIPlayer> newGeneration;//prepare a container for the next generation
    for(int i=0;i<nbGeneration;i++) {
        //for each generation
        newGeneration.clear();//make sure it is empty before filling it with players
        for(AIPlayer &ai:thisGeneration) {//simulate and score individuals
            for(Circuit &c:trainingCircuits) {//add all training circuits to the individuals of this generation
                ai.addGame(&c);
            }
            ai.playGames();
        }

        int tournament_size = 15;
            //std::max((unsigned long)2, thisGeneration.size()/10);

        // We keep the best one
        int min = thisGeneration[0].meanScore;
        int incideBest=0;
        for (int j=0; j<thisGeneration.size(); j++) {
            if (thisGeneration[i].meanScore < min) {
                min = thisGeneration[i].meanScore;
                incideBest = i;
            }
        }
        newGeneration.push_back(thisGeneration[incideBest]);

        for (int j=1;j<thisGeneration.size();j++) {
            // First inital 2
            AIPlayer* ai1 = pickBestOfRandomPool(thisGeneration, tournament_size);
            AIPlayer* ai2 = pickBestOfRandomPool(thisGeneration, tournament_size);
            // Final one that we will add
            AIPlayer ai3;
            ai3.meanScore = 0;
            // We crossover with probability of crossoverPercentage
            if (((double)rand()/(double)RAND_MAX) > crossoverPercentage) {
                ai1->crossover(ai2,&ai3);
            }
            else {
                ai3.copyGrid(ai1);
            }
            if (((double)rand()/(double)RAND_MAX) > mutatePercentage || 1==1) {
                // std::cout<<"Currently mutating"<<std::endl;
            ai3.mutate(mutationRate);
            }


            newGeneration.push_back(ai3);
        }
        /*
        for(int i=0;i<std::round(keepBestPercentage*nbAIPerGeneration);i++) {
            newGeneration.push_back(AIPlayer(thisGeneration[i]));//create NEW individual that has a COPY of this individual decisionGrid, so we can discard the previous generation
        }
        //KEEP AND MUTATE BEST INDIVIDUALS
        for(int i=0;i<std::round(mutateBestPercentage*nbAIPerGeneration);i++) {
            newGeneration.push_back(AIPlayer(thisGeneration[i]));//create NEW individual that has a COPY of this individual decisionGrid, so we can discard the previous generation
            newGeneration.back().mutate(mutationRate);//mutate the best individuals so they might have better strategies
        }

        //CROSSOVER ,MIX TWO INDIVIDUALS GENES
        for(int i=0;i<std::round(crossoverPercentage*nbAIPerGeneration);i++) {

        }
        */
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
        //GENERATE NEW (might find new strategies)
        /* Nope won't do that
        for(int i=0;i<std::round(generateNewPercentage*nbAIPerGeneration);i++) {
            AIPlayer ai = AIPlayer();
            ai.generateBullshitPlayer();
            newGeneration.push_back(ai);
        }
        */

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
