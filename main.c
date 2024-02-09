#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conio2.h"
#include "game.h"
#include "gui.h"
#include "menu.h"
#include "credits.h"

int main() 
{
  Conio2_Init();
  srand(time(NULL));
  settitle("BACKGAMMON - Kamil Prorok");

  startMainMenu();
  
  return 0;
}
