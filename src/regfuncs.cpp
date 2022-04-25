// regfuncs.cpp
//
#include "stdafx.h"
#include "fotobrow.h"
#include "regfuncs.h"

int getSurveyModelTransform(double *dest, const char *projectID)
{
	return getTransform("SurveyModelTransform",dest, projectID);
}
int saveSurveyModelTransform(double *source, const char *projectID)
{
	return saveTransform("SurveyModelTransform", source, projectID);
}
/////////////////////////////////////////////////////////////////////////////////////
int getMeasurementTransform(double *dest, const char *projectID)
{
	return getTransform("MeasurementTransform",dest, projectID);
}
int saveMeasurementTransform(double *source, const char *projectID)
{
	return saveTransform("MeasurementTransform",source, projectID);
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
int getTransform(const char *keyname, double *dest, const char *projectID)
{
	HKEY one,two,three;
	DWORD result;
	char value[256],classstring[256];
	DWORD length, len2;
//	char *str[] = {"SOFTWARE", "AsBuiltData", "PM3Lite", "1.0", "Projects" };
//	char *stuff[] = {"MeasurementTransform"};
	FILETIME ft;
//	struct EnvStrings *pStr;
///////////////////////////////////////////
	CStringList list;
	CString name;

//	int index;

	one = HKEY_LOCAL_MACHINE;
	for( int i=0; i<3; i++ )
	{
		if( RegOpenKeyEx(one,reg_strings[i], 0L, KEY_READ, &two ) != ERROR_SUCCESS )
		{
			AfxMessageBox("Failed on key enuming");
			RegCloseKey(one);
			return -1;
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
//		AfxMessageBox("Inside While");
		if( strcmp(value, projectID ) == 0 )
		{
//			AfxMessageBox("Inside project Compare");
			if( RegOpenKeyEx(one, value, 0L, KEY_QUERY_VALUE|KEY_WRITE, &two ) == ERROR_SUCCESS )
			{
				if( RegOpenKeyEx(two, keyname, 0L, KEY_QUERY_VALUE|KEY_WRITE, &three ) == ERROR_SUCCESS )
				{
					
					for( int m=0; m<12; m++	)
					{
						char xform_name[128];
						sprintf(xform_name, "transform%0d", m );
						//AfxMessageBox(xform_name);
						length = sizeof(value);
						if( RegQueryValueEx( three, xform_name, 0L, &result, (BYTE *)value, &length) != ERROR_SUCCESS)
						{
							//AfxMessageBox("No value found");
							switch(m)
							{
								case 0:
								case 4:
								case 8:
									dest[m] = 1.0;
									break;
								default:
									dest[m] = 0.0;
									break;
							}
							sprintf(value,"%f", dest[m]);
							RegSetValue(two, xform_name, REG_SZ, value, strlen(value));
						}
						//AfxMessageBox(value);
						value[length]='\0';
						dest[m] = atof(value);
					}
					RegCloseKey(three);
				}
				RegCloseKey(two);
				RegCloseKey(one);
				return -1;
			}
			else
			{
				AfxMessageBox( "Couldn't open transform subkey" );
				break;
			}
		}
//		three = two;
		i++;
		len2 = sizeof(classstring);
		length = sizeof(value);
	}	
	RegCloseKey(one);
	//UpdateData(TRUE);
 	return -1;
}

int saveTransform(const char *keyname, double *source, const char *projectID)
{
	HKEY one,two,three;
//	DWORD result;
	char value[256],classstring[256];
	DWORD length, len2;
//	char *str[] = {"SOFTWARE", "AsBuiltData", "PM3Lite", "1.0", "Projects" };
//	char *stuff[] = {"MeasurementTransform"};
	FILETIME ft;
//	struct EnvStrings *pStr;
///////////////////////////////////////////
	CStringList list;
	CString name;

//	int index;

	one = HKEY_LOCAL_MACHINE;
	for( int i=0; i<3; i++ )
	{
		if( RegOpenKeyEx(one,reg_strings[i], 0L, KEY_READ, &two ) != ERROR_SUCCESS )
		{
			AfxMessageBox("Failed on key enuming");
			RegCloseKey(one);
			return -1;
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
//		AfxMessageBox("Inside While");
		if( strcmp(value, projectID ) == 0 )
		{
//			AfxMessageBox("Inside project Compare");
			if( RegOpenKeyEx(one, value, 0L, KEY_QUERY_VALUE|KEY_WRITE, &two ) == ERROR_SUCCESS )
			{
				if( RegOpenKeyEx(two, keyname, 0L, KEY_QUERY_VALUE|KEY_WRITE, &three ) == ERROR_SUCCESS )
				{
					for( int m=0; m<12; m++	)
					{
						char xform_name[128];
						sprintf(xform_name, "transform%0d", m );
						//AfxMessageBox(xform_name);
						length = sizeof(value);
						sprintf(value,"%f", source[m]);
						RegSetValueEx(three, xform_name, 0L, REG_SZ, (CONST BYTE *)value, strlen(value));
					}
					return 0;
				}					
				return -1;
			}
			else
			{
				AfxMessageBox( "Couldn't open transform subkey" );
				break;
			}
		}
		three = two;
		i++;
		len2 = sizeof(classstring);
		length = sizeof(value);
	}	
	//UpdateData(TRUE);
 	return -1;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
int getUnits()
{
	HKEY key;
	DWORD result;
	char value[256];
	DWORD length;

	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\AsBuiltData\\PM3Lite",0,
		KEY_READ, &key) != ERROR_SUCCESS )
	{
		AfxMessageBox("Error in Registry:Units/PM3Lite - Run setup again");
		::PostQuitMessage(0);
		return -1;
	}
	length=256;
	if( RegQueryValueEx( key, "units", 0L, &result, (BYTE *)value, &length) != ERROR_SUCCESS)
	{
		AfxMessageBox("Error in Registry:Units/Value - Run setup again");
		::PostQuitMessage(0);
		return -1;
	}
	RegCloseKey(key);
	DWORD val = *(DWORD *)value;
	return val;
}

int saveUnits(int units)
{
	HKEY key;
//	DWORD result;
//	char value[256];
//	DWORD length;

	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\AsBuiltData\\PM3Lite",0,
		KEY_READ|KEY_WRITE, &key) != ERROR_SUCCESS )
	{
		AfxMessageBox("Error in Setting Registry:Units/PM3Lite");
		::PostQuitMessage(0);
		return -1;
	}
	if( RegSetValueEx(key, "units", 0L, REG_DWORD, (CONST BYTE *)&units, sizeof(DWORD)) != ERROR_SUCCESS )
	{
		AfxMessageBox("Error in Setting Registry:Units/Value");
		::PostQuitMessage(0);
		return -1;
	}
	RegCloseKey(key);
	return 0;
}
