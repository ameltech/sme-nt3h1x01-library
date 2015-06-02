/*
 * ndefText.c
 *
 *  Created on: Mar 24, 2015
 * by Mik (smkk@amel-tech.com)
 *
 */

#include "rtdText.h"
#include "rtdTypes.h"
#include <string.h>



uint8_t addRtdText(RtdTextTypeStr *typeStr) {

    //	return addNDEFTextPayload(bodyLength, ndefRecord);
    typeStr->status=0x2;
    typeStr->language[0]='e';
    typeStr->language[1]='n';

    return 3;
}

void prepareText(NDEFDataStr *data, RecordPosEnu position, char *text) {
    data->ndefPosition = position;
    data->rtdType = RTD_TEXT;
    data->rtdPayload = (uint8_t*) text;
    data->rtdPayloadlength = strlen((const char*)text);
}
