/**
 * QHeadsetMMD contains the necessary means to discover audio endpoints (Speakers, Microphones,
 * Auxialiry Audio Input)
 * thanks to Windows Multimedia Device API and assess their capabilities
 * in order to silence the master volume in case a specific audio endpoint is unplugged.
 *
 * regarding 'IUnknown' and 'IMMNotificationClient' :
 * see reference. http://msdn.microsoft.com/en-us/library/windows/desktop/dd370810(v=vs.85).aspx
 */

#ifndef _QHeadsetMMD_H_
#define _QHeadsetMMD_H_

#include "atlbase.h"

#include "endpointvolume.h"
#include "functiondiscoverykeys_devpkey.h"
#include "mmdeviceapi.h"

//--------------------------------------------------------------------------------------------------

// for debug and logging
#include <fstream>

//--------------------------------------------------------------------------------------------------
/** Smartpointers from the Active Template Library to manage the Component Object Model interfaces
 * (CComPtr) :
 */

typedef ATL::CComPtr< IMMDevice > cptrIMMDevice;
typedef ATL::CComPtr< IMMDeviceEnumerator > cptrIMMDeviceEnum;
typedef ATL::CComPtr< IAudioEndpointVolume > cptrIAudioVolume;

//--------------------------------------------------------------------------------------------------
/**
 * IMMNotificationClient is an interface who provides notifications when an audio endpoint device is
 * plugged/unplugged
 */

class QHeadsetMMD : public IMMNotificationClient
{
public:
    QHeadsetMMD( IAudioEndpointVolume* pVolumeControl, IMMDeviceEnumerator* pDeviceEnumerator );
    ~QHeadsetMMD( );

    void LogMessage( std::string sCallingMethod, std::string sMessage );
    BOOL SearchHeadset( LPCWSTR ptstrId );

public:
    // IUnknown methods: get pointers to interfaces on given object and manages them.
    ULONG __stdcall AddRef( );
    ULONG __stdcall Release( );

    HRESULT __stdcall QueryInterface( REFIID riid, void** ppvObject );

    // IMMNotificationClient methods: callback methods for audio endpoints device-events
    // notifications.
    HRESULT __stdcall OnPropertyValueChanged( LPCWSTR pwstrDeviceId, const PROPERTYKEY key );
    HRESULT __stdcall OnDefaultDeviceChanged( EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId );
    HRESULT __stdcall OnDeviceAdded( LPCWSTR pwstrDeviceId );
    HRESULT __stdcall OnDeviceRemoved( LPCWSTR pwstrDeviceId );
    HRESULT __stdcall OnDeviceStateChanged( LPCWSTR pwstrDeviceId, DWORD dwNewState );

private:
    // Debug logger for QHeadSet
    std::ofstream m_DebugLog;

    // Reference count for interface on an object, needed for AddRef, QueryInterface and Release
    // from IUnknown
    LONG m_CounterRef;

    // An interface to encapsulate generic multimedia features, in this case audio endpoint devices
    cptrIMMDevice m_Device;

    // An interface that provides methods for enumerating audio endpoint device resources
    cptrIMMDeviceEnum m_DeviceEnumerator;

    // An interface to volume controls of the audio stream to/from and audio endpoint device.
    cptrIAudioVolume m_VolumeControl;
};

//--------------------------------------------------------------------------------------------------

#endif

//--------------------------------------------------------------------------------------------------
