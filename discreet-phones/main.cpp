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
	// Gets the instance of the Service Control Manager.
	DPhonesSCM* myService = DPhonesSCM::getInstance();

	// Creates an entry table for the service.
	SERVICE_TABLE_ENTRY ServiceTable[] = { {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) DPhonesSCM::ServiceMain }, {NULL, NULL}	};

	// Calling the Service's ENTRY POINT (ServiceMain)
	if(StartServiceCtrlDispatcher (ServiceTable) == FALSE)
	{
		return GetLastError();
	}
	
	return 0;
}