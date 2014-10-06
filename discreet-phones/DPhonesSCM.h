#ifndef _DPHONESSCM_H_
#define _DPHONESSCM_H_

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>

#define SERVICE_NAME  _T("Discreet Headphones Service")
#define SERVICE_ERROR _T("Discreet Headphones Service: ServiceMain: SetService has returned an error")

class DPhonesSCM
{
	private:
		static DPhonesSCM*		m_ServiceInstance;

		SERVICE_STATUS			m_ServiceStatus;		
		SERVICE_STATUS_HANDLE	m_ServiceStatusHandle;		
		HANDLE					m_ServiceStopEvent;		

	private:
		DPhonesSCM(void);

	public:
		~DPhonesSCM(void);

		static DPhonesSCM*		getInstance(void);

		static VOID WINAPI		ServiceMain(DWORD argc, LPTSTR *argv);
		VOID WINAPI				ServiceCtrlHandler(DWORD ctrlCode);
		DWORD WINAPI			ServiceWorkThread(LPVOID lpParam);
		void					UpdateServiceStatus(DWORD sControlsAccepted, DWORD sCurrentState, DWORD sWin32ExitCode, DWORD sCheckPoint);			
};	

#endif _DPHONESSCM_H_