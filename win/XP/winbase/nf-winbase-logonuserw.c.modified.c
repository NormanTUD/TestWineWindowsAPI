
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stdbool.h>

int main () {
LogonUser(L"LocalService", L"NT AUTHORITY", NULL, LOGON32_LOGON_SERVICE, LOGON32_PROVIDER_DEFAULT, &hToken)


	return 0;
}
