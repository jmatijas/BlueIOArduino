/*
    BlueIO.h  - Arduino firmware enabling controller device (e.g. Android phone
    application) to use Arduino Mega 2560 IO capabilities over Bluetooth
    communication channel.
    
    BlueIO uses BlueIOBluetooth library for Bluetooth communication. Bluetooth
    module HC-06 is connected to serial interface (RX1 and TX1).

    Hobby project by Josip Matijas, 2018.
    */

#ifndef BlueIO_h
#define BlueIO_h   

#include "Arduino.h"


/*
 * Const
 */

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


enum BlueIOCommandID : uint8_t {
    READ_DIGITAL_PIN,
    WRITE_DIGITAL_PIN,
    READ_ANALOG_PIN,
    WRITE_PWM_PIN, // analog output pin not available, but PWM signal possible
    MEASURE_DISTANCE_ULTRASONIC_HC_SR04,
    READ_DISTANCE_ULTRASONIC_HC_SR04,
    MEASURE_TEMPERATURE_DS18B20,
    READ_TEMPERATURE_DS18B20,
    OPEN_MOTORIZED_VALVE,
    CLOSE_MOTORIZED_VALVE
}


/*
 * Main class
 */

class BlueIO {
    
    public:
    
    //
    // methods
    //
    
    BlueIO();
    
    /*
     * Should be called directly from the main arduino's sketch loop function.
     * 
     * In each loop following should be done:
     * Try to read an incomming command from the controller app.
     * If command is received, execute the command.
     * Immediatelly upon the execution, send the response with execution result
     * to the controller application.
     */
    void loop();
    
    private:
    
    //
    // methods
    //
    
    
    
    //
    // variables
    //
    
};

#endif