/*
  Adafruit Arduino - Lesson 4. 8 LEDs and a Shift Register
*/

#include "DHT.h"

#define DHTPIN 3     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)


DHT dht(DHTPIN, DHTTYPE);

int latchPin = 1;
int clockPin = 2;
int dataPin = 0;

byte leds = 0;

void setup()
{
  //Serial.begin(9600);
  //Serial.println("TEST");
  dht.begin();
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  leds = 0;
  updateShiftRegister();
}

void loop()
{

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  //Serial.print("Humidity: ");
  //Serial.print(h);
  //Serial.print(" %\t");
  //Serial.print("Temperature: ");
  //Serial.print(t);
  //Serial.println(" *C ");


  leds = 0;



  if (isnan(h) || isnan(t)) {
    bitSet(leds, 2);
    bitSet(leds, 3);
    bitClear(leds, 5);
    bitClear(leds, 6);
    bitSet(leds, 4);
    bitClear(leds, 7);
    updateShiftRegister();
    return;
  }
  else if ((t >= 35) && (h <= 25))
  {
    bitClear(leds, 2);  // everything GOOD --> GREEN & fan ON
    bitSet(leds, 3);
    bitSet(leds, 5);
    bitClear(leds, 6);
    bitClear(leds, 4);
    bitClear(leds, 7);
    updateShiftRegister();
  }
  else if (t < 35)
  {
    bitSet(leds, 2);  //temp. bad ----> show red
    bitClear(leds, 3);
    bitSet(leds, 5);
    bitSet(leds, 6);
    bitClear(leds, 4);
    bitClear(leds, 7);
    updateShiftRegister();
  }
  else if (h > 25)
  {
    bitSet(leds, 4);  //humidity bad ----> show blue
    bitClear(leds, 3);
    bitSet(leds, 5);
    bitClear(leds, 6);
    bitClear(leds, 2);
    bitClear(leds, 7);
    updateShiftRegister();
  }



  // Compute heat index
  // Must send in temp in Fahrenheit!
  delay(5000);


}

void updateShiftRegister()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}
