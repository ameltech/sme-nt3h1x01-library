/*
 * ndef.c
 *
 * Created: 3/29/2015 12:59:57 PM
 * by Mik (smkk@amel-tech.com)
 */ 

#include "ndef.h"
#include <string.h>
#include "rtd/nfcForum.h"
#include "rtd/rtdTypes.h"
#include "../NT3H1101_model.h"
#include "../NT3H1101.h"


/*
 * This strucure is used in the ADD record functionality
 * to store the last nfc page information, in order to continue from that point.
 */
typedef struct {
    uint8_t page;
    uint8_t usedBytes;
} UncompletePageStr;

typedef uint8_t (*composeRtdPtr)(const NDEFDataStr *ndef, NDEFRecordStr *ndefRecord, uint8_t *I2CMsg);
static composeRtdPtr composeRtd[] = {composeRtdText,composeRtdUri};


static bool NT3HReadUserData(uint8_t page) {

    bool ret = smeNfcDriver.readUserPage(page, nfcPageBuffer);

    if (ret == false) {
        errNo = NT3HERROR_READ_USER_MEMORY_PAGE;
    }

    return ret;
}




static bool NT3HWriteUserData(uint8_t page, const uint8_t* data) {
    bool ret = true;

    ret = smeNfcDriver.writeUserPage(page, data);
    if (ret == false) {
        errNo = NT3HERROR_WRITE_USER_MEMORY_PAGE;
        goto end;
    }

    end:
    return ret;
}


static bool NT3HWriteHeaderNfc(uint8_t endRecordsPtr, uint8_t ndefHeader) {

    // read the first page to see where is the end of the Records.
    bool ret = NT3HReadUserData(0);
    if (ret == true) {

        nfcPageBuffer[1] = endRecordsPtr;
        nfcPageBuffer[2] = ndefHeader;
        ret = NT3HWriteUserData(0, nfcPageBuffer);
        if (ret == false) {
            errNo = NT3HERROR_WRITE_HEADER;
        }
    } else {
        errNo = NT3HERROR_READ_HEADER;
    }

    return ret;
}

static bool NT3HReadHeaderNfc(uint8_t *endRecordsPtr, uint8_t *ndefHeader) {
    *endRecordsPtr=0;
    bool ret = NT3HReadUserData(0);

    // read the first page to see where is the end of the Records.
    if (ret == true) {
        // if the first byte is equals to NDEF_START_BYTE there are some records
        // store theend of that
        if ((NDEF_START_BYTE == nfcPageBuffer[0]) && (NTAG_ERASED != nfcPageBuffer[2])) {
            *endRecordsPtr = nfcPageBuffer[1];
            *ndefHeader    = nfcPageBuffer[2];
        }
        return true;
    } else {
        errNo = NT3HERROR_READ_HEADER;
    }

    return ret;
}



int16_t firstRecord(UncompletePageStr *page, const NDEFDataStr *data, RecordPosEnu rtdPosition) {

    NDEFRecordStr record;
    NDEFHeaderStr header;
    uint8_t typeFunct=0;

    switch (data->rtdType){
    case RTD_TEXT:
        typeFunct =0;
        break;

    case RTD_URI:
        typeFunct = 1;
        break;

    default:
        return -1;
        break;
    }

    // clear all buffers
    memset(&record,0,sizeof(NDEFRecordStr));
    memset(nfcPageBuffer, 0, NFC_PAGE_SIZE);

    // this is the first record
    header.startByte = NDEF_START_BYTE;
    composeNDEFMBME(true, true, &record);

    // prepare the NDEF Header and payload
    uint8_t recordLength = composeRtd[typeFunct](data, &record, &nfcPageBuffer[sizeof(NDEFHeaderStr)]);
    header.payloadLength = data->rtdPayloadlength + recordLength;

    // write first record
    memcpy(nfcPageBuffer, &header, sizeof(NDEFHeaderStr));

    return sizeof(NDEFHeaderStr)+recordLength;

}


int16_t addRecord(UncompletePageStr *pageToUse, const NDEFDataStr *data, RecordPosEnu rtdPosition) {
    NDEFRecordStr record;
    NDEFHeaderStr header;
    uint8_t       newRecordPtr, mbMe;
    uint8_t       tmpBuffer[NFC_PAGE_SIZE];

    uint8_t typeFunct=0;

    switch (data->rtdType){
    case RTD_TEXT:
        typeFunct =0;
        break;

    case RTD_URI:
        typeFunct = 1;
        break;

    default:
        return -1;
        break;
    }

    // first Change the Header of the first Record
    NT3HReadHeaderNfc(&newRecordPtr, &mbMe);
    record.header = mbMe;
    composeNDEFMBME(true, false, &record); // this is the first record
    mbMe = record.header;

    memset(&record,0,sizeof(NDEFRecordStr));
    memset(tmpBuffer,0,NFC_PAGE_SIZE);

    // prepare second record
    uint8_t recordLength = composeRtd[typeFunct](data, &record, tmpBuffer);

    if (rtdPosition == NDEFMiddlePos) {
        // this is a record in the middle adjust it on the buffet
        composeNDEFMBME(false, false, &record);
    } else if (rtdPosition == NDEFLastPos){
        // this is the last record adjust it on the buffet
        composeNDEFMBME(false, true, &record);
    }

    tmpBuffer[0] = record.header;

    header.payloadLength += data->rtdPayloadlength + recordLength;

    // save the new value of length on the first page
    NT3HWriteHeaderNfc((newRecordPtr+header.payloadLength), mbMe);


    // use the last valid page and start to add the new record
    NT3HReadUserData(pageToUse->page);
    if (pageToUse->usedBytes+recordLength< NFC_PAGE_SIZE) {
        memcpy(&nfcPageBuffer[pageToUse->usedBytes], tmpBuffer, recordLength);
        return recordLength+pageToUse->usedBytes;
    } else {
        uint8_t byteToCopy = NFC_PAGE_SIZE-pageToUse->usedBytes;
        memcpy(&nfcPageBuffer[pageToUse->usedBytes], tmpBuffer, byteToCopy);
        NT3HWriteUserData(pageToUse->page, nfcPageBuffer);
        // update the info with the new page
        pageToUse->page++;
        pageToUse->usedBytes=recordLength-byteToCopy;
        //copy the remain part in the pageBuffer because this is what the caller expect
        memcpy(nfcPageBuffer, &tmpBuffer[byteToCopy], pageToUse->usedBytes);
        return pageToUse->usedBytes;
    }

}



static bool writeUserPayload(int16_t payloadPtr, const NDEFDataStr *data, UncompletePageStr *addPage){
    uint8_t addedPayload;
    bool ret=false;


    bool endRecord = false;
    uint8_t copyByte=0;

    // if the header is less then the NFC_PAGE_SIZE, fill it with the payload
    if (NFC_PAGE_SIZE>payloadPtr) {
        if (data->rtdPayloadlength > NFC_PAGE_SIZE-payloadPtr)
            copyByte = NFC_PAGE_SIZE-payloadPtr;
        else
            copyByte = data->rtdPayloadlength;
    }

    // Copy the payload
    memcpy(&nfcPageBuffer[payloadPtr], data->rtdPayload, copyByte);
    addedPayload = copyByte;


    //if it is sufficient one send add the NDEF_END_BYTE
    if ((addedPayload >= data->rtdPayloadlength)&&((payloadPtr+copyByte) < NFC_PAGE_SIZE)) {
        nfcPageBuffer[(payloadPtr+copyByte)] = NDEF_END_BYTE;
        endRecord = true;
    }

    ret = NT3HWriteUserData(addPage->page, nfcPageBuffer);

    while (!endRecord) {
        addPage->page++; // move to a new register
        memset(nfcPageBuffer,0,NFC_PAGE_SIZE);

        //special case just the NDEF_END_BYTE remain out
        if (addedPayload == data->rtdPayloadlength) {
            nfcPageBuffer[0] = NDEF_END_BYTE;
            ret = NT3HWriteUserData(addPage->page, nfcPageBuffer);
            endRecord = true;
            if (ret == false) {
                errNo = NT3HERROR_WRITE_NDEF_TEXT;
            }
            goto end;
        }

        if (addedPayload < data->rtdPayloadlength) {

            // add the NDEF_END_BYTE if there is enough space
            if ((data->rtdPayloadlength-addedPayload) < NFC_PAGE_SIZE){
                memcpy(nfcPageBuffer, &data->rtdPayload[addedPayload], (data->rtdPayloadlength-addedPayload));
                nfcPageBuffer[(data->rtdPayloadlength-addedPayload)] = NDEF_END_BYTE;
            } else {
                memcpy(nfcPageBuffer, &data->rtdPayload[addedPayload], NFC_PAGE_SIZE);
            }

            addedPayload += NFC_PAGE_SIZE;
            ret = NT3HWriteUserData(addPage->page, nfcPageBuffer);


            if (ret == false) {
                errNo = NT3HERROR_WRITE_NDEF_TEXT;
                goto end;
            }
        } else {
            endRecord = true;
        }
    }

    end:
    return ret;
}


typedef int16_t (*addFunct_T) (UncompletePageStr *page, const NDEFDataStr *data, RecordPosEnu rtdPosition);
static addFunct_T addFunct[] = {firstRecord, addRecord, addRecord};

bool NT3HwriteRecord(const NDEFDataStr *data) {


    uint8_t recordLength=0, mbMe;
    UncompletePageStr addPage;
    addPage.page = 0;
    addPage.usedBytes=0;

    // calculate the last used page
    if (data->ndefPosition != NDEFFirstPos ) {
        NT3HReadHeaderNfc(&recordLength, &mbMe);
        addPage.page  = (recordLength+sizeof(NDEFHeaderStr)+1)/NFC_PAGE_SIZE;

        //remove the NDEF_END_BYTE byte because it will overwrite by the new Record
        addPage.usedBytes = (recordLength+sizeof(NDEFHeaderStr)+1)%NFC_PAGE_SIZE - 1;
    }


    // call the appropriate function and consider the pointer
    // within the NFC_PAGE_SIZE that need to be used
    int16_t payloadPtr = addFunct[data->ndefPosition](&addPage, data, data->ndefPosition);
    if (payloadPtr == -1) {
        errNo = NT3HERROR_TYPE_NOT_SUPPORTED;
        return false;
    }

    return  writeUserPayload(payloadPtr, data, &addPage);
}

