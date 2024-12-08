#include "earth.h"
#include "daisysp.h"
#include "usbh_midi.h"

extern "C"
{
    extern USBH_HandleTypeDef DMA_BUFFER_MEM_SECTION hUsbHostHS;
}

ccam::hw::Earth hw;

daisy::MidiUsbHandler midi;
daisy::USBHostHandle usbHost;
int speed;

void USBH_Connect(void* data)
{
    hw.som.PrintLine("device connected");
}

void USBH_Disconnect(void* data)
{
    hw.som.PrintLine("device connected");
}

void USBH_ClassActive(void* data)
{    hw.som.PrintLine("Noice!");

    if(usbHost.IsActiveClass(USBH_MIDI_CLASS))
    {
        daisy::MidiUsbHandler::Config midi_config;
        midi_config.transport_config.periph = daisy::MidiUsbTransport::Config::Periph::EXTERNAL;
        midi.Init(midi_config);
        midi.StartReceive();
    }
}

void USBH_Error(void* data)
{
    hw.som.PrintLine("device connected");
}




int main(void)
{
    hw.Init();

    hw.som.StartLog(true);

    hw.som.PrintLine("MIDI USB Host start");
    
    daisy::USBHostHandle::Config usbhConfig;
    usbhConfig.connect_callback = USBH_Connect,
    usbhConfig.disconnect_callback = USBH_Disconnect,
    usbhConfig.class_active_callback = USBH_ClassActive,
    usbhConfig.error_callback = USBH_Error,
    usbHost.Init(usbhConfig);

    daisy::USBHostHandle::Result result = usbHost.RegisterClass(USBH_MIDI_CLASS);

    hw.som.PrintLine("MIDI USB Host initialized %d", result);

    bool ledOn = false;
    while(1) {
        hw.ProcessAllControls();
        usbHost.Process();
        uint8_t bytes[3] = {0x90, 60, static_cast<uint8_t>(ledOn ? 127 : 0)};
        speed = daisysp::fmap(hw.knobs[0]->Value(), 0.0f, 100.0f);
        ledOn = !ledOn;

        if(usbHost.IsActiveClass(USBH_MIDI_CLASS))
        {
            hw.som.PrintLine("yippee");
            midi.SendMessage(bytes, 3);
        }

        //hw.leds[0].Set(usbHost.GetPresent() ? 1.0f : 0.0f);
        hw.leds[0].Set(hUsbHostHS.gState == HOST_IDLE ? 1.0f : 0.0f);
        hw.leds[1].Set(hUsbHostHS.gState == HOST_ABORT_STATE ? 1.0f : 0.0f);
        hw.leds[2].Set(hUsbHostHS.gState == HOST_DEV_DISCONNECTED ? 1.0f : 0.0f);

        hw.PostProcess();

    }
}