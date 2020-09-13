
//  Copyright (C) Microsoft. All rights reserved.

#include <windows.h>
#include <stdio.h>
#include <aclapi.h>
#include <tchar.h>
#include <strsafe.h> // for proper buffer handling
#include <authz.h>

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "authz.lib")

LPTSTR lpServerName = NULL;


PSID ConvertNameToBinarySid(LPTSTR pAccountName)
{
   LPTSTR pDomainName = NULL;
   DWORD dwDomainNameSize = 0;
   PSID pSid = NULL;
   DWORD dwSidSize = 0;
   SID_NAME_USE sidType;
   BOOL fSuccess = FALSE;
   HRESULT hr = S_OK;

   __try
   {
      LookupAccountName(
            lpServerName,      // look up on local system
            pAccountName,
            pSid,              // buffer to receive name
            &dwSidSize,
            pDomainName,
            &dwDomainNameSize,
            &sidType);
      
      //  If the Name cannot be resolved, LookupAccountName will fail with
      //  ERROR_NONE_MAPPED.
      if (GetLastError() == ERROR_NONE_MAPPED)
      {
         wprintf_s(_T("LookupAccountName failed with %d\n"), GetLastError());
         __leave;
      }
      else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
      {
         pSid = (LPTSTR)LocalAlloc(LPTR, dwSidSize * sizeof(TCHAR));
         if (pSid == NULL)
         {
            wprintf_s(_T("LocalAlloc failed with %d\n"), GetLastError());
            __leave;
         }

         pDomainName = (LPTSTR)LocalAlloc(LPTR, dwDomainNameSize * sizeof(TCHAR));
         if (pDomainName == NULL)
         {
            wprintf_s(_T("LocalAlloc failed with %d\n"), GetLastError());
            __leave;
         }

         if (!LookupAccountName(
               lpServerName,      // look up on local system
               pAccountName,
               pSid,              // buffer to receive name
               &dwSidSize,
               pDomainName,
               &dwDomainNameSize,
               &sidType))
         {
            wprintf_s(_T("LookupAccountName failed with %d\n"), GetLastError());
            __leave;
         }
      }
      
      //  Any other error code
      else
      {
         wprintf_s(_T("LookupAccountName failed with %d\n"), GetLastError());
         __leave;
      }

      fSuccess = TRUE;
   }
   __finally
   {
      if(pDomainName != NULL)
      {
          LocalFree(pDomainName);
          pDomainName = NULL;
      }
      
      //  Free pSid only if failed;
      //  otherwise, the caller has to free it after use.
      if (fSuccess == FALSE)
      {
         if(pSid != NULL)
      {
          LocalFree(pSid);
          pSid = NULL;
      }
      }
   }

   return pSid;
}


void DisplayError(char* pszAPI, DWORD dwError)
{
   LPVOID lpvMessageBuffer;

   if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_FROM_SYSTEM,
              GetModuleHandle(L"Kernel32.dll"), dwError, 
              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // the user default language
              (LPTSTR)&lpvMessageBuffer, 0, NULL))
   {
      wprintf_s(L"FormatMessage failed with %d\n", GetLastError());
      ExitProcess(GetLastError());
   }

   //  ...now display this string.
   wprintf_s(L"ERROR: API        = %s.\n", (char *)pszAPI);
   wprintf_s(L"       error code = %08X.\n", dwError);
   wprintf_s(L"       message    = %s.\n", (char *)lpvMessageBuffer);

   //  Free the buffer allocated by the system.
   LocalFree(lpvMessageBuffer);

   ExitProcess(GetLastError());
}

void DisplayAccessMask(ACCESS_MASK Mask)
{
      // This evaluation of the ACCESS_MASK is an example. 
      // Applications should evaluate the ACCESS_MASK as necessary.

   wprintf_s(L"Effective Allowed Access Mask : %8X\n", Mask);
   if (((Mask & GENERIC_ALL) == GENERIC_ALL)
      || ((Mask & FILE_ALL_ACCESS) == FILE_ALL_ACCESS))
   {
         wprintf_s(L"Full Control\n");
         return;
   }
   if (((Mask & GENERIC_READ) == GENERIC_READ)
      || ((Mask & FILE_GENERIC_READ) == FILE_GENERIC_READ))
         wprintf_s(L"Read\n");
   if (((Mask & GENERIC_WRITE) == GENERIC_WRITE)
      || ((Mask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE))
         wprintf_s(L"Write\n");
   if (((Mask & GENERIC_EXECUTE) == GENERIC_EXECUTE)
      || ((Mask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE))
         wprintf_s(L"Execute\n");

}

void GetAccess(AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClient, PSECURITY_DESCRIPTOR psd)
{
   AUTHZ_ACCESS_REQUEST AccessRequest = {0};
   AUTHZ_ACCESS_REPLY AccessReply = {0};
   BYTE     Buffer[1024];
   BOOL bRes = FALSE;  // assume error

   //  Do AccessCheck.
   AccessRequest.DesiredAccess = MAXIMUM_ALLOWED;
   AccessRequest.PrincipalSelfSid = NULL;
   AccessRequest.ObjectTypeList = NULL;
   AccessRequest.ObjectTypeListLength = 0;
   AccessRequest.OptionalArguments = NULL; 

   RtlZeroMemory(Buffer, sizeof(Buffer));
   AccessReply.ResultListLength = 1;
   AccessReply.GrantedAccessMask = (PACCESS_MASK) (Buffer);
   AccessReply.Error = (PDWORD) (Buffer + sizeof(ACCESS_MASK));


   if (!AuthzAccessCheck( 0,
                          hAuthzClient,
                          &AccessRequest,
                          NULL,
                          psd,
                          NULL,
                          0,
                          &AccessReply,
                          NULL) ) {
      wprintf_s(_T("AuthzAccessCheck failed with %d\n"), GetLastError());
   }
   else 
      DisplayAccessMask(*(PACCESS_MASK)(AccessReply.GrantedAccessMask));

   return;
}

BOOL GetEffectiveRightsForUser(AUTHZ_RESOURCE_MANAGER_HANDLE hManager,
                               PSECURITY_DESCRIPTOR psd,
                                      LPTSTR lpszUserName)
{
   PSID pSid = NULL;
   BOOL bResult = FALSE;
   LUID unusedId = { 0 };
   AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClientContext = NULL;

   pSid = ConvertNameToBinarySid(lpszUserName);
   if (pSid != NULL)
   {
      bResult = AuthzInitializeContextFromSid(0,
         pSid,
         hManager,
         NULL,
         unusedId,
         NULL,
         &hAuthzClientContext);
      if (bResult)
      {
         GetAccess(hAuthzClientContext, psd);
         AuthzFreeContext(hAuthzClientContext);
      }
      else
         wprintf_s(_T("AuthzInitializeContextFromSid failed with %d\n"), GetLastError());
   }
    if(pSid != NULL)
      {
          LocalFree(pSid);
          pSid = NULL;
      }

   return bResult;
}

void UseAuthzSolution(PSECURITY_DESCRIPTOR psd, LPTSTR lpszUserName)
{
   AUTHZ_RESOURCE_MANAGER_HANDLE hManager;
   BOOL bResult = FALSE;

   bResult = AuthzInitializeResourceManager(AUTHZ_RM_FLAG_NO_AUDIT,
      NULL, NULL, NULL, NULL, &hManager);
   if (bResult)
   {
      bResult = GetEffectiveRightsForUser(hManager, psd, lpszUserName);
      AuthzFreeResourceManager(hManager);
   }
   else
      wprintf_s(_T("AuthzInitializeResourceManager failed with %d\n"), GetLastError());
}


void wmain(int argc, wchar_t *argv[])
{
   DWORD                dw;
   PACL                 pacl;
   PSECURITY_DESCRIPTOR psd;
   PSID                 psid = NULL; 

   if (argc != 3)
   {
      wprintf_s(L"Usage: FileOrFolderName UserOrGroupName\n");
      wprintf_s(L"Usage: FileOrFolderName UserOrGroupName\n");
      return;
   }
  

    dw = GetNamedSecurityInfo(argv[1], SE_FILE_OBJECT, DACL_SECURITY_INFORMATION | 
      OWNER_SECURITY_INFORMATION |
      GROUP_SECURITY_INFORMATION, NULL, NULL, &pacl, NULL, &psd);
   if (dw != ERROR_SUCCESS)
   {  printf("couldn't do getnamedsecinfo \n");
      DisplayError("GetNamedSecurityInfo", dw);
   }

   
   UseAuthzSolution(psd, argv[2]);


   if(psid != NULL)
      {
          LocalFree(psid);
          psid = NULL;
      };

   LocalFree(psd);
}

