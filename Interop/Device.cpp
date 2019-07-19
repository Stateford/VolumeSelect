#include "Device.h"

#include <iostream>

namespace Interop
{
    Device::Device(System::String^ name, VolumeSelecter::AudioDevice* device)
    {
        this->name = name;
        this->device = device;
    }

    Device::~Device()
    {
        if (this->device != nullptr)
        {
            delete this->device;
            this->device = nullptr;
        }
    }

    System::String^ Device::getName()
    {
        return this->name;
    }

    List<Device^>^ Device::getDevices()
    {
        List<Device^>^ devices = gcnew List<Device^>();
        try {
            std::vector<VolumeSelecter::AudioDevice> unmanagedDevices = VolumeSelecter::getAudioDevices();
            for (auto& p : unmanagedDevices)
                devices->Add(gcnew Device(gcnew System::String(p.getName().c_str()), new VolumeSelecter::AudioDevice(p)));
        }
        catch (std::exception e) {
            std::cout << e.what() << std::endl;
        }

        return devices;
    }

    Device^ Device::getDefaultDevice()
    {
        auto device = VolumeSelecter::getDefaultAudioDevice();
        return gcnew Device(gcnew System::String(device.getName().c_str()), new VolumeSelecter::AudioDevice(device));
    }

    void Device::setVolume(System::Double volume)
    {
        device->setVolume(static_cast<float>(volume));
    }

    System::Double Device::getVolume()
    {
        return device->getVolume();
    }

    bool Device::operator==(const Device other)
    {
        return this->device == other.device;
    }
}