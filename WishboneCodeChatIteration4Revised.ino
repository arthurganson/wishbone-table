// Pin Definitions
#define TriggerPin 6
#define Reed2Pin 2
#define Opt2Pin 3
#define Reed1Pin 4
#define Opt1Pin 5
#define Transformer2Pin 7
#define Motor2Pin 8
#define Solenoid2Pin 9
#define Motor1Pin 10
#define Solenoid1Pin 11
#define TrackPin 12

// Constants
const unsigned long FinalStrollTime = 5000;  // 20 seconds

// Variables
bool isTrackOn = false;

void setup() {
  // Configure pins
  pinMode(TriggerPin, INPUT_PULLUP);
  pinMode(Reed2Pin, INPUT_PULLUP);
  pinMode(Opt2Pin, INPUT_PULLUP);
  pinMode(Reed1Pin, INPUT_PULLUP);
  pinMode(Opt1Pin, INPUT_PULLUP);
  pinMode(Transformer2Pin, OUTPUT);
  pinMode(Motor2Pin, OUTPUT);
  pinMode(Solenoid2Pin, OUTPUT);
  pinMode(Motor1Pin, OUTPUT);
  pinMode(Solenoid1Pin, OUTPUT);
  pinMode(TrackPin, OUTPUT);

  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("starting *************************************");
}



void loop() {


  // Waiting Phase
  waitForTrigger();
  
  Serial.println("triggered");
  // Walking Cycle
  for (int i = 0; i < 3; i++) {
    Serial.println("loop");
    Serial.println(i, DEC);
    if (! isOpt1Closed() || ! isOpt2Closed()) {
      Serial.println("detetced out of alignment");
      digitalWrite(TrackPin, LOW);
      break;
    }
    
    // Turn ON the Track
    digitalWrite(TrackPin, HIGH);
    Serial.println("Track is ON 1 waiting for a reed switch");

    // Wait for either Reed1 or Reed2 to close
    while (digitalRead(Reed1Pin) == HIGH && digitalRead(Reed2Pin) == HIGH) {
      // Wait until either Reed1 or Reed2 closes
      // TODO check optical sensor, say "return" instead of "break"
    }
    Serial.print("reed switch detected ");
    Serial.print(digitalRead(Reed1Pin), DEC);
    Serial.println(digitalRead(Reed2Pin), DEC);

    delay(100);  // wait until we're sure the reed switch is not bouncing

    if (digitalRead(Reed1Pin) == LOW) {
      // Reed1 is closed
      walkingCycleReed1();
    } else if (digitalRead(Reed2Pin) == LOW) {
      // Reed2 is closed
      walkingCycleReed2();
    }

  
  }

 
    // Final Stroll
    finalStroll();
}

void waitForTrigger() { // (THIS SUBROUTINE SEEMS TO BE WORKING)
  int count = 0;
  while (digitalRead(TriggerPin) != LOW) { // Returns TRUE when the state of the trigger pin is HIGH, which is when the switch is open.
  // as long as it is TRUE, the "while" loop continues indefinitely.
    count += 1;
    Serial.println("waiting for the trigger");
    Serial.println(count, DEC);
    delay (1000);
    // Wait until Trigger closes
  }
}

bool isOpt1Closed() {
  return digitalRead(Opt1Pin) == HIGH;
}

bool isOpt2Closed() {
  return digitalRead(Opt2Pin) == HIGH;
}

void walkingCycleReed1() {
  // Turn OFF the Track
  digitalWrite(TrackPin, LOW);
  Serial.println("In the middle of walking cycle Reed 1 and Track has been turned OFF");

  // Turn ON Transformer2, Solenoid1, and Motor1
  digitalWrite(Transformer2Pin, HIGH);
  digitalWrite(Solenoid1Pin, HIGH);
  digitalWrite(Motor1Pin, HIGH);
  Serial.println("waiting for opt1");
  // Wait until Opt1 closes
  while (!isOpt1Closed()) {
    // Wait until Opt1 closes
  }
  Serial.println("detected opt1");
  // Turn OFF Transformer2, Solenoid1, and Motor1
  digitalWrite(Transformer2Pin, LOW);
  digitalWrite(Solenoid1Pin, LOW);
  digitalWrite(Motor1Pin, LOW);

  // Turn ON Track
  digitalWrite(TrackPin, HIGH);
  Serial.println("Track is ON 2");
}

void walkingCycleReed2() {
  // Turn OFF the Track
  digitalWrite(TrackPin, LOW);
  Serial.println("In the middle of walking cycle Reed 2 and Track has been turned OFF");

  // Turn ON Transformer2, Solenoid2, and Motor2
  digitalWrite(Transformer2Pin, HIGH);
  Serial.println("turning on solenoid");
  digitalWrite(Solenoid2Pin, HIGH);
  delay (1000);  // wait a little so solenoid has time to get disengaged
  Serial.println("turning on motor");
  digitalWrite(Motor2Pin, HIGH);

  delay (5000);  // wait a little so table has moved enough to clear the optical detector

  Serial.println("waiting for opt2");


  // Wait until Opt2 closes
  while (!isOpt2Closed()) {
    // Wait until Opt2 closes
  }
  // ok the table has finished turning all the way around
  
  Serial.println("detected opt2");
  // Turn OFF Transformer2, Solenoid2, and Motor2
  digitalWrite(Transformer2Pin, LOW);
  digitalWrite(Solenoid2Pin, LOW);
  digitalWrite(Motor2Pin, LOW);

  // Turn ON Track
  digitalWrite(TrackPin, HIGH);
  Serial.println("Track is ON 3");
}

void finalStroll() {
  // Turn ON the Track
  digitalWrite(TrackPin, HIGH);
  Serial.println("final stroll");

  // Wait for the specified FinalStrollTime
  delay(FinalStrollTime);

  // Turn OFF the Track
  digitalWrite(TrackPin, LOW);
  Serial.println("In the final stroll phase and just turned the Track is OFF");

}
