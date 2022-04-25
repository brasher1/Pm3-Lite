// CylinderData.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "CylinderData.h"
#include "ddxfxns.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCylinderData dialog


CCylinderData::CCylinderData(CWnd* pParent /*=NULL*/)
	: CDialog(CCylinderData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCylinderData)
	m_EndEast = 0.0;
	m_EndHeight = 0.0;
	m_EndNorth = 0.0;
	m_Height = 0.0;
	m_Radius = 0.0;
	m_StartEast = 0.0;
	m_StartHeight = 0.0;
	m_StartNorth = 0.0;
	//}}AFX_DATA_INIT
}


void CCylinderData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCylinderData)
	DDX_Length(pDX, IDC_END_EAST, m_EndEast);
	DDX_Length(pDX, IDC_END_HEIGHT, m_EndHeight);
	DDX_Length(pDX, IDC_END_NORTH, m_EndNorth);
	DDX_Length(pDX, IDC_HEIGHT, m_Height);
	DDX_Length(pDX, IDC_RADIUS, m_Radius);
	DDX_Length(pDX, IDC_START_EAST, m_StartEast);
	DDX_Length(pDX, IDC_START_HEIGHT, m_StartHeight);
	DDX_Length(pDX, IDC_START_NORTH, m_StartNorth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCylinderData, CDialog)
	//{{AFX_MSG_MAP(CCylinderData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCylinderData message handlers

BOOL CCylinderData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCylinderData::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CCylinderData::OnOK() 
{
	UpdateData(TRUE);
	
	CDialog::OnOK();
}

void CCylinderData::setValues(double radius, double height, double *endpoints)
{
	m_EndEast = endpoints[0];
	m_EndHeight = endpoints[2];
	m_EndNorth = endpoints[1];
	m_Height = height;
	m_Radius = radius;
	m_StartEast = endpoints[3];
	m_StartHeight = endpoints[5];
	m_StartNorth = endpoints[4];

//	UpdateData(FALSE);

}

void CCylinderData::getValues(double *radius, double *height, double *endpoints)
{
//	UpdateData(TRUE);

	endpoints[0] = m_EndEast;
	endpoints[2] = m_EndHeight;
	endpoints[1] = m_EndNorth;
	*height = m_Height;
	*radius = m_Radius;
	endpoints[3] = m_StartEast;
	endpoints[5] = m_StartHeight;
	endpoints[4] = m_StartNorth;
}