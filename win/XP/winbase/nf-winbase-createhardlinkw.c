  BOOL fCreatedLink = CreateHardLink( pszNewLinkName, 
                                      pszExistingFileName, 
                                      NULL ); // reserved, must be NULL

  if ( fCreatedLink == FALSE )
   {
    ;// handle error condition
   }

