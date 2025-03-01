#include "tonedrum.h"

void ToneDrum::Init(float sample_rate) {
    voice.Init(sample_rate);
    fm_env.Init(sample_rate);
    amp_env.Init(sample_rate);
}

void ToneDrum::SetFreq(float freq) {
    this->freq_target = freq;
}

void ToneDrum::SetLength(float len) {
    amp_env.SetLength(len);
}

void ToneDrum::SetFmAmount(float fm) {
    fm = daisysp::fclamp(fm, 0.0f, 1.0f);
    this->fm = fm * 5000.0f;
}

void ToneDrum::SetFmLength(float len) {
    fm_env.SetLength(len);
}

void ToneDrum::SetAmp(float amp) {
    this->amp = amp;
}

float ToneDrum::Process(bool gate) {
    if (gate) {
        freq = freq_target;
    }
    voice.SetFreq(freq + (fm_env.Process(gate) * fm));
    curr_amp = amp_env.Process(gate);
    return amp * voice.Process() * curr_amp;
}

float ToneDrum::GetCurrAmp() {
    return curr_amp;
}