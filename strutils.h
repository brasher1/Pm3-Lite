#ifndef ___strutils_h___
#define ___strutils_h___

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

#include <time.h>

void strltrim( char *str );
void strrtrim( char *str );
void strpack( char *str );
void stripchar( char *str, char c );

int isStrInt( char *str );
int isStrFloat( char *str );

void getTokens( const char *input, char ***tokens, int *numtokens, char delim = '~' );
void freeTokens( char **tokens, int nNumTokens );

long tm_to_long( struct tm *t );
long eaDateStrToLong( const char *dateString );
void long_to_tm( const long d1, struct tm *t);
char *eaDateLongToStr( const long dateLong, char *dateString );

#endif	//#ifndef ___strutils_h___
