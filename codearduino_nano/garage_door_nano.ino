#define OPEN_BTN 2
#define CLOSE_BTN 3
#define STOP_BTN 4
#define LS1_OPEN 5
#define LS2_CLOSE 6
#define MOTOR_IN1 7      // Motor driver direction pin 1
#define MOTOR_IN2 8      // Motor driver direction pin 2
#define EN_PIN 9          // Enable pin (PWM control)
#define LAMP_AJAR 10
#define LAMP_OPEN 11
#define LAMP_SHUT 12

// ====== Motor Speed Control ======
const int openSpeed = 150;   // Adjust 0–255 (150 ≈ ~60% power)
const int closeSpeed = 130;  // Slightly slower for safety

// ====== State Variables ======
bool opening = false;
bool closing = false;
bool stopped = false;

// ====== Timing for Lamp Flashing ======
unsigned long previousMillis = 0;
const int flashInterval = 300;      // Fast flashing for Open/Shut lamps
const int slowFlashInterval = 1200; // Slow flashing for Ajar lamp
bool flashState = false;

void setup() {
pinMode(OPEN_BTN, INPUT_PULLUP);
pinMode(CLOSE_BTN, INPUT_PULLUP);
pinMode(STOP_BTN, INPUT_PULLUP);
pinMode(LS1_OPEN, INPUT_PULLUP);
pinMode(LS2_CLOSE, INPUT_PULLUP);

pinMode(MOTOR_IN1, OUTPUT);
pinMode(MOTOR_IN2, OUTPUT);
pinMode(EN_PIN, OUTPUT);

pinMode(LAMP_AJAR, OUTPUT);
pinMode(LAMP_OPEN, OUTPUT);
pinMode(LAMP_SHUT, OUTPUT);

stopDoor(); // Start in stopped state
Serial.begin(9600);
}

void loop() {
// Read buttons (active LOW)
bool openBtn = !digitalRead(OPEN_BTN);
bool closeBtn = !digitalRead(CLOSE_BTN);
bool stopBtn = !digitalRead(STOP_BTN);

bool lsOpen = !digitalRead(LS1_OPEN);   // Active LOW (pressed)
bool lsClose = !digitalRead(LS2_CLOSE); // Active LOW (pressed)

unsigned long currentMillis = millis();

// ===== STOP BUTTON =====
if (stopBtn) {
stopDoor();
}

// ===== OPEN BUTTON =====
else if (openBtn && !lsOpen && !opening) {
if (closing) stopDoor();
else openDoor();
}

// ===== CLOSE BUTTON =====
else if (closeBtn && !lsClose && !closing) {
if (opening) stopDoor();
else closeDoor();
}

// ===== LIMIT SWITCHES =====
if (opening && lsOpen) stopDoor();   // Fully open
if (closing && lsClose) stopDoor();  // Fully closed

// ===== LAMP CONTROL =====
updateLamps(currentMillis, lsOpen, lsClose);
}

// ====== Door Control Functions ======
void openDoor() {
opening = true;
closing = false;
stopped = false;

digitalWrite(MOTOR_IN1, HIGH);
digitalWrite(MOTOR_IN2, LOW);
analogWrite(EN_PIN, openSpeed);  // Control opening speed
Serial.println("Door Opening...");
}

void closeDoor() {
closing = true;
opening = false;
stopped = false;

digitalWrite(MOTOR_IN1, LOW);
digitalWrite(MOTOR_IN2, HIGH);
analogWrite(EN_PIN, closeSpeed); // Control closing speed
Serial.println("Door Closing...");
}

void stopDoor() {
opening = false;
closing = false;
stopped = true;

// Optional: short brake (both HIGH)
digitalWrite(MOTOR_IN1, LOW);
digitalWrite(MOTOR_IN2, LOW);
analogWrite(EN_PIN, 0);
Serial.println("Door Stopped");
}

// ====== Lamp Behavior ======
void updateLamps(unsigned long currentMillis, bool lsOpen, bool lsClose) {
bool doorInMotion = opening || closing;
bool doorFullyOpen = lsOpen;
bool doorFullyClosed = lsClose;

// Flash timing
if (currentMillis - previousMillis >= flashInterval) {
previousMillis = currentMillis;
flashState = !flashState;
}

// --- OPEN Lamp ---
if (doorFullyOpen) {
digitalWrite(LAMP_OPEN, HIGH);  // Steady ON
} else if (opening) {
digitalWrite(LAMP_OPEN, flashState); // Flash while opening
} else {
digitalWrite(LAMP_OPEN, LOW);
}

// --- SHUT Lamp ---
if (doorFullyClosed) {
digitalWrite(LAMP_SHUT, HIGH); // Steady ON
} else if (closing) {
digitalWrite(LAMP_SHUT, flashState); // Flash while closing
} else {
digitalWrite(LAMP_SHUT, LOW);
}

// --- AJAR Lamp ---
if (!doorFullyOpen && !doorFullyClosed) {
if (doorInMotion) {
digitalWrite(LAMP_AJAR, HIGH); // Steady when moving
} else {
if (currentMillis % slowFlashInterval < (slowFlashInterval / 2))
digitalWrite(LAMP_AJAR, HIGH);
else
digitalWrite(LAMP_AJAR, LOW);
}
} else {
digitalWrite(LAMP_AJAR, LOW);
}
}
