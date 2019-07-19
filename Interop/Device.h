#pragma once
#include <vector>
#include <vcclr.h>
#include "AudioDevice.h"
#include "AudioDevice.h"



namespace Interop
{
    using System::Collections::Generic::List;

    public ref class Device {
    private:
        
        VolumeSelecter::AudioDevice *device;
    public:
        Device(System::String^ name, VolumeSelecter::AudioDevice* device);
        ~Device();
        property System::String^ name;

        static List<Device^>^ getDevices();
        static Device^ getDefaultDevice();

        bool operator==(const Device);

        System::String^ getName();

        void setVolume(System::Double);
        System::Double getVolume();
    };
}