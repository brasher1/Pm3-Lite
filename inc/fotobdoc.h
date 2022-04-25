// fotobdoc.h : interface of the CFotobrowDoc class
//
/////////////////////////////////////////////////////////////////////////////
#include "dib.h"
#include "dibpal.h"
//class CFotobrowView;
#include "fotobvw.h"

class CFotobrowDoc : public CDocument
{
protected: // create from serialization only
	CFotobrowDoc();
	DECLARE_DYNCREATE(CFotobrowDoc)

// Attributes
public:
	CDIB * GetDIB() { return m_pDIB; }
	CDIBPal * GetPalette() { return m_pPal; }
	CString szName;

private:
	CDIB * m_pDIB;
	CDIBPal * m_pPal;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFotobrowDoc)
	public:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL


	CFotobrowView *GetView();
	int CheckSelected();
	void SetSelected( int flag ) { GetView()->SetSelected(flag); }
	const char *GetImageName() {return GetPathName(); }
	void setImageName(const char *str);

// Implementation
public:
	virtual ~CFotobrowDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFotobrowDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFotobrowDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
};

/////////////////////////////////////////////////////////////////////////////
