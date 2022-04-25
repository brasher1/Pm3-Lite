// colormenu.h
//
#include "stdafx.h"

#include "colormenu.h"

#define MENU_HEIGHT	15
#define MENU_WIDTH	50


CColorMenu::CColorMenu()
{
	VERIFY(CreateMenu());
}

CColorMenu::~CColorMenu()
{
	Detach();
}

void CColorMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	int iWidthCheck = LOWORD(GetMenuCheckMarkDimensions());

	lpMIS->itemHeight = MENU_HEIGHT;
	lpMIS->itemWidth = MENU_WIDTH - iWidthCheck;
}

void CColorMenu::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
//	CDC *pDC = CDC::FromHandle(lpDIS->hDC);
//	enum_colors nColor = (enum_colors)lpDIS->itemData;

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	COLORREF cr = (COLORREF)lpDIS->itemData; // RGB in item data

	if (lpDIS->itemAction & ODA_DRAWENTIRE)
	{
		// Paint the color item in the color requested
		CBrush br(cr);
		pDC->FillRect(&lpDIS->rcItem, &br);
	}

	if ((lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		// item has been selected - hilite frame
		COLORREF crHilite = RGB(255-GetRValue(cr),
						255-GetGValue(cr), 255-GetBValue(cr));
		CBrush br(crHilite);
		pDC->FrameRect(&lpDIS->rcItem, &br);
	}

	if (!(lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove frame
		CBrush br(cr);
		pDC->FrameRect(&lpDIS->rcItem, &br);
	}
}

void CColorMenu::AppendColorMenuItem(UINT nID, COLORREF color)
{
	VERIFY(AppendMenu(MF_ENABLED | MF_OWNERDRAW, nID, (LPCSTR)color));
}
