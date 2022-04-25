// selecteq.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "selecteq.h"
#include "common.h"
#include <transact.hpp>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectEqImageDlg dialog


CSelectEqImageDlg::CSelectEqImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectEqImageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectEqImageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	if( rvqTask != NULL )
		return;
	TRACE("Making a new rvqTask\n");
	rvqTask = new rvqLiteTask;
	rvqTask->MakeCurrent();
	CString temp = ((CFotobrowApp *)AfxGetApp())->m_DatabasePath + "rvq";
	rvqTask->SetPaths((char *)(const char *)temp,
					  (char *)(const char *)temp,
//					  (char *)(const char *)temp,
//					  (char *)(const char *)temp );
					NULL, NULL );
	if( !rvqTask->openDatabase("rvqlite") )
	{
		AfxMessageBox("Error opening database");
		delete rvqTask;
		rvqTask = NULL;
	}
	TRACE("Databases are Open\n");
	m_hIml = NULL;
}


void CSelectEqImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectEqImageDlg)
	DDX_Control(pDX, IDOK, m_CloseButton);
	DDX_Control(pDX, IDC_EQ_LIST, m_EqTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectEqImageDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectEqImageDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_EQ_LIST, OnDblclkEqList)
	ON_NOTIFY(NM_RDBLCLK, IDC_EQ_LIST, OnRdblclkEqList)
	ON_NOTIFY(TVN_SELCHANGED, IDC_EQ_LIST, OnSelchangedEqList)
	ON_BN_CLICKED(ID_UNLINK, OnUnlink)
	ON_BN_CLICKED(ID_OPEN_IMAGE, OnOpenImage)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_EQ_LIST, OnEndlabeleditEqList)
	ON_BN_CLICKED(IDC_EQUIP_TAG_DATA, OnEquipTagData)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_EQ_LIST, OnBeginlabeleditEqList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSelectEqImageDlg message handlers
#include "photog.h"
#include "../rvqLite/rvqLite1.hpp"

BOOL CSelectEqImageDlg::OnInitDialog() 
{
	image_equip_boxObj box;
	equipObj eq;
	image_ptrObj image;
	struct images img;
//	HIMAGELIST hIml;  
	
	CDialog::OnInitDialog();
///
	// initialize the tree view window
	// First, create the image list we will need
	m_hIml = ImageList_Create( 16,  // width
	    16,                      // height
		0,                                  // flags
		1,                        // number of bitmaps
		0 );                                // amount to grow

	// load the bitmaps and add them to the image lists
	HBITMAP hBmp1, hBmp2;
	hBmp1 = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_EQUIPMENT));
	int idxForSale = ImageList_Add(m_hIml,hBmp1,NULL);
	DeleteObject(hBmp1);
	hBmp2 = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_IMAGE));
	int idxImage = ImageList_Add(m_hIml,hBmp2,NULL);
	DeleteObject(hBmp2);
	CImageList *imgList = new CImageList();
	imgList->Attach(m_hIml);
	m_EqTree.SetImageList(imgList, TVSIL_NORMAL);

///
	TV_INSERTSTRUCT TreeCtrlItem;
	m_EqTree.DeleteAllItems();
//	TreeCtrlItem.hParent = TVI_ROOT;
//	TreeCtrlItem.hInsertAfter = TVI_LAST;
//	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
//	TreeCtrlItem.item.iImage = idxForSale;
//	TreeCtrlItem.item.iSelectedImage = TreeCtrlItem.item.iImage;
//	TreeCtrlItem.item.pszText = "Equipment";
//	TreeCtrlItem.item.lParam = 0;
//	HTREEITEM hTreeItem;// = m_EqTree.InsertItem(&TreeCtrlItem);
///////////////
	rvqTask->MakeCurrent();

	eq.ReadLock();
	box.ReadLock();
	image.ReadLock();

	for( eq.First(); eq.Okay(); eq.Next() )
	{
		TreeCtrlItem.hParent = TVI_ROOT;
		TreeCtrlItem.hInsertAfter = TVI_LAST;
		TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TreeCtrlItem.item.pszText = eq.name;
		TreeCtrlItem.item.iImage = idxForSale;
		TreeCtrlItem.item.iSelectedImage = TreeCtrlItem.item.iImage;
		TreeCtrlItem.item.lParam = eq.GetDba();
		HTREEITEM hTreeItem1 = m_EqTree.InsertItem(&TreeCtrlItem);
/////
		for( box.GotoMember(eq, EQUIP_BOX, FIRST );
			box.Okay(); box.GotoMember(eq, EQUIP_BOX, NEXT ) )
		{
			image.FromMember( &box, IMAGE_BOX );
			if( image.Okay() )
			{
				rdmRecLock( IMAGES, "r", TASKER );
				dt_crset( (DB_ADDR *)&(image.image_ptr::dba), TASKER );
				dt_recread( &img, TASKER );
				rdmRecFree(IMAGES, TASKER);
				TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
				TreeCtrlItem.item.iImage = idxImage;
				TreeCtrlItem.item.iSelectedImage = TreeCtrlItem.item.iImage;
				TreeCtrlItem.hParent = hTreeItem1;
				TreeCtrlItem.item.pszText = img.szName;
				TreeCtrlItem.item.lParam = image.image_ptr::dba;
				m_EqTree.InsertItem(&TreeCtrlItem);
			}
		}
/////
	}	
	eq.FreeLock();
	image.FreeLock();
	box.FreeLock();
//	m_EqTree.Expand(hTreeItem,TVE_EXPAND);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectEqImageDlg::OnDblclkEqList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	image_equip_boxObj box;
	equipObj eq;
	image_ptrObj image;
//	struct images img;

	HTREEITEM hTree = m_EqTree.GetSelectedItem();
	TV_ITEM item;
	HTREEITEM hItem = m_EqTree.GetParentItem(hTree);
	if( hItem == NULL )
	{
		return;
	}
//	TV_ITEM item;
	item.hItem = hTree;
	item.mask = TVIF_HANDLE|TVIF_CHILDREN;
	m_EqTree.GetItem(&item);	
	OpenImageByDba((DB_ADDR)(item.lParam) );
	*pResult = 0;
}

void CSelectEqImageDlg::OnRdblclkEqList(NMHDR* pNMHDR, LRESULT* pResult) 
{

}

void CSelectEqImageDlg::OnSelchangedEqList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//	char temp[120];
//	sprintf(temp,"%s -- Dba is %lu", (const char *)(pNMTreeView->itemNew.pszText), 
//				pNMTreeView->itemNew.lParam);
	//AfxMessageBox(temp);
	*pResult = 0;
}

BOOL CSelectEqImageDlg::DestroyWindow() 
{
	CObList list;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(&list);

	if( list.GetCount()	== 0 )
	{
		if( rvqTask )
		{
			delete rvqTask;
			rvqTask=NULL;
		}
	}
	else
	{
		while( list.GetCount() )
			list.RemoveTail();
	}
	if( m_hIml )
		ImageList_Destroy(m_hIml);

	return CDialog::DestroyWindow();
}

void CSelectEqImageDlg::OnUnlink() 
{
	image_equip_boxObj box;
	equipObj eq;
	image_ptrObj image;
//	struct images img;
//	char buff[128];

	TransAction a;
	a.Begin("unlink");
	
	rvqTask->MakeCurrent();
	HTREEITEM hTree = m_EqTree.GetSelectedItem();
	TV_ITEM item;
	item.hItem = hTree;
	item.mask = TVIF_HANDLE | TVIF_PARAM;
 	m_EqTree.GetItem(&item);	
	HTREEITEM hItem = m_EqTree.GetParentItem(hTree);
	eq.WriteLock();
	box.WriteLock();
	image.WriteLock();
	if( hItem == NULL )
	{				// This means we are on EQUIPMENT
		eq.GotoDirect(DbAddr(item.lParam));
		if( !(eq.Okay()) )
		{
			AfxMessageBox("Couldn't find Eq");
			return;
		}
		for( box.GotoMember(eq, EQUIP_BOX, FIRST );
			box.Okay(); box.GotoMember(eq, EQUIP_BOX, FIRST ) )
		{
			box.Delete();
//			AfxMessageBox("Delete one");
		}
		HTREEITEM child;
		while( (child = m_EqTree.GetChildItem(hTree)) )
			m_EqTree.DeleteItem(child);
		if(! eq.IsInUse() )
			eq.Delete();
		m_EqTree.DeleteItem(hTree);

	}
	else    	// We are now trying to delete IMAGE BOXES from Eq
	{
		image.Find( KeyObj( IMAGE_PTR_DBA, (Pvoid)&(item.lParam) ));
		for( box.GotoMember(image, IMAGE_BOX, FIRST );
			box.Okay(); box.GotoMember(image, IMAGE_BOX, NEXT ) )
		{
			eq.FromMember(&box, EQUIP_BOX);
			if( eq.Okay() )
			{
				TV_ITEM item2;
				item2.hItem = hItem;
				item2.mask = TVIF_HANDLE;
			 	m_EqTree.GetItem(&item2);	
				if( eq.GetDba() == item2.lParam )
				{
					box.Delete();
					m_EqTree.DeleteItem(hTree);
					break;
				}
			}
		}
	}
	a.End();
}


	

void CSelectEqImageDlg::OnOK() 
{
#if FALSE
	image_equip_boxObj box;
	equipObj eq;
	image_ptrObj image;
	struct images img;
	char buff[128];

	TransAction a;
	a.Begin("wow2");
	HTREEITEM hTree = m_EqTree.GetSelectedItem();
	TV_ITEM item;
	item.hItem = hTree;
	item.mask = TVIF_HANDLE|TVIF_CHILDREN|TVIF_TEXT;
	item.pszText = buff;
	item.cchTextMax = sizeof(buff);
 	m_EqTree.GetItem(&item);	
// 	AfxMessageBox(buff);

	HTREEITEM hTreeParent = m_EqTree.GetNextItem(hTree, TVGN_PARENT );
	TV_ITEM item2;
	item2.hItem = hTreeParent;
	item2.mask = TVIF_HANDLE|TVIF_CHILDREN;
	m_EqTree.GetItem(&item2);

	eq.WriteLock();
	eq.GotoDirect(DbAddr(item2.lParam));
	if( !(eq.Okay()) )
	{
		AfxMessageBox("Couldn't find Eq");
		return;
	}
//	else
//		AfxMessageBox("Got Equipment");
	box.WriteLock();
	image.WriteLock();
	for( box.GotoMember(eq, EQUIP_BOX, FIRST );
		box.Okay(); box.GotoMember(eq, EQUIP_BOX, NEXT ) )
	{
		image.FromMember( &box, IMAGE_BOX );
		if( image.Okay() )
		{
			rdmRecLock( IMAGES, "r", TASKER );
			dt_crset( (DB_ADDR *)&(image.image_ptr::dba), TASKER );
			dt_recread( &img, TASKER );
			rdmRecFree(IMAGES, TASKER);
			if( strcmp(img.szName, buff) == 0 )
			{
				box.DisConnect(eq);
				box.DisConnect(image);
				box.Delete();
				m_EqTree.DeleteItem(hTree);
//				AfxMessageBox("Unlinked");
				break;
			}
		}
	}
	a.End();
#endif		
	CDialog::OnOK();
}

void CSelectEqImageDlg::OnOpenImage() 
{
	rvqTask->MakeCurrent();
	HTREEITEM hTree = m_EqTree.GetSelectedItem();
	TV_ITEM item;
	item.hItem = hTree;
	item.mask = TVIF_HANDLE;
 	m_EqTree.GetItem(&item);	
	HTREEITEM hItem = m_EqTree.GetParentItem(hTree);
	if( hItem == NULL )
	{
		HTREEITEM child;
		TV_ITEM treeItem;

//		AfxMessageBox("Must be a Equip node");
		child = m_EqTree.GetChildItem(hTree);
		while( child != NULL )
		{
			treeItem.hItem = child;
			treeItem.mask = TVIF_HANDLE;
			if( !(m_EqTree.GetItem(&treeItem)))
				AfxMessageBox("Cannot get Item");
			OpenImageByDba(	(DB_ADDR)(treeItem.lParam) );
//			if( (Ptg3DNode *)(treeItem.lParam) == elem )
//				return child;
			child = m_EqTree.GetNextSiblingItem(child);
		}

	}
	else
	{
//		AfxMessageBox("Must be an Image");
		HTREEITEM hTree = m_EqTree.GetSelectedItem();
		TV_ITEM item;
		item.hItem = hTree;
		item.mask = TVIF_HANDLE|TVIF_CHILDREN;
		m_EqTree.GetItem(&item);	
		OpenImageByDba((DB_ADDR)(item.lParam) );
	}
}

void CSelectEqImageDlg::OnEndlabeleditEqList(NMHDR* pNMHDR, LRESULT* pResult) 
{
/*
When label editing is canceled or completed, a tree control sends a 
TVN_ENDLABELEDIT notification message. The lParam parameter is the 
address of a TV_DISPINFO structure. The item member is a TV_ITEM 
structure that identifies the item and includes the edited text. 
You are responsible for updating the item’s label, if appropriate, 
perhaps after validating the edited string. The pszText member of 
TV_ITEM is 0 if editing is canceled. 

During label editing, typically in response to the TVN_BEGINLABELEDIT 
notification message, you can get a pointer to the edit control used 
for label editing by using the GetEditControl member function. You 
can call the edit control’s SetLimitText member function to limit 
the amount of text a user can enter or subclass the edit control to 
intercept and discard invalid characters. Note, however, that the 
edit control is displayed only after TVN_BEGINLABELEDIT is sent.

typedef struct _TV_DISPINFO {  tvdi  
    NMHDR   hdr; 
    TV_ITEM item; 
} TV_DISPINFO; 
	
*/
	TV_DISPINFO *disp;
	disp = (TV_DISPINFO *)pNMHDR;
//	TREEITEM item;

//	AfxMessageBox(disp->item.pszText);
	equipObj eq;


	rvqTask->MakeCurrent();
		
	TransAction a;
	a.Begin("wow2");

	eq.WriteLock();

	eq.GotoDirect(DbAddr(disp->item.lParam));
	
	sprintf(eq.name,"%s", disp->item.pszText);
	eq.Write();
	a.End();
	m_EqTree.SetItemText(disp->item.hItem, disp->item.pszText);
	
	DWORD style = m_CloseButton.GetButtonStyle();
	m_CloseButton.SetButtonStyle(style | BS_DEFPUSHBUTTON,TRUE);	

	*pResult = 0;
}


void CSelectEqImageDlg::OpenImageByDba( DB_ADDR dba)
{
	struct images img;

	rdmRecLock( IMAGES, "r", TASKER );
	dt_crset( &dba, TASKER );
	dt_recread( &img, TASKER );
	rdmRecFree(IMAGES, TASKER);

	OpenImageByName( img.szName );
}

void CSelectEqImageDlg::OnEquipTagData() 
{
	image_equip_boxObj box;
	equipObj eq;
	image_ptrObj image;
//	struct images img;
//	char buff[128];

//	TransAction a;
//	a.Begin("unlink");
	
//	rvqTask->MakeCurrent();
	HTREEITEM hTree = m_EqTree.GetSelectedItem();
	TV_ITEM item;
	item.hItem = hTree;
	item.mask = TVIF_HANDLE | TVIF_PARAM;
 	m_EqTree.GetItem(&item);	
	HTREEITEM hItem = m_EqTree.GetParentItem(hTree);
//AfxMessageBox("Locking EQ in select");
//	eq.ReadLock();
//	box.ReadLock();
//	image.ReadLock();
	if( hItem == NULL )
	{				// This means we are on EQUIPMENT
//		eq.GotoDirect(DbAddr(item.lParam));
//		if( !(eq.Okay()) )
//		{
//			AfxMessageBox("Couldn't find Eq");
//			return;
//		}
		
//		CPropertySheet *dlgPropertySheet;
//		EngineeringData *engrPage;
//		Maintenance *maintPage;
		
//		for( box.GotoMember(eq, EQUIP_BOX, FIRST );
//			box.Okay(); box.GotoMember(eq, EQUIP_BOX, FIRST ) )
//		{
//			box.Delete();
//			AfxMessageBox("Delete one");
//		}
//		HTREEITEM child;
//		while( (child = m_EqTree.GetChildItem(hTree)) )
//			m_EqTree.DeleteItem(child);
//		if(! eq.IsInUse() )
//			eq.Delete();
//		m_EqTree.DeleteItem(hTree);
///////////////////////////////////
		dlgPropertySheet = new CPropertySheet("Equipment Data");
	//	CPropertyPage										
		engrPage = new EngineeringData;
		constPage = new Construction;
		maintPage = new Maintenance;
		commPage = new Commisioning;

		dlgPropertySheet->AddPage(constPage);
		dlgPropertySheet->AddPage(engrPage);
		dlgPropertySheet->AddPage(commPage);
		dlgPropertySheet->AddPage(maintPage);

		dlgPropertySheet->Create(this);
		dlgPropertySheet->SetActiveWindow();
		dlgPropertySheet->SetFocus();
		
///////////////////////////////////////////////////
	}
//	AfxMessageBox("Making select task current");
//	rvqTask->MakeCurrent();
//	eq.FreeLock();
//	box.FreeLock();
//	image.FreeLock();
}

void CSelectEqImageDlg::OnBeginlabeleditEqList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	
	//CButton
	DWORD style = m_CloseButton.GetButtonStyle();
	m_CloseButton.SetButtonStyle(style & !BS_DEFPUSHBUTTON,TRUE);	
//	AfxMessageBox("Beginning to Edit");

	*pResult = 0;
}
