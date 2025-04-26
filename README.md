## 📝 **Problem Statement**
Urban areas suffer from limited parking spaces and inefficient manual parking management. Traditional methods are prone to human error and lack real-time updates. This project introduces a **Smart Automated Parking System** using Arduino to streamline vehicle detection, space monitoring, and entry control, ensuring better space utilization and energy efficiency.

## 🎯 **Objective**
To design a **compact, cost-effective Arduino-based parking solution** that:
- Detects approaching vehicles using an **IR sensor**
- Monitors two individual parking spots with **ultrasonic sensors**
- Displays real-time parking availability on an **LCD screen**
- Manages entry using a **status LED**
- Adjusts **LCD brightness based on ambient light** using an **LDR sensor** to conserve power

## 🔩 **Components Used**
| Component                   | Quantity |
|----------------------------|----------|
| Arduino Uno/Nano           | 1        |
| Ultrasonic Sensor (HC-SR04)| 2        |
| IR Sensor                  | 1        |
| Entry Status LED           | 1        |
| I2C 16x2 LCD Display       | 1        |
| LDR (Light Dependent Resistor) | 1   |
| Breadboard & Jumper Wires  | As needed |
| Power Source (USB/Battery) | 1        |

## ⚙️ **How It Works**
1. **Vehicle Detection (IR Sensor)**:  
   - Detects vehicles at the entrance.
   - If spots are available, allows entry using a green LED.
   - If full, the LED blinks to indicate no entry.

2. **Slot Monitoring (Ultrasonic Sensors)**:  
   - Two sensors are assigned to two parking spaces.
   - Measures distance to detect if a car is present (threshold: 15 cm).
   - Updates internal status for each slot.

3. **Ambient Light Detection (LDR Sensor)**:  
   - Detects if it's day or night.
   - Increases LCD brightness during the day.
   - Reduces brightness at night to **save power**.

4. **Entry LED**:  
   - Lights up for 3 seconds when entry is allowed.
   - Blinks continuously if the system is full.

5. **LCD Display**:  
   - Shows real-time availability:
     - Example: “Available: 1/2”
     - Slot status: “S1:Full S2:Free”
   - Refreshes every 500 milliseconds.

6. **System Flow Summary**:  
   - Detects incoming vehicle → Checks space availability → Updates LED and LCD → Manages LCD brightness via LDR

![Automatic Parking system (4)](https://github.com/user-attachments/assets/e97b3a38-0fe2-4b72-b50a-0c9eba7057f8)

## 🚀 **Future Enhancements**
- **Mobile App Integration**: Monitor and reserve parking spots remotely.  
- **Scalable Slot Expansion**: Add more parking slots with additional sensors or advanced microcontrollers.  
- **Automated Entry Gate**: Control gate barriers using servo motors based on availability.  
- **Cloud Connectivity**: Push parking data to the cloud for real-time remote access and analytics.  
- **Solar Power Support**: Integrate solar panels for sustainable, off-grid operation.  
- **License Plate Recognition (Optional)**: Add a camera module for vehicle logging and enhanced security.  
- **Improved Brightness Control**: Fine-tune LCD brightness dynamically based on ambient light levels.
