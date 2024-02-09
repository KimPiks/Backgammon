#include "fileStream.h"
#include "game.h"
#include "gameUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include "conio2.h"

void initSave(char gameId[GAME_ID_LENGTH], char player1Name[PLAYER_NAME_LENGTH], char player2Name[PLAYER_NAME_LENGTH])
{
  FILE* f = openFile(f, gameId, "w");

  fprintf(f, "%s\n", player1Name);
  fprintf(f, "%s\n", player2Name);

  fclose(f);
}

void addWinner(char playerName[PLAYER_NAME_LENGTH], int points)
{
  FILE* f = fopen("winners.txt", "a");

  fprintf(f, "%s", playerName);
  fprintf(f, "%d\n", points);
  fclose(f);
}

void autoSave(char gameId[GAME_ID_LENGTH], struct Game* game)
{
  if (sameTurn(gameId, game) == 1)
    return;

  FILE* f = openFile(f, gameId, "a");

  fprintf(f, "%d ", game->state[game->tour-1].playerTour);
  
  int i;
  for (i = 0; i < BOARD_SIZE; i++)
  {
    fprintf(f, "%d ", game->state[game->tour-1].fields[i].fieldOwner);
    fprintf(f, "%d ", game->state[game->tour-1].fields[i].numberOfPawns);
  }

  fprintf(f, "\n");

  fclose(f);
}

int sameTurn(char gameId[GAME_ID_LENGTH], struct Game* gameToSave)
{
  struct Game game;
  loadSave(&game, gameId);

  if (gameToSave->state[gameToSave->tour-1].playerTour != game.state[game.tour-1].playerTour)
    return 0;

  int i;
  for (i = 0; i < BOARD_SIZE; i++)
  {
    if (gameToSave->state[gameToSave->tour-1].fields[i].fieldOwner != game.state[game.tour-1].fields[i].fieldOwner)
      return 0;

    if (gameToSave->state[gameToSave->tour-1].fields[i].numberOfPawns != game.state[game.tour-1].fields[i].numberOfPawns)
      return 0;
  }

  return 1;
}

void loadSave(struct Game* game, char gameId[GAME_ID_LENGTH])
{
  FILE* f = openFile(f, gameId, "r");

  setUpGame(game, gameId);
    
  char line[LINE_SIZE];
  int i;
  int fieldId = 0;
  int lineNum = 1;
  while (fgets(line, LINE_SIZE, f))
  {
    if (lineNum == 1 || lineNum == 2)
    {
      copyName(game, line, lineNum);
      lineNum += 1;
      continue;
    }

    int p = line[0]-'0';

    if (p != 1 && p != 2)
      continue;

    fieldId = 0;
    initializeNextTour(game, p);
    game->state[game->tour-1].playerTour = p;

    for (i = 2; i < LINE_SIZE-2; i+=4)
    {
      setUpField(&(game->state[game->tour-1].fields[fieldId]), fieldId, line, i);
      fieldId += 1;

      if (line[i+3] != 32)
        i += 1;

      if (fieldId == BOARD_SIZE)
        break;
    }
  }

  fclose(f);
}

FILE* openFile(FILE* f, char gameId[GAME_ID_LENGTH], char* mode)
{
  char fileName[GAME_ID_LENGTH+5];
  sprintf(fileName, "%s.txt", gameId);

  return fopen(fileName, mode);
}

void setUpGame(struct Game* game, char gameId[GAME_ID_LENGTH])
{
  copyGameId(game, gameId);

  game->state = malloc(0);
  game->tour = 0;
}

void setUpField(struct Field* field, int fieldId, char line[4*BOARD_SIZE+3], int i)
{
  (*field).number = fieldId;
  (*field).fieldOwner = line[i]-'0';
  if (line[i+3] != 32)
    (*field).numberOfPawns = (line[i+2]-'0')*10 + line[i+3]-'0';
  else
    (*field).numberOfPawns = line[i+2]-'0';
}

void copyGameId(struct Game* game, char gameId[GAME_ID_LENGTH])
{
  int i;
  for (i = 0; i < GAME_ID_LENGTH; i++)
    game->id[i] = gameId[i];
}

void copyName(struct Game* game, char name[PLAYER_NAME_LENGTH], int player)
{
  int i;
  if (player == 1)
    for (i = 0; i < PLAYER_NAME_LENGTH; i++)
      game->player1Name[i] = name[i];
  else
    for (i = 0; i < PLAYER_NAME_LENGTH; i++)
      game->player2Name[i] = name[i];
}