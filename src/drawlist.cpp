// drawlist.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "drawlist.h"

#include <gl/glu.h>
#include <gl/gl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawList dialog


CDrawList::CDrawList(CWnd* pParent /*=NULL*/, Ptg3DNode *listObj)
	: CDialog(CDrawList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrawList)
	m_Color = -1;
	m_DrawStyle = -1;
	//}}AFX_DATA_INIT
	ElementList = listObj;
	out = fopen("wow.txt", "w+");

}


void CDrawList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawList)
	DDX_Control(pDX, IDC_DRAWSTYLE, m_DrawStyleList);
	DDX_Control(pDX, IDC_COLOR, m_ColorList);
	DDX_Control(pDX, IDC_MEMBERLIST, m_MemberList);
	DDX_Control(pDX, IDC_DRAWLIST, m_DrawListTree);
	DDX_CBIndex(pDX, IDC_COLOR, m_Color);
	DDX_CBIndex(pDX, IDC_DRAWSTYLE, m_DrawStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDrawList, CDialog)
	//{{AFX_MSG_MAP(CDrawList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDrawList message handlers

BOOL CDrawList::OnInitDialog() 
{

	CDialog::OnInitDialog();
//	AfxMessageBox("Init Dlg");	
	// initialize the tree view window
	// First, create the image list we will need
	hIml = ImageList_Create( 16,  // width
	    16,                      // height
		0,                                  // flags
		4,                        // number of bitmaps
		0 );                                // amount to grow
	hIml2 = ImageList_Create( 16,  // width
	    16,                      // height
		0,                                  // flags
		4,                        // number of bitmaps
		0 );                                // amount to grow

	// load the bitmaps and add them to the image lists
	//HBITMAP hBmp1, hBmp2, hBmp3, hBmp4;
	hBmps[0] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_NODE));
	image_index[0] = ImageList_Add(hIml,hBmps[0],NULL);
	ImageList_Add(hIml2,hBmps[0],NULL);
	hBmps[1] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_EYE_CLOSED));
	image_index[1] = ImageList_Add(hIml,hBmps[1],NULL);
	ImageList_Add(hIml2,hBmps[1],NULL);
	hBmps[2] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_EYE_OPEN));
	image_index[2] = ImageList_Add(hIml,hBmps[2],NULL);
	ImageList_Add(hIml2,hBmps[2],NULL);
	hBmps[3] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_CYLINDER));
	image_index[3] = ImageList_Add(hIml,hBmps[3],NULL);
	ImageList_Add(hIml2,hBmps[3],NULL);

	for( int i=0; i<4; i++ )
		DeleteObject(hBmps[i]);

	//AfxMessageBox("Images");	
	
	CImageList *imgList = new CImageList();
	CImageList *imgList2 = new CImageList();
	imgList->Attach(hIml);
	imgList2->Attach(hIml2);
	m_MemberList.SetImageList( imgList, TVSIL_NORMAL );
	m_DrawListTree.SetImageList( imgList2, TVSIL_NORMAL );

//	TV_INSERTSTRUCT TreeCtrlItem;
	m_MemberList.DeleteAllItems();
    
	AddElementToTree( ElementList, TVI_ROOT );
//	AfxMessageBox("After Element to tree");	
	fclose(out);
//	m_MemberList.Expand(TVI_ROOT,TVE_EXPAND);
	//CComboBox
	m_ColorList.SetCurSel(0);
	m_DrawStyleList.SetCurSel(0);

//////////////////////
	//Fill the DrawList
	FillDrawList(ElementList);
//////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CDrawList::AddElementToTree(Ptg3DNode *elem, HTREEITEM hTreeItem) 
{
	TV_INSERTSTRUCT  TreeCtrlItem;
	HTREEITEM hTreeItem1;

	hTreeItem1 = hTreeItem;
	if( (char *)(elem->getName()) != NULL )
	{
		TreeCtrlItem.hParent = hTreeItem;
		TreeCtrlItem.hInsertAfter = TVI_LAST;
		TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TreeCtrlItem.item.pszText = (char *)(elem->getName());
		fprintf(out,"%s\n", (char *)(elem->getName()));
		TreeCtrlItem.item.iImage = image_index[1];
		TreeCtrlItem.item.iSelectedImage = TreeCtrlItem.item.iImage;
		TreeCtrlItem.item.lParam = (LPARAM)elem;//->getUniqID();
		hTreeItem1 = m_MemberList.InsertItem(&TreeCtrlItem);
		if( hTreeItem1 == NULL )
		{
			AfxMessageBox("Failed adding to Tree");
		}
	}
//	else
//	{
//		AfxMessageBox("No Name Found");
//		TRACE("No Name Found\n");
//	}
    POSITION pos;
    if( !elem->getMemberList().IsEmpty() )
    {
		for( pos = elem->getMemberList().GetHeadPosition(); pos != NULL; elem->getMemberList().GetNext(pos) )
		{
		    Ptg3DNode *anElem = (Ptg3DNode *)elem->getMemberList().GetAt(pos);
		    AddElementToTree( anElem, hTreeItem1 );
		}
    }
	return 0;
}


void CDrawList::OnAdd() 
{
//	TV_INSERTSTRUCT  TreeCtrlItem;
	HTREEITEM hTreeItem;
	TV_ITEM treeItem;
	BeginWaitCursor();
	hTreeItem = m_MemberList.GetSelectedItem();	

	treeItem.hItem = hTreeItem;
	treeItem.mask = TVIF_HANDLE;
	if( !(m_MemberList.GetItem(&treeItem)))
	{
		AfxMessageBox("Cannot get Item - You must select the item from the member list to Add");
		return;
	}

	((Ptg3DNode *)(treeItem.lParam))->setActive(TRUE);
	TRACE("Calling AddToDrawList\n");
  	
  	HTREEITEM parent = AddParent((Ptg3DNode *)((Ptg3DNode *)(treeItem.lParam))->getOwner() );
	
	if( (hTreeItem=FindMeInTree( parent, (Ptg3DNode *)(treeItem.lParam) )) != NULL )
		PruneDrawList(hTreeItem);	
	AddToDrawList((Ptg3DNode *)(treeItem.lParam), parent, TRUE, TRUE, FALSE);
	UpdateData(TRUE);
	switch( m_Color )
	{
		case 0:
			break;
		case 1:	//BLACK
			setColor((Ptg3DNode *)(treeItem.lParam),0.0,0.0,0.0);
			break;
		case 2: //BLUE
			setColor((Ptg3DNode *)(treeItem.lParam),0.0,0.0,1.0);
			break;
		case 3: //CYAN
			setColor((Ptg3DNode *)(treeItem.lParam),0.0,1.0,1.0);
			break;
		case 4: //GREEN
			setColor((Ptg3DNode *)(treeItem.lParam),0.0,1.0,0.0);
			break;
		case 5: //MAGENTA
			setColor((Ptg3DNode *)(treeItem.lParam),1.0,0.0,1.0);
			break;
		case 6: //RED
			setColor((Ptg3DNode *)(treeItem.lParam),1.0,0.0,0.0);
			break;
		case 7: //WHITE
			setColor((Ptg3DNode *)(treeItem.lParam),1.0,1.0,1.0);
			break;
		case 8: //YELLOW
			setColor((Ptg3DNode *)(treeItem.lParam),1.0,1.0,0.0);
			break;
	}
	if( m_DrawStyle )
		((Ptg3DNode *)(treeItem.lParam))->setDrawStyle(Ptg3DNode::DS_WIREFRAME);//TRUE);
	else
		((Ptg3DNode *)(treeItem.lParam))->setDrawStyle(Ptg3DNode::DS_SOLIDS);//TRUE);
	m_DrawListTree.InvalidateRect(NULL);
	EndWaitCursor();
}

void CDrawList::OnRemove() 
{
//	TV_INSERTSTRUCT  TreeCtrlItem;
	HTREEITEM hTreeItem;
	TV_ITEM treeItem;

	hTreeItem = m_DrawListTree.GetSelectedItem();	
	if( !hTreeItem )
	{
		AfxMessageBox("Nothing to Remove");
		return;
	}
	treeItem.hItem = hTreeItem;
	treeItem.mask = TVIF_HANDLE | TVIF_PARAM;
	if( !(m_DrawListTree.GetItem(&treeItem)))
		AfxMessageBox("Cannot get Item");
//	((Ptg3DNode *)(treeItem.lParam))->setDrawStyle(Ptg3DNode::DS_NONE);//TRUE);
	((Ptg3DNode *)(treeItem.lParam))->setActive(FALSE);//TRUE);
	PruneDrawList( hTreeItem );
}

void CDrawList::OnOK() 
{
	ImageList_Destroy(hIml);
	ImageList_Destroy(hIml2);
	
	CDialog::OnOK();
}

void CDrawList::setColor( Ptg3DNode *elem, GLfloat red, GLfloat green, GLfloat blue )
{
    POSITION pos;

	elem->setColor( red, green, blue );			
    if( !elem->getMemberList().IsEmpty() )
    {
		for( pos = elem->getMemberList().GetHeadPosition(); pos != NULL; 
						elem->getMemberList().GetNext(pos) )
		{
		    Ptg3DNode *anElem = (Ptg3DNode *)elem->getMemberList().GetAt(pos);
		    setColor( anElem, red, green, blue );
		}
    }
}

void CDrawList::FillDrawList(Ptg3DNode *elem)
{
    POSITION pos;

		
    if( !elem->getMemberList().IsEmpty() )
    {
		for( pos = elem->getMemberList().GetHeadPosition(); pos != NULL; 
						elem->getMemberList().GetNext(pos) )
		{
		    Ptg3DNode *anElem = (Ptg3DNode *)elem->getMemberList().GetAt(pos);
			if( anElem->isActive() ) 
			{
			  	HTREEITEM parent = AddParent((Ptg3DNode *)(anElem->getOwner()) );
				AddToDrawList(anElem, parent, TRUE, FALSE, FALSE );
			}
//		    FillDrawList( anElem );
		}
    }
 }

HTREEITEM CDrawList::AddToDrawList(Ptg3DNode *elem, HTREEITEM parent, int recursive, int modify, int check)
{
	TV_INSERTSTRUCT  TreeCtrlItem;
	HTREEITEM hTreeItem;
//	TV_ITEM treeItem;
	static int i=0;

	elem->setActive(TRUE);
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
			TreeCtrlItem.item.iImage = image_index[2];
			TreeCtrlItem.item.iSelectedImage = image_index[2];
			TreeCtrlItem.item.pszText = (char *)(elem->getName());
			TreeCtrlItem.item.lParam = (LPARAM)elem;
			hTreeItem = m_DrawListTree.InsertItem(&TreeCtrlItem);
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
			for( pos = elem->getMemberList().GetHeadPosition(); pos != NULL; elem->getMemberList().GetNext(pos) )
			{
			    Ptg3DNode *anElem = (Ptg3DNode *)elem->getMemberList().GetAt(pos);
				if( modify )
					anElem->setActive(TRUE);
				if( anElem->isActive()  )
			    	AddToDrawList( anElem, hTreeItem, recursive, modify, check );
			}
	    }
	}
	return hTreeItem;
}

HTREEITEM CDrawList::AddParent(Ptg3DNode *elem)
{
	HTREEITEM myParent = TVI_ROOT;

	if( elem == NULL )
		return TVI_ROOT;	
	myParent = AddParent( (Ptg3DNode *)(elem->getOwner()) );
	return AddToDrawList(elem, myParent, FALSE, FALSE );
}

HTREEITEM CDrawList::FindMeInTree( HTREEITEM parent , Ptg3DNode *elem )
{
	TV_ITEM treeItem;
	HTREEITEM child;
	
  	int i=0;

	if( parent == TVI_ROOT )
		child = m_DrawListTree.GetRootItem();
	else
		child = m_DrawListTree.GetChildItem(parent);

//	return m_DrawListTree.GetRootItem();
                             
	while( child != NULL )
	{
//		TRACE("Find me in tree - %d\n", i++);
		treeItem.hItem = child;
		treeItem.mask = TVIF_HANDLE;
		if( !(m_DrawListTree.GetItem(&treeItem)))
			AfxMessageBox("Cannot get Item");
		if( (Ptg3DNode *)(treeItem.lParam) == elem )
			return child;
		child = m_DrawListTree.GetNextSiblingItem(child);
	}
	return NULL;
}


HTREEITEM CDrawList::PruneDrawList( HTREEITEM item )
{
//	TV_INSERTSTRUCT  TreeCtrlItem;
	HTREEITEM parent;
	TV_ITEM treeItem;

	parent = m_DrawListTree.GetParentItem(item);
	if( parent == NULL )
		return NULL;
	treeItem.hItem = item;
	treeItem.mask = TVIF_HANDLE | TVIF_PARAM;
	if( !(m_DrawListTree.GetItem(&treeItem)))
		AfxMessageBox("Cannot get Item");
//	((Ptg3DNode *)(treeItem.lParam))->setDrawStyle(Ptg3DNode::DS_NONE);//TRUE);
	((Ptg3DNode *)(treeItem.lParam))->setActive(FALSE);//TRUE);
	
	m_DrawListTree.DeleteItem(item);
	
	treeItem.hItem = parent;
	treeItem.mask = TVIF_HANDLE | TVIF_CHILDREN;
	if( !(m_DrawListTree.GetItem(&treeItem)))
		AfxMessageBox("Cannot get Item");
	if( treeItem.cChildren == 0 )
		PruneDrawList( parent );
	return NULL;
}

