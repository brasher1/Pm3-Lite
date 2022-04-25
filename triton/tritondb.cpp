// tritondb.cpp : implementation file
//

#include "stdafx.h"
//#include "fotobrow.h"
#include "tritondb.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRaimaDatabaseDoc

IMPLEMENT_DYNCREATE(CRaimaDatabaseDoc, CDocument)

CRaimaDatabaseDoc::CRaimaDatabaseDoc()
{
}

BOOL CRaimaDatabaseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CRaimaDatabaseDoc::~CRaimaDatabaseDoc()
{
}


BEGIN_MESSAGE_MAP(CRaimaDatabaseDoc, CDocument)
	//{{AFX_MSG_MAP(CRaimaDatabaseDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CRaimaDatabaseDoc diagnostics

#ifdef _DEBUG
void CRaimaDatabaseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRaimaDatabaseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRaimaDatabaseDoc serialization

void CRaimaDatabaseDoc::Serialize(CArchive& ar)
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
// CRaimaDatabaseDoc commands
