#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
 
int main() 
{ 
   TCHAR   inBuf[80]; 
   HKEY   hKey1, hKey2; 
   DWORD  dwDisposition; 
   LONG   lRetCode; 
   TCHAR   szData[] = TEXT("USR:App Name\\Section1");
 
   // Create the .ini file key. 
   lRetCode = RegCreateKeyEx ( HKEY_LOCAL_MACHINE, 
       TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\IniFileMapping\\appname.ini"), 
       0, 
       NULL, 
       REG_OPTION_NON_VOLATILE, 
       KEY_WRITE, 
       NULL, 
       &hKey1, 
       &dwDisposition); 
 
   if (lRetCode != ERROR_SUCCESS)
   { 
      printf ("Error in creating appname.ini key (%d).\n", lRetCode); 
      return (0) ; 
   } 
 
   // Set a section value 
   lRetCode = RegSetValueEx ( hKey1, 
                              TEXT("Section1"), 
                              0, 
                              REG_SZ, 
                              (BYTE *)szData, 
                              sizeof(szData)); 
 
   if (lRetCode != ERROR_SUCCESS) 
   { 
      printf ("Error in setting Section1 value\n"); 
      // Close the key
      lRetCode = RegCloseKey( hKey1 );
      if( lRetCode != ERROR_SUCCESS )
      {
         printf("Error in RegCloseKey (%d).\n", lRetCode);
         return (0) ; 
      }
   } 
 
   // Create an App Name key 
   lRetCode = RegCreateKeyEx ( HKEY_CURRENT_USER, 
                               TEXT("App Name"), 
                               0, 
                               NULL, 
                               REG_OPTION_NON_VOLATILE,
                               KEY_WRITE, 
                               NULL, 
                               &hKey2, 
                               &dwDisposition); 
 
   if (lRetCode != ERROR_SUCCESS) 
   { 
      printf ("Error in creating App Name key (%d).\n", lRetCode); 

      // Close the key
      lRetCode = RegCloseKey( hKey2 );
      if( lRetCode != ERROR_SUCCESS )
      {
         printf("Error in RegCloseKey (%d).\n", lRetCode);
         return (0) ; 
      }
   } 
 
   // Force the system to read the mapping into shared memory 
   // so that future invocations of the application will see it 
   // without the user having to reboot the system 
   WritePrivateProfileStringW( NULL, NULL, NULL, L"appname.ini" ); 
 
   // Write some added values 
   WritePrivateProfileString (TEXT("Section1"), 
                              TEXT("FirstKey"), 
                              TEXT("It all worked out OK."), 
                              TEXT("appname.ini")); 
   WritePrivateProfileString (TEXT("Section1"), 
                              TEXT("SecondKey"), 
                              TEXT("By golly, it works!"), 
                              TEXT("appname.ini")); 
   WritePrivateProfileString (TEXT("Section1"), 
                              TEXT("ThirdKey"), 
                              TEXT("Another test..."), 
                              TEXT("appname.ini")); 

   // Test 
   GetPrivateProfileString (TEXT("Section1"), 
                            TEXT("FirstKey"), 
                            TEXT("Error: GPPS failed"), 
                            inBuf, 
                            80, 
                            TEXT("appname.ini")); 
   _tprintf (TEXT("Key: %s\n"), inBuf); 
 
   // Close the keys
   lRetCode = RegCloseKey( hKey1 );
   if( lRetCode != ERROR_SUCCESS )
   {
      printf("Error in RegCloseKey (%d).\n", lRetCode);
      return(0);
   }

   lRetCode = RegCloseKey( hKey2 );
   if( lRetCode != ERROR_SUCCESS )
   {
      printf("Error in RegCloseKey (%d).\n", lRetCode);
      return(0);
   }
   
   return(1); 
}

