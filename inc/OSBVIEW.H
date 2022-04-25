// osbview.h : interface of the COSBView class
//
/////////////////////////////////////////////////////////////////////////////

// define the CreateDIBSection function.
typedef HBITMAP (APIENTRY CDSPROC)
                (HDC hDC, 
                 BITMAPINFO* pbmi,
                 UINT iUsage, 
                 VOID** ppvBits,
                 HANDLE hSection,
                 DWORD dwOffset);

class CAnimDoc;

class COSBView : public CScrollView
{
protected: // Create from serialization only.
    COSBView();
    DECLARE_DYNCREATE(COSBView)

// Attributes
public:
    CDocument* GetDocument();
    CDIB* GetDIB() {return m_pDIB;}
    CDIBPal* GetPalette() {return m_pPal;}

// Operations
public:
    BOOL Create(CDIB* pDIB);             // Create a new buffer.
    void Draw(CRect* pClipRect = NULL);  // Draw off-screen buffer to screen.

// Implementation
public:
    virtual ~COSBView();
    virtual void OnDraw(CDC* pDC);  // Overridden to draw this view
    virtual void OnInitialUpdate(); // First time after construct


#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    CDIB *m_pDIB;           // The DIB buffer
    CDIBPal *m_pPal;        // Palette for drawing

private:
    BITMAPINFO* m_pOneToOneClrTab;  // Pointer to 1-to-1 color table
    BOOL m_bUseCreateDIBSection;    // Flag
    CDSPROC* m_pCDSProc;            // Pointer to CreateDIBSection
    HBITMAP m_hbmSection;           // Bitmap from section

// Generated message map functions
protected:
    //{{AFX_MSG(COSBView)
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
    afx_msg BOOL OnQueryNewPalette();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Debug version in osbview.cpp
inline CDocument* COSBView::GetDocument()
   { return (CDocument*) m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
