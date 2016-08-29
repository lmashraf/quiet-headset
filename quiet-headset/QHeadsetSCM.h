#ifndef _QHeadsetSCM_H_
#define _QHeadsetSCM_H_

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>

#include "QHeadsetMMD.h"

//--------------------------------------------------------------------------------------------------

#define SERVICE_NAME _T("QuietHeadset Service")
#define SERVICE_ERROR _T("QuietHeadset Service: ServiceMain: SetService has returned an error")

//--------------------------------------------------------------------------------------------------

class QHeadsetSCM
{
public:
    ~QHeadsetSCM( );

    static QHeadsetSCM* getInstance( );
    HRESULT ServiceMMDNotification( );

private:
    QHeadsetSCM();

public:
    static VOID WINAPI ServiceMain( DWORD argc, LPTSTR* argv );
    VOID WINAPI ServiceCtrlHandler( DWORD ctrlCode );
    DWORD WINAPI ServiceWorkThread( LPVOID lpParam );

    void UpdateServiceStatus( DWORD sControlsAccepted,
                              DWORD sCurrentState,
                              DWORD sWin32ExitCode,
                              DWORD sCheckPoint );

private:
    static QHeadsetSCM* m_ServiceInstance;
    static QHeadsetMMD* m_MultimediaInstance;

    SERVICE_STATUS m_ServiceStatus;
    SERVICE_STATUS_HANDLE m_ServiceStatusHandle;
    HANDLE m_ServiceStopEvent;
};

//--------------------------------------------------------------------------------------------------

#endif _QHeadsetSCM_H_

//--------------------------------------------------------------------------------------------------