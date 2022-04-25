// coordxfo.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "coordxfo.h"
#include "mathsubs.h"
#include "photog_math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoordXformDlg dialog


CCoordXformDlg::CCoordXformDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCoordXformDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCoordXformDlg)
	m_CalcEast = 0.0;
	m_CalcNorth = 0.0;
	m_CalcUp = 0.0;
	m_Point1East = 0.0;
	m_Point1North = 0.0;
	m_Point2East = 0.0;
	m_Point2North = 1000.0;
	m_AxisDir = 1;
	m_RealEast = 0.0;
	m_RealNorth = 0.0;
	m_RealUp = 0.0;
	//}}AFX_DATA_INIT
}


void CCoordXformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoordXformDlg)
	DDX_Text(pDX, IDC_CALC_EAST, m_CalcEast);
	DDX_Text(pDX, IDC_CALC_NORTH, m_CalcNorth);
	DDX_Text(pDX, IDC_CALC_UP, m_CalcUp);
	DDX_Text(pDX, IDC_POINT1_EAST, m_Point1East);
	DDX_Text(pDX, IDC_POINT1_NORTH, m_Point1North);
	DDX_Text(pDX, IDC_POINT2_EAST, m_Point2East);
	DDX_Text(pDX, IDC_POINT2_NORTH, m_Point2North);
	DDX_Radio(pDX, IDC_RADIO1, m_AxisDir);
	DDX_Text(pDX, IDC_REAL_EAST, m_RealEast);
	DDX_Text(pDX, IDC_REAL_NORTH, m_RealNorth);
	DDX_Text(pDX, IDC_REAL_UP, m_RealUp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCoordXformDlg, CDialog)
	//{{AFX_MSG_MAP(CCoordXformDlg)
	ON_BN_CLICKED(ID_MAKE_IDENTITY, OnMakeIdentity)
	ON_BN_CLICKED(ID_IMPORT_DATA, OnImportData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCoordXformDlg message handlers

void CCoordXformDlg::OnOK() 
{
	UpdateData(TRUE);

	VECTOR vDir1OldSys = vector( 1, 3 );
	VECTOR vDir2OldSys = vector( 1, 3 );
	VECTOR vDir1PtgSys;
	VECTOR vDir2PtgSys;
	VECTOR vXlateOldSys = vector( 1, 3 );
	VECTOR vXlateOldSysInPtgSys;
	VECTOR vXlateNewSys = vector( 1, 3 );
	VECTOR vOldXform;
	VECTOR vTemp;

	/* Get input data into TWW format */
	vDir1OldSys[1] = m_Point1East;	vDir1OldSys[2] = m_Point1North;	vDir1OldSys[3] = 0.0;
	vDir2OldSys[1] = m_Point2East;	vDir2OldSys[2] = m_Point2North;	vDir2OldSys[3] = 0.0;
	vXlateOldSys[1] = m_CalcEast;	vXlateOldSys[2] = m_CalcNorth;	vXlateOldSys[3] = m_CalcUp;
	vXlateNewSys[1] = m_RealEast;	vXlateNewSys[2] = m_RealNorth;	vXlateNewSys[3] = m_RealUp;

	/* Use a trick to get array to be 1 to 12 */
	vOldXform = (((CFotobrowApp *)AfxGetApp())->GetTransform()) - 1;

	/* Get inverse of current/old transform */
	VECTOR vOldXformInv = matinv(vOldXform);

	/* Convert inputs that are in current/old system to photog coordinate system */
	vDir1PtgSys = matpnt( vOldXformInv, vDir1OldSys );
	vDir2PtgSys = matpnt( vOldXformInv, vDir2OldSys );
	vXlateOldSysInPtgSys = matpnt( vOldXformInv, vXlateOldSys );		

	/* Calculate unit vector along new direction in photog coordinate system */
	vTemp = clone_vector( vDir2PtgSys, 1, 3, 1, 3 );
	subtract_vector( vTemp, 1, 3, vDir1PtgSys, 1, 3 );
	VECTOR vDirPtgSys = unit_vector( 3, vTemp );
	free_vector( vTemp, 1, 3 );

	/* If going to a negative axis, swap our direction */
	if( m_AxisDir > 1 )
		multvector( 3, vDirPtgSys, -1 );

	/* Calculate new rotation in photog coordinate system */
	VECTOR vNewXform = vector( 1, 12 );
	if( m_AxisDir == 0 || m_AxisDir == 2 )
	{
		vNewXform[1]=vDirPtgSys[1];	vNewXform[2]=0.0;	vNewXform[3]=vDirPtgSys[3];
		vNewXform[4]=vDirPtgSys[3];	vNewXform[5]=0.0;	vNewXform[6]=-vDirPtgSys[1];
		vNewXform[7]=0.0;			vNewXform[8]=1.0;	vNewXform[9]=0.0;
		vNewXform[10]=0.0;			vNewXform[11]=0.0;	vNewXform[12]=0.0;
	}
	else
	{
		vNewXform[1] = -vDirPtgSys[3];	vNewXform[2] = 0.0;		vNewXform[3] = vDirPtgSys[1];
		vNewXform[4] = vDirPtgSys[1];	vNewXform[5] = 0.0;		vNewXform[6] = vDirPtgSys[3];
		vNewXform[7] = 0.0;				vNewXform[8] = 1.0;		vNewXform[9] = 0.0;
		vNewXform[10]=0.0;				vNewXform[11] = 0.0;	vNewXform[12]=0.0;
	}

	/* Calculate new translation in photog coordinate system */
	vTemp = matpnt( vNewXform, vXlateOldSysInPtgSys );
	VECTOR vNewOrigin = clone_vector( vXlateNewSys, 1, 3, 1, 3 );
	subtract_vector( vNewOrigin, 1, 3, vTemp, 1, 3 );
	free_vector( vTemp, 1, 3 );

	vNewXform[10] = vNewOrigin[1];
	vNewXform[11] = vNewOrigin[2];
	vNewXform[12] = vNewOrigin[3];

	((CFotobrowApp *)AfxGetApp())->SetTransform(&vNewXform[1]);

	free_vector( vDir1OldSys, 1, 3 );
	free_vector( vDir2OldSys, 1, 3 );
	free_vector( vDir1PtgSys, 1, 3 );
	free_vector( vDir2PtgSys, 1, 3 );
	free_vector( vXlateOldSys, 1, 3 );
	free_vector( vXlateOldSysInPtgSys, 1, 3 );
	free_vector( vXlateNewSys, 1, 3 );
	/* DO NOT call free_vector on vOldXform */
	free_vector( vOldXformInv, 1, 12 );
	free_vector( vDirPtgSys, 1, 3 );
	free_vector( vNewOrigin, 1, 3 );
	free_vector( vNewXform, 1, 12 );

	CDialog::OnOK();

#if FALSE
	/* This is not the correct way to do this.  The inputs (m_Point2, mPoint1, and m_Real)
	 * should have the inverse of the current transform applied to them.  This will allow this
	 * to continue to function after one auxiliary coordinate system has been defined.  If this is
	 * done, the deltaz = -deltaz line would be unneccesary, as well as the negatives on the
	 * m_RealNorth and m_CalcNorth coordinates in the calculation of v[10-12].
	 *
	 * Also, the output to the grid is not correct.  It is not correct because the default
	 * transform is not 100 010 001 000, it is 100 00-1 010 000. This would map E to X, N to Y,
	 * and U to Z.
	 */

	double deltax, deltaz;

	UpdateData(TRUE);
	
	deltax = m_Point2East - m_Point1East;
	deltaz = m_Point2North - m_Point1North;
deltaz = -deltaz;
		
	double mag = sqrt( deltax*deltax + deltaz*deltaz);
	deltax /= mag;
	deltaz /= mag;

	if( m_AxisDir > 1 )
	{
//		AfxMessageBox("Negating");
		deltax = -deltax;  deltaz = -deltaz;
	}
	VECTOR v = vector( 1, 13 );
	if( m_AxisDir == 0 || m_AxisDir == 2 )
	{
//		AfxMessageBox("East Loop");
		v[1]=deltax; 	v[2]=0.0;		v[3]=deltaz;
		v[4]=deltaz;	v[5]=0.0;		v[6]=-deltax;
		v[7]=0.0;		v[8]=1.0;		v[9]=0.0;
	}
	else
	{
//		AfxMessageBox("North Loop");
		v[1]=-deltaz; 	v[2]=0.0;		v[3]=deltax;
		v[4]=deltax;	v[5]=0.0;		v[6]=deltaz;
		v[7]=0.0;		v[8]=1.0;		v[9]=0.0;
	}
	// From 3D point and To 3D Point on two diff coord systems
/*	VECTOR my_v = clone_vector(v,1,13,1,13);
	my_v[10] += m_RealEast - (v[1]*m_CalcEast + v[2]*m_CalcNorth + v[3]*m_CalcUp + my_v[10] );
	my_v[11] += m_RealNorth - (v[4]*m_CalcEast + v[5]*m_CalcNorth + v[6]*m_CalcUp + my_v[11] );
	my_v[12] += m_RealUp - (v[7]*m_CalcEast + v[8]*m_CalcNorth + v[9]*m_CalcUp + my_v[12] );
  */
//	v[10] = m_RealEast - (v[1]*m_CalcEast + v[2]*m_CalcNorth + v[3]*m_CalcUp );
//	v[11] = m_RealNorth - (v[4]*m_CalcEast + v[5]*m_CalcNorth + v[6]*m_CalcUp );
//	v[12] = m_RealUp - (v[7]*m_CalcEast + v[8]*m_CalcNorth + v[9]*m_CalcUp );
	v[10] = m_RealEast - (v[1]*m_CalcEast + v[2]*m_CalcUp + v[3]*(-m_CalcNorth));
	v[11] = m_RealUp - (v[4]*m_CalcEast + v[5]*m_CalcUp + v[6]*(-m_CalcNorth));
	v[12] = m_RealNorth - (v[7]*m_CalcEast + v[8]*m_CalcUp + v[9]*(-m_CalcNorth) );

	double newx, newy, newz;
	/*
	m_CalcEast = 
	m_CalcNorth = 
	m_CalcUp = 
	  */
//	newx = (v[1]*m_CalcEast + v[2]*m_CalcNorth + v[3]*m_CalcUp + v[10] );
//	newy = (v[4]*m_CalcEast + v[5]*m_CalcNorth + v[6]*m_CalcUp + v[11] );
//	newz = (v[7]*m_CalcEast + v[8]*m_CalcNorth + v[9]*m_CalcUp + v[12] );
	newx = (v[1]*m_CalcEast + v[2]*m_CalcUp + v[3]*m_CalcNorth + v[10] );
	newy = (v[4]*m_CalcEast + v[5]*m_CalcUp + v[6]*m_CalcNorth + v[11] );
	newz = (v[7]*m_CalcEast + v[8]*m_CalcUp + v[9]*m_CalcNorth + v[12] );

/*
	char temp[256];
	sprintf(temp, "v1=%f, v2=%f, v3=%f\r\nv4=%f, v5=%f, v6=%f\r\nv7=%f, v8=%f, v9=%f\r\n"
			"v[10]=%f, v[11]=%f, v[12]=%f\r\nx=%f, y=%f, z=%f",
		v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], newx, newy, newz );
	AfxMessageBox(temp);
*/
	((CFotobrowApp *)AfxGetApp())->SetTransform(&v[1]);

	CDialog::OnOK();
#endif	// #if FALSE
}

void CCoordXformDlg::OnMakeIdentity() 
{
	// TODO: Add your control notification handler code here
	double x[12];
	x[0]=1.0; x[1]=0.0; x[2]=0.0;
	x[3]=0.0; x[4]=0.0; x[5]=-1.0;
	x[6]=0.0; x[7]=1.0; x[8]=0.0;
	x[9]=0.0; x[10]=0.0; x[11]=0.0;

	((CFotobrowApp *)AfxGetApp())->SetTransform(x);
	OnCancel();
}

void CCoordXformDlg::OnImportData() 
{
//	int i;

	
}
