#ifndef __MYMDI_H__
#define __MYMDI_H__
// mymdi.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyMdi window

class CMyMdi : public CWnd
{
// Construction
public:
    CMyMdi();

// Attributes
public:
    CBitmap m_bmpBackGround ;   
    CSize m_sizeClient ;
    
// Operations
public:

// Implementation
public:
    virtual ~CMyMdi();
    virtual WNDPROC* GetSuperWndProcAddr() ;
    static WNDPROC pfnSuper;

    
    // Generated message map functions
protected:
    //{{AFX_MSG(CMyMdi)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__MYMDI_H__