// CreateQuickCylinderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "CreateQuickCylinderDlg.h"
#include "3dclass/Ptg3DCylinderObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PHOTO_FLOAT dist_pnt_to_line(VECTOR pnt, VECTOR org, VECTOR dir);
VECTOR prj_pnt_onto_line( VECTOR pnt, VECTOR org, VECTOR dir);

/////////////////////////////////////////////////////////////////////////////
// CCreateQuickCylinderDlg dialog


CCreateQuickCylinderDlg::CCreateQuickCylinderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateQuickCylinderDlg::IDD, pParent)
{
	cyliPoints[0] = NULL;
	cyliPoints[1] = NULL;
	cyliPoints[2] = NULL;
	cyliPoints[3] = NULL;

	//{{AFX_DATA_INIT(CCreateQuickCylinderDlg)
	m_WhichPoint = 0;
	//}}AFX_DATA_INIT
}


void CCreateQuickCylinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateQuickCylinderDlg)
	DDX_Radio(pDX, IDC_POINT1, m_WhichPoint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateQuickCylinderDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateQuickCylinderDlg)
	ON_BN_CLICKED(IDOK, OnCalculate)
	ON_BN_CLICKED(IDC_ACCEPT_POINT, OnAcceptPoint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateQuickCylinderDlg message handlers

void CCreateQuickCylinderDlg::OnCancel() 
{
//	setSelectionMode(FALSE);
	CDialog::OnCancel();
}

void CCreateQuickCylinderDlg::OnCalculate() 
{
	if( !(cyliPoints[0] && cyliPoints[1] && cyliPoints[2] && cyliPoints[3]) )
	{
		AfxMessageBox( "Got here somehow without enough data" );
		return;
	}

	MATRIX *mCyliPoints = matrix( 0, 4, 1, 3 );
	for( int i = 0; i < 4; i++ )
	{
		(*mCyliPoints)[i][1] = cyliPoints[i]->x;
		(*mCyliPoints)[i][2] = cyliPoints[i]->y;
		(*mCyliPoints)[i][3] = cyliPoints[i]->z;
	}

	// Calculate center line
	VECTOR p12 = vector( 1, 3 );
	p12[1] = ((*mCyliPoints)[1][1] + (*mCyliPoints)[0][1]) / 2.0;
	p12[2] = ((*mCyliPoints)[1][2] + (*mCyliPoints)[0][2]) / 2.0;
	p12[3] = ((*mCyliPoints)[1][3] + (*mCyliPoints)[0][3]) / 2.0;

	VECTOR p34 = vector( 1, 3 );
	p34[1] = ((*mCyliPoints)[3][1] + (*mCyliPoints)[2][1]) / 2.0;
	p34[2] = ((*mCyliPoints)[3][2] + (*mCyliPoints)[2][2]) / 2.0;
	p34[3] = ((*mCyliPoints)[3][3] + (*mCyliPoints)[2][3]) / 2.0;

	VECTOR centerLine = vector( 1, 3 );
	centerLine[1] = p34[1] - p12[1];
	centerLine[2] = p34[2] - p12[2];
	centerLine[3] = p34[3] - p12[3];
	VECTOR vTemp = unit_vector( 3, centerLine );
	free_vector( centerLine, 1, 3 );
	centerLine = vTemp;

	// Calculate height
	VECTOR endPoint1 = prj_pnt_onto_line( (*mCyliPoints)[0], p12, centerLine );
	VECTOR endPoint2 = prj_pnt_onto_line( (*mCyliPoints)[2], p12, centerLine );

	vTemp = vector( 1, 3 );
	vTemp[1] = endPoint2[1] - endPoint1[1];
	vTemp[2] = endPoint2[2] - endPoint1[2];
	vTemp[3] = endPoint2[3] - endPoint1[3];

	// Calculate radius
	double radii[4];
	double cyliRadius = 0.0;
	for( i = 0; i < 4; i++ )
	{
		cyliRadius += dist_pnt_to_line( (*mCyliPoints)[i], endPoint1, centerLine );

		radii[i] = dist_pnt_to_line( (*mCyliPoints)[i], endPoint1, centerLine );
		TRACE( "radii[%d] = %lf\n", radii[i] );
	}
	cyliRadius /= 4.0;

	// Finalize height and radius
	double cyliHeight = vector_length( 3, vTemp );

	// Calculate the transform
	VECTOR transMatrix = vector( 1, 12 );
	transMatrix[10] = (endPoint2[1] + endPoint1[1]) / 2.0;
	transMatrix[11] = (endPoint2[2] + endPoint1[2]) / 2.0;
	transMatrix[12] = (endPoint2[3] + endPoint1[3]) / 2.0;

	free_vector( vTemp, 1, 3 );
	vTemp = unit_vector(3,centerLine);
	transMatrix[7] = vTemp[1];
	transMatrix[8] = vTemp[2];
	transMatrix[9] = vTemp[3];

	VECTOR vTemp2 = vector( 1, 3 );
	vTemp2[1] = (*mCyliPoints)[0][1] - endPoint1[1];
	vTemp2[2] = (*mCyliPoints)[0][2] - endPoint1[2];
	vTemp2[3] = (*mCyliPoints)[0][3] - endPoint1[3];

	VECTOR vTemp3 = unit_vector( 3, vTemp2 );
	VECTOR vTemp4 = cross_vector( vTemp, vTemp3 );
	transMatrix[1] = vTemp3[1];
	transMatrix[2] = vTemp3[2];
	transMatrix[3] = vTemp3[3];

	transMatrix[4] = vTemp4[1];
	transMatrix[5] = vTemp4[2];
	transMatrix[6] = vTemp4[3];

	// Create the cylinder node
	Ptg3DCylinderObj *newCyli = new Ptg3DCylinderObj;
	newCyli->setHeight( cyliHeight );
	newCyli->setRadius( cyliRadius );
	newCyli->setMatrix( transMatrix );
	char tempStr[256];
	sprintf( tempStr, "Obj%d", newCyli->getUniqID() );
	newCyli->setName(tempStr);
	
	// Do not free/delete the cylinder...it is being turned over to the 3D hierarchy's control
	AfxGetApp()->m_pMainWnd->SendMessage( UM_NEW_3D_ELEMENT, (WPARAM)newCyli, (LPARAM)NULL );

}

void CCreateQuickCylinderDlg::OnAcceptPoint() 
{
	_Point3d newPoint[3];

	char *bundleName = SearchForBundle();
	if( bundleName != NULL )		
	{
		if( Calc3DPoint(bundleName, newPoint) )
		{
			AfxMessageBox("Must select point in at least two images\n");
			return;
		}

		if( cyliPoints[m_WhichPoint] == NULL )
			cyliPoints[m_WhichPoint] = new _Point3d;

		cyliPoints[m_WhichPoint]->x = newPoint[0].x;
		cyliPoints[m_WhichPoint]->y = newPoint[0].y;
		cyliPoints[m_WhichPoint]->z = newPoint[0].z;

		char tempStr[256];
		sprintf( tempStr, "x=%.1lf, y=%.1lf, z=%.1lf",
				cyliPoints[m_WhichPoint]->x,
				cyliPoints[m_WhichPoint]->y,
				cyliPoints[m_WhichPoint]->z );
		SetDlgItemText( IDC_POINT1_TEXT + m_WhichPoint, tempStr );
	}
	else
	{
		MessageBox("No Common Bundle");
		return;
	}

	if( cyliPoints[0] && cyliPoints[1] && cyliPoints[2] && cyliPoints[3] )
		GetDlgItem(IDOK)->EnableWindow(TRUE);

	m_WhichPoint += 1;
	if( m_WhichPoint > 3 )
		m_WhichPoint = 0;
	UpdateData(FALSE);
}

BOOL CCreateQuickCylinderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
