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
*	06/06/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#include "stdafx.h"
#include "photog.h"
#include "mathsubs.h"
#include "photog_math.h"
//#include "ptgGlobals.h"
#include "bundleutils.h"
#include "output.h"
#include "vista.h"
//#include "dbtype.h"

#include "rdm_utils.h"

#include "internal.h"
#define TASKER		&Mytask,dbn

#ifdef _FOTOBROW_CPP_
	DB_TASK	Mytask;
	int dbn;
#else
	extern DB_TASK	Mytask;
	extern int dbn;
#endif

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
*	xx/xx/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "GetCalcPnt"
void GetCalcPnt( DB_ADDR dbaBundle, DB_ADDR dbaPointsRec, VECTOR & vCalcPnt, 
	DB_ADDR & dbaCalcPnt )
{
    struct xyz xyzRec;
    DB_ADDR dba;
    int nFound = FALSE;
	int db_status;

    TRACE("GetCalcPnt\n");
    dt_csoset( POINTS_XYZ, &dbaPointsRec, TASKER );
    
    db_status = dt_findfm( POINTS_XYZ, TASKER );
    while( db_status == S_OKAY && !nFound )
    {
	dt_setmm( BUNDLE_XYZ, POINTS_XYZ, TASKER );
	dt_csoget( BUNDLE_XYZ, &dba, TASKER );
	
	if( dbaBundle == dba )
	{
	    db_status = dt_recread( &xyzRec, TASKER );
	    if( db_status == S_OKAY )
	    {
		dt_csmget( POINTS_XYZ, &dbaCalcPnt, TASKER );

		vCalcPnt = vector( 1, 3 );
		vCalcPnt[1] = xyzRec.dfCoor[0];
		vCalcPnt[2] = xyzRec.dfCoor[1];
		vCalcPnt[3] = xyzRec.dfCoor[2];
	    }
	    nFound = TRUE;
	}
	else
	    db_status = dt_findnm( POINTS_XYZ, TASKER );
    }
    
    if( !nFound )
    {
	/* Didn't find one */
//	Output( 4, "No calc point for %d:%d\n", GET_FNO(dbaPointsRec), GET_RNO(dbaPointsRec) );
	vCalcPnt = NULL;
	dbaCalcPnt = NULL_DBA;
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
*	xx/xx/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "GetSurvPnt"
void GetSurvPnt( DB_ADDR dbaPointsRec, VECTOR & vSurvPnt )
{
    struct xyz xyzRec;
    int db_status;

	TRACE("GetSurvPnt\n");
    dt_csoset( POINTS_SURVEY_XYZ, &dbaPointsRec, TASKER );
    
    db_status = dt_findlm( POINTS_SURVEY_XYZ, TASKER );
    if( db_status == S_OKAY )
    {
	db_status = dt_recread( &xyzRec, TASKER );
	if( Mytask.v.ptr->Db_status == S_OKAY )
	{
	    vSurvPnt = vector( 1, 3 );
	    
	    vSurvPnt[1] = xyzRec.dfCoor[0];
	    vSurvPnt[2] = xyzRec.dfCoor[1];
	    vSurvPnt[3] = xyzRec.dfCoor[2];
	}
    }
    else
    {
        /* Didn't find one */
//	Output( 1, "No survey point for %d:%d\n", GET_FNO(dbaPointsRec), GET_RNO(dbaPointsRec) );    
	vSurvPnt = NULL;
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
*	xx/xx/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "IsValidTMatrix"
int IsValidTMatrix( struct tmatrix TMatrixRec )
{
    int i;
    double dfMag = 0;
    
    for( i = 0; i < 12; i++ )
	dfMag += TMatrixRec.dfCameraPos[i] * TMatrixRec.dfCameraPos[i];
    if( dfMag > 0 )
	return( TRUE );
    else
	return( FALSE );
}
#undef FUNC
