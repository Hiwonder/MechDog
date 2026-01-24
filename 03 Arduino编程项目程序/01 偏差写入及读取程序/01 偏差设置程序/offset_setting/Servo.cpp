#include "Servo.h"

int Servo::channel_next_free = 0;

Servo::Servo(){ _resetFields();}

Servo::~Servo(){ detach();}

bool Servo::attach(int pin, int channel, 
                   int minAngle, int maxAngle, 
                   int minPulseWidth, int maxPulseWidth) 
{
    if(channel == CHANNEL_NOT_ATTACHED) {
        if(channel_next_free == CHANNEL_MAX_NUM) {
            return false;
        }
        _channel = channel_next_free;
        channel_next_free++;
    } else {
        _channel = channel;
    }

    _pin = pin;
    _minAngle = minAngle;
    _maxAngle = maxAngle;
    _minPulseWidth = minPulseWidth;
    _maxPulseWidth = maxPulseWidth;

    ledcSetup(_channel, 50, 16); // channel X, 50 Hz, 16-bit depth
    ledcAttachPin(_pin, _channel);
    return true;
}

bool Servo::detach() {
    if (!this->attached()) {
        return false;
    }

    if(_channel == (channel_next_free - 1))
        channel_next_free--;

    ledcDetachPin(_pin);
    _pin = PIN_NOT_ATTACHED;
    return true;
}

void Servo::write(int degrees) {
    degrees = constrain(degrees, _minAngle, _maxAngle);
    writeMicroseconds(_angleToUs(degrees));
}

void Servo::writeMicroseconds(int pulseUs) {
    if (!attached()) {
        return;
    }
    pulseUs = constrain(pulseUs, _minPulseWidth, _maxPulseWidth);
    _pulseWidthDuty = _usToDuty(pulseUs);
    ledcWrite(_channel, _pulseWidthDuty);
}

int Servo::read() {
    return _usToAngle(readMicroseconds());
}

int Servo::readMicroseconds() {
    if (!this->attached()) {
        return 0;
    }
    int duty = ledcRead(_channel);
    return _dutyToUs(duty);
}

bool Servo::attached() const { return _pin != PIN_NOT_ATTACHED; }

int Servo::attachedPin() const { return _pin; }

void Servo::_resetFields(void) {
    _pin = PIN_NOT_ATTACHED;
    _pulseWidthDuty = 0;
    _channel = CHANNEL_NOT_ATTACHED;
    _minAngle = MIN_ANGLE;
    _maxAngle = MAX_ANGLE;
    _minPulseWidth = MIN_PULSE_WIDTH;
    _maxPulseWidth = MAX_PULSE_WIDTH;
}
