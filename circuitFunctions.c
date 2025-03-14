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

char* openCircuit(char* fileName, circuit_s2* circuit) {

    FILE *file = fopen(fileName, "r");
    if (!file) {
        perror("Error opening file");
        return "Non";
    }
    int x,y;

    char buffer[256];

    // Read coords
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        printf("File is empty or error reading the first line.\n");
        fclose(file);
        return "Non";
    }

    if (sscanf(buffer, "%d;%d", &x, &y) != 2) {
        printf("Couldn't read coordinates");
        exit(1);
    };
    circuit->width=x;
    circuit->height=y;

    // Now read and print the rest of the file
    printf("Reading the rest of the file:\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);  // Process the line however you want
    }

    fclose(file);
    return "AA";
}