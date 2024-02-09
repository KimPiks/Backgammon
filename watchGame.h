#ifndef WATCHGAME_H
#define WATCHGAME_H

#include "game.h"

void watchGame();
void printScreen(struct Game game);
void gameControls(int cmd, int* actualTour, struct Game game);

#endif