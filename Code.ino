#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Configuration
#define IR_SENSOR_PIN 2
#define SERVO_PIN 4
#define TRIG_PIN 5
#define ECHO_PIN 6
#define LED_PIN1 7
#define LED_PIN2 8

// Constants
#define TOTAL_SPOTS 2
#define DISTANCE_THRESHOLD_CM 15
#define GATE_OPEN_ANGLE 0
#define GATE_CLOSED_ANGLE 90
#define GATE_DELAY 3000
#define LCD_UPDATE_INTERVAL 500
#define MAX_DISTANCE 200
#define DEBOUNCE_DELAY 500

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo gateServo;

int availableSpots = TOTAL_SPOTS;
bool isSpot1Occupied = false;
bool isSpot2Occupied = false;
bool isGateOpen = false;

unsigned long lastDistanceCheck = 0;
unsigned long lastLCDUpdate = 0;
unsigned long gateOpenTime = 0;
unsigned long lastIRTrigger = 0;

void setup() {
  Serial.begin(9600);

  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Parking");
  lcd.setCursor(0, 1);
  lcd.print("System v1.3");
  delay(2000);

  gateServo.attach(SERVO_PIN);
  gateServo.write(GATE_CLOSED_ANGLE);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready!");
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastDistanceCheck >= 200) {
    checkSpot1(); // Only spot 1 has ultrasonic sensor
    lastDistanceCheck = currentMillis;
  }

  if (currentMillis - lastLCDUpdate >= LCD_UPDATE_INTERVAL) {
    updateLCD();
    lastLCDUpdate = currentMillis;
  }

  checkIRSensor(currentMillis);
  checkGateTimeout(currentMillis);
  updateLEDs();
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float distance = duration * 0.0343 / 2;
  return (distance <= 0 || distance > MAX_DISTANCE) ? MAX_DISTANCE : distance;
}

void checkSpot1() {
  float distance = getDistance();
  bool carDetected = (distance < DISTANCE_THRESHOLD_CM);

  if (carDetected && !isSpot1Occupied) {
    isSpot1Occupied = true;
    availableSpots = max(0, availableSpots - 1);
    Serial.println("Car detected in Spot 1");
  } else if (!carDetected && isSpot1Occupied) {
    isSpot1Occupied = false;
    availableSpots = min(TOTAL_SPOTS, availableSpots + 1);
    Serial.println("Spot 1 now vacant");
  }
}

void checkIRSensor(unsigned long currentMillis) {
  if (digitalRead(IR_SENSOR_PIN) == LOW && currentMillis - lastIRTrigger > DEBOUNCE_DELAY) {
    lastIRTrigger = currentMillis;

    if (!isGateOpen && availableSpots > 0) {
      Serial.println("IR Triggered: Opening Gate");
      openGate();
      isGateOpen = true;
      gateOpenTime = currentMillis;

      // Auto-assign next free spot (only Spot 2 is logic-based)
      if (!isSpot1Occupied && !isSpot2Occupied) {
        // Spot 1 will be handled by ultrasonic
      } else if (!isSpot2Occupied) {
        isSpot2Occupied = true;
        availableSpots = max(0, availableSpots - 1);
        Serial.println("Assuming car went to Spot 2");
      }
    }
  }
}

void openGate() {
  gateServo.write(GATE_OPEN_ANGLE);
}

void closeGate() {
  gateServo.write(GATE_CLOSED_ANGLE);
}

void checkGateTimeout(unsigned long currentMillis) {
  if (isGateOpen && currentMillis - gateOpenTime >= GATE_DELAY) {
    closeGate();
    isGateOpen = false;
    Serial.println("Closing Gate (timeout)");
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Available: ");
  lcd.print(availableSpots);
  lcd.print("/");
  lcd.print(TOTAL_SPOTS);

  lcd.setCursor(0, 1);
  lcd.print("S1:");
  lcd.print(isSpot1Occupied ? "Full" : "Free");
  lcd.setCursor(8, 1);
  lcd.print("S2:");
  lcd.print(isSpot2Occupied ? "Full" : "Free");
}

void updateLEDs() {
  digitalWrite(LED_PIN1, isSpot1Occupied ? HIGH : LOW);
  digitalWrite(LED_PIN2, isSpot2Occupied ? HIGH : LOW);
}
