#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// ====== Blynk Credentials ======
#define BLYNK_TEMPLATE_ID "put yours here"
#define BLYNK_TEMPLATE_NAME "put yours here"
#define BLYNK_AUTH_TOKEN "put yours here"

// ====== Wi-Fi Networks ======
const char* ssids[] = {"ssid1", "ssid2", "ssid3"};
const char* passes[] = {"password1", "password2", "password3"};
const int wifiCount = sizeof(ssids) / sizeof(ssids[0]);

// ====== Motor Control Pins ======
#define OPEN_PIN 18
#define STOP_PIN 19
#define CLOSE_PIN 21

// ====== Limit Switch Pins ======
#define OPEN_LS_PIN 32
#define CLOSE_LS_PIN 33

// ====== NEW: IR Sensor Pin ======
// Connect IR sensor OUT to GPIO 23.
// Most IR sensors output 0 (LOW) when an object is detected.
#define IR_SENSOR_PIN 23 

BlynkTimer timer;

// ====== Flags ======
volatile bool openTriggered = false;
volatile bool closeTriggered = false;
bool openBlocked = false;
bool closeBlocked = false;

// To avoid IR continuously triggering while door is already moving
bool isOpeningByIR = false; 

// -------- Interrupt Service Routines --------
void IRAM_ATTR onOpenSwitch() { openTriggered = true; }
void IRAM_ATTR onCloseSwitch() { closeTriggered = true; }

// -------- Wi-Fi Connect --------
void connectWiFi() {
  for (int i = 0; i < wifiCount; i++) {
    Serial.printf("Trying Wi-Fi: %s\\n", ssids[i]);
    WiFi.begin(ssids[i], passes[i]);
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 7000) {
      delay(300);
      Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.printf("\\nConnected to: %s\\n", ssids[i]);
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      return;
    }
    Serial.println("\\nFailed, trying next...");
  }
  Serial.println("No Wi-Fi networks connected!");
}

// -------- Re-enable after 0.5 s --------
void releaseOpenBlock() {
  openBlocked = false;
  isOpeningByIR = false; // Reset IR state logic
  Serial.println("OPEN re-enabled");
}
void releaseCloseBlock() {
  closeBlocked = false;
  Serial.println("CLOSE re-enabled");
}

// -------- Blynk Button Controls --------
// OPEN = V0 (Switch), STOP = V1 (Push), CLOSE = V2 (Switch)
BLYNK_WRITE(V0) {  // OPEN
  int v = param.asInt();
  if (v && !openBlocked) {
    digitalWrite(OPEN_PIN, HIGH);
    digitalWrite(CLOSE_PIN, LOW);
    digitalWrite(STOP_PIN, LOW);
    Blynk.virtualWrite(V2, 0);
  } else {
    digitalWrite(OPEN_PIN, LOW);
  }
  Serial.println(v ? "OPEN ON" : "OPEN OFF");
}

BLYNK_WRITE(V1) {  // STOP (Push)
  int v = param.asInt();
  if (v) {
    digitalWrite(STOP_PIN, HIGH);
    digitalWrite(OPEN_PIN, LOW);
    digitalWrite(CLOSE_PIN, LOW);
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V2, 0);
    isOpeningByIR = false; // Stop overrides IR
  } else digitalWrite(STOP_PIN, LOW);
  Serial.println(v ? "STOP Pressed" : "STOP Released");
}

BLYNK_WRITE(V2) {  // CLOSE
  int v = param.asInt();
  if (v && !closeBlocked) {
    digitalWrite(CLOSE_PIN, HIGH);
    digitalWrite(OPEN_PIN, LOW);
    digitalWrite(STOP_PIN, LOW);
    Blynk.virtualWrite(V0, 0);
  } else {
    digitalWrite(CLOSE_PIN, LOW);
  }
  Serial.println(v ? "CLOSE ON" : "CLOSE OFF");
}

// -------- Connection Watchdog --------
void checkConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi dropped -> reconnecting...");
    connectWiFi();
  }
  if (!Blynk.connected()) {
    Serial.println("Blynk dropped -> reconnecting...");
    Blynk.connect();
  }
}

// -------- Helper: Start Opening Door --------
void triggerOpen() {
    // Only open if not blocked and not currently opening
    if(!openBlocked && digitalRead(OPEN_PIN) == LOW) {
        Serial.println("Auto-Opening Door via IR Sensor");
        digitalWrite(OPEN_PIN, HIGH);
        digitalWrite(CLOSE_PIN, LOW);
        digitalWrite(STOP_PIN, LOW);
        
        // Update Blynk Widgets
        Blynk.virtualWrite(V0, 1); // Set Open Button ON
        Blynk.virtualWrite(V2, 0); // Set Close Button OFF
        
        isOpeningByIR = true;
    }
}

// -------- Setup --------
void setup() {
  Serial.begin(115200);

  pinMode(OPEN_PIN, OUTPUT);
  pinMode(STOP_PIN, OUTPUT);
  pinMode(CLOSE_PIN, OUTPUT);
  digitalWrite(OPEN_PIN, LOW);
  digitalWrite(STOP_PIN, LOW);
  digitalWrite(CLOSE_PIN, LOW);

  pinMode(OPEN_LS_PIN, INPUT_PULLDOWN);
  pinMode(CLOSE_LS_PIN, INPUT_PULLDOWN);
  
  // NEW: IR Sensor Setup
  pinMode(IR_SENSOR_PIN, INPUT); 

  attachInterrupt(digitalPinToInterrupt(OPEN_LS_PIN), onOpenSwitch, RISING);
  attachInterrupt(digitalPinToInterrupt(CLOSE_LS_PIN), onCloseSwitch, RISING);

  connectWiFi();
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();

  timer.setInterval(5000L, checkConnection);
  Serial.println("System ready. IR Sensor monitoring on GPIO 23.");
}

// -------- Loop --------
void loop() {
  // ====== IR Sensor Logic ======
  // Most IR modules are active LOW (0 when object detected)
  int irState = digitalRead(IR_SENSOR_PIN);
  
  if (irState == LOW) { 
      // If sensor detects car/object, trigger OPEN
      triggerOpen();
  }

  // ====== Limit Switch Logic ======
  if (openTriggered && !openBlocked) {
    openTriggered = false;
    openBlocked = true;
    digitalWrite(OPEN_PIN, LOW);
    Blynk.virtualWrite(V0, 0);
    isOpeningByIR = false;
    Serial.println("OPEN limit triggered -> motor OFF (0.5 s brake)");
    timer.setTimeout(500, releaseOpenBlock);
  }

  if (closeTriggered && !closeBlocked) {
    closeTriggered = false;
    closeBlocked = true;
    digitalWrite(CLOSE_PIN, LOW);
    Blynk.virtualWrite(V2, 0);
    Serial.println("CLOSE limit triggered -> motor OFF (0.5 s brake)");
    timer.setTimeout(500, releaseCloseBlock);
  }

  Blynk.run();
  timer.run();
};
