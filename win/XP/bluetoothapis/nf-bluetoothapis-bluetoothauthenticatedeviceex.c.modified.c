
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stdbool.h>

int main () {
PBLUETOOTH_DEVICE_INFO pDeviceInfo; 
HRESULT status;
HANDLE hEvent = NULL;

HRESULT WINAPI AuthenticateService(){

	status = BluetoothAuthenticateDeviceEx( NULL,
					        NULL,
					        pDeviceInfo,
					        NULL,
					        MITMProtectionNotRequired );

	if ( ERROR_INVALID_PARAMETER == status ) {
		// goto Cleanup;
		// ...
		// Take cleanup action here,
		// ...
	}
//
// Wait for the Authentication callback to return before trying to unregister the handle
// Use an infinite timeout since the handle to the function that sets the event is being
// deleted
//

	if ( WAIT_FAILED == WaitForSingleObject(hEvent, INFINITE) ) {
        	status = GetLastError();
        	// goto Cleanup;
		// ...
		// Take cleanup action here,
		// ...
	}

      return status;
}

	return 0;
}
