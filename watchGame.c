#include "watchGame.h"
#include "game.h"
#include "gameUtils.h"
#include "gui.h"
#include "menu.h"
#include "fileStream.h"
#include "conio2.h"

void watchGame()
{
  struct Game game;

  char gameId[GAME_ID_LENGTH];
  getGameId(gameId);

  loadSave(&game, gameId);
  printScreen(game);

  int cmd = getch();
  int actualTour = 1;
  while (cmd != 'q')
  {
    gameControls(cmd, &actualTour, game);

    clearTurn();
    if (game.state[actualTour-1].playerTour == 1)
      printTurn(game.player1Name);
    else
      printTurn(game.player2Name);
      
    cmd = getch();
  } 

  startMainMenu();
}

void gameControls(int cmd, int* actualTour, struct Game game)
{
  switch (cmd)
  {
    case 75: // left arrow
      if (*actualTour > 1)
      {
        *actualTour -= 1;
        printBoard(game.state[*actualTour-1].fields);
      }
      break;
    case 77: // right arrow
      if (*actualTour < game.tour)
      {
        *actualTour += 1;
        printBoard(game.state[*actualTour-1].fields);
      }
      break;
    case 'e':
      printBoard(game.state[game.tour-1].fields);
      *actualTour = game.tour;
      break;
    case 'b':
      printBoard(game.state->fields);
      *actualTour = game.tour;
      break;
  }
}

void printScreen(struct Game game)
{
  clrscr();
  printBoard(game.state->fields);
  printGameId(game.id);
  printWatchMode();
  printTurn(game.player1Name);
}