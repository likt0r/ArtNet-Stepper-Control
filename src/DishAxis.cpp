#include <Arduino.h>
#
class dishAxis
{
private:
    int _steps = 100;    // you should increase this if you are using
                         // some of microstepping modes
    int _usDelay = 5000; // minimal is 950 for full step mode and NEMA15 motor
                         // minimal is 35 for sixteenth step mode
                         // >> use this to adjust speed of the motion
    int _stepResolution = 200;

public:
    uint8_t pullPin;
    uint8_t pinDir;
    uint8_t lightBarrierPin;

    dishAxis(int pin, int pinDirection, int stepResolution)
    { //, int pinBarrier

        _stepResolution = stepResolution;
        pullPin = pin;

        pinDir = pinDirection;
        // lightBarrierPin = pinBarrier;
        pinMode(pin, OUTPUT);
        pinMode(pinDir, OUTPUT);
        // pinMode(pinBarrier, INPUT);
        digitalWrite(pin, LOW);
        digitalWrite(pinDir, LOW);
    }

    void move(unsigned int motionDegree)
    {
        // set motor direction
        // int direction=-1;
        if (motionDegree >= 0)
        {
        }
        else
        {

            digitalWrite(pinDir, LOW); // motor direction ccw
        }
        // turn motor for no. of _steps
        // xx _steps equal 1 degree motion for ...
        // direction is set the same for both motors in code
        // direction is switched in the physical cabling

        // for(int y=0; y<=motionDegree; y++) {
        for (int x = 0; x < _steps; x++)
        {
            // switch on motor(s)
            digitalWrite(pullPin, HIGH);

            delayMicroseconds(_usDelay);
            // switch off motor(s)
            digitalWrite(pullPin, LOW);

            delayMicroseconds(_usDelay);
        }
        //}
    }

    // void ejectPart()
    // { // OBSOLETE FUNCTION
    //     while (digitalRead(lightBarrierPin) == HIGH)
    //     {

    //         // set motor direction
    //         int direction = -1;
    //         if (direction >= 0)
    //         {
    //             digitalWrite(pinDir, HIGH); // motor direction cw
    //         }
    //         else
    //         {
    //             digitalWrite(pinDir, LOW); // motor direction ccw
    //         }
    //         // turn motor for no. of _steps
    //         // direction is set the same for both motors in code
    //         // direction is switched in the physical cabling
    //         for (int x = 0; x < _steps; x++)
    //         {
    //             // switch on motor(s)
    //             digitalWrite(pullPin, HIGH);
    //             if (dualMotor)
    //             {
    //                 digitalWrite(pinLeftMotor, HIGH);
    //             }
    //             delayMicroseconds(_usDelay);
    //             // switch off motor(s)
    //             digitalWrite(pullPin, LOW);
    //             if (dualMotor)
    //             {
    //                 digitalWrite(pinLeftMotor, LOW);
    //             }
    //             delayMicroseconds(_usDelay);
    //         }
    //     }
    // }
};
