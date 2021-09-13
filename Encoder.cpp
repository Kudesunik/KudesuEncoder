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

Encoder::Encoder(int8_t encoderPin1, int8_t encoderPin2, int8_t buttonPin, void (*encoderFunction)(int), void (*buttonFunction)(int)) {
    self = this;
    _encoderPin1 = encoderPin1;
    _encoderPin2 = encoderPin2;
    _buttonPin = buttonPin;
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
    int buttonInterruptPin = digitalPinToInterrupt(buttonPin);
    if(buttonInterruptPin != NOT_AN_INTERRUPT) {
        attachInterrupt(buttonInterruptPin, buttonInterrupt, CHANGE);
        _isButtonInterrupt = true;
    }
    _isInitialized = true;
}

Encoder::Encoder(int8_t encoderPin1, int8_t encoderPin2, void (*encoderFunction)(int))
        : Encoder(encoderPin1, encoderPin2, -1, encoderFunction, nullptr) {}

void Encoder::update() {
    if(!_isEncoderInterrupt) {
        updateEncoder();
    }
    if(!_isButtonInterrupt) {
        updateButton();
    }
}

void Encoder::updateEncoder() {
    onEncoderInterrupt();
}

void Encoder::updateButton() {
    onButtonInterrupt();
}

void Encoder::onEncoderInterrupt() {
    if (_isInitialized) {
        _encoderState = digitalRead(_encoderPin1);
        if (_encoderState != _encoderLastState) {
            if (_encoderState != digitalRead(_encoderPin2)) {
                _counter++;
            } else {
                _counter--;
            }
            if (abs(_counter) == 2) {
                if (_counter > 0) {
                    encoderFunction(ENCODER_CW);
                } else {
                    encoderFunction(ENCODER_CCW);
                }
                _counter = 0;
            }
        }
        _encoderLastState = _encoderState;
    } else {
        encoderFunction(ENCODER_FAULT);
    }
}

void Encoder::onButtonInterrupt() {
    if(_isInitialized) {
        if(_buttonState && (digitalRead(_buttonPin) == HIGH)) {
            _buttonState = false;
            buttonFunction(ENCODER_BUTTON_OFF);
        } else if(!_buttonState && digitalRead(_buttonPin) == LOW) {
            _buttonState = true;
            buttonFunction(ENCODER_BUTTON_ON);
        }
    }
}
