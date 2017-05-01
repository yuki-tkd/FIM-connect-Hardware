#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__A
 #include <avr/power.h>
#endif

#define PIN       7
#define NUMPIXELS 11
#define DEVICEID  13

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(PIN, OUTPUT);  
  pixels.begin();

  Wire.begin(DEVICEID);         // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent);
}

void loop() {

}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  int d = analogRead(0);
  Wire.write(0); // respond with message of 6 bytes
  // as expected by master
}

void receiveEvent(int d) {
  while(Wire.available() > 0) {
    byte x = Wire.read();
    Serial.println(x);
    if(x == 1) {
      for(int i=0; i<NUMPIXELS; i++){
        pixels.setPixelColor(i, pixels.Color(255,140,0));
        pixels.setBrightness(200);
        pixels.show();
      } 
    } else {
      pixels.clear();
      pixels.show();
    }
  }
}
