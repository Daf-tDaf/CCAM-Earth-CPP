#include <array>


#include "daisysp.h"
#include "Utility/dsp.h"
#include "Effects/reverbsc.h"

#define PLATFORM_ESTUARY 1
#define PLATFORM_EARTH 2

#define __CCAM_TARGET_PLATFORM__ PLATFORM_EARTH

#if __CCAM_TARGET_PLATFORM__ == PLATFORM_ESTUARY
    #include <ccam/hw/estuary.h>
    ccam::hw::Estuary hw;
#elif __CCAM_TARGET_PLATFORM__ == PLATFORM_EARTH
    #include <ccam/hw/earth.h>
    ccam::hw::Earth hw;
#else
    #error Unknown target platform please define __CCAM_TARGET_PLATFORM__
#endif

daisysp::Oscillator vco;
daisysp::Oscillator lfo;
daisysp::ReverbSc verb;

static constexpr float MAX_U16_FLOAT = static_cast<float>(0xFFFF);

static void AudioCallback(daisy::AudioHandle::InputBuffer in,
            daisy::AudioHandle::OutputBuffer out, 
            size_t size) {
    hw.ProcessAllControls();
    hw.PostProcess();

    float time_cv = daisysp::fmap(hw.knobs[0]->Value(), 0.3f, 0.99f);
    float damp_cv = daisysp::fmap(hw.knobs[1]->Value(), 1000.f, 19000.f, daisysp::Mapping::LOG);

    verb.SetFeedback(time_cv);
    verb.SetLpFreq(damp_cv);

    for (size_t i = 0; i < size; i++)
    {
        verb.Process(IN_L[i], IN_R[i], &OUT_L[i], &OUT_R[i]);

        float voice = vco.Process() * (hw.knobs[2]->Value() - 0.05);
        OUT_L[i] += voice;
        OUT_R[i] += voice;
    }
}

static void CVOutCallback(uint16_t **out, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        float cvsum = 0.0f;
        for (auto* cvin : hw.cvins) { cvsum += cvin->Value(); }
        out[1][i] = static_cast<uint16_t>(cvsum * MAX_U16_FLOAT) >> 4;
        out[0][i] = static_cast<uint16_t>(lfo.Process()) >> 4;
    }
}

int main(void)
{
    hw.Init();
    hw.StartAudio(AudioCallback);
    hw.StartCV(CVOutCallback);

    vco.Init(hw.som.AudioSampleRate());
    vco.SetFreq(440.0f);

    lfo.Init(hw.CvOutSampleRate());    
    lfo.SetFreq(220.0f);
    lfo.SetAmp(MAX_U16_FLOAT);

    verb.Init(hw.som.AudioSampleRate());

    hw.som.StartLog(false);
    hw.som.PrintLine("Hello world");

    while(1) {
        for (unsigned i = 0; i < hw.knobs.size(); i++) {
            hw.leds[i].Set(hw.knobs[i]->Value());
        }

#if __CCAM_TARGET_PLATFORM__ == PLATFORM_EARTH
        // not enough knobs on earth
        hw.leds[6].Set(1.0f);
        hw.leds[7].Set(1.0f);

        for (unsigned i = 0; i < hw.leds.size(); i++) {
            if (hw.buttons[i].Pressed()) {
                hw.leds[i].Set(0.0f);
            }
        }
#endif

        daisy::System::Delay(1);
    }
}