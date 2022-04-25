#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "strutils.h"

#define YEAR_ADJ 1900

long tm_to_long( struct tm *t )
{
    return( ((long)((long)t->tm_year+YEAR_ADJ) * 10000L) +
	    (long)((long)t->tm_mon * 100L) +
	    (long)t->tm_mday );
}

#define FUNC "eaDateStrToLong()"
long eaDateStrToLong( const char *dateString )
{
    char **tokens;
    int nNumTokens;
    struct tm t;
    long lRetVal = 0L;
    
    memset( &t, 0, sizeof(struct tm) );
    
    getTokens( dateString, &tokens, &nNumTokens, '/' );
    if( nNumTokens == 3 )
    {
	t.tm_mon = atoi( tokens[0] );
	t.tm_mday = atoi( tokens[1] );
	t.tm_year = atoi( tokens[2] );
	lRetVal = tm_to_long( &t );
    }

    return( lRetVal );
}
#undef FUNC

void long_to_tm( const long d1, struct tm *t)
{
    memset( t, 0, sizeof(*t));
    t->tm_year = (int)(d1 / 10000L);
    if( t->tm_year > YEAR_ADJ - 1 )
	t->tm_year -= YEAR_ADJ;
    t->tm_mon = (int)((d1 % 10000L) / 100L);
    t->tm_mday = (int)(d1 % 100L);
}

char *eaDateLongToStr( const long dateLong, char *dateString )
{
    struct tm t;
    
    long_to_tm( dateLong, &t );
    if( t.tm_year == 0 || t.tm_mon == 0 || t.tm_mday == 0 )
	strcpy( dateString, "" );
    else
	sprintf( dateString, "%02hd/%02hd/%02hd", t.tm_mon, t.tm_mday, t.tm_year );
    return( dateString );
}

int isStrInt( char *str )
{
    if( str == NULL )
	return( FALSE );
	
    while( *str != '\0' && isdigit(*str) )
	str++;

    if( *str == '\0' )
	return( TRUE );
    else
	return( FALSE );
}

int isStrFloat( char *str )
{
    if( str == NULL )
	return( FALSE );
	
    while( *str != '\0' && (isdigit(*str) || ispunct(*str)) )
	str++;

    if( *str == '\0' )
	return( TRUE );
    else
	return( FALSE );
}

void strltrim( char *str )
{
    char *tmp;
    
    tmp = str;
    
    while( isspace(*tmp) )
	tmp++;
    
    while( tmp != NULL && *tmp != '\0' )
	*str++ = *tmp++;
    
    *str = '\0';
}

void strrtrim( char *str )
{
    char *tmp = str;

    while( tmp != NULL && *tmp != '\0' )
	tmp++;
    tmp--;
    
    while( isspace(*tmp) )
	*tmp-- = '\0';
}

void strpack( char *str )
{
    char *tmp = str;
    
    while( str != NULL && *str != '\0' )
    {
	if( !isspace(*str) )
	    *tmp++ = *str++;
	else
	    str++;
    }
    
    *tmp = '\0';
}

void stripchar( char *str, char c )
{
    char *tmp = str;
    
    while( str != NULL && *str != '\0' )
    {
	if( *str != c )
	    *tmp++ = *str++;
	else
	    str++;
    }
    
    *tmp = '\0';
}

void getTokens( const char *input, char ***tokens, int *numtokens, char delim )
{
    char *copyOfInputStr = strdup(input);
    
    *numtokens = 0;
    *tokens = NULL;
    char *temp = copyOfInputStr;
    
    while( temp != NULL /*&& *temp != '\0'*/ )
    {
	char *temp2 = strchr( temp, delim );
	if( temp2 != NULL )
	{
	    *temp2 = '\0';
	    *tokens = (char **)realloc( *tokens, ++(*numtokens)*sizeof(char *) );
	    (*tokens)[*numtokens-1] = strdup(temp);
	    
	    temp = temp2 + 1;
	}
	else
	{
	    *tokens = (char **)realloc( *tokens, ++(*numtokens)*sizeof(char *) );
	    (*tokens)[*numtokens-1] = strdup(temp);
	    temp = NULL;
	}
	strltrim((*tokens)[*numtokens-1]);
	strrtrim((*tokens)[*numtokens-1]);
//	strpack((*tokens)[*numtokens-1]);
    }
    
    free( copyOfInputStr );
}

void freeTokens( char **tokens, int nNumTokens )
{
    for( int i = 0; i < nNumTokens; i++ )
	free( tokens[i] );
    free( tokens );
}
