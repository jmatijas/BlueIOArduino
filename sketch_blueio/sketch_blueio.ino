/* -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 * Arduino as IO device for Android controller device, accessed over the
 * Bluetooth communication.
 * 
 * Hobby project by Josip Matijas, 2018.
 * josip.matijas@gmail.com
 * -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 */
#include "Arduino.h"
#include "BlueIOBluetooth.h"

/* -----------------------------------------------------------------------------
 * 
 * -----------------------------------------------------------------------------
 */

BlueIOBluetooth bluetooth(&Serial1);
uint8_t txData[58];

void setup() 
{
  Serial.begin(9600);
  Serial.println("---- setup() ----");

  bluetooth.begin(9600);
  delay(500);

  Serial.println("---- setup() END");
}

void loop()
{
  //Serial.println("---- loop() ----");

  txData[0] = 33;
  bluetooth.writeMessage(txData, 1);
}
