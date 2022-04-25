// fotogdoc.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "fotogdoc.h"

#include "paletteogl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFotogDoc

IMPLEMENT_DYNCREATE(CFotogDoc, CDocument)

CFotogDoc::CFotogDoc()
{
	m_pal = NULL;
	EnableAutomation();
}

BOOL CFotogDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CFotogDoc::~CFotogDoc()
{
	delete m_pal;
}

void CFotogDoc::OnFinalRelease()
{
	// When the last reference for an automation object is released
	//	OnFinalRelease is called.  This implementation deletes the 
	//	object.  Add additional cleanup required for your object before
	//	deleting it from memory.
//	AfxMessageBox("Got the Delete Called");
	delete this;
}


BEGIN_MESSAGE_MAP(CFotogDoc, CDocument)
	//{{AFX_MSG_MAP(CFotogDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CFotogDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CFotogDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFotogDoc diagnostics

#ifdef _DEBUG
void CFotogDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFotogDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFotogDoc serialization

void CFotogDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		if( m_pal )
		{
			delete m_pal;
			m_pal = NULL;
		}
		m_pal = new CPalette;
		m_pal->CreatePalette( GetOpenGLPalette() );
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFotogDoc commands

void CFotogDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
//	AfxMessageBox("Got the closeDocument");
	CDocument::OnCloseDocument();
}

BOOL CFotogDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
//	AfxMessageBox(lpszPathName);
//	return FALSE;
		
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	
	
	return TRUE;
}
