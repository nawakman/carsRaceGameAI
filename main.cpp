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
	srand(time(NULL));
	std::filesystem::remove_all("AI");//clear previous AI trainings and brains
	std::filesystem::create_directories("AI");//recreates deleted folders
	std::filesystem::create_directories("AI/brains");//recreates deleted folders
	gameTrainer game_trainer=gameTrainer(2000);
	game_trainer.addTrainingCircuit("circuits/test1Points.txt");
	game_trainer.train(500);
	return 0;
}

