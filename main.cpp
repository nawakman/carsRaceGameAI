/*
// Created by samsam on 3/11/25.
*/
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "circuitFunctions.h"

int main(int argc, char const *argv[])
{
	Circuit circuit2;
	circuit2.openCircuit("../circuits/test1Points.txt");

	circuit2.affiche();

	return 0;
}

