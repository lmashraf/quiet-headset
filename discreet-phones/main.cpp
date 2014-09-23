/**
 * Entry point to the main program and Windows Service Control Manager (SCM)
 *
 *
 *
 *
 */

#include "DPhonesSCM.h"

int _tmain(int argc, char** argv)
{
	// Gets the instance of the SCM.
	DPhonesSCM* myService = DPhonesSCM::getInstance();

	// Creates an entry table for the service which has two members:
	// serviceName and a pointer to a serviceMain function.
	SERVICE_TABLE_ENTRY ServiceTable[] = { {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) DPhonesSCM::ServiceMain }, {NULL, NULL}	};

	// Connects the main thread of a service process to the SCM.
	if(StartServiceCtrlDispatcher (ServiceTable) == FALSE)
	{
		return GetLastError();
	}
	
	return 0;
}