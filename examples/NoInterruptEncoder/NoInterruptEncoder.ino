/**
 * Encoder example with no interruption pin connection;
 * Warning: only use this if there are no free interrupt pins on your device!
 * Warning: loop delay will lead to unstable operation of the encoder!
 *
 * Encoder constructor in this example initialized with default event type value (ENCODER_EVENT_UPDATE)
 * ENCODER_EVENT_UPDATE - all events called by encoder.update() from loop()
 * ENCODER_EVENT_INTERRUPT - all events called by interrupt function directly (if interrupt pins used)
 * Only use ENCODER_EVENT_INTERRUPT if sure what you are doing
 * Note: in this example ENCODER_EVENT_INTERRUPT usage cause nothing (no interrupt pins used)
 *
 * Author: Kudesunik
 * License: MIT
 *
 */

#include <Encoder.h>

#define ENCODER_PIN_1 5 //No interrupt pin on Arduino Uno or Mega (bad case)
#define ENCODER_PIN_2 6 //No interrupt pin on Arduino Uno or Mega (bad case)

void handleEncoder(int result); //Declare encoder handler function
Encoder encoder(ENCODER_PIN_1, ENCODER_PIN_2, handleEncoder); //Create encoder

//Encoder handler definition
void handleEncoder(int result) {
    if(result == ENCODER_CW) {
        Serial.println("Clockwise");
    } else if(result == ENCODER_CCW) {
        Serial.println("Counterclockwise");
    }
}

void setup() {
    Serial.begin(9600); //Output debug data
}

void loop() {
    encoder.update(); //Always place this in loop() function
}
