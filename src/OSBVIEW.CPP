// osbview.cpp : implementation of the COSBView class
//

#include "stdafx.h"
#include "fotobrow.h"
#include "dib.h"
#include "dibpal.h"
#include "osbview.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COSBView

IMPLEMENT_DYNCREATE(COSBView, CScrollView)

BEGIN_MESSAGE_MAP(COSBView, CScrollView)
    //{{AFX_MSG_MAP(COSBView)
    ON_WM_PALETTECHANGED()
    ON_WM_QUERYNEWPALETTE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COSBView construction/destruction

COSBView::COSBView()
{
    m_pDIB = NULL;
    m_pPal = NULL;
    m_pOneToOneClrTab = NULL;
    m_bUseCreateDIBSection = FALSE;
    m_hbmSection = NULL;

    // See whether we are on Win32s, which exports CreateDIBSection but
    // does not support it
    DWORD dwVer = GetVersion();
    if ((dwVer & 0x800000FF) != 0x080000003) {
        // Not on Win32s so try to get the CreateDIBSection procedure address.
        HANDLE hMod = ::GetModuleHandle("gdi32");
        if (hMod) {
            m_pCDSProc = (CDSPROC*) GetProcAddress((HMODULE) hMod, "CreateDIBSection");
            if (m_pCDSProc) {
                m_bUseCreateDIBSection = TRUE;
            }
        }
    }
}

COSBView::~COSBView()
{
    if (m_pDIB) delete m_pDIB;
    if (m_pPal) delete m_pPal;
    if (m_pOneToOneClrTab) free(m_pOneToOneClrTab);
    if (m_hbmSection) ::DeleteObject(m_hbmSection);
}

// Create a new buffer, tables and palette to match a supplied DIB.
BOOL COSBView::Create(CDIB* pDIB)
{
    // Create the 1-to-1 palette index table.
    if (m_pOneToOneClrTab) free(m_pOneToOneClrTab);
    m_pOneToOneClrTab = 
        (LPBITMAPINFO) malloc(sizeof(BITMAPINFOHEADER)
                              + 256 * sizeof(WORD));
    if (!m_pOneToOneClrTab) {
        TRACE("Failed to create color table");
        return FALSE;
    }

    // Set up the table header to match the DIB
    // by copying the header and then constructing the 1-to-1
    // index translation table.
    memcpy(m_pOneToOneClrTab,
           pDIB->GetBitmapInfoAddress(),
           sizeof(BITMAPINFOHEADER));
    // Make sure the length of the table is set to 256 and not to
    // the number of colors in the DIB (which is irrelevant).
    m_pOneToOneClrTab->bmiHeader.biClrUsed = 0; // Default (256)
    WORD* pIndex;
    pIndex = (LPWORD)((LPBYTE)m_pOneToOneClrTab + sizeof(BITMAPINFOHEADER));
    for (int i = 0; i < 256; i++) {
        *pIndex++ = (WORD) i;
    }

    // Create a palette from the DIB so that we can use it to do
    // screen drawing.
    if (m_pPal) delete m_pPal;
    m_pPal = new CDIBPal;
    ASSERT(m_pPal);
    if (!m_pPal->Create(pDIB)) {
        TRACE("Failed to create palette");
        delete m_pPal;
        m_pPal = NULL;
        return FALSE;
    } else {
        // Map the colors so that we get an identity palette.
        m_pPal->SetSysPalColors();
    }

    // Delete any existing DIB and create a new one.
    if (m_pDIB) delete m_pDIB;
    m_pDIB = new CDIB;
    BOOL bResult = FALSE;
    if (m_bUseCreateDIBSection) {
        if (m_hbmSection) ::DeleteObject(m_hbmSection);
        ASSERT(m_pCDSProc);
        CDC* pDC = GetDC();
        CPalette* pPalOld = pDC->SelectPalette(m_pPal, FALSE);
        pDC->RealizePalette();
        BYTE *pBits = NULL;
        m_hbmSection = (*m_pCDSProc)(pDC->GetSafeHdc(),
                                     m_pOneToOneClrTab,
                                     DIB_PAL_COLORS,
                                     (VOID**) &pBits,
                                     NULL,
                                     0);
        pDC->SelectPalette(pPalOld, FALSE);
        ASSERT(m_hbmSection);
        ASSERT(pBits);
        ReleaseDC(pDC);
        bResult = m_pDIB->Create(pDIB->GetBitmapInfoAddress(), pBits);
    } else {
        bResult = m_pDIB->Create(pDIB->GetWidth(), pDIB->GetHeight());
    } 
    if (!bResult) {
        TRACE("Failed to create off-screen DIB");
        delete m_pDIB;
        m_pDIB = NULL;
        return FALSE;
    }

    CSize sizeTotal;
    sizeTotal.cx = m_pDIB->GetWidth();
    sizeTotal.cy = m_pDIB->GetHeight();
    SetScrollSizes(MM_TEXT, sizeTotal);


    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COSBView drawing

void COSBView::OnInitialUpdate()
{
    CSize sizeTotal;
    if (m_pDIB) {
        sizeTotal.cx = m_pDIB->GetWidth();
        sizeTotal.cy = m_pDIB->GetHeight();
    } else {
        sizeTotal.cx = 640;
        sizeTotal.cy = 480;
    }
    SetScrollSizes(MM_TEXT, sizeTotal);
}

void COSBView::OnDraw(CDC* pDC)
{
    Draw();
}

/////////////////////////////////////////////////////////////////////////////
// COSBView diagnostics

#ifdef _DEBUG
void COSBView::AssertValid() const
{
    CScrollView::AssertValid();
}

void COSBView::Dump(CDumpContext& dc) const
{
    CScrollView::Dump(dc);
}

CDocument* COSBView::GetDocument() // non-debug version is inline
{
    return m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COSBView message handlers

// Draw a section of the off-screen image buffer to the screen.
void COSBView::Draw(CRect* pRect)
{
    CClientDC dc(this);
    CRect rcDraw;

    // Make sure we have what we need to do a paint.
    if (!m_pDIB || !m_pOneToOneClrTab) {
        TRACE("No DIB or color table to paint from");
        return;
    }

    // See if a clip rect was supplied, and use the client area if not.
    if (pRect) {
        rcDraw = *pRect;
    } else {
        GetClientRect(rcDraw);
    }

    // Get the clip box.
    CRect rcClip;
    dc.GetClipBox(rcClip);

    // Create a rect for the DIB.
    CRect rcDIB;
    rcDIB.left = rcDIB.top = 0;
    rcDIB.right = m_pDIB->GetWidth() - 1;
    rcDIB.bottom = m_pDIB->GetHeight() - 1;

    // Find a rectangle that describes the intersection of the draw
    // rect, clip rect, and DIB rect.
    CRect rcBlt = rcDraw & rcClip & rcDIB;

    // Copy the update rectangle from the off-screen DC to the
    // window DC. Note that the DIB origin is the bottom-left corner.
    int w, h, xs, xd, yd, ys;
    w = rcBlt.right - rcBlt.left;
    h = rcBlt.bottom - rcBlt.top;
    xs = xd = rcBlt.left;
    yd = rcBlt.top;
    if (m_bUseCreateDIBSection) {
        ys = rcBlt.top;
    } else {
        ys = m_pDIB->GetHeight() - rcBlt.bottom;
    }

    // If we have a palette, select and realize it.
    CPalette* ppalOld = NULL;
    if (m_pPal) {
        ppalOld = dc.SelectPalette(m_pPal, 0);
        dc.RealizePalette();
    }

    BYTE* pBits;
    if (m_bUseCreateDIBSection) {
        HDC dcMem = ::CreateCompatibleDC(dc.GetSafeHdc());
        HBITMAP hbmOld = (HBITMAP) ::SelectObject(dcMem, m_hbmSection);
        // Note: you do not need to select the palette into
        // the memory DC because the DIB section is using palette
        // index values not colors
        ::BitBlt(dc.GetSafeHdc(),
                 xd, yd,
                 w, h,
                 dcMem,
                 xs, ys,
                 SRCCOPY);
        ::SelectObject(dcMem, hbmOld);
        ::DeleteDC(dcMem);
    } else {
        pBits = (BYTE*)m_pDIB->GetBitsAddress();
        ::StretchDIBits(dc.GetSafeHdc(),
                    xd,                 // Destination x
                    yd,                 // Destination y
                    w,                  // Destination width
                    h,                  // Destination height
                    xs,                 // Source x
                    ys,                 // Source y
                    w,                  // Source width
                    h,                  // Source height
                    pBits,              // Pointer to bits
                    m_pOneToOneClrTab,  // BITMAPINFO
                    DIB_PAL_COLORS,     // Options
                    SRCCOPY);           // Rop
    }

    // Select old palette if we altered it.
    if (ppalOld) dc.SelectPalette(ppalOld, 0);
}

void COSBView::OnPaletteChanged(CWnd* pFocusWnd)
{
    // See if the change was caused by us and ignore it if not
    if (pFocusWnd != this) {
        OnQueryNewPalette();
    }
}

// Note: Windows actually ignores the return value.
BOOL COSBView::OnQueryNewPalette()
{
    // We are going active so realize our palette
    if (m_pPal) {
        CDC* pdc = GetDC();
        CPalette* poldpal = pdc->SelectPalette(m_pPal, FALSE);
        UINT u = pdc->RealizePalette();
        ReleaseDC(pdc);
        if (u != 0) {
            // Some colors changed so we need to do a repaint.
            InvalidateRect(NULL, TRUE); // Repaint the lot.
            return TRUE; // Say we did something.
        }
    }
    return FALSE; // Say we did nothing.
}
