// CreateCylinderByEndpointsDlg.h : header file
//
#ifndef __CCreateCylinderByEndpointsDlg_h__
#define __CCreateCylinderByEndpointsDlg_h__
/////////////////////////////////////////////////////////////////////////////
// CCreateCylinderByEndpointsDlg dialog

class CCreateCylinderByEndpointsDlg : public CDialog
{
// Construction
public:
	CCreateCylinderByEndpointsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateCylinderByEndpointsDlg)
	enum { IDD = IDD_ENDPOINTCYLINDER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateCylinderByEndpointsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateCylinderByEndpointsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif	// #ifdef __CCreateCylinderByEndpointsDlg_h__
