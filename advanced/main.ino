
#include <Arduino.h>
#include <avr/interrupt.h>
#include <math.h>

// Arduino Uno / ATmega328P, 16 MHz
// PWM outputs: D9 (OC1A), D10 (OC1B)
// PWM frequency: 20 kHz
// Reference sine frequency: 50 Hz
// 200 sine samples, updated at 10 kHz

#define PWM_TOP       799
#define TABLE_SIZE    200
#define MODULATION    0.90

uint16_t sineA[TABLE_SIZE];
uint16_t sineB[TABLE_SIZE];

volatile uint16_t tableIndex = 0;
volatile uint8_t updateDivider = 0;

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  // Generate the sine reference values before enabling interrupts.
  // Keep the duty cycle away from 0% and 100%.
  for (uint16_t i = 0; i < TABLE_SIZE; i++) {
    float angle = 2.0f * PI * i / TABLE_SIZE;
    float s = sin(angle);

    sineA[i] = (uint16_t)(
      PWM_TOP * (0.5f + 0.5f * MODULATION * s)
    );

    sineB[i] = (uint16_t)(
      PWM_TOP * (0.5f - 0.5f * MODULATION * s)
    );
  }

  cli();

  // Stop and configure Timer1.
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // Fast PWM mode 14: TOP = ICR1.
  ICR1 = PWM_TOP;

  // Initial duty values.
  OCR1A = PWM_TOP / 2;
  OCR1B = PWM_TOP / 2;

  // Non-inverting PWM on OC1A (D9) and OC1B (D10).
  TCCR1A =
      (1 << COM1A1) |
      (1 << COM1B1) |
      (1 << WGM11);

  // Mode 14, clock prescaler = 1.
  TCCR1B =
      (1 << WGM13) |
      (1 << WGM12) |
      (1 << CS10);

  // Interrupt at each Timer1 overflow / PWM cycle.
  TIMSK1 |= (1 << TOIE1);

  sei();
}

ISR(TIMER1_OVF_vect) {
  // Update the sine reference every 2 PWM cycles.
  // 20,000 / 2 = 10,000 updates per second.
  // 10,000 / 200 samples = 50 Hz.
  updateDivider++;

  if (updateDivider >= 2) {
    updateDivider = 0;

    OCR1A = sineA[tableIndex];
    OCR1B = sineB[tableIndex];

    tableIndex++;

    if (tableIndex >= TABLE_SIZE) {
      tableIndex = 0;
    }
  }
}

void loop() {
  // Timer1 generates PWM independently.
  // Keep other code non-blocking.
}
