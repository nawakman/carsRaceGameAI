/*
// Created by samsam on 3/11/25.
*/
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include "AIBehaviour.h"

int main(int argc, char const *argv[])
{
	/*SAMUEL PLAYGROUND*/
	/*srand(time(NULL));
	Circuit test1 = Circuit("../circuits/test1Points.txt");

	AIPlayer ai;
	ai.generateBullshitPlayer();
	ai.addGame(&test1);
	ai.getGame(0)->setDirection(0,-1);
	std::array<int, 3> distanceCaptors{};
	distanceCaptors = ai.getGame(0)->getDistanceCaptors();
	std::cout << "Captors for direction 0 -1" << std::endl;
	std::cout << distanceCaptors[0] << "," << distanceCaptors[1] << "," << distanceCaptors[2] << std::endl;*/
	// ai.playGames()
	// ai.score
	// ai.games[X].getMoves

	// Dans l'idee, le but final c'est qu'on ait une giga classe trainer d'ia,
	// qu'elle crée genre 10 AIGames, qu'elle les mett

	/*THEO PLAYGROUND*/
	/*//TEST AngleToDirection
	 for(int i = -360; i < 360; i=i+10) {
		std::array<int,2> tmp=AIGame::AngleToDirection(i);
		std::cout<<"angle: "<<i<<" direction: "<<tmp.at(0)<<","<<tmp.at(1)<<std::endl;
	}
	*/

	/*//TEST GetNextDirectionFromDecision
	Circuit test1 = Circuit("../circuits/test1Points.txt");
	AIPlayer ai;
	ai.addGame(&test1);
	for (int i = 0; i <7 ; i++) {
		std::array<int,2> tmp=ai.getGame(0)->GetNextDirectionFromDecision(i);
		std::cout<<"playerAngle: "<<ai.getGame(0)->GetAngle()<<" direction: "<<tmp[0]<<";"<<tmp[1]<<std::endl;
	}*/

	//TEST Move //I CANNOT DEBUG THIS WITHOUT VISUALIZING IT
	srand(12);
	Circuit test1 = Circuit("../circuits/test1Points.txt");
	AIPlayer ai;
	ai.generateBullshitPlayer();
	ai.addGame(&test1);
	std::cout<<test1.end[0]<<','<<test1.end[1]<<" ; "<<test1.start[0]<<','<<test1.start[1]<<std::endl;
	std::cout<<test1.start[0]-test1.end[0]<<" : "<<test1.end[1]-test1.start[1]<<std::endl;
	std::cout<<(test1.start[0]-test1.end[0])/(test1.end[1]-test1.start[1])<<std::endl;
	double arc = static_cast<double>(test1.start[0] - test1.end[0])/(test1.end[1]-test1.start[1]);

	double angle = std::atan2(double(test1.start[0]-test1.end[0]),double(test1.end[1]-test1.start[1]));
	std::cout<<"Angle from start to finish :"<<coordsToAngle(0,50,test1.end[0],test1.end[1])<<std::endl;
	ai.playGames();
	ai.savePositionsToFile(0,true);
	/*
	for (int i = 0; i <50 ; i++) {
		std::array<int,2> tmp=ai.getGame(0)->GetPosition();
		std::cout<<"position: "<<tmp[0]<<","<<tmp[1]<<std::endl;
		ai.getGame(0)->PlayMoveFromGrid();
	}

	std::array<int,2> tmp=ai.getGame(0)->GetPosition();
	std::cout<<"position: "<<tmp[0]<<","<<tmp[1]<<std::endl;

	ai.SaveToFile(0,true);
	*/
	return 0;
}

