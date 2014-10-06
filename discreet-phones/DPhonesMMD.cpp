#include "DPhonesMMD.h"

//-------------------------------------------------
DPhonesMMD::DPhonesMMD(IAudioEndpointVolume* pVolumeControl, IMMDeviceEnumerator* pDeviceEnumerator)
{
	m_VolumeControl		= pVolumeControl;
	m_DeviceEnumerator	= pDeviceEnumerator;
	m_Counter_Ref		= 1;
}
//-------------------------------------------------
DPhonesMMD::~DPhonesMMD(void)
{
	// nothing to do here: self-release. (see. ATL::CComPtr)
}
//-------------------------------------------------

/**********************************
 * IUnknown methods
 **********************************/

// increments reference count to an interface on an object.
ULONG DPhonesMMD::AddRef(void)
{}

//-------------------------------------------------
// decrements reference count to an interface on an object.
ULONG DPhonesMMD::Release(void)
{}

//-------------------------------------------------
// retrieves pointers to supported interfaces on an object.
HRESULT DPhonesMMD::QueryInterface(REFIID riid, void **ppvObject)
{}

/**********************************
 * IMMNotificationClient methods
 **********************************/
	
// notifies that the value of a property belonging to an audio endpoint device has changed.
HRESULT STDMETHODCALLTYPE DPhonesMMD::OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key)
{}

//-------------------------------------------------
// notifies that the default audio endpoint device for a particular role has changed.
HRESULT STDMETHODCALLTYPE DPhonesMMD::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId)
{}

//-------------------------------------------------
// notifies when a new audio endpoint device has been added.
HRESULT STDMETHODCALLTYPE DPhonesMMD::OnDeviceAdded(LPCWSTR pwstrDeviceId)
{}

//-------------------------------------------------
// notifies when an audio endpoint device has been removed.
HRESULT STDMETHODCALLTYPE DPhonesMMD::OnDeviceRemoved(LPCWSTR pwstrDeviceId)
{}

//-------------------------------------------------
// notifies that the state of an audio endpoint device has changed.
HRESULT STDMETHODCALLTYPE DPhonesMMD::OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
{}

