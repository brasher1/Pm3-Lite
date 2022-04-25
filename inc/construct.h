// construct.h : header file
//
#ifndef __CONSTRUCT_H__
#define __CONSTRUCT_H__
/////////////////////////////////////////////////////////////////////////////
// Construction dialog

class Construction : public CPropertyPage
{
	DECLARE_DYNCREATE(Construction)

// Construction
public:
	Construction();
	~Construction();

// Dialog Data
	//{{AFX_DATA(Construction)
	enum { IDD = IDD_CONSTRUCTION };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(Construction)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(Construction)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#endif //__CONSTRUCT_H__
