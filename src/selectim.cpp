// selectim.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "selectim.h"

#include "photog.h"
#include "common.h"
#include <vista.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectImages dialog


CSelectImages::CSelectImages(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectImages::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectImages)
	m_SelectedString = _T("");
	//}}AFX_DATA_INIT
}


void CSelectImages::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectImages)
	DDX_Control(pDX, IDC_IMAGELIST, m_ImageList);
	DDX_LBString(pDX, IDC_IMAGELIST, m_SelectedString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectImages, CDialog)
	//{{AFX_MSG_MAP(CSelectImages)
	ON_LBN_DBLCLK(IDC_IMAGELIST, OnDblclkImagelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSelectImages message handlers

void CSelectImages::OnDblclkImagelist() 
{
	// TODO: Add your control notification handler code here
	char temp[128];
	UpdateData(TRUE);
//	AfxMessageBox(m_SelectedString);
	
	OpenImageByName( (char *)(const char *)m_SelectedString );

//	pTemplate->InitialUpdateFrame(pNewFrame, pDoc, TRUE);
}

BOOL CSelectImages::OnInitDialog() 
{
	CDialog::OnInitDialog();
	struct images image;
		
	int i = dt_recfrst(IMAGES, TASKER);
	while( i == S_OKAY )
	{
		dt_recread( &image, TASKER);
		m_ImageList.AddString( image.szName );
		i = dt_recnext(TASKER);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectImages::OnOK() 
{
	//CListBox
	int *indexes;
	UpdateData(TRUE);
	indexes = (int *)malloc(4096*sizeof(int));
	char name[128];
	int total = m_ImageList.GetSelItems(4096, indexes);
	for( int i=0; i<total; i++ )
	{
		m_ImageList.GetText( indexes[i], name );
		OpenImageByName( (char *)(const char *)name);
	}

	free(indexes);

	CDialog::OnOK();
}

