#include <SoftwareSerial.h>

// === SoftwareSerial for Bluetooth ===
SoftwareSerial BTSerial(2, 3); // RX, TX

// === MotorController Class ===
class MotorController {
  private:
    int enA, in1, in2, in3, in4, enB;

  public:
    MotorController(int _enA, int _in1, int _in2, int _in3, int _in4, int _enB) {
      enA = _enA; in1 = _in1; in2 = _in2; in3 = _in3; in4 = _in4; enB = _enB;
      pinMode(enA, OUTPUT); pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
      pinMode(in3, OUTPUT); pinMode(in4, OUTPUT); pinMode(enB, OUTPUT);
      stop();
    }

    void moveForward(int speed = 200) {
      digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
      analogWrite(enA, speed); analogWrite(enB, speed);
    }

    void moveBackward(int speed = 200) {
      digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
      analogWrite(enA, speed); analogWrite(enB, speed);
    }

    // === Fixed turn directions ===
    void turnLeft(int speed = 150) {
      // left motor forward, right motor backward
      digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
      digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
      analogWrite(enA, speed); analogWrite(enB, speed);
    }

    void turnRight(int speed = 150) {
      // left motor backward, right motor forward
      digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
      analogWrite(enA, speed); analogWrite(enB, speed);
    }

    void stop() {
      digitalWrite(in1, LOW); digitalWrite(in2, LOW);
      digitalWrite(in3, LOW); digitalWrite(in4, LOW);
      analogWrite(enA, 0); analogWrite(enB, 0);
    }
};

// === BluetoothCar Class ===
class BluetoothCar {
  private:
    MotorController* motors;
    int headlightPin;
    int smallLightPin;
    int rightIndicatorPin;
    int leftIndicatorPin;
    bool blinkingRight = false;
    bool blinkingLeft = false;
    unsigned long lastBlinkTime = 0;
    int blinkState = LOW;
    int blinkDelay = 500;

  public:
    BluetoothCar(MotorController* _motors, int _headlightPin, int _smallLightPin, int _rightIndicatorPin, int _leftIndicatorPin) {
      motors = _motors;
      headlightPin = _headlightPin;
      smallLightPin = _smallLightPin;
      rightIndicatorPin = _rightIndicatorPin;
      leftIndicatorPin = _leftIndicatorPin;
      pinMode(headlightPin, OUTPUT);
      pinMode(smallLightPin, OUTPUT);
      pinMode(rightIndicatorPin, OUTPUT);
      pinMode(leftIndicatorPin, OUTPUT);
      digitalWrite(headlightPin, LOW);
      digitalWrite(smallLightPin, LOW);
      digitalWrite(rightIndicatorPin, LOW);
      digitalWrite(leftIndicatorPin, LOW);
    }

    void updateBlinking() {
      if (millis() - lastBlinkTime >= blinkDelay) {
        blinkState = !blinkState;
        if (blinkingRight) digitalWrite(rightIndicatorPin, blinkState);
        if (blinkingLeft)  digitalWrite(leftIndicatorPin, blinkState);
        lastBlinkTime = millis();
      }
    }

    void startBlinkRight() { blinkingRight = true; lastBlinkTime = millis(); }
    void stopBlinkRight()  { blinkingRight = false; digitalWrite(rightIndicatorPin, LOW); }

    void startBlinkLeft()  { blinkingLeft = true; lastBlinkTime = millis(); }
    void stopBlinkLeft()   { blinkingLeft = false; digitalWrite(leftIndicatorPin, LOW); }

    void startBlinkBoth()  { startBlinkRight(); startBlinkLeft(); }
    void stopBlinkBoth()   { stopBlinkRight(); stopBlinkLeft(); }

    void handleCommand(char cmd) {
      switch (cmd) {
        case 'F': motors->moveForward(); break;
        case 'B': motors->moveBackward(); break;

        case 'L':
          motors->turnLeft();
          startBlinkLeft();
          delay(2000);
          motors->stop();
          stopBlinkLeft();
          break;

        case 'R':
          motors->turnRight();
          startBlinkRight();
          delay(2000);
          motors->stop();
          stopBlinkRight();
          break;

        case 'S': motors->stop(); stopBlinkBoth(); break;
        case 'H': digitalWrite(headlightPin, HIGH); break;
        case 'h': digitalWrite(headlightPin, LOW); break;
        case 'P': digitalWrite(smallLightPin, HIGH); break;
        case 'p': digitalWrite(smallLightPin, LOW); break;
        case 'T': startBlinkRight(); break;
        case 't': stopBlinkRight(); break;
        case 'Y': startBlinkLeft(); break;
        case 'y': stopBlinkLeft(); break;
        case 'O': startBlinkBoth(); break;
        case 'o': startBlinkBoth(); break;
      }
    }
};

// === Pin Configuration ===
const int HEADLIGHT_PIN = 12;
const int SMALL_LIGHT_PIN = 11;
const int RIGHT_INDICATOR_PIN = 13;
const int LEFT_INDICATOR_PIN = 10;
MotorController motors(9, 8, 7, 5, 6, 4);
BluetoothCar car(&motors, HEADLIGHT_PIN, SMALL_LIGHT_PIN, RIGHT_INDICATOR_PIN, LEFT_INDICATOR_PIN);

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  if (BTSerial.available()) {
    char command = BTSerial.read();
    car.handleCommand(command);
    Serial.print("Received via BT: "); Serial.println(command);
  }

  if (Serial.available()) {
    char command = Serial.read();
    car.handleCommand(command);
  }

  car.updateBlinking();
}
