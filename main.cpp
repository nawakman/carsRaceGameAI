/*
// Created by samsam on 3/11/25.
*/
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include <filesystem>
#include "AIBehaviour.h"
#include "gameTrainer.h"

int main(int argc, char const *argv[])
{
	omp_set_num_threads(1);
	std::cout<<"Running..."<<std::endl;
	srand(time(NULL));
	std::filesystem::remove_all("AI");//clear previous AI trainings and brains
	std::filesystem::create_directories("AI");//recreates deleted folders
	std::filesystem::create_directories("AI/brains");//recreates deleted folders
	gameTrainer game_trainer=gameTrainer(1500);
	game_trainer.addTrainingCircuit("circuits/test1Points.txt");
	//game_trainer.addTrainingCircuit("circuits/cornerBigToSmallPoints.txt");
	//game_trainer.addTrainingCircuit("circuits/cornerSmallToBigPoints.txt");
	game_trainer.train(200);
	return 0;
}

