/*
    SmeNfc Library - WriteUriNdef

    Demonstrate how to write an NDEF URI format on the NFC component.

	And the ends the green led continuously blink.
	Send on console the Manufactoring Data.

    created 27 Apr 2015
    by Mik (smkk@amel-tech.com)

    This example is in the public domain
	https://github.com/ameltech

	NXP  mor einformatioin available here:
	http://www.nxp.com/products/identification_and_security/nfc_and_reader_ics/connected_tag_solutions/series/NT3H1101_NT3H1201.html
 */

#include <Wire.h>

#include <SmeNfc.h>
#include <Arduino.h>

#define WEB             "amel-tech.com"

bool nfcOk;
byte buffer[NFC_PAGE_SIZE];


// the setup function runs once when you press reset or power the board
void setup() {

   // Initialize the SmartEverythin as Master
   Wire.begin();

  int i;

  // just clear the buffer
  for (i = 0; i < NFC_PAGE_SIZE; i++) {
    buffer[i] = 0;
  }

  // initialize USB console
  SerialUSB.begin(115200);

  if (smeNfcDriver.readManufactoringData(buffer)) {
    smeNfc.storeUrihttp(NDEFFirstPos, WEB);

    nfcOk = true;
  }
}

// the loop function runs over and over again forever
void loop() {

  // just waste time
  if (nfcOk == true) {

    int i = 0;
    
    // send Manufactoring data on Console
    for (i = 0; i < NFC_PAGE_SIZE; i++) {
      SerialUSB.print(buffer[i], HEX); 
      SerialUSB.print(':');
    }
    SerialUSB.println();

    ledRedLight(LOW);	// turn the LED Off
    delay(1000);

    ledRedLight(HIGH);  // turn the LED on
    delay(1000);        // wait for a second
  }
}