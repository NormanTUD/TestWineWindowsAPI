#include <windows.h>
#include <stdio.h>
#include <tchar.h>

void main(void) 
{
   HW_PROFILE_INFO   HwProfInfo;
   if (!GetCurrentHwProfile(&HwProfInfo)) 
   {
      _tprintf(TEXT("GetCurrentHwProfile failed with error %lx\n"), 
                 GetLastError());
      return;
   }
   _tprintf(TEXT("DockInfo = %d\n"), HwProfInfo.dwDockInfo);
   _tprintf(TEXT("Profile Guid = %s\n"), HwProfInfo.szHwProfileGuid);
   _tprintf(TEXT("Friendly Name = %s\n"), HwProfInfo.szHwProfileName);
}

