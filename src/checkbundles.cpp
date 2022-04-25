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

//#include "photo.h"
#include "stdafx.h"
#include "common.h"
#include "photog.h"
//#include "utils.h"
//#include "stringUtils.h"
//#include "XUtils.h"
//#include "ptgGlobals.h"
#include "output.h"
#include "checkbundle.h"
#include "photog_math.h"
//#include "profile.h"
#include "mathsubs.h"
#include <assert.h>

int ReadBundleData( const char *pszBundleID, BundleData &bd );
void FreeBundleData( BundleData &bd );

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
*	03/03/95 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
int checkBundle( BundleData & bd )
{
    int bCanCheckOrtho = FALSE;
    int nOrthoLowerResult;
    int nOrthoUpperResult;
    char szOrthoLower[256], szOrthoUpper[256];
    int nCheckBundleResult = CB_OK;
    int nCheckOutputLevel = 0;
    int i, j;
    int *bIsMatrixValid;

    bIsMatrixValid = (int *)malloc( bd.nNumPhotos*sizeof(int) );
    for( i = 0; i < bd.nNumPhotos; i++ )
	bIsMatrixValid[i] = TRUE;

    Output( 0, "Checking bundle '%s'.\n", bd.pszBundleName );

    /* Check for bundles that haven't been run */
    for( i = 0; i < bd.nNumPhotos; i++ )
    {
	if( bd.plTMatrix == NULL || (*(bd.plTMatrix))[i] == NULL )
	{
	    bIsMatrixValid[i] = FALSE;
	    Output( 0, "  image %s has an invalid matrix.\n", bd.pszPhotoNames[i] );
	}
	else
	{
	    double dfMag = 0;
	    for( j = 0; j < 12; j++ )
		dfMag += (*(bd.plTMatrix))[i][j] * (*(bd.plTMatrix))[i][j];
	    if( !(dfMag > 0) )
	    {
		Output( 0, "  image %s has an invalid matrix.\n", bd.pszPhotoNames[i] );
		bIsMatrixValid[i] = FALSE;
	    }
	}
    }

    bundlecheck( bd );

    GetPrivateProfileString( "Tolerances", "ortholower", "", szOrthoLower, sizeof(szOrthoLower), szAppIniFile );
    GetPrivateProfileString( "Tolerances", "ortholower", szOrthoLower, szOrthoLower, sizeof(szOrthoLower), szProjectIniFile );
    nOrthoLowerResult = GetPrivateProfileString( "Tolerances", "ortholower", szOrthoLower, szOrthoLower, sizeof(szOrthoLower), szUserIniFile );

    GetPrivateProfileString( "Tolerances", "orthoupper", "", szOrthoUpper, sizeof(szOrthoUpper), szAppIniFile );
    GetPrivateProfileString( "Tolerances", "orthoupper", szOrthoUpper, szOrthoUpper, sizeof(szOrthoUpper), szProjectIniFile );
    nOrthoUpperResult = GetPrivateProfileString( "Tolerances", "orthoupper", szOrthoUpper, szOrthoUpper, sizeof(szOrthoUpper), szUserIniFile );

    if( nOrthoLowerResult != 0 && nOrthoUpperResult != 0 )
    {
	double dfOrthoLower = atof(szOrthoLower);
	double dfOrthoUpper = atof(szOrthoUpper);
	
	for( i = 0; i < bd.nNumPhotos; i++ )
	{
	    if( bIsMatrixValid[i] )
	    {
		int nIsThisImageOK = CB_OK;
		VECTOR vScales = get_matrix_scales( (*(bd.plTMatrix))[i] );
		for( int j = 1; j <= 3 && nIsThisImageOK == CB_OK; j++ )
		{
		    double dfDelta = fabs(vScales[j] - 1.0);
		    if( dfDelta < dfOrthoLower || dfDelta > dfOrthoUpper )
			nIsThisImageOK = CB_BAD_ORTHO;		     
		}
    
		if( nIsThisImageOK == CB_OK )
		    nCheckOutputLevel = 2;
		else
		{
		    nCheckOutputLevel = 0;
		    nCheckBundleResult = CB_BAD_ORTHO;
		}
		Output( nCheckOutputLevel, "  image %s ortho measure = %lf, %lf, %lf\n", bd.pszPhotoNames[i], vScales[1], vScales[2], vScales[3] );
		free_vector( vScales, 1, 3 );
	    }
	}
    }
    else
	Output( 0, "Unable to check ortho values because of missing upper and/or lower tolerances.\n" );

    free( bIsMatrixValid );

    if( nCheckBundleResult == CB_OK )
	Output( 0, "Bundle %s - OK\n", bd.pszBundleName );
    else
	Output( 0, "Bundle %s - Out of tolerance\n", bd.pszBundleName );

    if( nCheckBundleResult != CB_OK )
	return( nCheckBundleResult );

    return( CB_OK );
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
*	05/16/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "SelectCheckBundleApplyCB"
void SelectCheckBundleApplyCB( Widget wgt, Widget wgtBundlesDialog, XtPointer xtpCallData )
{
    Widget wgtListBox = XtNameToWidget( wgtBundlesDialog, "*selectBundlesListBox" );
    XmString *xmsp;
    int *npPositions;
    int nNumSelected;
    char *pszBundleName;
    static LOCK_REQUEST lr[] = {
	{ BUNDLE, 'r' }, 
	{ TMATRIX, 'r' }, 
	{ IMAGES, 'r' }, 
	{ PIXELS, 'r' }, 
	{ POINTS, 'r' }, 
	{ XYZ, 'r' }
    };
    BundleData bd;
    
    if( wgtListBox )
    {
	nargs = 0;
	XtSetArg( args[nargs], XmNitems, &xmsp ); nargs++;
	XtGetValues( wgtListBox, args, nargs );
	
	XmListGetSelectedPos( wgtListBox, &npPositions, &nNumSelected );

	for( int i = 0; i < nNumSelected; i++ )
	{
	    XmStringGetLtoR( xmsp[npPositions[i]-1], XmSTRING_DEFAULT_CHARSET, &pszBundleName );
	    
	    if( pszBundleName != NULL )
	    {
		if( rdmGroupLock( 6, lr, dbn ) == S_OKAY )
		{
		    memset( &bd, 0, sizeof(BundleData) );
		    bd.pszBundleName = strdup(pszBundleName);

		    if( ReadBundleData( pszBundleName, bd ) )
		    {
			rdmGroupFree( 6, lr, dbn );
			switch( checkBundle( bd ) )
			{
			    case CB_OK:
				break;
			    case CB_BAD_ORTHO:
				break;
			    default:
				Output( 0, "Bundle '%s' - Error checking bundle.\n", pszBundleName );
				break;
			}
			
		    }
		    else
			rdmGroupFree( 6, lr, dbn );

		    FreeBundleData( bd );
		}
		else
		{
		    ErrorOutput( "Database error %d - Unable to read lock records\n", db_status );
		    return;
		}
		XtFree( pszBundleName );
	    }
	}

	XmListDeselectAllItems( wgtListBox );
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
*	03/02/95 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "CheckBundleProc"
void CheckBundlesProc( Widget wgt, char *client_data, caddr_t call_data )
{
    static Widget wgtSelectBundlesForm = NULL;
    Widget wgtSelectBundlesDialog;

    if( !IsMenuEnabled() )
    {
	MessageBox( getTopLevelWidget(), "Menu is not currently active.\n", "Error", MB_OK );
	return;
    }
    
    if( wgtSelectBundlesForm == NULL )
    {
	wgtSelectBundlesForm = CreateBundleSelectionBox( getTopLevelWidget(), "Check Bundles", TRUE );
	if( wgtSelectBundlesForm == NULL )
	{
	    ErrorOutput( "%s: Unable to create Select Bundle dialog.\n", FUNC );
	    return;
	}

	wgtSelectBundlesDialog = XtParent(wgtSelectBundlesForm);
	Widget wgtSelectBundlesApplyButton = XtNameToWidget( wgtSelectBundlesDialog, "*selectBundlesOKButton" );

	XtAddCallback( wgtSelectBundlesApplyButton, XmNactivateCallback, (XtCallbackProc)SelectCheckBundleApplyCB, wgtSelectBundlesDialog );

	XtRealizeWidget( wgtSelectBundlesForm );
    }
    else
    	wgtSelectBundlesDialog = XtParent(wgtSelectBundlesForm);

    Widget wgtSelectBundlesListBox = XtNameToWidget( wgtSelectBundlesDialog, "*selectBundlesListBox" );
    assert( wgtSelectBundlesListBox != NULL );
    FillBundleList( wgtSelectBundlesListBox );
    XtManageChild( wgtSelectBundlesForm );
    
    SetBundleSelectionBoxSize( wgtSelectBundlesDialog );
}
#undef FUNC

