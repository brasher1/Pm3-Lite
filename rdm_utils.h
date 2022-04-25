/*COPYRIGHT******************************************************************
*									    *
*   Copyright (c) 1994 AsBuilt Data, Inc.  These coded instructions,	    *
*   statements, computer programs and source code contain unpublished	    *
*   trade secrets of AsBuilt Data, Inc. and are protected by Federal	    *
*   Copyright Law.  They may not be disclosed or copied, duplicated,	    *
*   distributed, demonstrated or licensed in any form, in whole or in	    *
*   part, without the prior written consent of AsBuilt Data, Inc.  Any	    *
*   permitted use shall be considered as a personal license to the	    *
*   authorized recipient only.  Any such permitted use is provided on a	    *
*   "as is" basis without any warranty of any kind, express or implied,	    *
*   and specifically without any warranty of merchantibility or fitness	    *
*   for a particular purpose.  In no event shall AsBuilt Data, Inc. or	    *
*   its affiliates be liable for any special, incidental, direct or	    *
*   indirect damages of any kind, or for any liabilities in excess of the   *
*   royalty fee payments actually made by Licensee hereunder regardless of  *
*   legal theory or negligence of AsBuilt Data, Inc.			    *
*									    *
******************************************************************COPYRIGHT*/
#ifndef ___RDM_UTILS_H___
#define ___RDM_UTILS_H___

#define LOCK_FREE	0x0001
#define LOCK_READ	0x0002
#define LOCK_WRITE	0x0004
#define LOCK_EXCLUSIVE	0x0008
#define LOCK_KEEP	0x0010
#define LOCK_STATIC	0x0020

int IsTransActive(DB_TASK *task);
int rdmTransBegin( char *trans_name, DB_TASK *task );
int rdmTransEnd( DB_TASK *task);
int rdmTransAbort( DB_TASK *task );
int EXTERNAL_FIXED dt_internals(P1(DB_TASK DB_FAR *) Pi(int) Pi(int) Pi(int) Pi(DB_VOID DB_FAR *) Pi(unsigned));
int rdmRecLock( int nRecType, char *cpLockType, DB_TASK *task, int nDbn );
int rdmRecFree( int nRecType, DB_TASK *task, int nDbn );
int rdmGroupLock( int nCount, LOCK_REQUEST *lrPacket, DB_TASK *task, int nDbn );
int rdmGroupFree( int nCount, LOCK_REQUEST *lrPacket, DB_TASK *task, int nDbn );
int rdmFreeAll( DB_TASK *task, int nDbn );
int rdmSetFree( int setID, DB_TASK *task, int dbn );
int rdmSetLock( int setID, char *lockType, DB_TASK *task, int dbn );
int rdmRecWrite( void *value, int dbn, char *file, int lineno );
int rdmFillNew( int rectype, void *value, DB_TASK *task, int dbn, char *file, int lineno );
#endif	// ___RDM_UTILS_H___
