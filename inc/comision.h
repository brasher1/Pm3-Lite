// comision.h : header file
//
#ifndef __COMISION_H__
#define __COMISION_H__
/////////////////////////////////////////////////////////////////////////////
// Commisioning dialog

class Commisioning : public CPropertyPage
{
	DECLARE_DYNCREATE(Commisioning)

// Construction
public:
	Commisioning();
	~Commisioning();

// Dialog Data
	//{{AFX_DATA(Commisioning)
	enum { IDD = IDD_COMMISIONING };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(Commisioning)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(Commisioning)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#endif //__COMISION_H__