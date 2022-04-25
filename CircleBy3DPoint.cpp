// CircleBy3DPoint.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "mainfrm.h"
#include "CircleBy3DPoint.h"
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

/////////////////////////////////////////////////////////////////////////////
// CCircleBy3DPoint dialog


CCircleBy3DPoint::CCircleBy3DPoint(CWnd* pParent /*=NULL*/)
	: CDialog(CCircleBy3DPoint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCircleBy3DPoint)
	m_iNumberOfPoints = 0;
	//}}AFX_DATA_INIT
}


void CCircleBy3DPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCircleBy3DPoint)
	DDX_Text(pDX, IDC_NUMBER_OF_POINTS, m_iNumberOfPoints);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCircleBy3DPoint, CDialog)
	//{{AFX_MSG_MAP(CCircleBy3DPoint)
	ON_BN_CLICKED(IDC_CALCULATE, OnCalculate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCircleBy3DPoint message handlers

void CCircleBy3DPoint::OnOK() 
{
	char *p;
	_Point3d point[3];

//    LOCK_REQUEST lr[] = {
//	{ BUNDLE, 'r' }, 
//	{ TMATRIX, 'r' }, 
//	{ IMAGES, 'r' }, 
//	{ CAMERA_INFO, 'r' }, 
//	{ PIXELS, 'r' }, 
//	{ POINTS, 'r' } };

	
//	if( dt_lock( 6, lr, TASKER ) != S_OKAY )
  //  {
	//	ErrorOutput( "Database Error Lock request failed.\n" );
	//	return;
//    }

	p = SearchForBundle();
	if( p != NULL )		
	{
//		MessageBox(p);
		if( Calc3DPoint(p, point ) )
		{
			AfxMessageBox("Not Enough Points Selected");
//			dt_freeall(&Mytask); //release all database locks
			return;
		}
//		MessageBox("Adding a row");
	    if( m_iNumberOfPoints == 0 )
			pl3DPnts = matrix( 0, 1, 1, 3 );
	    else
		    add_matrix_rows( pl3DPnts, 1 );
//		MessageBox("Setting the values");
		((*pl3DPnts)[m_iNumberOfPoints])[1] = point[0].x;
		((*pl3DPnts)[m_iNumberOfPoints])[2] = point[0].y;
		((*pl3DPnts)[m_iNumberOfPoints])[3] = point[0].z;
	}
	else
	{
		MessageBox("No Common Bundle");
//		dt_freeall(&Mytask); //release all database locks
		return;
	}
//	MessageBox("Incrementing point");
	m_iNumberOfPoints++;
	if( m_iNumberOfPoints >= 3 )
			GetDlgItem(IDC_CALCULATE)->EnableWindow(TRUE);
	UpdateData(FALSE);
//	dt_freeall(&Mytask); //release all database locks
//	CDialog::OnOK();
}

void CCircleBy3DPoint::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CCircleBy3DPoint::OnCalculate() 
{
/****************************************
Need to make circle here
****************************************/
    struct Cylinder cyl_rec;
    struct _transform trans;
 //   PhotogMsg msg;
 //   LOCK_REQUEST lr[] = {
//	{ BUNDLE, 'r' }, 
//	{ TMATRIX, 'r' }, 
//	{ IMAGES, 'r' }, 
//	{ CAMERA_INFO, 'r' }, 
//	{ PIXELS, 'r' }, 
//	{ POINTS, 'r' } };

	
//	if( dt_lock( 6, lr, TASKER ) != S_OKAY )
  //  {
	//	ErrorOutput( "Database Error Lock request failed.\n" );
	//	return;
//    }
//    
    memset( &cyl_rec, 0, sizeof(struct Cylinder) );
    memset( &trans, 0, sizeof(struct _transform) );

//    PrintMakeCircBy3DPntsData( nPointNo, pl3DPnts );

    make_circle_by_3dpnts( m_iNumberOfPoints, pl3DPnts, cyl_rec.radius, trans );

    cyl_rec.height = 1.0;

//    PrintMakeCircData( cyl_rec.cyl_radius, transform_rec );     
//fprintf( stderr, "%s: Need to add image names to cylinder output %s:%d\n",
//	FUNC, __FILE__, __LINE__ );
//    PtgOutput( &cyl_rec, &transform_rec, getCurrentBundleName(), "" );

    if( pl3DPnts != NULL )
    {
		free_matrix( pl3DPnts, 0, m_iNumberOfPoints, 1, 3 );
		pl3DPnts = NULL;
    }

    m_iNumberOfPoints = 0;

//////////////////////////////////////////////////////////////////////////////////////
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
	//		0.0		0.0		-1.0
	//		0.0		1.0		 0.0
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

//	dt_freeall(&Mytask); //release all database locks

	CCircleDataDlg *cirdlg = new CCircleDataDlg;
	cirdlg->setValues(cyl_rec.radius, cyl_rec.height, (double *)ptg );
	cirdlg->DoModal();
	cirdlg->getValues(&(cyl_rec.radius), &(cyl_rec.height), (double *)ptg);
	delete cirdlg;

//	GETOVERLAY->addMember(cyli);
	// Do not free/delete the cylinder...it is being turned over to the 3D hierarchy's control
	AfxGetApp()->m_pMainWnd->SendMessage( UM_NEW_3D_ELEMENT, (WPARAM)cyli, (LPARAM)NULL );

	free_vector(v,1,13);
//////////////////////////////////////////////////////////////////////////////////////
	CDialog::OnOK();
}

BOOL CCircleBy3DPoint::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_iNumberOfPoints=0;	
    pl3DPnts = NULL;

	GetDlgItem(IDC_CALCULATE)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
