// Ultrasonic Sensor Pins
#define TRIG_PIN 13
#define ECHO_PIN 14

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

#define DIGIT1 25
#define DIGIT2 26
#define DIGIT3 27
#define DIGIT4 32

// Array for 7-Segment segment states (common cathode logic)
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
    {1, 1, 1, 1, 0, 1, 1}  // 9
};

// Setup Function
void setup() {
  // Initialize serial monitor
  Serial.begin(115200);

  // Configure ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

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

  // Turn off all segments and digits
  clearDisplay();
}

int lastDisplayedValue = -1; // Store last displayed value
// Loop Function
void loop() {
  float distance = measureDistance();
  if(abs(lastDisplayedValue - distance) > 2){//only two centimeter ka fluctuation ko neglect karo, to avoid flickring on 7 segment display even when we hold the device in our hands (our hands shake in air when we hold device and this can cause distance fluctuation, (anti shaking mode))
    lastDisplayedValue = distance;
  }else{
    distance = lastDisplayedValue;
  }

  int displayValue = (int)distance;

  displayNumber(displayValue); // Update display
  delayMicroseconds(2000);
  //digitalWrite(BLUE_LED,  distance <= 40 ? HIGH : LOW);
  
}

//calculate distance 5 times and returns its average (to avoid small fluctuation)
float measureDistance() {
  const int samples = 1; // Take 10 samples
  float total = 0;

  for (int i = 0; i < samples; i++) {
    // Trigger ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Measure echo duration
    long duration = pulseIn(ECHO_PIN, HIGH);
    total += duration * 0.0343 / 2; // Convert to cm
  }

  return total / samples; // Return average distance
}


// Function to clear the display
void clearDisplay() {
  // Turn off all segments
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  digitalWrite(SEG_DP, LOW);

  // Turn off all digits
  digitalWrite(DIGIT1, HIGH);
  digitalWrite(DIGIT2, HIGH);
  digitalWrite(DIGIT3, HIGH);
  digitalWrite(DIGIT4, HIGH);
}

// Function to display a number on the 7-segment display
void displayNumber(int number) {
  int digits[4] = {0, 0, 0, 0};

  // Extract individual digits
  digits[3] = number % 10;
  digits[2] = (number / 10) % 10;
  digits[1] = (number / 100) % 10;
  digits[0] = (number / 1000) % 10;

  auto swap = [](int& x,int& y){
      x = x^y;
      y = x^y;
      x = x^y;
  };
  swap(digits[0],digits[3]);
  swap(digits[1],digits[2]);

  //activateDigits();
  // Display digits one at a time
  for (int i = 0; i < 4; i++) {
    activateDigit(i + 1);
    setSegments(digits[i]);
    delayMicroseconds(2000); // Small delay for multiplexing
    deactivateDigits();
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
