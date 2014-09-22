#include "DPhonesSCM.h"


DPhonesSCM::DPhonesSCM(void)
{	
		// Service Status
		m_ServiceStatus.dwServiceType				= 0;	// The type of service: file system driver, device driver, runs own process or share process.
		m_ServiceStatus.dwCurrentState				= 0;	// The current state of the service: pending, paused, running, stopping, not running ...
		m_ServiceStatus.dwControlsAccepted			= 0;	// The control codes the service accepts and processes in its handler function.
															// A UI process can control a service by s
		m_ServiceStatus.dwCheckPoint				= 0;
		m_ServiceStatus.dwServiceSpecificExitCode	= 0;
		m_ServiceStatus.dwWaitHint					= 0;
		m_ServiceStatus.dwWin32ExitCode				= 0;

		// Service Status Handle
		m_ServiceHandle		= nullptr;

		// Handle 
		m_ServiceStopEvent	= INVALID_HANDLE_VALUE;
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
	// Initialise service status
	DWORD Status = E_FAIL;
}
//------------------------------------------------
VOID WINAPI	DPhonesSCM::ServiceCtrlHandler(DWORD argc)
{}
//------------------------------------------------
DWORD WINAPI DPhonesSCM::ServiceWorkThread(LPVOID lpParam)
{
	return 0;
}
