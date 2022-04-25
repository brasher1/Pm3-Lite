// sprite.cpp : implementation file
//

#include "stdafx.h"
#include "dib.h"
#include "spriteno.h"
#include "sprite.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSprite

IMPLEMENT_SERIAL(CSprite, CDIB, 0 /* Schema number */ )

CSprite::CSprite()
{
    m_x = 0;
    m_y = 0;
    m_z = 50;
    m_bTransIndex = 0;
    m_pNotifyObj = NULL;
}

CSprite::~CSprite()
{
}

// Set the initial state of the sprite from its DIB image.
void CSprite::Initialize()
{
    // Get the address of the top-left pixel.
    BYTE* p = (BYTE*)GetPixelAddress(0, 0);
    ASSERT(p);
    // Get the pixel value and save it.
    m_bTransIndex = *p;
}

/////////////////////////////////////////////////////////////////////////////
// CSprite serialization

void CSprite::Serialize(CArchive& ar)
{
    CDIB::Serialize(ar);
    if (ar.IsStoring()) {
    } else {
    }
}

/////////////////////////////////////////////////////////////////////////////
// CSprite commands

// Render a sprite to a DIB.
void CSprite::Render(CDIB* pDIB, CRect* pClipRect)
{
    ASSERT(pDIB);

    // Get the sprite rectangle.
    CRect rcDraw;
    GetRect(&rcDraw);

    // If a clip rectangle was supplied, see if the sprite
    // is visible inside the rectangle.
    if (pClipRect) {
        if (!rcDraw.IntersectRect(pClipRect, &rcDraw)) {
            return; // Not visible
        }
    }
    // Copy the image of the sprite
    // Copy the image of the sprite.
    CopyBits(pDIB,                          // Dest DIB
             rcDraw.left,                   // Dest x
             rcDraw.top,                    // Dest y
             rcDraw.right - rcDraw.left,    // Width
             rcDraw.bottom - rcDraw.top,    // Height
             rcDraw.left - m_x,             // Source x
             rcDraw.top - m_y,              // Source y
             PALETTEINDEX(m_bTransIndex));  // Transparent color index
}

// Load a sprite image from a disk file.
BOOL CSprite::Load(char* pszFileName)
{
    if (!CDIB::Load(pszFileName)) {
        return FALSE;
    }
    Initialize();
    return TRUE;
}

// Load a sprite image from a disk file
BOOL CSprite::Load(CFile* fp)
{
    if (!CDIB::Load(fp)) {
        return FALSE;
    }
    Initialize();
    return TRUE;
}

// Map colors to palette.
BOOL CSprite::MapColorsToPalette(CPalette* pPal)
{
    BOOL bResult = CDIB::MapColorsToPalette(pPal);
    // Get the transparency info again.
    Initialize();
    return bResult;
}

// Get the bounding rectangle.
void CSprite::GetRect(CRect* pRect)
{
    ASSERT(pRect);
    pRect->left = m_x;
    pRect->top = m_y;
    pRect->right = m_x + GetWidth();
    pRect->bottom = m_y + GetHeight();
}

// Test for a hit in a non-transparent area.
BOOL CSprite::HitTest(CPoint point)
{
    // Test if the point is inside the sprite rectangle.
    if ((point.x > m_x) 
    && (point.x < m_x + GetWidth())
    && (point.y > m_y)
    && (point.y < m_y + GetHeight())) {
        // See if this point is transparent by testing whether
        // the pixel value is the same as the top-left
        // corner value. Note that top-left of the
        // image is bottom left in the DIB.
        BYTE* p = (BYTE*)GetPixelAddress(point.x - m_x, point.y - m_y);
        if (*p != m_bTransIndex) {
            return TRUE; // Hit
        }
    }
    return FALSE;
}

// Set a new x,y position.
void CSprite::SetPosition(int x, int y)
{
    // Save the current position.
    CRect rcOld;
    GetRect(&rcOld);
    // Move to new position.
    m_x = x;
    m_y = y;
    CRect rcNew;
    GetRect(&rcNew);
    // Notify that we have moved from our old position to
    // our new position.
    if (m_pNotifyObj) {
        m_pNotifyObj->Change(this, 
                             CSpriteNotifyObj::POSITION,
                             &rcOld,
                             &rcNew);
    }
}

// Set a new Z-order.
void CSprite::SetZ(int z)
{
    if (m_z != z) {
        m_z = z;
        // See if we have to notify anyone.
        if (m_pNotifyObj) {
            CRect rc;
            GetRect(&rc);
            m_pNotifyObj->Change(this,
                                 CSpriteNotifyObj::ZORDER,
                                 &rc);
        }
    }
}
