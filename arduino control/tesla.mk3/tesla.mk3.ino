eak;
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
