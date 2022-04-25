// CreateQuickCylinderDlg.h : header file
//
#ifndef __CreateQuickCylinderDlg_h__
#define __CreateQuickCylinderDlg_h__

/////////////////////////////////////////////////////////////////////////////
// CCreateQuickCylinderDlg dialog

class CCreateQuickCylinderDlg : public CDialog
{
// Construction
public:
	CCreateQuickCylinderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateQuickCylinderDlg)
	enum { IDD = IDD_QUICKCYLINDER };
	int		m_WhichPoint;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateQuickCylinderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateQuickCylinderDlg)
	virtual void OnCancel();
	afx_msg void OnCalculate();
	afx_msg void OnAcceptPoint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	_Point3d *cyliPoints[4];
};

#endif	// #ifndef __CreateQuickCylinderDlg_h__