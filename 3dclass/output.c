/*COPYRIGHT******************************************************************
*									    *
*   Copyright (c) 1994 AsBuilt Data, Inc.  These coded instructions,	    *
*   statements, computer programs and source code contain unpublished	    *
*   trade secrets of AsBuilt Data, Inc. and are protected by Federal	    *
*   Copyright Law.  They may not be disclosed or copied, duplicated,	    *
*   distributed, demonstrated or licensed in any form, in whole or in	    *
*   part, without the prior written consent of AsBuilt Data, Inc.  Any	    *
*   permitted use shall be considered as a personal license to the	    *
*   authorized recipient only.  Any such permitted use is provided on a	    *
*   "as is" basis without any warranty of any kind, express or implied,	    *
*   and specifically without any warranty of merchantibility or fitness	    *
*   for a particular purpose.  In no event shall AsBuilt Data, Inc. or	    *
*   its affiliates be liable for any special, incidental, direct or	    *
*   indirect damages of any kind, or for any liabilities in excess of the   *
*   royalty fee payments actually made by Licensee hereunder regardless of  *
*   legal theory or negligence of AsBuilt Data, Inc.			    *
*									    *
******************************************************************COPYRIGHT*/

/*FF*************************************************************************
*									    *
*   FILE								    *
*									    *
*   DESCRIPTION								    *
*									    *
*   NOTES								    *
*									    *
*   BUGS								    *
*									    *
*   REVISION HISTORY							    *
*	12/01/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/param.h>
#include <limits.h>
#include <sys/stat.h>

#include "output.h"

static char szLogFilename[PATH_MAX] = "./default.log";

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
*	01/11/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
void setLogFileLocation( const char *pszPath, const char *pszFilename )
{
    if( strlen(pszPath) > 0 )
	strcpy( szLogFilename, pszPath );
    else
	strcpy( szLogFilename, "." );

    if( szLogFilename[strlen(szLogFilename)-1] != '/' )
	strcat( szLogFilename, "/" );
	
    if( strlen(pszFilename) > 0 )
	strcat( szLogFilename, pszFilename );
    else
	strcat( szLogFilename, "default.log" );
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
*	01/11/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
int vOutput( int olevel, const char *format, va_list vargs )
{
    int retVal = -1;
    
    if( olevel <= outputLevel )
	retVal = vprintf( format, vargs );
    
    return( retVal );
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
*	01/11/95 - jwb - Version 1.1.0					    *
*			    Separated function to allow one version to be   *
*			    called with varargs and the other with va_list. *
*	12/01/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
int Output( int olevel, const char *format, ... )
{
    va_list vargs;
    int retVal;

    va_start( vargs, format );
    retVal = vOutput( olevel, format, vargs );
    va_end( vargs );
    
    return( retVal );
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
*	12/01/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
int vDebugOutput( int olevel, const char *format, va_list vargs )
{
    int retVal = -1;
    
    if( olevel <= debugOutputLevel )
	retVal = vprintf( format, vargs );
    
    return( retVal );
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
*	12/01/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
int DebugOutput( int olevel, const char *format, ... )
{
    va_list vargs;
    int retVal;

    va_start( vargs, format );
    retVal = vDebugOutput( olevel, format, vargs );
    va_end( vargs );
    
    return( retVal );
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
*	12/01/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
int vErrorOutput( const char *format, va_list vargs )
{
    vWriteLogEntry( format, vargs );
    return( vfprintf( stderr, format, vargs ) );
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
*	12/01/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
int ErrorOutput( const char *format, ... )
{
    va_list vargs;
    int retVal;

    va_start( vargs, format );
    retVal = vErrorOutput( format, vargs );
    va_end( vargs );
    
    return( retVal );
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
*	01/10/95 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
int vWriteLogEntry( const char *format, va_list vargs )
{
    FILE *fpLogFile;
    char szTempStr[256];
    char szHostname[MAXHOSTNAMELEN];
    char *pszUsername;
    char *pszTime;
    pid_t pidCurrProcess;
    time_t t;
    mode_t oldUmask;
    int retVal = -1;

    oldUmask = umask( 000 );
    fpLogFile = fopen( szLogFilename, "a" );
    if( fpLogFile == NULL )
    {
	sprintf( szTempStr, "Cannot write to log file '%s'", szLogFilename );
	perror( szTempStr );
	umask( oldUmask );
	return( -1 );
    }
    
    setvbuf( fpLogFile, NULL, _IOLBF, 0 );

    /* Find out what time it is */
    t = time( NULL );
    pszTime = ctime( &t );
    pszTime[strlen(pszTime)-1] = '\0';

    /* Find out what machine we're on */
    if( gethostname( szHostname, sizeof(szHostname) ) != 0 )
	strcpy( szHostname, "Unknown host" );

    /* Find out who we are */
    if( (pszUsername = getlogin()) == NULL )
	pszUsername = "Unknown user";

    /* Find out what process we are */
    pidCurrProcess = getpid();

    fprintf( fpLogFile, "%s:%s:%s:%ld:", pszTime, szHostname, pszUsername,
	    pidCurrProcess );

    retVal = vfprintf( fpLogFile, format, vargs );

    if( format[strlen(format)-1] != '\n' )
	fprintf( fpLogFile, "\n" );
    
    fclose( fpLogFile );

    umask( oldUmask );

    return( retVal );
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
*	01/10/95 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
int WriteLogEntry( const char *format, ... )
{
    va_list vargs;
    int retVal;

    va_start( vargs, format );
    retVal = vWriteLogEntry( format, vargs );
    va_end( vargs );
    
    return( retVal );
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
*	10/19/95 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
int vLogOutput( int olevel, const char *format, va_list vargs )
{
    int retVal = -1;
    
    vWriteLogEntry( format, vargs );
    if( olevel <= outputLevel )
    {
	retVal = vprintf( format, vargs );
    }
    
    return( retVal );
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
*	10/19/95 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
int LogOutput( int olevel, const char *format, ... )
{
    va_list vargs;
    int retVal;

    va_start( vargs, format );
    retVal = vLogOutput( olevel, format, vargs );
    va_end( vargs );
    
    return( retVal );
}
