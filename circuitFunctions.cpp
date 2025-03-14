/*
// Created by samsam on 3/11/25.
*/
#include <fstream>
#include "circuitFunctions.h"

#include <iostream>
#include <vector>

// Trouve le depart et remplit le tableau start de la classe
void Circuit::findStart() {
    for (int i=0;i<circuit.size();i++) {
        for (int j=0;j<circuit[i].size();j++) {
            if (circuit[i][j] == 's') {
                start[0]=i;
                start[1]=j;
            }
        }
    }
}

// Renvoie C si le caractere est pas accessible
char Circuit::getIJ(int i, int j) const {
    return circuit[i][j];
}

// Ouvre un circuit et remplit N, M et le tableau dans la structure associée.
void Circuit::openCircuit(std::string fileName) {
    std::ifstream file(fileName);
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    int x,y;

    char sep;

    file>>width>>sep>>height;
    std::cout << width << " " << sep <<" " << height << std::endl;
    circuit.resize(height);
    // Now read and print the rest of the file
    printf("Reading the rest of the file:\n");

    std::string tmp;
    std::getline(file, tmp);
    for (int i=0;i<width && std::getline(file,tmp);i++) {
        circuit[i].resize(tmp.size());
        circuit[i].assign(tmp.begin(),tmp.end());
    }
    file.close();
}

void Circuit::affiche () const {
    std::cout<<circuit[0][0]<<"\n";
    for (int i=0;i<circuit.size();i++) {
        for (int j=0;j<circuit[i].size();j++) {
            std::cout<<circuit[i][j];
        }
        std::cout<<"\n";
    }
}