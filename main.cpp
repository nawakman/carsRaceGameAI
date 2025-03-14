/*
// Created by samsam on 3/11/25.
*/
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "AIPlayer.h"
#include "circuitFunctions.h"

int main(int argc, char const *argv[])
{
	/*SAMUEL PLAYGROUND*/
	/*srand(time(NULL));
	Circuit circuit;
	circuit.openCircuit("../circuits/test1Points.txt");

	circuit.affiche();

	circuit.findStart();*/

	/*THEO PLAYGROUND*/
	AIPlayer ai;
	std::cout<<"initial speed: "<<ai.GetSpeed()<<std::endl;
	ai.SetSpeed(ai.GetNextSpeedFromDecision(1));
	std::cout<<"decreased: "<<ai.GetSpeed()<<std::endl;//should print complaint message
	for(int i=1;i<15;i++) {
		ai.SetSpeed(ai.GetNextSpeedFromDecision(7));
		std::cout<<"increased: "<<ai.GetSpeed()<<std::endl;
	}
	return 0;
}

