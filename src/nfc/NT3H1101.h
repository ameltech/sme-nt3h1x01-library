#ifndef NT3H1101_h
#define NT3H1101_h


#include "NT3H1101_model.h"
#include "SmeNfc.h"


#define NXPNFC_ADDRESS 85  // 0x4 is the default for every NXP io ho visto 85

#define MANUFACTORING_DATA_REG 0x0 
#define USER_START_REG 0x1
#define NT3H1101_DEF 1 // just for Arduino debug

#ifdef NT3H1201_DEF             // for th 2K
#define USER_END_REG   0x77
#define CONFIG_REG	   0x7A
#elif  NT3H1101_DEF                     // for th 1K
#define USER_END_REG   0x38 // just the first 8 bytes for th 1K
#define CONFIG_REG	   0x3A
#else
#error NO NFC MODEL DEFINITION
#endif

#define SRAM_START_REG 0xF8
#define SRAM_END_REG   0xFB // just the first 8 bytes

class NT3H1101_C 
{
	private:
	unsigned int _address;
public:
    NT3H1101_C(void): _address(NXPNFC_ADDRESS){}; 
		
    /*
	* Initialize the I2C bus as master
	*/
    void begin(void);	
		
    /*
	* Read the manufacturing data
	*/
    bool readComponentData(uint8_t nfcPageBuffer[]);		
    		
    /*
	* Read the a page within the space reserved for the user
	*
	*	userPagePtr = 0 for the first page 0x37 for the last page
	*/
	bool readUserPage(uint8_t userPagePtr, uint8_t nfcPageBuffer[]);
	
	/*
	* write a page within the space reserved for the user
	*
	*	userPagePtr = 0 for the first page 0x37 for the last page
	*/
    bool writeUserPage(uint8_t userPagePtr, const uint8_t nfcPageBuffer[]);
    
};

extern NT3H1101_C smeNfcDriver;

#endif
