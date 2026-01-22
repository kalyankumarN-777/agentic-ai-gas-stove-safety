#define GAS_SENSOR_PIN 34
#define PIR_SENSOR_PIN 13
#define RELAY_PIN 26
#define BUZZER_PIN 25

int gasThreshold = 300;

// AI learning variables
unsigned long unattendedStart = 0;
unsigned long learnedThreshold = 15000; // initial safe time (15 sec)
int learningCount = 0;

void setup() {
  Serial.begin(9600);

  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(PIR_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Gas ON initially
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("Agentic AI Gas Stove Safety System Started");
}

void loop() {
  int gasValue = analogRead(GAS_SENSOR_PIN);
  int motion = digitalRead(PIR_SENSOR_PIN);

  // Gas detected and no human present
  if (gasValue > gasThreshold && motion == LOW) {

    if (unattendedStart == 0) {
      unattendedStart = millis();  // start timing
    }

    // AI decision using learned threshold
    if (millis() - unattendedStart > learnedThreshold) {
      digitalWrite(RELAY_PIN, LOW);   // Cut gas
      digitalWrite(BUZZER_PIN, HIGH); // Alert
    }

  }
  // Human returns → learn behavior
  else if (motion == HIGH && unattendedStart != 0) {

    unsigned long unattendedDuration = millis() - unattendedStart;

    // Experience-based learning (moving average)
    learnedThreshold =
      ((learnedThreshold * learningCount) + unattendedDuration) /
      (learningCount + 1);

    learningCount++;
    unattendedStart = 0;

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, HIGH); // Allow gas
  }

  delay(1000);
}
