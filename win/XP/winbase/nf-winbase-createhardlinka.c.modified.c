
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stdbool.h>

int main () {

const char *pszNewLinkName;
pszNewLinkName = "test.dat";
WCHAR *pszExistingFileName = L"file.dat";
  BOOL fCreatedLink = CreateHardLink( pszNewLinkName, 
                                      pszExistingFileName, 
                                      NULL ); // reserved, must be NULL

  if ( fCreatedLink == FALSE )
   {
    ;// handle error condition
   }


	return 0;
}
