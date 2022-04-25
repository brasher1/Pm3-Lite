// ElementListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CElementListView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "fotobrow.h"
#include "common.h"
#include <afxcmn.h>
#include "3dclass/Ptg3DNode.h"

#include <gl/glu.h>
#include <gl/gl.h>


class CElementListView : public CFormView
{
protected:
	CElementListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CElementListView)

// Form Data
public:
	//{{AFX_DATA(CElementListView)
	enum { IDD = IDD_ELEMENT_LIST_VIEW };
	CTreeCtrl	m_ElementList;
	//}}AFX_DATA

// Attributes
public:
	HIMAGELIST hIml;  
	HIMAGELIST hIml2;  
	HBITMAP hBmps[20];
	Ptg3DNode *ElementList;
	int image_index[20];

// Operations
public:
	void FillDrawList(Ptg3DNode *elem);
	HTREEITEM AddToDrawList(Ptg3DNode *elem, HTREEITEM parent, int recursive=TRUE, int modify=TRUE, int check=TRUE);
	HTREEITEM AddParent(Ptg3DNode *elem);
	HTREEITEM FindMeInTree( HTREEITEM parent , Ptg3DNode *elem );
	HTREEITEM PruneDrawList( HTREEITEM item );
	void OnMemberlistVisible(int style);
	void recurse3DNodeTree(Ptg3DNode *d, int style);
	void recurseElementTree(HTREEITEM parent, int style);
	void RecurseColorSet(Ptg3DNode *elem, COLORREF color);
	void PurgeExtraData(HTREEITEM node);

	void OnColorBlack();
	void OnColorMaroon();
	void OnColorOlive();
	void OnColorGreen();	
	void OnColorTeal();
	void OnColorNavy();
	void OnColorWine();
	void OnColorGrey();
	void OnColorPink();
	void OnColorOrange();
	void OnColorChartruse();
	void OnColorPurple();
	void OnColorSkyBlue();
	void OnColorRed();
	void OnColorYellow();
	void OnColorLime();
	void OnColorAqua();
	void OnColorBlue();
	void OnColorMagenta();
	void OnColorWhite();	

	void DoColorSet(COLORREF color);

// Overrides
// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElementListView)
	public:
		virtual void OnInitialUpdate();
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CElementListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CElementListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangedElementList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRclickElementList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMemberlistProperties();
	afx_msg void OnMemberlistVisibleSolid();
	afx_msg void OnMemberlistVisibleWireframe();
	afx_msg void OnMemberlistVisibleInvisible();
	afx_msg void OnMemberlistSetAsReferenceNode();
	afx_msg void OnMemberListCalcCircBy3DPoints();
	afx_msg void OnDestroy();
	afx_msg void OnMemberlistJwbTest();
	afx_msg void OnMemberlistDelete();
	afx_msg void OnMemberlistSavedgn();
	//}}AFX_MSG
	afx_msg LONG OnCurrentElementChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
