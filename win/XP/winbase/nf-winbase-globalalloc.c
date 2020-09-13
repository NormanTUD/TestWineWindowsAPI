#include <windows.h>
#include <stdio.h>
#include <tchar.h>

void _cdecl main()
{
    PSECURITY_DESCRIPTOR pSD;

    pSD = (PSECURITY_DESCRIPTOR) GlobalAlloc(
           GMEM_FIXED,
           sizeof(PSECURITY_DESCRIPTOR));

    // Handle error condition
    if( pSD == NULL )
    {
       _tprintf(TEXT("GlobalAlloc failed (%d)\n"), GetLastError());
       return;
    }

    //see how much memory was allocated
    _tprintf(TEXT("GlobalAlloc allocated %d bytes\n"), GlobalSize(pSD));

    // Use the memory allocated

    // Free the memory when finished with it
    GlobalFree(pSD);
}

