#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 28 // 24 pola + 2x banda + 2x dwór
#define PLAYER_NAME_LENGTH 32
#define GAME_ID_LENGTH 22

struct Field
{
  unsigned short int number;
  int fieldOwner; // 1 - player1 ; 2 - player2 ; 3 - player1 ; 4 - player2
  unsigned short int numberOfPawns;
};

struct GameState
{
  int playerTour;
  struct Field fields[BOARD_SIZE];
};

struct Game
{
  char id[GAME_ID_LENGTH];
  struct GameState *state;
  char player1Name[PLAYER_NAME_LENGTH];
  char player2Name[PLAYER_NAME_LENGTH];
  int tour;
};

int selectPawn();
void startGame();
void playTurn(struct Game* game, int player);
void makeMove(int move, int* actualFieldNumber);
int selectPawn(int player, struct Field* fields);
int possibleField(struct Field* fields, int player, int selectedPawn, int fieldNumber, int dice1, int dice2, int bonus);
int selectPosition(int player, struct Game* game, struct Field* fields, int selectedPawn, int dice1, int dice2, int bonus);
void movePawn(int player, struct Game* game, int* dice1, int* dice2, int* bonus, int defaultSelectedPawn);
void moveToBonus(struct Game* game, int selectedPawn, int moveToPosition, int player, int* dice1, int* dice2, int* bonus);
void moveToNonBonus(struct Game* game, int selectedPawn, int moveToPos, int player, int* dice1, int* dice2);
void move2Moves1Dice(struct Game* game, int selectedPawn, int gfd1, int player, int* dice1, int* dice2);
void move2Moves2Dice(struct Game* game, int selectedPawn, int gfd2, int player, int* dice1, int* dice2);
int selectPawnChoise(int defaultSelectedPawn, int player, struct Game* game);
void playPawn(struct Game* game, int player, int* dice1, int* dice2, int* bonus);
void loadGame();
enum SelectPawnMode checkBeating(struct Game *game, int player, int dice1, int dice2, int bonus, int fieldNumber);
enum SelectPawnMode checkGoingHome(struct Field *fields, int player, int selectedPawn, int dice1, int dice2);
int compareHomeField(int player, int gf, int homeField);
int checkGoingHomeDice(struct Field* fields, int player, int selectedPawn, int homeField, int dice);
int samePawn(int selectedPawn, int fieldNumber);
int possibleToGo(struct Field* fields, int player, int selectedPawn, int fieldNumber, int dice1, int dice2, int bonus);
enum SelectPawnMode setPawnMode(struct Game* game, struct Field* fields, int selectedPawn, int fieldNumber, int player, int dice1, int dice2, int bonus);
void printAfterMove(struct Game* game, int* dice1, int* dice2, int* bonus);
int win(struct Game* game, int player);
void finishGame(struct Game game, int player);
int farestPawnGoHome(struct GameState gs, int player, int selectedPawn, int moveToPos);
void resetBiggerDice(int* dice1, int* dice2);

#endif
