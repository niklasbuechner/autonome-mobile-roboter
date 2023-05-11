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

void moveRobotArmTo(int field) {
    printf("🤖 Robot moved to field %d.\n", field);
}

void drawCross() {
    printf("🤖 Robot draws X.\n");
}
void drawCircle() {
    printf("🤖 Robot draws ⏺.\n");
}