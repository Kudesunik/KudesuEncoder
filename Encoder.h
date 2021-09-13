#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"

#define ENCODER_FAULT -1
#define ENCODER_CW 1
#define ENCODER_CCW 2

#define ENCODER_BUTTON_ON 1
#define ENCODER_BUTTON_OFF 0

class Encoder {
public:
    Encoder(int8_t encoderPin1, int8_t encoderPin2, int8_t buttonPin, void (*encoderFunction)(int), void (*buttonFunction)(int));
    Encoder(int8_t encoderPin1, int8_t encoderPin2, void (*encoderFunction)(int));
    void update();

    /**
     * Internal function, direct call prohibited
     */
    void onEncoderInterrupt();

    /**
     * Internal function, direct call prohibited
     */
     void onButtonInterrupt();

private:
    int8_t _encoderPin1 = -1;
    int8_t _encoderPin2 = -1;
    int8_t _buttonPin = -1;
    float _resolutionPeriod = -1;
    bool _isEncoderInterrupt = false;
    bool _isButtonInterrupt = false;
    uint8_t _encoderState = 0;
    uint8_t _encoderLastState = 0;
    uint8_t _buttonState = 0;
    int8_t _counter = 0;
    bool _isInitialized = false;

    void updateEncoder();
    void updateButton();

    void (*encoderFunction)(int) = nullptr;
    void (*buttonFunction)(int) = nullptr;
};

#endif
