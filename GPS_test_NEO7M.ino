
#include <TimeLib.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define RXPin 4
#define TXPin 3

#define UTC_offset -3

TinyGPSPlus gps;
SoftwareSerial Serial_GPS(RXPin, TXPin);

void setup() {
  Serial.begin(115200);
  Serial_GPS.begin(9600);
  GPS_Timezone_Adjust();

  Serial.println(F("Date, Time, Latitude e Longitude"));
  Serial.println(F("Modulo GPS NEO-7M"));
  Serial.print(F("Biblioteca TinyGPS++ v. ")); 
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println();
    
}

void loop() {
  while (Serial_GPS.available() > 0)
    if (gps.encode(Serial_GPS.read()))

        if (millis() > 5000 && gps.charsProcessed() < 10) {
            Serial.println(F("No GPS detected: check wiring."));
            while (true);
        }

    GPS_Timezone_Adjust();
    delay(1000);
}

void displayInfo() {
  if (gps.location.isValid()) {
      Serial.print("Longitude = ");
      Serial.print(gps.location.lng(), 6);
      Serial.print(F("; "));
      Serial.print("Latitude = ");
      Serial.print(gps.location.lat(), 6);
  }
  else {
    Serial.print(F("Invalid"));
  }

  Serial.print(F("> Date/Time: "));
  if (gps.date.isValid()) {
    Print_Date()
  }
  else {
    Serial.print(F("Invalid"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour()); //hora
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute()); //minuto
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second()); //segundo
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else {
    Serial.print(F("Invalid"));
  }
  Serial.println();
}

void GPS_Timezone_Adjust() {
  while (Serial_GPS.available()) {
    if (gps.encode(Serial_GPS.read())) {
      int Year = gps.date.year();
      byte Month = gps.date.month();
      byte Day = gps.date.day();
      byte Hour = gps.time.hour();
      byte Minute = gps.time.minute();
      byte Second = gps.time.second();

      setTime(Hour, Minute, Second, Day, Month, Year);
      adjustTime(UTC_offset * SECS_PER_HOUR);
    }
  }
}

void Print_Date() {
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
}
