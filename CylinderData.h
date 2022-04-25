#ifndef __CCYLINDERDATA_H__
#define __CCYLINDERDATA_H__

// CylinderData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCylinderData dialog

class CCylinderData : public CDialog
{
// Construction
public:
	CCylinderData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCylinderData)
	enum { IDD = IDD_CYLINDER };
	double	m_EndEast;
	double	m_EndHeight;
	double	m_EndNorth;
	double	m_Height;
	double	m_Radius;
	double	m_StartEast;
	double	m_StartHeight;
	double	m_StartNorth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCylinderData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void setValues(double radius, double height, double *endpoints);
	void getValues(double *radius, double *height, double *endpoints);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCylinderData)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //__CCYLINDERDATA_H__
