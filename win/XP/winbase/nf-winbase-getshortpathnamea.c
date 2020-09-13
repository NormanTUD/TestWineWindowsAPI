//...
    long     length = 0;
    TCHAR*   buffer = NULL;

// First obtain the size needed by passing NULL and 0.

    length = GetShortPathName(lpszPath, NULL, 0);
    if (length == 0) ErrorExit(TEXT("GetShortPathName"));

// Dynamically allocate the correct size 
// (terminating null char was included in length)

    buffer = new TCHAR[length];

// Now simply call again using same long path.

    length = GetShortPathName(lpszPath, buffer, length);
    if (length == 0) ErrorExit(TEXT("GetShortPathName"));

    _tprintf(TEXT("long name = %s shortname = %s"), lpszPath, buffer);
    
    delete [] buffer;
///...

