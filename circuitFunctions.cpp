/*
// Created by samsam on 3/11/25.
*/
#include <fstream>
#include "circuitFunctions.h"

#include <iostream>
#include <vector>

// Finds start, set to -1,-1 if not found
void Circuit::findStart() {
    bool found = false;
    for (int i=0;i<circuit.size();i++) {
        for (int j=0;j<circuit[i].size();j++) {
            if (circuit[i][j] == 's') {
                start[0]=i;
                start[1]=j;
                found = true;
            }
        }
    }
    if (!found) {
        std::cout<<"No start, something is wrong"<<std::endl;
        start[0]=-1;
        start[1]=-1;
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

    char sep;//separator

    file>>width>>sep>>height;
    circuit.resize(height);

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