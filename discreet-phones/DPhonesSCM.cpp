#include "DPhonesSCM.h"


DPhonesSCM::DPhonesSCM(void)
{	
		// Structure containing data about a service's status
		Zero_Memory (&m_ServiceStatus, sizeof(m_ServiceStatus);
			
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
		m_ServiceStatusHandle	= nullptr;	

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

	else return m_ServiceInstance;
}
//------------------------------------------------
VOID WINAPI	DPhonesSCM::ServiceMain(DWORD argc, LPTSTR *argv)
{
	DWORD Status = E_FAIL;

	// Registers the service control handler with the SCM.
	m_ServiceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);

	if(m_ServiceStatusHandler == (nullptr || NULL))
		return;

	// Starts the service controller.
	if(SetServiceStatus(m_StatusHandle, &m_ServiceStatus) == FALSE)
	{
		OutputDebugString(_T("Discreet Headphones Service: ServiceMain: SetService has returned an error"));
	}

	/*
	 * TBD: Performs tasks necessary to start the service
	 */

	// Create a service stop event to wait on later
	m_ServiceStopEvent = CreateEvent(NULL,	// lpEventAttributes: The handle cannot be inherited by child processes.
									TRUE,	// bManualReset: Creates a manual-reset event object.  ResetEvent sets event state to nonsignaled.
									FALSE,	// bInitialState: The initial state of the event object is nonsignaled.
									NULL	// lpName: The name of the event object.
									);


}
//------------------------------------------------
VOID WINAPI	DPhonesSCM::ServiceCtrlHandler(DWORD argc)
{}
//------------------------------------------------
DWORD WINAPI DPhonesSCM::ServiceWorkThread(LPVOID lpParam)
{
	return 0;
}
