
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stdbool.h>

int main () {
// Television recording is beginning. Enable away mode and prevent
// the sleep idle time-out.
//
SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);

//
// Wait until recording is complete...
//

//
// Clear EXECUTION_STATE flags to disable away mode and allow the system to idle to sleep normally.
//
SetThreadExecutionState(ES_CONTINUOUS);


	return 0;
}
