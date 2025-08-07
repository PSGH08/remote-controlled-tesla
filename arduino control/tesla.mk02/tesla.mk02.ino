// === MotorController Class ===
class MotorController {
  private:
    int enA, in1, in2, in3, in4, enB;

  public:
    MotorController(int _enA, int _in1, int _in2, int _in3, int _in4, int _enB) {
      enA = _enA;
      in1 = _in1;
      in2 = _in2;
      in3 = _in3;
      in4 = _in4;
      enB = _enB;

      pinMode(enA, OUTPUT);
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
      pinMode(in3, OUTPUT);
      pinMode(in4, OUTPUT);
      pinMode(enB, OUTPUT);

      stop();
    }

    void moveForward(int speed = 200) {
      digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
      analogWrite(enA, speed);
      analogWrite(enB, speed);
    }

    void moveBackward(int speed = 200) {
      digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
      analogWrite(enA, speed);
      analogWrite(enB, speed);
    }

    void turnLeft(int speed = 150) {
      digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
      analogWrite(enA, speed);
      analogWrite(enB, speed);
    }

    void turnRight(int speed = 150) {
      digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
      digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
      analogWrite(enA, speed);
      analogWrite(enB, speed);
    }

    void stop() {
      digitalWrite(in1, LOW); digitalWrite(in2, LOW);
      digitalWrite(in3, LOW); digitalWrite(in4, LOW);
      analogWrite(enA, 0);
      analogWrite(enB, 0);
    }
};

// === BluetoothCar Class ===
class BluetoothCar {
  private:
    MotorController* motors;
    int ledPin;

  public:
    BluetoothCar(MotorController* _motors, int _ledPin) {
      motors = _motors;
      ledPin = _ledPin;

      pinMode(ledPin, OUTPUT);
      digitalWrite(ledPin, LOW);
    }

    void handleCommand(char cmd) {
      switch (cmd) {
        case 'F': motors->moveForward(); break;
        case 'B': motors->moveBackward(); break;
        case 'L': motors->turnLeft(); break;
        case 'R': motors->turnRight(); break;
        case 'S': motors->stop(); break;
        case '1': digitalWrite(ledPin, HIGH); break;
        case '0': digitalWrite(ledPin, LOW); break;
      }
    }
};

// === Hardware Setup ===
const int LED_PIN = 13;
MotorController motors(10, 9, 8, 7, 6, 5);  // ENA, IN1, IN2, IN3, IN4, ENB
BluetoothCar car(&motors, LED_PIN);

void setup() {
  Serial.begin(9600); // Bluetooth module communication
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    car.handleCommand(command);
  }
}
