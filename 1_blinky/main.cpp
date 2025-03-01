#include <ccam/hw/earth.h>
#include "daisysp.h"

ccam::hw::Earth hw;

int main(void)
{
    hw.Init();
    bool ledOn = false;
    while(1) {
        ledOn = !ledOn;
        hw.leds[0].Set(ledOn ? 0.0f : 1.0f);
        hw.PostProcess();
        daisy::System::Delay(1000);
    }
}