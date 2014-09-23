#ifndef _DPHONESSCM_H_
#define _DPHONESSCM_H_

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>

#define SERVICE_NAME _T("Discreet Headphones Service")

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
		VOID WINAPI				ServiceCtrlHandler(DWORD argc);
		DWORD WINAPI			ServiceWorkThread(LPVOID lpParam);
};	

#endif _DPHONESSCM_H_