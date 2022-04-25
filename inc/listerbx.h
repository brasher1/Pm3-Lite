#ifndef __LISTERBX_H__
#define __LISTERBX_H__
// searchbx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListerBox window

class CListerBox : public CComboBox
{
// Construction
public:
	CListerBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListerBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListerBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListerBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
