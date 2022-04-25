// OpenGLSettings.h : header file
//
#define ALPHATEST		0x00000001L
#define AUTONORMAL		0x00000002L
#define BLEND			0x00000004L
#define COLORMATERIAL	0x00000008L
#define CULLFACE		0x00000010L
#define DEPTHTEST		0x00000020L
#define DITHER			0x00000040L
#define FOG				0x00000080L
#define LIGHTING		0x00000100L
#define LINESMOOTH		0x00000200L
#define LINESTIPPLE		0x00000400L
#define LOGICOP			0x00000800L
#define NORMALIZE		0x00001000L
#define POINTSMOOTH		0x00002000L
#define POLYGONSMOOTH	0x00004000L
#define POLYGONSTIPPLE	0x00008000L
#define SCISSORTEST		0x00010000L
#define STENCILTEST		0x00020000L
#define TEXTURE1D		0x00040000L
#define TEXTURE2D		0x00080000L

/////////////////////////////////////////////////////////////////////////////
// COpenGLSettings dialog

class COpenGLSettings : public CDialog
{
// Construction
public:
	COpenGLSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COpenGLSettings)
	enum { IDD = IDD_OPENGL_SETTINGS };
	BOOL	m_AlphaTest;
	BOOL	m_AutoNormal;
	BOOL	m_Blend;
	BOOL	m_ColorMaterial;
	BOOL	m_CullFace;
	BOOL	m_DepthTest;
	BOOL	m_Dither;
	BOOL	m_Fog;
	BOOL	m_Lighting;
	BOOL	m_LineSmooth;
	BOOL	m_LineStipple;
	BOOL	m_LogicOp;
	BOOL	m_Normalize;
	BOOL	m_PointSmooth;
	BOOL	m_PolygonSmooth;
	BOOL	m_PolygonStipple;
	BOOL	m_ScissorTest;
	BOOL	m_StencilTest;
	BOOL	m_Texture1D;
	BOOL	m_Texture2D;
	//}}AFX_DATA

	long openGLbits;

	long getOpenGLBits( ) { return openGLbits; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGLSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenGLSettings)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
