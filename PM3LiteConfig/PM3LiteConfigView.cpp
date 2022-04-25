// PM3LiteConfigView.cpp : implementation of the CPM3LiteConfigView class
//

#include "stdafx.h"
#include "PM3LiteConfig.h"

#include "PM3LiteConfigDoc.h"
#include "PM3LiteConfigView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPM3LiteConfigView

IMPLEMENT_DYNCREATE(CPM3LiteConfigView, CView)

BEGIN_MESSAGE_MAP(CPM3LiteConfigView, CView)
	//{{AFX_MSG_MAP(CPM3LiteConfigView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPM3LiteConfigView construction/destruction

CPM3LiteConfigView::CPM3LiteConfigView()
{
	// TODO: add construction code here

}

CPM3LiteConfigView::~CPM3LiteConfigView()
{
}

BOOL CPM3LiteConfigView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPM3LiteConfigView drawing

void CPM3LiteConfigView::OnDraw(CDC* pDC)
{
	CPM3LiteConfigDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CPM3LiteConfigView printing

BOOL CPM3LiteConfigView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPM3LiteConfigView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPM3LiteConfigView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPM3LiteConfigView diagnostics

#ifdef _DEBUG
void CPM3LiteConfigView::AssertValid() const
{
	CView::AssertValid();
}

void CPM3LiteConfigView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPM3LiteConfigDoc* CPM3LiteConfigView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPM3LiteConfigDoc)));
	return (CPM3LiteConfigDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPM3LiteConfigView message handlers
