// PM3LiteConfigView.h : interface of the CPM3LiteConfigView class
//
/////////////////////////////////////////////////////////////////////////////

class CPM3LiteConfigView : public CView
{
protected: // create from serialization only
	CPM3LiteConfigView();
	DECLARE_DYNCREATE(CPM3LiteConfigView)

// Attributes
public:
	CPM3LiteConfigDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPM3LiteConfigView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPM3LiteConfigView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPM3LiteConfigView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PM3LiteConfigView.cpp
inline CPM3LiteConfigDoc* CPM3LiteConfigView::GetDocument()
   { return (CPM3LiteConfigDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
