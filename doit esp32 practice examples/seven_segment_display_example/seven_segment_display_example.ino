// Pin assignments for ESP32
#define SEG_A 4
#define SEG_B 5
#define SEG_C 18

#define SEG_D 19
#define SEG_E 21
#define SEG_F 22
#define SEG_G 23
#define SEG_DP 15

#define DIGIT1 25
#define DIGIT2 26
#define DIGIT3 27
#define DIGIT4 32

// Segment patterns for numbers 0-9 (common cathode)
const uint8_t digits[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

void setup() {
  // Initialize segment pins
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  pinMode(SEG_DP, OUTPUT);

  // Initialize digit control pins
  pinMode(DIGIT1, OUTPUT);
  pinMode(DIGIT2, OUTPUT);
  pinMode(DIGIT3, OUTPUT);
  pinMode(DIGIT4, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  displayDigit(1, DIGIT1);
  displayDigit(2, DIGIT2);
  displayDigit(3, DIGIT3);
  displayDigit(4, DIGIT4);
}

// Function to display a digit on a specific 7-segment
void displayDigit(uint8_t number, uint8_t digitPin) {
  digitalWrite(DIGIT1, HIGH); // Turn all digits off
  digitalWrite(DIGIT2, HIGH);
  digitalWrite(DIGIT3, HIGH);
  digitalWrite(DIGIT4, HIGH);

  setSegments(digits[number]);  // Set the correct segments
  digitalWrite(digitPin, LOW); // Turn on the specific digit
  delay(2);                    // Small delay for persistence
}

// Function to light up the segments based on the pattern
void setSegments(uint8_t pattern) {
  digitalWrite(SEG_A, pattern & 0x01);
  digitalWrite(SEG_B, (pattern >> 1) & 0x01);
  digitalWrite(SEG_C, (pattern >> 2) & 0x01);
  digitalWrite(SEG_D, (pattern >> 3) & 0x01);
  digitalWrite(SEG_E, (pattern >> 4) & 0x01);
  digitalWrite(SEG_F, (pattern >> 5) & 0x01);
  digitalWrite(SEG_G, (pattern >> 6) & 0x01);
  digitalWrite(SEG_DP, (pattern >> 7) & 0x01);
}
