// LightAdjust.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "LightAdjust.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightAdjust dialog


CLightAdjust::CLightAdjust(CWnd* pParent /*=NULL*/)
	: CDialog(CLightAdjust::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLightAdjust)
	m_AlphaValue = 0.0f;
	m_BlueValue = 0.0f;
	m_GreenValue = 0.0f;
	m_RedValue = 0.0f;
	m_SynchRGB = FALSE;
	//}}AFX_DATA_INIT
}


void CLightAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightAdjust)
	DDX_Control(pDX, IDC_LIGHT_RED, m_LightRed);
	DDX_Control(pDX, IDC_LIGHT_GREEN, m_LightGreen);
	DDX_Control(pDX, IDC_LIGHT_BLUE, m_LightBlue);
	DDX_Control(pDX, IDC_LIGHT_ALPHA, m_LightAlpha);
	DDX_Text(pDX, IDC_VALUE_ALPHA, m_AlphaValue);
	DDX_Text(pDX, IDC_VALUE_BLUE, m_BlueValue);
	DDX_Text(pDX, IDC_VALUE_GREEN, m_GreenValue);
	DDX_Text(pDX, IDC_VALUE_RED, m_RedValue);
	DDX_Check(pDX, IDC_SYNCH_RGB, m_SynchRGB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLightAdjust, CDialog)
	//{{AFX_MSG_MAP(CLightAdjust)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightAdjust message handlers

BOOL CLightAdjust::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_LightRed.SetRange(0,100);
	m_LightGreen.SetRange(0,100);
	m_LightBlue.SetRange(0,100);
	m_LightAlpha.SetRange(0,100);
	
	m_LightRed.SetTicFreq(10);
	m_LightGreen.SetTicFreq(10);
	m_LightBlue.SetTicFreq(10);
	m_LightAlpha.SetTicFreq(10);

	m_LightRed.SetPos(m_RedValue*100);
	m_LightGreen.SetPos(m_GreenValue*100);
	m_LightBlue.SetPos(m_BlueValue*100);
	m_LightAlpha.SetPos(m_AlphaValue*100);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLightAdjust::OnOK() 
{
	m_RedValue = (float)(m_LightRed.GetPos())/100.0;
	m_GreenValue = (float)(m_LightGreen.GetPos())/100.0;
	m_BlueValue = (float)(m_LightBlue.GetPos())/100.0;
	m_AlphaValue = (float)(m_LightAlpha.GetPos())/100.0;
	
	CDialog::OnOK();
}

void CLightAdjust::OnCancel() 
{

	//CSliderControl
	CDialog::OnCancel();
}

void CLightAdjust::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	AfxMessageBox("Got an HScroll");
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLightAdjust::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
#define ALPHA	0x01L
#define RED		0x02L
#define GREEN	0x04L
#define BLUE	0x08L

	int mask = 0;
	UpdateData(TRUE);
/*	switch( nSBCode )	{	
		case SB_BOTTOM://   Scroll to bottom.
		case SB_ENDSCROLL://   End scroll.
		case SB_LINEDOWN://   Scroll one line down.
		case SB_LINEUP://   Scroll one line up.
		case SB_PAGEDOWN://   Scroll one page down.
		case SB_PAGEUP://   Scroll one page up.
		case SB_THUMBPOSITION://   Scroll to the absolute position. The current position is provided in nPos.
		case SB_THUMBTRACK://   Drag scroll box to specified position. The current position is provided in nPos.
		case SB_TOP://   Scroll to top.
			break;
	}*/
	int value;
	if( *(CSliderCtrl *)pScrollBar == m_LightRed )
	{
		value = m_LightRed.GetPos();
		m_RedValue = value/100.0;
		mask = mask ^ RED;
	}
	else if( *(CSliderCtrl *)pScrollBar == m_LightGreen )
	{
		value = m_LightGreen.GetPos();
		m_GreenValue = value/100.0;
		mask = mask ^ GREEN;
	}
	else if( *(CSliderCtrl *)pScrollBar == m_LightBlue )
	{
		value = m_LightBlue.GetPos();
		m_BlueValue = value/100.0;
		mask = mask ^ BLUE;
	}
	else 
	{
		value = m_LightAlpha.GetPos();
		m_AlphaValue = value/100.0;
		m_LightAlpha.SetPos(value);
		mask =  ALPHA;
	}	

	if( m_SynchRGB && !(mask&ALPHA))
	{
		m_RedValue = value/100.0;
		m_LightRed.SetPos(value);
		m_GreenValue = value/100.0;
		m_LightGreen.SetPos(value);
		m_BlueValue = value/100.0;
		m_LightBlue.SetPos(value);
	}

	UpdateData(FALSE);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
