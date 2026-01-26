# Agentic AI-Based Unattended Gas Stove Safety System (ESP32-CAM)

## 1. Introduction
Household cooking is a daily activity, but it also involves safety risks when gas stoves are left unattended. Many accidents occur due to distractions, multitasking, or forgetfulness. Even a short period of unattended cooking can lead to hazardous situations such as overheating, fire hazards, and unsafe kitchen environments. Along with safety, continuous unattended operation wastes fuel and contributes to unnecessary energy consumption.

This project proposes a small-scale **Agentic AI-based kitchen safety system** using the **ESP32-CAM module**. The system monitors the cooking area using camera-based sensing and makes autonomous decisions to trigger emergency warnings when the stove is detected to be active while no person is present.

This solution is designed as a **sustainable and low-cost prototype**, suitable for internship-level implementation and real-world extension.

---

## 2. Problem Statement
Gas stoves are often left unattended in households, leading to:
- potential fire hazards,
- unsafe indoor cooking conditions,
- and energy/resource wastage.

Most existing systems provide only alerts but do not make autonomous decisions based on environmental context and user behavior. Therefore, there is a need for an intelligent system that can:
1. detect whether the stove is active,
2. detect whether a person is present,
3. estimate unattended duration,
4. take action automatically when risk becomes high.

---

## 3. Objectives of the Project
The main objectives of this project are:

1. **Flame/Active Stove Detection (Vision-Based):**  
   Detect stove flame activity using the ESP32-CAM camera without external flame sensors.

2. **Human Presence Detection (Vision-Based):**  
   Detect whether a person is present near the stove using motion analysis.

3. **Autonomous Risk-Based Decision Making:**  
   Calculate a risk score and decide when conditions become unsafe.

4. **Emergency Alert System:**  
   Trigger an emergency siren tone and indicator output for demo.

5. **Learning and Adaptation:**  
   Update the unattended-time threshold using experience-based learning to adapt to user behavior.

---

## 4. Sustainable Development Goals (SDG) Alignment
This project supports sustainability goals by improving safety and reducing unnecessary energy usage:

- **SDG 12: Responsible Consumption and Production (Primary)**  
  Prevents waste of fuel/energy due to unattended operation.

- **SDG 3: Good Health and Well-being**  
  Reduces safety risks inside homes and kitchens.

- **SDG 7: Affordable and Clean Energy**  
  Supports efficient energy usage and reduces avoidable losses.

---

## 5. System Overview
The system operates as an **Agentic AI agent**. Instead of only triggering an alert when something happens, it follows an intelligent decision cycle.

The system continuously:
1. captures camera frames,
2. checks if flame is present,
3. checks if a person is present,
4. measures unattended duration,
5. computes risk score,
6. triggers emergency siren when required,
7. learns user behavior and adapts thresholds.

---

## 6. Agentic AI Workflow (Sense → Think → Act → Learn)

### 6.1 Sense (Perception)
The ESP32-CAM captures grayscale image frames and extracts meaningful measurements:
- motion difference between frames (presence detection),
- ROI brightness and flicker changes (flame detection).

### 6.2 Think (Reasoning)
Using the sensed values, the agent performs reasoning such as:
- "Is the stove active?"
- "Is the kitchen unattended?"
- "How long has it been unattended?"
- "Is this condition risky based on learned behavior?"

### 6.3 Act (Autonomous Action)
When risk exceeds a threshold:
- the system triggers a siren sound,
- the indicator LED is turned OFF (to represent shutdown/unsafe state).

### 6.4 Learn (Adaptation)
The system updates a **learned safe unattended time threshold** based on past observations. This reduces false alarms and makes the system adaptive over time.

---

## 7. Hardware Components Used
This prototype uses minimal and safe components:

1. **ESP32-CAM (AI Thinker):**
   Main controller and camera sensor.

2. **5mm LED + Resistor (220Ω/330Ω):**
   Used as an indicator output for demonstration.

3. **Passive Buzzer / Small Speaker:**
   Used to generate emergency siren tones.

4. **FTDI USB-to-TTL Programmer:**
   Used to upload code into ESP32-CAM.

5. **5V Power Supply (recommended 5V 2A):**
   ESP32-CAM requires stable power to run camera processing.

6. **Breadboard + Jumper Wires:**
   For circuit connections.

---

## 8. Working Methodology and Modules

### Module 1: Vision-Based Human Presence Detection
Human presence is detected using motion analysis between consecutive frames. The ESP32-CAM captures a frame sample and compares it with the previous sample.

#### Steps
1. Capture current grayscale frame.
2. Extract a small 8×8 pixel sample (64 pixels).
3. Compare it with the previous sample.
4. Compute average difference value (**motion difference score**).

If the motion difference score exceeds a threshold, motion is detected and the system assumes human presence.

#### Presence Hold Logic (Stability Improvement)
A limitation of motion detection is that a person may stand still. To reduce false absence detection, a **presence hold time** is added:
- If motion is detected once, the system keeps "human present" for a few seconds.
- This improves reliability for real kitchen scenarios.

---

### Module 2: Vision-Based Flame Detection (Dim Blue Flame Compatible)
Gas stove flames can be dim and blue, so brightness-only detection may fail. This project improves flame detection using:

1. **ROI (Region of Interest):**  
   The system checks only the bottom-middle region of the frame where the stove flame is expected.  
   This reduces false triggers from ceiling lights or other room brightness.

2. **Brightness Mean:**  
   The average brightness of the ROI region is computed.

3. **Flicker Detection:**  
   Flames naturally flicker, so brightness changes over time.  
   If the ROI brightness changes beyond a flicker threshold, flame is more likely present.

#### Final Flame Decision
Flame is detected only when:
- ROI mean brightness is above a minimum threshold, AND
- flicker behavior is observed for multiple consecutive frames.

This reduces false positives from steady room lights.

---

### Module 3: Unattended Timer
The unattended timer starts only when:
- flame is detected, AND
- human presence is absent.

If the person returns, the timer resets.

This ensures the system reacts only to truly unattended flame conditions.

---

### Module 4: Risk Score Computation (0–100)
Instead of making decisions using only one rule, the system computes a **risk score**.

Risk score considers:
- flame detected (high risk factor),
- absence of human (high risk factor),
- unattended time exceeding learned safe time,
- low motion score.

The final score is clamped between 0 and 100.

A typical risk interpretation is:
- **0–40:** Safe
- **40–74:** Warning zone
- **75–100:** Emergency zone

---

### Module 5: Emergency Alert and Output
When the risk score reaches emergency level:
- the LED output is turned OFF (represents shutdown/unsafe state),
- a siren tone is generated using a speaker/buzzer.

This provides an immediate warning signal for the user.

---

### Module 6: Learning and Adaptation
The project includes experience-based learning to adjust the safe unattended time.

When the user returns after leaving the stove unattended, the system updates a moving average:
- the learned threshold increases/decreases based on past unattended duration.

This makes the system adaptive and personalized over time.

---

## 9. Results and Expected Output
The system is expected to show the following behavior:
1. When flame is not detected → system stays idle.
2. When flame is detected and motion is present → safe cooking condition.
3. When flame is detected and no motion is found → system starts unattended timer.
4. If unattended duration becomes long and risk crosses threshold → siren triggers.
5. If the user returns in time → system resets and learns safe behavior.

Serial monitor output helps debug flame and presence values, making the system explainable and transparent.

---

## 10. Tuning Parameters (Important for Practical Testing)
Different rooms and lighting conditions require tuning thresholds.

### Parameters to tune:
- `presenceThreshold`: motion sensitivity
- `presenceHoldMs`: how long to keep presence true after motion
- `roiMeanThreshold`: ROI minimum brightness for flame
- `flickerThreshold`: minimum flicker change for flame
- `flameStableCountNeeded`: number of frames required

---

## 11. Sustainability Impact
This system supports sustainability by:
- reducing unsafe cooking conditions,
- preventing potential accidents,
- promoting responsible fuel usage.

A low-cost embedded solution like this encourages sustainable living practices by improving safety at the household level.

---

## 12. Responsible AI Considerations
This project follows responsible AI principles:
- No face recognition or identity tracking is performed.
- No personal data or images are stored.
- Processing is local (edge-based AI), improving privacy.
- Decision-making is transparent (threshold + risk-based system).

---

## 13. Limitations
The main limitations include:
- Presence detection is motion-based; very still users may not be detected without hold logic.
- Flame detection may vary with camera angle and lighting conditions.
- The prototype uses LED and siren for demonstration instead of actual gas valve control.

---

## 14. Future Enhancements
Possible improvements include:
- Integrating MQ-2 gas sensor for gas leakage detection.
- Adding relay + solenoid valve for real gas cutoff control.
- Adding Telegram/Blynk mobile alerts.
- Multi-burner monitoring using multiple agents.
- Improved flame detection using lightweight ML models if required.

---

## 15. Conclusion
This project demonstrates a real-world sustainability-focused application of Agentic AI using the ESP32-CAM. By combining camera-based flame detection, human presence estimation, risk scoring, autonomous emergency alerts, and experience-based learning, the system works as an intelligent kitchen safety agent. The solution is scalable, low-cost, explainable, and aligned with sustainability goals.
