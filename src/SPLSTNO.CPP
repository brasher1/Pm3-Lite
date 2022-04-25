// splstno.cpp : implementation file
//

#include "stdafx.h"
#include "dib.h"
#include "dibpal.h"
#include "spriteno.h"
#include "splstno.h"
#include "sprite.h"
#include "spritlst.h"
#include "osbview.h"
#include "fotoview.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpriteListNotifyObj

CSpriteListNotifyObj::CSpriteListNotifyObj()
{
    m_pSpriteList = NULL;
    m_pFotoView = NULL;
}

CSpriteListNotifyObj::~CSpriteListNotifyObj()
{
}

// Notification callback from a CSprite object.
void CSpriteListNotifyObj::Change(CSprite* pSprite,
                                  CHANGETYPE change,
                                  CRect* pRect1,
                                  CRect* pRect2)
{
    if (change & CSpriteNotifyObj::ZORDER) {
        // Reposition the sprite in the z-order list.
        ASSERT(m_pSpriteList);
        m_pSpriteList->Reorder(pSprite);
        // Add the sprite position to the dirty list.
        ASSERT(m_pFotoView);
        m_pFotoView->AddDirtyRegion(pRect1);
    }
    if (change & CSpriteNotifyObj::POSITION) {
        // pRect1 and pRect2 point to old and new rectangle positions;
        // add these rectangles to the dirty list.
        ASSERT(m_pAnimView);
        m_pFotoView->AddDirtyRegion(pRect1);
        m_pFotoView->AddDirtyRegion(pRect2);
    }
}
