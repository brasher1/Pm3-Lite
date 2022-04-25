/*COPYRIGHT******************************************************************
*																			*
*   Copyright (c) 1997 AsBuilt Data, Inc.  These coded instructions,	    *
*   statements, computer programs and source code contain unpublished	    *
*   trade secrets of AsBuilt Data, Inc. and are protected by Federal	    *
*   Copyright Law.  They may not be disclosed or copied, duplicated,	    *
*   distributed, demonstrated or licensed in any form, in whole or in	    *
*   part, without the prior written consent of AsBuilt Data, Inc.  Any	    *
*   permitted use shall be considered as a personal license to the			*
*   authorized recipient only.  Any such permitted use is provided on a	    *
*   "as is" basis without any warranty of any kind, express or implied,	    *
*   and specifically without any warranty of merchantibility or fitness	    *
*   for a particular purpose.  In no event shall AsBuilt Data, Inc. or	    *
*   its affiliates be liable for any special, incidental, direct or			*
*   indirect damages of any kind, or for any liabilities in excess of the   *
*   royalty fee payments actually made by Licensee hereunder regardless of  *
*   legal theory or negligence of AsBuilt Data, Inc.						*
*																			*
******************************************************************COPYRIGHT*/

/*FF*************************************************************************
*																			*
*   FILE																	*
*																			*
*   DESCRIPTION																*
*																			*
*   NOTES																	*
*																			*
*   BUGS																	*
*																			*
*   REVISION HISTORY														*
*	05/30/97 - jwb - Version 1.0.0											*
*																			*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "output.h"
#include "strUtils.h"
#include "3dclass/Ptg3DPointObj.h"

#if !defined OKAY
#define OKAY 0
#endif

#define MAX_LINE_LENGTH 1024

// Borrowed from importReviewModel
extern char *readLine( char *s, int n, FILE *fp );

#define FUNC "importSurveyData"
int importSurveyData( char *pszSurveyTextFile, Ptg3DNode *theParent )
{
	if( pszSurveyTextFile == NULL || theParent == NULL )
	{
		ErrorOutput( "Invalid parameter passed to %s.\n", FUNC );
		return( !OKAY );
	}

	FILE *fpSurveyFile = fopen( pszSurveyTextFile, "r" );
	if( fpSurveyFile == NULL )
	{
		ErrorOutput( "Unable to open '%s'.\n", pszSurveyTextFile );
		return( !OKAY );
	}

	char szLine[MAX_LINE_LENGTH];

	while( readLine( szLine, sizeof(szLine), fpSurveyFile ) != NULL )
	{
		char **tokens = NULL;
		int numTokens = 0;

		getTokens( szLine, &tokens, &numTokens, ',' );
		if( numTokens == 4 )
		{
			double east, north, up;
			east = atof(tokens[1]);
			north = atof(tokens[2]);
			up = atof(tokens[3]);
			Ptg3DPointObj *point = new Ptg3DPointObj;
			point->setPosition( east, up, -north );
			point->setName( tokens[0] );
			point->setDrawStyle( Ptg3DNode::DS_WIREFRAME );
			point->setActive( TRUE );
			theParent->addMember( point );
		}
		freeTokens( tokens, numTokens );
	}

	fclose( fpSurveyFile );

	return OKAY;
}
#undef FUNC