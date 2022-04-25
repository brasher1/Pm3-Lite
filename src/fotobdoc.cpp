// fotobdoc.cpp : implementation of the CFotobrowDoc class
//

#include "stdafx.h"
#include "fotobrow.h"

#include "fotobdoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFotobrowDoc

IMPLEMENT_DYNCREATE(CFotobrowDoc, CDocument)

BEGIN_MESSAGE_MAP(CFotobrowDoc, CDocument)
	//{{AFX_MSG_MAP(CFotobrowDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CFotobrowDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CFotobrowDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFotobrowDoc construction/destruction

CFotobrowDoc::CFotobrowDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();

    m_pDIB = NULL;
    m_pPal = NULL;

}

CFotobrowDoc::~CFotobrowDoc()
{
	CFotobrowApp *p_App = (CFotobrowApp *)AfxGetApp();
	AfxOleUnlockApp();
	if( m_pDIB ) delete m_pDIB;
	if( m_pPal ) delete m_pPal;
	p_App->DelDocFromList(this);
}

BOOL CFotobrowDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
    if (m_pDIB) {
        delete m_pDIB;
        m_pDIB = NULL;
    }
    if (m_pPal) {
        delete m_pPal;
        m_pPal = NULL;
    }

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFotobrowDoc serialization

void CFotobrowDoc::Serialize(CArchive& ar)
{
	CFotobrowApp *p_App = (CFotobrowApp *)AfxGetApp();
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ASSERT(1);
	}
	else
	{
		// TODO: add loading code here
		// Get the file from the archive
	//	AfxMessageBox("Serilizing the Bitmap File");
//		p_App->AddDocToList(this);
		CFile *fp = ar.GetFile();
		ASSERT(fp);
		ar.Flush();
		//Load the DIB from the file
		if( m_pDIB ) delete m_pDIB;
		m_pDIB = new CDIB;
		if( !m_pDIB->Load(fp) )
			AfxMessageBox("Failed to Load Image");

		// Create the palette from the DIB
		if( m_pPal ) delete m_pPal;
		m_pPal = new CDIBPal;
		ASSERT(m_pPal);
		if( !m_pPal->Create(m_pDIB) )
		{
			AfxMessageBox("Failed to Create the palette from DIB file");
			delete m_pPal;
			m_pPal = NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFotobrowDoc diagnostics

#ifdef _DEBUG
void CFotobrowDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFotobrowDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFotobrowDoc commands
CFotobrowView *CFotobrowDoc::GetView()
{
	POSITION pos;

	pos = GetFirstViewPosition();
	return (CFotobrowView *)GetNextView(pos);
}

int CFotobrowDoc::CheckSelected()
{
	return GetView()? GetView()->CheckSelected() : FALSE;
}

void CFotobrowDoc::setImageName(const char *str)
{ 
	szName = str;
	
	CFotobrowView *view = GetView();
	view->setImageName();	
}
