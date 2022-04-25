//imgoverlay.cpp
//
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
*	05/12/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#include "stdafx.h"
#include "fotobdoc.h"
#include "fotobrow.h"

#define FUNC_DEBUG

#define PAUSEHERE() printf( "pausing at LOCATION: %s.%d\n", __FILE__, __LINE__ );getchar();

#define FAR_CLIPPING_PLANE 75000.0
#define RAY_LENGTH 100000.0

#include "photog.h"
#include "fotobvw.h"
#include "output.h"
#include "common.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include "../3Dclass/PtgCamera.h"
#include "../3Dclass/Ptg3DLineObj.h"
#include "../3Dclass/Ptg3DCylinderObj.h"

int CorrectPnts( char *pszImageName, int nNumPnts, 
	PNTLST plUncorrPnts, PNTLST & plCorrPnts, double reductionFactor );
Ptg3DLineObj *createRayElement( char *theImageName, long imageXPos, long imageYPos, 
			double reductionFactor, Ptg3DLineObj *theRay=NULL  );

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
*	06/07/95 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
BOOL FindTMatrix( struct tmatrix *matrix, const char *name )
{
	long count=0;
	int retval = FALSE;
	if( dt_keyfind( IMAGES_SZNAME, (const char *)name, TASKER ) == S_OKAY )
	{
		dt_setor( IMAGES_TMATRIX, TASKER );
		int q = dt_findfm( IMAGES_TMATRIX, TASKER );
		while( q == S_OKAY )
		{
			long temp;
			dt_setmm( BUNDLE_TMATRIX, IMAGES_TMATRIX, TASKER);
			dt_setro( BUNDLE_TMATRIX, TASKER );
			dt_members( BUNDLE_TMATRIX, &(temp), TASKER);
			if( temp > count )
			{
				count = temp;
				retval = TRUE;
				dt_setrm( IMAGES_TMATRIX, TASKER );
				dt_recread( matrix, TASKER );
			}

			q = dt_findnm( IMAGES_TMATRIX, TASKER );
		}
	}
	return retval;
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
*	06/07/95 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/


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
*	06/07/95 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "redrawZoomViewOverlay()"
void redrawZoomViewOverlay( CFotobrowView *view, Ptg3DNode *theOverlay )
{
//    if( !bWantOverlay )
//		return;
//return;
//    initOverlayElems();
    LOCK_REQUEST lr[] = {
	{ IMAGES, 'r' },
	{ CAMERA_INFO, 'r' },
	{ TMATRIX, 'r' },
	{ BUNDLE, 'r' },
    };
    
    struct tmatrix theTMatrixRec;
//    struct bundle theBundleRec;
    struct images theImageRec;
    struct camera_info theCameraRec;
    int i;

//	int CheckLocks(DB_TASK *task, int dbn);
//	CheckLocks(TASKER);
    if( rdmGroupLock( 4, lr, TASKER ) != S_OKAY )
    {
		ErrorOutput( "Unable to lock records.\n" );
		return;
    }
	
	CFotobrowDoc *doc = (CFotobrowDoc *)(view->GetDocument());
	ASSERT(doc);
    if( dt_keyfind( IMAGES_SZNAME, doc->szName, TASKER ) != S_OKAY )
    {
		ErrorOutput( "Unable to find image record.\n" );
		rdmGroupFree( 4, lr, TASKER );
		return;
    }

    dt_recread( &theImageRec, TASKER );
    dt_setmr( CAMERA_INFO_IMAGES, TASKER );
    dt_setro( CAMERA_INFO_IMAGES, TASKER );
    dt_recread( &theCameraRec, TASKER );

	VECTOR vTMatrix;
	if( FindTMatrix( &theTMatrixRec, (const char *)doc->szName ) )
	{
		vTMatrix = vector(1,13);
		for( i = 0; i < 12; i++ )
		    vTMatrix[i+1] = theTMatrixRec.dfCameraPos[i]; 

	}
	else
	{
		ErrorOutput( "Unable to find bundle record.\n" );
		rdmGroupFree( 4, lr, TASKER );
		return;
	}
	
    if( vTMatrix == NULL )
    {
		rdmGroupFree( 4, lr, TASKER );
		return;
    }

    rdmGroupFree( 4, lr, TASKER );

	CPoint pt = doc->GetView()->GetSelected();
	TRACE("The point selected is %d, %d\n", pt.x, pt.y);
	double reductionFactor = ((CFotobrowApp*)AfxGetApp())->m_imageReduction;
	
//	pt.x *= reductionFactor;
//	pt.y *= reductionFactor;

//	for(i=0; i<6; i++ )
//		theImageRec.dfFilmCorr[i]*=reductionFactor;
//	for(i=0; i<6; i++ )
	theImageRec.dfFilmCorr[0]*=reductionFactor;
	theImageRec.dfFilmCorr[1]*=reductionFactor;
	theImageRec.dfFilmCorr[3]*=reductionFactor;
	theImageRec.dfFilmCorr[4]*=reductionFactor;

    VECTOR vInv = matinv( vTMatrix );

    VECTOR vFilmCorr = vector( 1, 12 );
    vFilmCorr[1] = theImageRec.dfFilmCorr[0];
    vFilmCorr[2] = theImageRec.dfFilmCorr[1];
    vFilmCorr[3] = 0.0;
    vFilmCorr[4] = theImageRec.dfFilmCorr[3];
    vFilmCorr[5] = theImageRec.dfFilmCorr[4];
    vFilmCorr[6] = 0.0;
    vFilmCorr[7] = 0.0;
    vFilmCorr[8] = 0.0;
    vFilmCorr[9] = 1.0;
    vFilmCorr[10] = theImageRec.dfFilmCorr[2];
    vFilmCorr[11] = theImageRec.dfFilmCorr[5];
    vFilmCorr[12] = 0.0;
    VECTOR vInvFilmCorr = matinv( vFilmCorr );

//	for(i=0; i<6; i++ )
//		theImageRec.dfFilmCorr[i]/=(reductionFactor);

    PNTLST plUncorrPnts = matrix( 1, 1, 1, 3 );
    PNTLST plCorrPnts;
//////////////////
	(*plUncorrPnts)[1][1] = (PHOTO_FLOAT)pt.x;
	(*plUncorrPnts)[1][2] = (PHOTO_FLOAT)pt.y;
/////////////////
    (*plUncorrPnts)[1][3] = 0;
////////////////////////////////////////////
#ifdef WOWOWOWOOWOWOW
    CorrectPnts( ivi->szName, 1, plUncorrPnts, plCorrPnts );
#else //WOWOWOWOOWOWOW
	/* set film correction vector */
	VECTOR vFilmCorr2 = vector( 1, 6 );
	for( i = 0; i < 6; i++ )
	    vFilmCorr2[i+1] = theImageRec.dfFilmCorr[i]/**reductionFactor*/;
		
		
	/* set radial correction factor vector */
	VECTOR vRadCorrFact = vector( 1, 9 );
	for( i = 0; i < 9; i++ )
		vRadCorrFact[i+1] = theCameraRec.dfRadCorrFact[i];
		    
	/* Do film correction on pixel coordinates */
	plCorrPnts = corrfilmpnt( 1, theCameraRec.dfFocalLength, 
		vFilmCorr2, plUncorrPnts, vRadCorrFact );
//	copy_vector( (*plPnts)[snCurrPnt], 1, 3, (*plTemp)[1], 1, 3 );  
				
	//free_matrix( plUncorrPnts, 1, 9, 1, 2 );
	free_vector( vFilmCorr2, 1, 6 );
	free_vector( vRadCorrFact, 1, 3 );

#endif //WOWOWOWOOWOWOW
////////////////////////////////////////////
    (*plCorrPnts)[1][1] *= theCameraRec.dfFocalLength/(*plCorrPnts)[1][3];
    (*plCorrPnts)[1][2] *= theCameraRec.dfFocalLength/(*plCorrPnts)[1][3];
    (*plCorrPnts)[1][3] = theCameraRec.dfFocalLength;

    double xcu = vFilmCorr[1]*(*plUncorrPnts)[1][1] + 
			vFilmCorr[2]*(*plUncorrPnts)[1][2] + vFilmCorr[10];
    double ycu = vFilmCorr[4]*(*plUncorrPnts)[1][1] + 
			vFilmCorr[5]*(*plUncorrPnts)[1][2] + vFilmCorr[11];

    double dxf = xcu - (*plCorrPnts)[1][1];
    double dyf = ycu - (*plCorrPnts)[1][2];

    double dxp = vInvFilmCorr[1]*dxf + vInvFilmCorr[2]*dyf;
    double dyp = vInvFilmCorr[4]*dxf + vInvFilmCorr[5]*dyf;
    
	
    vInvFilmCorr[10] -= (double)(doc->GetDIB()->GetWidth() /** 
				reductionFactor*/)/2.0 - dxp;
    vInvFilmCorr[11] -= (double)(doc->GetDIB()->GetHeight() /** 
				reductionFactor*/)/2.0 - dyp;

    GLdouble glInvTMatrix[16];
    glInvTMatrix[0] = vInvFilmCorr[1];
    glInvTMatrix[1] = vInvFilmCorr[4];
    glInvTMatrix[2] = vInvFilmCorr[7]; 
    glInvTMatrix[3] = 0.0;
    glInvTMatrix[4] = vInvFilmCorr[2];
    glInvTMatrix[5] = vInvFilmCorr[5];
    glInvTMatrix[6] = vInvFilmCorr[8];
    glInvTMatrix[7] = 0.0;
    glInvTMatrix[8] = vInvFilmCorr[3]; 
    glInvTMatrix[9] = vInvFilmCorr[6];
    glInvTMatrix[10] = vInvFilmCorr[9];
    glInvTMatrix[11] = 0.0;
    glInvTMatrix[12] = vInvFilmCorr[10];
    glInvTMatrix[13] = vInvFilmCorr[11]; 
    glInvTMatrix[14] = vInvFilmCorr[12];
    glInvTMatrix[15] = 1.0;

    GLdouble thePerspective[16];
    memset( thePerspective, 0, sizeof(thePerspective) );
    
    double k = FAR_CLIPPING_PLANE/(-theCameraRec.dfFocalLength);
    thePerspective[0] = -theCameraRec.dfFocalLength;
    thePerspective[5] = -theCameraRec.dfFocalLength;
    thePerspective[10] = -(k+1.0)/(k-1.0);
    thePerspective[11] = -1.0;
    thePerspective[14] = -(2*k*(-theCameraRec.dfFocalLength))/(k-1.0);

    double scaleFactorX = 2.0/(doc->GetDIB()->GetWidth()/* * reductionFactor*/);
    double scaleFactorY = 2.0/(doc->GetDIB()->GetHeight()/* * reductionFactor*/);

    GLdouble theViewScaleXLate[16];
    memset( theViewScaleXLate, 0, sizeof(theViewScaleXLate) );

    theViewScaleXLate[0] = scaleFactorX;
    theViewScaleXLate[5] = scaleFactorY;
    theViewScaleXLate[10] = 1.0;
    theViewScaleXLate[15] = 1.0;

    GLdouble theTMatrix[16];
    theTMatrix[0] = vTMatrix[1];
    theTMatrix[1] = vTMatrix[4];
    theTMatrix[2] = vTMatrix[7]; 
    theTMatrix[3] = 0.0;
    theTMatrix[4] = vTMatrix[2];
    theTMatrix[5] = vTMatrix[5];
    theTMatrix[6] = vTMatrix[8];
    theTMatrix[7] = 0.0;
    theTMatrix[8] = vTMatrix[3]; 
    theTMatrix[9] = vTMatrix[6];
    theTMatrix[10] = vTMatrix[9];
    theTMatrix[11] = 0.0;
    theTMatrix[12] = vTMatrix[10];
    theTMatrix[13] = vTMatrix[11]; 
    theTMatrix[14] = vTMatrix[12];
    theTMatrix[15] = 1.0;

    // ivi->xpos
    // ivi->ypos
	CRect rect;
	view->GetClientRect(&rect);
    int nViewWidth = rect.Width();
    int nViewHeight = rect.Height();
    
    double centerX = /*reductionFactor**/(PHOTO_FLOAT)pt.x 
		- 0.5*(doc->GetDIB()->GetWidth()/**reductionFactor*/);
    double centerY = /*reductionFactor**/(PHOTO_FLOAT)pt.y 
		- 0.5*(doc->GetDIB()->GetHeight()/**reductionFactor*/);
    
    scaleFactorX = (2.0*(view->m_scale))/nViewWidth;
    scaleFactorY = (2.0*(view->m_scale))/nViewHeight;

    memset( theViewScaleXLate, 0, sizeof(theViewScaleXLate) );

    theViewScaleXLate[0] = scaleFactorX;
    theViewScaleXLate[5] = scaleFactorY;
    theViewScaleXLate[10] = 1.0;
    theViewScaleXLate[15] = 1.0;
    theViewScaleXLate[12] = -scaleFactorX*centerX;
    theViewScaleXLate[13] = -scaleFactorY*centerY;

    free_matrix( plUncorrPnts, 1, 1, 1, 3 );
    free_matrix( plCorrPnts, 1, 1, 1, 3 );

/* Actual drawing portion */
//    setViewport( ivi->rctZoom[nZoomID]->getXMin(), ivi->rctZoom[nZoomID]->getXMax(),
//	    ivi->rctZoom[nZoomID]->getYMin(), ivi->rctZoom[nZoomID]->getYMax() );

    glPushMatrix();
    glLoadIdentity();
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();

    glMultMatrixd( theViewScaleXLate );
    glMultMatrixd( glInvTMatrix );
    glMultMatrixd( thePerspective );

    glMatrixMode( GL_MODELVIEW );
    
    glMultMatrixd( theTMatrix );

#if FALSE
    if( theOverlayDrawlist == 0 )
    {
	theOverlayDrawlist = glGenLists( 1 );
	glNewList( theOverlayDrawlist, GL_COMPILE_AND_EXECUTE );
	theOverlay->draw();
	glEndList();
    }
    else
	glCallList( theOverlayDrawlist );
#else
	TRACE("Calling the Overlay->draw\n");

//	jwb 05/13/97 - attempting to get entire world to draw everywhere
//	GETWORLD.draw();
    theOverlay->draw();
#endif

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    free_vector( vInv, 1, 12 );
    free_vector( vFilmCorr, 1, 12 );
    free_vector( vInvFilmCorr, 1, 12 );
	free_vector( vTMatrix, 1, 13 );
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
*	06/07/95 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "updateRay()"
void updateRay( Ptg3DNode *theNode, char *szName, int bIsSelected, long lXPos, 
			   long lYPos, double reductionFactor )
{
    Ptg3DNode *rayNode = theNode->findByName( "RAYS", FALSE );

    if( rayNode == NULL )
    {
		rayNode = new Ptg3DNode;
		theNode->addMember( rayNode );
		rayNode->setDrawStyle( Ptg3DNode::DS_WIREFRAME );
		rayNode->setColor( 1.0, 0.0, 0.0 );
		rayNode->setName( "RAYS" );
    }

    Ptg3DLineObj *imageRayNode = (Ptg3DLineObj *)(rayNode->findByName( szName, FALSE ));

    if( bIsSelected )
    {
		if( imageRayNode == NULL )
		{
			imageRayNode = createRayElement( szName, lXPos, lYPos, reductionFactor );
			if( imageRayNode != NULL )
			{
				rayNode->addMember( imageRayNode );
				imageRayNode->setName( szName );
			}
		}
		else
			createRayElement( szName, lXPos, lYPos, reductionFactor, imageRayNode  );

		if( imageRayNode != NULL )
			imageRayNode->setDrawStyle( Ptg3DNode::DS_WIREFRAME );
	}
	else
	{
		if( imageRayNode != NULL )
		{
			POSITION pos = rayNode->getMemberList().Find(imageRayNode);
			Ptg3DLineObj *tempNode = (Ptg3DLineObj *)(rayNode->getMemberList().GetAt(pos));

			tempNode->setDrawStyle( Ptg3DNode::DS_INVISIBLE );

	//	    rayNode->getMemberList().RemoveAt( pos );
	//	    delete imageRayNode;
		}
    }
//    printHierarchyNode( theNode, 0 );
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
*	06/07/95 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "createRayElement()"
Ptg3DLineObj *createRayElement( char *theImageName, long imageXPos, long imageYPos, 
			double reductionFactor,  Ptg3DLineObj *theRay )
{
    LOCK_REQUEST lr[] = {
	{ IMAGES, 'r' },
	{ TMATRIX, 'r' },
	{ CAMERA_INFO, 'r' }
    };
    struct tmatrix theTMatrix;
//	struct images theImageRec;
    int i;

//	int CheckLocks(DB_TASK *task, int dbn);
//	CheckLocks(TASKER);
    if( rdmGroupLock( 3, lr, TASKER ) != S_OKAY )
    {
		ErrorOutput( "Unable to lock records.\n" );
		return( NULL );
    }

    VECTOR vTMatrix = vector( 1, 13 );
    PNTLST plUncorrPnts = matrix( 1, 1, 1, 3 );
    PNTLST plCorrPnts;

    (*plUncorrPnts)[1][1] = imageXPos;
    (*plUncorrPnts)[1][2] = imageYPos;
    (*plUncorrPnts)[1][3] = 0;

	if( !FindTMatrix( &theTMatrix, (const char *)theImageName ) )
	{
		rdmGroupFree( 3, lr, TASKER );
		return NULL;
	}


//    if( dt_keyfind( IMAGES_SZNAME, theImageName, TASKER ) != S_OKAY )
//		fprintf( stderr, "Couldn't find image record for %s\n", theImageName );
  //  if( dt_recread( &theImageRec, TASKER ) != S_OKAY )
//		fprintf( stderr, "Couldn't read image record for %s\n", theImageName );
//	VECTOR vFilmCorrMatrix = vector( 1, 12 );
//	for( i = 0; i < 12; i++ )
//	    vFilmCorrMatrix[i+1] = theImageRec.dfFilmCorr[i];
//	print_vector( vFilmCorrMatrix, 1, 12 );
//    dt_setor( IMAGES_TMATRIX, TASKER );
  //  dt_findfm( IMAGES_TMATRIX, TASKER );
	//dt_findnm( IMAGES_TMATRIX, TASKER );

//    dt_recread( &theTMatrix, TASKER );

    for( i = 0; i < 12; i++ )
		vTMatrix[i+1] = theTMatrix.dfCameraPos[i];

    CorrectPnts( theImageName, 1, plUncorrPnts, plCorrPnts, reductionFactor );

    /* Invert tmatrix */
    VECTOR vInvTMatrix = matinv( vTMatrix );

    /* scale corrected point */
    for( i = 1; i <= 3; i++ )
		(*plCorrPnts)[1][i] *= RAY_LENGTH;

    if( theRay == NULL )
	theRay = new Ptg3DLineObj;
    theRay->setName( theImageName );
    theRay->setColor( 1.0, 0.0, 0.0 );
    theRay->setOrigin( vInvTMatrix[10], vInvTMatrix[11], vInvTMatrix[12] );
//    theRay->setOrigin( 12682, 17959, -14423 );
    theRay->setEndPoint(
	    vInvTMatrix[1]*(*plCorrPnts)[1][1] + vInvTMatrix[2]*(*plCorrPnts)[1][2] + vInvTMatrix[3]*(*plCorrPnts)[1][3] + vInvTMatrix[10], 
	    vInvTMatrix[4]*(*plCorrPnts)[1][1] + vInvTMatrix[5]*(*plCorrPnts)[1][2] + vInvTMatrix[6]*(*plCorrPnts)[1][3] + vInvTMatrix[11], 
	    vInvTMatrix[7]*(*plCorrPnts)[1][1] + vInvTMatrix[8]*(*plCorrPnts)[1][2] + vInvTMatrix[9]*(*plCorrPnts)[1][3] + vInvTMatrix[12] );

    free_vector( vTMatrix, 1, 13 );
    free_vector( vInvTMatrix, 1, 13 );
    free_matrix( plUncorrPnts, 1, 1, 1, 3 );
    free_matrix( plCorrPnts, 1, 1, 1, 3 );
//	free_vector( vFilmCorrMatrix, 1, 12 );

    rdmGroupFree( 3, lr, TASKER );

    return( theRay );
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
*	05/12/93 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "CorrectPnts"
int CorrectPnts( char *pszImageName, int nNumPnts, 
	PNTLST plUncorrPnts, PNTLST & plCorrPnts, double reductionFactor )
{
    struct images image_rec;
    struct camera_info caminf_rec;
    VECTOR vFilmCorr, vRadCorrFact;
    int i;
/*//    static LOCK_REQUEST lr[] = {
//	{ IMAGES, 'r' }, 
//	{ CAMERA_INFO, 'r' }, 
  //  };

    if( rdmGroupLock( 2, lr, dbn ) != S_OKAY )
    {
	fprintf( stderr, "%s: Unable to lock records.\n", FUNC );
	return( !OKAY );
    }
*/    
    memset( &image_rec, 0, sizeof(struct images) );
    strcpy( image_rec.szName, pszImageName );
    int db_status = dt_keyfind( IMAGES_SZNAME, &(image_rec.szName), TASKER );
    if( db_status != S_OKAY )
    {
		fprintf( stderr, "%s; couldn't find image record\n", FUNC );
//		rdmGroupFree( 2, lr, dbn );
		return( !S_OKAY );
    }

    if( dt_recread( &image_rec, TASKER ) != S_OKAY ||
	dt_setmr( CAMERA_INFO_IMAGES, TASKER ) != S_OKAY ||
	dt_setro( CAMERA_INFO_IMAGES, TASKER ) != S_OKAY ||
	dt_recread( &caminf_rec, TASKER ) != S_OKAY )
    {
		fprintf( stderr, "%s: Error reading database\n", FUNC );
//		rdmGroupFree( 2, lr, dbn );
		return( !S_OKAY );
    }
    
    vFilmCorr = vector( 1, 6 );

	image_rec.dfFilmCorr[0]*=reductionFactor;
	image_rec.dfFilmCorr[1]*=reductionFactor;
	image_rec.dfFilmCorr[3]*=reductionFactor;
	image_rec.dfFilmCorr[4]*=reductionFactor;
    for( i = 0; i < 6; i++ )
		vFilmCorr[i+1] = image_rec.dfFilmCorr[i];
	
    vRadCorrFact = vector( 1, 9 );
    for( i = 0; i < 9; i++ )
	vRadCorrFact[i+1] = caminf_rec.dfRadCorrFact[i];
	
    plCorrPnts = corrfilmpnt( nNumPnts, caminf_rec.dfFocalLength, 
	    vFilmCorr, plUncorrPnts, vRadCorrFact );
	    
    free_vector( vFilmCorr, 1, 6 );
    free_vector( vRadCorrFact, 1, 3 );
	    
	
//    rdmGroupFree( 2, lr, dbn );
    return( S_OKAY );
}
#undef FUNC

int CheckLocks(DB_TASK *task, int dbn)
{
	char status;
	char temp[256];
	char *strings[] = {
"POINTS",
"IMAGES",
"PIXELS",
"XYZ"
"BUNDLE",
"CAMERA_INFO",
"FIDUCIAL",
"TMATRIX"};

	for( int i=POINTS; i<10008; i++)
	{
		dt_reclstat( i, &status, task, dbn);
		if( status != 'f' )
		{
			sprintf(temp,"Lock Not Free: %s, %c", strings[i-10000], status );
			AfxMessageBox(temp);
		}
	}
	return 0;
}
