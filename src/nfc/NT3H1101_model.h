/*
 * NT3H1101_model.h
 *
 * Created: 3/8/2015 11:37:26 AM
 * by Mik (smkk@amel-tech.com)
 *
 */
#include <stdint-gcc.h>

#ifndef NT3H1101_MODEL_H_
#define NT3H1101_MODEL_H_


#define NFC_PAGE_SIZE 16
#define UID_SIZE 7

typedef enum {
    NT3HERROR_NO_ERROR,
    NT3HERROR_READ_HEADER,
    NT3HERROR_WRITE_HEADER,
    NT3HERROR_INVALID_USER_MEMORY_PAGE,
    NT3HERROR_READ_USER_MEMORY_PAGE,
    NT3HERROR_WRITE_USER_MEMORY_PAGE,
    NT3HERROR_ERASE_USER_MEMORY_PAGE,
    NT3HERROR_READ_NDEF_TEXT,
    NT3HERROR_WRITE_NDEF_TEXT,
    NT3HERROR_TYPE_NOT_SUPPORTED
}NT3HerrNo;

// structure that contains the configuration data
typedef struct {
    uint8_t nc_reg;
    uint8_t last_ndef_block;
    uint8_t sram_mirror_block;
    uint8_t wdt_ls;
    uint8_t wdt_ms;
    uint8_t i2c_clock_str;
    uint8_t reg_clock;
    uint8_t unused;
} configS;


// structure that contains the manufactoring data
typedef struct {
    uint8_t  slaveAddr;
    uint8_t  serialNumber[6];
    uint8_t  internalData[3];
    uint16_t lockBytes;
    uint32_t capabilityContainer;
} manufS;

extern uint8_t      nfcPageBuffer[NFC_PAGE_SIZE];
extern NT3HerrNo    errNo;
#endif /* NT3H1101_MODEL_H_ */
