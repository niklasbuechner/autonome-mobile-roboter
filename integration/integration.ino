#include<Keypad.h>

//Festlegung Motor-Pins
#define inx1Pin 22      //Motor Input Pin in x-Richtung vorwärts/rückwärts (?!)
#define inx2Pin 21      //Motor Input Pin in x-Richtung vorwärts/rückwärts (?!)
#define enablexPin 23   //Motor enable Pin in x-Richtung

#define iny1Pin 5      //Motor Input Pin in y-Richtung rechts/links (?!)
#define iny2Pin 18       //Motor Input Pin in y-Richtung rechts/links (?!)
#define enableyPin 19   ////Motor enable Pin in y-Richtung

//vorläufig
//#define inz1Pin 26      //Motor Input Pin in z-Richtung rechts/links (?!)
//#define inz2Pin 27       //Motor Input Pin in z-Richtung rechts/links (?!)
//#define enablezPin 15   ////Motor enable Pin in z-Richtung

//Keypad
char keys [4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};  //Keypad Belegung definieren

byte rowPins[4] = {13, 12, 14, 25}; //Pins für die Reihen
byte colPins[4] = {33, 32, 2, 0}; //Pins für Zeilen
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4); //Objekt von Keypad deklarieren

void setup() {
  // put your setup code here, to run once:
  keypadSetup();  //Keypad Setup
  motorSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  runTicTacToeEngine();
}

void keypadSetup(){
  Serial.begin(115200); //Initialisiert serial Port und setzt baud rate auf 116200 (müsste eigentlich schon passiert sein)
  Serial.println("ESP32 is ready!");
}

//Variablen für Motor definiert
int channelx = 1; //Bin mir nicht sicher was das tut
int channely = 0; //Bin mir nicht sicher was das tut
int posVal = 0;
int frequency = 1000;
int precision = 11;
int max_duty = 2047;
int speed = 2037;

double moveLeftFactor = 1.07;
double moveUpFactor = 1.065;

void motorSetup(){
  //Driver SetUP
  //x Achse
  pinMode(enablexPin, OUTPUT);
  pinMode(inx1Pin, OUTPUT);
  pinMode(inx2Pin, OUTPUT);
  ledcSetup(channelx, frequency, precision);
  ledcAttachPin(enablexPin, channelx); 

  pinMode(enableyPin, OUTPUT);
  pinMode(iny1Pin, OUTPUT);
  pinMode(iny2Pin, OUTPUT);
  ledcSetup(channely, frequency, precision);
  ledcAttachPin(enableyPin, channely); 
}

struct TicTacToe {
    int16_t firstPlayerField;
    int16_t secondPlayerField;
    int16_t winningPosition;

    int isDone;
    int winner;
};

int16_t winningPositions[] = {
    0b000000111,
    0b000111000,
    0b111000000,
    0b100010001,
    0b001010100,
    0b100100100,
    0b010010010,
    0b001001001,
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

    printField(game->firstPlayerField, 'X');
    int16_t winningPosition = getWinningPosition(game->firstPlayerField);
    if (winningPosition != 0) {
        game->winner = 1;
        game->isDone = 1;
        game->winningPosition = winningPosition;
    }

    printField(game->secondPlayerField, 'O');
    winningPosition = getWinningPosition(game->secondPlayerField);
    if (winningPosition != 0) {
        game->winner = 2;
        game->isDone = 1;
        game->winningPosition = winningPosition;
    }
}

void printField(int16_t player, char symbol) {
  drawActivated(player, 1, symbol);
  drawActivated(player, 2, symbol);
  drawActivated(player, 3, symbol);
  Serial.print('\n');
  drawActivated(player, 4, symbol);
  drawActivated(player, 5, symbol);
  drawActivated(player, 6, symbol);
  Serial.print('\n');
  drawActivated(player, 7, symbol);
  drawActivated(player, 8, symbol);
  drawActivated(player, 9, symbol);
  Serial.print('\n');
}

void drawActivated(int16_t player, int16_t field, char symbol) {
  if ((player & getField(field)) == getField(field)) {
    Serial.print(symbol);
  } else {
    Serial.print('-');
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
    for (int counter = 0; counter < 8; counter += 1) {
        if ((field & winningPositions[counter]) == winningPositions[counter]) {
            return winningPositions[counter];
        }
    }

    Serial.println("No winning position");

    return 0;
}

int16_t activateField(int16_t player, int16_t fieldNumber) {
    return player | getField(fieldNumber);
}

int16_t getField(int16_t fieldNumber) {
    return 0b000000001 << (fieldNumber - 1);
}

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
                move = 0;
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

        if (containsField(winningPosition, 5) == 1) {
          drawTopToBottomWinningLine();
        } else if (containsField(winningPosition, 2) == 1) {
          drawHorizontalWinningLine();
        } else {
          drawVerticalWinningLine();
        }
    } else if (containsField(winningPosition, 4) == 1) {
        moveRobotArmToFieldFour();
        drawHorizontalWinningLine();
    } else if (containsField(winningPosition, 7) == 1) {
        moveRobotArmToFieldSeven();

        if (containsField(winningPosition, 4) == 1) {
            drawBottomToTopWinningLine();
        } else {
            drawHorizontalWinningLine();
        }
    } else if (containsField(winningPosition, 2) == 1) {
      moveRobotArmToFieldTwo();
      drawVerticalWinningLine();
    } else if (containsField(winningPosition, 3) == 1) {
      moveRobotArmToFieldThree();
      drawVerticalWinningLine();
    }

    moveRobotArmToOrigin();
}

int containsField(int16_t winningPosition, int16_t fieldNumber) {
    int16_t field = getField(fieldNumber);

    return (winningPosition & field) == field;
}

int xMove = 0;
int yMove = 0;

void moveLeft() {
  digitalWrite(iny1Pin, LOW);
  digitalWrite(iny2Pin, HIGH);
  ledcWrite(channely, speed);
}
void moveRight() {
  digitalWrite(iny2Pin, LOW);
  digitalWrite(iny1Pin, HIGH);
  ledcWrite(channely, speed);
}
void moveUp() {
  digitalWrite(inx2Pin, LOW);
  digitalWrite(inx1Pin, HIGH);
  ledcWrite(channelx, speed * 0.8);
}
void moveDown() {
  digitalWrite(inx1Pin, LOW);
  digitalWrite(inx2Pin, HIGH);
  ledcWrite(channelx, speed * 0.8);
}
void stop() {
  ledcWrite(channely, 0);
  ledcWrite(channelx, 0);
}
void moveLeftFor(int time) {
  moveLeft();
  delay(time * moveLeftFactor);
  stop();
}
void moveLeftUpFor(int time) {
  moveLeft();
  moveUp();
  delay(time * moveUpFactor);
  stop();

  moveLeft();
  delay((time * moveUpFactor) - (time * moveLeftFactor));
  stop();
}
void moveLeftDownFor(int time) {
  moveLeft();
  moveDown();
  delay(time);
  stop();

  moveLeft();
  delay(time * moveLeftFactor - time);
  stop();
}
void moveRightFor(int time) {
  moveRight();
  delay(time);
  stop();
}
void moveRightUpFor(int time) {
  moveRight();
  moveUp();
  delay(time);
  stop();

  moveUp();
  delay(time * moveUpFactor - time);
  stop();
}
void moveRightDownFor(int time) {
  moveRight();
  moveDown();
  delay(time);
  stop();
}
void moveUpFor(int time) {
  moveUp();
  delay(time * moveUpFactor);
  stop();
}
void moveDownFor(int time) {
  moveDown();
  delay(time);
  stop();
}

void moveRightForAndSave(int time) {
  moveRightFor(time);
  yMove = time;
}
void moveDownForAndSave(int time) {
  moveDownFor(time);
  xMove = time;
}


void waitUntilGameStarts() {
  Serial.println("Waiting to start...");
  char keyPressed = 0;  //Frag Input ab
  KeyState state = myKeypad.getState();

  while(!keyPressed || keyPressed == '#') {
    keyPressed = myKeypad.getKey();

    if (keyPressed == 'A') {
      moveRight();
      keyPressed = 0;
    } else if (keyPressed == 'B') {
      moveLeft();
      keyPressed = 0;
    } else if (keyPressed == 'C') {
      moveUp();
      keyPressed = 0;
    } else if (keyPressed == 'D') {
      moveDown();
      keyPressed = 0;
    } else {
      if (myKeypad.getState() != PRESSED && myKeypad.getState() != HOLD) {
        stop();
      }
    }

    delay(100);
  }

  Serial.println("Draw field");
  moveRightFor(2000);
  moveDownFor(6000);
  moveRightFor(2000);
  moveUpFor(6000);
  moveRightDownFor(2000);
  moveLeftFor(6000);
  moveDownFor(2000);
  moveRightFor(6000);
  moveUpFor(4000);
  moveLeftFor(6000);
  
  Serial.println("Game started!");
}
int getFieldSelectedByPlayer() {
  Serial.println("Waiting for field selection");
  char keyPressed = 0;

  while(!keyPressed) {
    keyPressed = myKeypad.getKey();

    if (keyPressed == 'A') {
      moveRight();
      keyPressed = 0;
    } else if (keyPressed == 'B') {
      moveLeft();
      keyPressed = 0;
    } else if (keyPressed == 'C') {
      moveUp();
      keyPressed = 0;
    } else if (keyPressed == 'D') {
      moveDown();
      keyPressed = 0;
    } else {
      if (myKeypad.getState() != PRESSED && myKeypad.getState() != HOLD) {
        stop();
      }
    }

    delay(100);
  }

  Serial.println("Key choosen!");
  Serial.println(keyPressed);

  return keyPressed - 48;
}
void showCannotPlayMove() {
  Serial.println("Cant play move");

  moveRight();
  moveDown();
  delay(200);
  moveLeft();
  moveUp();
  delay(200 * moveUpFactor);
}

void moveRobotArmToOrigin() {
  Serial.println("Move to origin");
  
  moveUpFor(xMove);
  moveLeftFor(yMove);
}

void moveRobotArmToFieldOne() {
  Serial.println("Move to field 1");
  moveRightForAndSave(1000);
  moveDownForAndSave(1000);
}
void moveRobotArmToFieldTwo() {
  Serial.println("Move to field 2");
  moveRightForAndSave(3250);
  moveDownForAndSave(1000);
}
void moveRobotArmToFieldThree() {
  Serial.println("Move to field 3");
  moveRightForAndSave(5500);
  moveDownForAndSave(1000);
}
void moveRobotArmToFieldFour() {
  Serial.println("Move to field 4");
  moveRightForAndSave(1000);
  moveDownForAndSave(3000);
}
void moveRobotArmToFieldFive() {
  Serial.println("Move to field 5");
  moveRightForAndSave(3250);
  moveDownForAndSave(3000);
}
void moveRobotArmToFieldSix() {
  Serial.println("Move to field 6");
  moveRightForAndSave(5500);
  moveDownForAndSave(3000);
}
void moveRobotArmToFieldSeven() {
  Serial.println("Move to field 7");
  moveRightForAndSave(1000);
  moveDownForAndSave(5000);
}
void moveRobotArmToFieldEight() {
  Serial.println("Move to field 8");
  moveRightForAndSave(3250);
  moveDownForAndSave(5000);
}
void moveRobotArmToFieldNine() {
  Serial.println("Move to field 9");
  moveRightForAndSave(5500);
  moveDownForAndSave(5000);
}

void drawCross() {
  Serial.println("Draw cross");
  
  moveLeftUpFor(500);
  moveRightDownFor(1000);
  moveUpFor(1000);
  moveLeftDownFor(1000);
  moveRightUpFor(500);
}
void drawCircle() {
  Serial.println("Draw circle");
  moveLeftUpFor(500);
  moveRightFor(1000);
  moveDownFor(1000);
  moveLeftFor(1000);
  moveUpFor(1000);
  moveRightDownFor(500);
}
void drawHorizontalWinningLine() {
  Serial.println("Draw horizontal winning line");
  moveRightFor(4000);
  moveLeftFor(4000);
  moveRobotArmToOrigin();
}
void drawVerticalWinningLine() {
  Serial.println("Draw vertical winning line");
  moveDownFor(4000);
  moveUpFor(4000);
  moveRobotArmToOrigin();
}
void drawTopToBottomWinningLine() {
  Serial.println("Draw top to bottom line");
  moveRightDownFor(4000);
  moveLeftUpFor(5000);
  moveRobotArmToOrigin();
}
void drawBottomToTopWinningLine() {
  Serial.println("Draw bottom to top line");
  moveRightUpFor(4000);
  moveLeftFor(5000);
  moveRobotArmToOrigin();
}

