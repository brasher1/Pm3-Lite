// CircleByEdge.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "mainfrm.h"
#include "CircleByEdge.h"
#include "elem_help.h"
#include "output.h"
#include "photog.h"
#include "ele_math.h"
#include "photog_msg.h"
#include "../3Dclass/Ptg3DCylinderObj.h"
#include "cylinderdata.h"
#include "circledatadlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void PrintMakeCircData( int npArcsPerCirc, 
	int *npPntsPerArc, PNTLST *plpArcPnts, 
	PNTLST plArcTMatrix );

/////////////////////////////////////////////////////////////////////////////
// CCircleByEdge dialog


CCircleByEdge::CCircleByEdge(CWnd* pParent /*=NULL*/)
	: CDialog(CCircleByEdge::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCircleByEdge)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCircleByEdge::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCircleByEdge)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCircleByEdge, CDialog)
	//{{AFX_MSG_MAP(CCircleByEdge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCircleByEdge message handlers

void CCircleByEdge::OnOK() 
{
	int i=0;
	CList< Edge_2D *, Edge_2D * > listEdges;
	Edge_2D *currentEdge;
	int status;
/////////////////////
    LOCK_REQUEST lr[] = {
	{ BUNDLE, 'r' }, 
	{ TMATRIX, 'r' }, 
	{ IMAGES, 'r' }, 
	{ CAMERA_INFO, 'r' }, 
	{ PIXELS, 'r' }, 
	{ POINTS, 'r' } };

	
	if( rdmGroupLock( 6, lr, TASKER ) != S_OKAY )
    {
		ErrorOutput( "Database Error Lock request failed.\n" );
		return;
    }
/////////////////
	do 
	{ 
		currentEdge = new Edge_2D;
		status = getSelectedEdge(currentEdge,TRUE);
		if( status )
		{
			listEdges.AddTail(currentEdge);
//			MessageBox(currentEdge->m_szImageName);
		}
	}
	while( status );

	char temp[128];
	sprintf(temp,"Count = %d", listEdges.GetCount());
//	MessageBox(temp);
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
	double reductionFactor = ((CFotobrowApp*)AfxGetApp())->m_imageReduction;
	PNTLST *plEdgePnts = (PNTLST *)malloc(listEdges.GetCount()*sizeof(PNTLST));
//	PNTLST plDirPnts = matrix(0,m_listEdges.GetCount()-1,1,3);
	char **names = (char **)malloc(listEdges.GetCount()*sizeof(char *));
	int *NumPntsPerEdge = (int *)malloc(listEdges.GetCount()*sizeof(int));
	PNTLST tMatrices = matrix(0,listEdges.GetCount()-1,1,13);
//	PNTLST plEndPnts = matrix(0,1,1,3);
//	PNTLST tEndMatrices = matrix(0,1,1,13);
	POSITION pos = listEdges.GetHeadPosition();
	while(pos)
	{
		Edge_2D *edge = listEdges.GetNext(pos);
		names[i] = edge->m_szImageName;
		i++;
	}
	char *bundlename = SearchForBundleSpecific(names,i);
	pos = listEdges.GetHeadPosition();
	i=0;
	while(pos)
	{
		Edge_2D *edge = listEdges.GetNext(pos);
		PNTLST plTemp;
		CorrectPnts(edge->m_szImageName,edge->m_iNumPoints,
			edge->m_pointList, plTemp, reductionFactor );
		plEdgePnts[i] = clone_matrix( plTemp, 1, edge->m_iNumPoints, 1, 3,
				0, edge->m_iNumPoints-1, 1, 3 );
		free_matrix(plTemp,1,edge->m_iNumPoints, 1, 3);

//		PNTLST plUncorrPnts = matrix( 1, 1, 1, 3 );
//		copy_vector( (*plUncorrPnts)[1], 1, 3, edge->m_dirPoint.getPoint(), 1, 3 );
//		PNTLST plDirTemp;
//		CorrectPnts( edge->m_szImageName, 1, plUncorrPnts, plDirTemp, reductionFactor );
//		copy_vector( (*plDirPnts)[i], 1, 3, (*plDirTemp)[1], 1, 3 );
//		free_matrix( plDirTemp, 1, 1, 1, 3 );
//		free_matrix( plUncorrPnts, 1, 1, 1, 3 );

		NumPntsPerEdge[i] = edge->m_iNumPoints;
		GETWORLD.FindTMatrix(bundlename, edge->m_szImageName, (*tMatrices)[i]); 
		i++;
	}
/*	for( i=0; i<2; i++ )
	{
		PNTLST plUncorrPnts = matrix( 1, 1, 1, 3 );
		copy_vector( (*plUncorrPnts)[1], 1, 3, m_endPoints[i].getPoint(), 1, 3 );
		PNTLST plDirTemp;
		CorrectPnts( (char *)(m_endPoints[i].getImageName()), 1, plUncorrPnts, plDirTemp, reductionFactor );
		copy_vector( (*plEndPnts)[i], 1, 3, (*plDirTemp)[1], 1, 3 );
		free_matrix( plDirTemp, 1, 1, 1, 3 );
		free_matrix( plUncorrPnts, 1, 1, 1, 3 );
		GETWORLD.FindTMatrix(bundlename, (char *)(m_endPoints[i].getImageName()), (*tEndMatrices)[i]); 
	}
*/
	rdmGroupFree( 6, lr, TASKER ); //release all database locks
	
//	PrintMakeCylData( m_listEdges.GetCount(), 
//		NumPntsPerEdge, plEdgePnts, 
//		tMatrices, plDirPnts, plEndPnts, tEndMatrices );
/////////////////////////////////////////////////////////////////////////////
    struct Cylinder cyl_rec;
    struct _transform trans;
    
    memset( &cyl_rec, 0, sizeof(struct Cylinder) );
    memset( &trans, 0, sizeof(struct _transform) );

 //   PrintMakeCircData( &nTotalArcs, npNumPntsPerArc,
//	    plpArcPnts, plArcTMatrix );

    MakeCirc( listEdges.GetCount(), NumPntsPerEdge,
	    plEdgePnts, tMatrices,
	    cyl_rec.radius, trans );
	PrintMakeCircData( listEdges.GetCount(), NumPntsPerEdge,
	    plEdgePnts, tMatrices );

	Output(0,"Radius=%lf, Height=%lf\n", cyl_rec.radius, cyl_rec.height);

	cyl_rec.height = 1.0;

//    PrintMakeCircData( cyl_rec.cyl_radius, transform_rec );     

//    PtgOutput( &cyl_rec, &transform_rec, getCurrentBundleName(), szImageNames );

//    DefCircCancel( wgt, xtpClientData, xtpCallData );
/////////////////////////////////////////////////////////////////////////////
//	struct _transform trans;
//	struct tCylinder cyl;
//	cyl.cyl_radius = 10.0;
//	cyl.cyl_height = 20.0;
	
//	make_cyl_by_lines(m_listEdges.GetCount(),
//		NumPntsPerEdge, plEdgePnts,tMatrices, plDirPnts,
  //      plEndPnts, tEndMatrices, cyl, trans);
	
	Ptg3DCylinderObj *cyli = new Ptg3DCylinderObj;
	cyli->setHeight(cyl_rec.height);
	cyli->setRadius(cyl_rec.radius);
	char tempStr[256];
	sprintf( tempStr, "Obj%d", cyli->getUniqID() );
	cyli->setName(tempStr);
	cyli->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
	cyli->setColor( 0.0, 1.0, 0.0 );
	cyli->setActive(TRUE);
	VECTOR v = vector(1,13);
//	v[1] = trans.rotation.r[0];
//	v[2] = trans.rotation.r[1];
//	v[3] = trans.rotation.r[2];
//	v[4] = trans.rotation.r[4];
//	v[5] = trans.rotation.r[5];
//	v[6] = trans.rotation.r[6];
//	v[7] = trans.rotation.r[8];
//	v[8] = trans.rotation.r[9];
//	v[9] = trans.rotation.r[10];
//	v[10] = trans.pozition.pos_x;
//	v[11] = trans.pozition.pos_y;
//	v[12] = trans.pozition.pos_z;

	//
	// jwb - 05/14/97....
	// In the process of updating the 3D Class library, the axis that the cylinder centerline
	// runs along changed.  To make this correct, the cylinder needs to be rotated by the
	// following matrix.....
	//		1.0		0.0		 0.0
	//		0.0		0.0		 1.0
	//		0.0		-1.0	 0.0
	// Instead of doing a real matrix multiply, I am taking care of it in the transfer
	// of the matrix from the _transform structure to the VECTOR tmatrix.  THIS IS A PRE-MULTIPLY!!!!!!!
//	v[1]  =  trans.rotation.r[0];
//	v[2]  =  trans.rotation.r[1];
//	v[3]  =  trans.rotation.r[2];
//	v[4]  =  trans.rotation.r[8];
//	v[5]  =  trans.rotation.r[9];
//	v[6]  =  trans.rotation.r[10];
//	v[7]  = -trans.rotation.r[4];
//	v[8]  = -trans.rotation.r[5];
//	v[9]  = -trans.rotation.r[6];
//	v[10] =  trans.pozition.pos_x;
//	v[11] =  trans.pozition.pos_y;
//	v[12] =  trans.pozition.pos_z;

	v[1] = trans.rotation.r[0];	v[2] = -trans.rotation.r[8];	v[3] = trans.rotation.r[4];
	v[4] = trans.rotation.r[1];	v[5] = -trans.rotation.r[9];	v[6] = trans.rotation.r[5];
	v[7] = trans.rotation.r[2];	v[8] = -trans.rotation.r[10];	v[9] = trans.rotation.r[6];
	v[10] = trans.pozition.pos_x;	v[11] = trans.pozition.pos_y;	v[12] = trans.pozition.pos_z;

	cyli->setMatrix(v);

/////////////////////////////////////////////////////////////////////
	_Point3d ptg[2];

	for( int k=0; k<2; k++ )
	{
		ptg[k].x = k;
		ptg[k].y = k;
		ptg[k].z = k;
	}
//	ConvertPoint(&ptg[0], &ptg[0]);
//	ConvertPoint(&ptg[1], &ptg[1]);
//////////////////////////////////////////////////////////////////////

	CCircleDataDlg *cirdlg = new CCircleDataDlg;
	cirdlg->setValues(cyl_rec.radius, cyl_rec.height, (double *)ptg );
	cirdlg->DoModal();
	cirdlg->getValues(&(cyl_rec.radius), &(cyl_rec.height), (double *)ptg);
	delete cirdlg;

//////////////////////////////////////////////////////////////////////

//	GETOVERLAY->addMember(cyli);
	// Do not free/delete the cylinder...it is being turned over to the 3D hierarchy's control
	AfxGetApp()->m_pMainWnd->SendMessage( UM_NEW_3D_ELEMENT, (WPARAM)cyli, (LPARAM)NULL );

	free_vector(v,1,13);

	for( i=0; i<listEdges.GetCount(); i++ )
	{
		free_matrix(plEdgePnts[i], 0, NumPntsPerEdge[i]-1, 1,3);		
	}
	free(names);
	free(plEdgePnts);
	free(NumPntsPerEdge);
	free_matrix(tMatrices,0,listEdges.GetCount()-1,1,13);
//	free_matrix(plDirPnts,0,m_listEdges.GetCount()-1,1,3);
//	free_matrix(plEndPnts,0,1,1,3);
//	free_matrix(tEndMatrices,0,1,1,13);

///////////////////////////////////////
	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->setSelectMode(FALSE);

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

	while( listEdges.GetCount() > 0 )
		delete listEdges.RemoveTail();

	DestroyWindow();
//	CDialog::OnOK();
}

void CCircleByEdge::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CCircleByEdge::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->setSelectMode(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
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
*	00/00/93 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "PrintMakeCircData"
void PrintMakeCircData( int npArcsPerCirc, 
	int *npPntsPerArc, PNTLST *plpArcPnts, 
	PNTLST plArcTMatrix ) 
{
    int i, j, k, l;
    int nTotalArcs = npArcsPerCirc;
    
    Output( 0, "%s:\n", FUNC );
    
    Output( 0, "\n  Number of arcs per circle:\n" );
    
/*    if( npArcsPerCirc != NULL )
    {
	for( i = 0; i < 1; i++ )
	{
	    Output( 0, "    [%d] = %d\n", i, npArcsPerCirc[i] );
	    nTotalArcs += npArcsPerCirc[i];
	}
    }
    else
    {
	Output( 0, "    npArcsPerCirc is NULL\n" );
	return;
    }
  */  
    Output( 0, "    Arcs per Circle = %d\n", npArcsPerCirc );
    Output( 0, "\n  Number of points per arc:\n" );
    if( npPntsPerArc != NULL )
    {
	for( int i = 0; i < nTotalArcs; i++ )
	{
	    Output( 0, "    [%d] = %d\n", i, npPntsPerArc[i] );
	}
    }
    else
    {
	Output( 0, "    npPntsPerArc is NULL\n" );
	return;
    }
    
    Output( 0, "\n  Arc Points:\n" );
    if( plpArcPnts != NULL )
    {
	for( i = 0; i < nTotalArcs; i++ )
	{
	    for( j = 0; j < npPntsPerArc[i]; j++ )
	    {
		Output( 0, "    [%d][%d] = %lf,%lf,%lf\n", 
			i, j, 
			(*(plpArcPnts[i]))[j][1], 
			(*(plpArcPnts[i]))[j][2], 
			(*(plpArcPnts[i]))[j][3] );
	    }
	    Output( 0, "\n" );
	}
    }
    else
	Output( 0, "    plpArcPnts is NULL\n" );
	
    Output( 0, "\n  Arc TMatrices:\n" );
    if( plArcTMatrix != NULL )
    {
	for( i = 0; i < nTotalArcs; i++ )
	{
	    Output( 0, "    [%d]:\n", i );
	    for( j = 1; j <= 13; j++ )
		Output( 0, "      [%d] = %lf\n", j,
			(*plArcTMatrix)[i][j] );
	}
    }
    else
	Output( 0, "    plArcTMatrix is NULL\n" );
}
#undef FUNC
