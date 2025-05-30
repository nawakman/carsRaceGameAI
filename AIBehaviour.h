/*
// Created by samsam on 3/14/25.
*/

#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector>
#include <array>
#include <cmath>

#include "circuitFunctions.h"

// All actions are defined here

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
constexpr int DISTANCE_TO_FINISH_RESOLUTION=7;

/* Represents the AI playing the game
 * Its decisionGrid is shaped as follows :
 * decisionGrid[LEFT_SENSOR][FRONT_SENSOR][RIGHT_SENSOR][ANGLE][SPEED][DISTANCE_TO_FINISH_RESOLUTION] = ASSOCIATED_DECISION
 * for instance, decisionGrid[3][4][1][0][8][3] = 11
 * which means that for these values of sensors, we choose action 11 (see actions above)
*/

class AIGame {
    std::array<int,2> position{};//stores current position
    std::array<int,2> direction={0,0};//stores current ABSOLUTE direction
    int angle=0;//stores current angle compared to trigonometric origin //same information as direction
    int speed=0;//stores current speed
    std::vector <std::array<int,2>> AIMoves;//to store every player moves on the current map //then returned to python script to visualize path
    std::vector <std::array<std::array<int,2>,2>> segmentThatCrash;//for python visualizer //python will replay these moves to know where the crash happened
    Circuit& circuitRef;// We need the player to have access to the grid
    AIPlayer* playerRef;

public:
    // Constructor
    AIGame(Circuit& circuit, AIPlayer* player): circuitRef (circuit) {
        this->playerRef = player;
        AIMoves.push_back(circuit.start);
        position = circuit.start;
    }
    AIGame& operator=(const AIGame& other) {//if not present raises an error
        if (this == &other) {
            return *this;
        }
        position = other.position;
        direction = other.direction;
        angle = other.angle;
        speed = other.speed;
        AIMoves = other.AIMoves;
        segmentThatCrash=other.segmentThatCrash;
        circuitRef = other.circuitRef;
        playerRef = other.playerRef;
        return *this;
    }
    void playGame();
    void playMoveFromGrid();
    void moveAIPlayer(char decision);//compute next player state when we choose decision x
    static std::array<int,2> angleToDirection(int angle);
    std::array<int,3> getDistanceCaptors() const;
    int getDistanceToFinish();
    int getNextSpeedFromDecision(char decision) const;
    int getNextAngleFromDecision(char decision) const;

    std::string getMovesAsString();
    std::string getSegmentThatCrashAsString();

    //if possible put getters and setters at the end
    const Circuit& getCircuitRef() const;
    void setDirection(int i, int j);// Only for testing, will remove later :
    std::array<int,2> getPosition() const;
    void setPosition(std::array<int,2> x);
    int getSpeed() const;
    void setSpeed(int x);
    int getAngle() const;
    void setAngle(int x);
};

class AIPlayer {
    // One octal instead of four, because we're only storing values between 0 and ~15
    char decisionGrid[DIRECTION_SENSOR_RESOLUTION][DIRECTION_SENSOR_RESOLUTION][DIRECTION_SENSOR_RESOLUTION][ANGLES_RESOLUTION][MAX_SPEED][DISTANCE_TO_FINISH_RESOLUTION]{};
    std::vector<AIGame> games;

public:
    int meanScore=0;

    AIPlayer()= default;//empty constructor
    AIPlayer(const AIPlayer& other) {//deep copy of decision grid //same as AIPlayer.copyGrid
        for (int i=0; i<DIRECTION_SENSOR_RESOLUTION; i++) {//distance in the front left direction
            for (int j=0; j<DIRECTION_SENSOR_RESOLUTION; j++) {//distance in the front direction
                for (int k=0; k<DIRECTION_SENSOR_RESOLUTION; k++) {//distance in the front right direction
                    for (int l=0; l<ANGLES_RESOLUTION; l++) {
                        for (int m=0; m<MAX_SPEED; m++) {
                            for (int n=0; n<DISTANCE_TO_FINISH_RESOLUTION;n++) {
                                decisionGrid[i][j][k][l][m][n] = other.decisionGrid[i][j][k][l][m][n];
                            }
                        }
                    }
                }
            }
        }
    }

    void generateBullshitPlayer();
    void playGames();
    char getRandomAllowedMove(int frontLeftDistance, int frontDistance, int frontRightDistance, int _angle, int _speed, int distanceToFinish);
    char getDecisionGrid(int i, int j, int k, int l, int m, int n) const;
    void copyGrid(AIPlayer *other);
    void saveDecisionGridToFile(int generation) const;
    void loadDecisionGridFromFile(const std::string& filePath);
    void savePositionsToFile(int generation,bool overwriteFile,bool stfu=false);
    void crossover(AIPlayer* other, AIPlayer* result);
    void mutate(float percentage);
    void addGame(Circuit* circ);
    AIGame* getGame(int i);
};

inline int PositiveModulo(const int x, const int mod) {//https://stackoverflow.com/questions/14997165/fastest-way-to-get-a-positive-modulo-in-c-c
    return (x % mod + mod) % mod;//the C++ modulo doesn't have the same behaviour as the python one, it can return a negative value
}

inline int coordsToAngle(int i1, int j1, int i2, int j2) {
    return PositiveModulo(static_cast<int>(std::round(std::atan2(i1-i2,j2-j1)/(M_PI/4))),8);
}
#endif
