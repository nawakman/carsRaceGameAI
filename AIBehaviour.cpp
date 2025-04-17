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
    // std::cout<<"Angle: "<<angle<<" /New angle: "<<newAngle<<std::endl;
    const std::array<int,2> newDirection=AngleToDirection(newAngle);
    int newSpeed=GetNextSpeedFromDecision(decision);
    for(int i=0;i<newSpeed;i++) {
        std::array<int,2> nextTileInDirection={scannedTile[0]+newDirection[0],scannedTile[1]+newDirection[1]};
        //std::cout <<"next tile in direction: "<<circuitRef.getIJ(nextTileInDirection[0],nextTileInDirection[1]) << std::endl;//DEBUG
        if (circuitRef.getIJ(nextTileInDirection[0],nextTileInDirection[1])=='n'){//if next move will crash in wall then put player just before the wall
            // std::cout<<"dumbass AI crahed into a wall what a failure!!!"<<std::endl;
            speed=0;
            direction={0,0};
            position=scannedTile;//if a crash is detected then the actual scannedTile is the position right before the wall
            if(scannedTile!=AIMoves.back()) {//don't put two same position else python visualiser will crash (division by 0 I guess)
                AIMoves.push_back(scannedTile);//if a crash is detected then the actual scannedTile is the position right before the wall
                segmentThatCrash.push_back(std::array<std::array<int,2>,2>{scannedTile,nextTileInDirection});
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

char AIPlayer::getDecisionGrid(int i, int j, int k, int l, int m, int n) const {
    return decisionGrid[i][j][k][l][m][n];
}

int AIGame::getDistanceToFinish() {
    return std::abs((position[0]-circuitRef.end[0])) + std::abs(circuitRef.end[1] - position[1]);
}

void AIGame::PlayMoveFromGrid() {
    const std::array<int, 3> distances=getDistanceCaptors();
    int distToFinish = std::min(getDistanceToFinish()/2,6);
    const char decision=playerRef->getDecisionGrid(distances[0],distances[1],distances[2],coordsToAngle(position[0],position[1],circuitRef.end[0],circuitRef.end[1]),speed,distToFinish);
    //std::cout<<"decision: "<<(int) decision<<std::endl;
    MoveAIPlayer(decision);
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
    // radius to 14
    while (radius<=8) {
        for (auto [x, y] : { std::pair{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1} }) {
            // If we haven't processed this direction already
            if (distanceCaptorsAllDirections[direction]==0) {
                // std::cout << "Processing new direction"<<std::endl;
                if (circuitRef.invalidPosition(position[0]+x*radius,position[1]+y*radius)) {//ON WHICH MAP ???
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

std::string AIGame::getMovesAsString() {
    std::stringstream ss;
    if(AIMoves.empty()){return"";}
    for(std::array<int,2> position:AIMoves) {
        ss<<position[0]<<","<<position[1]<<";";
    }
    std::string movesWithoutLastComma=ss.str();
    movesWithoutLastComma.pop_back();//remove last ";"
    return movesWithoutLastComma;
}

std::string AIGame::getSegmentThatCrashAsString() {
    std::stringstream ss;
    if(segmentThatCrash.empty()){return"";}
    for(std::array<std::array<int,2>,2> position:segmentThatCrash) {
        ss<<position[0][0]<<","<<position[0][1]<<","<<position[1][0]<<","<<position[1][1]<<";";
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
char AIPlayer::getRandomAllowedMove(int frontLeftDistance, int frontDistance, int frontRightDistance, int _angle, int _speed,int distanceToFinish){//underscore to differentiate with this.speed
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

void AIPlayer::crossover(AIPlayer& player) {
    int nbcases = DIRECTION_SENSOR_RESOLUTION*DIRECTION_SENSOR_RESOLUTION*DIRECTION_SENSOR_RESOLUTION*ANGLES_RESOLUTION*MAX_SPEED*DISTANCE_TO_FINISH_RESOLUTION;
    int i1 = rand() % nbcases;
    int i2 = rand() % nbcases;
    i1 = std::min(i1, i2);
    i2 = std::max(i1, i2);

    int count = 0;
    for (int i=0; i<DIRECTION_SENSOR_RESOLUTION; i++) {//distance in the front left direction
        for (int j=0; j<DIRECTION_SENSOR_RESOLUTION; j++) {//distance in the front direction
            for (int k=0; k<DIRECTION_SENSOR_RESOLUTION; k++) {//distance in the front right direction
                for (int l=0; l<ANGLES_RESOLUTION; l++) {
                    for (int m=0; m<MAX_SPEED; m++) {
                        for (int n=0; n<DISTANCE_TO_FINISH_RESOLUTION;n++) {
                            if (count > i1 && count < i2) {
                                decisionGrid[i][j][k][l][m][n]=player.decisionGrid[i][j][k][l][m][n];
                            }
                            count++;
                        }
                    }
                }
            }
        }
    }

}

// Full random player, used for gen 1.
void AIPlayer::generateBullshitPlayer() {
    for (int i=0; i<DIRECTION_SENSOR_RESOLUTION; i++) {//distance in the front left direction
        for (int j=0; j<DIRECTION_SENSOR_RESOLUTION; j++) {//distance in the front direction
            for (int k=0; k<DIRECTION_SENSOR_RESOLUTION; k++) {//distance in the front right direction
                for (int l=0; l<ANGLES_RESOLUTION; l++) {
                    for (int m=0; m<MAX_SPEED; m++) {
                        for (int n=0; n<DISTANCE_TO_FINISH_RESOLUTION;n++) {
                            decisionGrid[i][j][k][l][m][n] = getRandomAllowedMove(i,j,k,l,m,n);
                        }
                    }
                }
            }
        }
    }
}

void AIGame::playGame() {
    int maxMoves=50;
    int i=0;
    for (i=0; i<maxMoves && circuitRef.getIJ(position[0],position[1])!='e'; i++) {//while not at the finish line //"e" for end
        //std::cout << i <<"\ti:"<<position[0]<<" j:"<<position[1]<< std::endl;
        PlayMoveFromGrid();
    }
    // printf("i : %d\ndist : %d\n",i, getDistanceToFinish()*maxMoves/(circuitRef.getHeight()+circuitRef.getWidth()));
    this->playerRef->meanScore += i + getDistanceToFinish()*maxMoves/(circuitRef.getHeight()+circuitRef.getWidth());//take into account: nb of moves, normalized distance to end scaled by maxMoves
}

void AIPlayer::playGames() {
    for (auto& game:games) {
        game.playGame();
    }
}

void AIPlayer::mutate(float percentage) {//the percentage of mutatation that will happen at random places, relative to the number of elements in the decisionGrid
    int nbCases = DIRECTION_SENSOR_RESOLUTION*DIRECTION_SENSOR_RESOLUTION*DIRECTION_SENSOR_RESOLUTION*ANGLES_RESOLUTION*MAX_SPEED*DISTANCE_TO_FINISH_RESOLUTION;
    int nbMutations=(int)percentage*nbCases;

    for(int mutation=0;mutation<nbMutations;mutation++) {//same case can be modified many times
        int i=rand()%DIRECTION_SENSOR_RESOLUTION;
        int j=rand()%DIRECTION_SENSOR_RESOLUTION;
        int k=rand()%DIRECTION_SENSOR_RESOLUTION;
        int l=rand()%ANGLES_RESOLUTION;
        int m=rand()%MAX_SPEED;
        int n=rand()%DISTANCE_TO_FINISH_RESOLUTION;
        decisionGrid[i][j][k][l][m][n]=getRandomAllowedMove(i,j,k,l,m,n);
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

void AIPlayer::savePositionsToFile(const int generation, const bool overwriteFile,bool stfu) {
    std::stringstream ss;//handle conversion from int to string
    std::string filePath;
    for(AIGame game : games) {
        ss.str("");//empty the path so we can prepare the next one
        ss<<"AI/"+game.GetCircuitRef().mapName<<"-gen"<<generation<<".txt";
        filePath=ss.str();
        //std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

        if (overwriteFile) {
            std::ofstream file(filePath);// Create and open a text file
            if(!file.is_open()){std::cout<<"error creating the file "<<filePath<<std::endl;}
            file<<game.GetCircuitRef().mapName<<std::endl;//if we overwrite file we need to write this
            file.close();
        }
        std::ofstream file(filePath,std::ios::app);//append at the end of existing file
        if(!file.is_open()){std::cout<<"error opening the file "<<filePath<<std::endl;}
        file<<game.getMovesAsString()<<std::endl;
        file<<game.getSegmentThatCrashAsString()<<std::endl;
        file.close();
    }
    if(!stfu) {
        std::cout<<"positions saved at "<<filePath<<std::endl;
    }
}

void AIPlayer::saveDecisionGridToFile(const int generation) const {
    std::stringstream ss;//handle conversion from int to string
    ss<<"AI/brains/AI"<<"-gen"<<generation<<".bigBrain";
    std::string filePath=ss.str();
    std::ofstream file(filePath);// Create and open a text file
    if(!file.is_open()){std::cout<<"error creating the file "<<filePath<<std::endl;}
    file.write(&decisionGrid[0][0][0][0][0][0],DIRECTION_SENSOR_RESOLUTION*DIRECTION_SENSOR_RESOLUTION*DIRECTION_SENSOR_RESOLUTION*ANGLES_RESOLUTION*MAX_SPEED*DISTANCE_TO_FINISH_RESOLUTION*sizeof(char));
    file.close();
    std::cout<<"decisionGrid saved at "<<filePath<<std::endl;
}

void AIPlayer::loadDecisionGridFromFile(const std::string& filePath) {
    std::ifstream file(filePath);// read a text file
    if(!file.is_open()){std::cout<<"error reading the file "<<filePath<<std::endl;}
    file.read(&decisionGrid[0][0][0][0][0][0],DIRECTION_SENSOR_RESOLUTION*DIRECTION_SENSOR_RESOLUTION*DIRECTION_SENSOR_RESOLUTION*ANGLES_RESOLUTION*MAX_SPEED*DISTANCE_TO_FINISH_RESOLUTION*sizeof(char));
    std::cout<<"decisionGrid loaded from "<<filePath<<std::endl;
}