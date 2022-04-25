// CreateCylinderByEdgeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "mainfrm.h"
#include "CreateCylinderByEdgeDlg.h"
#include "elem_help.h"
#include "output.h"
#include "photog.h"
#include "ele_math.h"
#include "photog_msg.h"
#include "../3Dclass/Ptg3DCylinderObj.h"
#include "../3Dclass/Ptg3DLineObj.h"
#include "cylinderdata.h"

//int CheckLocks(DB_TASK *task, int dbn);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char *str[] = {
		"Select points along edge number ",
		"Select a point on cylinder side of edge ",
		"Select OK to process or \r\npoints along edge no.",
		"Select the start point",
		"Select the end point",
		"Done" };

#define ON_EDGE		0x0001L
#define ON_POINT	0x0002L
#define ON_ENDPOINT1 0x0004L
#define ON_ENDPOINT2 0x0008L

void PrintMakeCylData( int nNumLines, int *nNumPnts, PNTLST *plpPnts, 
	PNTLST plTMatrix, PNTLST plDirPnts, PNTLST plEndPnts, 
	PNTLST plEndPntsTMatrix );
/////////////////////////////////////////////////////////////////////////////
// CCreateCylinderByEdgeDlg dialog


CCreateCylinderByEdgeDlg::CCreateCylinderByEdgeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateCylinderByEdgeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateCylinderByEdgeDlg)
	m_Label = _T("");
	//}}AFX_DATA_INIT
}

CCreateCylinderByEdgeDlg::~CCreateCylinderByEdgeDlg()
{
	while( m_listEdges.GetCount() > 0 )
		delete m_listEdges.RemoveTail();
}

void CCreateCylinderByEdgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateCylinderByEdgeDlg)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_BACK, m_Back);
	DDX_Control(pDX, IDC_NEXT, m_Next);
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Text(pDX, IDC_LABEL, m_Label);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateCylinderByEdgeDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateCylinderByEdgeDlg)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateCylinderByEdgeDlg message handlers

void CCreateCylinderByEdgeDlg::OnBack() 
{
	// TODO: Add your control notification handler code here
	
}

void CCreateCylinderByEdgeDlg::OnNext() 
{
	if( m_state == ON_EDGE )
	{
		processEdge();
	}
	else if( m_state == ON_POINT )
	{
		processDirPoint();
	}

	UpdateButtons();
}

void CCreateCylinderByEdgeDlg::OnOK() 
{
///////////////////////////////////
    LOCK_REQUEST lr[] = {
	{ BUNDLE, 'r' }, 
	{ TMATRIX, 'r' }, 
	{ IMAGES, 'r' }, 
	{ CAMERA_INFO, 'r' }, 
	{ PIXELS, 'r' }, 
	{ POINTS, 'r' } };
	int i=0;
	
	if( m_state == ON_EDGE )
	{
		m_listEdges.RemoveTail();
		delete m_currentEdge;
		((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->setSelectMode(FALSE);
		m_state = ON_ENDPOINT1;
		UpdateLabel(str[3]);
		UpdateButtons();
		return;
	}
	if( m_state == ON_POINT )
	{
		processDirPoint();				// This adds an empty edge to the list of edges
		m_listEdges.RemoveTail();
		delete m_currentEdge;
		((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->setSelectMode(FALSE);
		m_state = ON_ENDPOINT1;
		UpdateLabel(str[3]);
		UpdateButtons();
		return;
	}
	if( m_state == ON_ENDPOINT1 )
	{
		if( processEndPoint() )
		{
			m_state = ON_ENDPOINT2;
			UpdateLabel(str[4]);
			UpdateButtons();
		}
		return;
	}

	if( !processEndPoint() )
		return;
	
//	CheckLocks(TASKER);
    
 	if( rdmGroupLock( 6, lr, TASKER ) != S_OKAY )
    {
		ErrorOutput( "Database Error Lock request failed.\n" );
		return;
    }
///////////////////////////////////
	double reductionFactor = ((CFotobrowApp*)AfxGetApp())->m_imageReduction;
	PNTLST *plEdgePnts = (PNTLST *)malloc(m_listEdges.GetCount()*sizeof(PNTLST));
	PNTLST plDirPnts = matrix(0,m_listEdges.GetCount()-1,1,3);
	char **names = (char **)malloc(m_listEdges.GetCount()*sizeof(char *));
	int *NumPntsPerEdge = (int *)malloc(m_listEdges.GetCount()*sizeof(int));
	PNTLST tMatrices = matrix(0,m_listEdges.GetCount()-1,1,13);
	PNTLST plEndPnts = matrix(0,1,1,3);
	PNTLST tEndMatrices = matrix(0,1,1,13);
	POSITION pos = m_listEdges.GetHeadPosition();
	while(pos)
	{
		Edge_2D *edge = m_listEdges.GetNext(pos);
		names[i] = edge->m_szImageName;
		i++;
	}
	char *bundlename = SearchForBundleSpecific(names,i);
	pos = m_listEdges.GetHeadPosition();
	i=0;
	while(pos)
	{
		Edge_2D *edge = m_listEdges.GetNext(pos);
		PNTLST plTemp;
		CorrectPnts(edge->m_szImageName,edge->m_iNumPoints,
			edge->m_pointList, plTemp, reductionFactor );
		plEdgePnts[i] = clone_matrix( plTemp, 1, edge->m_iNumPoints, 1, 3,
				0, edge->m_iNumPoints-1, 1, 3 );
		free_matrix(plTemp,1,edge->m_iNumPoints, 1, 3);

		PNTLST plUncorrPnts = matrix( 1, 1, 1, 3 );
		copy_vector( (*plUncorrPnts)[1], 1, 3, edge->m_dirPoint.getPoint(), 1, 3 );
		PNTLST plDirTemp;
		CorrectPnts( edge->m_szImageName, 1, plUncorrPnts, plDirTemp, reductionFactor );
		copy_vector( (*plDirPnts)[i], 1, 3, (*plDirTemp)[1], 1, 3 );
		free_matrix( plDirTemp, 1, 1, 1, 3 );
		free_matrix( plUncorrPnts, 1, 1, 1, 3 );

		NumPntsPerEdge[i] = edge->m_iNumPoints;
		GETWORLD.FindTMatrix(bundlename, edge->m_szImageName, (*tMatrices)[i]); 
		i++;
	}
	for( i=0; i<2; i++ )
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

	rdmGroupFree( 6, lr, TASKER ); //release all database locks
	
//	PrintMakeCylData( m_listEdges.GetCount(), 
//		NumPntsPerEdge, plEdgePnts, 
//		tMatrices, plDirPnts, plEndPnts, tEndMatrices );

	struct _transform trans;
	struct Cylinder cyl;
	cyl.radius = 10.0;
	cyl.height = 20.0;
	
	make_cyl_by_lines(m_listEdges.GetCount(),
		NumPntsPerEdge, plEdgePnts,tMatrices, plDirPnts,
        plEndPnts, tEndMatrices, cyl, trans);
	
	Ptg3DCylinderObj *cyli = new Ptg3DCylinderObj;
	cyli->setHeight(cyl.height);
	cyli->setRadius(cyl.radius);
	char tempStr[256];
	sprintf( tempStr, "Obj%d", cyli->getUniqID() );
	cyli->setName(tempStr);
	cyli->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
	cyli->setColor( 0.0, 1.0, 0.0 );
	cyli->setActive(TRUE);
	VECTOR v = vector(1,13);

	//
	// jwb - 03/07/97....
	// In the process of updating the 3D Class library, the axis that the cylinder centerline
	// runs along changed.  To make this correct, the cylinder needs to be rotated by the
	// following matrix.....
	//		1.0		0.0		 0.0
	//		0.0		0.0		-1.0
	//		0.0		1.0		 0.0
	// Instead of doing a real matrix multiply, I am taking care of it in the transfer
	// of the matrix from the _transform structure to the VECTOR tmatrix.  THIS IS A PRE-MULTIPLY!!!!!!!
/*
	v[1]  =  trans.rotation.r[0];
	v[2]  =  trans.rotation.r[1];
	v[3]  =  trans.rotation.r[2];
	v[4]  =  trans.rotation.r[8];
	v[5]  =  trans.rotation.r[9];
	v[6]  =  trans.rotation.r[10];
	v[7]  = -trans.rotation.r[4];
	v[8]  = -trans.rotation.r[5];
	v[9]  = -trans.rotation.r[6];
	v[10] =  trans.pozition.pos_x;
	v[11] =  trans.pozition.pos_y;
	v[12] =  trans.pozition.pos_z;
*/

/*
	v[1]  =  trans.rotation.r[0];
	v[2]  =  trans.rotation.r[1];
	v[3]  =  trans.rotation.r[2];
	v[4]  = -trans.rotation.r[8];
	v[5]  = -trans.rotation.r[9];
	v[6]  = -trans.rotation.r[10];
	v[7]  =  trans.rotation.r[4];
	v[8]  =  trans.rotation.r[5];
	v[9]  =  trans.rotation.r[6];
	v[10] =  trans.pozition.pos_x;
	v[11] =  trans.pozition.pos_y;
	v[12] =  trans.pozition.pos_z;
*/
/*
	v[1]  =  trans.rotation.r[0];
	v[2]  = -trans.rotation.r[8];
	v[3]  =  trans.rotation.r[4];
	v[4]  =  trans.rotation.r[1];
	v[5]  = -trans.rotation.r[9];
	v[6]  =  trans.rotation.r[5];
	v[7]  =  trans.rotation.r[2];
	v[8]  = -trans.rotation.r[10];
	v[9]  =  trans.rotation.r[6];
	v[10] =  trans.pozition.pos_x;
	v[11] =  trans.pozition.pos_y;
	v[12] =  trans.pozition.pos_z;
*/
	v[1] = trans.rotation.r[0];	v[2] = -trans.rotation.r[8];	v[3] = trans.rotation.r[4];
	v[4] = trans.rotation.r[1];	v[5] = -trans.rotation.r[9];	v[6] = trans.rotation.r[5];
	v[7] = trans.rotation.r[2];	v[8] = -trans.rotation.r[10];	v[9] = trans.rotation.r[6];
	v[10] = trans.pozition.pos_x;	v[11] = trans.pozition.pos_y;	v[12] = trans.pozition.pos_z;

	cyli->setMatrix(v);

//////////////////////////////////////////////////////////////////////
	_Point3d ptg[2];

	for( int k=0; k<2; k++ )
	{
		ptg[k].x = (*plEndPnts)[k][1];
		ptg[k].y = (*plEndPnts)[k][2];
		ptg[k].z = (*plEndPnts)[k][3];
	}
	ConvertPoint(&ptg[0], &ptg[0]);
	ConvertPoint(&ptg[1], &ptg[1]);
//////////////////////////////////////////////////////////////////////

	CCylinderData *cyldlg = new CCylinderData;
	cyldlg->setValues(cyl.radius, cyl.height, (double *)ptg );
	cyldlg->DoModal();
	cyldlg->getValues(&(cyl.radius), &(cyl.height), (double *)ptg);
	delete cyldlg;
//	char temp[512];
//	sprintf(temp,"Diameter:%f\r\n"
//			"Height: %f\r\n"
//			"Start Point: %fE, %fN, %fH\r\n"
//			"EndPoint: %fE, %fN, %fH",
//			cyl.cyl_radius*2.0,cyl.cyl_height,
//			ptg[0].x, ptg[0].y, ptg[0].z,
//			ptg[1].x, ptg[1].y, ptg[1].z);
//	AfxMessageBox(temp);
///////////////////////////////////////

//	GETOVERLAY->addMember(cyli);       //Adding elems to current element instead of 
//	GETCURRENTELEM->addMember(cyli);   //always to OVERLAY - DJB 10/25/96

	// Do not free/delete the cylinder...it is being turned over to the 3D hierarchy's control
	AfxGetApp()->m_pMainWnd->SendMessage( UM_NEW_3D_ELEMENT, (WPARAM)cyli, (LPARAM)NULL );

	free_vector(v,1,13);

	for( i=0; i<m_listEdges.GetCount(); i++ )
	{
		free_matrix(plEdgePnts[i], 0, NumPntsPerEdge[i]-1, 1,3);
	}
	free(names);
	free(plEdgePnts);
	free(NumPntsPerEdge);
	free_matrix(tMatrices,0,m_listEdges.GetCount()-1,1,13);
	free_matrix(plDirPnts,0,m_listEdges.GetCount()-1,1,3);
	free_matrix(plEndPnts,0,1,1,3);
	free_matrix(tEndMatrices,0,1,1,13);

///////////////////////////////////////
	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->setSelectMode(FALSE);

	while( m_listEdges.GetCount() > 0 )
		delete m_listEdges.RemoveTail();

	DestroyWindow();
	//CDialog::OnOK();
}

void CCreateCylinderByEdgeDlg::OnCancel() 
{
	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->setSelectMode(FALSE);
	DestroyWindow();
	//CDialog::OnCancel();

}

BOOL CCreateCylinderByEdgeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_currentEdge = new Edge_2D;
	m_listEdges.AddTail(m_currentEdge);

	m_edge = 1;
	m_state = ON_EDGE;
	UpdateLabel(str[0]);
	UpdateButtons();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCreateCylinderByEdgeDlg::UpdateLabel(char *str)
{

	m_Label	= str;
	if( m_state == ON_EDGE || m_state == ON_POINT )
	{
		char temp[25];
		itoa(m_listEdges.GetCount(),temp,10);
		m_Label += temp;
	}
	UpdateData(FALSE);
}

void CCreateCylinderByEdgeDlg::processEdge() 
{
	if( !getSelectedEdge(m_currentEdge) || 
		m_currentEdge->m_iNumPoints<3)
	{
		AfxMessageBox("Failed - Getting Edge");
		return;
	}
	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->setSelectMode(FALSE);
	m_state = ON_POINT;
	UpdateLabel(str[1]);
}
void CCreateCylinderByEdgeDlg::processDirPoint() 
{
	//CPoint *p;
	if( !getSelectedPoint(m_currentEdge->m_dirPoint, m_currentEdge->m_szImageName) )
	{
		AfxMessageBox("Get Point Failed");
		return;
	}
	m_state = ON_EDGE;

	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->setSelectMode(TRUE);
	m_currentEdge = new Edge_2D;
	m_listEdges.AddTail(m_currentEdge);

	if( m_listEdges.GetCount() > 3 )
		UpdateLabel(str[2]);
	else
		UpdateLabel(str[0]);
}
int CCreateCylinderByEdgeDlg::processEndPoint() 
{
	if( m_state != ON_ENDPOINT1 && m_state != ON_ENDPOINT2 )
		return FALSE;
	if( !getSelectedPoint( m_state == ON_ENDPOINT1 ? m_endPoints[0] :
			m_endPoints[1], NULL ) )
	{
		AfxMessageBox("Get Point Failed");
		return FALSE;
	}
	
	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->setSelectMode(FALSE);
	m_state == ON_ENDPOINT1 ? UpdateLabel(str[4]) : UpdateLabel(str[5]);
//	if( m_listEdges.GetCount() > 3 )
//		UpdateLabel(str[2]);
//	else
//		UpdateLabel(str[0]);
	return TRUE;
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
#define FUNC "PrintMakeCylData"
void PrintMakeCylData( int nNumLines, int *nNumPnts, PNTLST *plpPnts, 
	PNTLST plTMatrix, PNTLST plDirPnts, PNTLST plEndPnts, 
	PNTLST plEndPntsTMatrix )
{
    int i, j;
    
    Output( 4, "Cylinder by edge input data:\n" );
    Output( 4, "number of lines = %d\n", nNumLines );
    
    for( i = 0; i < nNumLines; i++ )
    {
	Output( 4, "line %d\n", nNumPnts[i] );
	
	Output( 4, "  Pnts = \n" );
	for( j = 0; j < nNumPnts[i]; j++ )
	{
	    Output( 4, "    [%d] = %lf,%lf,%lf\n",
		    j, (*(plpPnts[i]))[j][1], (*(plpPnts[i]))[j][2], 
		    (*(plpPnts[i]))[j][3] );
	}
	
	Output( 4, "  TMatrix = \n" );
	for( j = 1; j <= 13; j++ )
	{
	    Output( 4, "    [%d] = %lf\n", j, 
		    (*plTMatrix)[i][j] );
	}
	
	Output( 4, "  Dir Pnt = %lf,%lf,%lf\n", 
		(*plDirPnts)[i][1], (*plDirPnts)[i][2], 
		(*plDirPnts)[i][3] );
    }
    
    
    for( i = 0; i < 2; i++ )
    {
	Output( 4, "  End Pnt[%d] = %lf,%lf,%lf\n",
		i, (*plEndPnts)[i][1], (*plEndPnts)[i][2], 
		(*plEndPnts)[i][3] );
	
	Output( 4, "  End Pnt TMatrix\n" );
	for( j = 1; j <= 13; j++ )
	{
	    Output( 4, "    [%d] = %lf\n", j, 
		    (*plEndPntsTMatrix)[i][j] );
	}
    }
    Output( 4, "\n" );

}
#undef FUNC

void CCreateCylinderByEdgeDlg::OnClose() 
{
	while( m_listEdges.GetCount() > 0 )
		delete m_listEdges.RemoveTail();
	
	CDialog::OnClose();
}

void CCreateCylinderByEdgeDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	while( m_listEdges.GetCount() > 0 )
		delete m_listEdges.RemoveTail();
	
}

void CCreateCylinderByEdgeDlg::UpdateButtons(void)
{
	//CButton
	int numEdges = m_listEdges.GetCount();

	switch( m_state )
	{
		case ON_EDGE:
			if( numEdges > 3 )
				m_Ok.EnableWindow(TRUE);
			else
				m_Ok.EnableWindow(FALSE);

			m_Back.EnableWindow(FALSE);
			m_Next.EnableWindow(TRUE);
			m_Cancel.EnableWindow(TRUE);
			break;

		case ON_POINT:
			if( numEdges >= 3 )
				m_Ok.EnableWindow(TRUE);
			else
				m_Ok.EnableWindow(FALSE);

			m_Back.EnableWindow(FALSE);
			m_Next.EnableWindow(TRUE);
			m_Cancel.EnableWindow(TRUE);
			break;

		case ON_ENDPOINT1:
		case ON_ENDPOINT2:
			m_Ok.EnableWindow(TRUE);
			m_Back.EnableWindow(FALSE);
			m_Next.EnableWindow(FALSE);
			m_Cancel.EnableWindow(TRUE);
			break;
	}
}
