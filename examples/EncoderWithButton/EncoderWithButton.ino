/**
 * Full encoder with button example
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
    Serial.begin(9600); //Output data to serial
}

void loop() {
    encoder.update(); //Update will be triggered if one of the encoder or button wire is not on the interrupt pin
    delay(100);
}
