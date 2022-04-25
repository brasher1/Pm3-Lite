// __TREEMENU_H__
//
#ifndef __TREEMENU_H__
#define __TREEMENU_H__

int EquipRightClick( struct _func_table *pFunc, 
					HTREEITEM item, CWnd &wndObj, CPoint &p, int submenu );
int ElementListRightClick( struct _func_table *pFunc, 
					   HTREEITEM item, CWnd &winObj, CPoint &p, int submenu );
int PartsRightClick( struct _func_table *pFunc, 
					HTREEITEM item, CWnd &winObj, CPoint &p, int submenu );
int ImagesRightClick( struct _func_table *pFunc, 
					 HTREEITEM item, CWnd &winObj, CPoint &p, int submenu );
int ProceduresRightClick( struct _func_table *pFunc, 
						 HTREEITEM item, CWnd &winObj, CPoint &p, int submenu );
int CommissionRightClick( struct _func_table *pFunc, 
						 HTREEITEM item, CWnd &winObj, CPoint &p, int submenu );
int HistoryRightClick( struct _func_table *pFunc, 
					  HTREEITEM item, CWnd &winObj, CPoint &p, int submenu );

struct _func_table
{
	long dba;
	LPARAM elem;
	int (* rightClick)(struct _func_table *pFunc, 
		HTREEITEM item, CWnd &wndObj,CPoint &p, int submenu);
	int (* leftClick)(struct _func_table *pFunc, 
		HTREEITEM item, CWnd &wndObj, CPoint &p, int submenu);
	int (* doubleClick)(struct _func_table *pFunc, 
		HTREEITEM item, CWnd &wndObj, CPoint &p, int submenu);
};

typedef struct _func_table		FuncTbl;


#endif //__TREEMENU_H__
//
