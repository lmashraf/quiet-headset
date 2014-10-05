#include "DPhonesSCM.h"


DPhonesSCM::DPhonesSCM(void)
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

		// A handle to a service status
		m_ServiceStatusHandle	= NULL;	

		// Handle 
		m_ServiceStopEvent		= INVALID_HANDLE_VALUE;
}

//-------------------------------------------------
DPhonesSCM::~DPhonesSCM(void)
{}

//------------------------------------------------
DPhonesSCM* DPhonesSCM::m_ServiceInstance = nullptr;
DPhonesSCM*	DPhonesSCM::getInstance(void)
{
	if(m_ServiceInstance == nullptr)
		m_ServiceInstance = new DPhonesSCM();

		return m_ServiceInstance;
}
//------------------------------------------------
VOID WINAPI	DPhonesSCM::ServiceMain(DWORD argc, LPTSTR *argv)
{
	/* calls to member function should be done by pointer to member function! */

	DWORD Status = E_FAIL;

	// Registers the service control handler with the SCM.
	getInstance()->m_ServiceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, [](DWORD ctrlCode){ return getInstance()->ServiceCtrlHandler(ctrlCode); });

	if(getInstance()->m_ServiceStatusHandle == NULL)
		return;

	// Starts the service controller.
	if(SetServiceStatus(getInstance()->m_ServiceStatusHandle, &(getInstance()->m_ServiceStatus)) == FALSE)
	{
		OutputDebugString(_T("Discreet Headphones Service: ServiceMain: SetService has returned an error"));
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
		getInstance()->m_ServiceStatus.dwControlsAccepted = 0;
		getInstance()->m_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		getInstance()->m_ServiceStatus.dwWin32ExitCode = GetLastError();
		getInstance()->m_ServiceStatus.dwCheckPoint = 1;

		if(SetServiceStatus(getInstance()->m_ServiceStatusHandle, &(getInstance()->m_ServiceStatus)) == FALSE)
		{
			OutputDebugString(_T("Discreet Headphones Service: ServiceMain: SetService has returned an error"));
		}	

		return;
	}

	// Tell the service we are started.
	getInstance()->m_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	getInstance()->m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	getInstance()->m_ServiceStatus.dwWin32ExitCode = 0;
	getInstance()->m_ServiceStatus.dwCheckPoint = 0;	

	if(SetServiceStatus(getInstance()->m_ServiceStatusHandle, &(getInstance()->m_ServiceStatus)) == FALSE)
	{
		OutputDebugString(_T("Discreet Headphones Service: ServiceMain: SetService has returned an error"));
	}	

	// Start a thread that will perform the main task of the service.
	HANDLE hThread = CreateThread(NULL,
								  0,
								  [](LPVOID lpParam) { return getInstance()->ServiceWorkThread(lpParam); },
								  NULL,
								  0,
								  NULL);

	WaitForSingleObject(hThread, INFINITE);

	/**
	 * Perform any cleanup tasks
	 */

	CloseHandle(getInstance()->m_ServiceStopEvent);

	// Tell the service controller we are stopped
	getInstance()->m_ServiceStatus.dwControlsAccepted = 0;
	getInstance()->m_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	getInstance()->m_ServiceStatus.dwWin32ExitCode = 0;
	getInstance()->m_ServiceStatus.dwCheckPoint = 3;

	if(SetServiceStatus(getInstance()->m_ServiceStatusHandle, &(getInstance()->m_ServiceStatus)) == FALSE)
	{
		OutputDebugString(_T("Discreet Headphones Service: ServiceMain: SetService has returned an error"));
	}	

	return;
}
//------------------------------------------------
VOID WINAPI	DPhonesSCM::ServiceCtrlHandler(DWORD ctrlCode)
{
	switch(ctrlCode)
	{
		case SERVICE_CONTROL_STOP:
			{
				if(m_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
					break;
				/*
				 * Perform tasks necessary to stop the service
			     */
				m_ServiceStatus.dwControlsAccepted = 0;
				m_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
				m_ServiceStatus.dwWin32ExitCode = 0;
				m_ServiceStatus.dwCheckPoint = 4;
	
				if(SetServiceStatus(m_ServiceStatusHandle, &m_ServiceStatus) == FALSE)
				{
					OutputDebugString(_T("Discreet Headphones Service: ServiceMain: SetService has returned an error"));				
				}

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
DWORD WINAPI DPhonesSCM::ServiceWorkThread(LPVOID lpParam)
{
	while(WaitForSingleObject(m_ServiceStopEvent, 0) != WAIT_OBJECT_0)
	{
		/*
		 * Perform main service function here: do some work.
		 */
	}

	return ERROR_SUCCESS;
}
