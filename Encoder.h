#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"

#define ENCODER_EVENT_UPDATE 1
#define ENCODER_EVENT_INTERRUPT 2

#define ENCODER_FAULT -1
#define ENCODER_CW 1
#define ENCODER_CCW 2
#define ENCODER_BUTTON_ON 3
#define ENCODER_BUTTON_OFF 4

class Encoder {
public:
    Encoder(int8_t encoderPin1, int8_t encoderPin2, int8_t buttonPin, void (*encoderFunction)(int), void (*buttonFunction)(int), uint8_t eventType);
    Encoder(int8_t encoderPin1, int8_t encoderPin2, int8_t buttonPin, void (*encoderFunction)(int), void (*buttonFunction)(int));
    Encoder(int8_t encoderPin1, int8_t encoderPin2, void (*encoderFunction)(int), uint8_t eventType);
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

    static const uint8_t ENCODER_ACTIONS_SIZE = 128;

    int8_t _encoderPin1 = -1;
    int8_t _encoderPin2 = -1;
    int8_t _buttonPin = -1;
    uint8_t _eventType = ENCODER_EVENT_UPDATE;
    bool _isEncoderInterrupt = false;
    bool _isButtonInterrupt = false;
    uint8_t _encoderState = 0;
    uint8_t _encoderLastState = 0;
    uint8_t _buttonState = 0;
    int8_t _counter = 0;
    bool _isEncoderInitialized = false;
    bool _isButtonInitialized = false;

    uint8_t* _actions = new uint8_t[ENCODER_ACTIONS_SIZE];
    uint8_t _actionsWriteIndex = 0;
    uint8_t _actionsReadIndex = 0;

    void executeAction(uint8_t action);
    void handleAction(uint8_t action);
    void executeEvents();

    void (*encoderFunction)(int) = nullptr;
    void (*buttonFunction)(int) = nullptr;
};

#endif
