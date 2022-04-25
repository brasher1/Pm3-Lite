// tritondb.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRaimaDatabaseDoc document

class CRaimaDatabaseDoc : public CDocument
{
protected:
	CRaimaDatabaseDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRaimaDatabaseDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRaimaDatabaseDoc)
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRaimaDatabaseDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CRaimaDatabaseDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
