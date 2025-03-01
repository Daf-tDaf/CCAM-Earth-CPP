#include "noisedrum.h"

void NoiseDrum::Init(float sample_rate) {
    noise.Init();
    filter.Init(sample_rate);
    filter.SetFilterMode(daisysp::LadderFilter::FilterMode::BP12);
    env.Init(sample_rate);
}

void NoiseDrum::SetLength(float len){
    env.SetLength(len);
}

void NoiseDrum::SetFreq(float freq) {
    this->freq = freq;
}

void NoiseDrum::SetAmp(float amp) {
    this->amp = amp;
}

float NoiseDrum::Process(bool gate) {
    if (gate) {
        filter.SetFreq(freq);
    }

    curr_amp = env.Process(gate);
    return amp * filter.Process(noise.Process()) * curr_amp;
}

float NoiseDrum::GetCurrAmp() {
    return curr_amp;
}