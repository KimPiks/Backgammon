#include "game.h"
#include "gui.h"
#include "conio2.h"
#include "menu.h"
#include <stdio.h>

void printBoard(struct Field* board)
{
  printBorders();
  printFields();
  printBarPawns(1, board[26].numberOfPawns);
  printBarPawns(2, board[27].numberOfPawns);
  printHomePawnsCount(board);

  for (int i = 1; i < 25; i++)
  {
    printPawns(board[i]);
  }
}

void printUI()
{
  gotoxy(3, BOARD_HEIGHT + INITIAL_POS_Y + 5);
  printf("Move: Arrows | Select: Enter");
}

void printForceBeating(int field)
{
  gotoxy(1, 5);
  textcolor(15);
  printf("*Beat %d", field);
}

void clearForceBeating()
{
  gotoxy(1, 5);
  printf("          ");
}

void printHomePawnsCount(struct Field* fields)
{
  gotoxy(BOARD_WIDTH + INITIAL_POS_X + 3, BOARD_HEIGHT / 2 + INITIAL_POS_Y - 2);
  printf("%d", fields[25].numberOfPawns);
  gotoxy(BOARD_WIDTH + INITIAL_POS_X + 1, BOARD_HEIGHT / 2 + INITIAL_POS_Y);
  printf("------");
  gotoxy(BOARD_WIDTH + INITIAL_POS_X + 1, BOARD_HEIGHT / 2 + INITIAL_POS_Y + 1);
  printf("------");
  gotoxy(BOARD_WIDTH + INITIAL_POS_X + 3, BOARD_HEIGHT / 2 + INITIAL_POS_Y + 3);
  printf("%d", fields[0].numberOfPawns);
}

void printPawns(struct Field field)
{
  int xPos = getFieldXPos(field.number);
  if (field.number >= 13)
  {
    printTopPawns(field, xPos);
  }
  else
  {
    printBottomPawns(field, xPos);
  }
}

void printTopPawns(struct Field field, int xPos)
{
  if (field.numberOfPawns > FIELD_HEIGHT)
  {
    gotoxy(xPos, INITIAL_POS_Y + FIELD_HEIGHT + 1);
    printf("+%d", field.numberOfPawns - FIELD_HEIGHT);
    field.numberOfPawns = FIELD_HEIGHT;
  }

  for (int i = INITIAL_POS_Y + 1; i < INITIAL_POS_Y + 1 + field.numberOfPawns; i++)
  {
    printPawn(field, xPos, i);
  }
}

void printBottomPawns(struct Field field, int xPos)
{
  if (field.numberOfPawns > FIELD_HEIGHT)
  {
    gotoxy(xPos, BOARD_HEIGHT + INITIAL_POS_Y - FIELD_HEIGHT);
    printf("+%d", field.numberOfPawns - FIELD_HEIGHT);
    field.numberOfPawns = FIELD_HEIGHT;
  }

  for (int i = BOARD_HEIGHT + INITIAL_POS_Y; i > BOARD_HEIGHT + INITIAL_POS_Y - field.numberOfPawns; i--)
  {
    printPawn(field, xPos, i);
  }
}

void printPawn(struct Field field, int initialX, int y)
{
  if (field.fieldOwner == 1)
  {
    textcolor(6);
    gotoxy(initialX, y);
    putch('X');
    gotoxy(initialX + 1, y);
    putch('X');
  }
  else
  {
    textcolor(11);
    gotoxy(initialX, y);
    putch('O');
    gotoxy(initialX + 1, y);
    putch('O');
  }
  textcolor(15);
}
void printFields()
{
  for (int i = 1; i <= 24; i++)
  {    
    int xPos = getFieldXPos(i);

    if (i >= 13)
    {
      for (int y = INITIAL_POS_Y + 1; y < INITIAL_POS_Y + FIELD_HEIGHT + 1; y++)
      {
        printField(i, xPos - 1, y);
      }
    }
    else
    {
      for (int y = BOARD_HEIGHT + INITIAL_POS_Y; y > BOARD_HEIGHT + INITIAL_POS_Y - FIELD_HEIGHT; y--)
      {
        printField(i, xPos - 1, y);
      }
    }
  }
}

void printField(int fieldNumber, int initialX, int y)
{
  char fieldIdentifierChar = '#';

  if (fieldNumber % 2 == 0)
  {
    fieldIdentifierChar = '@';
  }

  gotoxy(initialX, y);
  putch(fieldIdentifierChar);
  gotoxy(initialX + 1, y);
  printf("--");
  gotoxy(initialX + 3, y);
  putch(fieldIdentifierChar);
}

int getFieldXPos(int field)
{
  int exp = 0;
  int rightSide = 0; 
  if (field >= 19)
  {
    exp = field - 19;
    rightSide = 1;
  }
  else if (field >= 13)
    exp = field - 13;
  else if (field >= 7)
    exp = 12 - field;
  else
  {
    exp = 6 - field;
    rightSide = 1;
  }
  return ((rightSide) ? BOARD_RIGHT_SIDE_INITIAL_X : 0) + INITIAL_POS_X + (BOARD_WIDTH / 2) / 6 * exp + (BOARD_WIDTH / 2) / 12;
}

void printBorders()
{
  printHorizontalBorders();
  printVerticalBorders();
  printBar();
  printFieldNumbers();
}

void printHorizontalBorders()
{
  for (int i = INITIAL_POS_X; i <= BOARD_WIDTH + INITIAL_POS_X; i++)
  {
    gotoxy(i, INITIAL_POS_Y);
    putch('-');
  }

  for (int i = INITIAL_POS_X; i <= BOARD_WIDTH + INITIAL_POS_X; i++)
  {
    gotoxy(i, BOARD_HEIGHT + INITIAL_POS_Y + 1);
    putch('-');
  }
}

void printVerticalBorders()
{
  for (int i = INITIAL_POS_Y + 1; i <= BOARD_HEIGHT + INITIAL_POS_Y; i++)
  {
    gotoxy(INITIAL_POS_X, i);
    putch('|');
  }  
  for (int i = INITIAL_POS_Y + 1; i <= BOARD_HEIGHT + INITIAL_POS_Y; i++)
  {
    gotoxy(BOARD_WIDTH + INITIAL_POS_X, i);
    putch('|');
  }  
}

void printBar()
{
  for (int i = INITIAL_POS_Y + 1; i <= BOARD_HEIGHT + INITIAL_POS_Y; i++)
  {
    gotoxy(BOARD_WIDTH / 2 + INITIAL_POS_X - 2, i);
    putch('|');
  }
  for (int i = INITIAL_POS_Y + 1; i <= BOARD_HEIGHT + INITIAL_POS_Y; i++)
  {
    gotoxy(BOARD_WIDTH / 2 + INITIAL_POS_X + 2, i);
    putch('|');
  }

  gotoxy(BOARD_WIDTH / 2 + INITIAL_POS_X - 1, BOARD_HEIGHT / 2 + INITIAL_POS_Y);
  printf("BAR");
}

void printBarPawns(int player, int pawns)
{
  int yPos;
  if (player == 1)
    yPos = BOARD_HEIGHT / 2 + INITIAL_POS_Y - 5;
  else
    yPos = BOARD_HEIGHT / 2 + INITIAL_POS_Y + 5;

  gotoxy(BOARD_WIDTH / 2 + INITIAL_POS_X, yPos);
  printf("%d", pawns);
}

void printFieldNumbers()
{
  printFieldNumbersTop();
  printFieldNumbersBottom();
}

void printFieldNumbersTop()
{
  for (int i = 13; i <= 18; i++)
  {
    int xPos = getFieldXPos(i);
    gotoxy(xPos, INITIAL_POS_Y - 1);
    printf("%d", i);
  }

  for (int i = 19; i <= 24; i++)
  {
    int xPos = getFieldXPos(i);
    gotoxy(xPos, INITIAL_POS_Y - 1);
    printf("%d", i);
  }
}

void printFieldNumbersBottom()
{
  for (int i = 12; i >= 7; i--)
  {
    int xPos = getFieldXPos(i);
    gotoxy(xPos, BOARD_HEIGHT + INITIAL_POS_Y + 2);
    printf("%d", i);
  }

  for (int i = 6; i >= 1; i--)
  {
    int xPos = getFieldXPos(i);
    gotoxy(xPos, BOARD_HEIGHT + INITIAL_POS_Y + 2);
    printf("%d", i);
  }
}

void selectPawnPointer(int fieldNumber, enum SelectPawnMode selectMode)
{
  int xPos = getFieldXPos(fieldNumber);
  int yPos = 0;

  if (selectMode == OK)
  {
    textcolor(15);
  }
  else if (selectMode == BAD)
  {
    textcolor(4);
  }
  else if (selectMode == CHECKED)
  {
    textcolor(14);
  }

  if (fieldNumber <= 12)
  {
    yPos = BOARD_HEIGHT + INITIAL_POS_Y - FIELD_HEIGHT;
    gotoxy(xPos, yPos);
    printf("\\/");
  }
  else
  {
    yPos = INITIAL_POS_Y + FIELD_HEIGHT + 1;
    gotoxy(xPos, yPos);
    printf("/\\");
  }
}

void clearPawnPointer(int fieldNumber)
{
  int xPos = getFieldXPos(fieldNumber);
  int yPos = 0;
  if (fieldNumber <= 12)
  {
    yPos = BOARD_HEIGHT + INITIAL_POS_Y - FIELD_HEIGHT;
    gotoxy(xPos, yPos);
    printf("  ");
  }
  else
  {
    yPos = INITIAL_POS_Y + FIELD_HEIGHT + 1;
    gotoxy(xPos, yPos);
    printf("  ");
  } 
}

void printGameId(char gameId[GAME_ID_LENGTH])
{
  gotoxy(1, 1);
  printf("GameId: %s", gameId);
}

void printTurn(char playerName[PLAYER_NAME_LENGTH])
{
  gotoxy(BOARD_WIDTH / 2 + INITIAL_POS_X + 4, 1);
  printf("Now it's your turn: %s", playerName);
}

void clearTurn()
{
  gotoxy(BOARD_WIDTH / 2 + INITIAL_POS_X + 4, 1);
  int i;
  for (i = 0; i < PLAYER_NAME_LENGTH; i++)
    printf(" ");
}

void printRollTheDiceButton()
{
  gotoxy(BOARD_WIDTH / 2 + INITIAL_POS_X - 3, BOARD_HEIGHT + INITIAL_POS_Y + 3);
  printf("[SPACE]");
}

void clearRollTheDiceButton()
{
  gotoxy(BOARD_WIDTH / 2 + INITIAL_POS_X - 3, BOARD_HEIGHT + INITIAL_POS_Y + 3);
  printf("       ");
}

void printResultsFromTheDice(int dice1, int dice2, int bonus)
{
  gotoxy(BOARD_WIDTH / 2 + INITIAL_POS_X - 9, BOARD_HEIGHT + INITIAL_POS_Y + 3);
  printf("Dice1: %d | Dice2: %d", dice1, dice2);
  if (bonus != 0)
    printf(" +x%d", bonus);
}

void clearResultsFromTheDice()
{
  gotoxy(BOARD_WIDTH / 2 + INITIAL_POS_X - 9, BOARD_HEIGHT + INITIAL_POS_Y + 3);
  printf("                       ");
}

void printWatchMode()
{
  gotoxy(1, 36);
  printf("[WATCH MODE]");
  gotoxy(3, BOARD_HEIGHT + INITIAL_POS_Y + 5);
  printf("Next: Right arrow | Back: Left arrow | Beginning: B | End: E | q: quit");
}

void printWinScreen(char name[PLAYER_NAME_LENGTH], int points)
{
  clrscr();
  gotoxy(1, 1);
  
  int i;
  for (i = 0; name[i+1] != '\0'; i++)
  {
    printf("%c", name[i]);
  }
  printf(" won the game! +%d", points);

  gotoxy(1, 5);
  printf("q: quit | p: play again");
  startWinScreenMenu();
}

void startWinScreenMenu()
{
  char cmd = getch();
  
  while (cmd != 'q' || cmd != 'p')
  {
    if (cmd == 'q')
    {
      startMainMenu();
      return;
    }
    else if (cmd == 'p')
    {
      startGame();
      return;
    }
    cmd = getch();
  }  
}