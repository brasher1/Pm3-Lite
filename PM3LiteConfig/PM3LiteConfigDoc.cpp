// PM3LiteConfigDoc.cpp : implementation of the CPM3LiteConfigDoc class
//

#include "stdafx.h"
#include "PM3LiteConfig.h"

#include "PM3LiteConfigDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPM3LiteConfigDoc

IMPLEMENT_DYNCREATE(CPM3LiteConfigDoc, CDocument)

BEGIN_MESSAGE_MAP(CPM3LiteConfigDoc, CDocument)
	//{{AFX_MSG_MAP(CPM3LiteConfigDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPM3LiteConfigDoc construction/destruction

CPM3LiteConfigDoc::CPM3LiteConfigDoc()
{
	// TODO: add one-time construction code here

}

CPM3LiteConfigDoc::~CPM3LiteConfigDoc()
{
}

BOOL CPM3LiteConfigDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPM3LiteConfigDoc serialization

void CPM3LiteConfigDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPM3LiteConfigDoc diagnostics

#ifdef _DEBUG
void CPM3LiteConfigDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPM3LiteConfigDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPM3LiteConfigDoc commands
