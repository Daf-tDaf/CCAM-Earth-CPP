#pragma once

#include <Synthesis/oscillator.h>
#include <Utility/dsp.h>
#include <array>

class SmoothOsc {
    std::array<daisysp::Oscillator, 3> oscs;
public:
    void Init(float sample_rate);
    void SetFreq(float freq);
    float Process();
    void SetWaveshape(float value);
};