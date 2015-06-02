/*
 * Nfc.h
 *
 *  Created on: Apr 27, 2015
 *  by Mik (smkk@amel-tech.com)
 *
 */

#ifndef NFC_H_
#define NFC_H_


#include <Arduino.h>

#include "nfc/NT3H1101.h"

typedef enum {
    NDEFFirstPos,
    NDEFMiddlePos,
    NDEFLastPos
} RecordPosEnu;



typedef struct {
    RecordPosEnu ndefPosition;
    byte rtdType;
    byte *rtdPayload;
    byte rtdPayloadlength;
    void    *specificRtdData;
}NDEFDataStr;


class SmeNfc {
public:
    SmeNfc();
    virtual ~SmeNfc();
    void begin(void);
    
    /*
     * The function write in the NT3H a new URI Rtd on the required position
     *
     * param:
     *      position: where add the record
     *      http:     the address to write
     *
     */
    bool storeUrihttp(RecordPosEnu position, char *http);


    /*
     * The function write in the NT3H a new Text Rtd on the required position
     *
     * param:
     *      position: where add the record
     *      text:     the text to write
     *
     */
    bool storeText(RecordPosEnu position,char *text);
};

extern SmeNfc smeNfc;
#endif /* NFC_H_ */
