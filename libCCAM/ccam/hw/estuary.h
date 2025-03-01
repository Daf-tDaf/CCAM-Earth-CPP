#ifndef __CCAM_ESTUARY_H__
#define __CCAM_ESTUARY_H__

#include <array>

#include "daisy_patch_sm.h"

namespace ccam {

namespace hw {

struct Estuary {

    void Init() 
    {
        som.Init();

        std::array<daisy::Pin, 8> ledPins = {
            daisy::patch_sm::DaisyPatchSM::D2,
            daisy::patch_sm::DaisyPatchSM::D3,
            daisy::patch_sm::DaisyPatchSM::D4,
            daisy::patch_sm::DaisyPatchSM::D5,
            daisy::patch_sm::DaisyPatchSM::D7,
            daisy::patch_sm::DaisyPatchSM::D6,
            daisy::patch_sm::DaisyPatchSM::D1,
            daisy::patch_sm::DaisyPatchSM::D10
        };

        for (uint8_t i = 0; i < leds.size(); i++) {
            leds[i].Init(ledPins[i], false);
            leds[i].Set(0.0f);
        }

        uint8_t adc_idx = 0;
        for (uint8_t i = 0; i < cvins.size(); i++) {
            cvins[i] = &som.controls[adc_idx++];
        }

        for (uint8_t i = 0; i < knobs.size(); i++) {
            knobs[i] = &som.controls[adc_idx];
            if (i >= 4) {
                knobs[i]->Init(som.adc.GetPtr(adc_idx++), som.AudioCallbackRate());
            } else {
                knobs[i]->InitBipolarCv(som.adc.GetPtr(adc_idx++), som.AudioCallbackRate());
            }
        }

        switches[0].Init(
            daisy::patch_sm::DaisyPatchSM::B8,
            daisy::patch_sm::DaisyPatchSM::B7
        );

        switches[1].Init(
            daisy::patch_sm::DaisyPatchSM::A9,
            daisy::patch_sm::DaisyPatchSM::A8
        );
    }

    void PostProcess()
    {
        for (uint8_t i = 0; i < leds.size(); i++) {
            leds[i].Update();
        };
    }

    void ProcessAllControls() 
    {
        som.ProcessAllControls();
    }

    void SetAudioSampleRate(size_t sample_rate) 
    {
        som.SetAudioSampleRate(sample_rate);
    }

    inline void SetAudioBlockSize(size_t block_size) 
    {
        som.SetAudioBlockSize(block_size);
    }

    inline void StartAudio(daisy::AudioHandle::AudioCallback cb)
    {
        som.StartAudio(cb);
    }

    inline void StartCV(daisy::DacHandle::DacCallback cb)
    {
        som.StartDac(cb);
    }

    inline size_t CvOutSampleRate() {
        return som.dac.GetConfig().target_samplerate;
    }

    inline size_t CvOutCallbackRate() {
        // correspond to size of dsy_patch_sm_dac_buffer
        return som.dac.GetConfig().target_samplerate / 48;
    }

    daisy::patch_sm::DaisyPatchSM som;
    std::array<daisy::AnalogControl*, 8> knobs;
    std::array<daisy::AnalogControl*, 4> cvins;
    std::array<daisy::Led, 8> leds;
    std::array<daisy::Switch3, 2> switches;
};

} // namespace hw

} // namspace ccam

#endif // __CCAM_ESTUARY_H__