/*
// Created by samsam on 3/11/25.
*/
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include "AIBehaviour.h"
#include "gameTrainer.h"

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	gameTrainer game_trainer=gameTrainer(10000);
	game_trainer.addTrainingCircuit("../circuits/test2Points.txt");
	game_trainer.train(1);
	//game_trainer.printBestScore();
	game_trainer.SaveGenerationToFile();
	return 0;
}

