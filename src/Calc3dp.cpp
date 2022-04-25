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
*	05/16/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#include "stdafx.h"

#include "photog.h"
#include "photog_math.h"
#include "output.h"

#include "fotobrow.h"
#include "fotobdoc.h"
#include "fotobvw.h"
#include "common.h"

int point_increment[][2] = 
{{ 0,0 },
{ 1,0 },
{ 1, -1},
{0,-1},
{-1,-1},
{ -1,0},
{-1,1},
{0,1},
{1,1}};

//Prototype from TOM MATH in photog
PNT get3dpnt_e(int nf,PNTLST pnts, PHOTO_FLOAT **tmxs ,PNTLST error);

/////////////////////////////////////////////////////////////////////////////
int moment(double data[], int n, double *ave, double *adev, double *sdev,
	double *var, double *skew, double *curt)
{
//	void nrerror(char error_text[]);
	int j;
	float ep=0.0,s,p;

	if (n <= 1) return -1;
	s=0.0;
	for (j=1;j<=n;j++) s += data[j];
		*ave=s/n;
	*adev=(*var)=(*skew)=(*curt)=0.0;
	for (j=1;j<=n;j++) {
		*adev += fabs(s=data[j]-(*ave));
		*var += (p=s*s);
		*skew += (p *= s);
		*curt += (p *= s);
	}
	*adev /= n;
	*var=(*var-ep*ep/n)/(n-1);
	*sdev=sqrt(*var);
	if (*var) {
		*skew /= (n*(*var)*(*sdev));
		*curt=(*curt)/(n*(*var)*(*var))-3.0;
	} else return -1;

	return 0;
}
/////////////////////////////////////////////////////////////////////////////
int CalculateStdDev( VECTOR *v, int snNumPoints, _Point3d *pt )
{
	double ave;
	double adev;
	double sdev;
	double var;
	double skew;
	double curt;

	int j;
	double ep=0.0,s,p;

	for( int i=0; i<3; i++ )
	{
		if (snNumPoints <= 1) 
			return -1;
		s=0.0;
		for (j=1;j<=snNumPoints;j++) 
			s += v[j-1][i+1];
		ave=s/snNumPoints;
		adev=(var)=(skew)=(curt)=0.0;
		for (j=1;j<=snNumPoints;j++) 
		{
			adev += fabs(s=v[j-1][i+1]-(ave));
			var += (p=s*s);
			skew += (p *= s);
			curt += (p *= s);
		}
		adev /= snNumPoints;
		var=(var-ep*ep/snNumPoints)/(snNumPoints-1);
		sdev=sqrt(var);
		switch(i)
		{	
			case 0:
				pt->x = sdev;
				break;
			case 1:
				pt->y = sdev;
				break;
			case 2:
				pt->z = sdev;
				break;
		}
		if (var) 
		{
			skew /= (snNumPoints*(var)*(sdev));
			curt=(curt)/(snNumPoints*(var)*(var))-3.0;
		} 
		else 
			return -1;
	}
	return 0;
}	

/*FF***********************************************************************
*									    *
*   NAME								    *
*	Calc3DPoint							    *
*									    *
*   DESCRIPTION								    *
*	Gets all selected pixels from currently open images and calculates  *
*	a 3D point for the requested bundle.				    *
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
#define FUNC "Calc3DPoint"
int Calc3DPoint( char *pszBundle, _Point3d *ptr3d )
{
    struct images img_rec;
    struct bundle bundle_rec;
    struct tmatrix tmatrix_rec;
    struct camera_info caminf_rec;
    short snNumPoints;
    PNTLST *plPnts;
    MATRIX *mTMatrix;
    short snCurrPnt;
    PNTLST plUnCorrPnt;
    VECTOR vFilmCorr;
    VECTOR vRadCorrFact;
    short snFound;
    int i;
//    PNTLST plTemp;
    VECTOR *v;
//    static VECTOR vOldPnt = NULL;
    double dfDist;
    LOCK_REQUEST lr[] = {
	{ BUNDLE, 'r' }, 
	{ TMATRIX, 'r' }, 
	{ IMAGES, 'r' }, 
	{ CAMERA_INFO, 'r' }, 
	{ PIXELS, 'r' }, 
	{ POINTS, 'r' } };
/*
    if( (htList = GetImageWinList()) == NULL )
    {
	ErrorOutput( "Unable to get image window list\n" );
	return( !OKAY );
    }
  
    if( htList->count <= 1 )
    {
	ErrorOutput( "Must select points in more than one image.\n" );
	return( !OKAY );
    }
    
    nt = htList->first;
*/
	CObList ImageList;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(&ImageList);
    snNumPoints = 0;
	POSITION pos;
	for( pos = ImageList.GetHeadPosition(); pos != NULL; )
	{
		CDocument *doc2 = (CDocument *)ImageList.GetNext(pos);
		CFotobrowDoc *doc = (CFotobrowDoc *)doc2;
	    TRACE("DOC name - %s\n", doc->GetPathName() );
		if( doc2->IsKindOf( RUNTIME_CLASS( CFotobrowDoc )) && doc->CheckSelected() )
		    snNumPoints++;
    }
    
    if( snNumPoints < 2 )
    {
		ErrorOutput( "You must define at least two points.\n" );
		return( !S_OKAY );
    }
    
    memset( &bundle_rec, 0, sizeof(struct bundle) );
    strcpy( bundle_rec.szTag, pszBundle );
    
//	int CheckLocks(DB_TASK *task, int dbn);
//	CheckLocks(TASKER);
    if( rdmGroupLock( 6, lr, TASKER ) != S_OKAY )
    {
		ErrorOutput( "Database Error Lock request failed.\n" );
		return( !S_OKAY );
    }
    
    if( dt_keyfind( BUNDLE_SZTAG, &(bundle_rec.szTag), TASKER) != S_OKAY )
    {
		ErrorOutput( "Database Error Couldn't find bundle record for %s\n",
			bundle_rec.szTag );
		rdmGroupFree( 6, lr, TASKER );
		return( !S_OKAY );
    }
    dt_setor( BUNDLE_TMATRIX, TASKER);
    
	plPnts = (PNTLST *)malloc( (snNumPoints) * sizeof(PNTLST));
//	for( i=0; i< snNumPoints; i++ )
//	    plPnts = matrix( 1, 9, 1, 3 );
    mTMatrix = matrix( 1, snNumPoints, 1, 12 );
    
    snCurrPnt = 0;
//    nt = htList->first;
	for( pos = ImageList.GetHeadPosition(); pos != NULL; )
	{
		CDocument *doc2 = (CDocument *)ImageList.GetNext(pos);
		CFotobrowDoc *doc = (CFotobrowDoc *)doc2;
	    TRACE("DOC name - %s\n", doc->GetPathName() );
		if( doc2->IsKindOf( RUNTIME_CLASS( CFotobrowDoc )) && doc->CheckSelected() )
		{
		    snFound = FALSE;
		    int q = dt_findfm( BUNDLE_TMATRIX, TASKER );
		    while( q == S_OKAY && !snFound )
		    {
				dt_setmm( IMAGES_TMATRIX, BUNDLE_TMATRIX, TASKER );
				dt_setro( IMAGES_TMATRIX, TASKER );
				dt_recread( &img_rec, TASKER );
//				CString s = doc->GetImageName();
//				int k = s.ReverseFind('\\');
				CString name;// = s.Right(s.GetLength() - k-1 );
				name = doc->szName;
				if( stricmp( img_rec.szName, name/*ivi->szName */) == 0 )
				{
			    	snFound = TRUE;
			    	dt_setmo( CAMERA_INFO_IMAGES, IMAGES_TMATRIX, TASKER );
			    	dt_setro( CAMERA_INFO_IMAGES, TASKER );
			    	dt_recread( &caminf_rec, TASKER );
			    	dt_setrm( IMAGES_TMATRIX, TASKER );
			    	dt_recread( &tmatrix_rec, TASKER );
				}
				else
		    		q = dt_findnm( BUNDLE_TMATRIX, TASKER );
	    	}
		    
	    	if( snFound )
	    	{
				snCurrPnt++;
		/* Get pixel coordinates */
				plUnCorrPnt = matrix(1, 9, 1, 2);

//#ifdef REDUCED_RES
				CPoint pt = doc->GetView()->GetSelected();
				double reductionFactor = ((CFotobrowApp*)AfxGetApp())->m_imageReduction;
//				char temp[128];
//				sprintf(temp,"Reduction is %f", reductionFactor);
//				AfxMessageBox(temp);
				for( i=0; i<9; i++ )
				{
					(*plUnCorrPnt)[i+1][1] = reductionFactor*(PHOTO_FLOAT)pt.x + point_increment[i][0];
					(*plUnCorrPnt)[i+1][2] = reductionFactor*(PHOTO_FLOAT)pt.y + point_increment[i][1];
				}

//		(*plUnCorrPnt)[1][1] = (PHOTO_FLOAT)ivi->xpos;
//		(*plUnCorrPnt)[1][2] = (PHOTO_FLOAT)ivi->ypos;
//#endif		
//				(*plUnCorrPnt)[1][1] = (PHOTO_FLOAT)nImageScale*(PHOTO_FLOAT)ivi->xpos;
//				(*plUnCorrPnt)[1][2] = (PHOTO_FLOAT)nImageScale*(PHOTO_FLOAT)ivi->ypos;

		/* set film correction vector */
				vFilmCorr = vector( 1, 6 );
				for( i = 0; i < 6; i++ )
				    vFilmCorr[i+1] = img_rec.dfFilmCorr[i];
		
		
		/* set radial correction factor vector */
				vRadCorrFact = vector( 1, 9 );
				for( i = 0; i < 9; i++ )
			   		vRadCorrFact[i+1] = caminf_rec.dfRadCorrFact[i];
		    
		/* Do film correction on pixel coordinates */
				plPnts[snCurrPnt-1] = corrfilmpnt( 9, caminf_rec.dfFocalLength, 
					vFilmCorr, plUnCorrPnt, vRadCorrFact );
//				copy_vector( (*plPnts)[snCurrPnt], 1, 3, (*plTemp)[1], 1, 3 );  
				
				for( i = 0; i < 12; i++ )
				    (*mTMatrix)[snCurrPnt][i+1] = tmatrix_rec.dfCameraPos[i]; 
			
				free_matrix( plUnCorrPnt, 1, 9, 1, 2 );
				free_vector( vFilmCorr, 1, 6 );
				free_vector( vRadCorrFact, 1, 3 );
		    }
		    else
				ErrorOutput( "Could not find image(%s) in bundle(%s)\n", 
					doc->GetImageName()/*ivi->szName*/, bundle_rec.szTag );
			
	    	doc->SetSelected(FALSE);//ivi->bPointSelected = FALSE;
			doc->UpdateAllViews(NULL);
//	    GLXwinset( XtDisplay(((ImageWgtInfo *)(nt->item))->wgt),
//		    XtWindow(((ImageWgtInfo *)(nt->item))->wgt));
	    	doc->GetView()->DrawCross(); //drawcross( ivi, 0 );
//	    	drawcross( ivi, 1 );
//	    	drawpoints( ivi );
		}
	}

    rdmGroupFree( 6, lr, TASKER );

    if( snCurrPnt == snNumPoints )
    {
	/*** call routine to calculate 3D point ***/
		DebugOutput( 2, "Calling get3dpnt\n", FUNC );
		DebugOutput( 2, "    snNumPoints = %d\n", snNumPoints );
/*
		for( int i = 1; i <= snNumPoints; i++ )
		{
		    DebugOutput( 2, "plPnts[%d] = %lf, %lf, %lf\n",
			    i, (*plPnts)[i][1], (*plPnts)[i][2], (*plPnts)[i][3] );
		    DebugOutput( 2, "tmatrix[%d] = \n", i );
		    for( int j = 0; j < 12; j++ )
				DebugOutput( 2, "[%d] = %lf\n", j+1, (*mTMatrix)[i][j+1] );
		}*/
		CStdioFile fil("Data.dat", 
			CFile::modeCreate | CFile::modeWrite | CFile::typeText );
		char temp[255];
		PNTLST tempPoint = matrix(1,snNumPoints,1,3);
		v = (VECTOR *)malloc((snNumPoints) * 9 * sizeof(VECTOR) );
		for( i=0; i<snNumPoints; i++ )
			for( int j=0; j<9; j++ )
			{
				for( int k=0; k<snNumPoints; k++ )
				{
					(*tempPoint)[k+1][1] = (*(plPnts[k]))[1][1];
					(*tempPoint)[k+1][2] = (*(plPnts[k]))[1][2];
					(*tempPoint)[k+1][3] = (*(plPnts[k]))[1][3];

				}	
				(*tempPoint)[i+1][1] = (*(plPnts[i]))[j+1][1];
				(*tempPoint)[i+1][2] = (*(plPnts[i]))[j+1][2];
				(*tempPoint)[i+1][3] = (*(plPnts[i]))[j+1][3];
				
				if( i || j  )
					v[i*9 + j] = get3dpnt( snNumPoints, tempPoint, *mTMatrix );
////////////////////////////////////////////////////////////////////////////
				else
				{
				  PNTLST error = matrix(1,snNumPoints,1,3);
  
				  v[0] = get3dpnt_e(snNumPoints, tempPoint, *mTMatrix,error);
				  for(int m=1;m<=snNumPoints;m++)
				  {
					sprintf(temp, "foto %d ge3dpoint error - %e,  %e, %e\n"
									,m,(*error)[m][1],(*error)[m][2],(*error)[m][3]);
					fil.WriteString(temp);
					double dist = sqrt((*error)[m][1]*(*error)[m][1] + 
									(*error)[m][2]*(*error)[m][2] +	
									(*error)[m][3]*(*error)[m][3]  );
					if( m==1 || ptr3d[2].x < dist )
					{	
						ptr3d[2].x = dist;
					}
				  }
					fil.WriteString("\n");
				  free_matrix(error,1,snNumPoints,1,3);
				}
////////////////////////////////////////////////////////////////////////////
			}
		free_matrix(tempPoint,1,snNumPoints,1,3 );
		
		for( i=0; i<snNumPoints; i++ )
		{
			for( int j=0; j<9; j++ )
			{
				sprintf(temp,"%15.5f : %15.5f : %15.5f\n", v[i*9+j][1],v[i*9+j][2],
					v[i*9+j][3] );
				fil.WriteString(temp);
			}
		}
		
		CalculateStdDev( v, snNumPoints*9, &ptr3d[1] );

		if( ptr3d != NULL )
		{
		    ptr3d[0].x = v[0][1];
		    ptr3d[0].y = v[0][2];
		    ptr3d[0].z = v[0][3];
		}
//		char temp10[256];
		Output( 0, "3D Point Info\n", FUNC );
//		sprintf(temp10,"    Current point is %.4f, %.4f %.4f\n", v[1], v[2], v[3] );
//		AfxMessageBox(temp10);
		//Output( 0, "    Current point is %.4f, %.4f %.4f\n", v[1], v[2], v[3] );
/*		if( vOldPnt != NULL )
		{
		    Output( 0, "    Last point was %.4f, %.4f %.4f\n", vOldPnt[1], vOldPnt[2], vOldPnt[3] );
     	
		    dfDist = sqrt( pow( vOldPnt[1]-v[0][1], 2 ) + pow( vOldPnt[2]-v[0][2], 2 ) + 
			    pow( vOldPnt[3]-v[0][3], 2 ) );

	    	Output( 0, "    Distance = %f\n", dfDist );
	    	free_vector( vOldPnt, 1, 3 );
		}
		vOldPnt = v[0];*/
    }
    else
    {
		ErrorOutput( "I thought I would find %d points but only found %d points\n", 
			snNumPoints, snCurrPnt );
		return( !S_OKAY );
    }
    
	for( i=0; i<snNumPoints; i++ )
		free_matrix( plPnts[i], 1, 9, 1, 3 );
	free(plPnts);
    free_matrix( mTMatrix, 1, snNumPoints, 1, 12 );
	for( i=0; i<snNumPoints*9; i++ )
		free_vector(v[i], 1,3);
    free(v);

    return( S_OKAY );
}
#undef FUNC

char bundle_name[128];
struct TestData
{
	DB_ADDR dba;
	long memcount;
	int imagecount;
	char bundle_name[128];
};

void KillBundleList( CPtrList *pList)
{
	POSITION pos;

	for( pos = pList->GetHeadPosition(); pos != NULL; )
 	{
		struct TestData *current = (struct TestData *)(pList->GetNext(pos));
		if( current != NULL )
			delete current;
	}
}

char *SearchForBundle( )
{
    LOCK_REQUEST lr[] = {
	{ BUNDLE, 'r' }, 
	{ TMATRIX, 'r' }, 
	{ IMAGES, 'r' }, 
	{ CAMERA_INFO, 'r' }, 
	{ PIXELS, 'r' }, 
	{ POINTS, 'r' } };
	int snNumPoints;
	CPtrList pList;
  	struct TestData *pData;
	
	CObList ImageList;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(&ImageList);

//	int CheckLocks(DB_TASK *task, int dbn);
//	CheckLocks(TASKER);
    if( rdmGroupLock( 6, lr, TASKER ) != S_OKAY )
    {
		ErrorOutput( "Database Error Lock request failed.\n" );
		return( NULL );
    }
    snNumPoints = 0;
	POSITION pos;
	for( pos = ImageList.GetHeadPosition(); pos != NULL; )
	{
		CDocument *doc2 = (CDocument *)ImageList.GetNext(pos);
		CFotobrowDoc *doc = (CFotobrowDoc *)doc2;
	    TRACE("DOC name - %s\n", doc->GetPathName() );
		if( doc2->IsKindOf( RUNTIME_CLASS( CFotobrowDoc )) && doc->CheckSelected() )
		{
		    snNumPoints++;
//			CString s = doc->GetImageName();
//			int k = s.ReverseFind('\\');
			CString name;// = s.Right(s.GetLength() - k-1 );
//			name.MakeLower();
			name = doc->szName;
			if( dt_keyfind( IMAGES_SZNAME, (const char *)name, TASKER ) == S_OKAY )
			{
				dt_setor( IMAGES_TMATRIX, TASKER );
			    int q = dt_findfm( IMAGES_TMATRIX, TASKER );
				while( q == S_OKAY )
				{
					dt_setmm( BUNDLE_TMATRIX, IMAGES_TMATRIX, TASKER);
					dt_setro( BUNDLE_TMATRIX, TASKER );
					pData = new struct TestData;
					pData->imagecount = 1;
					dt_crget( &(pData->dba), TASKER );
					dt_crread( BUNDLE_SZTAG, pData->bundle_name, TASKER );
					dt_members( BUNDLE_TMATRIX, &(pData->memcount ), TASKER);
					POSITION pos2;
					int found = FALSE;
					for( pos2 = pList.GetHeadPosition(); pos2 != NULL; )
					{
						struct TestData *current = (struct TestData *)(pList.GetNext(pos2));
						if( pData->dba == current->dba )
						{
							current->imagecount++;
							found = TRUE;
							break;
						}
					}					 
					if( !found )
						pList.AddTail(pData);
					else
						delete pData;

					q = dt_findnm( IMAGES_TMATRIX, TASKER );
				}
			}
		}
	}
	POSITION pos2;
	int found = FALSE;
	struct TestData *remember=NULL;

	for( pos2 = pList.GetHeadPosition(); pos2 != NULL; )
 	{
		struct TestData *current = (struct TestData *)(pList.GetNext(pos2));
		if( current->imagecount == snNumPoints )
		{
			if( remember == NULL )
				remember = current;
			else if( current->memcount > remember->memcount )
				remember = current;				
		}
	}
    rdmGroupFree( 6, lr, TASKER );
	if( remember != NULL )
	{
		strcpy( bundle_name, remember->bundle_name );
		KillBundleList(&pList);
//		AfxMessageBox(bundle_name);
		return bundle_name;
	}
	else
	{
		KillBundleList(&pList);
	 	AfxMessageBox("No common photo group for selected images");
		return NULL;
	}
}


char *SearchForBundleSpecific(char **ImageNames, int count )
{
/*    LOCK_REQUEST lr[] = {
	{ BUNDLE, 'r' }, 
	{ TMATRIX, 'r' }, 
	{ IMAGES, 'r' }, 
	{ CAMERA_INFO, 'r' }, 
	{ PIXELS, 'r' }, 
	{ POINTS, 'r' } };
*/	int snNumPoints;
	CPtrList pList;
  	struct TestData *pData;
	
	CObList ImageList;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(&ImageList);

  /*  if( dt_lock( 6, lr, TASKER ) != S_OKAY )
    {
		ErrorOutput( "Database Error Lock request failed.\n" );
		return( NULL );
    }*/
    snNumPoints = 0;
	POSITION pos;
	for( int i=0; i<count; i++ )
	{
		if( dt_keyfind( IMAGES_SZNAME, ImageNames[i], TASKER ) == S_OKAY )
		{
			dt_setor( IMAGES_TMATRIX, TASKER );
			int q = dt_findfm( IMAGES_TMATRIX, TASKER );
			while( q == S_OKAY )
			{
				dt_setmm( BUNDLE_TMATRIX, IMAGES_TMATRIX, TASKER);
				dt_setro( BUNDLE_TMATRIX, TASKER );
				pData = new struct TestData;
				pData->imagecount = 1;
				dt_crget( &(pData->dba), TASKER );
				dt_crread( BUNDLE_SZTAG, pData->bundle_name, TASKER );
				dt_members( BUNDLE_TMATRIX, &(pData->memcount ), TASKER);
				POSITION pos2;
				int found = FALSE;
				for( pos2 = pList.GetHeadPosition(); pos2 != NULL; )
				{
					struct TestData *current = (struct TestData *)(pList.GetNext(pos2));
					if( pData->dba == current->dba )
					{
						current->imagecount++;
						found = TRUE;
						break;
					}
				}					 
				if( !found )
					pList.AddTail(pData);
				else
					delete pData;

				q = dt_findnm( IMAGES_TMATRIX, TASKER );
			}
		}
	}
	POSITION pos2;
	int found = FALSE;
	struct TestData *remember=NULL;

	for( pos2 = pList.GetHeadPosition(); pos2 != NULL; )
 	{
		struct TestData *current = (struct TestData *)(pList.GetNext(pos2));
		if( current->imagecount == count )
		{
			if( remember == NULL )
				remember = current;
			else if( current->memcount > remember->memcount )
				remember = current;				
		}
	}
//    dt_freeall( &Mytask );
	if( remember != NULL )
	{
		strcpy( bundle_name, remember->bundle_name );
		KillBundleList(&pList);
//		AfxMessageBox(bundle_name);
		return bundle_name;
	}
	else
	{
	 	AfxMessageBox("No common photo group for selected images");
		KillBundleList(&pList);
		return NULL;
	}
}
