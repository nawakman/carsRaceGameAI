/*
// Created by samsam on 3/14/25.
*/

#include "gameTrainer.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>

void gameTrainer::addTrainingCircuit(const std::string filePath) {
    Circuit* newCircuit=new Circuit(filePath);//store Circuit outside of local variable
    for(AIPlayer &ai:thisGeneration) {
        ai.addGame(newCircuit);
    }
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

int gameTrainer::printBestScore() {//needs array to be sorted
    std::cout<<"Best score(minimum): "<<thisGeneration[0].meanScore<<std::endl;
    std::cout<<"saving best individual...\t";
    thisGeneration[0].savePositionsToFile(-1,true,true);//we save the best individual as a generation with -1
    return thisGeneration[0].meanScore;
}

void gameTrainer::SaveGenerationToFile(){
    thisGeneration[0].savePositionsToFile(generationIndex,true);//overwrite the last file
    for (int i=1;i<thisGeneration.size();i++) {
        thisGeneration[i].savePositionsToFile(generationIndex,false,true);
    }
}

void gameTrainer::train(int nbGereration) {
    if (nbGereration==0) {
        std::cout<<"AAALLLLLLLOOOOOOOOOOOOOOOO"<<std::endl;
        return;
    }
    for(int i=0;i<nbGereration;i++) {
        //for each generation
        std::vector<AIPlayer> newGeneration;//prepare a container for the next generation
        for(AIPlayer &ai:thisGeneration) {//simulate and score individuals
            ai.playGames();
        }
        sortDescendingOrder();
        //KEEP BEST INDIVIDUALS
        for(int i=0;i<std::round(mutateBestPercentage*nbAIPerGeneration);i++) {
            newGeneration.push_back(thisGeneration[i]);//copies this good individual in the next generation
        }

        //CROSSOVER ,MIX TWO INDIVIDUALS GENES
        for(int i=0;i<std::round(crossoverPercentage*nbAIPerGeneration);i++) {
            //make crossovers
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
        //GENERATE NEW (might find new strategies)
        for(int i=0;i<std::round(generateNewPercentage*nbAIPerGeneration);i++) {
            AIPlayer ai = AIPlayer();
            ai.generateBullshitPlayer();
            newGeneration.push_back(ai);
        }

        std::cout<<"new Generation scores"<<std::endl;
        for(AIPlayer &ai:newGeneration) {
            std::cout<<ai.meanScore<<",";
        }
        std::cout<<std::endl;
    }
}
