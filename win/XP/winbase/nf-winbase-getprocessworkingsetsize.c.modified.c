#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    SIZE_T  dwMin, dwMax;
    HANDLE hProcess;

    if (argc != 2)
    {
        printf("This program requires a process ID as an argument.\n");
        return 1;
    }

    // Retrieve a handle to the process.

    hProcess = OpenProcess( PROCESS_QUERY_INFORMATION, 
                            FALSE, atoi(argv[1]));
     if (!hProcess)
    {
        printf( "OpenProcess failed (%d)\n", GetLastError() );
        return 1;
    }

    // Retrieve the working set size of the process.

    if (!GetProcessWorkingSetSize(hProcess, &dwMin, &dwMax))
    {
        printf("GetProcessWorkingSetSize failed (%d)\n",
            GetLastError());
        return 1;
    }

    printf("Process ID: %d\n", atoi(argv[1]));
    printf("Minimum working set: %lu KB\n", dwMin/1024);
    printf("Maximum working set: %lu KB\n", dwMax/1024);

    CloseHandle(hProcess);

    return 0;
}

