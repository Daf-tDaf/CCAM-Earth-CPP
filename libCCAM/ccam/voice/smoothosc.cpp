#include "smoothosc.h"

void SmoothOsc::Init(float sample_rate) {
    for (daisysp::Oscillator& osc : oscs) {
        osc.Init(sample_rate);
    }
    oscs[0].SetWaveform(daisysp::Oscillator::WAVE_SIN);
    oscs[1].SetWaveform(daisysp::Oscillator::WAVE_TRI);
    oscs[2].SetWaveform(daisysp::Oscillator::WAVE_SQUARE);
}

void SmoothOsc::SetFreq(float freq) {
    for (daisysp::Oscillator& osc : oscs) {
        osc.SetFreq(freq);
    }
}

float SmoothOsc::Process() {
    float sum = 0.0f;
    for (daisysp::Oscillator& osc : oscs) {
        sum += osc.Process();
    }
    return sum;
}

void SmoothOsc::SetWaveshape(float value) {
    value = daisysp::fclamp(value, 0.0f, 1.0f);
    value *= oscs.size() - 1;

    for (daisysp::Oscillator& osc : oscs) {
        osc.SetAmp(0.0f);
    }

    float osc_idx = static_cast<uint8_t>(value);
    float amount = daisysp::fastmod1f(value);

    oscs[osc_idx].SetAmp(1.0f - amount);
    oscs[osc_idx + 1].SetAmp(amount);
}