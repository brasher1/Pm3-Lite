#include "TextToColumns.h"

/****************************************************************************
**
**	Locate occurences of multiple single characters in sequence and replace
**	with a single occurence.
**
**	int compressDelims( CString &string, CString &delims )
**
**	Return Value:
**		TRUE if successful.  FALSE if unsuccessful.
**
**	Parameters:
**		string	string in which to compress delimiters.
**		delims	string containing single character delimiters.
**
****************************************************************************/
int compressDelims( CString &string, CString &delims )
{
	CString result;

	int insideDelimString = TRUE;
	for( int i = 0; i < string.GetLength(); i++ )
	{
		if( delims.Find( string[i] ) == -1 )
		{
			result += string[i];
			insideDelimString = FALSE;
		}
		else if( !insideDelimString )
		{
			result += string[i];
			insideDelimString = TRUE;
		}
	}

	string = result;

	return TRUE;
}

/****************************************************************************
**
**	Break a string into fields of data.
**
**	int TextToColumns( char *text, char ***columnData, int *numColumns, 
**			char *columnDelims, BOOL multDelimsFlag )
**
**	Return Value:
**		Number of fields found in the string.
**
**	Parameters:
**		text		input string
**		columnData	pointer to array of character strings where column data 
**					is stored
**		numColumns	number of columns(fields) found
**		columnDelims	string containing single character delimiters for 
**						separating the fields.  All delimiters are single 
**						character and treated equally.
**		multDelimFlags	indicates how to handle sequential delimiters.  TRUE
**						will cause sequential delimiters to be treated as one.
**						FALSE will cause sequential delimiters to be treated
**						as delimiting empty fields.
**
****************************************************************************/
int TextToColumns( char *text, char ***columnData, int *numColumns, char *columnDelims, BOOL multDelimsFlag )
{
	*columnData = NULL;
	*numColumns = 0;

	if( text == NULL )
		return(*numColumns);

	CString csText(text);

	if( multDelimsFlag )
		compressDelims( csText, CString(columnDelims) );

	int numFields = 0;
	int index = 0, lastIndex = 0;
	CString currField;

	while( (index = csText.FindOneOf(columnDelims)) != -1 && (index+1) != csText.GetLength() )
	{
		currField = csText.Left(index);
		csText = csText.Right(csText.GetLength() - index - 1);

		numFields++;
		*columnData = (char **)realloc(*columnData, numFields*sizeof(char *));
		(*columnData)[numFields-1] = strdup((LPCSTR)currField);

		lastIndex = index;
	}

	if( csText.GetLength() > 0 )
	{
		numFields++;
		*columnData = (char **)realloc(*columnData, numFields*sizeof(char *));
		(*columnData)[numFields-1] = strdup((LPCSTR)csText);
	}

	if( numColumns )
		*numColumns = numFields;

	return(numFields);
}

int FreeTextToColumns( char **columnData, int numColumns )
{
	if( !columnData )
		return FALSE;

	for( int i = 0; i < numColumns; i++ )
	{
		if( columnData[i] )
			free( columnData[i] );
	}

	free(columnData);

	return TRUE;
}
