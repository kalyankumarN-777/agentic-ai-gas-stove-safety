Agentic AI-Based Unattended Gas Stove Safety System (ESP32-CAM)

Overview
This project implements an **Agentic AI-based kitchen safety system** using the **ESP32-CAM** module.  
The system detects whether the stove flame is ON (vision-based dim flame detection) and checks whether a person is present near the stove (vision-based motion detection). If the stove is left unattended, the system computes a **risk score**, learns user behavior, and triggers an emergency siren alert.

This is a **small-scale and sustainable prototype** designed for real-world household safety.

---

Problem Statement
Gas stoves are often left unattended in households due to distractions or multitasking. This can lead to:
- Fuel wastage
- Fire hazards
- Unsafe cooking environments

Most existing systems only provide alerts and lack autonomous decision-making and action.

---

Proposed Solution
The system acts as an intelligent agent that:
- **Perceives** the kitchen environment using the ESP32-CAM camera
- **Detects stove flame** using ROI brightness + flicker analysis (works with dim blue flame)
- **Detects human presence** using motion-based frame difference analysis
- **Calculates risk score (0–100)**
- **Learns normal unattended time** and adapts safety timing
- **Acts autonomously** using an emergency siren tone and an LED indicator output

---

Why Agentic AI?
This system follows the Agentic AI loop:

**Sense → Think → Act → Learn**

- **Sense:** captures camera frames continuously
- **Think:** identifies flame + presence + risk score
- **Act:** triggers emergency siren when risk is high
- **Learn:** adapts unattended threshold based on user behavior

---

How Human Presence is Detected
The system does not use face recognition. Instead, it detects **human presence using motion-based vision**:
1. The ESP32-CAM captures consecutive grayscale frames.
2. A small 8×8 sample (64 pixels) is extracted from each frame.
3. The average difference between the current and previous frame samples is calculated.
4. If the difference exceeds a threshold, motion is detected and human presence is assumed.

To reduce false "no presence" cases, the system also keeps a short **presence hold time**, meaning it will treat the user as present for a few seconds even if motion becomes low.

---

SDG Alignment
**Primary SDG:** SDG 12 – Responsible Consumption and Production  
**Secondary SDGs:** SDG 3 – Good Health and Well-being, SDG 7 – Affordable and Clean Energy

---

Components Used
- ESP32-CAM (AI Thinker)
- 5mm LED + 220Ω/330Ω resistor (indicator)
- Passive buzzer / small speaker (emergency siren)
- FTDI USB-to-TTL programmer (uploading)
- Jumper wires + breadboard
- 5V power supply (recommended 5V 2A)

---

Connections
### LED (Indicator)
GPIO12 → 220Ω resistor → LED (+)  
LED (–) → GND

### Speaker/Buzzer (Emergency Tone)
GPIO15 → Speaker (+)  
Speaker (–) → GND

---

How to Run
1. Upload code to ESP32-CAM using FTDI.
2. Remove GPIO0–GND after uploading.
3. Press RST to run.
4. Open Serial Monitor at **115200 baud**.
5. Demo:
   - Use a phone flashlight/candle near the camera to simulate flame ON.
   - Move away from camera to simulate unattended stove.
   - Risk score increases → siren plays.

---

Sustainability Impact
- Helps prevent unattended stove hazards
- Encourages responsible energy and fuel usage
- Low-cost, low-power, on-device processing

---

Responsible AI Considerations
- No personal data storage
- No face identification
- On-device processing only
- Transparent decision logic (explainable thresholds)

---

Future Enhancements
- Add real gas sensor (MQ-2) for gas leakage detection
- Add relay + solenoid valve for real gas shutoff
- Add mobile alerts (Telegram/Blynk)
- Multi-burner multi-agent system

---
