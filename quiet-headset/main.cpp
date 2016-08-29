#include "QHeadsetSCM.h"

//--------------------------------------------------------------------------------------------------
/**
 * Entry point to the main program and Windows Service Control Manager (SCM)
 */

int
_tmain( int argc, char** argv )
{
    // Gets the instance of the SCM.
    QHeadsetSCM* myService = QHeadsetSCM::getInstance( );

    // Creates an entry table for the service which has two members:
    // serviceName and a pointer to a serviceMain function.
    SERVICE_TABLE_ENTRY ServiceTable[]
        = {// The name of the service to be run and its main function.
           {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)QHeadsetSCM::ServiceMain},
           {NULL, NULL}};

    // Connects the main thread of a service process to the SCM.
    if ( StartServiceCtrlDispatcher( ServiceTable ) == FALSE )
    {
        return GetLastError( );
    }

    return 0;
}

//--------------------------------------------------------------------------------------------------