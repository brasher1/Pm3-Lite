// engineer.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// EngineeringData dialog
#ifndef __ENGINEER_H__
#define __ENGINEER_H__

class EngineeringData : public CPropertyPage
{
	DECLARE_DYNCREATE(EngineeringData)

// Construction
public:
	EngineeringData();
	~EngineeringData();

// Dialog Data
	//{{AFX_DATA(EngineeringData)
	enum { IDD = IDD_ENGINEERING };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(EngineeringData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(EngineeringData)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif //__ENGINEER_H__