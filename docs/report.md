# FlameGuard AI – Agentic Gas Stove Safety System

## 1. Introduction
Unattended gas stove usage is a common household issue that can lead to hazardous accidents, gas wastage, and unsafe indoor environments. Many households rely on human attention or basic alarms, which may fail during emergencies. To overcome this limitation, this project introduces an Agentic AI-based gas stove safety system that can monitor risk conditions and take autonomous action.

## 2. Problem Statement
Gas leakage or unattended stove operation can cause serious safety risks including fire hazards and explosions. Existing solutions mostly provide alerts without autonomous control. Hence, a system is required that can detect gas presence, confirm human absence, and automatically shut off gas supply to prevent accidents while promoting responsible energy usage.

## 3. Objectives
The objectives of this project are:
- Detect gas leakage or gas flow using MQ-2 sensor
- Detect human presence using PIR sensor
- Measure unattended duration when gas is detected without human presence
- Compute a risk score to classify safety condition
- Automatically shut off gas supply using solenoid valve
- Trigger emergency alarm and enhance safety
- Learn user behavior and adapt safety threshold

## 4. SDG Alignment
This project aligns with:
- SDG 12 (Responsible Consumption and Production): prevents fuel wastage
- SDG 3 (Good Health and Well-being): improves household safety
- SDG 7 (Affordable and Clean Energy): supports efficient energy usage

## 5. System Architecture
The system contains the following modules:
1. Sensing module (MQ-2 + PIR)
2. Context understanding module (gas + presence)
3. Agentic AI decision module (risk scoring)
4. Action module (relay + solenoid + alarm)
5. Learning module (adaptive threshold)

## 6. Working Methodology
The system continuously monitors MQ-2 analog sensor values and PIR presence signals. If gas levels rise above a threshold and no presence is detected, the system starts a timer. Instead of shutting off gas immediately, it evaluates unattended duration and computes a risk score. If risk exceeds a limit, the system triggers a relay to shut the solenoid valve and activates a buzzer for emergency alert.

## 7. Agentic AI Decision Logic
The system behaves as an autonomous agent following Sense–Think–Act–Learn:
- Sense: gas level + presence signal
- Think: unattended duration + risk score evaluation
- Act: close solenoid valve + alarm
- Learn: update safe unattended time based on observed user behavior

## 8. Learning Module
The learning module updates the safe unattended time threshold using a moving average. If the user returns within a certain time, the learned threshold increases slightly, reducing false alarms. If unattended durations become longer, the agent reduces tolerance, increasing safety.

## 9. Sustainability Impact
This system reduces gas wastage and prevents accidents. It promotes responsible energy consumption and supports sustainable living.

## 10. Conclusion
FlameGuard AI demonstrates how Agentic AI can be implemented in a small-scale real-world project to improve household safety and sustainability. By integrating sensor monitoring, autonomous decision making, adaptive learning, and emergency control, the solution provides an effective safety system for homes and community kitchens.
