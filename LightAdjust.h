// LightAdjust.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLightAdjust dialog

class CLightAdjust : public CDialog
{
// Construction
public:
	CLightAdjust(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLightAdjust)
	enum { IDD = IDD_LIGHT_ADJUST };
	CSliderCtrl	m_LightRed;
	CSliderCtrl	m_LightGreen;
	CSliderCtrl	m_LightBlue;
	CSliderCtrl	m_LightAlpha;
	float	m_AlphaValue;
	float	m_BlueValue;
	float	m_GreenValue;
	float	m_RedValue;
	BOOL	m_SynchRGB;
	//}}AFX_DATA

	float m_red,m_green,m_blue,m_alpha;

	void setLightValues(float red, float green, float blue, float alpha)
	{
		m_RedValue = red;
		m_GreenValue = green;
		m_BlueValue = blue;
		m_AlphaValue = alpha;
	}
	void getLightValues(float *red, float *green, float *blue, float *alpha)
	{
		*red = m_RedValue;
		*green = m_GreenValue;
		*blue = m_BlueValue;
		*alpha = m_AlphaValue;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightAdjust)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLightAdjust)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
