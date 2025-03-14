/*
// Created by samsam on 3/14/25.
*/

#include "AIPlayer.h"

#include <cstdlib>

// Full random player
void AIPlayer::generateBullshitPlayer() {
    for (int i=0; i<CAPTEUR_DIRECTION_MAX_NB; i++) {
        for (int j=0; j<CAPTEUR_DIRECTION_MAX_NB; j++) {
            for (int k=0; k<CAPTEUR_DIRECTION_MAX_NB; k++) {
                for (int l=0; l<ANGLES; l++) {
                    for (int m=0; m<NB_VITESSES; m++) {
                        decisionGrid[i][j][k][l][m] = rand() % 9;
                    }
                }
            }
        }
    }
}

