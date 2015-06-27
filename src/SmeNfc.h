/*
 * Nfc.h
 *
 *  Created on: Apr 27, 2015
 *  by Mik (smkk@amel-tech.com)
 *
 * License Information
 * -------------------
 *
 * Copyright (c) Amel Technology. All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
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