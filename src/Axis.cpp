#include <Arduino.h>
#include "protothreads.h"
#include <pt.h>
class Axis
{
public:
    uint8_t pinPull;
    uint8_t pinDir;
    int maxSteps = 512;
    int stepTarget = 0;
    int stepCounter = 0;
    unsigned long sleepTime = 5;
    int maxSleepTime = 20;
    pt ptAxis;

    Axis(int pinPullUp, int pinDirection, int maxStps, int maxSlpTime)
    { //, int pinBarrier

        pinPull = pinPullUp;
        pinDir = pinDirection;
        maxSteps = maxStps;
        maxSleepTime = maxSlpTime;
        // lightBarrierPin = pinBarrier;
    }

    int axisThread(struct pt *pt)
    {
        PT_BEGIN(pt);
        // Loop forever
        for (;;)
        {
            int distanceSteps = stepTarget - stepCounter;
            if (distanceSteps > 0)
            {
                digitalWrite(pinDir, HIGH);  // motor direction cw
                digitalWrite(pinPull, HIGH); // turn the LED on (HIGH is the voltage level)
                stepCounter++;
            }
            else if (distanceSteps < 0)
            {
                digitalWrite(pinDir, LOW);   // motor direction cw
                digitalWrite(pinPull, HIGH); // turn the LED on (HIGH is the voltage level)
                stepCounter--;
            } // if distanceSteps == 0 do nothing

            PT_SLEEP(pt, sleepTime);
            digitalWrite(pinPull, LOW); // turn the LED off by making the voltage LOW
            PT_SLEEP(pt, sleepTime);
        }
        PT_END(pt);
    }

    void setTarget(const uint8_t data)
    {
        stepTarget = int(data) * 3;
    }

    void setSleepTimeFromDmx(const uint8_t data)
    {
        sleepTime = (int)((float)maxSleepTime - round((float)data / 255 * (float)maxSleepTime));
    }
    void setup()
    {
        pinMode(pinPull, OUTPUT);
        pinMode(pinDir, OUTPUT);
        // pinMode(pinBarrier, INPUT);
        digitalWrite(pinPull, LOW);
        digitalWrite(pinDir, LOW);
        PT_INIT(&ptAxis);
    }

    void loop()
    {
        PT_SCHEDULE(axisThread(&ptAxis));
        //}
    }
};
