/*
// Created by samsam on 3/14/25.
*/

#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector>
#include <array>

#include "circuitFunctions.h"

// All actions are defined here, SLOWER means the speed decreases,
// KEEP means it stays the same, FASTER means increase
// we can get direction choice using x%3 and get speed offet using (x//3)-1
#define GO_LEFT_SLOWER 0
#define GO_FORWARD_SLOWER 1
#define GO_RIGHT_SLOWER 2
#define GO_LEFT_KEEP 3
#define GO_FORWARD_KEEP 4
#define GO_RIGHT_KEEP 5
#define GO_LEFT_FASTER 6
#define GO_FORWARD_FASTER 7
#define GO_RIGHT_FASTER 8


class AIPlayer;

constexpr int DIRECTION_SENSOR_MAX_NB=9;
constexpr int ANGLES=8;
constexpr int NB_SPEEDS=10;
constexpr int NB_MAPS=1;

/* Represents the AI playing the game
 * Its decisionGrid is shaped as follows :
 * decisionGrid[LEFT_SENSOR][FRONT_SENSOR][RIGHT_SENSOR][ANGLE][SPEED] = ASSOCIATED_DECISION
 * for instance, decisionGrid[3][4][1][0][8] = 2
 * which means that for these values of sensors, we choose action 2 (see actions above)
*/

class AIGame {
    std::array<int,2> position;//stores current position
    std::array<int,2> direction={0,0};//stores current ABSOLUTE direction
    int speed=0;//stores current speed
    std::vector <int[2]>AIMoves;//to store every player moves on the current map //then returned to python script to visualize path
    Circuit* circuit;
    bool invalidPosition(int i, int j) const;
    // We need the player to have access to the grid
    AIPlayer* player;

public:
    // Only for testing, will remove later :
    void setDirection(int i, int j);
    std::array<int,3> getDistanceCaptors();
    // Constructor
    AIGame(Circuit* circuit, AIPlayer* player) {
        this->player = player;
        this->circuit = circuit;
        position = circuit->start;
    }
    // We need game related context to make a decision.
    int getDistanceToFinish();
    int takeDecision();
    int GetNextSpeedFromDecision(int decision);
    void MovePlayer(int decision);
    int GetSpeed();
    void SetSpeed(int x);
};

class AIPlayer {
    char decisionGrid[DIRECTION_SENSOR_MAX_NB][DIRECTION_SENSOR_MAX_NB][DIRECTION_SENSOR_MAX_NB][ANGLES][NB_SPEEDS];
    int meanScore=-1;
    std::vector<AIGame> games;

public:

    void generateBullshitPlayer();
    void addGame(Circuit* circ);
    AIGame* getGame(int i);
    // I don't get this one
    std::array<int,2> GetNextDirectionFromDecision(int decision);

};

#endif
