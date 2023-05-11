#include "./tic-tac-toe.h"
#include "./tic-tac-toe-test.h"

int main() {
    assertInt("Field 1, 2, 3 win", 1, isWinningPosition(0b000000111));
    assertInt("Field 4, 5, 6 win", 1, isWinningPosition(0b000111000));
    assertInt("Field 7, 8, 9 win", 1, isWinningPosition(0b111000000));
    assertInt("Field 1, 5, 9 win", 1, isWinningPosition(0b100010001));
    assertInt("Field 3, 5, 7 win", 1, isWinningPosition(0b001010100));

    printf("\n");

    assertInt("Field 1, 2, 3 win ignores noise", 1, isWinningPosition(0b010100111));
    assertInt("Field 4, 5, 6 win ignores noise", 1, isWinningPosition(0b010111001));
    assertInt("Field 7, 8, 9 win ignores noise", 1, isWinningPosition(0b111010001));
    assertInt("Field 1, 5, 9 win ignores noise", 1, isWinningPosition(0b100110101));
    assertInt("Field 3, 5, 7 win ignores noise", 1, isWinningPosition(0b001110101));

    printf("\n");

    assertInt("Get field 1", 0b000000001, getField(1));
    assertInt("Get field 2", 0b000000010, getField(2));
    assertInt("Get field 3", 0b000000100, getField(3));
    assertInt("Get field 4", 0b000001000, getField(4));
    assertInt("Get field 5", 0b000010000, getField(5));
    assertInt("Get field 6", 0b000100000, getField(6));
    assertInt("Get field 7", 0b001000000, getField(7));
    assertInt("Get field 8", 0b010000000, getField(8));
    assertInt("Get field 9", 0b100000000, getField(9));

    printf("\n");

    assertInt("Activates player position 1", 0b000100001, activateField(0b000100000, 1));
    assertInt("Activates player position 2", 0b000100010, activateField(0b000100000, 2));
    assertInt("Activates player position 3", 0b000100100, activateField(0b000100000, 3));
    assertInt("Activates player position 4", 0b000101000, activateField(0b000100000, 4));
    assertInt("Activates player position 5", 0b000110000, activateField(0b000100000, 5));
    assertInt("Activates player position 6", 0b000100000, activateField(0b000100000, 6));
    assertInt("Activates player position 7", 0b001100000, activateField(0b000100000, 7));
    assertInt("Activates player position 8", 0b010100000, activateField(0b000100000, 8));
    assertInt("Activates player position 9", 0b100100000, activateField(0b000100000, 9));

    printf("\n");

    assertInt("Detect winner is player one", 1, getWinner(0b111000000, 0b000110100));
    assertInt("Detect winner is player two", 2, getWinner(0b000110100, 0b111000000));
    assertInt("Detect no winner", 0, getWinner(0b000110100, 0b101000000));

    printf("\n");

    struct TicTacToe game = startTicTacToe();
    assertGame("Init game", 0, 0, 0b000000000, 0b000000000, game);
    playTicTacToe(&game, 1, 1);
    assertGame("Round 1:", 0, 0, 0b000000001, 0b000000000, game);
    playTicTacToe(&game, 2, 5);
    assertGame("Round 2:", 0, 0, 0b000000001, 0b000010000, game);
    playTicTacToe(&game, 1, 2);
    assertGame("Round 3:", 0, 0, 0b000000011, 0b000010000, game);
    playTicTacToe(&game, 2, 4);
    assertGame("Round 4:", 0, 0, 0b000000011, 0b000011000, game);
    playTicTacToe(&game, 1, 3);
    assertGame("Round 5:", 1, 1, 0b000000111, 0b000011000, game);
}

void assertInt(char* name, int expected, int actual) {
    if (expected == actual) {
        printf("✅ '%s' passed\n", name);
    } else {
        printf("❌ '%s' failed\n", name);
    }
}

void assertGame(
    char* name,
    int isDone,
    int winner,
    int16_t firstPlayerField,
    int16_t secondPlayerField,
    struct TicTacToe actual
) {
    if (firstPlayerField == actual.firstPlayerField
        && secondPlayerField == actual.secondPlayerField
        && isDone == actual.isDone
        && winner == actual.winner
    ) {
        printf("✅ '%s' passed\n", name);
    } else {
        printf(
            "❌ '%s' failed: isDone %d, winner %d, p1 field %d, p2 field %d\n",
            name,
            actual.isDone,
            actual.winner,
            actual.firstPlayerField,
            actual.secondPlayerField
        );
    }
}