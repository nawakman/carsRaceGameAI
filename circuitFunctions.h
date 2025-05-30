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
    void openCircuit(const std::string& fileName);
    void findStart();
    void findEnd();

public:
    mutable std::string mapName;
    std::array<int,2> start;
    std::array<int,2> end;

    Circuit(const std::string& fileName) {
        openCircuit(fileName);
        findStart();
        findEnd();
    }
    bool invalidPosition(int i, int j) const;
    void displayCircuit() const;
    int getHeight() const;
    int getWidth() const;
    char getIJ(int i, int j) const;
};

