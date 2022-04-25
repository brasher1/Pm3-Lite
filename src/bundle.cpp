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

//#define MAIN
//#define EXTERN
#define OKAY	0
#define WriteLogEntry	TRACE
#ifdef WIN32
#include "stdafx.h"
#endif
//#include "photo.h"
//#include "common.h"
#include "photog.h"
#include "mathsubs.h"
#include "photog_math.h"

#include "vista.h"
//#include "dbtype.h"

#include "rdm_utils.h"

#include "internal.h"
//#include "ptgGlobals.h"
//#include "tplsSecurity.h"
#include "bundleutils.h"
#include "output.h"
//#include "XUtils.h"
//#include "profile.h"

//#include "bundlever.h"
//#include "photogmathver.h"
#include "mathsubs.h"
//#include "checkbundle.h"

#include <assert.h>
/////////////////////////////////////////////////////////////////////////////
#define TASKER		&Mytask,dbn

#ifdef _FOTOBROW_CPP_
	DB_TASK	Mytask;
	int dbn;
#else
	extern DB_TASK	Mytask;
	extern int dbn;
#endif

//////////////////////////////////////////////////////////////////////////////
#ifdef PHOTOAB
#define APP_TITLE "Photo-AB bundle"
#else
#define APP_TITLE "PM^3 bundle"
#endif

int setProjectVars( char *pszProjName = NULL );

/***************************************************************************/
void PrintUsage( void );

int ReadBundleData( const char *pszBundleID, BundleData & bd );
int ReadBundleTMatrices( BundleData & bd );
int ReadBundlePhotos( BundleData & bd );

int GetPointIndex( BundleData & bd, DB_ADDR dbaPnt );
int AddPointRow( BundleData & bd );

int WriteBundleData( BundleData & bd );
int WriteBundleTMatrices( const BundleData & bd );
int WriteBundleCalcPoints( const BundleData & bd );

void FreeBundleData( BundleData & bd );
	
void PrintBundleData( const BundleData & bd );
/***************************************************************************/

#define FUNC "bundlemain"
int bundlemain( char *pszBundle, int nBundleType )
{
    BundleData bd;
    static LOCK_REQUEST lr[] = {
	{ BUNDLE, 'r' }, 
	{ TMATRIX, 'r' }, 
	{ IMAGES, 'r' }, 
	{ PIXELS, 'r' }, 
	{ POINTS, 'r' }, 
	{ XYZ, 'r' }
    };
    int i;
    
    WriteLogEntry( "bundle started - type = %s - bundle = '%s'", 
	    nBundleType ? "incremental" : "full", pszBundle );
    Output( 0, "Running Bundle '%s'\n", pszBundle );
	    
    /* Read database to obtain everything we need to know about a
     * bundle.
     */
    memset( &bd, 0, sizeof(BundleData) );

    DebugOutput( 2, "memcount before ReadBundleData = %ld\n", memcount(0) );

    if( rdmGroupLock( 6, lr, TASKER ) != S_OKAY )
    {
//	ErrorOutput( "Database error %d - Unable to read lock records\n", Mytask.v.ptr->Db_status );
	WriteLogEntry( "bundle exiting abnormally." );
	return( !OKAY );
    }
    
    bd.pszBundleName = strdup(pszBundle);

    if( ReadBundleData( pszBundle, bd ) )
    {
	rdmGroupFree( 6, lr, TASKER );
	
	if( nBundleType == 0 )
	{
	    for( i = 0; i < bd.nNumPhotos; i++ )
	    {
		if( (*(bd.plTMatrix))[i] != NULL )
		{
		    free_vector( (*(bd.plTMatrix))[i], 1, 13 );
		    (*(bd.plTMatrix))[i] = NULL;
		}
	    }
	}		    
	
	DebugOutput( 1, "///////////////////////////////////////////\n" );
	DebugOutput( 1, "%s: bd = \n", FUNC );
	PrintBundleData( bd );	    
	DebugOutput( 1, "///////////////////////////////////////////\n" );
	time_t start, end;
	time(&start);
	if( bundle( bd ) )
	{
		time(&end);
		TRACE("The time period was %d\n", end-start);
		char temp[128];
		sprintf(temp,"The time period was %d\n", end-start);
		AfxMessageBox(temp);
	    DebugOutput( 1, "///////////////////////////////////////////\n" );
	    DebugOutput( 1, "%s: After bundle:    bd = \n", FUNC );
	    PrintBundleData( bd );	    
	    DebugOutput( 1, "///////////////////////////////////////////\n" );

	    int bWantToSave = FALSE;
/*
	    switch( checkBundle( bd ) )
	    {
		case CB_OK:
		    bWantToSave = TRUE;
		    break;
		
		case CB_BAD_ORTHO:
		    bWantToSave = (MessageBox( getTopLevelWidget(), "Ortho measure is outside tolerance.\nSave bundle results anyway?",
			    "Bundle Results Error", MB_YESNO ) == IDYES );
		    break;
		
		default:
		    MessageBox( getTopLevelWidget(), "Error checking bundle results for bundle '%s'.",
			    "Bundle Results Error", MB_OK );
		    break;
	    }
*/
	    if( bWantToSave )
	    {
		if( !WriteBundleData( bd ) )
		    ErrorOutput( "Unable to write bundle data for '%s'\n", pszBundle );
		else
		    WriteLogEntry( "bundle finished successfully - bundle = '%s'", pszBundle );
	    }
	    else
		WriteLogEntry( "bundle aborted - bundle = '%s'", pszBundle );
	}
	else
	    ErrorOutput( "bundle '%s' failed\n", pszBundle );
    }
    else
    {
	ErrorOutput( "Unable to read bundle data for '%s'.\n", pszBundle );
	rdmGroupFree( 6, lr, TASKER );
    }
    
    FreeBundleData( bd );
    Output( 0, "Bundle complete '%s'\n", pszBundle );

    return( OKAY );
}
#undef FUNC

/*********************************************************************************/
#if FALSE
#define FUNC "bundlemain"
void main( int argc, char *argv[] )
{
    char *pszBundle;
    BundleData bd;
    static LOCK_REQUEST lr[] = {
	{ BUNDLE, 'r' }, 
	{ TMATRIX, 'r' }, 
	{ IMAGES, 'r' }, 
	{ PIXELS, 'r' }, 
	{ POINTS, 'r' }, 
	{ XYZ, 'r' }
    };
    int nBundleType;
    int i;
    
    // Initialize global output variables
//    outputLevel = 10;
//    debugOutputLevel = 10;

    Output( 0, "%s - version %s\n", APP_TITLE, VERSION );
    Output( 1, "PhotogMath - version %s\n", PHOTOGMATH_VERSION );

    switch( argc )
    {
	case 6:
	    outputLevel = atoi(argv[1]);
	    debugOutputLevel = atoi(argv[2]);

	    setProjectVars( argv[3] );
#ifdef PHOTOAB
	    setLogFileLocation( szProjectLocation, "photo-ab.log" );
#else
	    setLogFileLocation( szProjectLocation, "pm3.log" );
#endif
	    nBundleType = atoi(argv[4]);
	    if( nBundleType < 0 || nBundleType > 1 )
		PrintUsage();
	    pszBundle = argv[5];
	    WriteLogEntry( "bundle started - type = %s - bundle = '%s'", 
		    nBundleType ? "incremental" : "full", pszBundle );
	    break;
	
	default:
	    PrintUsage();
    }

    initGlobals();
    DebugOutput( 1, "currSecurityLevel = %d\n", currSecurityLevel );
    
    if( !(currSecurityLevel & (SECLEV_BUNDLE|SECLEV_FULL)) )
    {
	ErrorOutput( "%s Security Error: Not licensed for bundling.\n", LOGO );
	exit( EXIT_FAILURE );
    }
    
    initRDM();

    Output( 0, "Running Bundle '%s'\n", pszBundle );
	    
    /* Read database to obtain everything we need to know about a
     * bundle.
     */
    memset( &bd, 0, sizeof(BundleData) );

    DebugOutput( 2, "memcount before ReadBundleData = %ld\n", memcount(0) );

    if( rdmGroupLock( 6, lr, dbn ) != S_OKAY )
    {
	ErrorOutput( "Database error %d - Unable to read lock records\n", Mytask.v.ptr->Db_status );
	WriteLogEntry( "bundle exiting abnormally." );
	exit(1);
    }
    
    bd.pszBundleName = strdup(pszBundle);

    if( ReadBundleData( pszBundle, bd ) )
    {
	rdmGroupFree( 6, lr, dbn );
	
	if( nBundleType == 0 )
	{
	    for( i = 0; i < bd.nNumPhotos; i++ )
	    {
		if( (*(bd.plTMatrix))[i] != NULL )
		{
		    free_vector( (*(bd.plTMatrix))[i], 1, 13 );
		    (*(bd.plTMatrix))[i] = NULL;
		}
	    }
	}		    
	
	DebugOutput( 1, "///////////////////////////////////////////\n" );
	DebugOutput( 1, "%s: bd = \n", FUNC );
	PrintBundleData( bd );	    
	DebugOutput( 1, "///////////////////////////////////////////\n" );

	if( bundle( bd ) )
	{
	    DebugOutput( 1, "///////////////////////////////////////////\n" );
	    DebugOutput( 1, "%s: After bundle:    bd = \n", FUNC );
	    PrintBundleData( bd );	    
	    DebugOutput( 1, "///////////////////////////////////////////\n" );

	    if( !WriteBundleData( bd ) )
		ErrorOutput( "Unable to write bundle data for '%s'\n", pszBundle );
	    else
		WriteLogEntry( "bundle finished successfully - bundle = '%s'", pszBundle );
	}
	else
	    ErrorOutput( "bundle '%s' failed\n", pszBundle );
    }
    else
	ErrorOutput( "Unable to read bundle data for '%s'.\n", pszBundle );

//    rdmGroupFree( 6, lr, dbn );		
    FreeBundleData( bd );
    
    freeSecurityLevel();
}
#undef FUNC
#endif
/*********************************************************************************/

#define FUNC "PrintUsage"
void PrintUsage( void )
{
    fprintf( stderr, "USAGE: bundle [type] name.\n" );
    fprintf( stderr, "  type = 0 for full bundle\n" );
    fprintf( stderr, "         1 for incremental bundle(default)\n" );
    exit(1);
}
#undef FUNC

#define FUNC "ReadBundleData"
int ReadBundleData( const char *pszBundleTag, BundleData & bd )
{
//    FUNCENTRY(FUNC);

    struct bundle BundleRec;
    int db_status;
	TRACE("Reading Bundle\n");
    /* find the bundle record and store its dba */
    memset( &BundleRec, 0, sizeof(struct bundle) );
    strcpy( BundleRec.szTag, pszBundleTag );
    db_status = dt_keyfind( BUNDLE_SZTAG, &(BundleRec.szTag), TASKER );
    if( db_status != S_OKAY )
    {
	ErrorOutput( "Unable to find bundle '%s'\n", BundleRec.szTag );
	return( FALSE );
    }
    dt_crget( &(bd.dbaBundle), TASKER );

    if( !ReadBundleTMatrices( bd ) )
    {
	ErrorOutput( "Unable to read bundle(%s) tmatrix data.\n", BundleRec.szTag );
	return( FALSE );
    }

    if( !ReadBundlePhotos( bd ) )
    {
	ErrorOutput( "Unable to read bundle(%s) photo data.\n", BundleRec.szTag );
	return( FALSE );
    }
    
//    FUNCEXIT(FUNC);
    return( TRUE );
}
#undef FUNC

#define FUNC "ReadBundleTMatrices"
int ReadBundleTMatrices( BundleData & bd )
{
    long lnNumPhotos;
    int nCurrPhoto;
    struct tmatrix TMatrixRec;
    int bStatus = TRUE;
    int i;
	int db_status;

    /* set owner of BUNDLE_TMATRIX to bundle we're doing */    
    dt_crset( &(bd.dbaBundle), TASKER );
    dt_setor( BUNDLE_TMATRIX, TASKER );
    
	TRACE("Reading TMatrices\n");
    /* Find out how many tmatrix members the bundle has.
     * This is also the number of photos in the bundle.
     */
    dt_members( BUNDLE_TMATRIX, &(lnNumPhotos), TASKER );
    bd.nNumPhotos = (int)lnNumPhotos;
    if( bd.nNumPhotos <= 0 )
    {
	ErrorOutput( "No photos in bundle.\n" );
	return( FALSE );
    }
    
    bd.plTMatrix = matrix_shell( 0, bd.nNumPhotos-1, 1, 13 );
    bd.dbaTMatrix = (DB_ADDR *)malloc( sizeof(DB_ADDR) * bd.nNumPhotos );
    if( bd.plTMatrix == NULL || bd.dbaTMatrix == NULL )
    {
	ErrorOutput( "Memory allocation error\n" );
	return( FALSE ); 
    }
    
    memset( bd.dbaTMatrix, 0, sizeof(DB_ADDR) * bd.nNumPhotos );

    nCurrPhoto = 0;    
    db_status = dt_findfm( BUNDLE_TMATRIX, TASKER );
    while( db_status == S_OKAY  && bStatus )
    {
	db_status = dt_recread( &TMatrixRec, TASKER );
	
	if( db_status == S_OKAY )
	{
	    /* Make sure we have a valid tmatrix */
	    if( IsValidTMatrix( TMatrixRec ) )
	    {
		(*(bd.plTMatrix))[nCurrPhoto] = vector( 1, 13 );
		if( (*(bd.plTMatrix))[nCurrPhoto] != NULL )
		{
		    for( i = 0; i < 12; i++ )
			(*(bd.plTMatrix))[nCurrPhoto][i+1] = TMatrixRec.dfCameraPos[i];
		}
		else
		    bStatus = FALSE;
	    }
	    else
		DebugOutput( 2, "Found an invalid TMatrix(%d)\n", nCurrPhoto );

	    dt_csmget( BUNDLE_TMATRIX, &(bd.dbaTMatrix[nCurrPhoto]), TASKER );
	    nCurrPhoto++;
	}
	
	db_status = dt_findnm( BUNDLE_TMATRIX, TASKER );
    }
    TRACE("Out of Read TMatrices\n");
    if( nCurrPhoto != bd.nNumPhotos || !bStatus )
    {
	ErrorOutput( "Unable to find all tmatrix records.\n" );
	for( nCurrPhoto = 0; nCurrPhoto < bd.nNumPhotos; nCurrPhoto++ )
	{
	    if( bd.plTMatrix[nCurrPhoto] != NULL )
		free( bd.plTMatrix[nCurrPhoto] );
	}
	free( bd.dbaTMatrix );
	free( bd.plTMatrix );
	bd.dbaTMatrix = NULL;
	bd.plTMatrix = NULL;
	return( FALSE );
    }
    
    return( TRUE );
}
#undef FUNC

#define FUNC "ReadBundlePhotos"
int ReadBundlePhotos( BundleData & bd )
{
    DB_ADDR dbaPointsRec;
    int i;
    int nPointIndex;
    int nCurrPhoto;
    struct pixels PixelRec;
    struct images theImage;
    struct points thePoint;
    struct camera_info theCamera;
    int db_status;

    bd.plPixels = (PNTLST *)malloc( sizeof(PNTLST) * bd.nNumPhotos );
	TRACE("Read Bundle Photos\n");    
    if( bd.plPixels == NULL )
    {
	ErrorOutput( "Memory allocation error.\n" );
	return( FALSE );
    }
    
    memset( bd.plPixels, 0, sizeof(PNTLST) * bd.nNumPhotos );

    bd.nNumPoints = 0;
    bd.pszPhotoNames = (char **)malloc( bd.nNumPhotos*sizeof(char *) );
    bd.vfl = vector( 0, bd.nNumPhotos-1 );
        
    for( nCurrPhoto = 0; nCurrPhoto < bd.nNumPhotos; nCurrPhoto++ )
    {
	dt_crset( &(bd.dbaTMatrix[nCurrPhoto]), TASKER );
	dt_setmr( IMAGES_TMATRIX, TASKER );

	dt_setro( IMAGES_TMATRIX, TASKER );
	dt_recread( &theImage, TASKER );
	if( theImage.lUpToDate != 0 )
	    ErrorOutput( "Image '%s' appears to be out of date.\n", theImage.szName );
	
	bd.pszPhotoNames[nCurrPhoto] = strdup(theImage.szName);

	/* Get focal length for each image */
	dt_setmo( CAMERA_INFO_IMAGES, IMAGES_TMATRIX, TASKER );
	dt_setro( CAMERA_INFO_IMAGES, TASKER );
	dt_recread( &theCamera, TASKER );
	bd.vfl[nCurrPhoto] = theCamera.dfFocalLength;

	dt_setoo( IMAGES_PIXELS, IMAGES_TMATRIX, TASKER );
	
	db_status = dt_findfm( IMAGES_PIXELS, TASKER );
	while( db_status == S_OKAY )
	{
	    dt_recread( &PixelRec, TASKER );
	    
	    dt_setmm( POINTS_PIXELS, IMAGES_PIXELS, TASKER );
	    dt_csoget( POINTS_PIXELS, &dbaPointsRec, TASKER );
	    
	    nPointIndex = GetPointIndex( bd, dbaPointsRec );
	    /* if couldn't get point index, need a new row */
	    if( nPointIndex == -1 )
	    {
		nPointIndex = AddPointRow( bd );
		if( nPointIndex != -1 )
		{
		    dt_crset( &dbaPointsRec, TASKER );		
		    dt_recread( &thePoint, TASKER );
		    bd.pszPointNames = (char **)realloc( bd.pszPointNames, (nPointIndex+1)*sizeof(char *) );
		    bd.pszPointNames[nPointIndex] = strdup(thePoint.szLabel);

		    /* For a new row, set the dba of the points record, the calculated point data, and
		     * the survey point data.
		     */
		    bd.dbaPnts[nPointIndex] = dbaPointsRec;
			TRACE("Calling the GetPnt Calc and Survey routines\n");
		    GetCalcPnt( bd.dbaBundle, dbaPointsRec, (*(bd.plCalcPnts))[nPointIndex], 
			    bd.dbaCalcPnts[nPointIndex] );
		    GetSurvPnt( dbaPointsRec, (*(bd.plSurvPnts))[nPointIndex] );
			TRACE("After the Get Routines\n");
		}
	    }
	    if( nPointIndex != -1 )
	    {
		VECTOR v = vector( 1, 3 );
		for( i = 0; i < 3; i++ )
		    v[i+1] = PixelRec.dfCorr[i];

		(*((bd.plPixels)[nCurrPhoto]))[nPointIndex] = v;
	    }
	    else
		DebugOutput( 2, "Point index was %d\n", nPointIndex );
		
	    db_status = dt_findnm( IMAGES_PIXELS, TASKER );
	}
    }
    return( TRUE );
}
#undef FUNC


#define FUNC "GetPointIndex"
int GetPointIndex( BundleData & bd, DB_ADDR dbaPnt )
{
    int nCurrCalcPnt;
    
    for( nCurrCalcPnt = 0; nCurrCalcPnt < bd.nNumPoints; nCurrCalcPnt++ )
    {
	if( bd.dbaPnts[nCurrCalcPnt] == dbaPnt )
	    return( nCurrCalcPnt );
    }
    return( -1 );
}
#undef FUNC

#define FUNC "AddPointRow"
int AddPointRow( BundleData & bd )
{
    int i;
    
    /* Allocate point lists if needed */
    for( i = 0; i < bd.nNumPhotos; i++ )
    {
	if( bd.plPixels[i] == NULL )
	    bd.plPixels[i] = matrix_shell( 0, 0, 1, 3 );
	else
	    add_matrix_shell_rows( bd.plPixels[i], 1 );
	if( bd.plPixels[i] == NULL )
	{
	    ErrorOutput( "Memory allocation error\n" );
	    return( -1 );
	}
    }
    
    bd.dbaPnts = (DB_ADDR *)realloc( bd.dbaPnts, sizeof(DB_ADDR) * (bd.nNumPoints + 1) );
    if( bd.dbaPnts == NULL )
    {
	ErrorOutput( "Memory allocation error\n" );
	return( -1 );
    }
    
    bd.dbaCalcPnts = (DB_ADDR *)realloc( bd.dbaCalcPnts,
	    sizeof(DB_ADDR) * (bd.nNumPoints + 1) );
    if( bd.dbaCalcPnts == NULL )
    {
	ErrorOutput( "Memory allocation error.\n" );
	return( -1 );
    }

    if( bd.plCalcPnts == NULL )
	bd.plCalcPnts = matrix_shell( 0, 0, 1, 3 );
    else
	add_matrix_shell_rows( bd.plCalcPnts, 1 );
    if( bd.plCalcPnts == NULL )
    {
	ErrorOutput( "Memory allocation error.\n" );
	return( -1 );
    }
    
    if( bd.plSurvPnts == NULL )
	bd.plSurvPnts = matrix_shell( 0, 0, 1, 3 );
    else
	add_matrix_shell_rows( bd.plSurvPnts, 1 );
    if( bd.plSurvPnts == NULL )
    {
	ErrorOutput( "Memory allocation error\n" );
	return( -1 );
    }
    
    bd.nNumPoints++;
    return( bd.nNumPoints-1 );
}
#undef FUNC

#define FUNC "WriteBundleData"
int WriteBundleData( BundleData & bd )
{
    static LOCK_REQUEST lr[] = {
	{ BUNDLE, 'w' }, 
	{ TMATRIX, 'w' }, 
	{ IMAGES, 'w' }, 
	{ PIXELS, 'w' }, 
	{ POINTS, 'w' }, 
	{ XYZ, 'w' }
    };

    DebugOutput( 3, "%s: Entering\n", FUNC );
    
    rdmTransBegin( FUNC, &Mytask);
    if( Mytask.v.ptr->Db_status != S_OKAY )
    {
	ErrorOutput( "Database Error %d - Unable to start transaction.\n", Mytask.v.ptr->Db_status );
	return( FALSE );
    }

    if( rdmGroupLock( 6, lr, TASKER ) != S_OKAY )
    {
	ErrorOutput( "Database Error %d - Unable to read lock records.\n", Mytask.v.ptr->Db_status );
	exit(1);
    }
    
    if( !WriteBundleTMatrices( bd ) )
    {
	ErrorOutput( "WriteBundleTMatrices failed\n" );
	rdmTransAbort(&Mytask);
	return( FALSE );
    }
    
    if( !WriteBundleCalcPoints( bd ) )
    {
	ErrorOutput( "WriteBundleCalcPoints failed\n" );
	rdmTransAbort(&Mytask);
	return( FALSE );
    }
    
    rdmTransEnd(&Mytask);
    return( TRUE );
}
#undef FUNC

#define FUNC "WriteBundleTMatrices"
int WriteBundleTMatrices( const BundleData & bd )
{
    struct tmatrix tmatrix_rec;
    int nCurrPhoto;
    
    if( bd.plTMatrix == NULL || bd.dbaTMatrix == NULL )
    {
	ErrorOutput( "NULL TMatrix array\n" );
	return( FALSE );
    }

    rdmRecLock( TMATRIX, "w", TASKER );
    
    for( nCurrPhoto = 0; nCurrPhoto < bd.nNumPhotos; nCurrPhoto++ )
    {
	if( bd.dbaTMatrix[nCurrPhoto] == NULL )
	{
	    ErrorOutput( "dbaTMatrix[%d] is NULL\n", nCurrPhoto );
	    rdmRecFree( TMATRIX, TASKER );
	    return( FALSE );
	}
	
//	DebugOutput( 1, "Writing back record %d:%d\n",
//		GET_FNO( bd.dbaTMatrix[nCurrPhoto] ), 
//		GET_RNO( bd.dbaTMatrix[nCurrPhoto] ) );
	dt_crset( &(bd.dbaTMatrix[nCurrPhoto]), TASKER );
	
	if( (*(bd.plTMatrix))[nCurrPhoto] == NULL )
	{
	    ErrorOutput( "TMatrix[%d] is NULL\n", nCurrPhoto );
	    memset( &tmatrix_rec, 0, sizeof(struct tmatrix) );
	}
	else
	{
	    dt_recread( &tmatrix_rec, TASKER );
	}
	
	if( Mytask.v.ptr->Db_status == S_OKAY )
	{
	    for( int i = 0; i < 12; i++ )
		tmatrix_rec.dfCameraPos[i] =
			(*(bd.plTMatrix))[nCurrPhoto][i+1];
	    if( dt_recwrite( &tmatrix_rec, TASKER ) != S_OKAY )
	    {
		ErrorOutput( "Database Error %d - Unable to write tmatrix record\n", Mytask.v.ptr->Db_status );
		rdmRecFree( TMATRIX, TASKER );
		return( FALSE );
	    }
	}
	else
	{
	    ErrorOutput( "Database Error %d - reading tmatrix record\n", Mytask.v.ptr->Db_status );
	    rdmRecFree( TMATRIX, TASKER );
	    return( FALSE );
	}
    }

    rdmRecFree( TMATRIX, TASKER );
    return( TRUE );
}
#undef FUNC

#define FUNC "WriteBundleCalcPoints"
int WriteBundleCalcPoints( const BundleData & bd )
{
    struct xyz xyz_rec;
    int nCurrPnt;
    static LOCK_REQUEST lr[] = {
	{ BUNDLE, 'w' }, 
	{ XYZ, 'w' }, 
	{ POINTS, 'w' }
    };

    assert( bd.dbaCalcPnts != NULL );
    assert( bd.dbaPnts != NULL );
    assert( bd.plCalcPnts != NULL );
    assert( bd.dbaBundle != NULL );
    
    rdmGroupLock( 3, lr, TASKER );
    
    DB_ADDR tmpdba = bd.dbaBundle;
    dt_csoset( BUNDLE_XYZ, &tmpdba, TASKER );
    
    for( nCurrPnt = 0; nCurrPnt < bd.nNumPoints; nCurrPnt++ )
    {
	if( ((*(bd.plCalcPnts))[nCurrPnt] != NULL ) )
	{
	    if( (bd.dbaCalcPnts[nCurrPnt] != NULL_DBA) )
	    {
//		DebugOutput( 1, "Updating calc pnt (xyz) record %d:%d\n",
//			GET_FNO(bd.dbaCalcPnts[nCurrPnt]), 
//			GET_RNO(bd.dbaCalcPnts[nCurrPnt]) );
		dt_crset( &(bd.dbaCalcPnts[nCurrPnt]), TASKER );
		dt_recread( &xyz_rec, TASKER );
		if( Mytask.v.ptr->Db_status != S_OKAY )
		{
//		    ErrorOutput( "Database Error %d - Unable to read calc pnt (xyz) %d:%d\n", 
//			    Mytask.v.ptr->Db_status, GET_FNO(bd.dbaCalcPnts[nCurrPnt]), 
//			    GET_RNO(bd.dbaCalcPnts[nCurrPnt]) );
		    rdmGroupFree( 3, lr, TASKER );
		    return( FALSE );
		}
		
		xyz_rec.dfCoor[0] = (*(bd.plCalcPnts))[nCurrPnt][1];
		xyz_rec.dfCoor[1] = (*(bd.plCalcPnts))[nCurrPnt][2];
		xyz_rec.dfCoor[2] = (*(bd.plCalcPnts))[nCurrPnt][3];
		
		
		if( dt_recwrite( &xyz_rec, TASKER ) != S_OKAY )
		{
//		    ErrorOutput( "Database Error %d - Unable to write calc pnt (xyz) %d:%d\n", 
//			    Mytask.v.ptr->Db_status, GET_FNO(bd.dbaCalcPnts[nCurrPnt]), 
//			    GET_RNO(bd.dbaCalcPnts[nCurrPnt]) );
		    rdmGroupFree( 3, lr, TASKER );
		    return( FALSE );
		}
	    }
	    else
	    {
		Output( 1, "Creating new calc pnt (xyz) record for pnt index = %d\n", nCurrPnt );
		if( bd.dbaPnts[nCurrPnt] == NULL_DBA )
		{
		    ErrorOutput( "bd.dbaPnts[%d] is NULL_DBA\n", nCurrPnt );
		    rdmGroupFree( 3, lr, TASKER );
		    return( FALSE ); 
		}
		
		if( (*(bd.plCalcPnts))[nCurrPnt] != NULL )
		{
		    memset( &xyz_rec, 0, dbn );
		    xyz_rec.dfCoor[0] = (*(bd.plCalcPnts))[nCurrPnt][1];
		    xyz_rec.dfCoor[1] = (*(bd.plCalcPnts))[nCurrPnt][2];
		    xyz_rec.dfCoor[2] = (*(bd.plCalcPnts))[nCurrPnt][3];
		    
		    if( dt_fillnew( XYZ, &xyz_rec, TASKER) != S_OKAY )
		    {
			ErrorOutput( "Database Error %d - creating new xyz record.\n", Mytask.v.ptr->Db_status );
			rdmGroupFree( 3, lr, TASKER );
			return( FALSE );
		    }
		    
		    dt_csoset( POINTS_XYZ, &(bd.dbaPnts[nCurrPnt]), TASKER );
		    
		    if( dt_connect( POINTS_XYZ, TASKER ) != S_OKAY )
		    {
			ErrorOutput( "Database Error %d - Unable to connect new xyz to POINTS record.\n", Mytask.v.ptr->Db_status );
			rdmGroupFree( 3, lr, TASKER );
			return( FALSE );
		    }
		    if( dt_connect( BUNDLE_XYZ, TASKER ) != S_OKAY )
		    {
			ErrorOutput( "Database Error %d - Unable to connect new xyz to BUNDLE record.\n", Mytask.v.ptr->Db_status );
			rdmGroupFree( 3, lr, TASKER );
			return( FALSE );
		    }
		}
		else
		    DebugOutput( 1, "bd.plCalcPnts[%d] is NULL.\n", nCurrPnt );
	    }
	}
	else
	{
	    DebugOutput( 1, "bd.plCalcPnts[%d] is NULL\n", nCurrPnt );
	    if( (bd.dbaCalcPnts[nCurrPnt] != NULL_DBA) )
	    {
//		Output( 1, "Deleting calculated point record %d:%d.\n",
//			GET_FNO(bd.dbaCalcPnts[nCurrPnt]), GET_RNO(bd.dbaCalcPnts[nCurrPnt]) );
		if( (dt_crset( &(bd.dbaPnts[nCurrPnt]), TASKER ) != S_OKAY) || (dt_disdel( TASKER) != S_OKAY) )
		{
		    ErrorOutput( "Database Error %d - Unable to delete calculated point.\n", Mytask.v.ptr->Db_status );
		    rdmGroupFree( 3, lr, TASKER );
		    return( FALSE );
		}
	    }
	}
    }
    
    rdmGroupFree( 3, lr, TASKER );
    return( TRUE );
}
#undef FUNC

#define FUNC "FreeBundleData"
void FreeBundleData( BundleData & bd )
{
    if( bd.dbaTMatrix != NULL )
    {
	free( bd.dbaTMatrix );
	bd.dbaTMatrix = NULL;
    }
    else
	DebugOutput( 1, "%s: bd.dbaTMatrix was NULL\n", FUNC );
    
    if( bd.dbaPnts != NULL )
    {
	free( bd.dbaPnts );
	bd.dbaPnts = NULL;
    }
    else
	DebugOutput( 1, "%s: bd.dbaPnts was NULL\n", FUNC );
    
    if( bd.dbaCalcPnts != NULL )
    {
	free( bd.dbaCalcPnts );
	bd.dbaCalcPnts = NULL;
    }
    else
	DebugOutput( 1, "%s: bd.dbaCalcPnts was NULL\n", FUNC );
    
    if( bd.plTMatrix != NULL )
    {
	free_matrix( bd.plTMatrix, 0, bd.nNumPhotos-1, 1, 13 );
	bd.plTMatrix = NULL;
    }
    else
	DebugOutput( 1, "%s: bd.plTMatrix was NULL\n", FUNC );

    if( bd.plPixels != NULL )
    {
	for( int i = 0; i < bd.nNumPhotos; i++ )
	{
	    if( bd.plPixels[i] != NULL )
	    {
		free_matrix( bd.plPixels[i], 0, bd.nNumPoints-1, 1, 3 );
		bd.plPixels[i] = NULL;
	    }
	    else
		DebugOutput( 1, "%s: bd.plPixels[%d] = NULL\n", FUNC, i );
	}
	free( bd.plPixels );
	bd.plPixels = NULL;
    }
    else
	DebugOutput( 1, "%s: bd.plPixels was NULL\n", FUNC );
    
    if( bd.plCalcPnts != NULL )
    {
	free_matrix( bd.plCalcPnts, 0, bd.nNumPoints-1, 1, 3 );
	bd.plCalcPnts = NULL;
    }
    else
	DebugOutput( 1, "%s: bd.plCalcPnts was NULL\n", FUNC );
    
    if( bd.plSurvPnts != NULL )
    {
	free_matrix( bd.plSurvPnts, 0, bd.nNumPoints-1, 1, 3 );
	bd.plSurvPnts = NULL;
    }
    else
	DebugOutput( 1, "%s: bd.plSurvPnts was NULL\n", FUNC );

    if( bd.pszBundleName )
	free( bd.pszBundleName );
    if( bd.pszPhotoNames )
    {
	for( int i = 0; i < bd.nNumPhotos; i++ )
	    if( bd.pszPhotoNames[i] )
		free( bd.pszPhotoNames[i] );
    }
    if( bd.pszPointNames )
    {
	for( int i = 0; i < bd.nNumPoints; i++ )
	    if( bd.pszPointNames[i] )
		free( bd.pszPointNames[i] );
    }
}
#undef FUNC

extern void print_rotation( VECTOR );

#define FUNC "PrintBundleData"
void PrintBundleData( const BundleData & bd )
{
    int i, j;

    DebugOutput( 1, "Bundle:\n" );
    DebugOutput( 1, "    name = %s\n", bd.pszBundleName );
//    DebugOutput( 2, "    dba = %d:%d\n", GET_FNO(bd.dbaBundle), GET_RNO(bd.dbaBundle) );
    DebugOutput( 1, "    # of photos = %d\n", bd.nNumPhotos );
    DebugOutput( 1, "    # of points = %d\n", bd.nNumPoints );

    if( bd.dbaTMatrix != NULL )
    {
	Output( 0, "Images:\n" );
	for( i = 0; i < bd.nNumPhotos; i++ )
	{
	    Output( 0, "    %s\n", bd.pszPhotoNames[i] );
	    DebugOutput( 2, "        index = %d\n", i );
//	    DebugOutput( 2, "        dba = %d:%d\n", GET_FNO(bd.dbaTMatrix[i]), GET_RNO(bd.dbaTMatrix[i]) );

	    DebugOutput( 2, "        transform\n" );
	    if( (*(bd.plTMatrix))[i] != NULL )
	    {
		for( j = 1; j <= 12; j++ )
		    DebugOutput( 2, "            [%d] = %g\n", j, (*(bd.plTMatrix))[i][j] );
		print_rotation( (*(bd.plTMatrix))[i] );
	    }
	    else
		DebugOutput( 2, "            (NULL)\n" );
	    
	    DebugOutput( 2, "        pixel coordinates:\n" );
	    if( bd.plPixels[i] != NULL )
	    {
		for( j = 0; j < bd.nNumPoints; j++ )
		{
		    DebugOutput( 2, "        [%d] = ", j );
		    if( (*(bd.plPixels[i]))[j] != NULL )
			DebugOutput( 1, "%lf, %lf, %lf\n", (*((bd.plPixels)[i]))[j][1], (*((bd.plPixels)[i]))[j][2], (*((bd.plPixels)[i]))[j][3] );
		    else
			DebugOutput( 1, "(NULL)\n" );
		}
	    }
	    else
		DebugOutput( 1, "(NULL)\n" );

	}
    }

    DebugOutput( 2, "Calculated Points:\n" );
    if( bd.plCalcPnts != NULL )
    {
	for( i = 0; i < bd.nNumPoints; i++ )
	{
	    DebugOutput( 2, "    %s [%d] = ", bd.pszPointNames[i], i );
	    if( (*(bd.plCalcPnts))[i] != NULL )
		DebugOutput( 2, "%lf, %lf, %lf\n", (*(bd.plCalcPnts))[i][1], (*(bd.plCalcPnts))[i][2], (*(bd.plCalcPnts))[i][3] );
	    else
		DebugOutput( 2, "(NULL)\n" ); 
	}
    }
    else
	DebugOutput( 2, "(NULL)\n" );

    DebugOutput( 2, "Survey Points:\n" );
    if( bd.plSurvPnts != NULL )
    {
	for( i = 0; i < bd.nNumPoints; i++ )
	{
	    DebugOutput( 2, "    %s [%d] = ", bd.pszPointNames[i], i );
	    if( (*(bd.plSurvPnts))[i] != NULL )
		DebugOutput( 2, "%lf, %lf, %lf\n", (*(bd.plSurvPnts))[i][1], (*(bd.plSurvPnts))[i][2], (*(bd.plSurvPnts))[i][3] );
	    else
		DebugOutput( 2, "(NULL)\n" ); 
	}
    }
    else
	DebugOutput( 2, "(NULL)\n" );

#if FALSE    
    DebugOutput( 1, "\n\n\n" );
    DebugOutput( 1, "dbaBundle = %d:%d\n",
	    GET_FNO(bd.dbaBundle), GET_RNO(bd.dbaBundle) );
    DebugOutput( 1, "nNumPhotos = %d\n", bd.nNumPhotos );
    DebugOutput( 1, "nNumPoints = %d\n", bd.nNumPoints );

    DebugOutput( 1, "dbaTMatrix = " );
    if( bd.dbaTMatrix != NULL )
    {
	DebugOutput( 1, "\n" );
	
	for( i = 0; i < bd.nNumPhotos; i++ )
	    DebugOutput( 1, "[%d] = %d:%d\n", i, GET_FNO(bd.dbaTMatrix[i]), GET_RNO(bd.dbaTMatrix[i]) );
    }
    else
	DebugOutput( 1, "(NULL)\n" );
    DebugOutput( 1, "\n" );

    DebugOutput( 1, "plTMatrix = " );
    if( bd.plTMatrix != NULL )
    {
	DebugOutput( 1, "\n" );
	
	for( i = 0; i < bd.nNumPhotos; i++ )
	{
	    if( (*(bd.plTMatrix))[i] != NULL )
	    {
		Output( 0, "Image %s\n", bd.pszPhotoNames[i] );
		print_rotation( (*(bd.plTMatrix))[i] );
	    
		for( j = 1; j <= 12; j++ )
		    DebugOutput( 1, "[%d][%d] = %g\n", i, j, (*(bd.plTMatrix))[i][j] );
	    }
	    else
		DebugOutput( 1, "[%d] = (NULL)\n", i );
	}
    }
    else
	DebugOutput( 1, "(NULL)\n" );
    DebugOutput( 1, "\n" );

    DebugOutput( 1, "plPixels\n" );
    for( i = 0; i < bd.nNumPhotos; i++ )
    {
	DebugOutput( 1, "  Photo[%d] = ", i );
	if( bd.plPixels[i] != NULL )
	{
	    DebugOutput( 1, "\n" );
	    for( j = 0; j < bd.nNumPoints; j++ )
	    {
		DebugOutput( 1, "[%d] = ", j );
		if( (*(bd.plPixels[i]))[j] != NULL )
		    DebugOutput( 1, "%lf, %lf, %lf\n", (*((bd.plPixels)[i]))[j][1], (*((bd.plPixels)[i]))[j][2], (*((bd.plPixels)[i]))[j][3] );
		else
		    DebugOutput( 1, "(NULL)\n" );
	    }
	}
	else
	    DebugOutput( 1, "(NULL)\n" );
    }
    
    DebugOutput( 1, "plCalcPnts = " );
    if( bd.plCalcPnts != NULL )
    {
	DebugOutput( 1, "\n" );
	for( i = 0; i < bd.nNumPoints; i++ )
	{
	    DebugOutput( 1, "[%d] = ", i );
	    if( (*(bd.plCalcPnts))[i] != NULL )
		DebugOutput( 1, "%lf, %lf, %lf\n", (*(bd.plCalcPnts))[i][1], (*(bd.plCalcPnts))[i][2], (*(bd.plCalcPnts))[i][3] );
	    else
		DebugOutput( 1, "(NULL)\n" ); 
	}
    }
    else
	DebugOutput( 1, "(NULL)\n" );

    DebugOutput( 1, "plSurvPnts = " );
    if( bd.plSurvPnts != NULL )
    {
	DebugOutput( 1, "\n" );
	for( i = 0; i < bd.nNumPoints; i++ )
	{
	    DebugOutput( 1, "%s[%d] = ", bd.pszPointNames[i], i );

	    if( (*(bd.plSurvPnts))[i] != NULL )
		DebugOutput( 1, "%lf, %lf, %lf\n", (*(bd.plSurvPnts))[i][1], (*(bd.plSurvPnts))[i][2], (*(bd.plSurvPnts))[i][3] );
	    else
		DebugOutput( 1, "(NULL)\n" ); 
	}
    }
    else
	DebugOutput( 1, "(NULL)\n" );

    DebugOutput( 1, "dbaPnts = " );
    if( bd.dbaPnts != NULL )
    {
	DebugOutput( 1, "\n" );
	for( i = 0; i < bd.nNumPoints; i++ )
	    DebugOutput( 1, "[%d] = %d:%d\n", i, GET_FNO(bd.dbaPnts[i]), GET_RNO(bd.dbaPnts[i]) );
    }
    else
	DebugOutput( 1, "(NULL)\n" );
	
    DebugOutput( 1, "dbaCalcPnts = " );
    if( bd.dbaCalcPnts != NULL )
    {
	DebugOutput( 1, "\n" );
	for( i = 0; i < bd.nNumPoints; i++ )
	    DebugOutput( 1, "[%d] = %d:%d\n", i, GET_FNO(bd.dbaCalcPnts[i]), GET_RNO(bd.dbaCalcPnts[i]) );
    }
    else
	DebugOutput( 1, "(NULL)\n" );
#endif
}

void print_rotation( VECTOR v )
{
    VECTOR s = vector( 1, 3 );
    VECTOR tmRot = decomp_matrix( v, s );

    Output( 0, "        ortho measure = %f, %f, %f\n", s[1], s[2], s[3] );

#if FALSE
    float temp[4][4];
    int i, j;
    
    temp[0][0] = v[1];
    temp[0][1] = v[2];
    temp[0][2] = v[3];
    
    temp[1][0] = v[4];
    temp[1][1] = v[5];
    temp[1][2] = v[6];

    temp[2][0] = v[7];
    temp[2][1] = v[8];
    temp[2][2] = v[9];

    temp[3][0] = 0.0;
    temp[3][1] = 0.0;
    temp[3][2] = 0.0;
    
    temp[0][3] = v[10];
    temp[1][3] = v[11];
    temp[2][3] = v[12];
    
    temp[3][3] = 1.0;

    SbMatrix inventor_mat( temp );
    SbMatrix r, u, pr;
    SbVec3f s, t;
    
    inventor_mat = inventor_mat.transpose();
    inventor_mat.factor( r, s, u, t, pr );

    Output( 0, "        ortho measure = %f, %f, %f\n", s[0], s[1], s[2] );

fprintf( stderr, "inventor_mat = \n" );
inventor_mat.print( stderr );
fprintf( stderr, "\n" );

    fprintf( stderr, "r = \n" );
    r.print( stderr );
    fprintf( stderr, "s = %f, %f, %f\n", s[0], s[1], s[2] );
    fprintf( stderr, "u = \n" );
    u.print( stderr );
    fprintf( stderr, "t = %f, %f, %f\n", t[0], t[1], t[2] );
    fprintf( stderr, "pr = \n" );
    pr.print( stderr );
#endif
}
