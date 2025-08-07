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

// === RelayController Class ===
class RelayController {
  private:
    int relayPin;

  public:
    RelayController(int pin) {
      relayPin = pin;
      pinMode(relayPin, OUTPUT);
      digitalWrite(relayPin, LOW);
    }

    void turnOn() {
      digitalWrite(relayPin, HIGH);
    }

    void turnOff() {
      digitalWrite(relayPin, LOW);
    }
};

// === BluetoothCar Class ===
class BluetoothCar {
  private:
    MotorController* motors;
    RelayController* relay1;
    RelayController* relay2;

  public:
    BluetoothCar(MotorController* _motors, RelayController* _relay1, RelayController* _relay2) {
      motors = _motors;
      relay1 = _relay1;
      relay2 = _relay2;
    }

    void handleCommand(char cmd) {
      switch (cmd) {
        case 'F': motors->moveForward(); break;
        case 'B': motors->moveBackward(); break;
        case 'L': motors->turnLeft(); break;
        case 'R': motors->turnRight(); break;
        case 'S': motors->stop(); break;
        case '1': relay1->turnOn(); break;
        case '2': relay1->turnOff(); break;
        case '3': relay2->turnOn(); break;
        case '4': relay2->turnOff(); break;
      }
    }
};

// === Pin Definitions ===
MotorController motors(10, 9, 8, 7, 6, 5);   // ENA, IN1, IN2, IN3, IN4, ENB
RelayController relay1(12);
RelayController relay2(11);
BluetoothCar car(&motors, &relay1, &relay2);

void setup() {
  Serial.begin(9600); // HC-05 Bluetooth
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    car.handleCommand(command);
  }
}
