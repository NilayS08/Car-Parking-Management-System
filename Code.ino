#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Configuration
#define IR_SENSOR_PIN 2
#define TRIG_PIN1 5
#define ECHO_PIN1 6
#define TRIG_PIN2 9  // Second ultrasonic sensor trig pin
#define ECHO_PIN2 10 // Second ultrasonic sensor echo pin
#define LED_PIN1 7   // LED for spot 1
#define LED_PIN2 8   // LED for spot 2
#define ENTRY_LED_PIN 4  // Using former servo pin for entry indicator LED

// Constants
#define TOTAL_SPOTS 2
#define DISTANCE_THRESHOLD_CM 15
#define LCD_UPDATE_INTERVAL 500
#define MAX_DISTANCE 200
#define DEBOUNCE_DELAY 500
#define ENTRY_LED_DURATION 3000  // How long entry LED stays on

LiquidCrystal_I2C lcd(0x27, 16, 2);

int availableSpots = TOTAL_SPOTS;
bool isSpot1Occupied = false;
bool isSpot2Occupied = false;
bool isEntryLedOn = false;

unsigned long lastDistanceCheck = 0;
unsigned long lastLCDUpdate = 0;
unsigned long entryLedOnTime = 0;
unsigned long lastIRTrigger = 0;

void setup() {
  Serial.begin(9600);

  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(ENTRY_LED_PIN, OUTPUT);

  digitalWrite(ENTRY_LED_PIN, LOW);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Parking");
  lcd.setCursor(0, 1);
  lcd.print("System v1.3");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready!");
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastDistanceCheck >= 200) {
    checkSpot1();
    checkSpot2();
    lastDistanceCheck = currentMillis;
  }

  if (currentMillis - lastLCDUpdate >= LCD_UPDATE_INTERVAL) {
    updateLCD();
    lastLCDUpdate = currentMillis;
  }

  checkIRSensor(currentMillis);
  checkEntryLedTimeout(currentMillis);
  updateLEDs();
}

float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  float distance = duration * 0.0343 / 2;
  return (distance <= 0 || distance > MAX_DISTANCE) ? MAX_DISTANCE : distance;
}

void checkSpot1() {
  float distance = getDistance(TRIG_PIN1, ECHO_PIN1);
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

void checkSpot2() {
  float distance = getDistance(TRIG_PIN2, ECHO_PIN2);
  bool carDetected = (distance < DISTANCE_THRESHOLD_CM);

  if (carDetected && !isSpot2Occupied) {
    isSpot2Occupied = true;
    availableSpots = max(0, availableSpots - 1);
    Serial.println("Car detected in Spot 2");
  } else if (!carDetected && isSpot2Occupied) {
    isSpot2Occupied = false;
    availableSpots = min(TOTAL_SPOTS, availableSpots + 1);
    Serial.println("Spot 2 now vacant");
  }
}

void checkIRSensor(unsigned long currentMillis) {
  if (digitalRead(IR_SENSOR_PIN) == LOW && currentMillis - lastIRTrigger > DEBOUNCE_DELAY) {
    lastIRTrigger = currentMillis;

    if (availableSpots > 0) {
      Serial.println("IR Triggered: Entry allowed");
      turnOnEntryLed();
      isEntryLedOn = true;
      entryLedOnTime = currentMillis;
    } else {
      Serial.println("IR Triggered: No spots available");
      // Optional: Add a different signal for when no spots are available
      // Like blinking the entry LED briefly
      for (int i = 0; i < 3; i++) {
        digitalWrite(ENTRY_LED_PIN, HIGH);
        delay(200);
        digitalWrite(ENTRY_LED_PIN, LOW);
        delay(200);
      }
    }
  }
}

void turnOnEntryLed() {
  digitalWrite(ENTRY_LED_PIN, HIGH);
}

void turnOffEntryLed() {
  digitalWrite(ENTRY_LED_PIN, LOW);
}

void checkEntryLedTimeout(unsigned long currentMillis) {
  if (isEntryLedOn && currentMillis - entryLedOnTime >= ENTRY_LED_DURATION) {
    turnOffEntryLed();
    isEntryLedOn = false;
    Serial.println("Entry LED turning off (timeout)");
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
