// Blue LED Pin (optional)
#define BLUE_LED 2

// 7-Segment Display Pins
#define SEG_A 4
#define SEG_B 5
#define SEG_C 18
#define SEG_D 19
#define SEG_E 21
#define SEG_F 22
#define SEG_G 23
#define SEG_DP 15 // Decimal Point (optional)

// Digit Select Pins
#define DIGIT1 25
#define DIGIT2 26
#define DIGIT3 27
#define DIGIT4 32

// Variables
int32_t value = 0;  // Stores input value from Serial Monitor

// Digit-to-Segment Mapping (0–9)
const uint8_t digitToSegment[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 0, 0, 1, 1}  // 9
};

// Setup Function
void setup() {
  Serial.begin(115200);

  // Configure LED pin
  pinMode(BLUE_LED, OUTPUT);

  // Configure 7-segment pins
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  pinMode(SEG_DP, OUTPUT);

  pinMode(DIGIT1, OUTPUT);
  pinMode(DIGIT2, OUTPUT);
  pinMode(DIGIT3, OUTPUT);
  pinMode(DIGIT4, OUTPUT);

  // Turn off all segments and digits initially
  clearDisplay();
}

// Loop Function
void loop() {
  // Read input from Serial Monitor
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // Read input until newline
    value = input.toInt();                       // Convert to int32_t

    Serial.print("You entered: ");
    Serial.println(value);                       // Print the value
  }

  int32_t last = value % 10;//take last digit only
  digitalWrite(DIGIT1,LOW);
  
  setSegments(last);
  delay(10);  // Short delay for smooth multiplexing
}

// Function to display number on 7-segment display
void displayNumber(int number) {
  int digits[4] = {0, 0, 0, 0}; // Stores each digit

  // Split number into digits
  digits[3] = number % 10;          // Last digit
  digits[2] = (number / 10) % 10;  // 2nd last digit
  digits[1] = (number / 100) % 10; // 3rd last digit
  digits[0] = (number / 1000) % 10;// 4th last digit

  // Display digits one by one (multiplexing)
  for (int i = 0; i < 4; i++) {
    activateDigit(i + 1);       // Activate digit
    setSegments(digits[i]);     // Set segments for the digit
    delay(5);                   // Multiplexing delay
    deactivateDigits();         // Deactivate all digits
  }
}

// Function to set the segments for a digit
void setSegments(int digit) {
  digitalWrite(SEG_A, digitToSegment[digit][0]);
  digitalWrite(SEG_B, digitToSegment[digit][1]);
  digitalWrite(SEG_C, digitToSegment[digit][2]);
  digitalWrite(SEG_D, digitToSegment[digit][3]);
  digitalWrite(SEG_E, digitToSegment[digit][4]);
  digitalWrite(SEG_F, digitToSegment[digit][5]);
  digitalWrite(SEG_G, digitToSegment[digit][6]);
}

// Function to activate a specific digit
void activateDigit(int digit) {
  digitalWrite(DIGIT1, !(digit == 1));
  digitalWrite(DIGIT2, !(digit == 2));
  digitalWrite(DIGIT3, !(digit == 3));
  digitalWrite(DIGIT4, !(digit == 4));
}

// Function to deactivate all digits
void deactivateDigits() {
  digitalWrite(DIGIT1, HIGH);
  digitalWrite(DIGIT2, HIGH);
  digitalWrite(DIGIT3, HIGH);
  digitalWrite(DIGIT4, HIGH);
}

// Function to clear display
void clearDisplay() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  digitalWrite(SEG_DP, LOW);

  deactivateDigits(); // Turn off all digits
}
