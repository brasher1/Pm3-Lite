// numcnvt.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
//#include <calc.h>
double str_value(char *str);

char *strsub(char *s, int sub, int key )
{
	char *temp;
	
	temp = s;
	while( *temp != '\0' )
	{
		if( *temp == key )
			*temp = sub;
		temp++;
	}
	return s;
}

char *strstrip(char *s, int key )
{
	char *temp1;
	char *temp2;
	char str[128];
//	int i;
		
	temp1 = s;
	temp2 = str;
	
	memset(str,0,sizeof(str));
		
	while( *temp1 != '\0' )
	{       
		if( *temp1 != key ) 
		{
			*temp2 = *temp1;		
			temp2++;
		}           
		temp1++;
	}        
	strcpy(s,str);       
	return s;
}

char *ConvertUnits( char *s, int size, double x1 )
{
double base_denom;
int df_flag;
//double x, y;
double feet, inches, frac_in, numfrac, denfrac;//, value;
char temp[48], temp2[48], answer_str[80];
char ft_mark[]= {"\'\0"};
char in_mark[]= {"\"\0"};
int redu_flag, app_flag;

df_flag = 1;
base_denom = 64.0;

memset(s,0,size);

if( df_flag == 0 )
	gcvt(x1,12,s);
else
	{
	if( x1 < 0 )
		strcpy( answer_str, "-" );
	else
		strcpy( answer_str, "" );
	x1 = fabs(x1);
	inches = floor(fmod(x1,12));
	frac_in = fmod(x1,12) - inches;
	redu_flag = 1;
	denfrac = base_denom;
	app_flag = 1;
	numfrac = (denfrac*frac_in);
	if( floor(numfrac) == numfrac )
		{
		numfrac = floor(numfrac);
		app_flag = 0;
		}
	else if( numfrac - floor(numfrac) >= .5)
		numfrac = ceil(numfrac);
	else
		numfrac = floor(numfrac);
	while( redu_flag == 1 )
		{
		if( fmod(denfrac,2)==0 && fmod(numfrac,2)==0)
			{
			denfrac = denfrac / 2;
			numfrac = numfrac / 2;
			}
		else if( fmod(denfrac,3)==0 && fmod(numfrac,3)==0)
			{
			denfrac = denfrac / 3;
			numfrac = numfrac / 3;
			}
		else if( fmod(denfrac,5)==0 && fmod(numfrac,5)==0)
			{
			denfrac = denfrac / 5;
			numfrac = numfrac / 5;
			}
		else
			redu_flag = 0;
		}
	if(numfrac == denfrac)
		{
		inches = inches + 1;
		frac_in = frac_in - 1;
		numfrac = 0;
		}
	feet = (x1-inches-frac_in)/12;

	// Addition by jwb attempting to correct for 24" coming out 1'12"
	if( inches == 12 )
	{
		feet += 1;
		inches -= 12;
	}

	gcvt(feet,10,temp2);
	gcvt(fabs(inches),3, temp);
	strcat( answer_str, temp2 );
	strcat(answer_str, ft_mark);
	strcat(answer_str, temp);
	if( numfrac != 0 )
		{
		strcat(answer_str, " \0");
		strcat(answer_str,gcvt(numfrac,12, temp));
		strcat(answer_str, "/\0");
		strcat(answer_str,gcvt(denfrac,12, temp));
		}
	strcat(answer_str, in_mark);
	strcat(answer_str, "\0");
	if( app_flag == 1 )
		{
//		strcat(answer_str, "\xF7\0");
		app_flag = 0;
		}
	if( strlen(answer_str) > 45)
		gcvt(x1,12,s);
	else
		strcpy(s,answer_str);
	}
	strstrip(s,'.');
	return s;
}

BOOL EnglishToMetric( char *source, double *destination)
{
	char temp[128];
	char *ps, *pt;
	int num, denom;
	int flag=0;
			
	ps = source;
	pt = temp;

	memset(temp,0,sizeof(temp));
	while( *ps != '\0' )
	{
		switch( *ps )
		{
			case '\'':
				*destination = *destination + atoi(temp)*12;
				memset(temp,0,sizeof(temp));
				ps++;
				break;
			case '\"':
				*destination = *destination + atoi(temp);
				memset(temp,0,sizeof(temp));
				ps++;
				flag = 1;
				break;
			case '/':
				if( flag == 1 )
				{
					flag = 2;
					num = atoi(temp);
					memset(temp,0,sizeof(temp));
					ps++;
				}
				break;
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				*pt = *ps;
				pt++;
				ps++;
				break;
		}
		if( flag == 2 )
		{
			denom = atoi(temp);
			*destination = *destination + (double)num/(double)denom;
		}
	}	
	return TRUE;
}
