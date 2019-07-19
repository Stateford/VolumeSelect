#pragma once

#ifdef VOLUMESELECT_EXPORTS
#define VOLUMESELECT_API __declspec(dllexport)
#else
#define VOLUMESELECT_API __declspec(dllimport)
#endif

#include <Windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <string>
#include <vector>


namespace VolumeSelecter
{
    class AudioDevice
    {
    private:
        std::wstring _name;
        IMMDevice* _device;
        IMMEndpoint* _endpoint = nullptr;
        int _rc = 0;
        
    public:
        VOLUMESELECT_API explicit AudioDevice(std::wstring name, IMMDevice* device, IMMEndpoint* endpoint) noexcept :
            _name(name), 
            _device(device),
            _endpoint(endpoint) {};
        VOLUMESELECT_API AudioDevice(const AudioDevice&) noexcept;
        VOLUMESELECT_API AudioDevice(AudioDevice&&) noexcept;

        ~AudioDevice() noexcept {
            if (_device != nullptr && _rc <= 0)
            {
                _device->Release();
                _device = nullptr;
            }

            if (_endpoint != nullptr && _rc <= 0)
            {
                _endpoint->Release();
                _endpoint = nullptr;
            }
        }

        AudioDevice& operator=(const AudioDevice&) noexcept;
        AudioDevice& operator=(AudioDevice&&) noexcept;

        const bool operator==(const AudioDevice&) const;

        VOLUMESELECT_API std::wstring getName() const { return _name; }
        VOLUMESELECT_API const IMMDevice* getDevice() const { return _device; }
        VOLUMESELECT_API float getVolume() const;
        VOLUMESELECT_API const void setVolume(float) const;
        VOLUMESELECT_API const void setVolume(int) const;
    };

    typedef std::vector<AudioDevice> AudioDevices;

    VOLUMESELECT_API AudioDevices getAudioDevices();
    VOLUMESELECT_API AudioDevice getDefaultAudioDevice();
}