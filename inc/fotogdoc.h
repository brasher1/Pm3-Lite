#ifndef __FOTOGDOC_H__
#define __FOTOGDOC_H__
// fotogdoc.h : header file
//
//#include "fotoview.h"

/////////////////////////////////////////////////////////////////////////////
// CFotogDoc document

class CFotogDoc : public CDocument
{
protected:
	CFotogDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFotogDoc)
	CPalette *m_pal;

// Attributes
public:

// Operations
public:
	CPalette * GetPalette() { return m_pal; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFotogDoc)
	public:
	virtual void OnFinalRelease();
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFotogDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFotogDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFotogDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
};

#endif
