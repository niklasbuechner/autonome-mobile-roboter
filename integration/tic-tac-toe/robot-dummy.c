#include <stdio.h>

void waitUntilGameStarts() {
    printf("🤖 Robot started game.\n");
}

int fieldSelectionCounter = 0;
int fieldsToSelect[] = {1,4,2,5,3};

int getFieldSelectedByPlayer() {
    int field = fieldsToSelect[fieldSelectionCounter];
    fieldSelectionCounter += 1;

    printf("🤖 Robot selected field %d.\n", field);

    return field;
}

void showCannotPlayMove() {
    printf("🤖 Robot showed it could not play move.\n");
}
void moveRobotArmToOrigin() {
    printf("🤖 Robot move to origin\n");
}

void moveRobotArmTo(int field) {
    printf("🤖 Robot moved to field %d.\n", field);
}

void moveRobotArmToFieldOne() { moveRobotArmTo(1); }
void moveRobotArmToFieldTwo() { moveRobotArmTo(2); }
void moveRobotArmToFieldThree() { moveRobotArmTo(3); }
void moveRobotArmToFieldFour() { moveRobotArmTo(4); }
void moveRobotArmToFieldFive() { moveRobotArmTo(5); }
void moveRobotArmToFieldSix() { moveRobotArmTo(6); }
void moveRobotArmToFieldSeven() { moveRobotArmTo(7); }
void moveRobotArmToFieldEight() { moveRobotArmTo(8); }
void moveRobotArmToFieldNine() { moveRobotArmTo(9); }

void drawCross() {
    printf("🤖 Robot draws X.\n");
}
void drawCircle() {
    printf("🤖 Robot draws ⏺.\n");
}

void drawStraightWinningLine() {
    printf("🤖 Robot draws - line.\n");
};
void drawTopToBottomWinningLine() {
    printf("🤖 Robot draws \\ line.\n");
};
void drawBottomToTopWinningLine() {
    printf("🤖 Robot draws / line.\n");
};