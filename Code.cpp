#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Configuration
#define IR_SENSOR_PIN 3
#define SERVO_PIN 4
#define TRIG_PIN 5
#define ECHO_PIN 6

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

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Parking");
  lcd.setCursor(0, 1);
  lcd.print("System v1.2");
  delay(2000);

  gateServo.attach(SERVO_PIN);
  gateServo.write(GATE_CLOSED_ANGLE);
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready!");
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastDistanceCheck >= 100) {
    checkParkingSpots();
    lastDistanceCheck = currentMillis;
  }

  if (currentMillis - lastLCDUpdate >= LCD_UPDATE_INTERVAL) {
    updateLCDDisplay();
    lastLCDUpdate = currentMillis;
  }

  checkIRSensor(currentMillis);
  checkGateTimeout(currentMillis);
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

void checkParkingSpots() {
  float distance = getDistance();
  
  if (distance < DISTANCE_THRESHOLD_CM) {
    if (!isSpot1Occupied) {
      isSpot1Occupied = true;
      availableSpots = max(0, availableSpots - 1);
      Serial.println("Car detected in Spot 1");
    }
  } else {
    if (isSpot1Occupied) {
      isSpot1Occupied = false;
      availableSpots = min(TOTAL_SPOTS, availableSpots + 1);
      Serial.println("Spot 1 is now vacant");
    }
  }
}

void checkIRSensor(unsigned long currentMillis) {
  if (digitalRead(IR_SENSOR_PIN) == LOW && currentMillis - lastIRTrigger > DEBOUNCE_DELAY) {
    lastIRTrigger = currentMillis;
    
    if (!isGateOpen && availableSpots > 0) {
      Serial.println("Vehicle detected - Opening gate");
      openGate();
      isGateOpen = true;
      gateOpenTime = currentMillis;
      
      if (!isSpot1Occupied) {
        isSpot1Occupied = true;
        availableSpots--;
      } else if (!isSpot2Occupied) {
        isSpot2Occupied = true;
        availableSpots--;
      }
    } else if (!isGateOpen && availableSpots == 0) {
      // Vehicle is leaving
      if (isSpot2Occupied) {
        isSpot2Occupied = false;
        availableSpots++;
        openGate();
        isGateOpen = true;
        gateOpenTime = currentMillis;
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

void checkGateTimeout(unsigned long currentTime) {
  if (isGateOpen && currentTime - gateOpenTime >= GATE_DELAY) {
    Serial.println("Closing gate (timeout)");
    closeGate();
    isGateOpen = false;
  }
}

void updateLCDDisplay() {
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
