// ElementListView.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "ElementListView.h"
#include "mainfrm.h"
#include "treemenu.h"
#include "colormenu.h"
#include "ObjDGNX/DgnFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int saveNodeToDGN( DGN dgn, Ptg3DNode *node );

/////////////////////////////////////////////////////////////////////////////
// CElementListView

IMPLEMENT_DYNCREATE(CElementListView, CFormView)

CElementListView::CElementListView()
	: CFormView(CElementListView::IDD)
{
	//{{AFX_DATA_INIT(CElementListView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CElementListView::~CElementListView()
{

/*	TRACE("Find me in tree - %d\n", i++);
		treeItem.hItem = child;
		treeItem.mask = TVIF_HANDLE;
		if( !(m_ElementList.GetItem(&treeItem)))*/

}

void CElementListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElementListView)
	DDX_Control(pDX, IDC_ELEMENT_LIST, m_ElementList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElementListView, CFormView)
	//{{AFX_MSG_MAP(CElementListView)
	ON_WM_CREATE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_ELEMENT_LIST, OnSelchangedElementList)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_ELEMENT_LIST, OnRclickElementList)
	ON_COMMAND(ID_MEMBERLIST_PROPERTIES, OnMemberlistProperties)
	ON_COMMAND(ID_MEMBERLIST_VISIBLE_SOLID, OnMemberlistVisibleSolid)
	ON_COMMAND(ID_MEMBERLIST_VISIBLE_WIREFRAME, OnMemberlistVisibleWireframe)
	ON_COMMAND(ID_MEMBERLIST_VISIBLE_INVISIBLE, OnMemberlistVisibleInvisible)
	ON_COMMAND(ID_MEMBERLIST_SETASREFERENCENODE, OnMemberlistSetAsReferenceNode)
	ON_COMMAND(ID_MEMBERLIST_CALC_CIRC_3DPOINTS, OnMemberListCalcCircBy3DPoints)
	ON_WM_DESTROY()
	ON_COMMAND(ID_MEMBERLIST_DELETE, OnMemberlistDelete)
	ON_COMMAND(ID_MEMBERLIST_SAVEDGN, OnMemberlistSavedgn)
	//}}AFX_MSG_MAP
	ON_COMMAND(COLOR_MENU_BASE+0, OnColorBlack)
	ON_COMMAND(COLOR_MENU_BASE+1, OnColorMaroon)
	ON_COMMAND(COLOR_MENU_BASE+2, OnColorOlive)
	ON_COMMAND(COLOR_MENU_BASE+3, OnColorGreen)
	ON_COMMAND(COLOR_MENU_BASE+4, OnColorTeal)
	ON_COMMAND(COLOR_MENU_BASE+5, OnColorNavy)
	ON_COMMAND(COLOR_MENU_BASE+6, OnColorWine)
	ON_COMMAND(COLOR_MENU_BASE+7, OnColorGrey)
	ON_COMMAND(COLOR_MENU_BASE+8, OnColorPink)
	ON_COMMAND(COLOR_MENU_BASE+9, OnColorOrange)
	ON_COMMAND(COLOR_MENU_BASE+10, OnColorChartruse)
	ON_COMMAND(COLOR_MENU_BASE+11, OnColorPurple)
	ON_COMMAND(COLOR_MENU_BASE+12, OnColorSkyBlue)
	ON_COMMAND(COLOR_MENU_BASE+13, OnColorRed)
	ON_COMMAND(COLOR_MENU_BASE+14, OnColorYellow)
	ON_COMMAND(COLOR_MENU_BASE+15, OnColorLime)
	ON_COMMAND(COLOR_MENU_BASE+16, OnColorAqua)
	ON_COMMAND(COLOR_MENU_BASE+17, OnColorBlue)
	ON_COMMAND(COLOR_MENU_BASE+18, OnColorMagenta)
	ON_COMMAND(COLOR_MENU_BASE+19, OnColorWhite)
	ON_MESSAGE(UM_CURRENT_ELEMENT_CHANGE, OnCurrentElementChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElementListView diagnostics

#ifdef _DEBUG
void CElementListView::AssertValid() const
{
	CFormView::AssertValid();
}

void CElementListView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CElementListView message handlers


int CElementListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect r;
	GetClientRect(&r);
	lpCreateStruct->cx = r.Width()/5;
	lpCreateStruct->cy = r.Height()/2;
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CElementListView::OnSelchangedElementList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_ITEM item;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	const char *p = ((Ptg3DNode *)(((FuncTbl *)(pNMTreeView->itemNew.lParam))->elem))->getName();
//	AfxMessageBox(p);

	Ptg3DNode *found = (Ptg3DNode *)(((FuncTbl *)(pNMTreeView->itemNew.lParam))->elem); 
    CWnd* pMainFrame = AfxGetApp()->m_pMainWnd;
	Ptg3DNode *found2;
	if( found->getName() == NULL ) 
		found2 = (Ptg3DNode *)(found->getOwner());
	else
		found2 = found;
	AfxGetApp()->m_pMainWnd->SendMessage(UM_CURRENT_ELEMENT_CHANGE,(WPARAM)found2,(LPARAM)found2);

	*pResult = 0;
}

void CElementListView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	GetParent()->SetWindowText(/*GetDocument()->GetTitle()+"-*/"ElementList");

	hIml = ImageList_Create( 16,  // width
	    16,                      // height
		0,                                  // flags
		4,                        // number of bitmaps
		0 );                                // amount to grow

	// load the bitmaps and add them to the image lists
	//HBITMAP hBmp1, hBmp2, hBmp3, hBmp4;
	hBmps[0] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_NODE));
	image_index[0] = ImageList_Add(hIml,hBmps[0],NULL);

	hBmps[1] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_EYE_CLOSED));
	image_index[1] = ImageList_Add(hIml,hBmps[1],NULL);

	hBmps[2] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_EYE_OPEN));
	image_index[2] = ImageList_Add(hIml,hBmps[2],NULL);

	hBmps[3] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_CYLINDER));
	image_index[3] = ImageList_Add(hIml,hBmps[3],NULL);

	for( int i=0; i<4; i++ )
		DeleteObject(hBmps[i]);

	//AfxMessageBox("Images");	

	CImageList *imgList = new CImageList();
	imgList->Attach(hIml);
	m_ElementList.SetImageList( imgList, TVSIL_NORMAL );

//	TV_INSERTSTRUCT TreeCtrlItem;
	m_ElementList.DeleteAllItems();
    
//	AddElementToTree( ElementList, TVI_ROOT );
//	AfxMessageBox("After Element to tree");	
//////////////////////
	//Fill the DrawList
	//((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
	FillDrawList(&GETWORLD);
//////////////////////
//	AfxMessageBox("Expanding");

	HTREEITEM hTemp;

	hTemp = m_ElementList.GetRootItem();
	ASSERT(hTemp);
	m_ElementList.Expand(hTemp,TVE_EXPAND);

	// Set our initial position in the hierarchy
	CFotobrowApp *theApp = (CFotobrowApp *)AfxGetApp();
	CMainFrame *theMainFrame = (CMainFrame *)(theApp->m_pMainWnd);
	HTREEITEM hTreeItem = (HTREEITEM)theMainFrame->m_3DtoTree[(void *)theMainFrame->currentElem];
	m_ElementList.SelectItem(hTreeItem);		

	//	if( (hTemp=m_ElementList.GetNextSiblingItem( TVI_ROOT )) )
//		m_ElementList.Expand(hTemp,TVE_EXPAND);

//	AfxMessageBox("Expanded");
	
}

void CElementListView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	CRect rect;
	GetClientRect(&rect);
	CWnd *cwnd = GetDlgItem(IDC_ELEMENT_LIST);

//	::SetWindowPos( GetSafeHwnd(), HWND_TOP, 0,0, rect.Width(), rect.Height(), SWP_NOMOVE |
//			SWP_FRAMECHANGED | SWP_DRAWFRAME );

	::SetWindowPos( cwnd->GetSafeHwnd(), HWND_TOP, 0,0,
		rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	
	
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void CElementListView::FillDrawList(Ptg3DNode *elem)
{
    POSITION pos;

		
    if( !elem->getMemberList().IsEmpty() )
    {
		for( pos = elem->getMemberList().GetHeadPosition(); pos != NULL; 
						elem->getMemberList().GetNext(pos) )
		{
		    Ptg3DNode *anElem = (Ptg3DNode *)elem->getMemberList().GetAt(pos);
//			if( anElem->isActive() ) 
//			{
//			m_ElementList.Expand(TVI_ROOT,TVE_EXPAND);
			HTREEITEM parent = AddParent((Ptg3DNode *)(anElem->getOwner()) );
			AddToDrawList(anElem, parent, TRUE, FALSE, FALSE );
//			}
//		    FillDrawList( anElem );
		}
    }
 }

// Here's the poop on the 3 flags to this function
//	recursive - add all children of elem to the treeview???
//	modify - want drawstyle set to none???
//	check - Ok to check for node existence in tree???
HTREEITEM CElementListView::AddToDrawList(Ptg3DNode *elem, HTREEITEM parent, int recursive, int modify, int check)
{
	TV_INSERTSTRUCT  TreeCtrlItem;
	HTREEITEM hTreeItem;
//	TV_ITEM treeItem;
	static int i=0;

//	elem->setActive(TRUE);
	if( (char *)(elem->getName()) != NULL )
	{
	//	TRACE("Adding to DrawList - %d\n",  i++);
		if( modify )
			elem->setDrawStyle(Ptg3DNode::DS_NONE);//,FALSE);

		if( !check || (hTreeItem=FindMeInTree( parent, elem )) == NULL)
//		if( (hTreeItem=FindMeInTree( parent, elem )) == NULL )
		{
			TreeCtrlItem.hParent = parent;
			TreeCtrlItem.hInsertAfter = TVI_LAST;
			TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			if( elem->isActive() && (elem->getDrawStyle() & (Ptg3DNode::DS_WIREFRAME|Ptg3DNode::DS_SOLIDS)) )
			{
				TreeCtrlItem.item.iImage = image_index[2];
				TreeCtrlItem.item.iSelectedImage = image_index[2];}
			else
			{
//				MessageBeep(MB_OK);
				TreeCtrlItem.item.iImage = image_index[1];
				TreeCtrlItem.item.iSelectedImage = image_index[1];
			}
///////////////////////////////////////////////////////
			TreeCtrlItem.item.lParam = (long)(new FuncTbl);
			((FuncTbl *)(TreeCtrlItem.item.lParam))->dba = 0;
			((FuncTbl *)(TreeCtrlItem.item.lParam))->elem=(LPARAM)elem;
			((FuncTbl *)(TreeCtrlItem.item.lParam))->rightClick = ElementListRightClick;
			((FuncTbl *)(TreeCtrlItem.item.lParam))->leftClick = 0;
			((FuncTbl *)(TreeCtrlItem.item.lParam))->doubleClick = 0;
///////////////////////////////////////////////////////
			TreeCtrlItem.item.pszText = (char *)(elem->getName());
//			TreeCtrlItem.item.lParam = (LPARAM)elem;
			hTreeItem = m_ElementList.InsertItem(&TreeCtrlItem);
			AfxGetApp()->m_pMainWnd->SendMessage(UM_NEW_TREE_NODE, (WPARAM)elem, (LPARAM)hTreeItem );
		}
		if( hTreeItem == NULL )
		{
			AfxMessageBox("Cannot add the Tree Node");
			TRACE("Cannot add node - %s\n", (char *)(elem->getName()) );
			return NULL;
		}

	}
//	else if ( !elem->getMemberList().IsEmpty() )
//	{
//		AfxMessageBox("No Name Found");
//		TRACE("No Name Found\n");
//	}
	if( recursive )
	{
		POSITION pos;
	    if( !elem->getMemberList().IsEmpty() )
	    {
//			int count = 0;
			for( pos = elem->getMemberList().GetHeadPosition(); pos != NULL; elem->getMemberList().GetNext(pos) )
			{
//				count++;
//				if( count > 65 )
//					m_ElementList.SelectItem(hTreeItem);
			    Ptg3DNode *anElem = (Ptg3DNode *)elem->getMemberList().GetAt(pos);
				if( modify )
					anElem->setActive(TRUE);
//				if( anElem->isActive()  )
		    	AddToDrawList( anElem, hTreeItem, recursive, modify, check );
			}
	    }
	}
	return hTreeItem;
}

HTREEITEM CElementListView::AddParent(Ptg3DNode *elem)
{
	HTREEITEM myParent = TVI_ROOT;

	if( elem == NULL )
		return TVI_ROOT;	
	myParent = AddParent( (Ptg3DNode *)(elem->getOwner()) );
	return AddToDrawList(elem, myParent, FALSE, FALSE );
}

HTREEITEM CElementListView::FindMeInTree( HTREEITEM parent , Ptg3DNode *elem )
{
	TV_ITEM treeItem;
	HTREEITEM child;
	
  	int i=0;

	if( parent == TVI_ROOT )
		child = m_ElementList.GetRootItem();
	else
		child = m_ElementList.GetChildItem(parent);

//	return m_ElementList.GetRootItem();
                             
	while( child != NULL )
	{
//		TRACE("Find me in tree - %d\n", i++);
		treeItem.hItem = child;
		treeItem.mask = TVIF_HANDLE;
		if( !(m_ElementList.GetItem(&treeItem)))
			AfxMessageBox("Cannot get Item");
		if( (Ptg3DNode *)(((FuncTbl *)(treeItem.lParam))->elem) == elem )
			return child;
		child = m_ElementList.GetNextSiblingItem(child);
	}
	return NULL;
}


HTREEITEM CElementListView::PruneDrawList( HTREEITEM item )
{
//	TV_INSERTSTRUCT  TreeCtrlItem;
	HTREEITEM parent;
	TV_ITEM treeItem;

	parent = m_ElementList.GetParentItem(item);
	if( parent == NULL )
		return NULL;
	treeItem.hItem = item;
	treeItem.mask = TVIF_HANDLE | TVIF_PARAM;
	if( !(m_ElementList.GetItem(&treeItem)))
		AfxMessageBox("Cannot get Item");
//	((Ptg3DNode *)(treeItem.lParam))->setDrawStyle(Ptg3DNode::DS_NONE);//TRUE);
	((Ptg3DNode *)(((FuncTbl *)(treeItem.lParam))->elem))->setActive(FALSE);//TRUE);
	
	m_ElementList.DeleteItem(item);
	
	treeItem.hItem = parent;
	treeItem.mask = TVIF_HANDLE | TVIF_CHILDREN;
	if( !(m_ElementList.GetItem(&treeItem)))
		AfxMessageBox("Cannot get Item");
	if( treeItem.cChildren == 0 )
		PruneDrawList( parent );
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void CElementListView::OnRclickElementList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_ITEM item;
	CPoint screenCursorPos, clientCursorPos;
	UINT hittestFlags;

	::GetCursorPos(&screenCursorPos);
	clientCursorPos = screenCursorPos;
	m_ElementList.ScreenToClient(&clientCursorPos);

	HTREEITEM hitItem = m_ElementList.HitTest(clientCursorPos,&hittestFlags);
	if( hitItem )
		m_ElementList.SelectItem( hitItem );

	item.hItem = m_ElementList.GetSelectedItem();
	if( item.hItem )
	{
		item.mask = TVIF_HANDLE|TVIF_CHILDREN;
		m_ElementList.GetItem(&item);

		if( item.lParam != NULL )
		{
			if( ((FuncTbl *)(item.lParam))->rightClick != NULL )
			{
				((FuncTbl *)(item.lParam))->rightClick(
						((FuncTbl *)(item.lParam)), item.hItem, *this, CPoint(screenCursorPos), 0 );
			}
		}
	}

	*pResult = 0;
}



/****************************************************************************
** DJB's old code for this
****************************************************************************/
#if FALSE
	HTREEITEM hTree = m_ElementList.GetSelectedItem();
	TV_ITEM item;
//	HTREEITEM hItem = m_EqTreeView.GetParentItem(hTree);
	
	ASSERT(hTree);

	POINT p;							   
	::GetCursorPos(&p);
	CPoint p1(p);
	UINT flags;
	
	CPoint p2(p1);
	m_ElementList.ScreenToClient(&p2);
	HTREEITEM hitItem = m_ElementList.HitTest(p2,&flags);

	if( hitItem != hTree )
	{
		*pResult = 0;
		return;
	}
	
	
	item.hItem = hTree;
	item.mask = TVIF_HANDLE|TVIF_CHILDREN;
	m_ElementList.GetItem(&item);	
	if( item.lParam != NULL )
		if( ((FuncTbl *)(item.lParam))->rightClick != NULL)
			((FuncTbl *)(item.lParam))->rightClick(((FuncTbl *)(item.lParam)),
							hTree, *this, p1, 0);

	*pResult = 0;
}
#endif


void CElementListView::OnMemberlistProperties() 
{
	// TODO: Add your command handler code here
	
}

void CElementListView::OnMemberlistVisible(int style) 
{
//	AfxMessageBox("Visible");
	HTREEITEM hTree = m_ElementList.GetSelectedItem();
///////////////////////////////////////////////////////	
	TV_ITEM item;
	Ptg3DNode *elem;
	
	ASSERT(hTree);

//////******************************************************************//////////////
	item.hItem = hTree;
	item.mask = TVIF_HANDLE|TVIF_CHILDREN;
	m_ElementList.GetItem(&item);	
/////////////////////////////////////////////
	elem = (Ptg3DNode *)((FuncTbl *)(item.lParam))->elem;
//	AfxMessageBox("Calling recurseElement");
	recurseElementTree( hTree, style);
	return;
	switch( style) 
	{
	case Ptg3DNode::DS_NONE:
		elem->setDrawStyle(Ptg3DNode::DS_INVISIBLE);
		break;
	case Ptg3DNode::DS_WIREFRAME:
		elem->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
		elem->setActive(TRUE);
		break;
	case Ptg3DNode::DS_SOLIDS:
		elem->setDrawStyle(Ptg3DNode::DS_SOLIDS);
		elem->setActive(TRUE);
		break;
	}

	if( elem->isActive() && style != Ptg3DNode::DS_NONE ) 
	{
		m_ElementList.SetItem( hTree, TVIF_IMAGE|TVIF_SELECTEDIMAGE, 
			NULL,image_index[2], image_index[2], 0, 0, 0 );
	}
	else
	{
		m_ElementList.SetItem( hTree, TVIF_IMAGE|TVIF_SELECTEDIMAGE, 
			NULL,image_index[1], image_index[1], 0, 0, 0 );
	}
/////////////////////////////////////
	if( 1 /*recursive*/ )
	{
		POSITION pos;
	    if( !elem->getMemberList().IsEmpty() )
	    {
			for( pos = elem->getMemberList().GetHeadPosition(); pos != NULL; elem->getMemberList().GetNext(pos) )
			{
			    Ptg3DNode *anElem = (Ptg3DNode *)elem->getMemberList().GetAt(pos);
//				if( modify )
//					anElem->setActive(TRUE);
//				if( anElem->isActive()  )
//			    	OnMemberlistVisible( anElem, hTreeItem, recursive, modify, check );
			}
	    }
	}

//////////////////////////////////////
}

void CElementListView::OnMemberlistVisibleSolid() 
{
	OnMemberlistVisible(Ptg3DNode::DS_SOLIDS);
	((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->UpdateAllViews();
}

void CElementListView::OnMemberlistVisibleWireframe() 
{
	OnMemberlistVisible(Ptg3DNode::DS_WIREFRAME);
	((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->UpdateAllViews();
}

void CElementListView::OnMemberlistVisibleInvisible() 
{
	OnMemberlistVisible(Ptg3DNode::DS_NONE);
	((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->UpdateAllViews();
}

void CElementListView::OnMemberlistSetAsReferenceNode() 
{
	HTREEITEM hTree = m_ElementList.GetSelectedItem();
	TV_ITEM item;
	Ptg3DNode *elem;
	
	ASSERT(hTree);

	item.hItem = hTree;
	item.mask = TVIF_HANDLE|TVIF_CHILDREN;
	m_ElementList.GetItem(&item);

	((CFotobrowApp *)AfxGetApp())->SetReferenceNode((Ptg3DNode *)((FuncTbl *)(item.lParam))->elem);
#if FALSE
	((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode = 
			(Ptg3DNode *)((FuncTbl *)(item.lParam))->elem;

//CFotobrowApp *theApp = (CFotobrowApp *)AfxGetApp();
//CMainFrame *mainFrame = (CMainFrame *)theApp->m_pMainWnd;
//PointGridBar *gridBar = mainFrame->get3DPointGridBar();
//gridBar->RefreshGrid();

	(((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->get3DPointGridBar())->RefreshGrid();
#endif
}

void CElementListView::OnMemberListCalcCircBy3DPoints()
{
	HTREEITEM hTree = m_ElementList.GetSelectedItem();
	TV_ITEM item;
	Ptg3DNode *elem;

	ASSERT(hTree);

	item.hItem = hTree;
	item.mask = TVIF_HANDLE|TVIF_CHILDREN;
	m_ElementList.GetItem(&item);

	//(Ptg3DNode *)((FuncTbl *)(item.lParam))->elem
	AfxMessageBox( "Need to calculate the circle!!!" );
}

void CElementListView::recurse3DNodeTree( Ptg3DNode *elem, int style)
{
	switch( style) 
	{
	case Ptg3DNode::DS_NONE:
		elem->setDrawStyle(Ptg3DNode::DS_INVISIBLE);
		break;
	case Ptg3DNode::DS_WIREFRAME:
		elem->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
		elem->setActive(TRUE);
		break;
	case Ptg3DNode::DS_SOLIDS:
		elem->setDrawStyle(Ptg3DNode::DS_SOLIDS);
		elem->setActive(TRUE);
		break;
	}
/////////////////////////////////////
//	TRACE("Rcursing through the 3D Tree\n");

//	AfxMessageBox("If 1");
	if( 1 /*recursive*/ )
	{
		POSITION pos;
	    if( !elem->getMemberList().IsEmpty() )
	    {
			for( pos = elem->getMemberList().GetHeadPosition(); pos != NULL; elem->getMemberList().GetNext(pos) )
			{
			    Ptg3DNode *anElem = (Ptg3DNode *)elem->getMemberList().GetAt(pos);
//				if( modify )
//					anElem->setActive(TRUE);
//				if( anElem->isActive()  )
//			    	OnMemberlistVisible( anElem, hTreeItem, recursive, modify, check );
				if( anElem != NULL )
				{
					//AfxMessageBox("Calling recurse again");
		    		recurse3DNodeTree( anElem, style );
				}
			}
	    }
	}

//////////////////////////////////////
}

void CElementListView::recurseElementTree(HTREEITEM parent, int style)
{
	TV_ITEM treeItem,item;
	HTREEITEM child;
	Ptg3DNode *elem;
  	int i=0;
	
//*****************************************************
	item.hItem = parent;
	item.mask = TVIF_HANDLE|TVIF_CHILDREN;
	m_ElementList.GetItem(&item);	
/////////////////////////////////////////////
	elem = (Ptg3DNode *)((FuncTbl *)(item.lParam))->elem;
	ASSERT(elem);
//	AfxMessageBox("Inside recurseElement");
//	TRACE("The Element is %s\n", elem->getName());
	if( (int)(elem) < 128 )
		TRACE("The element pointer is trash\n");
	switch( style) 
	{
	case Ptg3DNode::DS_NONE:
		elem->setDrawStyle(Ptg3DNode::DS_NONE);
		break;
	case Ptg3DNode::DS_WIREFRAME:
		elem->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
		elem->setActive(TRUE);
		break;
	case Ptg3DNode::DS_SOLIDS:
		elem->setDrawStyle(Ptg3DNode::DS_SOLIDS);
		elem->setActive(TRUE);
		break;
	}

	if( elem->isActive() && style != Ptg3DNode::DS_NONE ) 
	{
		m_ElementList.SetItem( parent, TVIF_IMAGE|TVIF_SELECTEDIMAGE, 
			NULL,image_index[2], image_index[2], 0, 0, 0 );
	}
	else
	{
		m_ElementList.SetItem( parent, TVIF_IMAGE|TVIF_SELECTEDIMAGE, 
			NULL,image_index[1], image_index[1], 0, 0, 0 );
	}

//*****************************************************
	if( parent == TVI_ROOT )
	{
		child = m_ElementList.GetRootItem();
//		TRACE("Tried to get a child from the root\n");
	}
	else
	{
		child = m_ElementList.GetChildItem(parent);
//		TRACE("Tried to get a child from the PARENT\n");
	}
	
//	ASSERT(child);
//	AfxMessageBox("After getChildItem");
//	return m_DrawListTree.GetRootItem();
//	if( child == NULL )
//		TRACE("Child == NULL\n");
//	else
//		TRACE("Child seems valid\n");

	if( child != NULL )                             
	{
		while( child != NULL )
		{
			recurseElementTree(child,style);
//			TRACE("Just called recurese element - %d\n", i++);
			child = m_ElementList.GetNextSiblingItem(child);
		}
	}
	else
	{
		Ptg3DNode *anElem;
//		TRACE("Child was NULL and recursing 3D tree\n");
		treeItem.hItem = parent;
		treeItem.mask = TVIF_HANDLE;
		if( !(m_ElementList.GetItem(&treeItem)))
			AfxMessageBox("Cannot get Item");
		anElem = (Ptg3DNode *)((FuncTbl *)(treeItem.lParam))->elem;
		ASSERT(anElem);
		recurse3DNodeTree(anElem,style);
	}
}

void CElementListView::OnColorBlack(){DoColorSet(colors[0]);}
void CElementListView::OnColorMaroon(){DoColorSet(colors[1]);}
void CElementListView::OnColorOlive(){DoColorSet(colors[2]);}
void CElementListView::OnColorGreen(){DoColorSet(colors[3]);}	
void CElementListView::OnColorTeal(){DoColorSet(colors[4]);}
void CElementListView::OnColorNavy(){DoColorSet(colors[5]);}
void CElementListView::OnColorWine(){DoColorSet(colors[6]);}
void CElementListView::OnColorGrey(){DoColorSet(colors[7]);}
void CElementListView::OnColorPink(){DoColorSet(colors[8]);}
void CElementListView::OnColorOrange(){DoColorSet(colors[9]);}
void CElementListView::OnColorChartruse(){DoColorSet(colors[10]);}
void CElementListView::OnColorPurple(){DoColorSet(colors[11]);}
void CElementListView::OnColorSkyBlue(){DoColorSet(colors[12]);}
void CElementListView::OnColorRed(){DoColorSet(colors[13]);}
void CElementListView::OnColorYellow(){DoColorSet(colors[14]);}
void CElementListView::OnColorLime(){DoColorSet(colors[15]);}
void CElementListView::OnColorAqua(){DoColorSet(colors[16]);}
void CElementListView::OnColorBlue(){DoColorSet(colors[17]);}
void CElementListView::OnColorMagenta(){DoColorSet(colors[18]);}
void CElementListView::OnColorWhite(){DoColorSet(colors[19]);}	

void CElementListView::DoColorSet(COLORREF color)
{
	HTREEITEM hTree = m_ElementList.GetSelectedItem();
///////////////////////////////////////////////////////	
	TV_ITEM item;
	Ptg3DNode *elem;
	
	ASSERT(hTree);

//////******************************************************************//////////////
	item.hItem = hTree;
	item.mask = TVIF_HANDLE|TVIF_CHILDREN;
	m_ElementList.GetItem(&item);	
/////////////////////////////////////////////
	elem = (Ptg3DNode *)((FuncTbl *)(item.lParam))->elem;
	elem->setColor(GetRValue(color)/255.0, GetGValue(color)/255.0, GetBValue(color)/255.0);
	RecurseColorSet(elem, color);
	((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->UpdateAllViews();
}

void CElementListView::RecurseColorSet(Ptg3DNode *elem, COLORREF color)
{
	POSITION pos;
	elem->setColor(GetRValue(color)/255.0, GetGValue(color)/255.0, GetBValue(color)/255.0);

	if( !elem->getMemberList().IsEmpty() )
	{
		for( pos = elem->getMemberList().GetHeadPosition(); pos != NULL; elem->getMemberList().GetNext(pos) )
		{
			Ptg3DNode *anElem = (Ptg3DNode *)elem->getMemberList().GetAt(pos);
			if( anElem != NULL )
			{
		    	RecurseColorSet( anElem, color );
			}
		}
	}

}

LONG CElementListView::OnCurrentElementChange(WPARAM wParam, LPARAM lParam)
{
//	Ptg3DNode *node = (Ptg3DNode *)lParam;
	HTREEITEM newCurrElement = (HTREEITEM)lParam;
	if( newCurrElement == NULL )
		return TRUE;

	m_ElementList.SelectItem(newCurrElement);		
	return TRUE;
}

void CElementListView::PurgeExtraData(HTREEITEM node)
{
	HTREEITEM child;
	TV_ITEM treeItem;

	child = m_ElementList.GetChildItem(node);
//	TRACE("Tried to get a child from the PARENT\n");
	
//	if( child == NULL )
//		TRACE("Child == NULL\n");
//	else
//		TRACE("Child seems valid\n");

	if( child != NULL )                             
	{
		TRACE("Purging the item handle - %d", child);
		PurgeExtraData(child);
		while( child != NULL )
		{
			treeItem.hItem = child;
			treeItem.mask = TVIF_HANDLE;
			m_ElementList.GetItem(&treeItem);
			
			if( treeItem.lParam != NULL )
				free( (void *)(treeItem.lParam) );

			child = m_ElementList.GetNextSiblingItem(child);
//			TRACE("Got next child item - %d\n", child);
			if( child != NULL )
				PurgeExtraData(child);
		}
	}
	
}

void CElementListView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	TV_ITEM treeItem;
	HTREEITEM hItem;
	hItem = m_ElementList.GetRootItem();
	PurgeExtraData(hItem);

	treeItem.hItem = hItem;
	treeItem.mask = TVIF_HANDLE;
	m_ElementList.GetItem(&treeItem);
			
	if( treeItem.lParam != NULL )
		free( (void *)(treeItem.lParam) );

	CImageList *list = m_ElementList.GetImageList(TVSIL_NORMAL);
	if( list )
		delete list;
}

void deleteNode( Ptg3DNode *node );

void CElementListView::OnMemberlistDelete() 
{
	HTREEITEM hTree = m_ElementList.GetSelectedItem();

	TV_ITEM item;
	Ptg3DNode *elem;
	
	ASSERT(hTree);

	item.hItem = hTree;
	item.mask = TVIF_HANDLE|TVIF_CHILDREN;
	m_ElementList.GetItem(&item);	

	elem = (Ptg3DNode *)((FuncTbl *)(item.lParam))->elem;

//CTreeCtrl
//EnsureVisible
	deleteNode( elem );
	m_ElementList.DeleteItem( hTree );
	Invalidate(TRUE);
	((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->UpdateAllViews();
}

void deleteNode( Ptg3DNode *node )
{
	// recurse for all the children...make SURE to do the children before the parent or you will orphan
	// the children forever.
    POSITION pos;
    if( !node->getMemberList().IsEmpty() )
    {
		for( pos = node->getMemberList().GetHeadPosition(); pos != NULL; pos = node->getMemberList().GetHeadPosition() )
		{
		    Ptg3DNode *anElem = (Ptg3DNode *)node->getMemberList().GetAt(pos);
			deleteNode( anElem );
		}
    }

	// delete the node we were called for
	Ptg3DNode *parentNode = (Ptg3DNode *)(node->getOwner());
	if( parentNode != NULL )
	{
		POSITION pos = parentNode->getMemberList().Find(node);
		parentNode->getMemberList().RemoveAt( pos );
	}
	
	// CMapPtrToPtr
	CFotobrowApp *theApp = (CFotobrowApp *)AfxGetApp();
	CMainFrame *theMainFrame = (CMainFrame *)(theApp->m_pMainWnd);
	(HTREEITEM)theMainFrame->m_3DtoTree.RemoveKey( node );
	delete node;
}

void CElementListView::OnMemberlistSavedgn() 
{
	HTREEITEM hCurrElemInTree;
	TV_ITEM ti;
	Ptg3DNode *p3nCurrElem;

	hCurrElemInTree = m_ElementList.GetSelectedItem();
	ti.hItem = hCurrElemInTree;
	ti.mask = TVIF_HANDLE | TVIF_CHILDREN;
	m_ElementList.GetItem(&ti);

	p3nCurrElem = (Ptg3DNode *)((FuncTbl *)(ti.lParam))->elem;

	if( p3nCurrElem != NULL )
	{
		CFileDialog theDialog( TRUE );
		theDialog.m_ofn.Flags |= OFN_NOCHANGEDIR;
		if( theDialog.DoModal() == IDOK )
		{
			char *pszDGNFileName = strdup( (LPCSTR)(theDialog.GetPathName()) );
	
			DGN dgn = openDgnFile( pszDGNFileName );
			if( dgn != NULL )
			{
				setUORConversion(dgn, 63, "mm");
				dgnPushCurrentTransform(dgn);

				saveNodeToDGN( dgn, p3nCurrElem );

				dgnPopCurrentTransform(dgn);
				closeDgnFile( dgn );
			}
			else
				AfxMessageBox( "Could not open dgn." );

			free(pszDGNFileName);
		}
	}
	else
		AfxMessageBox( "Node pointer was NULL" );
}
