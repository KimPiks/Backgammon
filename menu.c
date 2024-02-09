#include "menu.h"
#include "credits.h"
#include "game.h"
#include "watchGame.h"
#include "conio2.h"
#include <stdio.h>
#include "gui.h"

void startMainMenu()
{
  printMainMenu();

  int optionSelected = 0;
  
  while (!optionSelected)
  {
    char selectedOption = getch();

    switch (selectedOption)
    {
      case '1':
        startGame();
        optionSelected = 1;
        break;
      case '2':
        loadGame();
        optionSelected = 1;
        break;
      case '3':
        watchGame();
        optionSelected = 1;
        break;
      case '4':
        printCredits();
        showBackButton();
        optionSelected = 1;
        break;
      default:
        break;
    }
  }
}

void printMainMenu()
{
  clrscr();
  gotoxy(1, 1);
  printf("[1] Play\n");
  printf("[2] Load Game\n");
  printf("[3] Watch Game\n");
  printf("[4] Credits");
}

void showBackButton()
{
  gotoxy(1, 10);
  printf("[0] Go back to main menu");

  while (getch() != '0') {}

  startMainMenu();
}