#include "AudioDevice.h"
#include <iostream>
#include <audiopolicy.h>


namespace VolumeSelecter
{
    AudioDevice::AudioDevice(const AudioDevice& other) noexcept
    {
        this->_name = other._name;
        this->_device = other._device;
        this->_rc++;
    }

    AudioDevice::AudioDevice(AudioDevice&& other) noexcept
    {
        this->_name = other._name;
        this->_device = other._device;
        other._device = nullptr;
    }

    AudioDevice& AudioDevice::operator=(const AudioDevice& other) noexcept
    {
        if (this == &other)
            return *this;
        this->_name = other._name;
        this->_device = other._device;
        this->_rc++;

        return *this;
    }

    AudioDevice& AudioDevice::operator=(AudioDevice&& other) noexcept
    {
        if (this == &other)
            return *this;

        this->_name = other._name;
        this->_device = other._device;
        other._device = nullptr;

        return *this;
    }

    const bool AudioDevice::operator==(const AudioDevice& other) const
    {
        return this->_device == other._device;
    }

    float AudioDevice::getVolume() const
    {
        HRESULT hr;

        IAudioEndpointVolume* endpointVolume = nullptr;

        hr = _device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)& endpointVolume);

        float currentVolume = 0.0f;
        endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
        endpointVolume->Release();
        endpointVolume = nullptr;

        return currentVolume;
    }

    const void AudioDevice::setVolume(int volume) const 
    {
        float f_volume = volume / 100.0f;
        this->setVolume(f_volume);
    }

    const void AudioDevice::setVolume(float volume) const 
    {
        HRESULT hr;

        IAudioEndpointVolume* endpointVolume = nullptr;

        hr = _device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)& endpointVolume);
        if (FAILED(hr))
        {
            if (endpointVolume != nullptr)
                endpointVolume->Release();

            throw "could not set volume";
        }

        endpointVolume->SetMasterVolumeLevelScalar(volume, NULL);
    }

    AudioDevices getAudioDevices()
    {
        HRESULT result = CoInitialize(NULL);

        if (result == S_OK)
            throw "error initializing";

        AudioDevices devices;

        const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
        const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
        IMMDeviceEnumerator* pEnumerator = nullptr;

        auto hr = CoCreateInstance(
            CLSID_MMDeviceEnumerator,
            NULL,
            CLSCTX_ALL,
            IID_IMMDeviceEnumerator,
            reinterpret_cast<LPVOID*>(& pEnumerator)
        );

        if (hr != S_OK)
            throw "Error";

        IMMDeviceCollection *pDeviceCollection = NULL;
        pEnumerator->EnumAudioEndpoints(eAll, DEVICE_STATE_ACTIVE, &pDeviceCollection);
        pEnumerator->Release();
        pEnumerator = nullptr;

        UINT deviceCount;
        pDeviceCollection->GetCount(&deviceCount);

        for (unsigned int i = 0; i < deviceCount; i++)
        {
            IMMDevice* pDevice = nullptr;
            IPropertyStore* pProperty = nullptr;
            IAudioEndpointVolume* endpointVolume = nullptr;
            IMMEndpoint* endpoint = nullptr;
            IAudioSessionManager* manager = nullptr;

            // get the item in the array
            pDeviceCollection->Item(i, &pDevice);
            // get properties
            pDevice->OpenPropertyStore(STGM_READ, &pProperty);
            pDevice->QueryInterface<IMMEndpoint>(&endpoint);

            // GET 2nd property key
            PROPERTYKEY key;
            pProperty->GetAt(2, &key);
            
            // get 2nd property name
            PROPVARIANT propVari;
            pProperty->GetValue(key, &propVari);
            // pointer to string at this location in the struct
            LPWSTR _name = propVari.pwszVal;
            std::wstring name(_name);
            
            pProperty->GetAt(1, &key);
            pProperty->GetValue(key, &propVari);

            if (propVari.boolVal == -1)
                continue;

            pProperty->Release();
            pProperty = nullptr;

            name += L'(' + std::wstring(propVari.pwszVal) + L')';

            devices.push_back(AudioDevice(name, pDevice, endpoint));
        }
        pDeviceCollection->Release();
        pDeviceCollection = nullptr;
        

        return devices;
    }

    AudioDevice getDefaultAudioDevice() 
    {
        HRESULT result = CoInitialize(NULL);

        if (FAILED(result))
            throw "error initializing";

        const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
        const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
        IMMDeviceEnumerator* pEnumerator = nullptr;

        auto hr = CoCreateInstance(
            CLSID_MMDeviceEnumerator,
            NULL,
            CLSCTX_ALL,
            IID_IMMDeviceEnumerator,
            reinterpret_cast<LPVOID*>(&pEnumerator)
        );

        if (hr != S_OK)
            throw "Error";

        IMMDevice* defaultDevice = nullptr;
        result = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
        pEnumerator->Release();
        pEnumerator = nullptr;

        IMMEndpoint* endpoint = nullptr;
        defaultDevice->QueryInterface<IMMEndpoint>(&endpoint);

        IPropertyStore* pProperty = nullptr;
        defaultDevice->OpenPropertyStore(STGM_READ, &pProperty);
        // GET 2nd property key
        PROPERTYKEY key;
        pProperty->GetAt(2, &key);

        // get 2nd property name
        PROPVARIANT propVari;
        pProperty->GetValue(key, &propVari);
        // pointer to string at this location in the struct
        LPWSTR _name = propVari.pwszVal;
        std::wstring name(_name);

        pProperty->GetAt(1, &key);
        pProperty->GetValue(key, &propVari);
        pProperty->Release();
        pProperty = nullptr;

        return AudioDevice(name, defaultDevice, endpoint);
    }
}