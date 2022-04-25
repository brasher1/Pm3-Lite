#ifndef ___output_h___
#define ___output_h___

#include <stdarg.h>

#ifdef WIN32
#include "stdafx.h"
//#define printf		TRACE
//#define fprintf(x)			
#endif

#ifdef PHOTOG_DLL
#define PHOTOG_PORTING		//__declspec(dllexport)
#else
#define PHOTOG_PORTING		//__declspec(dllimport)
#endif

#ifndef EXTERN
#define EXTERN extern
#endif	// #ifndef EXTERN

EXTERN int outputLevel;			/* always have most basic output */
EXTERN int debugOutputLevel;	        /* always start with debug off */

int vOutput( int olevel, const char *format, va_list args );
int Output( int olevel, const char *format, ... );

int vDebugOutput( int olevel, const char *format, va_list args );
int DebugOutput( int olevel, const char *format, ... );

int vErrorOutput( int olevel, const char *format, va_list args );
PHOTOG_PORTING int ErrorOutput( const char *format, ... );

#endif	// #ifndef ___output_h___
