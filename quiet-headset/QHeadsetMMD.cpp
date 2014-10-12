#include "QHeadsetMMD.h"


//-------------------------------------------------
QHeadsetMMD::QHeadsetMMD(IAudioEndpointVolume* pVolumeControl, IMMDeviceEnumerator* pDeviceEnumerator)
{
	//debug_mode
	m_DebugLog.open("qHeadset_Debug.txt");

	//mmdevapi
	m_VolumeControl		= pVolumeControl;
	m_DeviceEnumerator	= pDeviceEnumerator;
	m_CounterRef		= 1;
}

//-------------------------------------------------
QHeadsetMMD::~QHeadsetMMD(void)
{
	// we are using ATL::CComPtr smart pointers, no need to safe-release anything.
	m_DebugLog.close();
}

//-------------------------------------------------
void QHeadsetMMD::LogMessage(std::string sCallingMethod, std::string sMessage)
{
	#ifdef _WIN32
	MessageBox(NULL, sMessage.c_str(), "Quiet Headset: ERROR OCCURED", MB_OK | MB_ICONERROR );
	#endif

	m_DebugLog << sCallingMethod.c_str() << " : " << sMessage.c_str();
}

//--------------------------------------------------
BOOL QHeadsetMMD::SearchHeadset(LPCWSTR pwstrId)
{
	std::string sDeviceName, err;
	HRESULT hr  = S_OK;

	ATL::CComPtr<IPropertyStore> Properties;
	PROPVARIANT varProperties;

	PropVariantInit(&varProperties);

	// I picked the jump-on-fail for error handling 
	// see other ways to do it here: http://msdn.microsoft.com/en-us/library/windows/desktop/ff485842(v=vs.85).aspx
	hr = m_DeviceEnumerator->GetDevice(pwstrId, &m_Device);
	if(FAILED(hr))
	{
		err = "Failed to enumerate audio endpoint devices.";
		goto failure_exit;
	}

	hr = m_Device->OpenPropertyStore(STGM_READ, &Properties);
	if(FAILED(hr))
	{
		err = "Failed to read the audio endpoint device's properties.";
		goto failure_exit;
	}

	hr = Properties->GetValue(PKEY_Device_FriendlyName, &varProperties);
	if(FAILED(hr))
	{
		err = "Failed to retrieve the audio endpoint device's name.";
		goto failure_exit;
	}

	sDeviceName = CW2A(varProperties.pwszVal);	
	if (sDeviceName.find("Jack") != std::string::npos) 
	{
		PropVariantClear(&varProperties);
		return TRUE;
	}
	else
	{
		err = "The plugged/unplugged device is not a headset.";
		goto failure_exit;
	}

failure_exit:
		LogMessage("SearchHeadset", err);
		PropVariantClear(&varProperties);
		return FALSE;
}

/**********************************
 * IUnknown methods
 **********************************/

// increments reference count to an interface on an object.
ULONG __stdcall QHeadsetMMD::AddRef(void)
{
	// returns an increment to the value of the specified 32-bit variable as an atomic operation.
	return InterlockedIncrement(&m_CounterRef);
}

//-------------------------------------------------
// decrements reference count to an interface on an object.
ULONG __stdcall QHeadsetMMD::Release(void)
{
	// decrements the value of the specified 32-bit variable as an atomic operation.
	ULONG ulRef = InterlockedDecrement(&m_CounterRef);

	// when the counter reference reaches zero, the interface on the object is no longer maintained.
	if(ulRef == 0)
	{
		delete this;
	}

	return ulRef;
}

//-------------------------------------------------
// retrieves pointers to supported interfaces on an object.
HRESULT __stdcall QHeadsetMMD::QueryInterface(REFIID riid, void **ppvObject)
{
	if(riid == IID_IUnknown)
	{
		AddRef();
		*ppvObject = (IUnknown*) this; 
	}
	else if( riid == __uuidof(IMMNotificationClient))
	{
		AddRef();
		*ppvObject = (IMMNotificationClient*) this;
	}
	else
	{
		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}

	return S_OK;
}

/**********************************
 * IMMNotificationClient methods
 **********************************/

// notifies that the value of a property belonging to an audio endpoint device has changed.
HRESULT __stdcall QHeadsetMMD::OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key)
{
	BOOL	bMute;

	// Checks whether the speakers are muted.
	HRESULT hr = m_VolumeControl->GetMute(&bMute);

	if(SUCCEEDED(hr) && !bMute)
	{
		// In case the speakers are not muted, mute them!
		hr = m_VolumeControl->SetMute(TRUE, NULL);
	}
	else if(SUCCEEDED(hr) && bMute)
	{
		// In case the speaker is already muted, we unmute it since the headset has probably been plugged back in!
		hr = m_VolumeControl->SetMute(FALSE, NULL);
	}
	else
	{
		// Failed to check whether the speakers are muted!
		hr = S_OK;
		LogMessage("OnPropertyValueChanged", "Mute failed!");
	}

	return hr;
}

//-------------------------------------------------
// notifies that the default audio endpoint device for a particular role has changed.
HRESULT __stdcall QHeadsetMMD::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId)
{
	return S_OK;
}

//-------------------------------------------------
// notifies when a new audio endpoint device has been added.
HRESULT __stdcall QHeadsetMMD::OnDeviceAdded(LPCWSTR pwstrDeviceId)
{
	return S_OK;
}

//-------------------------------------------------
// notifies when an audio endpoint device has been removed.
HRESULT __stdcall QHeadsetMMD::OnDeviceRemoved(LPCWSTR pwstrDeviceId)
{
	return S_OK;
}

//-------------------------------------------------
// notifies that the state of an audio endpoint device has changed.
HRESULT __stdcall QHeadsetMMD::OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
{
	return S_OK;
}