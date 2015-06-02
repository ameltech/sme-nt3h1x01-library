/*
 * Nfc.cpp
 *
 *  Created on: Apr 27, 2015
 *  by Mik (smkk@amel-tech.com)
 *
 */

#include "SmeNfc.h"
#include "NT3H1101.h"
#include "ndef/rtd/rtdUri.h"
#include "ndef/rtd/rtdText.h"
#include "ndef/ndef.h"

SmeNfc::SmeNfc() {
    // TODO Auto-generated constructor stub

}

SmeNfc::~SmeNfc() {
    // TODO Auto-generated destructor stub
}

void SmeNfc::begin(void) {
    smeNfcDriver.begin();
}

bool SmeNfc::storeUrihttp(RecordPosEnu position, char *http){

    NDEFDataStr data;


    prepareUrihttp(&data, position, http);
    return   NT3HwriteRecord( &data );
}


bool SmeNfc::storeText(RecordPosEnu position, char *text){
    NDEFDataStr data;


    prepareText(&data, position, text);
    return NT3HwriteRecord( &data );
}

uint8_t      nfcPageBuffer[NFC_PAGE_SIZE];
NT3HerrNo    errNo;

SmeNfc smeNfc;
