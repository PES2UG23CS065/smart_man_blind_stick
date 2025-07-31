// Define pins
const int buzzerPin = 5;  // Buzzer connected to pin 5
const int irSensorPin = 7;  // IR sensor pin (replace with actual pin)
const int trigPin = 6;  // Ultrasonic sensor trigger pin
const int echoPin = 8;  // Ultrasonic sensor echo pin

void setup() {
  pinMode(buzzerPin, OUTPUT);  // Set buzzer pin as output
  pinMode(irSensorPin, INPUT);  // Set IR sensor pin as input
  pinMode(trigPin, OUTPUT);  // Set ultrasonic sensor trigger pin as output
  pinMode(echoPin, INPUT);  // Set ultrasonic sensor echo pin as input
  Serial.begin(9600);  // Initialize serial communication for debugging
}

void loop() {
  // Check if data is available from the Python program
  if (Serial.available() > 0) {
    char receivedData = Serial.read();  // Read the incoming byte

    if (receivedData == 'P') {  // If 'P' is received from Python program
      digitalWrite(buzzerPin, HIGH);  // Turn buzzer on
      Serial.println("⚠ Buzzer ON (Person detected)");  // Debugging message
      delay(1000);  // Keep buzzer on for 1 second
      digitalWrite(buzzerPin, LOW);  // Turn buzzer off
    }
  }

  // IR sensor check (optional)
  int irState = digitalRead(irSensorPin);

  // Ultrasonic sensor distance check (optional)
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;  // Calculate distance in cm

  // Optional buzzer trigger based on IR sensor or distance
  if (irState == HIGH || distance < 10) {  // If IR sensor is triggered or object within 10 cm
    digitalWrite(buzzerPin, LOW);  // Turn buzzer on
    Serial.println("⚠ Buzzer ON (IR or Ultrasonic Trigger)");  // Debugging message
  } else {
    digitalWrite(buzzerPin, HIGH);  // Turn buzzer off
    Serial.println(" Buzzer OFF (IR and Ultrasonic conditions not met)");  // Debugging message
  }

  delay(100);  // Small delay for stability
}

