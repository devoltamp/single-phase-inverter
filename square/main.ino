void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}

void loop() {
  // State 1 (Positive half-cycle)
  digitalWrite(8, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  delay(10); // 10ms (50Hz half-period)

  // State 2 (Negative half-cycle)
  digitalWrite(8, LOW);
  digitalWrite(11, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  delay(10); // 10ms (50Hz half-period)
}
