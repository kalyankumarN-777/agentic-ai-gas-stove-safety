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
      ((learnedThreshold * l#include "esp_camera.h"

// ================= ESP32-CAM AI Thinker Pins =================
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ================= OUTPUT =================
#define LED_PIN      12
#define SPEAKER_PIN  15

// ================= Presence Detection =================
int presenceThreshold = 18;  // motion detection threshold

// Presence hold time (keeps "human present" true for some seconds)
unsigned long presenceHoldMs = 6000; // 6 seconds
unsigned long lastMotionTime = 0;

// ================= Flame Detection (DIM BLUE FLAME SAFE) =================
int roiMeanThreshold = 90;       // lower = more sensitive
int flickerThreshold = 6;        // lower = more sensitive
int flameStableCountNeeded = 3;  // consecutive frames needed

int prevRoiMean = 0;
int flameConsecutive = 0;

// ================= Learning =================
unsigned long learnedThreshold = 20000;
unsigned long unattendedStart = 0;
int learningCount = 0;

// ================= Frame Sampling =================
uint8_t prevSample[64];
bool prevReady = false;

// ================= Camera Init =================
void initCamera() {
  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk  = XCLK_GPIO_NUM;
  config.pin_pclk  = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href  = HREF_GPIO_NUM;

  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;

  config.pin_pwdn  = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;

  config.frame_size   = FRAMESIZE_QQVGA; // 160x120
  config.jpeg_quality = 12;
  config.fb_count     = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("❌ Camera init failed");
    while (true);
  }

  Serial.println("✅ Camera initialized");
}

// ================= Speaker Tone (NEW API safe) =================
void toneESP(int freq) {
  if (freq <= 0) ledcWriteTone(SPEAKER_PIN, 0);
  else ledcWriteTone(SPEAKER_PIN, freq);
}

void emergencySiren(int seconds) {
  unsigned long start = millis();
  while (millis() - start < (unsigned long)seconds * 1000) {
    for (int f = 700; f <= 1400; f += 40) { toneESP(f); delay(15); }
    for (int f = 1400; f >= 700; f -= 40) { toneESP(f); delay(15); }
  }
  toneESP(0);
}

// ================= Presence sampling (8x8) =================
void makeSample8x8(camera_fb_t *fb, uint8_t sampleOut[64]) {
  int w = 160;
  int idx = 0;

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      int px = x * (w / 8);
      int py = y * (120 / 8);
      int pos = py * w + px;
      sampleOut[idx++] = fb->buf[pos];
    }
  }
}

int diffSample(uint8_t a[64], uint8_t b[64]) {
  long sum = 0;
  for (int i = 0; i < 64; i++) sum += abs(a[i] - b[i]);
  return sum / 64;
}

// ================= ROI Mean Brightness (Flame Detection) =================
int computeRoiMean(camera_fb_t *fb) {
  int w = 160;

  // ROI = bottom-middle area
  int xStart = 50;
  int xEnd   = 110;
  int yStart = 70;
  int yEnd   = 118;

  long sum = 0;
  long count = 0;

  for (int y = yStart; y <= yEnd; y += 3) {
    for (int x = xStart; x <= xEnd; x += 3) {
      int pos = y * w + x;
      sum += fb->buf[pos];
      count++;
    }
  }

  if (count == 0) return 0;
  return (int)(sum / count);
}

// ================= Flame Decision (Dim flame safe) =================
bool detectFlameFromVision(int roiMean) {
  int flicker = abs(roiMean - prevRoiMean);
  prevRoiMean = roiMean;

  bool brightEnough = (roiMean >= roiMeanThreshold);
  bool flickerPresent = (flicker >= flickerThreshold);

  if (brightEnough && flickerPresent) flameConsecutive++;
  else flameConsecutive = 0;

  return (flameConsecutive >= flameStableCountNeeded);
}

// ================= Risk Score =================
int calculateRisk(bool flameOn, bool humanPresent, unsigned long unattendedTime, unsigned long learnedTime, int presenceDiff) {
  int risk = 0;

  if (flameOn) risk += 45;
  if (!humanPresent) risk += 25;
  if (unattendedTime > learnedTime) risk += 20;

  if (presenceDiff < presenceThreshold) risk += 10;

  if (risk > 100) risk = 100;
  return risk;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  ledcAttach(SPEAKER_PIN, 2000, 8);
  toneESP(0);

  initCamera();

  Serial.println("🔥 Agentic AI Started (DIM Flame + Motion Presence + Hold Time)");
}

void loop() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("❌ Capture failed");
    delay(1000);
    return;
  }

  // ---- Presence Detection ----
  uint8_t currentSample[64];
  makeSample8x8(fb, currentSample);

  int presenceDiff = 0;
  bool motionDetected = false;

  if (prevReady) {
    presenceDiff = diffSample(prevSample, currentSample);
    motionDetected = (presenceDiff > presenceThreshold);
  } else {
    prevReady = true;
    motionDetected = true;
  }

  for (int i = 0; i < 64; i++) prevSample[i] = currentSample[i];

  // ---- Presence Hold Logic ----
  unsigned long now = millis();

  if (motionDetected) {
    lastMotionTime = now;
  }

  bool humanPresent = ((now - lastMotionTime) <= presenceHoldMs);

  // ---- Flame Detection (ROI + Flicker) ----
  int roiMean = computeRoiMean(fb);
  bool flameDetected = detectFlameFromVision(roiMean);

  esp_camera_fb_return(fb);

  // ---- Unattended Timer + Learning ----
  unsigned long unattendedTime = 0;

  if (flameDetected && !humanPresent) {
    if (unattendedStart == 0) unattendedStart = now;
    unattendedTime = now - unattendedStart;
  } else {
    if (unattendedStart != 0 && humanPresent) {
      unsigned long duration = now - unattendedStart;

      learnedThreshold =
        ((learnedThreshold * learningCount) + duration) /
        (learningCount + 1);

      learningCount++;

      Serial.print("🧠 Learned threshold updated: ");
      Serial.print(learnedThreshold);
      Serial.println(" ms");
    }
    unattendedStart = 0;
  }

  // ---- Risk Score ----
  int risk = calculateRisk(flameDetected, humanPresent, unattendedTime, learnedThreshold, presenceDiff);

  // ---- Debug Output ----
  Serial.print("ROI Mean: ");
  Serial.print(roiMean);
  Serial.print(" | Flame: ");
  Serial.print(flameDetected ? "YES" : "NO");
  Serial.print(" | MotionDiff: ");
  Serial.print(presenceDiff);
  Serial.print(" | Human(PresenceHold): ");
  Serial.print(humanPresent ? "YES" : "NO");
  Serial.print(" | Risk: ");
  Serial.println(risk);

  // ---- Actions ----
  if (risk >= 75) {
    digitalWrite(LED_PIN, LOW);
    Serial.println("🚨 HIGH RISK! Siren...");
    emergencySiren(4);
  } else {
    digitalWrite(LED_PIN, flameDetected ? HIGH : LOW);
    toneESP(0);
  }

  delay(1200);
}
earningCount) + unattendedDuration) /
      (learningCount + 1);

    learningCount++;
    unattendedStart = 0;

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, HIGH); // Allow gas
  }

  delay(1000);
}
