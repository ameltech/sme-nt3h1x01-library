#include "NT3H1101.h"
#include <Arduino.h>
#include <Wire.h>


static bool readRegisters(byte slaveAddress, byte regToRead, int bytesToRead, byte * dest)
{
	Wire.beginTransmission(slaveAddress);
	Wire.write(regToRead);
	Wire.endTransmission(false); //endTransmission but keep the connection active

	uint8_t bytes = Wire.requestFrom(slaveAddress, bytesToRead); //Ask for bytes, once done, bus is released by default

	// return with error if not readsy
	if (bytes ==0)
	return false;

	while(Wire.available() < bytesToRead); //Hang out until we get the # of bytes we expect

	for(int x = 0 ; x < bytesToRead ; x++) {
		dest[x] = Wire.read();
	}

	Wire.endTransmission(true);

	return true; //
}

static bool writeBufferRegister(uint8_t slaveAddress, byte regToWrite, const uint8_t* dataToWrite, uint16_t dataLen) {
	Wire.beginTransmission(slaveAddress);

	if (!Wire.write(regToWrite))
	return false;

	if (Wire.write(dataToWrite, dataLen)!= dataLen)
	return false;

	if (Wire.endTransmission()!=0) //Stop transmitting
	return false;

	return true;
}

void NT3H1101_C::begin(void){
    Wire.begin();    
}

bool NT3H1101_C::readComponentData(uint8_t nfcPageBuffer[]) {
    return readRegisters(_address, MANUFACTORING_DATA_REG, NFC_PAGE_SIZE, nfcPageBuffer);
}

bool NT3H1101_C::readUserPage(uint8_t userPagePtr, uint8_t nfcPageBuffer[]) {
	if ((userPagePtr + USER_START_REG) > USER_END_REG) {
		return false;
	}
    return  readRegisters(_address, (userPagePtr + USER_START_REG), NFC_PAGE_SIZE, nfcPageBuffer);
}

bool NT3H1101_C::writeUserPage(uint8_t userPagePtr, const uint8_t nfcPageBuffer[]) {  
	
	if ((userPagePtr + USER_START_REG) > USER_END_REG) {
		return false;
	}
	  
    bool ret = writeBufferRegister(_address, (userPagePtr + USER_START_REG), nfcPageBuffer, NFC_PAGE_SIZE);
    if (ret)
        delay(100); // give some time to NC for store the buffer

    return ret;
}


NT3H1101_C smeNfcDriver;
