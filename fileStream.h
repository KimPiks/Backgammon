#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "game.h"
#include <stdio.h>

#define LINE_SIZE 2*4*BOARD_SIZE+3

void initSave(char gameId[GAME_ID_LENGTH], char player1Name[PLAYER_NAME_LENGTH], char player2Name[PLAYER_NAME_LENGTH]);
void autoSave(char gameId[GAME_ID_LENGTH], struct Game* game);
void loadSave(struct Game* game, char gameId[GAME_ID_LENGTH]);
void copyGameId(struct Game* game, char gameId[GAME_ID_LENGTH]);
void setUpField(struct Field* field, int fieldId, char line[4*BOARD_SIZE+3], int i);
FILE* openFile(FILE* f, char gameId[GAME_ID_LENGTH], char* mode);
void copyName(struct Game* game, char name[PLAYER_NAME_LENGTH], int player);
int sameTurn(char gameId[GAME_ID_LENGTH], struct Game* gameToSave);
void setUpGame(struct Game* game, char gameId[GAME_ID_LENGTH]);
void addWinner(char playerName[PLAYER_NAME_LENGTH], int points);

#endif