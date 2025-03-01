#pragma once

#include <array>
#include <ccam/hw/estuary.h>

#include "lockedCtrl.h"

struct LockedEstaury {
    std::array<LockedAnalogControl, 8> knobs;

    void Init(ccam::hw::Estuary& hw, uint8_t switch_num, uint8_t mask) {
        for (uint8_t i = 0; i < hw.knobs.size(); i++) {
            knobs[i].Init(hw.knobs[i], &hw.switches[switch_num], mask);
        }
    }
    void Process() {
        for (auto& knob : knobs) {
            knob.Process();
        } 
    }
    float Value(uint8_t knob_idx) {
        return knobs[knob_idx].Value();
    }

    LockedAnalogControl& Get(uint8_t knob_idx) {
        return knobs[knob_idx];
    }
};