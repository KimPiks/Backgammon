#include "gameUtils.h"
#include "game.h"
#include "gui.h"
#include "fileStream.h"
#include <time.h>
#include <stdlib.h>
#include "conio2.h"

void setDefaultFieldsValues(struct Field* board)
{
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    board[i].number = i;
    board[i].fieldOwner = 0;
    board[i].numberOfPawns = 0;
  }
  board[0].fieldOwner = 3; // dwór
  board[25].fieldOwner = 4; // dwór
  board[26].fieldOwner = 3; // banda
  board[27].fieldOwner = 4; // banda
}

void placePawnsOnTheBoard(struct Field* board)
{
  board[1].fieldOwner = 2;
  board[1].numberOfPawns = 2;
  board[6].fieldOwner = 1;
  board[6].numberOfPawns = 5;
  board[8].fieldOwner = 1;
  board[8].numberOfPawns = 3;
  board[12].fieldOwner = 2;
  board[12].numberOfPawns = 5;
  board[13].fieldOwner = 1;
  board[13].numberOfPawns = 5;
  board[17].fieldOwner = 2;
  board[17].numberOfPawns = 3;
  board[19].fieldOwner = 2;
  board[19].numberOfPawns = 5;
  board[24].fieldOwner = 1;
  board[24].numberOfPawns = 2;
}

void initializeBoard(struct Field* board)
{
  setDefaultFieldsValues(board);
  placePawnsOnTheBoard(board);
}

struct Game initializeGame()
{
  struct Game game;
  game.state = malloc(sizeof(struct GameState));
  game.tour = 1;
  game.state->playerTour = 1;
  generateGameId(game.id);
  initializeBoard(game.state->fields);
  getPlayersNames(&game);
  printBoard(game.state->fields);
  printUI();
  printGameId(game.id);

  initSave(game.id, game.player1Name, game.player2Name);

  return game;
}

void getPlayersNames(struct Game* game)
{
  gotoxy(1, 1);
  clrscr();
  printf("Enter player1 name: ");
  getName(game->player1Name);

  gotoxy(1, 2);
  printf("Enter player2 name: ");
  getName(game->player2Name);

  clrscr();
}

void getName(char playerName[PLAYER_NAME_LENGTH])
{
  char name[PLAYER_NAME_LENGTH];
  int i = 0;
  while ((name[i] = getche()) != 13)
  {
    i++;
    if (i == PLAYER_NAME_LENGTH)
      break;
  }
  name[i] = '\0';
  int y = 0;
  for (y = 0; y < i; y++)
  {
    playerName[y] = name[y];
  }
  playerName[y] = '\0';
}

void generateGameId(char gameId[GAME_ID_LENGTH])
{
  time_t currentTime = time(0);
  struct tm *lc = localtime(&currentTime);
  strftime(gameId, GAME_ID_LENGTH, "%Y-%m-%d-%H-%M-", lc);
  gameId[GAME_ID_LENGTH-5] = rand() % (57 - 48 + 1) + 48; // losowanie liczb z przedziału 0-9
  gameId[GAME_ID_LENGTH-4] = rand() % (57 - 48 + 1) + 48;
  gameId[GAME_ID_LENGTH-3] = rand() % (57 - 48 + 1) + 48;
  gameId[GAME_ID_LENGTH-2] = rand() % (57 - 48 + 1) + 48;
  gameId[GAME_ID_LENGTH-1] = '\0';
}

void initializeNextTour(struct Game* game, int player)
{
  game->tour += 1;
  game->state = realloc(game->state, game->tour * sizeof(struct GameState));
  game->state[game->tour-1].playerTour = player;

  int i;
  for (i = 0; i < BOARD_SIZE; i++)
  {
    game->state[game->tour-1].fields[i] = game->state[game->tour-2].fields[i];
  }
}

void changePreviousField(struct Field* previousField)
{
  previousField->numberOfPawns -= 1;
  if (previousField->numberOfPawns == 0)
    previousField->fieldOwner = 0;
}

void changeNextField(struct Field* nextField, struct Field* barPlayer1, struct Field* barPlayer2, int player)
{
  if (nextField->fieldOwner != player)
  {
    if (nextField->fieldOwner == 1)
      barPlayer1->numberOfPawns += nextField->numberOfPawns;
    else if (nextField->fieldOwner == 2)
      barPlayer2->numberOfPawns += nextField->numberOfPawns;

    if (nextField->fieldOwner < 3)
    {
      nextField->fieldOwner = player;
      nextField->numberOfPawns = 0;
    }
  }

  nextField->numberOfPawns += 1;
}

int getFarestFromHome(struct Field* fields, int player)
{
  int i;
  if (player == 1)
  {
    for (i = 6; i > 0; i--)
    {
      if (fields[i].fieldOwner == player)
      {
        return i;
      }
    }
  }
  else
  {
    for (i = 19; i < 25; i++)
    {
      if (fields[i].fieldOwner == player)
      {
        return i;
      }
    }
  }
  return 0;
}

int canMove(int player, struct Field* fields, int fieldNumber, int selectedPawn)
{
  struct Field field;
  if (fieldNumber > 0 && fieldNumber < 25)
  {
    field = fields[fieldNumber];
    if ((field.fieldOwner == player || field.fieldOwner == 0))
      return 1;
  }

  if (readyToGoHome(fields, player) == 1)
  {
    if (canMoveHome(player, fieldNumber, selectedPawn, fields) == 1)
    {
      return 1;
    }
  }
  
  // przeciwnik jest na polu
  if (fieldNumber < 25 && fieldNumber > 0)
  {
    if (field.numberOfPawns == 1)
    return 1;
  }

  return 0;
}

int canMoveHome(int player, int fieldNumber, int selectedPawn, struct Field* fields)
{
  if (player == 1)
  {
    if (fieldNumber == 0)
    {
      return 1;
    }
    else if (fieldNumber < 0 && selectedPawn == getFarestFromHome(fields, player))
    {
      return 1;
    }
  }
  else if (player == 2)
  {
    if (fieldNumber == 25)
    {
      return 1;
    }
    else if (fieldNumber > 25 && selectedPawn == getFarestFromHome(fields, player))
    {
      return 1;
    }
  }
  return 0;
}

int goodField(int player, int pawnFieldNumber, int dice)
{
  // ustawienie pozycji startowej jeśli są pionki na bandzie
  if (pawnFieldNumber == 26)
    pawnFieldNumber = 25;
  else if (pawnFieldNumber == 27)
    pawnFieldNumber = 0;

  if (player == 1)
    return pawnFieldNumber - dice;
  else
    return pawnFieldNumber + dice;
}

int rollTheDice()
{
  return rand() % 6 + 1;
}

int correctSelectedPawnNumber(int selectedPawn)
{
  if (selectedPawn == 26)
    return 24;
  else if (selectedPawn == 27)
    return 1;
  return selectedPawn;
}

int readyToGoHome(struct Field* fields, int player)
{
  int i;

  if (player == 1)
  {
    for (i = 7; i <= 24; i++)
    {
      if (fields[i].fieldOwner == player)
      {
        return 0;
      }
    }
  }
  else
  {
    for (i = 1; i <= 18; i++)
    {
      if (fields[i].fieldOwner == player)
        return 0;
    }
  }
  return 1;
}

int countOfPawnsInBand(struct Field* fields, int player)
{
  if (player == 1)
    return fields[26].numberOfPawns;
  else
    return fields[27].numberOfPawns;
}

int possibleField(struct Field* fields, int player, int selectedPawn, int fieldNumber, int dice1, int dice2, int bonus)
{
  int bandNumber = selectedPawn;
  if (selectedPawn == 26)
    selectedPawn = 25;
  else if (selectedPawn == 27)
    selectedPawn = 0;
  
  int check = possibleNonBonusField(fields, player, fieldNumber, selectedPawn, dice1, dice2);
  if (check == 1)
    return 1;
  return possibleBonusField(fields, fieldNumber, player, selectedPawn, dice1, bonus);
}

int possibleNonBonusField(struct Field* fields, int player, int fieldNumber, int selectedPawn, int dice1, int dice2)
{
  if ((dice1 != 0) 
    && (fieldNumber == goodField(player, selectedPawn, dice1))
    && canMove(player, fields, goodField(player, selectedPawn, dice1), selectedPawn))
    return 1;
  if (dice2 != 0 
    && fieldNumber == goodField(player, selectedPawn, dice2)
    && canMove(player, fields, goodField(player, selectedPawn, dice2), selectedPawn))
    return 1;
  int check = possibleDoubleNonBonusField(fields, fieldNumber, player, selectedPawn, dice1, dice2);
  if (check == 1)
    return 1;

  return 0;
}

int possibleDoubleNonBonusField(struct Field* fields, int fieldNumber, int player, int selectedPawn, int dice1, int dice2)
{
  if ((dice1 != 0 && dice2 != 0) && fieldNumber == goodField(player, selectedPawn, dice1+dice2)
    && countOfPawnsInBand(fields, player) <= 1)
  {
    if ((canMove(player, fields, goodField(player, selectedPawn, dice1), selectedPawn) 
      || canMove(player, fields, goodField(player, selectedPawn, dice2), selectedPawn))
      && canMove(player, fields, goodField(player, selectedPawn, dice1+dice2), selectedPawn))
      return 1;
  }
  return 0;
}

int possibleBonusField(struct Field* fields, int fieldNumber, int player, int selectedPawn, int dice1, int bonus)
{
  if (bonus > 0)
  {
    if (fieldNumber == goodField(player, selectedPawn, 3*dice1)
        && checkAllPositionsCanMove(fields, player, selectedPawn, dice1, 3)
        && countOfPawnsInBand(fields, player) <= 1)
        return 1;
    if (bonus == 2 && fieldNumber == goodField(player, selectedPawn, 4*dice1)
        && checkAllPositionsCanMove(fields, player, selectedPawn, dice1, 4)
        && countOfPawnsInBand(fields, player) <= 1)
        return 1;
  }
  return 0;
}

int checkAllPositionsCanMove(struct Field* fields, int player, int selectedPawn, int dice, int maxI)
{
  int i;
  for (i = 0; i < maxI; i++)
    if (canMove(player, fields, goodField(player, selectedPawn, i*dice), selectedPawn) == 0)
      return 0;
  return 1;
}

void getGameId(char gameId[GAME_ID_LENGTH])
{
  clrscr();
  gotoxy(1, 1);
  printf("Enter gameId: ");

  int i = 0;
  while ((gameId[i] = getche()) != 13)
  {
    i++;
    if (i == GAME_ID_LENGTH)
      break;
  }
  gameId[i] = '\0';
}

int hasPossibleMoveFromBand(struct Game* game, int player, int dice)
{
  if (dice == 0)
    return 0;

  if (player == 1 
      && (game->state[game->tour-1].fields[25-dice].fieldOwner == player
      || game->state[game->tour-1].fields[25-dice].numberOfPawns <= 1))
      return 1;
  else if (player == 2
      && (game->state[game->tour-1].fields[dice].fieldOwner == player
      || game->state[game->tour-1].fields[dice].numberOfPawns <= 1))
      return 1;

  return 0;
}

int hasPossibleMove(struct Field* fields, int player, int dice1, int dice2)
{
  if (dice1 != 0)
  {
    int i;
    for (i = 1; i <= 24; i++)
    {
      if (fields[i].fieldOwner != player)
        continue;

      if (canMove(player, fields, goodField(player, i, dice1), i) == 1)
        return 1;
    }
  }
  else if (dice2 != 0)
  {
    int i;
    for (i = 1; i <= 24; i++)
    {
      if (fields[i].fieldOwner != player)
        continue;

      if (canMove(player, fields, goodField(player, i, dice2), i) == 1)
        return 1;
    }
  }
  return 0;
}

int hasPawnsInBand(struct Game* game, int player)
{
  if (player == 1)
  {
    if (game->state[game->tour-1].fields[26].numberOfPawns == 0)
      return 0;
    else
      return 1;
  }
  else
  {
    if (game->state[game->tour-1].fields[27].numberOfPawns == 0)
      return 0;
    else
      return 1;
  }
}

int possibleBeating(struct Game* game, struct Field* fields, int player, int dice1, int dice2, int bonus)
{
  int i;

  if (player == 1)
  {
    for (i = 24; i > 0; i--)
    {
      if (enemyPossibleToBeat(fields, i, player) == 0)
        continue;

      if (playerPawnAtPositionCheck(game, fields, dice1, dice2, player, i, bonus) == 1)
      {
        if (hasPawnsInBand(game, player) == 1)
        {
          if (i < 25 && i > 18)
            return i;
          return 0;
        }
        return i;
      }
    }
  }
  else
  {
    for (i = 0; i < 25; i++)
    {
      if (enemyPossibleToBeat(fields, i, player) == 0)
        continue;

      if (playerPawnAtPositionCheck(game, fields, dice1, dice2, player, i, bonus) == 1)
      {
        if (hasPawnsInBand(game, player) == 1)
        {
          if (i > 0 && i < 7)
            return i;
          return 0;
        }
        return i;
      }
    }
  }
  return 0;
}

int enemyPossibleToBeat(struct Field* fields, int fieldNumber, int player)
{
  if (fields[fieldNumber].fieldOwner == player)
    return 0;

  if (fields[fieldNumber].numberOfPawns == 1)
    return 1;

  return 0;
}

int playerPawnAtPositionCheck(struct Game* game, struct Field* fields, int dice1, int dice2, int player, int i, int bonus)
{
  if (playerPawnAtPosition(game, fields, goodField(player, i, -dice1), player) == 1
    || playerPawnAtPosition(game, fields, goodField(player, i, -dice2), player) == 1
    || playerPawnAtPosition(game, fields, goodField(player, i, -(dice1+dice2)), player) == 1) 
    return 1;

  if (bonus == 1 && playerPawnAtPosition(game, fields, goodField(player, i, -3*dice1), player) == 1)
    return 1;

  if (bonus == 2 && playerPawnAtPosition(game, fields, goodField(player, i, -4*dice1), player) == 1)
    return 1;
  return 0;
}

// sprawdza czy pionek gracza znajduje sie na danym polu
int playerPawnAtPosition(struct Game* game, struct Field* fields, int fieldNumber, int player)
{
  if (fields[fieldNumber].fieldOwner == player)
    return 1;

  if (hasPawnsInBand(game, player) == 1)
  {
    if (player == 1 && fieldNumber == 25)
      return 1;
    else if (player == 2 && fieldNumber == 0)
      return 1;
  }

  return 0;
}

void updateBoard(struct Game* game, int selectedPawn, int moveTo, int player)
{
  if (moveTo > 25)
    moveTo = 25;
  else if (moveTo < 0)
    moveTo = 0;

  struct Field* previousField = &(game->state[game->tour-1].fields[selectedPawn]);
  struct Field* nextField = &(game->state[game->tour-1].fields[moveTo]);
  struct Field* barPlayer1 = &(game->state[game->tour-1].fields[26]);
  struct Field* barPlayer2 = &(game->state[game->tour-1].fields[27]);

  changePreviousField(previousField);
  changeNextField(nextField, barPlayer1, barPlayer2, player);
}