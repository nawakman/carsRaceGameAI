/*
// Created by samsam on 3/14/25.
*/

#include "AIPlayer.h"

#include <iostream>
#include <algorithm>
#include <cstdlib>

// Full random player, used for gen 1.
void AIPlayer::generateBullshitPlayer() {
    for (int i=0; i<DIRECTION_SENSOR_MAX_NB; i++) {
        for (int j=0; j<DIRECTION_SENSOR_MAX_NB; j++) {
            for (int k=0; k<DIRECTION_SENSOR_MAX_NB; k++) {
                for (int l=0; l<ANGLES; l++) {
                    for (int m=0; m<NB_SPEEDS; m++) {
                        decisionGrid[i][j][k][l][m] = rand() % 9;
                    }
                }
            }
        }
    }
}

void AIPlayer::addGame(Circuit* circ) {
    this->games.push_back(AIGame(circ, this));
}

std::array<int,2> AIPlayer::GetNextDirectionFromDecision(int decision){
      std::array<int,2> newDirection;
      int angleOffset=90*((decision%3)-1);//WIP
      /*switch((decision%3)-1){
        case 0://left
          return {};
        }*/
}

void AIGame::MovePlayer(int chosenMove){
    std::array<int,2> scannedTile=position;
    //WIP
}

int AIGame::GetNextSpeedFromDecision(int decision){
    int minSpeed=std::max(1,speed-1);//at no point your next speed should be 0
    int speedOffset=(decision/3)-1;
    if((speedOffset==-1 && (speed==0 || speed==1)) || (speedOffset==1 && speed==NB_SPEEDS)){
        std::cout<<"AI tried to go slower than (1 or 0) or faster than NB_SPEEDS , shouldn't happen since you are supposed to remove illegal moves"<<std::endl;
    }
    return std::clamp(speed+speedOffset,minSpeed,NB_SPEEDS);//limit speed
}

int AIGame::GetSpeed() {
    return speed;
}

void AIGame::SetSpeed(int x) {
    speed=x;
}

// Returns true if position is invalid
bool AIGame::invalidPosition(int i, int j) const {
    return i<0||i>circuit->getHeight()||j<0||j>circuit->getWidth();
}

void AIGame::setDirection(int i, int j) {
    this->direction[0]=i;
    this->direction[1]=j;
}


// Distance to obstacle : 1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19---------
// Captor value         : 1-2-3-4-5-6-6-7-7-7 -8 -8 -8 -8 -9 -9 9 9 9 9 9
std::array<int, 3> AIGame::getDistanceCaptors() {
    std::array<int, 8> distanceCaptorsAllDirections{0};
    std::array<int, 3> distanceCaptors{0};
    int radius=1;
    int direction=0;

    while (radius<=14) {
        for (auto [x, y] : { std::pair{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1} }) {
            // If we haven't processed this direction already
            if (distanceCaptorsAllDirections[direction]==0) {
                // std::cout << "Processing new direction"<<std::endl;
                if (invalidPosition(position[0]+x*radius,position[1]+y*radius)) {
                    // std::cout << "Invalid position ! " << position[0]+y << " : " << position[1]+x << std::endl;
                    distanceCaptorsAllDirections[direction]=radius;
                }
                // If position is valid check if there's an 'n' on the tile
                else {
                    if (this->circuit->getIJ(position[0]+x*radius,position[1]+y*radius)=='n') {
                        distanceCaptorsAllDirections[direction]=radius;
                    }
                }
            }
            direction+=1;
        }
        direction=0;
        radius+=1;
    }
    // Debug
    /*
    std::cout << "Output for distance : " << std::endl;
    for (int i=0;i<distanceCaptorsAllDirections.size();i++) {
        std::cout << i << ":" << distanceCaptorsAllDirections[i] << "\n";
    }
    */
    // TODO chose how we implement the initial direction, for now I'm doing it this way
    int i=0;
    if (this->direction != std::array<int,2> {0,0}) {
        for (auto [x, y] : { std::pair{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1} }) {
            if (this->direction[0]==x && this->direction[1]==y) {
                // Case 0 is left, so direction + 1
                distanceCaptors[0]=distanceCaptorsAllDirections[(i+1)%8];
                distanceCaptors[1]=distanceCaptorsAllDirections[i];
                distanceCaptors[2]=distanceCaptorsAllDirections[((i-1)%8 + 8)%8];
            }
            i++;
        }
    }
    return distanceCaptors;
}

AIGame* AIPlayer::getGame(int i) {
    if (i>=games.size()) {
        std::cout << "Game index out of bounds" << std::endl;
        return NULL;
    }
    return &games[i];
}