# FlameGuard AI – Agentic Gas Stove Safety System (ESP32 + MQ-2 + Solenoid)

Overview
FlameGuard AI is a small-scale **Agentic AI-based kitchen safety system** that detects unsafe gas stove conditions and autonomously takes action. The system monitors gas concentration using an **MQ-2 gas sensor** and detects human presence using a **PIR sensor**. If gas is detected while no person is present for a risky duration, the system automatically **shuts off the gas supply using a solenoid valve** (via relay) and triggers an emergency alarm.

This project supports sustainability by reducing gas wastage and preventing accidents.

---

Problem Statement
Gas stoves are often left unattended in households due to distraction or multitasking, leading to:
- Gas wastage
- Fire hazards and accidents
- Unsafe indoor conditions

Most existing solutions only provide alerts and depend on humans to respond. This creates the need for an autonomous system that can detect risk and take action automatically.

---

Proposed Solution
The proposed system acts as an intelligent agent that:
- Detects gas leakage or gas flow using MQ-2 sensor
- Detects human presence using PIR motion sensor
- Computes a risk score based on gas level, unattended duration, and learned behavior
- Automatically cuts off gas using a relay-controlled solenoid valve
- Triggers an emergency buzzer alarm

---

Why Agentic AI?
The system follows the Agentic AI loop:

**Sense → Think → Act → Learn**
- **Sense:** reads gas levels + presence data
- **Think:** evaluates risk score and unattended time
- **Act:** cuts gas + triggers alarm
- **Learn:** adapts safe unattended threshold over time

---

SDG Alignment
**Primary SDG:** SDG 12 – Responsible Consumption and Production  
**Secondary SDGs:** SDG 3 – Good Health and Well-being, SDG 7 – Affordable and Clean Energy

---

Components Used
- ESP32 Development Board
- MQ-2 Gas Sensor (Analog output)
- PIR Motion Sensor
- Relay Module (1-channel, 5V)
- Solenoid Gas Valve (gas hose control)
- Buzzer / Speaker (alarm)
- Jumper wires + breadboard
- 5V / 12V power supply (based on solenoid)

---

Connections (Example Wiring)
### MQ-2 Gas Sensor
- VCC → 5V
- GND → GND
- AO  → ESP32 GPIO34 (Analog)

PIR Sensor
- VCC → 5V
- GND → GND
- OUT → ESP32 GPIO13

Relay (Solenoid control)
- Relay IN  → ESP32 GPIO26
- Relay VCC → 5V
- Relay GND → GND

Buzzer
- Buzzer + → ESP32 GPIO25
- Buzzer – → GND

> Note: Solenoid power must match its rating (commonly 12V).  
> Use relay contacts (COM/NO) to control solenoid power safely.

---

How It Works
1. Detect gas presence using MQ-2 sensor
2. Detect human presence using PIR sensor
3. If gas is detected and no one is present:
   - Start unattended timer
   - Compute risk score
4. If risk becomes high:
   - Cut off gas supply using solenoid valve
   - Activate emergency alarm
5. System learns user behavior and adapts safe unattended duration

---

Responsible AI Considerations
- No personal data collection
- No camera surveillance
- Transparent decision logic (risk scoring + thresholds)
- Local processing only

---

Future Enhancements
- Add ESP32-CAM for vision-based presence detection
- Add Telegram/mobile notifications
- Add multi-burner stove monitoring
- Add cloud dashboard for analytics

---
