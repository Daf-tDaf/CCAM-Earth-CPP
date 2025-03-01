#pragma once
#include <src/hid/ctrl.h>
#include <array>

class LockedAnalogControl {
    float val;
    float last_val = 0.0f;
    uint8_t mask = 0;
    bool awake = true;
    bool selected = false;
    daisy::Switch3* sw;
    daisy::AnalogControl* ctrl;
public:
    float tolerance = 0.10f; 

    void Init(daisy::AnalogControl* ctrl, daisy::Switch3* sw, uint8_t mask) {
        this->ctrl = ctrl;
        this->sw = sw;
        this->mask = mask;
        val = ctrl->Value();
    }

    void Process() {
        if (((1 << sw->Read()) & mask) == 0) {
            selected = false;
            awake = false;
            return;
        }
        float curr_val = ctrl->Value();
        if (!selected) {
            last_val = curr_val;
            selected = true;
        }
        if (fabs(curr_val - last_val) > tolerance) {
            awake = true;
        }
        if (awake) {
            val = curr_val;
        }
    }

    float Value() {
        return val;
    }
};