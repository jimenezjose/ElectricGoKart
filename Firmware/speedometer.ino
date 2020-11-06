
#define MAX_SPEED 120

const int ledPin = 9;
const int knobPin = A0;
const int switchPin = 8;
const int buttonPin = 7;

int currentSpeed = 0;
int prevSpeed = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(knobPin, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(buttonPin, INPUT);
  
  digitalWrite(ledPin, HIGH);
}

void loop() {
  displaySpeed();
  displayTransmission();
}

void displayTransmission() {
  int switchState = digitalRead(switchPin);
  int buttonState = digitalRead(buttonPin);

  if( buttonState == LOW && switchState == LOW ) {
    Serial.println("R");
  }
  else if( buttonState == LOW && switchState == HIGH ) {
    Serial.println("D");
  }
  else if( buttonState ==  HIGH ) {
    Serial.println("P");
  }
}

void displaySpeed() {
  Serial.println(currentSpeed);
  currentSpeed = map(analogRead(knobPin), 5, 1010, 0, MAX_SPEED);
  if( abs(currentSpeed - prevSpeed) == 1 ) currentSpeed = prevSpeed; 
  prevSpeed = currentSpeed;
}
