/*
// Created by samsam on 3/11/25.
*/
#include <stdio.h>
#include <string.h>

#include "circuitFunctions.h"

int main(int argc, char const *argv[])
{
	//printf("Depart : (%d,%d)\n",*start, *(start + 1));
	circuit_s circuit;
	openCircuit("../circuits/test1Points.txt", &circuit);

	for (int i = 0; i < circuit.height*circuit.width; i++) {
		printf("%c", circuit.circuit[i]);
	}
	return 0;
}

