#include "stdafx.h"
#include "fotobrow.h"

void AFXAPI DDX_Length( CDataExchange* pDX, int nIDC, double value )
{
	char tempStr[256];
	CString temp;
	
	//if m_bSaveAndValidate, then set the data state from the controls
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Text(pDX,nIDC,temp);
		strcpy(tempStr,(const char *) temp);
	}
	else
	{
		if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
			sprintf( tempStr, "%lf", value );
		else
			ConvertUnits( tempStr, sizeof(tempStr), value/25.4 );

		temp = tempStr;
		DDX_Text(pDX,nIDC,temp);
	}
}

void AFXAPI DDX_Text(CDataExchange *pDX, int nIDC, char *data)
{   
	CString temp;
	
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Text(pDX,nIDC,temp);
		_fstrcpy(data,(const char *) temp);
	}
	else
	{
		temp = data;
		DDX_Text(pDX,nIDC,temp);
	}
}

void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, short& value)
{
	int temp;
	
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Text(pDX,nIDC,temp);
		value = temp;
	}
	else
	{
		temp = value;
		DDX_Text(pDX,nIDC,temp);
	}
}

void AFXAPI DDX_Zerofill(CDataExchange* pDX, int nIDC, short& value, int digits)
{
	int temp;
	char tempstr[100];  // how is it that CStrings are an improvement?
	
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Text(pDX,nIDC,temp);
		value = temp;
	}
	else
	{
		sprintf(tempstr,"%0*d",digits,value);
		DDX_Text(pDX,nIDC,tempstr);
	}
}

void AFXAPI DDX_SeqFormat(CDataExchange* pDX, int nIDC, double &value)
{
	CString temp;
	char tempstr[100];  // how is it that CStrings are an improvement?
	
	if (pDX->m_bSaveAndValidate)
		DDX_Text(pDX,nIDC,value);
	else
	{
		sprintf(tempstr,"%.2f",value);
		DDX_Text(pDX,nIDC,tempstr);
	}
}

void AFXAPI DDV_InList(CDataExchange *pDX, int nIDC, const char *find)
{
	if (pDX->m_bSaveAndValidate)
	{
		CComboBox *cbox = (CComboBox *) pDX->m_pDlgWnd->GetDlgItem(nIDC);
		if (cbox->FindString(-1,find) == CB_ERR)
		{
			AfxMessageBox("ERR: Invalid Record ID");
			pDX->Fail();
		}
 	}
}

void AFXAPI DDX_CBString(CDataExchange *pDX, int nIDC, char *data)
{   
	CString temp;
	
	if (pDX->m_bSaveAndValidate)
	{
		DDX_CBString(pDX,nIDC,temp);
		_fstrcpy(data,(const char *) temp);
	}
	else
	{
		temp = data;
		DDX_CBString(pDX,nIDC,temp);
	}
}

void AFXAPI DDX_Check(CDataExchange *pDX, int nIDC, short &data)
{   
	int temp;
	
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Check(pDX,nIDC,temp);
		data = temp;
	}
	else
	{
		temp = data;
		DDX_Check(pDX,nIDC,temp);
	}
}

void AFXAPI DDX_Text(CDataExchange *pDX, int nIDC, char &data)
{   
	CString temp;
	char string[16];
	
	if (pDX->m_bSaveAndValidate)
	{
		((CEdit *) pDX->m_pDlgWnd->GetDlgItem(nIDC))->GetWindowText(temp);
		if (temp.GetLength() > 0)
			data = temp.GetAt(0);
		else
			data = 0;
	}
	else
	{
		sprintf(string,"%c",data);
		temp = string;
		((CEdit *) pDX->m_pDlgWnd->GetDlgItem(nIDC))->SetWindowText(temp);
	}
}

void AFXAPI DDX_Check(CDataExchange *pDX, int nIDC, char &data)
{   
	int temp;
	
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Check(pDX,nIDC,temp);
		data = temp;
	}
	else
	{
		temp = data;
		DDX_Check(pDX,nIDC,temp);
	}
}

void AFXAPI DDX_Radio(CDataExchange *pDX, int nIDC, short &data)
{   
	int temp;
	
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Radio(pDX,nIDC,temp);
		data = temp;
	}
	else
	{
		temp = data;
		DDX_Radio(pDX,nIDC,temp);
	}
}

void AFXAPI DDX_CBString(CDataExchange *pDX, int nIDC, char data)
{   
	CString temp;
	char string[4];
	
	if (pDX->m_bSaveAndValidate)
	{
		DDX_CBString(pDX,nIDC,temp);
		data = temp.GetAt(0);
	}
	else
	{
		sprintf(string,"%c",data);
		temp = string;
		DDX_CBString(pDX,nIDC,temp);
	}
}

void AFXAPI DDX_BitCheck(CDataExchange *pDX, int nIDC, short &data, short bit)
{   
	int temp;
	
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Check(pDX,nIDC,temp);
		if (temp)
			data = data | bit;
		else
			data = data & (~ bit);
	}
	else
	{
		temp = (data & bit) ? 1 : 0;
		DDX_Check(pDX,nIDC,temp);
	}
}
