#pragma once

#include "daisysp.h"

template <typename Ctrl>
class GateClock {
    daisysp::Metro metro;
    size_t ticks_since_high = -1;
    float sample_rate;
    bool changed = false;
    bool is_high = false;
    size_t timeout_ticks = 0;
    bool gate_active = false;

    daisy::GateIn* clk;
    Ctrl* knob;
public:
    float clk_freq = 0.0f;

    void Init(daisy::GateIn* clk, Ctrl* knob, float sample_rate) {
        this->clk = clk;
        this->knob = knob;
        this->sample_rate = sample_rate;
        metro.Init(1.0f, sample_rate);
        SetTimeout(1.0f);
    }

    void SetTimeout(float secs) {
        timeout_ticks = static_cast<size_t>(secs * sample_rate);
    }

    bool RisingEdge() {
        return changed && is_high;
    }

    bool FallingEdge() {
        return changed && !is_high;
    }

    void Process() {
        gate_active = true;
        if (clk->Trig()) {
            metro.Reset();
            clk_freq = sample_rate / static_cast<float>(ticks_since_high);
            ticks_since_high = 0;
        } else if (ticks_since_high < timeout_ticks) {
            ticks_since_high++;
        } else {
            gate_active = false;
        }

        if (false) { // TODO: if (gate_active)
            metro.SetFreq(daisysp::fmap(knob->Value(), 1.0f, 256.0f));
        } else {
            float multiplier = daisysp::fmap(knob->Value(), 0.0f, 5.0f);
            metro.SetFreq(clk_freq * multiplier * 2.0f);
        }

        if (metro.Process()) {
            changed = true;
            is_high = !is_high;
        } else {
            changed = false;
        }
    }
};