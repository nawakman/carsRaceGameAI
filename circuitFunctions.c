/*
// Created by samsam on 3/11/25.
*/

#include "circuitFunctions.h"

#include <stdio.h>
#include <stdlib.h>

int* findStart(circuit_s circuit) {
    int* depart = (int*) malloc(sizeof(int)*2);
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (circuit.circuit[i][j] == 's') {
                depart[0]=i;
                depart[1]=j;
                return depart;
            }
        }
    }
    depart[0]=-1;
    depart[1]=-1;
    return depart;
}