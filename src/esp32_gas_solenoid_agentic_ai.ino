/******************************************************
 FlameGuard AI – Agentic Gas Stove Safety System
 Hardware: ESP32 + MQ-2 + PIR + Relay + Solenoid + Buzzer
******************************************************/

#define MQ2_PIN 34          // Analog pin (ESP32 ADC)
#define PIR_PIN 13          // Digital pin for PIR sensor
#define RELAY_PIN 26        // Relay to control solenoid
#define BUZZER_PIN 25       // Buzzer for emergency alert

// Thresholds (tune after testing)
int gasThreshold = 300;      // Adjust based on MQ-2 readings
int riskLimit = 75;          // Risk score threshold to trigger shutdown

// Learning parameters
unsigned long learnedThreshold = 20000; // 20 sec initial safe unattended duration
int learningCount = 0;

// Timing variables
unsigned long unattendedStart = 0;

// Buzzer beep function
void emergencyBeep(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(250);
    digitalWrite(BUZZER_PIN, LOW);
    delay(250);
  }
}

// Risk scoring function (0 - 100)
int calculateRisk(int gasValue, bool humanPresent, unsigned long unattendedTime, unsigned long learnedTime) {
  int risk = 0;

  // Gas intensity factor
  if (gasValue > gasThreshold + 150) risk += 40;
  else if (gasValue > gasThreshold) risk += 25;

  // Presence factor
  if (!humanPresent) risk += 25;

  // Unattended time factor
  if (unattendedTime > learnedTime) risk += 30;
  else risk += 10;

  if (risk > 100) risk = 100;
  return risk;
}

void setup() {
  Serial.begin(9600);

  pinMode(MQ2_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Default: Gas allowed (Relay depends on module)
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("✅ FlameGuard AI Started (ESP32 + MQ-2 + PIR + Solenoid)");
}

void loop() {
  int gasValue = analogRead(MQ2_PIN);
  bool humanPresent = (digitalRead(PIR_PIN) == HIGH);

  unsigned long now = millis();
  unsigned long unattendedTime = 0;

  bool gasDetected = (gasValue > gasThreshold);

  // Case: Gas detected but no human
  if (gasDetected && !humanPresent) {

    if (unattendedStart == 0) {
      unattendedStart = now;
      Serial.println("⚠️ Gas detected and no human present → monitoring...");
    }

    unattendedTime = now - unattendedStart;

    int riskScore = calculateRisk(gasValue, humanPresent, unattendedTime, learnedThreshold);

    Serial.print("Gas=");
    Serial.print(gasValue);
    Serial.print(" | Human=NO");
    Serial.print(" | Unattended(ms)=");
    Serial.print(unattendedTime);
    Serial.print(" | Learned(ms)=");
    Serial.print(learnedThreshold);
    Serial.print(" | Risk=");
    Serial.println(riskScore);

    // Emergency Action
    if (riskScore >= riskLimit) {
      Serial.println("🚨 HIGH RISK! Closing solenoid + Alarm!");
      digitalWrite(RELAY_PIN, LOW);   // Close solenoid valve
      emergencyBeep(10);
    }

  } 
  // Safe / Human present / Gas off
  else {

    // Learning: if user returns after being away
    if (unattendedStart != 0 && humanPresent) {
      unsigned long duration = now - unattendedStart;

      learnedThreshold =
        ((learnedThreshold * learningCount) + duration) /
        (learningCount + 1);

      learningCount++;

      Serial.print("🧠 Learned safe unattended time updated to: ");
      Serial.print(learnedThreshold);
      Serial.println(" ms");
    }

    unattendedStart = 0;

    // Normal state: allow gas, buzzer off
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);

    Serial.print("Gas=");
    Serial.print(gasValue);
    Serial.print(" | Human=");
    Serial.print(humanPresent ? "YES" : "NO");
    Serial.println(" | Status=SAFE");
  }

  delay(1000);
}
