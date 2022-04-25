#ifndef __COLORMENU_H__
#define __COLORMENU_H__

#define COLOR_MENU_BASE	33000

const COLORREF colors[] = 
{
	RGB(0,0,0),
	RGB(128,0,0),
	RGB(128,128,0),
	RGB(0,128,0),
	RGB(0,128,128),
	RGB(0,0,128),
	RGB(128,0,128),
	RGB(128,128,128),

	RGB(255,0,128),
	RGB(255,128,0),
	RGB(128,255,0),
	RGB(0,255,128),
	RGB(128,0,255),
	RGB(0,128,255),

	RGB(255,0,0),
	RGB(255,255,0),
	RGB(0,255,0),
	RGB(0,255,255),
	RGB(0,0,255),
	RGB(255,0,255),
	RGB(255,255,255)
};


class CColorMenu : public CMenu
{
public:
	enum enum_colors {BLACK};


// Operations
//	void ChangeMenuItem(UINT nID, enum_colors color);
//	void Init();
	void AppendColorMenuItem(UINT nID, COLORREF color);

// Implementation
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

	CColorMenu();
	virtual ~CColorMenu();

private:
	CSize m_sizeCheck;
//	CSimpleDIB m_DIB[3];
};

#endif //__COLORMENU_H__