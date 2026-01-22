#define GAS_SENSOR_PIN 34
#define PIR_SENSOR_PIN 13
#define RELAY_PIN 26
#define BUZZER_PIN 25

int gasThreshold = 300;
unsigned long learnedThreshold = 15000;
unsigned long unattendedStart = 0;
int learningCount = 0;

void setup() {
  Serial.begin(9600);

  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(PIR_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  int gasValue = analogRead(GAS_SENSOR_PIN);
  int motion = digitalRead(PIR_SENSOR_PIN);

  if (gasValue > gasThreshold && motion == LOW) {
    if (unattendedStart == 0) {
      unattendedStart = millis();
    }

    if (millis() - unattendedStart > learnedThreshold) {
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(BUZZER_PIN, HIGH);
    }

  } else if (motion == HIGH && unattendedStart != 0) {
    unsigned long unattendedDuration = millis() - unattendedStart;

    learnedThreshold =
      ((learnedThreshold * learningCount) + unattendedDuration) /
      (learningCount + 1);

    learningCount++;
    unattendedStart = 0;

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, HIGH);
  }

  delay(1000);
}
