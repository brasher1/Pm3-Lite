// splitfra.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplitFrame frame

class CSplitFrame : public CMDIFrameWnd
{
	DECLARE_DYNCREATE(CSplitFrame)
protected:
	CSplitFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CSplitterWnd m_wndSplitter;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSplitFrame();

	// Generated message map functions
	//{{AFX_MSG(CSplitFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
