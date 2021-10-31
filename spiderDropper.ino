/* SPIDER DROPPER 2.0
 * 
 * This version of the spider dropper utilizes an HC-sr204 ultrasonic distance sensor
 * 
 * When an object obstructs the path of the sensor we drive the motor to drop the spider
 * 
 * The "home" or default state of the spider is retracted and the rubber band provides a passive force to keep it in place
 * When the motor drives the spider it is counter acting the spring force of the rubber band
 * You want the rubber band to be just strong enough to get the spider retracted. Much more and it's hard for the motor to develop the torque needed without a gearbox
 * You need a 5V motor with enough torque to do this
 * 
 */



#define trigPin 13    // trig and echo pin for HC-sr04 sensor
#define echoPin 12
#define relayPin 11 // 5v relay

#define MIN_DIST  35  //35cm, ~ 1 ft
#define MAX_DIST  190 //150cm, ~5 ft

#define NOT_DOOR_HANDLE distance!=94 && distance!=95 && distance!=96
int highCount = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin,LOW); // verify motor is not on
  
  Serial.begin (9600);

}

void loop() {
  long duration, distance; // some variables for figuring out distance from HC-sr04 sensor

  /* Send Ultrasonic Pulse and Listen for Echo */
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH); // listen for echo

  /* Calculate Distance to Object */
  distance = (duration/2) / 29.412; // calculate distance (29.... is speed of sound conversion factor)
  
    Serial.print(distance); // print some data so we can monitor WTF is going on
    Serial.println(" cm ");
    Serial.println(highCount);    
  
  /* Check for Obstruction at the Correct Distance */
  // if sensor detects object in target range, victim is present
  // (too far and it's the environment, noise or nothing. Too close and it's just the spider swinging in the breeze and obstructing the sensor...or real bugs)
  if(distance < MAX_DIST && distance > MIN_DIST && NOT_DOOR_HANDLE) 
  {
    highCount++;

    if(highCount > 2)
    {
      highCount = 0;
      delay(400); // delay a bit after detecting motion before dropping on the victim
      digitalWrite(relayPin,HIGH); // RELEASE THE KRACKEN
      delay(5000); // hold in place for 5 seconds
  
      digitalWrite(relayPin, LOW); // make sure motor is off
      delay(25000); // pause for 25 seconds to make sure spider is fully retracted and victims have gotten their candy unperturbed     
    } 
  }else{
    highCount = 0;  
  }
  
  
  delay(50); // make sure we don't overload the processor on loops where we only check distance (distance isn't short enough)
}
