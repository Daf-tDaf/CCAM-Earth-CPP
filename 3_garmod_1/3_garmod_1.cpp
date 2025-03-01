#include "daisysp.h"

#include <ccam/hw/estuary.h>
#include <ccam/utils/gubbins.h>
#include <ccam/utils/quantizer.h>

ccam::hw::Estuary hw;

constexpr float MAX_MIDI_NOTE = 12.0f * 5.0f;
float raw_note = 0.0f;
float note = 0.0f;
float inval = 0.0f;
float outval = 0.0f;
float freq = 0.0f;
float voltage = 0.0f;

static void AudioCallback(daisy::AudioHandle::InputBuffer in,
            daisy::AudioHandle::OutputBuffer out, 
            size_t size) {
    hw.ProcessAllControls();

    //inval = hw.cvins[0]->Value();
    inval = hw.knobs[0]->Value();

    raw_note = daisysp::fmap(inval, 0.0f, MAX_MIDI_NOTE) + 33.0f;

    note = Quantizer::apply(
        Quantizer::Scale::ALL,
        raw_note
    );

    freq = daisysp::mtof(note);
    voltage = ftov(freq);

    hw.som.WriteCvOut(0, voltage);

    for (size_t i = 0; i < size; i++) {}

    hw.PostProcess();
}

int main(void)
{
    hw.Init();    
    hw.som.StartLog(false);
    hw.StartAudio(AudioCallback);

    while(1) {
        hw.som.PrintLine("input: %f output %f", freq, voltage);
        daisy::System::Delay(100);
    }
}