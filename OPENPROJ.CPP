// openproj.cpp : implementation file
//

#include "stdafx.h"
#ifdef FOTOBROW
# include <afxdisp.h>
# include "fotobrow.h"
#else
# include "maintsys.h"
#endif
#include "openproj.h"
#include "common.h"
#include "maintcom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


////	/////////////////////////////////////////////////////////////////////////
// COpenProjectDlg dialog


COpenProjectDlg::COpenProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenProjectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenProjectDlg)
	m_ProjectPick = _T("");
	//}}AFX_DATA_INIT
}


void COpenProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenProjectDlg)
	DDX_Control(pDX, IDC_PROJECT_LIST, m_ProjectList);
	DDX_LBString(pDX, IDC_PROJECT_LIST, m_ProjectPick);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenProjectDlg, CDialog)
	//{{AFX_MSG_MAP(COpenProjectDlg)
	ON_LBN_DBLCLK(IDC_PROJECT_LIST, OnDblclkProjectList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COpenProjectDlg message handlers

void COpenProjectDlg::OnOK() 
{
	struct EnvStrings *pStr = NULL;
	int index = m_ProjectList.GetCurSel();
	if( index >= 0 )
	{
		pStr = (struct EnvStrings *)(m_ProjectList.GetItemData(index));
		m_ProjectList.GetText(index, pStr->projectID );
	}
	else
	{
		AfxMessageBox("You must select a project");
		return;
	}
#ifdef FOTOBROW 
	((CFotobrowApp *)AfxGetApp())->OnSetProject(pStr);
#else
	((CMaintSysApp *)AfxGetApp())->OnSetProject(pStr);
#endif
	for(int i=m_ProjectList.GetCount(); i>=0; i--)
	{
		DWORD value = m_ProjectList.GetItemData(i);
		if( value != LB_ERR )
		{
			delete (struct EnvStrings *)value;
		}
	}
	CDialog::OnOK();
}

void COpenProjectDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL COpenProjectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
///////////////////////////////////
	HKEY one,two,three;
	DWORD result;
	char value[256],classstring[256];
	DWORD length, len2;
//	char *str[] = {"SOFTWARE", "AsBuiltData", "PM3Lite", "1.0", "Projects" };
	char *stuff[] = {"DatabasePath", "ImagePath", "ReductionFactor"};
	FILETIME ft;
//	AfxMessageBox("On Init");
	struct EnvStrings *pStr;
///////////////////////////////////////////
	CStringList list;

	one = HKEY_LOCAL_MACHINE;
	for( int i=0; i<3; i++ )
	{
/*		if( RegOpenKeyEx(one,reg_strings[i], 0L, KEY_READ, &two ) != ERROR_SUCCESS )
		{
			AfxMessageBox("Failed on key enuming");
			RegCloseKey(one);
			return FALSE;
		}
		RegCloseKey(one);
		one = two;*/
		if( RegCreateKeyEx(one,reg_strings[i], 0L, NULL,REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE,NULL, &two, &result ) != ERROR_SUCCESS )
		{
			RegCloseKey(two);
			if( RegOpenKeyEx(one,reg_strings[i], 0L, KEY_READ, &two ) != ERROR_SUCCESS )
			{
				AfxMessageBox("Failed on key enuming");
				RegCloseKey(one);
				return FALSE;
			}
		}
		RegCloseKey(one);
		one = two;
	}
//	RegSaveKey(one,"e:\\dev\\execution\\asblt.2", 0L );

	if( result == REG_CREATED_NEW_KEY )
	{
/*		
		AfxMessageBox("Pm3 Lite has not been setup properly on this computer.\r\n"
					"Exit the program and login as an Administrator and\r\n"
					"restore the registry file \"demo.reg\" undex the registry key \r\n"
					"\"HKEY_LOCAL_MACHINE\\SOFTWARE\\AsBuiltData\\PM3Lite\\1.0\\Projects\"\r\n"
					"Use the program regedt32.exe");
*//*		DWORD status;
		if( (status=RegRestoreKey(one, "e:\\dev\\execution\\asblt.key",0L)) != ERROR_SUCCESS )
		{
			AfxMessageBox("Registry not restored");
			sprintf(value,"status = %d", status);
			AfxMessageBox(value);
		}
		else
			AfxMessageBox("Registry restored");*/
		AfxMessageBox("Pm3 Lite has not been setup properly on this computer.\r\n"
						"Press cancel on the Open Projects Dialog Box and\r\n"
						"run setup again");
	}

	i=0;
	int errval;
	len2 = sizeof(classstring);
	length = sizeof(value); 
///////////////////////////////////////////
//	AfxMessageBox("Into the RegEnum");

	while( (errval = RegEnumKeyEx(one,	// handle of key to enumerate 
    			i,	// index of subkey to enumerate 
    			value,	// address of buffer for subkey name 
    			&length,	// address for size of subkey buffer 
    			0L,	// reserved 
    			classstring,	// address of buffer for class string 
    			&len2,	// address for size of class buffer 
    			&ft 	// address for time key last written to 
   				)) == ERROR_SUCCESS )
	{
		if( RegOpenKeyEx(one, value, 0L, KEY_QUERY_VALUE, &two ) == ERROR_SUCCESS )
		{
			CString temp;
			temp=(char *)value;
//			AfxMessageBox(value);
			pStr = new struct EnvStrings;
////
//#if FALSE
			length = sizeof(value);
			memset(value,0,sizeof(value));
			if( RegQueryValueEx( two, stuff[0], 0L, &result, (BYTE *)value, &length) != ERROR_SUCCESS)
				AfxMessageBox("No value found");
//			AfxMessageBox(value);
			value[length]='\0';
			strcpy( pStr->dbPath, value);
			length = sizeof(value);
			memset(value,0,sizeof(value));
			RegQueryValueEx( two, stuff[1], 0L, &result, (BYTE *)value, &length);
			value[length] = '\0';
//			AfxMessageBox(value);
			strcpy(pStr->imgPath, value);
//#endif
			length = sizeof(value);
			memset(value,0,sizeof(value));
			RegQueryValueEx( two, stuff[2], 0L, &result, (BYTE *)value, &length);
			value[length] = '\0';
//			AfxMessageBox(value);
			strcpy(pStr->imageReductionFactor, value);
////
			m_ProjectList.SetItemData((m_ProjectList.AddString((const char *)temp)), (LONG)pStr);
//			delete pStr;
		}
		else
		{
			AfxMessageBox( "Couldn't open subkey" );
			break;
		}
		three = two;
		i++;
		len2 = sizeof(classstring);
		length = sizeof(value); 
		RegCloseKey(two);
//		AfxMessageBox("Next Subkey");
	}	
	RegCloseKey(one);
//	char szTemp[128];
//	sprintf( szTemp, "After Enuming - %d(0x%0X)", errval, errval );
//	AfxMessageBox(szTemp);
//	AfxMessageBox("Updated Data");
	UpdateData(FALSE);
///////////////////////////////////	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int COpenProjectDlg::setProjectsList(HKEY one1)
{ 
	one = one1; 
//	AfxMessageBox("Setting the List"); 
	return 0;
}

void COpenProjectDlg::OnDblclkProjectList() 
{
	OnOK();
}
