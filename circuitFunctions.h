/*
// Created by samsam on 3/11/25.
*/

#ifndef CIRCUITFUNCTIONS_H
#define CIRCUITFUNCTIONS_H

#include <string>
#include <vector>
#include <array>

#endif CIRCUITFUNCTIONS_H

class Circuit {
    int width;
    int height;
    std::vector<std::vector <char>> circuit;
    void findStart();
    void openCircuit(std::string fileName);

public:
    Circuit(std::string fileName) {
        openCircuit(fileName);
        findStart();
    }
    std::array<int,2> start;
    void affiche() const;
    char getIJ(int i, int j) const;
};

