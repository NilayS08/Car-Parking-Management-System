## 📝 **Problem Statement**
In crowded urban environments, efficient and automated parking management is a growing need. Manual monitoring is time-consuming and error-prone. This project addresses the problem by designing a compact **Smart Parking Management System** that automates vehicle detection, entry control, and availability indication using sensors and microcontroller-based logic.

## 🎯 **Objective**
To develop a **low-cost, Arduino-based parking system** that:
- Automatically detects incoming vehicles at the gate using an IR sensor
- Monitors two parking spots using **separate ultrasonic sensors** for accurate detection
- Displays parking status on an **I2C LCD screen**
- Controls entry using a **status LED indicator**

## 🔩 **Components Used**
| Component                  | Quantity |
|---------------------------|----------|
| Arduino Uno/Nano          | 1        |
| Ultrasonic Sensors (HC-SR04) | 2     |
| IR Sensor                 | 1        |
| Entry Status LED          | 1        |
| I2C 16x2 LCD Display      | 1        |
| Breadboard + Jumper Wires | As required |
| Power Source (USB or Battery) | 1    |

## ⚙️ **How It Works**
1. **IR Sensor (Gate Detection)**:  
   - Detects when a car is at the entrance.
   - If a parking spot is available, triggers the **entry LED** to indicate access is permitted.
   
2. **Ultrasonic Sensors (Parking Monitoring)**:  
   - Two sensors positioned to independently monitor each parking slot.
   - Each measures distance to detect presence of vehicles in its assigned slot.
   - Based on distance thresholds (15cm), determines if slots are **occupied or free**.
   
3. **Entry LED Indicator**:  
   - Illuminates for 3 seconds when a vehicle is detected at the entrance and parking is available.
   - Blinks rapidly if a vehicle is detected but no spots are available.
   
4. **LCD Display**:  
   - Shows real-time parking status information:
     - Number of available spots (e.g., "Available: 1/2")
     - Status of each parking spot (e.g., "S1:Full S2:Free")
     - Updates every 500ms for real-time information

5. **System Flow**:  
   - Continuously monitors parking spots with ultrasonic sensors
   - Updates display with current availability
   - Processes entrance detection and controls entry indication
   - Provides visual feedback through entry LED
