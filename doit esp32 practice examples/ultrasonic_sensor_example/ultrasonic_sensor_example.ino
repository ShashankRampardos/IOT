#define TRIG_PIN 13  // GPIO 13 connected to TRIG
#define ECHO_PIN 12  // GPIO 12 connected to ECHO
#define BlueLed 2
void setup() {
  Serial.begin(115200);  // Initialize Serial Monitor
  pinMode(TRIG_PIN, OUTPUT);//output, emit ultrasonic sound at in a cone shape (around 15 degree angle)
  pinMode(ECHO_PIN, INPUT);//input, reseive ultrasonic that is bounced back by the object 
  pinMode(BlueLed,OUTPUT);//on board blue led 

  Serial.println("Ultrasonic Sensor Example");
}

void loop() {
  // Send a pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);  // Ensure clean pulse, this is different from delay(), delay() take milli seconds as input and this one take microseconds as input, this sensor work precicely with micro seconds milli second is 1000 times bigger than 1 micro second
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);  // Pulse duration = 10 µs
  digitalWrite(TRIG_PIN, LOW);

  // Measure the pulse duration
  long duration = pulseIn(ECHO_PIN, HIGH);//this will tell, kitna time laga sound ko wapas ana may, it will return duration in micro seconds

  // Calculate distance in cm
  float distance = duration * (0.0343 / 2);//distance = time * speed, wahi simple formula. speed is divided by 2 because sound traveled in two way, one from the sensor and another back to the sensor.

  // Print distance to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if(distance <= 45){//blink if less than 45 cm
    digitalWrite(BlueLed,HIGH);
    delay(400);
    digitalWrite(BlueLed,LOW);
    delay(400);
  }else{
    digitalWrite(BlueLed,LOW);
  }

  delay(500);  // Update every 500 ms
}
