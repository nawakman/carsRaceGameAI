/*
// Created by samsam on 3/14/25.
*/

#include "AIPlayer.h"

#include <iostream>
#include <algorithm>
#include <cstdlib>

// Full random player
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
void AIPlayer::MovePlayer(int chosenMove){
    std::array<int,2> scannedTile=position;
    //WIP
  }

std::array<int,2> AIPlayer::GetNextDirectionFromDecision(int decision){
      std::array<int,2> newDirection;
      int angleOffset=90*((decision%3)-1);//WIP
      /*switch((decision%3)-1){
        case 0://left
          return {};
        }*/
  }
int AIPlayer::GetNextSpeedFromDecision(int decision){
    int minSpeed=std::max(1,speed-1);//at no point your next speed should be 0
    int speedOffset=(decision/3)-1;
    if((speedOffset==-1 && (speed==0 || speed==1)) || (speedOffset==1 && speed==NB_SPEEDS)){
        std::cout<<"AI tried to go slower than (1 or 0) or faster than NB_SPEEDS , shouldn't happen since you are supposed to remove illegal moves"<<std::endl;
    }
    return std::clamp(speed+speedOffset,minSpeed,NB_SPEEDS);//limit speed
}


int AIPlayer::GetSpeed() {
    return speed;
}
void AIPlayer::SetSpeed(int x) {
    speed=x;
}