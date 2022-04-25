// mymdi.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "mymdi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyMdi

CMyMdi::CMyMdi()   
    : m_sizeClient(0,0)
{
//    VERIFY(m_bmpBackGround.LoadBitmap(IDB_ANNOY_BACKGROUND)) ; 
              
//    ASSERT(m_bmpBackGround.GetSafeHandle()) ;

}

CMyMdi::~CMyMdi()
{
//    if (m_bmpBackGround.GetSafeHandle())
//        m_bmpBackGround.DeleteObject() ;
}


BEGIN_MESSAGE_MAP(CMyMdi, CWnd)
    //{{AFX_MSG_MAP(CMyMdi)
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyMdi message handlers


BOOL CMyMdi::OnEraseBkgnd(CDC* pDC)
{
   COLORREF rgbBkColor = RGB(128,128,128) ; 
   //
   // Erase only the area needed
   //
   CRect rect;
   GetClientRect(&rect) ;
                                           
   //                                           
   // Make a brush to erase the background.
   //
   CBrush NewBrush(rgbBkColor) ; 

   pDC->SetBrushOrg(0,0) ;

   CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);   
   
   //
   // Paint the Background....
   //
   pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(),PATCOPY);  

/*
   //
   // Place the bitmap
   //                 
      BITMAP bm ;
      CDC dcMem ;
      
      VERIFY(m_bmpBackGround.GetObject(sizeof(bm), (LPVOID)&bm));
      
      dcMem.CreateCompatibleDC(NULL);
      CBitmap* pbmpOld = (CBitmap*)dcMem.SelectObject(&m_bmpBackGround); 
      ASSERT(pbmpOld) ;

      pDC->SetBkColor(rgbBkColor) ;
      
      pDC->BitBlt((rect.right-bm.bmWidth)>>1,   // centered
                (rect.bottom-bm.bmHeight)>>1,
                bm.bmWidth,
                bm.bmHeight,
                &dcMem,
                0, 0,
                SRCCOPY);
  
   //
   // CleanUp
   //    
   dcMem.SelectObject(pbmpOld) ;
*/   
   pDC->SelectObject(pOldBrush);                                           
   return TRUE;
}      

////////////////////////////////////////////////////////////////////
//
//
//
WNDPROC  CMyMdi::pfnSuper = NULL;

WNDPROC* CMyMdi::GetSuperWndProcAddr()
{
    return &pfnSuper;
}


void CMyMdi::OnSize(UINT nType, int cx, int cy)
{
    Default() ;                
    // if the app is just starting up, save the window
    // dimensions and get out

    if ((m_sizeClient.cx == 0) && (m_sizeClient.cy==0))
        {
        m_sizeClient.cx = cx;
        m_sizeClient.cy = cy;

        return ;
        }

    // if the size hasn't changed, break and pass to default

    if ((m_sizeClient.cx == cx) && ( m_sizeClient.cy == cy))
    { 
        return ;
    }

    // window size has changed so save new dimensions and force
    // entire background to redraw, including icon backgrounds

    m_sizeClient.cx = cx;
    m_sizeClient.cy = cy;
             
    RedrawWindow(NULL, NULL,
        RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN);    
            
    return ;                
}
