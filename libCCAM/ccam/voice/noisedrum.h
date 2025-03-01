#pragma once

#include "daisysp.h"
#include <ccam/utils/delayenv.h>

class NoiseDrum {
    daisysp::WhiteNoise noise;
    daisysp::LadderFilter filter;
    DelayEnv env;
    float amp;
    float freq;
    float curr_amp;
public:
    void Init(float sample_rate);
    void SetLength(float len);
    void SetFreq(float freq);
    float GetCurrAmp();
    void SetAmp(float amp);
    float Process(bool gate);
};