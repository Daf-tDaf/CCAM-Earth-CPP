#include "delayenv.h"
#include "daisysp.h"

void DelayEnv::Init(float sample_rate) {
    delta = 1.0f / sample_rate;
}

void DelayEnv::SetLength(float length) {
    length = daisysp::fclamp(length, 0.0f, 1.0f);
    length = 1.0f - length;
    length = daisysp::fmap(length, 0.1f, 10.0f);
    this->length = length;
}

float DelayEnv::Process(bool gate) {
    if (!gate) {
        trig = false;
    }
    
    if (gate && !trig) {
        elapsed = 0.0f;
        trig = true;
    }

    amp = (amp * slew) + ((1.0f - slew) * daisysp::pow10f(-1.0f * length * elapsed));
    elapsed += delta;
    return amp;
}