/*
// Created by samsam on 3/11/25.
*/
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "AIPlayer.h"

int main(int argc, char const *argv[])
{
	/*SAMUEL PLAYGROUND*/
	srand(time(NULL));
	Circuit test1 = Circuit("../circuits/test1Points.txt");

	AIPlayer ai;
	ai.generateBullshitPlayer();
	ai.addGame(&test1);
	ai.getGame(0)->setDirection(0,-1);
	std::array<int, 3> distanceCaptors{};
	distanceCaptors = ai.getGame(0)->getDistanceCaptors();
	std::cout << "Captors for direction 0 -1" << std::endl;
	std::cout << distanceCaptors[0] << "," << distanceCaptors[1] << "," << distanceCaptors[2] << std::endl;
	// ai.playGames()
	// ai.score
	// ai.games[X].getMoves

	// Dans l'idee, le but final c'est qu'on ait une giga classe trainer d'ia,
	// qu'elle crée genre 10 AIGames, qu'elle les mett

	/*THEO PLAYGROUND*/
	/* Deso j'ai tout cassé mais c'est pour le mieux
	AIPlayer ai;
	std::cout<<"initial speed: "<<ai.GetSpeed()<<std::endl;
	ai.SetSpeed(ai.GetNextSpeedFromDecision(1));
	std::cout<<"decreased: "<<ai.GetSpeed()<<std::endl;//should print complaint message
	for(int i=1;i<15;i++) {
		ai.SetSpeed(ai.GetNextSpeedFromDecision(7));
		std::cout<<"increased: "<<ai.GetSpeed()<<std::endl;
	}
	*/

	return 0;
}

