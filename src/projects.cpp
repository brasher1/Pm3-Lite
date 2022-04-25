// projects.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "projects.h"
#include "common.h"
#include "transfor.h"
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectSettings dialog

extern CFotobrowApp theApp;

CProjectSettings::CProjectSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectSettings)
	m_DatabasePath = _T("");
	m_ImagePath = _T("");
	m_ProjectID = _T("");
	m_ProjectSel = _T("");
	m_ReductionFactor = _T("");
	//}}AFX_DATA_INIT
}


void CProjectSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectSettings)
	DDX_Control(pDX, IDC_PROJLIST, m_ProjectList);
	DDX_Text(pDX, IDC_DATABASE_PATH, m_DatabasePath);
	DDX_Text(pDX, IDC_IMAGEPATH, m_ImagePath);
	DDX_Text(pDX, IDC_PROJECTNAME, m_ProjectID);
	DDX_LBString(pDX, IDC_PROJLIST, m_ProjectSel);
	DDX_Text(pDX, IDC_IMAGE_REDUCE_FACTOR, m_ReductionFactor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProjectSettings, CDialog)
	//{{AFX_MSG_MAP(CProjectSettings)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_LBN_DBLCLK(IDC_PROJLIST, OnDblclkProjlist)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDC_SURVEY_XFORM, OnSurveyXform)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CProjectSettings message handlers

BOOL CProjectSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
////////////////////FFFFFFFFFFFFFFFFFFFFFF///////////////////////////////////
	HKEY one,two,three;
	DWORD result;
	char value[256],classstring[256];
	DWORD length, len2;
//	char *str[] = {"SOFTWARE", "AsBuiltData", "PM3Lite", "1.0", "Projects" };
	char *stuff[] = {"DatabasePath", "ImagePath","ReductionFactor" };
	FILETIME ft;
	struct EnvStrings *pStr;
///////////////////////////////////////////
	CStringList list;

	one = HKEY_LOCAL_MACHINE;
	for( int i=0; i<3; i++ )
	{
		if( RegOpenKeyEx(one,reg_strings[i], 0L, KEY_READ, &two ) != ERROR_SUCCESS )
		{
			AfxMessageBox("Failed on key enuming");
			RegCloseKey(one);
			return FALSE;
		}
		RegCloseKey(one);
		one = two;
	}

	i=0;
	int errval;
	len2 = sizeof(classstring);
	length = sizeof(value);
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
			pStr = new struct EnvStrings;
			length = sizeof(value);
			memset(value,0,sizeof(value));
			if( RegQueryValueEx( two, stuff[0], 0L, &result, (BYTE *)value, &length) != ERROR_SUCCESS)
				AfxMessageBox("No value found");
			value[length]='\0';
			strcpy( pStr->dbPath, value);
			length = sizeof(value);
			memset(value,0,sizeof(value));
			RegQueryValueEx( two, stuff[1], 0L, &result, (BYTE *)value, &length);
			value[length] = '\0';
			strcpy(pStr->imgPath, value);

			length = sizeof(value);
			memset(value,0,sizeof(value));
			RegQueryValueEx( two, stuff[2], 0L, &result, (BYTE *)value, &length);
			value[length] = '\0';
			strcpy(pStr->imageReductionFactor, value);
			m_ProjectList.SetItemData((m_ProjectList.AddString((const char *)temp)), (LONG)pStr);
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
	}	
	UpdateData(TRUE);
	
////////////////////FFFFFFFFFFFFFFFFFFFFFF///////////////////////////////////
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProjectSettings::OnRemove() 
{
	AfxMessageBox("Cannot remove projects - contact Administrator");
}

void CProjectSettings::OnApply() 
{
	HKEY one,two,three;
//	DWORD result;
	char value[256],classstring[256];
	DWORD length, len2;
//	char *str[] = {"SOFTWARE", "AsBuiltData", "PM3Lite", "1.0", "Projects" };
	char *stuff[] = {"DatabasePath", "ImagePath", "ReductionFactor" };
	FILETIME ft;
//	struct EnvStrings *pStr;
///////////////////////////////////////////
	CStringList list;
	CString name;

	int index;

	
	if( (index=m_ProjectList.GetCurSel()) == LB_ERR )
		return;
	m_ProjectList.GetText(index, name);

	one = HKEY_LOCAL_MACHINE;
	for( int i=0; i<3; i++ )
	{
		if( RegOpenKeyEx(one,reg_strings[i], 0L, KEY_READ, &two ) != ERROR_SUCCESS )
		{
			AfxMessageBox("Failed on key enuming");
			RegCloseKey(one);
			return;
		}
		RegCloseKey(one);
		one = two;
	}

	i=0;
	int errval;
	len2 = sizeof(classstring);
	length = sizeof(value);
	UpdateData(TRUE);
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
//		AfxMessageBox("Inside While");
		if( strcmp(value, m_ProjectID ) == 0 )
		{
//			AfxMessageBox("Inside project Compare");
			if( RegOpenKeyEx(one, value, 0L, KEY_QUERY_VALUE|KEY_WRITE, &two ) == ERROR_SUCCESS )
			{
				RegSetValueEx( two, stuff[0],0, REG_SZ, 
						(const unsigned char*)(const char *)m_DatabasePath, 
						m_DatabasePath.GetLength() );
				RegSetValueEx( two, stuff[1],0, REG_SZ, 
						(const unsigned char*)(const char *)m_ImagePath, 
						m_ImagePath.GetLength() );
				RegSetValueEx( two, stuff[2],0, REG_SZ, 
						(const unsigned char*)(const char *)m_ReductionFactor, 
						m_ReductionFactor.GetLength() );
//				AfxMessageBox("RegSettingValues");
				RegCloseKey(one);
				return;
			}
			else
			{
				AfxMessageBox( "Couldn't open subkey" );
				break;
			}
		}
		three = two;
		i++;
		len2 = sizeof(classstring);
		length = sizeof(value);
	}	
	RegCloseKey(one);
	struct EnvStrings str;
	strcpy(	str.dbPath,m_DatabasePath); 
	strcpy( str.imgPath,m_ImagePath);
	strcpy( str.imageReductionFactor,m_ReductionFactor); 
	theApp.OnSetProject(&str);
	UpdateData(TRUE);
}


void CProjectSettings::OnOK() 
{
	struct EnvStrings str;
	strcpy(	str.dbPath,m_DatabasePath); 
	strcpy( str.imgPath,m_ImagePath);
	strcpy( str.imageReductionFactor,m_ReductionFactor); 
	theApp.OnSetProject(&str);
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

void CProjectSettings::OnDblclkProjlist() 
{
	struct EnvStrings *pStr;

	int index;
	if( (index=m_ProjectList.GetCurSel()) != LB_ERR )
	{
		pStr = (struct EnvStrings *)(m_ProjectList.GetItemData(index));
		if( pStr != NULL )
		{
			m_DatabasePath = pStr->dbPath;
			m_ImagePath = pStr->imgPath;
			m_ReductionFactor = pStr->imageReductionFactor;
			m_ProjectList.GetText(index, m_ProjectID);
		}
	}
	UpdateData(FALSE);
}

void CProjectSettings::OnSurveyXform() 
{
	CTransformDlg dlg;
	dlg.setProjectID(m_ProjectID);
	dlg.DoModal();	
}
