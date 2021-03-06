
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stdbool.h>

int main () {
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

void _cdecl _tmain()
{
    LPTSTR pszBuf=NULL;

    pszBuf = (LPTSTR)LocalAlloc(
              LPTR,
              MAX_PATH*sizeof(TCHAR));

    // Handle error condition
    if( pszBuf == NULL )
    {
       _tprintf(TEXT("LocalAlloc failed (%d)\n"), GetLastError());
       return 0;
    }

    //see how much memory was allocated
    _tprintf(TEXT("LocalAlloc allocated %d bytes\n"), LocalSize(pszBuf));

    // Use the memory allocated

    // Free the memory when finished with it
    LocalFree(pszBuf);
}


	return 0;
}
