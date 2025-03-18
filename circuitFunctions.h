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
    void openCircuit(const std::string& fileName);

public:
    Circuit(const std::string& fileName) {
        openCircuit(fileName);
        findStart();
    }
    std::string mapName;
    std::array<int,2> start;
    int getHeight() const;
    int getWidth() const;
    void affiche() const;
    char getIJ(int i, int j) const;
};

