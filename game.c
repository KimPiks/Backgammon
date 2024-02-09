#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "gui.h"
#include "conio2.h"
#include "fileStream.h"
#include "menu.h"
#include "gameUtils.h"

void startGame()
{
  struct Game game = initializeGame();
  int playerTurn = rand() % 2 + 1;

  while (1)
  {
    playTurn(&game, playerTurn);

    if (win(&game, playerTurn) == 1)
    {
      finishGame(game, playerTurn);
      break;
    }

    if (playerTurn == 1)
      playerTurn = 2;
    else
      playerTurn = 1;
  }
}

void loadGame()
{
  struct Game game;

  char gameId[GAME_ID_LENGTH];
  getGameId(gameId);

  loadSave(&game, gameId);
  clrscr();
  printBoard(game.state[game.tour - 1].fields);
  printUI();
  printGameId(game.id);

  int playerTurn = game.state[game.tour - 1].playerTour;

  while (1)
  {
    playTurn(&game, playerTurn);

    if (playerTurn == 1)
      playerTurn = 2;
    else
      playerTurn = 1;
  }
}

void finishGame(struct Game game, int player)
{
  int points = 1;
  struct GameState gs = game.state[game.tour-1];

  if (player == 1)
  {
    if (gs.fields[25].numberOfPawns == 0)
    {
      points += 1;
    }
    if (gs.fields[27].numberOfPawns > 0)
    {
      points = 3;
    }
    addWinner(game.player1Name, points);
    printWinScreen(game.player1Name, points);
  }
  else if (player == 2)
  {
    if (gs.fields[0].numberOfPawns == 0)
    {
      points += 1;
    }
    if (gs.fields[26].numberOfPawns > 0)
    {
      points = 3;
    }
    addWinner(game.player1Name, points);
    printWinScreen(game.player2Name, points);
  }
}

void playTurn(struct Game *game, int player)
{
  initializeNextTour(game, player);
  autoSave(game->id, game);
  printRollTheDiceButton();

  if (player == 1)
    printTurn((*game).player1Name);
  else
    printTurn((*game).player2Name);

  while (getch() != 32)
  {
  } // wait for space

  int dice1 = rollTheDice();
  int dice2 = rollTheDice();

  int bonus = 0;
  if (dice1 == dice2)
    bonus = 2;

  clearRollTheDiceButton();
  printResultsFromTheDice(dice1, dice2, bonus);

  while (dice1 != 0 || dice2 != 0)
  {
    playPawn(game, player, &dice1, &dice2, &bonus);

    if (win(game, player) == 1)
    {
      finishGame(*game, player);
      return;
    }
  }

  clearResultsFromTheDice();
  clearTurn();
}

int win(struct Game *game, int player)
{
  int i;
  for (i = 1; i < 25; i++)
  {
    if (game->state[game->tour - 1].fields[i].fieldOwner == player)
      return 0;
  }
  if (hasPawnsInBand(game, player) == 1)
    return 0;
  return 1;
}

void playPawn(struct Game *game, int player, int *dice1, int *dice2, int *bonus)
{
  if (hasPawnsInBand(game, player) == 0)
  {
    if (hasPossibleMove(game->state[game->tour - 1].fields, player, *dice1, *dice2) == 0 && readyToGoHome(game->state[game->tour - 1].fields, player) == 0)
    {
      *dice1 = 0;
      *dice2 = 0;
      return;
    }
    movePawn(player, game, dice1, dice2, bonus, 0);
  }
  else
  {
    if (hasPossibleMoveFromBand(game, player, *dice1) == 0 && hasPossibleMoveFromBand(game, player, *dice2) == 0)
    {
      *dice1 = 0;
      *dice2 = 0;
      return;
    }

    movePawn(player, game, dice1, dice2, bonus, 1);
  }

  // avoid double tour
  if (*dice1 == 0 && *dice2 == 0)
    return;

  initializeNextTour(game, player);
}

void movePawn(int player, struct Game *game, int *dice1, int *dice2, int *bonus, int defaultSelectedPawn)
{
  checkBeating(game, player, *dice1, *dice2, *bonus, 0);

  int sp = selectPawnChoise(defaultSelectedPawn, player, game);
  int mtp = selectPosition(player, game, game->state[game->tour - 1].fields, sp, *dice1, *dice2, *bonus);

  if (sp == mtp)
  {
    movePawn(player, game, dice1, dice2, bonus, 0);
    return;
  }

  if (mtp > 25)
    mtp = 25;
  else if (mtp < 0)
    mtp = 0;

  if (*bonus > 0)
    moveToBonus(game, sp, mtp, player, dice1, dice2, bonus);
  else
    moveToNonBonus(game, sp, mtp, player, dice1, dice2);

  printAfterMove(game, dice1, dice2, bonus);
  clearForceBeating();
}

void printAfterMove(struct Game *game, int *dice1, int *dice2, int *bonus)
{
  clearResultsFromTheDice();
  printResultsFromTheDice(*dice1, *dice2, *bonus);
  printBoard(game->state[game->tour - 1].fields);
}

int selectPawnChoise(int defaultSelectedPawn, int player, struct Game *game)
{
  if (defaultSelectedPawn == 1)
    if (player == 1)
      return 26;
    else
      return 27;
  else
    return selectPawn(player, game->state[game->tour - 1].fields);
}

void moveToBonus(struct Game *game, int selectedPawn, int moveToPosition, int player, int *dice1, int *dice2, int *bonus)
{
  int tempPos;
  while (selectedPawn != moveToPosition)
  {
    tempPos = goodField(player, selectedPawn, *dice1);
    *bonus -= 1;

    updateBoard(game, selectedPawn, tempPos, player);
    selectedPawn = tempPos;
  }

  if (*bonus == -1)
    *dice2 = 0;
  if (*bonus == -2)
  {
    *dice1 = 0;
    *dice2 = 0;
  }
  if (*bonus < 0)
    *bonus = 0;
}

void moveToNonBonus(struct Game *game, int selectedPawn, int moveToPos, int player, int *dice1, int *dice2)
{
  int gfd1 = goodField(player, selectedPawn, *dice1);
  int gfd2 = goodField(player, selectedPawn, *dice2);
  struct GameState gs = game->state[game->tour-1];

  if (gfd2 == moveToPos)
    *dice2 = 0;
  else if (gfd1 == moveToPos)
    *dice1 = 0;
  else if (farestPawnGoHome(gs, player, selectedPawn, moveToPos) == 1)
    resetBiggerDice(dice1, dice2);
  else if (canMove(player,gs.fields, gfd2, selectedPawn))
  {
    move2Moves2Dice(game, selectedPawn, gfd2, player, dice1, dice2);
    return;
  }
  else
  {
    move2Moves1Dice(game, selectedPawn, gfd1, player, dice1, dice2);
    return;
  }

  updateBoard(game, selectedPawn, moveToPos, player);
}

void resetBiggerDice(int* dice1, int* dice2)
{
  if (*dice1 > *dice2)
    *dice1 = 0;
  else
    *dice2 = 0;
}

int farestPawnGoHome(struct GameState gs, int player, int selectedPawn, int moveToPos)
{
  if ((moveToPos == 0 || moveToPos == 25) && selectedPawn == getFarestFromHome(gs.fields, player))
    return 1;
  return 0;
}

void move2Moves1Dice(struct Game *game, int selectedPawn, int gfd1, int player, int *dice1, int *dice2)
{
  int gfd12 = goodField(player, gfd1, *dice2);

  updateBoard(game, selectedPawn, gfd1, player);
  updateBoard(game, gfd1, gfd12, player);
  *dice1 = 0;
  *dice2 = 0;
}

void move2Moves2Dice(struct Game *game, int selectedPawn, int gfd2, int player, int *dice1, int *dice2)
{
  int gfd21 = goodField(player, gfd2, *dice1);

  updateBoard(game, selectedPawn, gfd2, player);
  updateBoard(game, gfd2, gfd21, player);
  *dice1 = 0;
  *dice2 = 0;
}

int selectPosition(int player, struct Game *game, struct Field *fields, int selectedPawn, int dice1, int dice2, int bonus)
{
  int fieldNumber = correctSelectedPawnNumber(selectedPawn);

  enum SelectPawnMode pawnMode = OK;
  while (1)
  {
    pawnMode = setPawnMode(game, fields, selectedPawn, fieldNumber, player, dice1, dice2, bonus);

    selectPawnPointer(fieldNumber, pawnMode);

    int m = getch();
    if (m != 13)
    {
      makeMove(m, &fieldNumber);
      continue;
    }

    if (pawnMode != BAD)
    {
      clearPawnPointer(fieldNumber);
      return fieldNumber;
    }
  }
  return 0;
}

enum SelectPawnMode setPawnMode(struct Game *game, struct Field *fields, int selectedPawn, int fieldNumber, int player, int dice1, int dice2, int bonus)
{
  if (samePawn(selectedPawn, fieldNumber) == 1)
    return CHECKED;
  else if (possibleToGo(fields, player, selectedPawn, fieldNumber, dice1, dice2, bonus) == 1)
  {
    enum SelectPawnMode beat = checkBeating(game, player, dice1, dice2, bonus, fieldNumber);
    if (beat == BAD && hasPawnsInBand(game, player) > 0)
      return OK;
    return beat;
  }

  if ((player == 1 && fieldNumber == 0 && readyToGoHome(fields, player) == 1) || (player == 2 && fieldNumber == 25 && readyToGoHome(fields, player) == 1))
  {
    return checkGoingHome(fields, player, selectedPawn, dice1, dice2);
  }

  return BAD;
}

int samePawn(int selectedPawn, int fieldNumber)
{
  if (fieldNumber == selectedPawn && selectedPawn < 25 && selectedPawn > 0)
    return 1;
  return 0;
}

int possibleToGo(struct Field *fields, int player, int selectedPawn, int fieldNumber, int dice1, int dice2, int bonus)
{
  if (possibleField(fields, player, selectedPawn, fieldNumber, dice1, dice2, bonus) == 1 && fieldNumber > 0 && fieldNumber < 25)
    return 1;
  return 0;
}

enum SelectPawnMode checkBeating(struct Game *game, int player, int dice1, int dice2, int bonus, int fieldNumber)
{
  int beat = possibleBeating(game, game->state[game->tour - 1].fields, player, dice1, dice2, bonus);
  if (beat == 0)
  {
    return OK;
  }
  else
  {
    printForceBeating(beat);
    if (beat == fieldNumber)
      return OK;
    else
      return BAD;
  }
}

enum SelectPawnMode checkGoingHome(struct Field *fields, int player, int selectedPawn, int dice1, int dice2)
{
  int homeField;
  if (player == 1)
    homeField = 0;
  else
    homeField = 25;

  int dice1Check = checkGoingHomeDice(fields, player, selectedPawn, homeField, dice1);
  if (dice1Check == 1)
    return OK;

  int dice2Check = checkGoingHomeDice(fields, player, selectedPawn, homeField, dice2);
  if (dice2Check == 1)
    return OK;

  return BAD;
}

int checkGoingHomeDice(struct Field *fields, int player, int selectedPawn, int homeField, int dice)
{
  if (compareHomeField(player, goodField(player, selectedPawn, dice), homeField) == 1)
  {
    if (dice != 0 && canMove(player, fields, goodField(player, selectedPawn, dice), selectedPawn) == 1)
    {
      return 1;
    }
  }
  else if (goodField(player, selectedPawn, dice) == homeField)
  {
    return 1;
  }
  return 0;
}

int compareHomeField(int player, int gf, int homeField)
{
  if (player == 1)
  {
    if (gf < homeField)
      return 1;
    else
      return 0;
  }
  else
  {
    if (gf > homeField)
      return 1;
    else
      return 0;
  }
}

int selectPawn(int player, struct Field *fields)
{
  int fieldNumber = 1;
  enum SelectPawnMode pawnMode = OK;
  while (1)
  {
    if (fields[fieldNumber].fieldOwner == player)
      pawnMode = OK;
    else
      pawnMode = BAD;

    selectPawnPointer(fieldNumber, pawnMode);

    int move = getch();
    if (move != 13)
    {
      makeMove(move, &fieldNumber);
      continue;
    }

    if (pawnMode == OK)
    {
      clearPawnPointer(fieldNumber);
      return fieldNumber;
    }
  }
  return 0;
}

void makeMove(int move, int *actualFieldNumber)
{
  clearPawnPointer(*actualFieldNumber);

  switch (move)
  {
  case 72:
  case 80: // up, down arrow
    if (*actualFieldNumber <= 12)
      *actualFieldNumber = 25 - *actualFieldNumber;
    else if (*actualFieldNumber >= 13)
      *actualFieldNumber = 25 - *actualFieldNumber;
    break;
  case 75: // left arrow
    if (*actualFieldNumber < 12)
      *actualFieldNumber += 1;
    else if (*actualFieldNumber > 13)
      *actualFieldNumber -= 1;
    break;
  case 77: // right arrow
    if (*actualFieldNumber < 25 && *actualFieldNumber >= 13)
      *actualFieldNumber += 1;
    else if (*actualFieldNumber > 0 && *actualFieldNumber <= 12)
      *actualFieldNumber -= 1;
    break;
  }
}