// drawlist.h : header file
//
#include "fotobrow.h"
#include "common.h"
#include <afxcmn.h>
#include "../3dclass/Ptg3DNode.h"

#include <gl/glu.h>
#include <gl/gl.h>
/////////////////////////////////////////////////////////////////////////////
// CDrawList dialog

class CDrawList : public CDialog
{
// Construction
public:
	CDrawList(CWnd* pParent = NULL);   // standard constructor
	CDrawList(CWnd* pParent = NULL, Ptg3DNode *node=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDrawList)
	enum { IDD = IDD_DRAWLIST };
	CComboBox	m_DrawStyleList;
	CComboBox	m_ColorList;
	CTreeCtrl	m_MemberList;
	CTreeCtrl	m_DrawListTree;
	int		m_Color;
	int		m_DrawStyle;
	//}}AFX_DATA
	FILE *out;
	int AddElementToTree(Ptg3DNode *elem, HTREEITEM hTreeItem);
	void setColor( Ptg3DNode *elem, GLfloat red, GLfloat green, GLfloat blue );
	void FillDrawList(Ptg3DNode *elem);
	HTREEITEM AddToDrawList(Ptg3DNode *elem, HTREEITEM hTreeItem, int recursive=TRUE, int modifiy=TRUE, int check=TRUE);
	HTREEITEM AddParent(Ptg3DNode *ptr);
	HTREEITEM FindMeInTree( HTREEITEM parent , Ptg3DNode *elem );
	HTREEITEM PruneDrawList( HTREEITEM item );
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HIMAGELIST hIml;  
	HIMAGELIST hIml2;  
	HBITMAP hBmps[20];
	Ptg3DNode *ElementList;
	int image_index[20];

	// Generated message map functions
	//{{AFX_MSG(CDrawList)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
