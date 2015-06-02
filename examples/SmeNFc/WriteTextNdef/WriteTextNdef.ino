/*
    SmeNfc Library - WriteTextNdef

    Demostrate how to write an NDEF Text format on the NFC component.
	
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

#include <smeSensors.h>
#include <Arduino.h>

#define SME_2_1         "SmartEverything"

bool nfcOk;
byte buffer[NFC_PAGE_SIZE];

// the setup function runs once when you press reset or power the board
void setup() {
    // initialize digital pin 13 as an output.
    pinMode(PIN_LED_13, OUTPUT);
        
    // initialize the NFC Driver.
    smeNfc.begin();
	SerialUSB.begin(115200);

    if (smeNfcDriver.readComponentData(buffer)) {
        smeNfc.storeText(NDEFFirstPos, SME_2_1);
        nfcOk=true;
    }
}

// the loop function runs over and over again forever
void loop() {

    // just waste time
    if (nfcOk==true) {
        
		SerialUSB.write(buffer, NFC_PAGE_SIZE);		// send Manufacoring data on Console
		
        analogWrite(PIN_LED_13, 128);	// turn the LED On 
        delay(1000);

        analogWrite(PIN_LED_13, 255);   // turn the LED off 
        delay(2000);					// wait for a second
    }
}
