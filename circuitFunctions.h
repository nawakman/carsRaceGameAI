/*
// Created by samsam on 3/11/25.
*/

#ifndef CIRCUITFUNCTIONS_H
#define CIRCUITFUNCTIONS_H

#endif /*CIRCUITFUNCTIONS_H*/

#include <string>
#include <vector>
#include <array>

class Circuit {
    int width;
    int height;
    std::vector<std::vector <char>> circuit;
    std::array<int,2> start;
public:
    void affiche() const;
    void findStart();
    void openCircuit(std::string fileName);
    char getIJ(int i, int j) const;
};

