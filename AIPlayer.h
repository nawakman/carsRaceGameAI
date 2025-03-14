/*
// Created by samsam on 3/14/25.
*/

#ifndef PLAYER_H
#define PLAYER_H

// All actions are defined here, SLOWER means the speed decreases,
// KEEP means it stays the same, FASTER means increase
#define GO_LEFT_SLOWER 0
#define GO_LEFT_KEEP 1
#define GO_LEFT_FASTER 2
#define GO_FORWARD_SLOWER 3
#define GO_FORWARD_KEEP 4
#define GO_FORWARD_FASTER 5
#define GO_RIGHT_SLOWER 6
#define GO_RIGHT_KEEP 7
#define GO_RIGHT_FASTER 8

constexpr int CAPTEUR_DIRECTION_MAX_NB=9;
constexpr int ANGLES=8;
constexpr int NB_VITESSES=10;

/* Représente une IA qui joue au jeu.
 * Son tableau decisionGrid est de la forme :
 * decisionGrid[CAPTEUR_GAUCHE][CAPTEUR_DEVANT][CAPTEUR_DROITE][ANGLE][VITESSE] = DECISION_ASSOCIEE
 * par exemple, decisionGrid[3][4][1][0][8] = 2
 * signifie que pour ces valeurs de capteurs on fait l'action 2 ( voir les actions plus haut )
*/
class AIPlayer {
    void generateBullshitPlayer();
    public:
    int takeDecision();
    char decisionGrid[CAPTEUR_DIRECTION_MAX_NB][CAPTEUR_DIRECTION_MAX_NB][CAPTEUR_DIRECTION_MAX_NB][ANGLES][NB_VITESSES];
};



#endif //PLAYER_H
