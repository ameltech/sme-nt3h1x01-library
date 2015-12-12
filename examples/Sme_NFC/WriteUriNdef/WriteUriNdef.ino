/*
    SmeNfc Library - WriteUriNdef

    Demonstrate how to write an NDEF URI format on the NFC component.
    In the end, make the RBG led continuously blink red and
    send to the console the 7-byte UID of the NFC NT3H1101 chip

    created 27 Apr 2015
    Authors: Mik (smkk@amel-tech.com), Bill (github@unbiot.com)

    This example is in the public domain
    https://github.com/ameltech

    NXP more information available here:
    http://www.nxp.com/products/identification_and_security/nfc_and_reader_ics/connected_tag_solutions/series/NT3H1101_NT3H1201.html
*/

#include <Wire.h>
#include <SmeNfc.h>
#include <Arduino.h>

#define WEB             "amel-tech.com"

bool nfcOk;
byte buffer[UID_SIZE];


// the setup function runs once when you press reset or power the board
void setup() {

  // Initialize the SmartEverything as Master
  Wire.begin();

  // just clear the buffer
  for (int i = 0; i < UID_SIZE; i++) {
    buffer[i] = 0;
  }

  // initialize USB console
  SerialUSB.begin(115200);

  if (smeNfcDriver.readUID(buffer)) {
    smeNfc.storeUrihttp(NDEFFirstPos, WEB);
    nfcOk = true;
  }
}

// the loop function runs over and over again forever
void loop() {

  // just waste time
  if (nfcOk == true) {
    
    // Send the UID of the NT3H1101 to the Console
    SerialUSB.print("Serial number (UID): ");
    for (int i = 0; i < UID_SIZE; i++) {
      SerialUSB.print(buffer[i], HEX); 
      SerialUSB.print(':');
    }
    SerialUSB.println();

    ledGreenLight(LOW);  // turn the LED Off
    delay(1000);

    ledGreenLight(HIGH);  // turn the LED On
    delay(1000);
  }
}
