/*
// Created by samsam on 3/14/25.
*/

#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector>
#include <array>

#include "circuitFunctions.h"

// All actions are defined hereSLOWER means the speed decreases,

//when the speed is 0 only these 8 movements are allowed
//the next speed will be one
#define RIGHT_FIRST_MOVE 0
#define UP_RIGHT_FIRST_MOVE 1
#define UP_FIRST_MOVE 2
#define UP_LEFT_FIRST_MOVE 3
#define LEFT_FIRST_MOVE 4
#define DOWN_LEFT_FIRST_MOVE 5
#define DOWN_FIRST_MOVE 6
#define DOWN_RIGHT_FIRST_MOVE 7

//SLOWER means the speed decreases, KEEP means it stays the same, FASTER means increase
//for these moves we can get direction choice using ((x+1)%3)-1 and get speed offet using ((x+1)//3)-1
#define GO_LEFT_SLOWER 8
#define GO_FORWARD_SLOWER 9
#define GO_RIGHT_SLOWER 10
#define GO_LEFT_KEEP 11
#define GO_FORWARD_KEEP 12
#define GO_RIGHT_KEEP 13
#define GO_LEFT_FASTER 14
#define GO_FORWARD_FASTER 15
#define GO_RIGHT_FASTER 16


class AIPlayer;

constexpr int DIRECTION_SENSOR_RESOLUTION=9;
constexpr int ANGLES_RESOLUTION=8;
constexpr int MAX_SPEED=10;
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
    int angle=0;//stores current angle compared to trigonometric origin //same information as direction
    int speed=0;//stores current speed
    std::vector <std::array<int,2>> AIMoves;//to store every player moves on the current map //then returned to python script to visualize path
    Circuit* circuitRef;// We need the player to have access to the grid
    AIPlayer* playerRef;

public:
    // Constructor
    AIGame(Circuit* circuit, AIPlayer* player) {
        this->playerRef = player;
        this->circuitRef = circuit;//je comprenais pas j'essayait de faire circuit[i][j] mais eft c'est une ref a l'OBJET Circuit qui posède circuit
        AIMoves.push_back(circuit->start);
        position = circuit->start;
    }

    static std::array<int,2> AngleToDirection(int angle);
    std::array<int,3> getDistanceCaptors() const;
    bool invalidPosition(int i, int j) const;
    int getDistanceToFinish();
    int GetNextSpeedFromDecision(char decision) const;
    int GetNextAngleFromDecision(char decision) const;
    void MoveAIPlayer(char decision);//compute next player state when we choose decision x
    void PlayMoveFromGrid();
    std::string GetMovesAsString();

    //if possible put getters and setters at the end
    void setDirection(int i, int j);// Only for testing, will remove later :
    std::array<int,2> GetPosition() const;
    void SetPosition(std::array<int,2> x);
    int GetSpeed() const;
    void SetSpeed(int x);
    int GetAngle() const;
    void SetAngle(int x);
    Circuit *GetCircuitRef();
};

class AIPlayer {
    // One octal instead of four, because we're only storing values between 0 and ~15
    char decisionGrid[DIRECTION_SENSOR_RESOLUTION][DIRECTION_SENSOR_RESOLUTION][DIRECTION_SENSOR_RESOLUTION][ANGLES_RESOLUTION][MAX_SPEED];
    int meanScore=-1;
    std::vector<AIGame> games;

public:
    char getDecisionGrid(int i, int j, int k, int l, int m) const;
    int getRandomAllowedMove(int frontLeftDistance, int frontDistance, int frontRightDistance, int _angle, int _speed);
    void generateBullshitPlayer();
    void addGame(Circuit* circ);
    AIGame* getGame(int i);
    void savePositionsToFile(int generation,bool overwriteFile);
    void saveDecisionGridToFile(int generation);
    void loadDecisionGridFromFile(std::string filePath);

};

inline int PositiveModulo(const int x, const int mod) {//https://stackoverflow.com/questions/14997165/fastest-way-to-get-a-positive-modulo-in-c-c
    return (x % mod + mod) % mod;//the C++ modulo doesn't have the same behaviour as the python one, it can return a negative value
}

#endif
