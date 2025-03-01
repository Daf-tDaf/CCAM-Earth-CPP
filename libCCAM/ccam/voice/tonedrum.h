#pragma once

#include "daisysp.h"
#include <ccam/utils/delayenv.h>

class ToneDrum {
    daisysp::Oscillator voice;
    DelayEnv fm_env;
    DelayEnv amp_env;
    float fm;
    float amp;
    float freq;
    float freq_target;
    float curr_amp;
public:
    void Init(float sample_rate);
    void SetFreq(float freq);
    void SetLength(float len);
    void SetFmAmount(float fm);
    void SetFmLength(float len);
    void SetAmp(float amp);
    float GetCurrAmp();
    float Process(bool gate);
};