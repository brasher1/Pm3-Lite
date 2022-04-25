// PM3LiteConfigDoc.h : interface of the CPM3LiteConfigDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CPM3LiteConfigDoc : public CDocument
{
protected: // create from serialization only
	CPM3LiteConfigDoc();
	DECLARE_DYNCREATE(CPM3LiteConfigDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPM3LiteConfigDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPM3LiteConfigDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPM3LiteConfigDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
