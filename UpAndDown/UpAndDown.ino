int yPins =32;
int y_channel = 0;
int y_in1Pin = 12;      // Define L293D channel 1 pin
int y_in2Pin = 14;      // Define L293D channel 2 pin
int y_enable1Pin = 13;  // Define L293D enable 1 pin

int xPins =33;
int x_channel = 1;
int x_in1Pin = 0;      // Define L293D channel 1 pin
int x_in2Pin = 2;      // Define L293D channel 2 pin
int x_enable1Pin = 15;  // Define L293D enable 1 pin

int frequency = 1000;
int precision = 11;
int max_duty = 2047;

void setup() {
    Serial.begin(115200);
    Serial.println("Setup running");

    // Initialize the pin into an output mode:
    pinMode(y_in1Pin, OUTPUT);
    pinMode(y_in2Pin, OUTPUT);
    pinMode(y_enable1Pin, OUTPUT);

    pinMode(x_in1Pin, OUTPUT);
    pinMode(x_in2Pin, OUTPUT);
    pinMode(x_enable1Pin, OUTPUT);

    ledcSetup(y_channel,frequency,precision);         //Set PWM to 11 bits, range is 0-2047
    ledcSetup(x_channel,frequency,precision);         //Set PWM to 11 bits, range is 0-2047
    ledcAttachPin(y_enable1Pin,y_channel);
    ledcAttachPin(x_enable1Pin,x_channel);
}

void loop() {
    int yVal = analogRead(yPins);
    int xVal = analogRead(xPins);

    if(yVal<1000){
        driveMotorUp(1);
    }     
    if(yVal>3000){
        driveMotorDown(1);
    }
     if(xVal<1000){
        driveMotorUp(0);
    }
    if(xVal>3000){
        driveMotorDown(0);
    } 
}

void driveMotorDown(int axis){
  if(axis==0){
      digitalWrite(x_in1Pin, HIGH);
      digitalWrite(x_in2Pin, LOW);

      ledcWrite(x_channel, max_duty);
      delay(500);
      ledcWrite(x_channel, 0);
  }else if(axis==1){
      digitalWrite(y_in1Pin, HIGH);
      digitalWrite(y_in2Pin, LOW);
        
      ledcWrite(y_channel, max_duty);
      delay(500);
      ledcWrite(y_channel, 0);
  }  
}

void driveMotorUp(int axis){
  if(axis==0){
      Serial.println("x Up");
    
      digitalWrite(x_in1Pin, LOW);
      digitalWrite(x_in2Pin, HIGH);

      ledcWrite(x_channel, max_duty);
      delay(500);
      ledcWrite(x_channel, 0);
  }else if(axis==1){
      Serial.println("y Up");
    
      digitalWrite(y_in1Pin, LOW);
      digitalWrite(y_in2Pin, HIGH);

      ledcWrite(y_channel, max_duty);
      delay(500);
      ledcWrite(y_channel, 0);
  }
}
