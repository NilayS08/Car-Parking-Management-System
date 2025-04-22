Perfect — thanks for the updated details! Here's a clean and clear version of your README sections tailored exactly to your setup (1 ultrasonic sensor for parking, IR for entry, LCD screen, no buzzer, etc.):

---

## 📝 **Problem Statement**

In crowded urban environments, efficient and automated parking management is a growing need. Manual monitoring is time-consuming and error-prone. This project addresses the problem by designing a compact **Smart Parking Management System** that automates vehicle detection, entry control, and availability indication using sensors and microcontroller-based logic.

---

## 🎯 **Objective**

To develop a **low-cost, Arduino-based parking system** that:
- Automatically detects incoming vehicles at the gate
- Monitors two parking spots using a **single ultrasonic sensor**
- Displays parking status on an **LCD screen**
- Controls a **servo gate** based on availability
- Uses **LEDs** to visually indicate whether parking is available or full

---

## 🔩 **Components Used**

| Component                  | Quantity |
|---------------------------|----------|
| Arduino Uno/Nano          | 1        |
| Ultrasonic Sensor (HC-SR04) | 1        |
| IR Sensor                 | 1        |
| Servo Motor               | 1        |
| 16x2 LCD Screen           | 1        |
| Red LED                   | 1        |
| Green LED                 | 1        |
| Breadboard + Jumper Wires | As required |
| Power Source (USB or Battery) | 1    |

---

## ⚙️ **How It Works**

1. **IR Sensor (Gate Detection)**:  
   - Detects when a car is at the entrance.
   - If a parking spot is available, triggers the **servo motor** to open the gate.

2. **Ultrasonic Sensor (Parking Monitoring)**:  
   - Positioned to monitor **both parking slots**.
   - Measures distance to detect presence of vehicles in slots.
   - Based on distance thresholds, determines if slots are **occupied or free**.

3. **LED Indicators**:  
   - **Green LED** turns on when at least one slot is free.
   - **Red LED** turns on when both slots are occupied.

4. **LCD Display**:  
   - Shows real-time parking status, such as:
     - “1 Slot Available”
     - “Parking Full”
     - “Car Detected – Opening Gate”

5. **Servo Motor (Gate Control)**:  
   - Opens when a car is detected and parking is available.
   - Stays closed if the parking is full.
