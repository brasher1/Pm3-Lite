#ifndef __FOTOBROW_H__
#define __FOTOBROW_H__

// fotobrow.h : main header file for the FOTOBROW application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

//#define NO_DT_COVER
#include "editbar.h"
#include "dib.h"
#include "dibpal.h"
#include "fotogdoc.h"
//#define _FOTOBROW_CPP_
#include "common.h"
#include <storeobj.hpp>

#include "3dclass/Ptg3DNode.h"
//#include <vista.h>

 // Global Variables
extern HCURSOR hMainCursor; 		//Current application cursor
//extern HCURSOR hViewCursor; 		//Current application cursor

/////////////////////////////////////////////////////////////////////////////
// CFotobrowApp:
// See fotobrow.cpp for the implementation of this class
//

class CFotobrowApp : public CWinApp
{
public:
	CFotobrowApp();
	~CFotobrowApp();

	CMultiDocTemplate* m_pDocTemplate;
	CMultiDocTemplate* m_pGraphicTemplate;
	CMultiDocTemplate* m_pZoneGridTemplate;
	CMultiDocTemplate* m_pColumnGridTemplate;
	CMultiDocTemplate* m_pElementListTemplate;
		
	CObList m_DocList;

	DB_TASK MainTask;

	int m_Units;
	CString m_ImagePath;
	double m_Transform[12];
	CString m_DatabasePath;
	CString m_ProjectName;
	double m_imageReduction;

	int databaseOpenFlag;

	void SetImagePath(char *s) { m_ImagePath = s; }
	void SetUnits(int i ) { m_Units = i; }
	void SetReferenceNode( Ptg3DNode *newRefNode );

	afx_msg LONG OnSetProject( struct EnvStrings *pStr );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFotobrowApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	void GetDocumentList( CObList *pDocList );
	void KillTheTAF();
	
	double * GetTransform() { return m_Transform; }
	void SetTransform( double data[] );

	void WriteProfile();
	int ReadProfile();
	int ReadRegistry();
	void WriteRegistry();

	int isDatabaseOpen(){ return databaseOpenFlag; }

	COleTemplateServer m_server;
		// Server object for document creation

	//{{AFX_MSG(CFotobrowApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void AddDocToList(CDocument *p);
	void DelDocFromList(CDocument *p);
	CObList *GetDocList(){ return &m_DocList; }
};


/////////////////////////////////////////////////////////////////////////////
#endif //__FOTOBROW_H__
