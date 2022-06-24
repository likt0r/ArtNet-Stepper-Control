#include <Arduino.h>
// Please include ArtnetEther.h to use Artnet on the platform
// which can use both WiFi and Ethernet
#include "Artnet.h"
#include <pt.h>
#include "protothreads.h"
// this is also valid for other platforms which can use only Ethernet
// #include <Artnet.h>

#define PT_SLEEP_MICROS(pt, delay)                                                                     \
    {                                                                                                  \
        do                                                                                             \
        {                                                                                              \
            static unsigned long protothreads_sleep = micros();                                        \
            PT_WAIT_UNTIL(pt, micros() - protothreads_sleep > delay || micros() < protothreads_sleep); \
        } while (false);                                                                               \
    }

// Ethernet stuff
const IPAddress ip(192, 168, 0, 201);
uint8_t mac[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB};

ArtnetReceiver artnet;
uint32_t universe0 = 0; // 0 - 15

uint8_t AXIS1_PUL_PIN = 40;
uint8_t AXIS1_DIR_PIN = 42;
uint8_t AXIS2_PIN = 44;
uint8_t AXIS2_DIR_PIN = 46;
int maxSteps = 512;
int stepTarget = 0;
int stepCounter = 0;
int direction = 0;
unsigned long sleepTime = 1;
int maxSleepTime = 20;
pt ptAxis1;
pt ptArtNet;
int axis1Thread(struct pt *pt)
{
    PT_BEGIN(pt);
    // Loop forever
    for (;;)
    {
        int distanceSteps = stepTarget - stepCounter;
        if (distanceSteps > 0)
        {
            digitalWrite(AXIS1_DIR_PIN, HIGH); // motor direction cw
            digitalWrite(AXIS1_PUL_PIN, HIGH); // turn the LED on (HIGH is the voltage level)
            stepCounter++;
        }
        else if (distanceSteps < 0)
        {
            digitalWrite(AXIS1_DIR_PIN, LOW);  // motor direction cw
            digitalWrite(AXIS1_PUL_PIN, HIGH); // turn the LED on (HIGH is the voltage level)
            stepCounter--;
        } // if distanceSteps == 0 do nothing

        PT_SLEEP(pt, sleepTime);
        digitalWrite(AXIS1_PUL_PIN, LOW); // turn the LED off by making the voltage LOW
        PT_SLEEP(pt, sleepTime);
    }
    PT_END(pt);
}

int artNetThread(struct pt *pt)
{
    PT_BEGIN(pt);

    // Loop forever
    for (;;)
    {
        // if distanceSteps == 0 do nothing
        artnet.parse(); // check if artnet packet has come and execute callback
        PT_SLEEP(pt, 20);
    }
    PT_END(pt);
}

void callback(const uint8_t *data, const uint16_t size)
{
    // Serial.println(" Recieved package");

    stepTarget = int(data[0]) * 3;
    // Serial.println(stepTarget);
    sleepTime = (int)((float)maxSleepTime - round((float)data[1] / 255 * (float)maxSleepTime));
    // for (int i = 0; i < size; i++)
    // {
    //     Serial.println(data[i]);
    // }

    // you can also use pre-defined callbacks
}

void setup()
{
    Serial.begin(115200);

    Ethernet.begin(mac, ip);
    artnet.begin();
    // artnet.subscribe_net(0);     // optionally you can change
    // artnet.subscribe_subnet(0);  // optionally you can change

    // you can also use pre-defined callbacks
    artnet.subscribe(universe0, callback);
    pinMode(AXIS1_PUL_PIN, OUTPUT);
    pinMode(AXIS1_DIR_PIN, OUTPUT);
    pinMode(AXIS2_PIN, OUTPUT);
    pinMode(AXIS2_DIR_PIN, OUTPUT);

    PT_INIT(&ptArtNet);
    PT_INIT(&ptAxis1);
}

void loop()
{
    PT_SCHEDULE(artNetThread(&ptArtNet));
    PT_SCHEDULE(axis1Thread(&ptAxis1));
}