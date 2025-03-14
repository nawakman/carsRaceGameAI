/*
// Created by samsam on 3/11/25.
*/
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "circuitFunctions.h"

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	Circuit circuit;
	circuit.openCircuit("../circuits/test1Points.txt");

	circuit.affiche();

	circuit.findStart();
	return 0;
}

