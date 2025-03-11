/*
// Created by samsam on 3/11/25.
*/
#include <stdio.h>
#include <string.h>

#include "circuitFunctions.h"

int main(int argc, char const *argv[])
{
	char circuit[52][52] = { {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','n','n','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','e','e','y','y','y','y','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','e','e','e','e','y','y','y','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','e','e','e','e','y','y','y','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','e','e','e','e','y','y','y','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','n','n','n'},{'n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','n','n','n'},{'n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','n','n','n'},{'n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','n','n','n'},{'n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n'},{'n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n'},{'n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n'},{'n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n'},{'n','n','n','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n'},{'n','n','n','n','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n'},{'n','n','n','n','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n'},{'n','n','n','n','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n'},{'n','n','n','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n'},{'n','n','n','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n'},{'n','n','n','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n'},{'n','n','n','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n'},{'n','n','n','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','y','y','y','y','s','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','y','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','y','y','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','n','y','y','y','y','y','y','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'},{'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n'} };

	printf("%c\n",circuit[0][0]);
	circuit_s circ;
	memcpy(circ.circuit, circuit, sizeof(circuit));
	int* start = findStart(circ);
	printf("Depart : (%d,%d)",*start, *(start + 1));
	return 0;
}

