#include <stdio.h>

struct TicTacToe {
    int16_t firstPlayerField;
    int16_t secondPlayerField;
    int16_t winningPosition;

    int isDone;
    int winner;
};

struct TicTacToe startTicTacToe();
int canPlayMove(struct TicTacToe *game, int16_t move);
void playTicTacToe(struct TicTacToe *game, int player, int16_t move);

int16_t getWinningPosition(int16_t field);
int getWinner(int16_t firstPlayer, int16_t secondPlayer);

int16_t activateField(int16_t player, int16_t fieldNumber);
int16_t getField(int16_t fieldNumber);
