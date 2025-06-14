/*
// Created by samsam on 3/11/25.
*/
#include <fstream>
#include "circuitFunctions.h"

#include <iostream>
#include <tgmath.h>
#include <vector>
#include <filesystem>

// Ouvre un circuit et remplit N, M et le tableau dans la structure associée.
void Circuit::openCircuit(const std::string& fileName) {
    mapName=fileName.substr(fileName.rfind('/') + 1);//get last filename after last "/" //https://stackoverflow.com/questions/12261657/how-to-get-last-element-in-tokenized-string-in-c-separated-by
    mapName=mapName.substr(0, mapName.find("Points.txt")); //get mapName before this string //used to name saved AI files
    std::cout<<"cpp working directory: "<<std::filesystem::current_path().string()<<std::endl;
    std::ifstream file(fileName);
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
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
    std::cout<<"map "<<mapName<<" loaded from "<<fileName<<std::endl;
}

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

// Finds start, set to -1,-1 if not found. It also marks around the end !!!
void Circuit::findEnd() {
    bool found = false;
    for (int i=0;i<circuit.size();i++) {
        for (int j=0;j<circuit[i].size();j++) {
            if (circuit[i][j] == 'e') {
                end[0]=i;
                end[1]=j;
                found = true;
            }
        }
    }
    if (!found) {
        std::cout<<"No end, something is wrong"<<std::endl;
        end[0]=-1;
        end[1]=-1;
        return;
    }
    // Radius around
}

bool Circuit::invalidPosition(const int i, const int j) const {
    return i<0||i>=getHeight()||j<0||j>=getWidth();
}

void Circuit::displayCircuit() const {
    std::cout<<circuit[0][0]<<"\n";
    for (int i=0;i<circuit.size();i++) {
        for (int j=0;j<circuit[i].size();j++) {
            std::cout<<circuit[i][j];
        }
        std::cout<<"\n";
    }
}

int Circuit::getHeight() const {
    return height;
}

int Circuit::getWidth() const {
    return width;
}

// Renvoie C si le caractere est pas accessible
char Circuit::getIJ(int i, int j) const {
    /*if(invalidPosition(i,j)) {
        std::cout << "index out of bounds i:"<<i<<" j:"<<j<<" in Circuit::getIJ"<<std::endl;
    }*/
    return circuit[i][j];
}