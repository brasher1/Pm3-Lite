/*FF*************************************************************************
*									    *
*   Copyright (c) 1994 BE&K, Inc.  These coded instructions,                *
*   statements, computer programs and source code contain unpublished	    *
*   trade secrets of BE&K, Inc. and are protected by Federal                *
*   Copyright Law.  They may not be disclosed or copied, duplicated,	    *
*   distributed, demonstrated or licensed in any form, in whole or in	    *
*   part, without the prior written consent of BE&K, Inc.  Any              *
*   permitted use shall be considered as a personal license to the	    *
*   authorized recipient only.  Any such permitted use is provided on a	    *
*   "as is" basis without any warranty of any kind, express or implied,	    *
*   and specifically without any warranty of merchantibility or fitness	    *
*   for a particular purpose.  In no event shall BE&K, Inc. be              *
*   liable for any special, incidental, direct or indirect damages of	    *
*   any kind, regardless of legal theory or negligence of		    *
*   BE&K, Inc.                                                              *
*									    *
****************************************************************************/

/*FF*************************************************************************
*									    *
*   FILE								    *
*	rvqLite.cpp						    *
*									    *
*   DESCRIPTION								    *
*									    *
*   NOTES								    *
*									    *
*   BUGS								    *
*									    *
*   REVISION HISTORY							    *
*	08/03/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
//#ifdef WIN32
#include "stdafx.h"
//#endif
#define VISTACALLS
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <stream.h>
#include "rvqLite1.hpp"

DB_INIT(rvqLiteDb);

/*FF*************************************************************************
*									    *
*   NAME								    *
*									    *
*   DESCRIPTION								    *
*									    *
*   INPUTS								    *
*									    *
*   OUTPUTS								    *
*									    *
*   NOTES								    *
*									    *
*   BUGS								    *
*									    *
*   REVISION HISTORY							    *
*	08/03/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "rvqLiteTask::rvqLiteTask()"
rvqLiteTask::rvqLiteTask() : StoreTask( "s" )
{
//    char hostName[64];
    char theUser[256];
	unsigned long size;
    		 
 //   gethostname( hostName, sizeof(hostName) );
 //   sprintf( theUser, "%s%s%d", hostName, getlogin(), getpid() );
 	size = sizeof(theUser);
  	WNetGetUser( NULL, theUser, &size);
//	AfxMessageBox(theUser);
	strcpy( theUser, "WOWPM3MGT");
    lockcomm( LMC_INTERNAL );
    dbuserid( theUser );
    thervqLiteDb = new rvqLiteDb;
}
#undef FUNC


rvqLiteTask::rvqLiteTask(char *theUser) : StoreTask( "s" )
{
//    char hostName[64];
//    char theUser[256];
//	unsigned long size;
    		 
 //   gethostname( hostName, sizeof(hostName) );
 //   sprintf( theUser, "%s%s%d", hostName, getlogin(), getpid() );
// 	size = sizeof(theUser);
//  	WNetGetUser( NULL, theUser, &size);
//	AfxMessageBox(theUser);
//	strcpy( theUser, "WOWPM3MGT");
    lockcomm( LMC_INTERNAL );
    dbuserid( theUser );
    thervqLiteDb = new rvqLiteDb;
}

/*FF*************************************************************************
*									    *
*   NAME								    *
*									    *
*   DESCRIPTION								    *
*									    *
*   INPUTS								    *
*									    *
*   OUTPUTS								    *
*									    *
*   NOTES								    *
*									    *
*   BUGS								    *
*									    *
*   REVISION HISTORY							    *
*	08/03/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "rvqLiteTask::~rvqLiteTask()"
rvqLiteTask::~rvqLiteTask()
{
    closeDatabase();
    delete thervqLiteDb;
}
#undef FUNC

/*FF*************************************************************************
*									    *
*   NAME								    *
*									    *
*   DESCRIPTION								    *
*									    *
*   INPUTS								    *
*									    *
*   OUTPUTS								    *
*									    *
*   NOTES								    *
*									    *
*   BUGS								    *
*									    *
*   REVISION HISTORY							    *
*	08/03/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "rvqLiteTask::openDatabase()"
T_F rvqLiteTask::openDatabase( Pchar name )
{
    thervqLiteDb->SetName( name );

{
	char buff[1024];
	GetCurrentDirectory(sizeof(buff), buff);
	printf( "rvqLiteTask::openDatabase() - currentDir = '%s'\n", buff );
}
	if( thervqLiteDb->Open() )
		return( True );
    else
    {
	//	cout << "Unable to open " << name << endl;
		TRACE("Unable to open %s\n" , name );
		return( False );
    }
}
#undef FUNC

/*FF*************************************************************************
*									    *
*   NAME								    *
*									    *
*   DESCRIPTION								    *
*									    *
*   INPUTS								    *
*									    *
*   OUTPUTS								    *
*									    *
*   NOTES								    *
*									    *
*   BUGS								    *
*									    *
*   REVISION HISTORY							    *
*	08/03/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "rvqLiteTask::closeDatabase()"
T_F rvqLiteTask::closeDatabase()
{
    if( !thervqLiteDb->Close() )
    {
	//	cout << "Unable to close database." << endl;
		TRACE("Unable to close database\n");
		return( False );
    }
	return True;
}
#undef FUNC

/*FF*************************************************************************
*									    *
*   NAME								    *
*									    *
*   DESCRIPTION								    *
*									    *
*   INPUTS								    *
*									    *
*   OUTPUTS								    *
*									    *
*   NOTES								    *
*									    *
*   BUGS								    *
*									    *
*   REVISION HISTORY							    *
*	08/03/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "rvqLiteTask::initializeDatabase()"
T_F rvqLiteTask::initializeDatabase( Pchar name )
{
    if( openDatabase( name ) )
    {
		thervqLiteDb->initialize();
	//	cout << name << " has been initialized." << endl;
		TRACE("%s has been initialized.\n", name);
		return( True );
    }
    else
    {
//		cout << name << " could not be initialized." << endl;
		TRACE("%s could not be initialized.\n", name);
		return( False );
    }
}
#undef FUNC
