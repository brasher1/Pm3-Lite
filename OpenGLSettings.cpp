// OpenGLSettings.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "OpenGLSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGLSettings dialog


COpenGLSettings::COpenGLSettings(CWnd* pParent /*=NULL*/)
	: CDialog(COpenGLSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenGLSettings)
	m_AlphaTest = FALSE;
	m_AutoNormal = FALSE;
	m_Blend = FALSE;
	m_ColorMaterial = FALSE;
	m_CullFace = FALSE;
	m_DepthTest = FALSE;
	m_Dither = FALSE;
	m_Fog = FALSE;
	m_Lighting = FALSE;
	m_LineSmooth = FALSE;
	m_LineStipple = FALSE;
	m_LogicOp = FALSE;
	m_Normalize = FALSE;
	m_PointSmooth = FALSE;
	m_PolygonSmooth = FALSE;
	m_PolygonStipple = FALSE;
	m_ScissorTest = FALSE;
	m_StencilTest = FALSE;
	m_Texture1D = FALSE;
	m_Texture2D = FALSE;
	//}}AFX_DATA_INIT
}


void COpenGLSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenGLSettings)
	DDX_Check(pDX, IDC_ALPHA_TEST, m_AlphaTest);
	DDX_Check(pDX, IDC_AUTO_NORMAL, m_AutoNormal);
	DDX_Check(pDX, IDC_BLEND, m_Blend);
	DDX_Check(pDX, IDC_COLOR_MATERIAL, m_ColorMaterial);
	DDX_Check(pDX, IDC_CULL_FACE, m_CullFace);
	DDX_Check(pDX, IDC_DEPTH_TEST, m_DepthTest);
	DDX_Check(pDX, IDC_DITHER, m_Dither);
	DDX_Check(pDX, IDC_FOG, m_Fog);
	DDX_Check(pDX, IDC_LIGHTING, m_Lighting);
	DDX_Check(pDX, IDC_LINE_SMOOTH, m_LineSmooth);
	DDX_Check(pDX, IDC_LINE_STIPPLE, m_LineStipple);
	DDX_Check(pDX, IDC_LOGIC_OP, m_LogicOp);
	DDX_Check(pDX, IDC_NORMALIZE, m_Normalize);
	DDX_Check(pDX, IDC_POINT_SMOOTH, m_PointSmooth);
	DDX_Check(pDX, IDC_POLYGON_SMOOTH, m_PolygonSmooth);
	DDX_Check(pDX, IDC_POLYGON_STIPPLE, m_PolygonStipple);
	DDX_Check(pDX, IDC_SCISSOR_TEST, m_ScissorTest);
	DDX_Check(pDX, IDC_STENCIL_TEST, m_StencilTest);
	DDX_Check(pDX, IDC_TEXTURE_1D, m_Texture1D);
	DDX_Check(pDX, IDC_TEXTURE_2D, m_Texture2D);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenGLSettings, CDialog)
	//{{AFX_MSG_MAP(COpenGLSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGLSettings message handlers

void COpenGLSettings::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void COpenGLSettings::OnOK() 
{
	long value = 0L;
	UpdateData(TRUE);

	if( m_AlphaTest )
		value = value | ALPHATEST;
	if( m_AutoNormal )
		value = value | AUTONORMAL;
	if( m_Blend )
		value = value | BLEND;
	if( m_ColorMaterial )
		value = value | COLORMATERIAL;
	if( m_CullFace ) 
		value = value | CULLFACE;
	if( m_DepthTest )
		value = value | DEPTHTEST;
	if( m_Dither )
		value = value | DITHER;
	if( m_Fog )
		value = value | FOG;
	if( m_Lighting )
		value = value | LIGHTING;
	if( m_LineSmooth )
		value = value | LINESMOOTH;
	if( m_LineStipple )
		value = value | LINESTIPPLE;
	if( m_LogicOp )
		value = value | LOGICOP;
	if( m_Normalize )
		value = value | NORMALIZE;
	if( m_PointSmooth )
		value = value | POINTSMOOTH;
	if( m_PolygonSmooth )
		value = value | POLYGONSMOOTH;
	if( m_PolygonStipple )
		value = value | POLYGONSTIPPLE;
	if( m_ScissorTest )
		value = value | SCISSORTEST;
	if( m_StencilTest )
		value = value | STENCILTEST;
	if( m_Texture1D ) 
		value = value | TEXTURE1D;
	if( m_Texture2D )
		value = value | TEXTURE2D;
	openGLbits = value;	
	CDialog::OnOK();
}
