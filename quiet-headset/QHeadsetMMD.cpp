#include "QHeadsetMMD.h"


//-------------------------------------------------
QHeadsetMMD::QHeadsetMMD(IAudioEndpointVolume* pVolumeControl, IMMDeviceEnumerator* pDeviceEnumerator)
{
	//debug_mode
	m_DebugMode			= true;
	
	if(m_DebugMode)
		m_DebugLog.open("qHeadset_Debug.txt");

	//mmdevapi
	m_VolumeControl		= pVolumeControl;
	m_DeviceEnumerator	= pDeviceEnumerator;
	m_CounterRef		= 1;
}
//-------------------------------------------------
QHeadsetMMD::~QHeadsetMMD(void)
{
	// ATL::CComPtr smart pointers are self-released

	if(m_DebugMode)
		m_DebugLog.close();
}
//-------------------------------------------------
void QHeadsetMMD::LogMessage(std::string sCallingMethod, std::string sMessage)
{
	if(m_DebugMode)
	{
		#ifdef _WIN32
			MessageBox(NULL, sMessage.c_str(), "Quiet Headset: ERROR OCCURED", MB_OK | MB_ICONERROR );
		#endif

		m_DebugLog << sCallingMethod.c_str() << " : " << sMessage.c_str();
	}
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
//-------------------------------------------------
// notifies that the state of an audio endpoint device has changed.
HRESULT __stdcall QHeadsetMMD::OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
{
	char* pszState;

	switch (dwNewState)
	{
		case DEVICE_STATE_ACTIVE:
			pszState = "ACTIVE";
			break;

		case DEVICE_STATE_DISABLED:
			pszState = "DISABLED";
			break;

		case DEVICE_STATE_NOTPRESENT:
			pszState = "NOTPRESENT";
			break;

		case DEVICE_STATE_UNPLUGGED:
			pszState = "UNPLUGGED";
			break;
	}

	LogMessage("OnDeviceStateChanged", pszState);

	return S_OK;
}

// notifies that the value of a property belonging to an audio endpoint device has changed.
HRESULT __stdcall QHeadsetMMD::OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key)
{
	return S_OK;
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
