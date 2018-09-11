/*
    BlueIOBluetooth.cpp  - Arduino library for binary message communication
    between Android application and Arduino device.

    Hobby project by Josip Matijas, 2018.
    */

#include "Arduino.h"
#include <stdlib.h>

#include "BlueIOBluetooth.h"


BlueIOBluetooth::BlueIOBluetooth(HardwareSerial *serial) {
    this->bt = serial;
}


void BlueIOBluetooth::begin(long speed) {
    bt->begin(speed);
}


size_t BlueIOBluetooth::readMessage(uint8_t *buff, int length) {
    readMessageStatus = ReadMessageStatus_OK;
    int pos = 0;
    int msgByte_start = -1;
    int msgByte_length = -1;
    int msgByte_headerChecksum = -1;
    uint16_t msgDataChecksum = 0;
    
    if(bt->available() > 5) { // 3 bytes header and 2 bytes footer (checksum)
        msgByte_start = bt->read();
        msgByte_length = bt->read();
        msgByte_headerChecksum = bt->read();
      
    } else {
        // No enough bytes to read valid msg
        readMessageStatus = ERROR_NO_ENUGH_BYTES;
        // clear serial buffer by reading rest of invalid data in it
        while(bt->available() > 0) bt->read();
        return -1;
    }
    
    // check start byte
    if(msgByte_start != MSG_MARK_START_BYTE) {
        readMessageStatus = ERROR_START_BYTE_NOT_RECOGNIZED;
        // clear serial buffer by reading rest of invalid data in it
        while(bt->available() > 0) bt->read();
        return -1;
    }
    
    // check length byte
    if(msgByte_length > MSG_MAX_BYTES) {
        readMessageStatus = ERROR_LENGTH;
        // clear serial buffer by reading rest of invalid data in it
        while(bt->available() > 0) bt->read();
        return -1;
    }
    
    // validate header checksum (sum of 1st and 2nd byte)
    if(msgByte_headerChecksum != (msgByte_start + msgByte_length)) {
        readMessageStatus = ERROR_HEADER_CHECKSUM;
        // clear serial buffer by reading rest of invalid data in it
        while(bt->available() > 0) bt->read();
        return -1;
    }
    
    // check if rest of available data matches the msgByte_length - 3
    if(bt->available() != (msgByte_length - 3)) {
        readMessageStatus = ERROR_DATA_LESS_THAN_LENGTH;
        // clear serial buffer by reading rest of invalid data in it
        while(bt->available() > 0) bt->read();
        return -1;
    }
    
    // read 2 bytes (little endian) data checksum
    msgDataChecksum = bt->read();
    msgDataChecksum = (msgDataChecksum << 8 | (uint16_t)(bt->read()));
    
    // read rest of msg data
    for(int i = 0; i < (msgByte_length - 5); i++) {
        buff[i] = bt->read();
    }
    
    // calculate Flatcher checksum of received data and compare with received
    uint16_t calcChksum = Fletcher16Checksum(buff, msgByte_length - 5);
    
    Serial.print("calcChksum: ");
    Serial.println(calcChksum);
    if(calcChksum == msgDataChecksum) {
        Serial.println("_JM_ checksum OK !!!");
        return msgByte_length - 5; // Return positive status (positive number)
    } else {
        readMessageStatus = ERROR_DATA_CHECKSUM;
        Serial.print("_JM_ checksum ERROR !!! rcvd chksum: ");
        Serial.println(msgDataChecksum);
        // else return error
        return -1;
    }
}


size_t BlueIOBluetooth::writeMessage(uint8_t *buff1, int length) {
    size_t returnBytesWritten = 0; // not yet data written
    writeMessageStatus = WriteMessageStatus_OK;
    
    if (bt->availableForWrite() < (length + 5)) {
        writeMessageStatus = ERROR_NO_AVAILABLE_SERIAL_BUFFER_SPACE;
        return -1;
    }
    
    if ((length + 5) > MSG_MAX_BYTES) {
        writeMessageStatus = ERROR_MESSAGE_TO_LONG;
        return -1;
    }
    
    uint8_t headerChksum = headerChecksum((uint8_t)MSG_MARK_START_BYTE, (uint8_t)(length + 5));
    uint16_t dataChksum = Fletcher16Checksum(buff1, length);
    bt->write(length + 5);
    
    returnBytesWritten += bt->write(MSG_MARK_START_BYTE);
    returnBytesWritten += bt->write(headerChksum);
    returnBytesWritten += bt->write(dataChksum & 255);
    returnBytesWritten += bt->write((dataChksum >> 8) & 255);
    returnBytesWritten += bt->write(buff1, length);
    
    Serial.print("_JM_ hChk, dataChk: ");
    Serial.print(headerChksum);
    Serial.print(" ");
    Serial.print(dataChksum);
    
    return returnBytesWritten;
}


enum ReadMessageStatus BlueIOBluetooth::getLastReadMessageStatus() {
    return readMessageStatus;
}


enum WriteMessageStatus BlueIOBluetooth::getLastWriteMessageStatus() {
    return writeMessageStatus;
}


/*
 * 16-bit Flatcher checksum (wikipedia)
 */
uint16_t BlueIOBluetooth::Fletcher16Checksum(uint8_t *data, int count ) {
  uint16_t sum1 = 0;
  uint16_t sum2 = 0;
  int index;

  for( index = 0; index < count; ++index )
  {
    sum1 = (sum1 + data[index]) % 255;
    sum2 = (sum2 + sum1) % 255;
  }

  return (sum2 << 8) | sum1;
}

/*
 * 8-bit header checksum - sum of first two message bytes
 */
uint8_t BlueIOBluetooth::headerChecksum(uint8_t startByte, uint8_t length) {
    return (uint8_t)(startByte + length);
}
