
#pragma once
#include <Arduino.h>

class Servo {
    static const int MIN_ANGLE = 0;
    static const int MAX_ANGLE = 180;
    static const int MIN_PULSE_WIDTH = 500;     // the shortest pulse sent to a servo
    static const int MAX_PULSE_WIDTH = 2500;     // the longest pulse sent to a servo
    static const int MAX_COMPARE = ((1 << 16) - 1); // 65535
    static const int TAU_MSEC = 20;
    static const int TAU_USEC = (TAU_MSEC * 1000);
    static const int CHANNEL_MAX_NUM = 16;

public:
    static const int CHANNEL_NOT_ATTACHED = -1;
    static const int PIN_NOT_ATTACHED = -1;
    
    Servo();
    ~Servo();

    bool attach(int pin, int channel = CHANNEL_NOT_ATTACHED, 
                int minAngle = MIN_ANGLE, int maxAngle = MAX_ANGLE, 
                int minPulseWidth = MIN_PULSE_WIDTH, int maxPulseWidth = MAX_PULSE_WIDTH);

    bool detach();
    void write(int degrees);
    void writeMicroseconds(int pulseUs);
    int read();
    int readMicroseconds();
    bool attached() const;
    int attachedPin() const;

private:
    void _resetFields(void);

    int _usToDuty(int us)    { return map(us, 0, TAU_USEC, 0, MAX_COMPARE); }
    int _dutyToUs(int duty)  { return map(duty, 0, MAX_COMPARE, 0, TAU_USEC); }
    int _usToAngle(int us)   { return map(us, _minPulseWidth, _maxPulseWidth, _minAngle, _maxAngle); }
    int _angleToUs(int angle){ return map(angle, _minAngle, _maxAngle, _minPulseWidth, _maxPulseWidth); }

    static int channel_next_free;

    int _pin;
    int _pulseWidthDuty;
    int _channel;
    int _min, _max;
    int _minPulseWidth, _maxPulseWidth;
    int _minAngle, _maxAngle;
};
