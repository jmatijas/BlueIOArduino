/*
    BlueIOBluetooth.h  - Arduino library for binary message communication
    between Android application and Arduino device.
    
    Library ensures integrity and accuracy of messages received over
    unreliable bluetooth connection, by calculating checksum of data of
    explicit length.
    
    Library is suitable for applications requiring fast responses with
    relatively small messages, because the message length is limited to maximum
    size that can be received at once in Arduino's serial buffer, preventing
    possible buffer overflows and avoiding pauses until rest of data are
    received. (Message length limit is defined with MSG_MAX_BYTES constant. By
    default it's value is set for Arduino Mega 2560. Effective size of message
    payload is 5 bytes less than MSG_MAX_BYTES, because 5 byte header is
    automatically added by library to the begining of each message.)
    
    Library uses serial interface (RX1 and TX1), to which the bluetooth module
    (HC-06) is connected.

    Hobby project by Josip Matijas, 2018.
    */

#ifndef BlueIOBluetooth_h
#define BlueIOBluetooth_h   

#include "Arduino.h"

#include <stdlib.h>

/*
 * Const
 */
 
#define MSG_MAX_BYTES 63 // 5 bytes header + data payload

#define MSG_MARK_START_BYTE 170

#define PIN_PWM_OUT_START 2
#define PIN_PWM_OUT_END 13
#define PIN_PWM_OUT_SIZE 12

#define PIN_INTERRUPT_START 2
#define PIN_INTERRUPT_END 3
#define PIN_INTERRUPT_SIZE 2

#define PIN_DIGITAL_START 22
#define PIN_DIGITAL_END 53
#define PIN_DIGITAL_SIZE 32

#define PIN_ANALOG_IN_START 0
#define PIN_ANALOG_IN_END 15
#define PIN_ANALOG_IN_SIZE 16


/*
 * Enum with readMessage() execution status values
 */
enum ReadMessageStatus : byte {
    ReadMessageStatus_OK = 0,
    ERROR_START_BYTE_NOT_RECOGNIZED = 1,
    ERROR_LENGTH = 2,
    ERROR_HEADER_CHECKSUM = 3,
    ERROR_DATA_LESS_THAN_LENGTH = 4,
    ERROR_DATA_CHECKSUM = 5,
    ERROR_NO_ENUGH_BYTES = 6
};

/*
 * Enum with writeMessage() execution status values
 */
enum WriteMessageStatus : byte {
    WriteMessageStatus_OK = 0,
    ERROR_NO_AVAILABLE_SERIAL_BUFFER_SPACE = 1,
    ERROR_MESSAGE_TO_LONG = 2
};


/*
 * Main library class
 */

class BlueIOBluetooth {

    public:
    // methods
    BlueIOBluetooth(HardwareSerial *serial);
    void begin(long speed);
    size_t readMessage(uint8_t *buff, int length);
    size_t writeMessage(uint8_t *buff1, int length);
    // Should be used only if readMessage() returns -1, to get error ID,
    // if it is required to know why read has failed.
    enum ReadMessageStatus getLastReadMessageStatus();
    enum WriteMessageStatus getLastWriteMessageStatus();
    
    private:
    // methods
    uint16_t Fletcher16Checksum(uint8_t *data, int count );
    uint8_t headerChecksum(uint8_t startByte, uint8_t length);
    
    // variables
    HardwareSerial *bt;
    enum ReadMessageStatus readMessageStatus;
    enum WriteMessageStatus writeMessageStatus;
    

};

#endif