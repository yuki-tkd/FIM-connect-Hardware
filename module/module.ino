#include <Wire.h>

#define DEVICEID 11

void setup() {
  Wire.begin(DEVICEID);         // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop() {

}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  int d = analogRead(0);
  Wire.write(d >> 2); // respond with message of 6 bytes
  // as expected by master
}
