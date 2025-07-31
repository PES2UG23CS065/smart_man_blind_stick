// Define pins
const int buzzerPin = 5;        // Buzzer connected to pin 5
const int irSensorPin = 7;      // IR sensor pin
const int trigPin = 6;          // Ultrasonic trigger
const int echoPin = 8;          // Ultrasonic echo

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(irSensorPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  bool alert = false;

  // 1. Check for data from Python
  if (Serial.available() > 0) {
    char receivedData = Serial.read();
    if (receivedData == 'P') {
      Serial.println("Python Signal: Person detected");
      alert = true;
    }
  }

  // 2. Check IR sensor
  int irValue = digitalRead(irSensorPin);
  if (irValue == LOW) {  // Most IR modules output LOW when object detected
    Serial.println("IR Sensor: Obstacle detected");
    alert = true;
  }

  // 3. Ultrasonic sensor reading
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 30000);  // Timeout after 30 ms
  distance = duration * 0.034 / 2;           // cm

  if (distance > 0 && distance < 10) {  // valid range and near object
    Serial.print("Ultrasonic Sensor: Object at ");
    Serial.print(distance);
    Serial.println(" cm");
    alert = true;
  }

  // Final action
  if (alert) {
    digitalWrite(buzzerPin, HIGH);  
  } else {
    digitalWrite(buzzerPin, LOW);  
  }

  delay(100);
}
