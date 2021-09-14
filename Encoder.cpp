#include "Encoder.h"

#if defined(ESP8266) || defined(ESP32)
#define ISR_ATTR IRAM_ATTR
#else
#define ISR_ATTR
#endif

static Encoder *self;

void ISR_ATTR encoderInterrupt() {
    self->onEncoderInterrupt();
}

void ISR_ATTR buttonInterrupt() {
    self->onButtonInterrupt();
}

Encoder::Encoder(int8_t encoderPin1, int8_t encoderPin2, int8_t buttonPin, void (*encoderFunction)(int), void (*buttonFunction)(int), uint8_t eventType) {
    self = this;
    _encoderPin1 = encoderPin1;
    _encoderPin2 = encoderPin2;
    _buttonPin = buttonPin;
    _eventType = eventType;
    this->encoderFunction = encoderFunction;
    this->buttonFunction = buttonFunction;
    pinMode(encoderPin1, INPUT);
    pinMode(encoderPin2, INPUT);
    digitalWrite(encoderPin1, HIGH);
    digitalWrite(encoderPin2, HIGH);
    if(buttonPin != (-1)) {
        pinMode(buttonPin, INPUT);
        digitalWrite(buttonPin, HIGH);
    }
    int encoderInterruptPin1 = digitalPinToInterrupt(encoderPin1);
    int encoderInterruptPin2 = digitalPinToInterrupt(encoderPin2);
    if ((encoderInterruptPin1 != NOT_AN_INTERRUPT) || (encoderInterruptPin2 != NOT_AN_INTERRUPT)) {
        if(encoderInterruptPin1 != NOT_AN_INTERRUPT) {
            attachInterrupt(encoderInterruptPin1, encoderInterrupt, CHANGE);
        }
        if(encoderInterruptPin2 != NOT_AN_INTERRUPT) {
            attachInterrupt(encoderInterruptPin2, encoderInterrupt, CHANGE);
        }
        _isEncoderInterrupt = true;
    }
    _isEncoderInitialized = true;
    if(buttonPin != (-1)) {
        int buttonInterruptPin = digitalPinToInterrupt(buttonPin);
        if(buttonInterruptPin != NOT_AN_INTERRUPT) {
            attachInterrupt(buttonInterruptPin, buttonInterrupt, CHANGE);
            _isButtonInterrupt = true;
        }
        _isButtonInitialized = true;
    }
}

Encoder::Encoder(int8_t encoderPin1, int8_t encoderPin2, int8_t buttonPin, void (*encoderFunction)(int), void (*buttonFunction)(int))
        : Encoder(encoderPin1, encoderPin2, buttonPin, encoderFunction, buttonFunction, ENCODER_EVENT_UPDATE) {}

Encoder::Encoder(int8_t encoderPin1, int8_t encoderPin2, void (*encoderFunction)(int), uint8_t eventType)
        : Encoder(encoderPin1, encoderPin2, -1, encoderFunction, nullptr, eventType) {}

Encoder::Encoder(int8_t encoderPin1, int8_t encoderPin2, void (*encoderFunction)(int))
        : Encoder(encoderPin1, encoderPin2, -1, encoderFunction, nullptr) {}

void Encoder::update() {
    if(!_isEncoderInterrupt) {
        onEncoderInterrupt();
    }
    if(!_isButtonInterrupt) {
        onButtonInterrupt();
    }
    executeEvents();
}

void Encoder::handleAction(uint8_t action) {
    if(_eventType == ENCODER_EVENT_UPDATE) {
        _actions[_actionsWriteIndex] = action;
        _actionsWriteIndex++;
        if(_actionsWriteIndex == ENCODER_ACTIONS_SIZE) {
            _actionsWriteIndex = 0;
        }
    } else if (_eventType == ENCODER_EVENT_INTERRUPT) {
        executeAction(action);
    }
}

void Encoder::executeEvents() {
    while(_actionsReadIndex != _actionsWriteIndex) {
        executeAction(_actions[_actionsReadIndex]);
        _actionsReadIndex++;
        if(_actionsReadIndex == ENCODER_ACTIONS_SIZE) {
            _actionsReadIndex = 0;
        }
    }
}

void Encoder::executeAction(uint8_t action) {
    if(action < 3) {
        encoderFunction(action);
    } else {
        buttonFunction(action);
    }
}

void Encoder::onEncoderInterrupt() {
    if (_isEncoderInitialized) {
        _encoderState = digitalRead(_encoderPin1);
        if (_encoderState != _encoderLastState) {
            if (_encoderState != digitalRead(_encoderPin2)) {
                _counter++;
            } else {
                _counter--;
            }
            if (abs(_counter) == 2) {
                if (_counter > 0) {
                    handleAction(ENCODER_CW);
                } else {
                    handleAction(ENCODER_CCW);
                }
                _counter = 0;
            }
        }
        _encoderLastState = _encoderState;
    } else {
        handleAction(ENCODER_FAULT);
    }
}

void Encoder::onButtonInterrupt() {
    if(_isButtonInitialized) {
        if(_buttonState && (digitalRead(_buttonPin) == HIGH)) {
            _buttonState = false;
            handleAction(ENCODER_BUTTON_OFF);
        } else if(!_buttonState && digitalRead(_buttonPin) == LOW) {
            _buttonState = true;
            handleAction(ENCODER_BUTTON_ON);
        }
    }
}
