#define DRIVE 1
#define REVERSE 0

const int pedal = A0;
const int motor = 9;
const int brake = 11;
const int gearShift = 12;
const int forward = 7;
const int reverse = 8;

void setup() {
  Serial.begin( 9600 );
  pinMode( pedal, INPUT );
  pinMode( gearShift, INPUT );
  pinMode( motor, OUTPUT );
  pinMode( brake, OUTPUT );
  pinMode( forward, OUTPUT);
  pinMode( reverse, OUTPUT );

}

void loop() {
  int speed = map( analogRead(pedal), 173, 876, 0, 35 );
  analogWrite( motor, speed );

  Serial.print( "PEDAL:\t" );
  Serial.print( analogRead(pedal) );
  Serial.print( "\tPWM:\t" );
  Serial.println( speed);

  if( digitalRead(gearShift) == DRIVE && digitalRead(forward) == LOW ) {
    /* Set motor gear to forward drive - once per gear shift */
    Serial.println( "forward drive" );
    digitalWrite( forward, HIGH );
    digitalWrite( reverse, LOW );
  }
  else if( digitalRead(gearShift) == REVERSE && digitalRead(reverse) == LOW ) {
    /* Set motor gear in reverse - once per gear shift */
    Serial.println( "reverse" );
    digitalWrite( reverse, HIGH );
    digitalWrite( forward, LOW );
  }

  

}
