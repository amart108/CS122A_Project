#include "SoftwareSerial.h"
#include "TinyGPS.h"

#define latFlag 6
#define lonFlag 7
#define reachedLat 8
#define reachedLon 9
long lat, lon;
unsigned long Date, Time, Age; // the variable for latitude and longitude object
//float latit, longit, diff1, diff2;
long la, lo, LatC, LatC2, LonC, LonC2, diff1, diff2, diff1Prev, diff2Prev;

SoftwareSerial serial_connection(3,2); // RX Pin = 3, TX Pin = 2
TinyGPS gps; // create gps object
String NMEAL1; // Variable for 1st NMEA sentence
String NMEAL2; // Variable for 2nd NMEA sentence
String temp;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // connect serial
  pinMode(latFlag, OUTPUT);
  pinMode(lonFlag, OUTPUT);
  serial_connection.begin(9600);// connect gps sensor
  //gpsSerial.begin(4800); 
  Serial.println("GPS Start");
}

void loop() {

  GPS();
 // put your main code here, to run repeatedly:
  
   
  
}

void GPS()
{
  while(!serial_connection.available())
  {
    //Serial.println("Starting Good Signal");
    //TinyGPS();
    
  }
  Serial.flush();
  if(gps.encode(serial_connection.read()))
    {
      //la = 33982130;
      //lo = -117337942;
      la = 33981954;
      lo = -117338032;
      Serial.println(gps.satellites());
      Serial.println("Results:");
      gps.get_position(&lat, &lon); // get latitude and longitude
     // lat = 33981585;
     // lon = -117338073;
     
      diff1Prev = diff1;
      diff2Prev = diff2;
      diff1 = la - lat; 
      diff2 = lo -  lon;
      LatC = (lat / 1000000);
      LatC2 = (lat % 1000000); 
     // Serial.println(LatC2);
      Serial.print("Latitude: " + String(lat));
      Serial.println(", Parsed: " + String(LatC) + "." + String(LatC2)); 
//     
      LonC = (lon / 1000000);
      LonC2 = (lon % 1000000);
      LonC2 *= -1; 
     // Serial.println(LonC2);
      Serial.print("Longitude: " + String(lon));
      Serial.println(", Parsed: " + String(LonC) + "." + String(LonC2));

     // Serial.println(lon);
      Serial.println("Differences: ");
      
      Serial.println("Latitude differences: " + String(la) + " - " + String(lat) + " = " + String(diff1));
      Serial.println("Previous diff: " + String(diff1Prev)); 

      Serial.println("Longitude differences: " + String(lo) + " - " + String(lon)  + " = " + String(diff2)); 
      Serial.println("Previous diff: " + String(diff2Prev)); 
      if((diff1 <= 3) || (diff1 >= -3))
      {
        digitalWrite(reachedLat, HIGH);
        digitalWrite(latFlag, LOW);
      }
      else
      {
        digitalWrite(reachedLat, LOW);
      }
      
      
      if(diff1Prev > diff1)
      {
        digitalWrite(latFlag, HIGH);
      }
      else
      {
        digitalWrite(latFlag, LOW);
      }

      if((diff2 <= 3) || (diff2 >= -3))
      {
        digitalWrite(reachedLon, HIGH);
        digitalWrite(lonFlag, LOW);
      }
      else
      {
        digitalWrite(reachedLon, LOW);
      }
      
      
      if(diff2Prev > diff2)
      {
        digitalWrite(lonFlag, HIGH);
      }
      else
      {
        digitalWrite(lonFlag, LOW);
      }
     // Serial.println("Altitude feet:");
     // Serial.println(gps.f_altitude());
      Serial.println();
    } 
    //Serial.flush();
}

