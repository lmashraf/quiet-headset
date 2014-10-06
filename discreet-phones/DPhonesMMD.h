/**
 * DPhonesMMD contains the necessary means to discover audio endpoints (Speakers, Microphones, Auxialiry Audio Input)
 * thanks to Windows Multimedia Device API and assess their capabilities
 * in order to silence the master volume in case a specific audio endpoint is unplugged.
 *
 * regarding 'IUnknown' and 'IMMNotificationClient' : 
 * see reference. http://msdn.microsoft.com/en-us/library/windows/desktop/dd370810(v=vs.85).aspx
 */

#ifndef _DPHONESMMD_H_
#define _DPHONESMMD_H_

#include "atlbase.h"
#include "mmdeviceapi.h"
#include "endpointvolume.h"

// Smartpointers from the Active Template Library to manage the Component Object Model interfaces (CComPtr) :
typedef ATL::CComPtr<IAudioEndpointVolume> IAUDIO_VOLUME;
typedef ATL::CComPtr<IMMDeviceEnumerator>  IAUDIO_DEVICE;


// IMMNotificationClient is an interface who provides notifications when an audio endpoint device is plugged/unplugged.
class DPhonesMMD : public IMMNotificationClient 
{
	private:
		LONG			m_Counter_Ref;			// reference count for interface on an object. necessary for AddRef/QueryInterface/Release from IUnknown

		IAUDIO_VOLUME	m_VolumeControl;		// an interface to volume controls of the audio stream to/from and audio endpoint device.
		IAUDIO_DEVICE	m_DeviceEnumerator;		// an interface that provides methods for enumerating audio endpoint device resources

	public:
		DPhonesMMD(IAudioEndpointVolume* pVolumeControl, IMMDeviceEnumerator* pDeviceEnumerator);
		~DPhonesMMD(void);

	public:
		// IUnknown methods: get pointers to interfaces on given object and manages them.
		ULONG	__stdcall AddRef(void);											
		ULONG	__stdcall Release(void);										
		HRESULT	__stdcall QueryInterface(REFIID riid, void **ppvObject);	

	public:
		// IMMNotification methods: notify changes on audio endpoint device.
		HRESULT __stdcall OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key);
		HRESULT __stdcall OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId);
		HRESULT __stdcall OnDeviceAdded(LPCWSTR pwstrDeviceId);
		HRESULT __stdcall OnDeviceRemoved(LPCWSTR pwstrDeviceId);
		HRESULT __stdcall OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState);
};

#endif

