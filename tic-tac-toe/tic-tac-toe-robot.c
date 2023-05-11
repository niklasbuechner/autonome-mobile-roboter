#include <stdio.h>
#include "./tic-tac-toe-robot.h"
#include "./tic-tac-toe.h"
#include "./robot.h"

enum GameStatus {
    WAITING_TO_START,
    FIRST_PLAYERS_TURN,
    SECOND_PLAYERS_TURN,
    DONE,
};

void runTicTacToeEngine() {
    int status = WAITING_TO_START;
    waitUntilGameStarts();
    struct TicTacToe game = startTicTacToe();
    status = FIRST_PLAYERS_TURN;

    while (status != DONE) {
        int player = 1;
        if (status == SECOND_PLAYERS_TURN) {
            player = 2;
        }

        int move = 0;
        while (move == 0) {
            move = getFieldSelectedByPlayer();

            if (canPlayMove(&game, move) == 0) {
                showCannotPlayMove();
            }
        }
        playTicTacToe(&game, player, move);
        moveRobotArmTo(move);

        if (player == 1) {
            drawCross();
            status = SECOND_PLAYERS_TURN;
        } else {
            drawCircle();
            status = FIRST_PLAYERS_TURN;
        }

        if (game.isDone) {
            status = DONE;
        }
    }

    printf("Run engine\n");
}