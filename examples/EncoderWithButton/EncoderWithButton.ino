/**
 * Full encoder with button example;
 *
 * handleEncoder function receives encoder rotation events (ENCODER_CW and ENCODER_CCW values)
 * handleEncoderButton function receives encoder button event (ENCODER_BUTTON_ON and ENCODER_BUTTON_OFF values)
 *
 * Encoder constructor in this example initialized with default event type value (ENCODER_EVENT_UPDATE)
 * ENCODER_EVENT_UPDATE - all events called by encoder.update() from loop()
 * ENCODER_EVENT_INTERRUPT - all events called by interrupt function directly (if interrupt pins used)
 * Only use ENCODER_EVENT_INTERRUPT if sure what you are doing
 *
 * Author: Kudesunik
 * License: MIT
 *
 */

#include <Encoder.h>

#define ENCODER_PIN_1 2 //Interrupt pin on Arduino Uno or Mega (good case), for ESP8266 D0 - D16 support interrupts
#define ENCODER_PIN_2 3 //Interrupt pin on Arduino Uno or Mega (good case), for ESP8266 D0 - D16 support interrupts
#define BUTTON_PIN 4 //Not interrupt pin, need update to be called in loop for button handler, for ESP8266 D0 - D16 support interrupts

void handleEncoder(int result); //Declare encoder handler function
void handleEncoderButton(int result); //Declare encoder button handler function
Encoder encoder(ENCODER_PIN_1, ENCODER_PIN_2, BUTTON_PIN, handleEncoder, handleEncoderButton); //Create encoder

//Encoder handler definition
void handleEncoder(int result) {
    if(result == ENCODER_CW) {
        Serial.println("Clockwise");
    } else if(result == ENCODER_CCW) {
        Serial.println("Counterclockwise");
    }
}

//Encoder button handler definition
void handleEncoderButton(int result) {
    if(result == ENCODER_BUTTON_ON) {
        Serial.println("Button on");
    } else {
        Serial.println("Button off");
    }
}

void setup() {
    Serial.begin(9600); //Output debug data
}

void loop() {
    encoder.update(); //Always place this in loop() function
    delay(100);
}
