void setup() {
  // Maps to digital pin 13:
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);   // Turn the LED on (HIGH is the voltage level).
  delay(1000);              // Wait for a second.
  digitalWrite(13, LOW);    // Turn the LED off.
  delay(1000);              // Wait for a second.
}
