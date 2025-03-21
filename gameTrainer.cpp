/*
// Created by samsam on 3/14/25.
*/

#include "gameTrainer.h"
#include <fstream>

void gameTrainer::SortIndividualsByScore(){
  //WIP
  }

void gameTrainer::SaveGenerationToFile(){
    //SortIndividualsByScore();//UNCOMMENT WHEN METHOD IS WRITTEN
    thisGeneration[0].savePositionsToFile(generationIndex,true);//overwrite the last file
    for (int i=1;i<thisGeneration.size();i++) {
        thisGeneration[i].savePositionsToFile(generationIndex,false);
    }
  }


