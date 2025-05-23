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
    void findEnd();
    void openCircuit(const std::string& fileName);
public:
    Circuit(const std::string& fileName) {
        openCircuit(fileName);
        findStart();
        findEnd();
    }
    mutable std::string mapName;
    std::array<int,2> start;
    std::array<int,2> end;
    int getHeight() const;
    int getWidth() const;
    void affiche() const;
    bool invalidPosition(int i, int j) const;
    char getIJ(int i, int j) const;
};

