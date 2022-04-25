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
*	msysDatabase.c++						    *
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stream.h>
#include "msysDatabase.h"

DB_INIT(msysDb);

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
#define FUNC "msysTask::msysTask()"
msysTask::msysTask() : StoreTask( "s" )
{
    char hostName[64];
    char theUser[256];
    
    gethostname( hostName, sizeof(hostName) );
    sprintf( theUser, "%s%s%d", hostName, getlogin(), getpid() );
    
    lockcomm( LMC_TCP );
    dbuserid( theUser );
    theMSysDb = new msysDb;
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
#define FUNC "msysTask::~msysTask()"
msysTask::~msysTask()
{
    closeDatabase();
    delete theMSysDb;
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
#define FUNC "msysTask::openDatabase()"
T_F msysTask::openDatabase( Pchar name )
{
    theMSysDb->SetName( name );
    if( theMSysDb->Open() )
	return( True );
    else
    {
	cout << "Unable to open " << name << endl;
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
#define FUNC "msysTask::closeDatabase()"
T_F msysTask::closeDatabase()
{
    if( !theMSysDb->Close() )
    {
	cout << "Unable to close database." << endl;
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
#define FUNC "msysTask::initializeDatabase()"
T_F msysTask::initializeDatabase( Pchar name )
{
    if( openDatabase( name ) )
    {
	theMSysDb->initialize();
	cout << name << " has been initialized." << endl;
	return( True );
    }
    else
    {
	cout << name << " could not be initialized." << endl;
	return( False );
    }
}
#undef FUNC
