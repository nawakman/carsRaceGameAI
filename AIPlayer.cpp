/*
// Created by samsam on 3/14/25.
*/

#include "AIPlayer.h"

#include <iostream>
#include <algorithm>
#include <cstdlib>

/*==============AI GAME==============*/
std::array<int,2> AIGame::GetNextDirectionFromDecision(const int decision) const{
    const int angleOffset=45*((decision%3)-1);
      return AngleToDirection(angle+angleOffset);
}

void AIGame::MoveAIPlayer(const int decision){
    std::array<int,2> scannedTile=position;//cpp std::array makes deep copy on assignment
    const std::array<int,2> newDirection=GetNextDirectionFromDecision(decision);
    for(int i=0;i<GetNextSpeedFromDecision(decision);i++) {
        std::array<int,2> nextTileInDirection={scannedTile[0]+newDirection[0],scannedTile[1]+newDirection[1]};
        if (circuitRef->getIJ(nextTileInDirection[0],nextTileInDirection[1])=='n'){//if next move will crash in wall then put player just before the wall
            std::cout<<"dumbass AI crahed into a wall what a failure!!!"<<std::endl;
            speed=0;
            direction={0,0};
            position=scannedTile;//if a crash is detected then the actual scannedTile is the position right before the wall
            if(scannedTile!=AIMoves.back()) {//don't put two same position else python visualiser will crash (division by 0 I guess)
                AIMoves.push_back(scannedTile);//if a crash is detected then the actual scannedTile is the position right before the wall
            }
            return;
        }else{//if no crash were detected, scannedTile is now at the good position
            scannedTile=nextTileInDirection;
        }
        speed=GetNextSpeedFromDecision(decision);//if no crash were detected, scannedTile is now at the good position
        direction=GetNextDirectionFromDecision(decision);
        position=scannedTile;
        AIMoves.push_back(scannedTile);
    }

}

int AIGame::GetNextSpeedFromDecision(const int decision) const{
    const int minSpeed=std::max(1,speed-1);//at no point your next speed should be 0
    const int speedOffset=(decision/3)-1;
    if((speedOffset==-1 && (speed==0 || speed==1)) || (speedOffset==1 && speed==NB_SPEEDS)){
        std::cout<<"AI tried to go slower than (1 or 0) or faster than NB_SPEEDS , shouldn't happen since you are supposed to remove illegal moves"<<std::endl;
    }
    return std::clamp(speed+speedOffset,minSpeed,NB_SPEEDS);//limit speed
}

// Returns true if position is invalid
bool AIGame::invalidPosition(const int i, const int j) const {
    return i<0||i>circuitRef->getHeight()||j<0||j>circuitRef->getWidth();
}

std::array<int,2> AIGame::AngleToDirection(const int angle) {
    constexpr std::array<std::array<int,2>,8> angleToDirectionLUT={//lookup table faster than switch
        std::array<int,2>{0,1},//0
        std::array<int,2>{-1,1},//45
        std::array<int,2>{-1,0},//90
        std::array<int,2>{-1,-1},//135
        std::array<int,2>{0,-1},//180
        std::array<int,2>{1,-1},//225
        std::array<int,2>{1,0},//270
        std::array<int,2>{1,1}//315
    };
    const int positiveAngle=PositiveModulo(angle,360);//so angle is in 0,360 range
    return angleToDirectionLUT[positiveAngle/45];//only possible angles are 45° division //int division is euclidean division
}

// Distance to obstacle : 1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19---------
// Captor value         : 1-2-3-4-5-6-6-7-7-7 -8 -8 -8 -8 -9 -9 9 9 9 9 9
std::array<int, 3> AIGame::getDistanceCaptors() const {
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
                    if (this->circuitRef->getIJ(position[0]+x*radius,position[1]+y*radius)=='n') {
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

void AIGame::setDirection(const int i, const int j) {
    this->direction[0]=i;
    this->direction[1]=j;
}

std::array<int,2> AIGame::GetPosition() const {
    return position;
}
void AIGame::SetPosition(const std::array<int,2> x) {
    position=x;
}

int AIGame::GetSpeed() const {
    return speed;
}

void AIGame::SetSpeed(int x) {
    speed=x;
}

int AIGame::GetAngle() const {
    return angle;
}
void AIGame::SetAngle(int x) {
    angle=x;
}

/*==============AI PLAYER==============*/

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

AIGame* AIPlayer::getGame(const int i) {
    if (i>=games.size()) {
        std::cout << "Game index out of bounds" << std::endl;
        return NULL;
    }
    return &games[i];
}