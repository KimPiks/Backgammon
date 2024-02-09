#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#include "game.h"

void setDefaultFieldsValues(struct Field* board);
void placePawnsOnTheBoard(struct Field* board);
void initializeBoard(struct Field* board);
struct Game initializeGame();
void generateGameId(char gameId[GAME_ID_LENGTH]);
void getPlayersNames(struct Game* game);
void getName(char playerName[PLAYER_NAME_LENGTH]);
void initializeNextTour(struct Game* game, int player);
void changePreviousField(struct Field* previousField);
void changeNextField(struct Field* nextField, struct Field* barPlayer1, struct Field* barPlayer2, int player);
int getFarestFromHome(struct Field* fields, int player);
int goodField(int player, int fieldNumber, int dice);
int canMove(int player, struct Field* fields, int fieldNumber, int selectedPawn);
int rollTheDice();
int correctSelectedPawnNumber(int selectedPawn);
int readyToGoHome(struct Field* fields, int player);
int countOfPawnsInBand(struct Field* fields, int player);
int possibleBonusField(struct Field* fields, int fieldNumber, int player, int selectedPawn, int dice1, int bonus);
int checkAllPositionsCanMove(struct Field* fields, int player, int selectedPawn, int dice, int maxI);
int possibleNonBonusField(struct Field* fields, int player, int fieldNumber, int selectedPawn, int dice1, int dice2);
int possibleDoubleNonBonusField(struct Field* fields, int fieldNumber, int player, int selectedPawn, int dice1, int dice2);
void getGameId(char gameId[GAME_ID_LENGTH]);
int hasPossibleMove(struct Field* fields, int player, int dice1, int dice2);
int hasPawnsInBand(struct Game* game, int player);
int hasPossibleMoveFromBand(struct Game* game, int player, int dice);
int enemyPossibleToBeat(struct Field* fields, int fieldNumber, int player);
int playerPawnAtPosition(struct Game* game, struct Field* fields, int fieldNumber, int player);
int possibleBeating(struct Game* game, struct Field* fields, int player, int dice1, int dice2, int bonus);
int playerPawnAtPositionCheck(struct Game* game, struct Field* fields, int dice1, int dice2, int player, int i, int bonus);
void updateBoard(struct Game* game, int selectedPawn, int moveTo, int player);
int canMoveHome(int player, int fieldNumber, int selectedPawn, struct Field* fields);

#endif