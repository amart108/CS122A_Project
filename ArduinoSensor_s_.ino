#define trig_Front 3 // For Front Sensor
#define echo_Front 2 // For Front Sensor
#define trig_Right 5 // For Right Sensor
#define echo_Right 4 // For Right Sensor
#define trig_Left 7 // For Left Sensor
#define echo_Left 6 // For Left Sensor
#define leftFlag 8
#define frontFlag 9
#define rightFlag 10
#define leftClear 11
#define rightClear 12

float signalDur_Front, distance_Front,signalDur_Right, distance_Right, signalDur_Left, distance_Left;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //First HC-SR04 sensor
  pinMode(trig_Left, OUTPUT);
  pinMode(echo_Left, INPUT);
  // Second sensor
  pinMode(trig_Front, OUTPUT);
  pinMode(echo_Front, INPUT);
  //Third Sensor
  pinMode(trig_Right, OUTPUT);
  pinMode(echo_Right, INPUT);
  //Sensor flags to send to the ATmega 1284
  pinMode(leftFlag, OUTPUT);
  pinMode(frontFlag, OUTPUT);
  pinMode(rightFlag, OUTPUT);
  pinMode(leftClear, OUTPUT);
  pinMode(rightClear, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Giving a pulse to A Sensor
  digitalWrite(trig_Left, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_Left, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_Left, LOW);

// Time it takes to gets a signal on the echo pin/speaker
   signalDur_Left = pulseIn(echo_Left,HIGH);
  //Detected objects distance from the sensor
  // Using 343 meters per second since it is the speed of sound.
   distance_Left = (signalDur_Left / 2) * 0.0343;
  
   delay(500);

  digitalWrite(trig_Front, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_Front, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_Front, LOW);

  
  // Time it takes to gets a signal on the echo pin/speaker
  signalDur_Front = pulseIn(echo_Front,HIGH);

  //Detected objects distance from the sensor
  // Using 343 meters per second since it is the speed of sound.
  distance_Front = (signalDur_Front / 2) * 0.0343;

   delay(500);

  digitalWrite(trig_Right, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_Right, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_Right, LOW);

  
  // Time it takes to gets a signal on the echo pin/speaker
  signalDur_Right = pulseIn(echo_Right,HIGH);

  //Detected objects distance from the sensor
  // Using 343 meters per second since it is the speed of sound.
  distance_Right = (signalDur_Right / 2) * 0.0343;

  
  // Sends the result to serial monitor
  Serial.print("Left = ");
  if(distance_Left >= 400 || distance_Left <= 2)
  {
    Serial.print("Out of Range! ");
    digitalWrite(leftFlag, LOW);
  }
  else if(distance_Left <= 30 && distance_Left > 20)
  {
    Serial.print(distance_Left);
    Serial.print(" cm");
    Serial.print(" Close! ");
    digitalWrite(leftFlag, HIGH);
    //delay(1000);
  }
  else if(distance_Left <= 20)
  {
    Serial.print(distance_Left);
    Serial.print(" cm");
    Serial.print(" Too Close! ");
    digitalWrite(leftFlag, HIGH);
    //delay(1000);
  }
  else
  {
    Serial.print(distance_Left);
    Serial.print(" cm ");
    digitalWrite(leftFlag, LOW);
    //delay(1000);
  }

  // watch out for the freuqnecy of the other may be affecting it.
  Serial.print("Front = ");
  if(distance_Front >= 400 || distance_Front <= 2)
  {
    Serial.print("Out of Range!");
    digitalWrite(frontFlag, LOW);
  }
  else if(distance_Front <= 30 && distance_Front > 20)
  {
    Serial.print(distance_Front);
    Serial.print(" cm");
    Serial.print(" Close! ");
    digitalWrite(frontFlag, HIGH);
    //delay(1000);
  }
  else if(distance_Front <= 20)
  {
    Serial.print(distance_Front);
    Serial.print(" cm");
    Serial.print(" Too Close! ");
    digitalWrite(frontFlag, HIGH);
    //delay(1000);
  }
  else
  {
    Serial.print(distance_Front);
    Serial.print(" cm ");
    digitalWrite(frontFlag, LOW);
    //delay(1000);
    //delay(500);
  }


  Serial.print("Right = ");
  if(distance_Right >= 400 || distance_Right <= 2)
  {
    Serial.print("Out of Range!");
    digitalWrite(rightFlag, LOW);
  }
  else if(distance_Right <= 30 && distance_Right > 20)
  {
    Serial.print(distance_Right);
    Serial.print(" cm");
    Serial.print(" Close! ");
    digitalWrite(rightFlag, HIGH);
    //delay(1000);
  }
  else if(distance_Right <= 20)
  {
    Serial.print(distance_Right);
    Serial.print(" cm");
    Serial.print(" Too Close! ");
   digitalWrite(rightFlag, HIGH);
   // at 12 cm should reverse left or right  
   //delay(1000);
  }
  else
  {
    Serial.print(distance_Right);
    Serial.print(" cm");
    digitalWrite(rightFlag, LOW);
    //delay(1000);
    //delay(500);
  }
   Serial.println(" ");
  Serial.println("Which has more space the left or right sensor: ");
  if(distance_Left > distance_Right)
  {
    Serial.println("Left!!");
    digitalWrite(leftClear, HIGH);
    digitalWrite(rightClear, LOW);
  }
  else if(distance_Left < distance_Right)
  {
    Serial.println("Right!!");
    digitalWrite(leftClear, LOW);
    digitalWrite(rightClear, HIGH);
  }
  else if(distance_Left == distance_Right)
  {
    Serial.println("Tie!!");
    digitalWrite(leftClear, HIGH);
    digitalWrite(rightClear, LOW);
  }

   Serial.println(" ");
   //delay(500);
 
 }
  
