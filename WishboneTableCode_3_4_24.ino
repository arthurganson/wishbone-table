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
const unsigned long FinalStrollTime = 10000;  // 10 seconds
int NumberOfCyclesBeforeRest = 6; // Add 1 to the actual number of cycles desired


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
  for (int i = 1; i < NumberOfCyclesBeforeRest; i++) {
    Serial.print("loop #");
    Serial.println(i, DEC);

      if (! isOpt1Closed() || ! isOpt2Closed()) { // checking for misalignment of track
      Serial.println("detected out of alignment BEFORE track is turned on");
      digitalWrite(TrackPin, LOW);
      return;
    }
    
    // Turn ON the Track
    digitalWrite(TrackPin, HIGH);
    Serial.println("Track is ON and waiting for either reed switch to be closed");

    // Mr. Wishbone is walking in either direction now....

    // Wait for either Reed1 or Reed2 to close
    while (digitalRead(Reed1Pin) == HIGH && digitalRead(Reed2Pin) == HIGH) { // both are HIGH while walking
      // Wait until either Reed1 or Reed2 closes

      // Checking optical sensors while walking to detect track mis-alignment
      if (! isOpt1Closed() || ! isOpt2Closed()) { 
      Serial.println("detected out of alignment WHILE track is on");
      digitalWrite(TrackPin, LOW);
      return;
      }
    }
    Serial.print("reed switch detected ");
    Serial.print(digitalRead(Reed1Pin), DEC);
    Serial.println(digitalRead(Reed2Pin), DEC);

    delay(100);  // wait until we're sure the reed switch is not bouncing

    if (digitalRead(Reed1Pin) == LOW) {
      // Reed1 is closed
      Serial.println("Left side has been triggered");
      walkingCycleReed1();
    } else if (digitalRead(Reed2Pin) == LOW) {
      // Reed2 is closed
      Serial.println("Right side has been triggered");
      walkingCycleReed2();
    }

  
  }

 
    // Final Stroll
    finalStroll();
}

void waitForTrigger() { // Waiting for the trigger switch
  int count = 0;
  while (digitalRead(TriggerPin) != LOW) { // Returns TRUE when the state of the trigger pin is HIGH, which is when the switch is open.
  // as long as it is TRUE, the "while" loop continues indefinitely.
    count += 1;
    Serial.print("Waiting for the trigger for ");
    Serial.print(count, DEC);
    Serial.println( " seconds");
    delay (2000);
    // Wait until Trigger closes

    //good idea to check for track alignment

  }
}

bool isOpt1Closed() {
  return digitalRead(Opt1Pin) == HIGH;
}

bool isOpt2Closed() {
  return digitalRead(Opt2Pin) == HIGH;
}

void walkingCycleReed1() {
  // Turn OFF the Track but wait a tiny bit more so the wishbone goes a little farther
  delay(400);
  digitalWrite(TrackPin, LOW);
  Serial.println("Track has been turned OFF");

  // Turn ON Transformer2, Solenoid1, and Motor1
  digitalWrite(Transformer2Pin, HIGH);
  digitalWrite(Solenoid1Pin, HIGH);
  Serial.println("Solenoid 1 is on");
  delay (200);  // wait a little so solenoid has time to get disengaged
  digitalWrite(Motor1Pin, HIGH);
  Serial.println("Motor 1 is on");

  delay (5000);  // wait a little so table has moved enough to clear the optical detector

  Serial.println("waiting for opt1, the left optical sensor, to be triggered........");
  // Wait until Opt1 closes
  while (!isOpt1Closed()) {  // Wait until Opt1 closes
  }
  // ok the table has finished turning all the way around

  Serial.println("detected opt1");
  // Turn OFF Transformer2, Solenoid1, and Motor1

  delay(400); // wait a tiny bit so that the obstruction vane is more centered on the opto sensor

  digitalWrite(Transformer2Pin, LOW);
  digitalWrite(Solenoid1Pin, LOW);
  digitalWrite(Motor1Pin, LOW);

  // Turn ON Track
  digitalWrite(TrackPin, HIGH);
  Serial.println("Track is ON now that side 1, the left side, has completed turning");
}

void walkingCycleReed2() {
  // Turn OFF the Track but wait a tiny bit more so the wishbone goes a little farther
  delay(400);
  digitalWrite(TrackPin, LOW);
  Serial.println("Track has been turned OFF");

  // Turn ON Transformer2, Solenoid2, and Motor2
  digitalWrite(Transformer2Pin, HIGH);
  digitalWrite(Solenoid2Pin, HIGH);
  Serial.println("Solenoid 2 is on");
  delay (500);  // wait a little so solenoid has time to get disengaged
  digitalWrite(Motor2Pin, HIGH);
  Serial.println("Motor 2 is on");

  delay (5000);  // wait a little so table has moved enough to clear the optical detector

  Serial.println("waiting for Opt2, the right side optical sensor, to be triggered........");


  // Wait until Opt2 closes
  while (!isOpt2Closed()) {  // Wait until Opt2 closes
  }
  // ok the table has finished turning all the way around
  
  Serial.println("detected opt2");
  delay(400); // wait a tiny bit so that the obstruction vane is more centered on the opto sensor
  
  // Turn OFF Transformer2, Solenoid2, and Motor2
  digitalWrite(Transformer2Pin, LOW);
  digitalWrite(Solenoid2Pin, LOW);
  digitalWrite(Motor2Pin, LOW);

  // Turn ON Track
  digitalWrite(TrackPin, HIGH);
  Serial.println("Track is ON after side 2, the right side, has completed turning");
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
