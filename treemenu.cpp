// treemenu.cpp
//
#include "stdafx.h"
#include "resource.h"
#include "treemenu.h"
#include "colormenu.h"

int color_bmps[] = {
	IDB_OGL_BLACK,
	IDB_OGL_RED,
	IDB_OGL_GREEN,
	IDB_OGL_YELLOW,
	IDB_OGL_BLUE,
	IDB_OGL_PURPLE,
	IDB_OGL_CYAN,
	IDB_OGL_GREY,
	IDB_OGL_LT_RED,
	IDB_OGL_LT_GREEN,
	IDB_OGL_LT_YELLOW,
	IDB_OGL_LT_BLUE,
	IDB_OGL_LT_MAGENTA,
	IDB_OGL_LT_CYAN,
	IDB_OGL_LT_WHITE
};


int ElementListRightClick( struct _func_table *pFunc, HTREEITEM item, CWnd &winObj, CPoint &p, int submenu )
{
	CColorMenu m_colorMenu;

	CMenu menu;
	if( !menu.LoadMenu(IDR_MEMBER_LIST_POP_MENU) )
		AfxMessageBox( "ElementListRightClick() - LoadMenu failed" );

	CMenu *pMenu = menu.GetSubMenu(0);
	if( !pMenu )
		AfxMessageBox( "ElementListRightClick() - GetSubMenu failed" );

	for(int iColor = 0; iColor < sizeof(colors)/sizeof(colors[0]); iColor++)
		m_colorMenu.AppendColorMenuItem(COLOR_MENU_BASE+iColor, colors[iColor]);

	char szString[256];     // don't change the string

	int numBytes = pMenu->GetMenuString(ID_MEMBERLIST_COLOR, szString, sizeof(szString), MF_BYCOMMAND);

	BOOL bResult = pMenu->ModifyMenu(ID_MEMBERLIST_COLOR, MF_BYCOMMAND | MF_POPUP,
		(UINT)m_colorMenu.m_hMenu, szString);
	VERIFY(bResult);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,p.x,p.y, &winObj );
	menu.DestroyMenu();

	return 0;
}

#if FALSE

int EquipRightClick( struct _func_table *pFunc, HTREEITEM item, CWnd &winObj, CPoint &p, int submenu )
{
 //	AfxMessageBox("IN the Equip Right Click");
	CMenu menu;
//	menu.CreatePopupMenu();
	menu.LoadMenu(IDR_EQUIP_POP_MENU);
	menu.GetSubMenu(submenu)->TrackPopupMenu(TPM_LEFTALIGN,p.x,p.y, &winObj );
	menu.DestroyMenu();
	return 0;
}

int PartsRightClick( struct _func_table *pFunc, HTREEITEM item, CWnd &winObj, CPoint &p, int submenu )
{
//	AfxMessageBox("IN the Document Right Click");
	CMenu menu;
//	menu.CreatePopupMenu();
	menu.LoadMenu(IDR_PARTS_POP_MENU);
	menu.GetSubMenu(submenu)->TrackPopupMenu(TPM_LEFTALIGN,p.x,p.y, &winObj );
	menu.DestroyMenu();
	return 0;
}
int ImagesRightClick( struct _func_table *pFunc, HTREEITEM item, CWnd &winObj, CPoint &p, int submenu )
{
//	AfxMessageBox("IN the Document Right Click");
	CMenu menu;
//	menu.CreatePopupMenu();
	menu.LoadMenu(IDR_IMAGES_POP_MENU);
	menu.GetSubMenu(submenu)->TrackPopupMenu(TPM_LEFTALIGN,p.x,p.y, &winObj );
	menu.DestroyMenu();
	return 0;
}
int ProceduresRightClick( struct _func_table *pFunc, HTREEITEM item, CWnd &winObj, CPoint &p, int submenu )
{
//	AfxMessageBox("IN the Document Right Click");
	CMenu menu;
//	menu.CreatePopupMenu();
	menu.LoadMenu(IDR_PROCEDURES_POP_MENU);
	menu.GetSubMenu(submenu)->TrackPopupMenu(TPM_LEFTALIGN,p.x,p.y, &winObj );
	menu.DestroyMenu();
	return 0;
}
int CommissionRightClick( struct _func_table *pFunc, HTREEITEM item, CWnd &winObj, CPoint &p, int submenu )
{
//	AfxMessageBox("IN the Document Right Click");
	CMenu menu;
//	menu.CreatePopupMenu();
	menu.LoadMenu(IDR_COMMISSION_POP_MENU);
	menu.GetSubMenu(submenu)->TrackPopupMenu(TPM_LEFTALIGN,p.x,p.y, &winObj );
	menu.DestroyMenu();
	return 0;
}
int HistoryRightClick( struct _func_table *pFunc, HTREEITEM item, CWnd &winObj, CPoint &p, int submenu )
{
//	AfxMessageBox("IN the Document Right Click");
	CMenu menu;
//	menu.CreatePopupMenu();
	menu.LoadMenu(IDR_HISTORY_POP_MENU);
	menu.GetSubMenu(submenu)->TrackPopupMenu(TPM_LEFTALIGN,p.x,p.y, &winObj );
	menu.DestroyMenu();
	return 0;
}
#endif
