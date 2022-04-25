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
#include "stdafx.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
//#include <unistd.h>
#include <time.h>
//#include <sys/param.h>
#include <limits.h>
#include <sys/stat.h>
#define EXTERN
#include "output.h"

//static char szLogFilename[PATH_MAX] = "./default.log";

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
	char temp[4096*3];
	    
    if( olevel <= outputLevel )
	{
		retVal = vsprintf( temp, format, vargs );
		FILE *output;
		output = fopen("d:\\out.txt", "a+");
		if( output )
		{
			vfprintf(output, format, vargs);
			fclose(output);
		}
		TRACE("%s", temp);
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
*	01/11/95 - jwb - Version 1.1.0					    *
*			    Separated function to allow one version to be   *
*			    called with varargs and the other with va_list. *
*	12/01/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
int Output( int olevel, const char *format, ... )
{
    va_list vargs;

    va_start( vargs, format );
    int retVal = vOutput( olevel, format, vargs );
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

    va_start( vargs, format );
    int retVal = vDebugOutput( olevel, format, vargs );
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

    va_start( vargs, format );
    int retVal = vErrorOutput( format, vargs );
    va_end( vargs );
    
    return( retVal );
}

