const int pinQ1 = 8;   // Top-Left
const int pinQ2 = 9;   // Bottom-Left
const int pinQ3 = 10;  // Top-Right
const int pinQ4 = 11;  // Bottom-Right

// Bipolar SPWM lookup table for a 50Hz cycle
// Values represent the positive pulse width (in microseconds) per carrier cycle
const int spwmTable[] = {
  100, 115, 130, 145, 160, 172, 184, 192, 197, 199,
  200, 199, 197, 192, 184, 172, 160, 145, 130, 115,
  100,  85,  70,  55,  40,  28,  16,   8,   3,   1,
    0,   1,   3,   8,  16,  28,  40,  55,  70,  85
};
const int tableSize = sizeof(spwmTable) / sizeof(spwmTable[0]);
const int carrierPeriod = 200; // Total carrier window (microseconds)

void setup() {
  pinMode(pinQ1, OUTPUT);
  pinMode(pinQ2, OUTPUT);
  pinMode(pinQ3, OUTPUT);
  pinMode(pinQ4, OUTPUT);
}

void loop() {
  for (int i = 0; i < tableSize; i++) {
    int posWidth = spwmTable[i];                 // Time for Q1 & Q4 ON
    int negWidth = carrierPeriod - posWidth;     // Time for Q2 & Q3 ON

    // 1. Positive State (Q1 & Q4 ON)
    if (posWidth > 0) {
      digitalWrite(pinQ1, HIGH);
      digitalWrite(pinQ4, HIGH);
      digitalWrite(pinQ2, LOW);
      digitalWrite(pinQ3, LOW);
      delayMicroseconds(posWidth);
    }

    // 2. Negative State (Q2 & Q3 ON)
    if (negWidth > 0) {
      digitalWrite(pinQ1, LOW);
      digitalWrite(pinQ4, LOW);
      digitalWrite(pinQ2, HIGH);
      digitalWrite(pinQ3, HIGH);
      delayMicroseconds(negWidth);
    }
  }
}
