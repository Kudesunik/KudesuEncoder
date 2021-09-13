/**
 * Encoder example with no interruption pin connection;
 * Warning: only use this if there are no free interrupt pins on your device!
 * Warning: loop delay will lead to unstable operation of the encoder!
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
    Serial.begin(9600); //Output data to serial
}

void loop() {
    encoder.update();
}
