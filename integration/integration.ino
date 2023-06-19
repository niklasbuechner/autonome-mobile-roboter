#include<Keypad.h>

//Festlegung Motor-Pins
#define inx1Pin 22      //Motor Input Pin in x-Richtung vorwärts/rückwärts (?!)
#define inx2Pin 21      //Motor Input Pin in x-Richtung vorwärts/rückwärts (?!)
#define enablexPin 23   //Motor enable Pin in x-Richtung

#define iny1Pin 18      //Motor Input Pin in y-Richtung rechts/links (?!)
#define iny2Pin 5       //Motor Input Pin in y-Richtung rechts/links (?!)
#define enableyPin 19   ////Motor enable Pin in y-Richtung

//Keypad
char keys [4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};  //Keypad Belegung definieren
byte rowPins[4] = {13, 14, 25, 12}; //Pins für die Reihen
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
        if ((field & winningPositions[counter]) == winningPositions[counter]) {
            return winningPositions[counter];
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

void waitUntilGameStarts() {
  Serial.println("Waiting to start...");
  char keyPressed = myKeypad.getKey();  //Frag Input ab
  KeyState state = myKeypad.getState();

  while(!keyPressed || keyPressed == '#') {
    delay(100);

    keyPressed = myKeypad.getKey();
  }

  digitalWrite(iny1Pin, LOW);
  digitalWrite(iny2Pin, HIGH);
  ledcWrite(channely, speed);
  delay(2000);
  ledcWrite(channely, speed);

  digitalWrite(inx1Pin, LOW);
  digitalWrite(inx2Pin, HIGH);
  ledcWrite(channely, speed);
  delay(8000);
  ledcWrite(channely, speed);

  digitalWrite(iny1Pin, LOW);
  digitalWrite(iny2Pin, HIGH);
  ledcWrite(channely, speed);
  delay(2000);
  ledcWrite(channely, speed);

  digitalWrite(iny2Pin, LOW);
  digitalWrite(iny1Pin, HIGH);
  ledcWrite(channely, speed);
  delay(8000);
  ledcWrite(channely, speed);

  digitalWrite(iny1Pin, LOW);
  digitalWrite(iny2Pin, HIGH);
  ledcWrite(channely, speed);
  delay(2000);
  ledcWrite(channely, speed);

  Serial.println("Game started!");
}
int getFieldSelectedByPlayer() {
  Serial.println("Waiting for field selection");
  char keyPressed = 0;

  while(!keyPressed) {
    keyPressed = myKeypad.getKey();

    if (keyPressed == 'A') {
      digitalWrite(iny2Pin, LOW);
      digitalWrite(iny1Pin, HIGH);
      ledcWrite(channely, speed);

      keyPressed = 0;
    } else if (keyPressed == 'B') {
      digitalWrite(iny1Pin, LOW);
      digitalWrite(iny2Pin, HIGH);
      ledcWrite(channely, speed);

      keyPressed = 0;
    } else if (keyPressed == 'C') {
      digitalWrite(inx1Pin, LOW);
      digitalWrite(inx2Pin, HIGH);
      ledcWrite(channelx, speed);

      keyPressed = 0;
    } else if (keyPressed == 'D') {
      digitalWrite(inx2Pin, LOW);
      digitalWrite(inx1Pin, HIGH);
      ledcWrite(channelx, speed);

      keyPressed = 0;
    } else {
      if (myKeypad.getState() != PRESSED && myKeypad.getState() != HOLD) {
        ledcWrite(channely, 0);
        ledcWrite(channelx, 0);
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
}

int xMove = 0;
int yMove = 0;

void moveRobotArmToOrigin() {
  Serial.println("Move to origin");
  digitalWrite(iny1Pin, LOW);
  digitalWrite(iny2Pin, HIGH);
  ledcWrite(channely, speed);

  delay(yMove * 1.05);

  ledcWrite(channely, 0);

  digitalWrite(inx2Pin, LOW);
  digitalWrite(inx1Pin, HIGH);
  ledcWrite(channelx, speed);

  delay(xMove * 1.05);

  ledcWrite(channelx, 0);
}

void moveRobotOnY(int time) {
  digitalWrite(iny2Pin, LOW);
  digitalWrite(iny1Pin, HIGH);
  ledcWrite(channely, speed);

  yMove = time;
  delay(yMove);

  ledcWrite(channely, 0);
}

void moveRobotOnX(int time) {
  digitalWrite(inx1Pin, LOW);
  digitalWrite(inx2Pin, HIGH);
  ledcWrite(channelx, speed);

  xMove = time;
  delay(time);

  ledcWrite(channelx, 0);
}

void moveRobotArmToFieldOne() {
  Serial.println("Move to field 1");
  moveRobotOnY(1000);
  moveRobotOnX(1000);
}
void moveRobotArmToFieldTwo() {
  Serial.println("Move to field 2");
  moveRobotOnY(1000);
  moveRobotOnX(3000);
}
void moveRobotArmToFieldThree() {
  Serial.println("Move to field 3");
  moveRobotOnY(1000);
  moveRobotOnX(5000);
}
void moveRobotArmToFieldFour() {
  Serial.println("Move to field 4");
  moveRobotOnY(3000);
  moveRobotOnX(1000);
}
void moveRobotArmToFieldFive() {
  Serial.println("Move to field 5");
  moveRobotOnY(3000);
  moveRobotOnX(3000);
}
void moveRobotArmToFieldSix() {
  Serial.println("Move to field 6");
  moveRobotOnY(3000);
  moveRobotOnX(5000);
}
void moveRobotArmToFieldSeven() {
  Serial.println("Move to field 7");
  moveRobotOnY(5000);
  moveRobotOnX(1000);
}
void moveRobotArmToFieldEight() {
  Serial.println("Move to field 8");
  moveRobotOnY(5000);
  moveRobotOnX(3000);
}
void moveRobotArmToFieldNine() {
  Serial.println("Move to field 9");
  moveRobotOnY(5000);
  moveRobotOnX(5000);
}

void drawCross() {}
void drawCircle() {}
void drawStraightWinningLine() {
  Serial.println("Win straight");
}
void drawTopToBottomWinningLine() {
  Serial.println("Top bottom");
}
void drawBottomToTopWinningLine() {
  Serial.println("Win bottom top");
}

