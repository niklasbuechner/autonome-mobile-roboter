int yPins =32;
int channel = 0;

int in1Pin = 12;      // Define L293D channel 1 pin
int in2Pin = 14;      // Define L293D channel 2 pin
int enable1Pin = 13;  // Define L293D enable 1 pin

void setup() {
    Serial.begin(115200);

    // Initialize the pin into an output mode:
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
    pinMode(enable1Pin, OUTPUT);

    ledcSetup(channel,1000,11);         //Set PWM to 11 bits, range is 0-2047
    ledcAttachPin(enable1Pin,channel);
}

void loop() {
    int yVal = analogRead(yPins);

    if(yVal<1000){
        driveMotorUp(yVal);
    }

    if(yVal>3000){
        driveMotorDown(yVal);
    } 
}

void driveMotorDown(int val){
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);

    ledcWrite(channel, 1000);

    Serial.printf("Down:%d",val);
    delay(500);
    ledcWrite(channel, 0);
}

void driveMotorUp(int val){
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);

    ledcWrite(channel, 1000);

    Serial.printf("Up:%d",val);
    delay(500);
    ledcWrite(channel, 0);
}
