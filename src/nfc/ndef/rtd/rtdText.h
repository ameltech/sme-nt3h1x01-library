/*
 * ndefText.h
 *
 *  Created on: Mar 24, 2015
 * by Mik (smkk@amel-tech.com)
 *
 */

#ifndef RTDTEXT_H_
#define RTDTEXT_H_

#include <stdint-gcc.h>
#include "..\..\..\SmeNfc.h"

#define BIT_STATUS (1<<7)
#define BIT_RFU	   (1<<6)


#define MASK_STATUS 0x80
#define MASK_RFU    0x40
#define MASK_IANA   0b00111111

typedef struct {
    char *body;
    uint8_t bodyLength;
}RtdTextUserPayload;

typedef struct {
    uint8_t     status;
    uint8_t     language[2];
    RtdTextUserPayload rtdPayload;
}RtdTextTypeStr;


uint8_t addRtdText(RtdTextTypeStr *typeStr);

void prepareText(NDEFDataStr *data, RecordPosEnu position,char *text);
#endif /* NDEFTEXT_H_ */
