#ifndef __TextToColumns_h__
#define __TextToColumns_h__

#include <stdio.h>
#include <afx.h>

int TextToColumns( char *text, char ***columnData, int *numColumns = NULL, char *columnDelims = " \t", BOOL multDelimsFlag = TRUE );
int FreeTextToColumns( char **columnData, int numColumns );

#endif	// #ifndef __TextToColumns_h__
