#include "./tic-tac-toe.h"

int16_t winningPosition[] = {
    0b000000111,
    0b000111000,
    0b111000000,
    0b100010001,
    0b001010100,
};

struct TicTacToe startTicTacToe() {
    struct TicTacToe game;
    game.firstPlayerField = 0;
    game.secondPlayerField = 0;
    game.winningPosition = 0;
    game.isDone = 0;
    game.winner = 0;

    return game;
}

void playTicTacToe(struct TicTacToe *game, int player, int16_t move) {
    if (player == 1) {
        game->firstPlayerField = activateField(game->firstPlayerField, move);
    } else {
        game->secondPlayerField = activateField(game->secondPlayerField, move);
    }

    int16_t winningPosition = getWinningPosition(game->firstPlayerField);
    if (winningPosition != 0) {
        game->winner = 1;
        game->isDone = 1;
        game->winningPosition = winningPosition;
    }

    winningPosition = getWinningPosition(game->secondPlayerField);
    if (winningPosition != 0) {
        game->winner = 2;
        game->isDone = 1;
        game->winningPosition = winningPosition;
    }
}

int canPlayMove(struct TicTacToe *game, int16_t move) {
    int16_t field = getField(move);

    if ((game->firstPlayerField & field) == field) {
        return 0;
    }

    if ((game->secondPlayerField & field) == field) {
        return 0;
    }

    return 1;
}

int16_t getWinningPosition(int16_t field) {
    for (int counter = 0; counter < 5; counter += 1) {
        if ((field & winningPosition[counter]) == winningPosition[counter]) {
            return winningPosition[counter];
        }
    }

    return 0;
}

int16_t activateField(int16_t player, int16_t fieldNumber) {
    return player | getField(fieldNumber);
}

int16_t getField(int16_t fieldNumber) {
    return 0b000000001 << (fieldNumber - 1);
}
