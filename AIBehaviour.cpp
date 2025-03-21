/*
// Created by samsam on 3/14/25.
*/

#include "AIBehaviour.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <sstream>

#include <filesystem> // C++17 and later
/*==============AI GAME==============*/

void AIGame::MoveAIPlayer(const char decision){
    std::array<int,2> scannedTile=position;//cpp std::array makes deep copy on assignment
    const int newAngle=GetNextAngleFromDecision(decision);
    std::cout<<"Angle: "<<angle<<" /New angle: "<<newAngle<<std::endl;
    const std::array<int,2> newDirection=AngleToDirection(newAngle);
    int newSpeed=GetNextSpeedFromDecision(decision);
    for(int i=0;i<newSpeed;i++) {
        std::array<int,2> nextTileInDirection={scannedTile[0]+newDirection[0],scannedTile[1]+newDirection[1]};
        if (circuitRef.getIJ(nextTileInDirection[0],nextTileInDirection[1])=='n'){//if next move will crash in wall then put player just before the wall
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
    }
    speed=newSpeed;//if no crash were detected, scannedTile is now at the good position
    angle=newAngle;
    direction=newDirection;
    position=scannedTile;
    AIMoves.push_back(scannedTile);
}

int AIGame::GetNextSpeedFromDecision(const char decision) const{
    if(decision<8) {
        return 1;
    }
    const int minSpeed=std::max(1,speed-1);//at no point your next speed should be 0
    const int speedOffset=((decision+1)/3)-4;
    if((speedOffset==-1 && (speed==0 || speed==1)) || (speedOffset==1 && speed==MAX_SPEED)){
        std::cout<<"ILLEGAL MOVE, AI tried to go slower than (1 or 0) or faster than NB_SPEEDS"<<std::endl;
    }
    return std::clamp(speed+speedOffset,minSpeed,MAX_SPEED);//limit speed
}

int AIGame::GetNextAngleFromDecision(const char decision) const{
    if(decision<8) {//decision 0-7 means the player is stopped and can choose any directionç
        if(speed>0) {
            std::cout<<"ILLEGAL MOVE, AI is not stopped but tries to force a new direction"<<std::endl;
        }
        return 45*decision;
    }
    if(speed==0) {
        std::cout<<"ILLEGAL MOVE, AI is stopped but tries to steer left or right"<<std::endl;
    }
    const int angleOffset=45*(((decision+1)%3)-1);//decision 8-15 are when player is already moving
    return angle+angleOffset;
}

char AIPlayer::getDecisionGrid(int i, int j, int k, int l, int m) const {
    return decisionGrid[i][j][k][l][m];
}


void AIGame::PlayMoveFromGrid() {
    const std::array<int, 3> distances=getDistanceCaptors();
    const char decision=playerRef->getDecisionGrid(distances[0],distances[1],distances[2],angle/ANGLES_RESOLUTION,speed);
    std::cout<<"decision: "<<(int) decision<<std::endl;
    MoveAIPlayer(decision);
}

// Returns true if position is invalid
bool AIGame::invalidPosition(const int i, const int j) const {
    return i<0||i>circuitRef.getHeight()||j<0||j>circuitRef.getWidth();
}

std::array<int,2> AIGame::AngleToDirection(const int angle) {//in degrees
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
                    if (this->circuitRef.getIJ(position[0]+x*radius,position[1]+y*radius)=='n') {
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

std::string AIGame::GetMovesAsString() {
    std::stringstream ss;
    for(std::array<int,2> position:AIMoves) {
        ss<<position[0]<<","<<position[1]<<";";
    }
    std::string movesWithoutLastComma=ss.str();
    movesWithoutLastComma.pop_back();//remove last ";"
    return movesWithoutLastComma;
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
const Circuit& AIGame::GetCircuitRef() const{
    return circuitRef;
}

/*==============AI PLAYER==============*/
int AIPlayer::getRandomAllowedMove(int frontLeftDistance, int frontDistance, int frontRightDistance, int _angle, int _speed){//underscore to differentiate with this.speed
    if(_speed==0) {
        return rand()%8;//choose new direction, speed will be 1
    }else if(_speed==1) {
        return (rand()%6)+8+3;//remove slow down //we can only stay at the same speed or go faster
    }else if(_speed==MAX_SPEED) {
        return (rand()%6)+8;//remove accelerate //we can only stay at the same speed or go slower
    }else{
        return (rand()%9)+8;//steer left, center right, acceleration and deceleration
    }
}

// Full random player, used for gen 1.
void AIPlayer::generateBullshitPlayer() {
    for (int i=0; i<DIRECTION_SENSOR_RESOLUTION; i++) {//distance in the front left direction
        for (int j=0; j<DIRECTION_SENSOR_RESOLUTION; j++) {//distance in the front direction
            for (int k=0; k<DIRECTION_SENSOR_RESOLUTION; k++) {//distance in the front right direction
                for (int l=0; l<ANGLES_RESOLUTION; l++) {
                    for (int m=0; m<MAX_SPEED; m++) {
                        decisionGrid[i][j][k][l][m] = getRandomAllowedMove(i,j,k,l,m);
                    }
                }
            }
        }
    }
}

void AIPlayer::addGame(Circuit* circ) {
    this->games.push_back(AIGame(*circ, this));
}

AIGame* AIPlayer::getGame(const int i) {
    if (i>=games.size()) {
        std::cout << "Game index out of bounds" << std::endl;
        return nullptr;
    }
    return &games[i];
}

void AIPlayer::SaveToFile(const int generation, const bool overwriteFile) {
    std::stringstream ss;//handle conversion from int to string
    std::string filePath;
    for(AIGame game : games) {
        ss<<"../AI/"+game.GetCircuitRef().mapName<<"-gen"<<generation<<".txt";
        filePath=ss.str();
        //std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

        if (overwriteFile) {
            std::ofstream file(filePath);// Create and open a text file
            if(!file.is_open()){std::cout<<"error creating the file "<<filePath<<std::endl;}
            file<<game.GetCircuitRef().mapName<<std::endl<<games.size()<<std::endl;//if we overwrite file we need to write these
            file.close();
        }
        std::ofstream file(filePath,std::ios::app);//append at the end of existing file
        if(!file.is_open()){std::cout<<"error opening the file "<<filePath<<std::endl;}
        file<<game.GetMovesAsString()<<std::endl;
        file.close();
    }
    std::cout<<"file saved at "<<filePath<<std::endl;
}