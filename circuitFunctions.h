/*
// Created by samsam on 3/11/25.
*/

#ifndef CIRCUITFUNCTIONS_H
#define CIRCUITFUNCTIONS_H


#define GRID_SIZE 52

// All actions are defined here, SLOWER means the speed decreases,
// KEEP means it stays the same, FASTER means increase
#define GO_LEFT_SLOWER 0
#define GO_LEFT_KEEP 1
#define GO_LEFT_FASTER 2
#define GO_FORWARD_SLOWER 3
#define GO_FORWARD_KEEP 4
#define GO_FORWARD_FASTER 5
#define GO_RIGHT_SLOWER 6
#define GO_RIGHT_KEEP 7
#define GO_RIGHT_FASTER 8

#endif /*CIRCUITFUNCTIONS_H*/

typedef struct {
    char* circuit;
    int width;
    int height;
    int depart[2];
} circuit_s;

int* findStart(circuit_s circuit);
void openCircuit(char* fileName, circuit_s* circuit);
char getIJ(circuit_s* circuit_s, int i, int j);