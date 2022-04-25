// PtgWorld.cpp
//
#ifdef WIN32
#include "stdafx.h"
#endif

#include "common.h"

#include "fotobrow.h"

#include "../3Dclass/Ptg3DPyramidObj.h"
#include "../3Dclass/Ptg3DLineObj.h"
#include "../3Dclass/PtgCamera.h"
#include "../3Dclass/Ptg3DCylinderObj.h"
#include "../3Dclass/Ptg3DBoxObj.h"
#include "../3Dclass/Ptg3DPointObj.h"

#include "photog.h"
#include "../photog/output.h"
#include <vista.h>

#include "ptgworld.h"

ID_NAME(PtgWorld);
VECTOR vectorTMatrixTransposeRotation( VECTOR v );

PtgWorld::PtgWorld()
{
	theSystem = NULL;
	theModel = NULL;
	theReview = NULL;

	theCameras = NULL;
	theAxes = NULL;
	theTargets = NULL;
	theReference = NULL;
	theRays = NULL;
	theCalculated = NULL;

	setName("World");
	setDrawStyle(Ptg3DNode::DS_INVISIBLE);
	setColor( 0.0, 1.0, 0.0 );
	setActive(TRUE);
}

PtgWorld::~PtgWorld()
{
/*	if( theCameras ) //busby fix for nodes added to *this
	{
		delete theCameras;
		theCameras = NULL;
	}
	if( theAxis )
	{
		delete theAxis;
		theAxis = NULL;
	}
	if( theModel )
	{
		delete theModel;
		theModel = NULL;
	}
	if( theRays )
	{
		delete theRays;
		theRays = NULL;
	}*/
}

DWORD PtgWorld::InitWorld(void *word)
{
	if( theCameras )
		return FALSE;
	CreateSystem(this);
	CreateReview(this);
	CreateModel(this);

	HANDLE hEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, "WaitFor3DWorld");
	SetEvent(hEvent);
	
	return TRUE;
}

int PtgWorld::CreateSystem( Ptg3DNode *parent )
{
	if( !parent )
		parent = this;

	if( theSystem )
		return FALSE;

	theSystem = new Ptg3DNode;
	theSystem->setName("System");
	theSystem->setDrawStyle(Ptg3DNode::DS_INVISIBLE);
	theSystem->setColor( 0.0, 1.0, 0.0 );
	theSystem->setActive(TRUE);
	parent->addMember(theSystem);

	CreateCameras(theSystem);
	CreateTargets(theSystem);
	CreateAxes(theSystem);
	CreateRays(theSystem);
	CreateReference(theSystem);

	return TRUE;
}

int PtgWorld::CreateTargets( Ptg3DNode *parent )
{
	if( theTargets )
		return FALSE;

	if( !parent )
		parent = this;

	theTargets = new Ptg3DNode;
	theTargets->setName("Survey");
	theTargets->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
	theTargets->setColor( 0.0, 1.0, 0.0 );
	theTargets->setActive(TRUE);
	parent->addMember(theTargets);

	rdmRecLock( XYZ, "r", TASKER );
	rdmRecLock( POINTS, "r", TASKER );
	struct points pt;
	struct xyz survey;
	int numTargets = 0;
	int status = dt_recfrst( POINTS, TASKER );
	while( status == S_OKAY )
	{
		dt_recread( &pt, TASKER );
		if( pt.cType == 'S' || pt.cType == 's' )
		{
			dt_setor( POINTS_SURVEY_XYZ, TASKER );
			dt_findlm( POINTS_SURVEY_XYZ, TASKER );
			dt_recread( &survey, TASKER );

			Ptg3DPointObj *pPoint = new Ptg3DPointObj;
			pPoint->setName(pt.szLabel);
			pPoint->setDrawStyle( Ptg3DNode::DS_NONE );
			pPoint->setActive(FALSE);
			pPoint->setColor( 0.0, 1.0, 0.5 );
			pPoint->setPosition( survey.dfCoor[0], survey.dfCoor[1], survey.dfCoor[2], NULL );
			theTargets->addMember(pPoint);

			dt_setro( POINTS_SURVEY_XYZ, TASKER );

			numTargets++;
		}
		status = dt_recnext( TASKER );
	} 

	TRACE( "added %d targets\n", numTargets );

	rdmRecFree(XYZ, TASKER );
	rdmRecFree(POINTS, TASKER);

	return TRUE;
}

int PtgWorld::CreateReference( Ptg3DNode *parent )
{
	if( theReference )
		return FALSE;

	if( !parent )
		parent = this;

	theReference = new Ptg3DNode;
	theReference->setName("Measurement References");
	theReference->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
	theReference->setColor( 0.0, 1.0, 0.0 );
	theReference->setActive(TRUE);
	parent->addMember(theReference);

	// Create node to represent the calculated coordinate system
	Ptg3DNode *measureXform = new Ptg3DNode;
	measureXform->setName( "Default" );
	measureXform->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
	measureXform->setColor( 0.0, 1.0, 0.0 );
	measureXform->setActive(TRUE);

	VECTOR v1 = ((CFotobrowApp *)AfxGetApp())->m_Transform - 1;
	VECTOR v2 = matinv(v1);
	measureXform->setMatrix( v2 );
#if FALSE
	VECTOR v2 = vector( 1, 12 );
	v2[1] = 1.0;	v2[2] = 0.0;	v2[3] = 0.0;
	v2[4] = 0.0;	v2[5] = 0.0;	v2[6] = 1.0;
	v2[7] = 0.0;	v2[8] = -1.0;	v2[9] = 0.0;
	v2[10] = 0.0;	v2[11] = 0.0;	v2[12] = 0.0;

	VECTOR v4 = matinv( v1 );

	measureXform->setMatrix( v4 );

	free_vector( v4, 1, 12 );
	free_vector( v2, 1, 12 );
#endif

	theReference->addMember( measureXform  );
	((CFotobrowApp *)AfxGetApp())->SetReferenceNode(measureXform);

	return TRUE;
}

int PtgWorld::CreateAxes( Ptg3DNode *parent )
{
	if( theAxes )
		return FALSE;

	if( !parent )
		parent = this;

	theAxes = new Ptg3DNode;
	theAxes->setName("Axes");
	theAxes->setDrawStyle(Ptg3DNode::DS_INVISIBLE);
	theAxes->setColor( 0.0, 1.0, 0.0 );
	theAxes->setActive(FALSE);
	parent->addMember(theAxes);
	
	Ptg3DLineObj *line = new Ptg3DLineObj();
	line->setOrigin(0,0,0);
	line->setEndPoint(25000,0,0);
	line->setColor(1.0,0.0,0.0,1.0);
	line->setName("X-axis");
	line->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
	theAxes->addMember(line);

	line = new Ptg3DLineObj();
	line->setOrigin(0,0,0);
	line->setEndPoint(0,25000,0);
	line->setColor(0.0,1.0,0.0,1.0);
	line->setName("Y-axis");
	line->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
	theAxes->addMember(line);

	line = new Ptg3DLineObj();
	line->setOrigin(0,0,0);
	line->setEndPoint(0,0,25000);
	line->setColor(0.0,0.0,1.0,1.0);
	line->setName("Z-axis");
	line->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
	theAxes->addMember(line);

//	Ptg3DPyramidObj *pyr = new Ptg3DPyramidObj;
//	pyr->setBaseDims(100,500);
//	pyr->setHeight(1000);
//	pyr->setColor(1.0,1.0,1.0,1.0);
//	pyr->setName("Pyramid 1");
//	pyr->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
//	theAxes->addMember(pyr);

	return TRUE;
}

int PtgWorld::CreateCameras( Ptg3DNode *parent )
{
	if( theCameras )
		return FALSE;

	if( !parent )
		parent = this;

	int r;
    static LOCK_REQUEST lr[] = {
	{ BUNDLE, 'r' }, 
	{ TMATRIX, 'r' }, 
	{ IMAGES, 'r' }, 
	{ PIXELS, 'r' }, 
	{ POINTS, 'r' }, 
	{ XYZ, 'r' }
    };
    struct bundle theBundle;
    struct images theImage;
    int nNumCamerasWritten = 0;
    VECTOR tm;

	theCameras = new Ptg3DNode;
	parent->addMember(theCameras);

//	int CheckLocks(DB_TASK *task, int dbn);
//	CheckLocks(TASKER);
    if( rdmGroupLock( 6, lr, TASKER ) != S_OKAY )
    {
		fprintf( stderr, "Unable to read lock records\n" );
		AfxMessageBox("Could not lock record");
		exit(1);
    }

    tm = vector( 1, 13 ); 
	
//	theCameras = new Ptg3DNode;
	theCameras->setName("Cameras");
	theCameras->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
	theCameras->setColor( 0.0, 1.0, 0.0 );
	theCameras->setActive(TRUE);
	tm[1] = tm[8] = 1.0;
	tm[6] = -1.0;
	tm[2] = tm[3] = tm[4] = tm[5] = tm[7] = tm[9] = 0.0;
//	theCameras->setMatrix(tm);
	//theCamera->addMember(theCameraNode);
    r=dt_keyfrst( BUNDLE_SZTAG, TASKER );
    while( r == S_OKAY )
    {
		dt_recread( &theBundle, TASKER );
		dt_setor( BUNDLE_TMATRIX, TASKER );
		int r2 = dt_findfm( BUNDLE_TMATRIX, TASKER );
		while( r2 == S_OKAY )
		{
			dt_setmm( IMAGES_TMATRIX, BUNDLE_TMATRIX, TASKER );
			dt_setro( IMAGES_TMATRIX, TASKER );
			dt_recread( &theImage, TASKER );

			DB_ADDR_P ctab;
			int csize;
			dt_rdcurr( &ctab, &csize, TASKER );
			if( !FindTMatrix( theBundle.szTag, theImage.szName, tm ) )
			{
				double total = 0.0;
				for( int i = 1; i < 13; i++ )
					total += tm[i];
				if( total == 0.0 )
					TRACE( "%s - %s: Invalid tmatrix.\n", theBundle.szTag, theImage.szName );
				else
				{
					Ptg3DNode *newNode;
					if( !(newNode = theCameras->findByName(theImage.szName, TRUE)) )
					{	
						newNode = new Ptg3DNode;
						VECTOR ret = ConvertCameraMatrix(tm+1);
						// Yet another wad of code that got screwed when Ptg3DNode::setMatrix started transposing rotation
						VECTOR v2 = vectorTMatrixTransposeRotation(ret);
						free_vector(ret,1,12);
						ret = v2;
						newNode->setMatrix( ret );
						newNode->setName(theImage.szName);			
						newNode->setActive(TRUE);
						free_vector( ret, 1, 12 );
						theCameras->addMember( newNode );
					}
					Ptg3DPyramidObj *thePyra = new Ptg3DPyramidObj;
					thePyra->setHeight( 600.0 );
					thePyra->setBaseDims( 500.0, 500.0*0.75 );
					thePyra->setTopDims( 0.0, 0.0 );
					thePyra->setOffsets( 0.0, 0.0 );

					VECTOR v = vector( 1, 12 );
/*
					v[1] = 1.0;
					v[5] = v[9] = -1.0;
					v[2] = v[3] = v[4] = v[6] = v[7] = v[8] = 0.0;
					v[10] = v[11] = 0.0;
					v[12] = 300.0;
*/
					v[1] = v[5] = v[9] = 1.0;
					v[2] = v[3] = 0.0;
					v[4] = v[6] = 0.0;
					v[7] = v[8] = 0.0;
					v[10] = v[11] = 0.0;
					v[12] = -300.0;
					thePyra->setMatrix( v );

					thePyra->setColor(0.0,1.0,0.2);
//					char temp[128];
//					sprintf(temp,"$s/Pyramid", theImage.szName);
//					thePyra->setName(temp);			
					thePyra->setDrawStyle(Ptg3DNode::DS_WIREFRAME); 
					thePyra->setActive(TRUE);
					free_vector(v,1,13);

					newNode->addMember( thePyra );
					nNumCamerasWritten++;
				}
			}
			else
				fprintf( stderr, "Unable to find image '%s' in bundle '%s'.\n",
					theImage.szName, theBundle.szTag );
			dt_wrcurr( ctab, TASKER );

			r2=dt_findnm( BUNDLE_TMATRIX, TASKER );
		}
		r = dt_keynext( BUNDLE_SZTAG, TASKER );
    }

    rdmGroupFree( 6, lr, TASKER );

    Output( 0, "Wrote %d cameras.\n", nNumCamerasWritten );

    free_vector( tm, 1, 13 );
	return TRUE;
}
int PtgWorld::CreateReview( Ptg3DNode *parent )
{
	if( theReview )
		return FALSE;
	if( !parent )
		parent = this;

	theReview = new Ptg3DNode;
	theReview->setName( "Review Model" );
	theReview->setActive( TRUE );

	VECTOR v = vector( 1, 12 );
	 v[1] = 1.0;   v[2] =  0.0;   v[3] = 0.0;
	 v[4] = 0.0;   v[5] =  0.0;   v[6] = 1.0;
	 v[7] = 0.0;   v[8] = -1.0;   v[9] = 0.0;
	v[10] = 0.0;  v[11] =  0.0;  v[12] = 0.0;

	VECTOR v2 = vector( 1, 12 );
	getSurveyModelTransform(&(v2[1]), ((CFotobrowApp *)AfxGetApp())->m_ProjectName);
	VECTOR v3 = vectorTMatrixTransposeRotation(v2);

	VECTOR v4 = matmat(v,v3);

	theReview->setMatrix(v4);

	free_vector( v, 1, 12 );
	free_vector( v2, 1, 12 );
	free_vector( v3, 1, 12 );
	free_vector( v4, 1, 12 );

	parent->addMember(theReview);

	return TRUE;
}

int PtgWorld::CreateModel( Ptg3DNode *parent )
{
	if( theModel )
		return FALSE;
	if( !parent )
		parent = this;

	Ptg3DNode *theModel2;

	theModel = new Ptg3DNode;
	theModel->setName("Model");
	theModel->setDrawStyle(Ptg3DNode::DS_INVISIBLE);
	theModel->setColor( 0.0, 1.0, 0.0 );
	theModel->setActive(TRUE);
	parent->addMember(theModel);

	return TRUE;
}

int PtgWorld::CreateRays( Ptg3DNode *parent )
{
	if( theRays )
		return FALSE;

	if( !parent )
		parent = this;

	theRays = new Ptg3DNode;
	theRays->setName("Rays");
	theRays->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
	theRays->setColor( 0.0, 1.0, 0.0 );
	theRays->setActive(TRUE);
//	parent->addMember(theRays);
	return TRUE;
}

int PtgWorld::CreateCalculated()
{
	if( theCalculated )
		return FALSE;
	theCalculated = new Ptg3DNode;
	theCalculated->setName("Overlay");
	theCalculated->setDrawStyle(Ptg3DNode::DS_INVISIBLE);
	theCalculated->setColor( 0.0, 1.0, 1.0 );
	theCalculated->setActive(TRUE);
	addMember(theCalculated);//	theCalculated->elemMembers->m_nCount = 0x777;

	return TRUE;
}

#define FUNC "FindTMatrix"
int PtgWorld::FindTMatrix( char *pszBundleID, char *pszImageName, VECTOR vTMatrix )
{
	int r;
    struct tmatrix tmatrix_rec;
    struct images img_rec;
    struct bundle bundle_rec;
    int snFound = FALSE;
    static LOCK_REQUEST lr[] = {
	{ BUNDLE, 'r' },
	{ TMATRIX, 'r' }, 
	{ IMAGES, 'r' }
    };

    
    memset( &bundle_rec, 0, sizeof(struct bundle) );
    strcpy( bundle_rec.szTag, pszBundleID );
    
    snFound = FALSE;
    r = dt_keyfind( BUNDLE_SZTAG, &(bundle_rec.szTag), TASKER );
    if( r == S_OKAY )
    {
	dt_setor( BUNDLE_TMATRIX, TASKER );
	r = dt_findfm( BUNDLE_TMATRIX, TASKER );
	while( r == S_OKAY && !snFound )
	{
	    dt_setmm( IMAGES_TMATRIX, BUNDLE_TMATRIX, TASKER );
	    dt_setro( IMAGES_TMATRIX, TASKER );
	    
	    dt_recread( &img_rec, TASKER );
	    if( strcmp( img_rec.szName, pszImageName ) == 0 )
		snFound = TRUE;
	    else
		dt_findnm( BUNDLE_TMATRIX, TASKER );
	}
    }
    
    if( snFound )
    {
	dt_setrm( IMAGES_TMATRIX, TASKER );
	dt_recread( &tmatrix_rec, TASKER );
	
	for( int i = 0; i < 12; i++ )
	    vTMatrix[i+1] = tmatrix_rec.dfCameraPos[i];

	return( S_OKAY );
    }
    else
    {
	fprintf( stderr, "%s: Could not find image %s in bundle %s\n", 
		FUNC, pszImageName, pszBundleID );
	return( !S_OKAY );
    }
}
#undef FUNC

#define FUNC "WriteCameraToPDMS"
VECTOR PtgWorld::ConvertCameraMatrix( double theMatrix[] )
{
    int i;
    
    VECTOR tm = vector( 1, 13 );
    for( i = 0; i < 12; i++ )
		tm[i+1] = theMatrix[i];
    VECTOR tminv = matinv(tm);

    VECTOR s = vector( 1, 3 );
    VECTOR tmRot = decomp_matrix( tm, s );

    MATRIX *u = matrix( 0, 3, 0, 3 );
	VECTOR ret = vector(1,13);
/*
    ret[1] = tmRot[1];
    ret[2] = -tmRot[3];
    ret[3] = tmRot[2];
    
    ret[4] = -tmRot[4];
    ret[5] = tmRot[6];
    ret[6] = -tmRot[5];

    ret[7] = -tmRot[7];
    ret[8] = tmRot[9];
    ret[9] = -tmRot[8];
    
    ret[10]	= tminv[10];
    ret[11]	= -tminv[12];
    ret[12]	= tminv[11];
*/
	
    ret[1] = tmRot[1];
    ret[2] = tmRot[2];
    ret[3] = tmRot[3];
    
    ret[4] = tmRot[4];
    ret[5] = tmRot[5];
    ret[6] = tmRot[6];

    ret[7] = tmRot[7];
    ret[8] = tmRot[8];
    ret[9] = tmRot[9];
    
    ret[10]	= tminv[10];
    ret[11]	= tminv[11];
    ret[12]	= tminv[12];

    free_vector( tm, 1, 13 );
    free_vector( tminv, 1, 13 );
    free_vector( s, 1, 3 );
    free_vector( tmRot, 1, 13 );
    free_matrix( u, 0, 3, 0, 3 );
    
    return( ret );
}
#undef FUNC

int PtgWorld::CreateColumns()
{
	if( theColumns )
		return FALSE;

	Ptg3DNode *theColumns;

	theColumns = new Ptg3DNode;
	theColumns->setName("Columns");
	theColumns->setDrawStyle(Ptg3DNode::DS_INVISIBLE);
	theColumns->setColor( 0.0, 1.0, 0.0 );
	theColumns->setActive(TRUE);
	addMember(theColumns);
	return TRUE;
}

int PtgWorld::CreateZones()
{
	if( theZones )
		return FALSE;

	Ptg3DNode *theZones;

	theZones = new Ptg3DNode;
	theZones->setName("Zones");
	theZones->setDrawStyle(Ptg3DNode::DS_INVISIBLE);
	theZones->setColor( 0.0, 1.0, 0.0 );
	theZones->setActive(TRUE);
	addMember(theZones);
	return TRUE;
}

