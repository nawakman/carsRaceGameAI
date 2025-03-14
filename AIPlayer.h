/*
// Created by samsam on 3/14/25.
*/

#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector>
#include <array>

// All actions are defined here, SLOWER means the speed decreases,
// KEEP means it stays the same, FASTER means increase
//we can get direction choice using x%3 and get speed offet using (x//3)-1
#define GO_LEFT_SLOWER 0
#define GO_FORWARD_SLOWER 1
#define GO_RIGHT_SLOWER 2
#define GO_LEFT_KEEP 3
#define GO_FORWARD_KEEP 4
#define GO_RIGHT_KEEP 5
#define GO_LEFT_FASTER 6
#define GO_FORWARD_FASTER 7
#define GO_RIGHT_FASTER 8



constexpr int DIRECTION_SENSOR_MAX_NB=9;
constexpr int ANGLES=8;
constexpr int NB_SPEEDS=10;

/* Represents the AI playing the game
 * Its decisionGrid is shaped like follows :
 * decisionGrid[LEFT_SENSOR][FRON_SENSO][RIGHT_SENSOR][ANGLE][SPEED] = ASSOCIATED_DECISION
 * for example, decisionGrid[3][4][1][0][8] = 2
 * which means that for these values of sensors, we chooe action 2 (see actions above)
*/
class AIPlayer {
    char decisionGrid[DIRECTION_SENSOR_MAX_NB][DIRECTION_SENSOR_MAX_NB][DIRECTION_SENSOR_MAX_NB][ANGLES][NB_SPEEDS];

    std::array<int,2> position;//stores current position
    std::array<int,2> direction={0,0};//stores current ABSOLUTE direction
    int speed=0;//stores current speed
    std::vector <int[2]>AIMoves;//to store every player moves on the current map //then returned to python script to visualize path

public:
    void generateBullshitPlayer();
    int takeDecision();
    void MovePlayer(int decision);

    std::array<int,2> GetNextDirectionFromDecision(int decision);
    int GetNextSpeedFromDecision(int decision);


    int GetSpeed();
    void SetSpeed(int x);
};

#endif
