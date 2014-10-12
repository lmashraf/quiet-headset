#include "QHeadsetSCM.h"

QHeadsetMMD* QHeadsetSCM::m_MultimediaInstance	= nullptr;
QHeadsetSCM* QHeadsetSCM::m_ServiceInstance		= nullptr;
//----------------------------------------------------
QHeadsetSCM::QHeadsetSCM(void)
{
		// Structure containing data about a service's status
		ZeroMemory (&m_ServiceStatus, sizeof(m_ServiceStatus));
			
		// The type of service: file system driver, device driver, runs own process or share process.
		m_ServiceStatus.dwServiceType				= SERVICE_WIN32_OWN_PROCESS;	

		// The current state of the service: pending, paused, running, stopping, not running ...
		m_ServiceStatus.dwCurrentState				= SERVICE_START_PENDING;	

		// The control codes the service accepts and processes in its handler function.
		m_ServiceStatus.dwControlsAccepted			= 0;	

		// An error code the service uses to report an error that occures while it's starting or stopping
		m_ServiceStatus.dwWin32ExitCode				= 0;	

		// An error code service-specific that reports an error that occures while it's starting or stopping.
		m_ServiceStatus.dwServiceSpecificExitCode	= 0;	

		// Check point increments periodically during lengthy states.
		m_ServiceStatus.dwCheckPoint				= 0;	

		// The estimated time for a state in milliseconds.
		m_ServiceStatus.dwWaitHint					= 0;	

		// A handle to a service status.
		m_ServiceStatusHandle	= NULL;	

		// Another handle for when the service is stopped.
		m_ServiceStopEvent		= INVALID_HANDLE_VALUE;
}

//-------------------------------------------------
QHeadsetSCM::~QHeadsetSCM(void)
{
	delete m_MultimediaInstance;
}

//------------------------------------------------
QHeadsetSCM*	QHeadsetSCM::getInstance(void)
{
	if(m_ServiceInstance == nullptr)
		m_ServiceInstance = new QHeadsetSCM();

	return m_ServiceInstance;
}
//------------------------------------------------
// This is used to update the service status properties in a more readable manner.
void QHeadsetSCM::UpdateServiceStatus(DWORD pControlsAccepted, DWORD pCurrentState, DWORD pWin32ExitCode, DWORD pCheckPoint)
{
	m_ServiceStatus.dwControlsAccepted = pControlsAccepted;
	m_ServiceStatus.dwCurrentState = pCurrentState;
	m_ServiceStatus.dwWin32ExitCode = pWin32ExitCode;
	m_ServiceStatus.dwCheckPoint = pCheckPoint;
}	

//------------------------------------------------
/**
 * The ServiceMain is the entry point of our Windows Service.
 * It initializes anything we need, registers the service control handler
 * which will handle the service's different status (STOP, PAUSE, CONTINUE, ...) 
 * It also serves to set the service status and perform startup tasks
 * such as creating mutex, threads, IPCs and such.
 */
VOID WINAPI	QHeadsetSCM::ServiceMain(DWORD argc, LPTSTR *argv)
{
	DWORD Status = E_FAIL;

	// Registers the service control handler with the SCM.
	getInstance()->m_ServiceStatusHandle = RegisterServiceCtrlHandler
		(
			SERVICE_NAME,	// Service Name
			[] (DWORD ctrlCode)	{ return getInstance()->ServiceCtrlHandler(ctrlCode); } // Service Handler: Fixed C3867 with lambda
		);

	if(getInstance()->m_ServiceStatusHandle == NULL)
	{
		OutputDebugString(SERVICE_ERROR);
		return;
	}

	// Updates the SCM's status information for the calling service, in other words we start the service control here:
	if(SetServiceStatus(getInstance()->m_ServiceStatusHandle, &(getInstance()->m_ServiceStatus)) == FALSE)
	{
		OutputDebugString(SERVICE_ERROR);
	}


	/*
	 * TBD: Performs tasks necessary to start the service
	 */


	// Create a service stop event to wait on later
	getInstance()->m_ServiceStopEvent = CreateEvent(NULL,	// lpEventAttributes: The handle cannot be inherited by child processes.
									FALSE,	// bManualReset: Creates a manual-reset event object.  ResetEvent sets event state to nonsignaled.
									FALSE,	// bInitialState: The initial state of the event object is nonsignaled.
									NULL	// lpName: The name of the event object.
									);

	if(getInstance()->m_ServiceStopEvent == NULL)
	{
		// Error creating event
		// Tell the service controller we are stopped, then exit.
		getInstance()->UpdateServiceStatus(0, SERVICE_STOPPED, GetLastError(), 1);

		if(SetServiceStatus(getInstance()->m_ServiceStatusHandle, &(getInstance()->m_ServiceStatus)) == FALSE)
		{
			OutputDebugString(SERVICE_ERROR);
		}

		return;
	}

	// Tell the service we are started.
	getInstance()->UpdateServiceStatus(SERVICE_ACCEPT_STOP, SERVICE_RUNNING, 0, 0);

	if(SetServiceStatus(getInstance()->m_ServiceStatusHandle, &(getInstance()->m_ServiceStatus)) == FALSE)
	{
		OutputDebugString(SERVICE_ERROR);
	}

	// Start a thread that will perform the main task of the service.
	HANDLE hThread = CreateThread(NULL,	
								  0,	
								  [](LPVOID lpParam) { return getInstance()->ServiceWorkThread(lpParam); },	// function executed by the thread: Fixed C3867 with lambda
								  NULL,
								  0,
								  NULL);

	// Wait until the thread enters the signaled state. The time elapse is set to infinity.
	WaitForSingleObject(hThread, INFINITE);


	/**
	 * TBD: Perform any cleanup tasks
	 */


	CloseHandle(getInstance()->m_ServiceStopEvent);

	// Tell the service controller we are stopped
	getInstance()->UpdateServiceStatus(0, SERVICE_STOPPED, 0, 3);

	if(SetServiceStatus(getInstance()->m_ServiceStatusHandle, &(getInstance()->m_ServiceStatus)) == FALSE)
	{
		OutputDebugString(SERVICE_ERROR);
	}

	return;
}
//------------------------------------------------
VOID WINAPI	QHeadsetSCM::ServiceCtrlHandler(DWORD ctrlCode)
{
	switch(ctrlCode)
	{
		case SERVICE_CONTROL_STOP:
			{
				if(m_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
					break;


				/*
				 * TBD: Perform tasks necessary to stop the service
			     */


				getInstance()->UpdateServiceStatus(0, SERVICE_STOP_PENDING, 0, 4);
	
				if(SetServiceStatus(m_ServiceStatusHandle, &m_ServiceStatus) == FALSE)
					OutputDebugString(SERVICE_ERROR);				

				// Signals the worker thread to start shutting down.
				SetEvent(m_ServiceStopEvent);
			}
			break;

		case SERVICE_CONTROL_HARDWAREPROFILECHANGE:
			break;

		case SERVICE_CONTROL_SESSIONCHANGE:
			break;

		default:
			break;
	}
}

//------------------------------------------------
DWORD WINAPI QHeadsetSCM::ServiceWorkThread(LPVOID lpParam)
{
	this->ServiceMMDNotification();

	while(WaitForSingleObject(m_ServiceStopEvent, 0) != WAIT_OBJECT_0)
	{

		/*
		 * TBD: Perform main service function here: do some work.
		 */

	}

	return ERROR_SUCCESS;
}


//-------------------------------------------------
HRESULT QHeadsetSCM::ServiceMMDNotification(void)
{
	HRESULT hr = S_FALSE;

	cptrIMMDevice		d_Speaker;
	cptrIMMDeviceEnum	d_Enumerator;
	cptrIAudioVolume	d_VolumeControl;

	// Initialize the COM library on a thread.
	CoInitialize(NULL);

	// Create an instance of our Multimedia Device enumerator COM object.
	hr = d_Enumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator));
	if(SUCCEEDED(hr))
	{
		// We look first for the default audio endpoint (speakers) which will have their volume lowered/muted
		d_Enumerator->GetDefaultAudioEndpoint(eRender,		// dataflow for a rendering device.
											 eMultimedia,	// role of the audio endpoint device.
											 &d_Speaker);

		hr = d_Speaker->Activate(__uuidof(IAudioEndpointVolume),	// We create a COM object with the IAudioEndpointVolume interface
								 CLSCTX_ALL,					// Execution context
								 NULL,							// default: NULL for activating an IAudioEndpointVolume object
								 (void**) &d_VolumeControl);		// COM interface.

		if(SUCCEEDED(hr))
		{
			m_MultimediaInstance = new QHeadsetMMD(d_VolumeControl, d_Enumerator);

			hr = d_Enumerator->RegisterEndpointNotificationCallback( static_cast<IMMNotificationClient*> (m_MultimediaInstance) );
		}
	}

	return hr;
}