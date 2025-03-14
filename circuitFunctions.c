/*
// Created by samsam on 3/11/25.
*/

#include "circuitFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* findStart(circuit_s circuit) {
    int* depart = (int*) malloc(sizeof(int)*2);
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            //if (circuit.circuit[i][j] == 's') {
              //  depart[0]=i;
                //depart[1]=j;
               // return depart;
            //}
        }
    }
    depart[0]=-1;
    depart[1]=-1;
    return depart;
}

char

void test() {
    printf("aaaaa");
}

void openCircuit(char* fileName, circuit_s* circuit) {

    FILE *file = fopen(fileName, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    int x,y;

    char buffer[256];

    // Read coords
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        printf("File is empty or error reading the first line.\n");
        fclose(file);
        exit(1);
    }

    if (sscanf(buffer, "%d;%d", &x, &y) != 2) {
        printf("Couldn't read coordinates");
        exit(1);
    };
    circuit->width=x;
    circuit->height=y;

    int totalSize = x*y;
    char* result = (char*) malloc(sizeof(char)*totalSize);
    // Now read and print the rest of the file
    printf("Reading the rest of the file:\n");
    int index=0;
    while (fgets(buffer, sizeof(buffer), file) && index < totalSize) {
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline
        int len = strlen(buffer);
        if (index + len > totalSize) len = totalSize - index; // prevent overflow
        memcpy(result + index, buffer, len);
        index += len;
    }
    fclose(file);
    circuit->circuit=result;
    printf("%d\n",strlen(circuit->circuit));
}