#define BLYNK_TEMPLATE_ID  "TMPL1XB1XVcO"
#define BLYNK_DEVICE_NAME  "Quickstart Device"
#define BLYNK_AUTH_TOKEN   "V7IIujAO2xZsB0DXzwrEgdS1Amgrs1ST"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32_Servo.h>
//#include <analogWrite.h>

# define STOPPED 0
# define FOLLOWING_LINE 1
# define NO_LINE 2
# define KIRI_BANYAK 3
# define KIRI 4
# define KANAN 5
# define KANAN_BANYAK 6

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "izzati";
//char pass[] = "zatikacak";

char ssid[] = "Palof";
char pass[] = "kakikuke";

BlynkTimer timer;

const int dir = 2;
const int motor = 4;
const int dir1 = 5;
const int motor1 = 18;
const int lefty = 32; //left
const int mid = 35; //mid
const int righty = 34; //right
const int relayus = 14;
const int UVC2 = 13;
const int trig1 = 21;
const int echo1 = 19;
//const int trig2 = 13;
//const int echo2 = 12;
int k, distance1, distance2, distance3;
long duration1, duration2, duration3;

//PID value
float Kp=30;//50
float Ki=0;
float Kd=0;

float error=0, P=35, I=0, D=0, PIDvalue=0;//35
float previousError=0, previousI=0;

int speed = 35;
int speed1 = 32;
int right = 50;
int left = 10;
int stop = 0;
int LFSensor[3]={0, 0, 0};
int mode = 0;
const int iniMotorPower = 25;
const int adj = 1;
Servo myservo;
float pos = 0; 

BLYNK_WRITE(V0){
  int val2 = param.asInt();
  Blynk.virtualWrite(V1,val2);
  Serial.println("No PID");


  /*while(val2 == HIGH){

    calculatePID();

  }*/
  if (val2 == HIGH){
    digitalWrite(relayus, HIGH);
   
  }
  else if (val2 == LOW){
    digitalWrite(relayus, LOW);
  }

}

BLYNK_WRITE(V6){

  for (pos = 0; pos <= 180; pos += 0.5) 
  { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 0.5) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }


}


BLYNK_WRITE(V10){

  //Pusing kiri
  int rollout = param.asInt();

  if (rollout == 1){

    motorleft();
    Serial.println("motor leftu");
  }   
  else{

    stopping();

  }

}

BLYNK_WRITE(V11){
  //pusing kanan
  int rollout = param.asInt();

  if (rollout == 1){

    motorright();
    Serial.println("motor right");
  }
  else{

    stopping();

  }

}

BLYNK_WRITE(V4){
  
  //digitalWrite(relayus, HIGH);//default on state of relay for light
  int val1 = param.asInt(); 
  //Blynk.virtualWrite(V1, val1);
  
  Serial.println("Motor gerak");

  if (val1 == 1)
  {

    //Forward or backward
    motorstart();
    //delay(1000);
  
  }
  else if(val1 == 0)
  {
    Serial.println("Slowly stopping...");

    delay(50);
    
    int speed = 30;
    int speed1 = 27;

    motorstart();

    Serial.println("speed1 = ");
    Serial.print(speed1);

    delay(50);

    stopping();

    Serial.println("speed1 = ");
    Serial.print(stop);
    
    //Serial.println(k);    

  }
  
  else 
  {
    stopping();
    Serial.println("Speed=");
    Serial.println(stop);

  }

}

BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void motorstart(){

  digitalWrite(dir1, LOW);
  analogWrite(motor1, speed);
  digitalWrite(dir, LOW);
  analogWrite(motor, speed1);

}

void reverse(){

  digitalWrite(dir1, HIGH);
  analogWrite(motor1, 20);
  digitalWrite(dir, HIGH);
  analogWrite(motor, 17);

}

void motorleft(){

  digitalWrite(dir1, LOW);
  analogWrite(motor1, left);
  digitalWrite(dir, LOW);
  analogWrite(motor, right);

}

void motorright(){

  digitalWrite(dir1, LOW);
  analogWrite(motor1, right);
  digitalWrite(dir, LOW);
  analogWrite(motor, left);

}

void stopping(){

  digitalWrite(dir1, LOW);
  analogWrite(motor1, stop);
  digitalWrite(dir, LOW);
  analogWrite(motor, stop);

}

void calculatePID()
{
  P = error;
  I = I + error;
  D = error-previousError;
  PIDvalue = (Kp*P) + (Ki*I) + (Kd*D);
  previousError = error;
}

void motorPIDcontrol()
{
  calculatePID();
  
  int right = 30 - iniMotorPower - PIDvalue;
  int left = 27 + iniMotorPower*adj - PIDvalue;
  
  // The motor speed should not exceed the max PWM value
   constrain(right, 20, 100);
   constrain(left, 20, 100);

   int speed = right;
   int speed1 = left;

   motorstart();
  
  //Serial.print (PIDvalue);
  //Serial.print (" ==> Left, Right:  ");
  //Serial.print (leftMotorSpeed);
  //Serial.print ("   ");
  //Serial.println (rightMotorSpeed);
}

void ultrasonic(){

  digitalWrite(trig1, LOW);
  //digitalWrite(trig2, LOW);
  //digitalWrite(trig3, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig1, HIGH);
  //digitalWrite(trig2, HIGH);
  //digitalWrite(trig3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  //digitalWrite(trig2, LOW);
  //digitalWrite(trig3, HIGH);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration1 = pulseIn(echo1, HIGH);
  //duration2 = pulseIn(echo2, HIGH);
  //duration3 = pulseIn(echo3, HIGH);

  // Calculating the distance
  distance1 = duration1 * 0.034 / 2; //front
  //distance2 = duration2 * 0.034 / 2; //back
  //distance3 = duration3 * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Front: ");
  Serial.println(distance1);
 // Serial.print("Back: ");
  //Serial.println(distance2);
  //Serial.print("Right: ");
  //Serial.println(distance3);

  Blynk.virtualWrite(V5, distance1);
  Blynk.virtualWrite(V6, distance2);
  //Blynk.virtualWrite(V8, distance3);

}

void followline()
{
  LFSensor[0] = digitalRead(lefty);
  LFSensor[1] = digitalRead(mid);
  LFSensor[2] = digitalRead(righty);

  
  //Jadi
  if((     LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 0 ))  {mode = KANAN_BANYAK; error = 2;}
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 ))  {mode = KANAN; error = 1;}
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 ))  {mode = FOLLOWING_LINE; error = 0;}
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 ))  {mode = KIRI; error = -1;}
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 ))  {mode = KIRI_BANYAK; error = -2;}
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 ))  {mode = STOPPED; error = 0;}
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 ))  {mode = NO_LINE; error = 0;}

  /*if((     LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 0 ))  {mode = FOLLOWING_LINE; error = 2;}
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 ))  {mode = FOLLOWING_LINE; error = 1;}
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 ))  {mode = FOLLOWING_LINE; error = 0;}
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 ))  {mode = FOLLOWING_LINE; error = -1;}
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 ))  {mode = FOLLOWING_LINE; error = -2;}
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 ))  {mode = STOPPED; error = 0;}
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 ))  {mode = NO_LINE; error = 0;}*/

  /*if((     LFSensor[0]== 1 )&&(LFSensor[2]== 0 ))  {mode = KANAN_BANYAK; error = 1;}
  //else if((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 ))  {mode = KANAN; error = 1;}
  else if((LFSensor[0]== 1 )&&(LFSensor[2]== 1 ))  {mode = FOLLOWING_LINE; error = 0;}
  //else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 ))  {mode = KIRI; error = -1;}
  else if((LFSensor[0]== 0 )&&(LFSensor[2]== 1 ))  {mode = KIRI_BANYAK; error = -1;}
  //else if((LFSensor[0]== 1 )&&(LFSensor[2]== 1 ))  {mode = STOPPED; error = 0;}
  else if((LFSensor[0]== 0 )&&(LFSensor[2]== 0 ))  {mode = NO_LINE; error = 0;}*/

  /*if((     LFSensor[0]== 1 )&&(LFSensor[2]== 0 ))  {mode = KANAN_BANYAK; error = 1;}
  //else if((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 ))  {mode = KANAN; error = 1;}
  else if((LFSensor[0]== 0 )&&(LFSensor[2]== 0 ))  {mode = FOLLOWING_LINE; error = 0;}
  //else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 ))  {mode = KIRI; error = -1;}
  else if((LFSensor[0]== 0 )&&(LFSensor[2]== 1 ))  {mode = KIRI_BANYAK; error = -1;}*/
  //else if((LFSensor[0]== 1 )&&(LFSensor[2]== 1 ))  {mode = STOPPED; error = 0;}
 // else if((LFSensor[0]== 0 )&&(LFSensor[2]== 0 ))  {mode = STOPPED; error = 0;}


}

void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);


}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000, myTimerEvent);
  pinMode(dir, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(motor1, OUTPUT);
  pinMode(trig1,OUTPUT);
  pinMode(echo1, INPUT);
  //pinMode(trig2,OUTPUT);
  //pinMode(echo2, INPUT);
  pinMode(relayus, OUTPUT);
  pinMode(lefty, INPUT);
  pinMode(mid, INPUT);
  pinMode(righty, INPUT);
  myservo.attach(27);

  
}

void loop()
{
  Blynk.run();
  timer.run();
  ultrasonic();
  followline();

  Serial.println("left:");
  Serial.println(LFSensor[0]);
  Serial.println("mid:");
  Serial.println(LFSensor[1]);
  Serial.println("right:");
  Serial.println(LFSensor[2]);

  Blynk.virtualWrite(V7,LFSensor[2]);
  Blynk.virtualWrite(V9,LFSensor[0]);
  Blynk.virtualWrite(V12,LFSensor[1]);



  if (distance1 == 15){
    
    reverse();
    delay(100);
    stopping();
    digitalWrite(relayus, LOW);//light off

  }
  else if(distance2 == 15){
    
    motorstart();
    delay(100);
    stopping();    
    digitalWrite(relayus, LOW);//light off
    
  }
  else{
    //digitalWrite(relayus, LOW);//light off
  }

  switch (mode)
  {
    case STOPPED: 
    
      stopping();
      previousError = error;
      
      break;

    case NO_LINE: 

      stopping();
      motorleft();
      previousError = 0;

      break;

    case FOLLOWING_LINE:   

      motorPIDcontrol();    

      break;     
    case KANAN_BANYAK:
      
      //motorPIDcontrol();
      motorright();

      break;
    
    case KANAN:

      motorright();

      break;
  
    case KIRI:

      motorleft();

      break;
    case KIRI_BANYAK:
      
      //motorPIDcontrol();
      motorleft();
      
      break;      
  }

}
