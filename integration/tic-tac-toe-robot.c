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
        switch(move) {
            case 1:
                moveRobotArmToFieldOne();
            break;
            case 2:
                moveRobotArmToFieldTwo();
            break;
            case 3:
                moveRobotArmToFieldThree();
            break;
            case 4:
                moveRobotArmToFieldFour();
            break;
            case 5:
                moveRobotArmToFieldFive();
            break;
            case 6:
                moveRobotArmToFieldSix();
            break;
            case 7:
                moveRobotArmToFieldSeven();
            break;
            case 8:
                moveRobotArmToFieldEight();
            break;
            case 9:
                moveRobotArmToFieldNine();
            break;
        }

        if (player == 1) {
            drawCross();
            status = SECOND_PLAYERS_TURN;
        } else {
            drawCircle();
            status = FIRST_PLAYERS_TURN;
        }

        moveRobotArmToOrigin();

        if (game.isDone) {
            status = DONE;
        }
    }

    printf("🏆 Player %d won\n", game.winner);

    int16_t winningPosition = game.winningPosition;
    if (containsField(winningPosition, 1) == 1) {
        moveRobotArmToFieldOne();

        if (containsField(winningPosition, 4) == 1) {
            drawTopToBottomWinningLine();
        } else {
            drawStraightWinningLine();
        }
    } else if (containsField(winningPosition, 4) == 1) {
        moveRobotArmToFieldFour();
    } else if (containsField(winningPosition, 7) == 1) {
        moveRobotArmToFieldSeven();

        if (containsField(winningPosition, 4) == 1) {
            drawBottomToTopWinningLine();
        } else {
            drawStraightWinningLine();
        }
    }

    moveRobotArmToOrigin();
}

int containsField(int16_t winningPosition, int16_t fieldNumber) {
    int16_t field = getField(fieldNumber);

    return (winningPosition & field) == field;
}