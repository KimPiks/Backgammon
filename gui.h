#ifndef GUI_H
#define GUI_H

#define INITIAL_POS_X 10
#define INITIAL_POS_Y 5
#define BOARD_WIDTH 100
#define BOARD_HEIGHT 18
#define FIELD_HEIGHT 7

#define BOARD_RIGHT_SIDE_INITIAL_X (BOARD_WIDTH / 2) + 1

enum SelectPawnMode {
  OK,
  BAD,
  CHECKED
};

void printBoard(struct Field* board);
void printBorders();
void printHorizontalBorders();
void printVerticalBorders();
void printBar();
void printFieldNumbers();
void printFieldNumbersTop();
void printFieldNumbersBottom();
void printFields();
void printField(int fieldNumber, int initialX, int y);
int getFieldXPos(int field);
void printPawns(struct Field field);
void printTopPawns(struct Field field, int xPos);
void printBottomPawns(struct Field field, int xPos);
void printPawn(struct Field field, int initialX, int y);
void selectPawnPointer(int fieldNumber, enum SelectPawnMode selectMode);
void printGameId(char gameId[22]);
void printTurn(char* playerName);
void printRollTheDiceButton();
void clearRollTheDiceButton();
void clearResultsFromTheDice();
void printResultsFromTheDice(int dice1, int dice2, int bonus);
void clearPawnPointer(int fieldNumber);
void clearTurn();
void printBarPawns(int player, int pawns);
void printHomePawnsCount(struct Field* fields);
void printUI();
void printWatchMode();
void printForceBeating(int field);
void clearForceBeating();
void printWinScreen(char name[PLAYER_NAME_LENGTH], int points);
void startWinScreenMenu();

#endif