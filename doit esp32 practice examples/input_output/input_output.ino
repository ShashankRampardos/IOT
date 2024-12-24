int32_t value;

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    // Read input until newline ('\n')
    String input = Serial.readStringUntil('\n');  // Takes whole input line
    value = input.toInt();                      // Convert to float

    Serial.print("You entered: ");
    Serial.println(value);                         // Print value
  }
}
