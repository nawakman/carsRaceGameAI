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
	// test();
	openCircuit("../circuits/test1Points.txt", &circuit);

	printf("Char at 0 0 : %c\n", getIJ(&circuit, 51, 51));
	return 0;
}

