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

/*FF*************************************************************************
*									    *
*   FILE								    *
*									    *
*   DESCRIPTION								    *
*									    *
*   NOTES								    *
*									    *
*   BUGS								    *
*									    *
*   REVISION HISTORY							    *
*	06/06/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/

//#define FUNC_DEBUG
//#include "photo.h"
#include "output.h"
#include "vista.h"
#include "dbtype.h"

#include "rdm_utils.h"

#include "internal.h"

#include "output.h"
#define KEYMARK 30000

short snNumRecs = 0;
short snNumKeys = 0;
short snNumSets = 0;

int *nRecLockTable = NULL;
int *nKeyLockTable = NULL;
int *nSetLockTable = NULL;

int c_lock2_i( char lock );
char i_lock2_c( int lock );

int CreateRecLockTable( void );
int DestroyRecLockTable( void );
int IncRecLock( int nRecType, DB_TASK *task, int nDbn );
int DecRecLock( int nRecType, DB_TASK *task, int nDbn );
int GetRecLockCount( int nRecType, DB_TASK *task, int nDbn );
int ClearLockTables( void );

#define FUNC "rdmGroupLock"
/*FF*************************************************************************

    NAME
	rdmGroupLock

    DESCRIPTION
	
    INPUTS

    OUTPUTS
    
    NOTES
    
    BUGS
    
    REVISION HISTORY
	01/27/94 - jwb - Version 1.0.0

****************************************************************************/
int rdmGroupLock( int nCount, LOCK_REQUEST *lrPacket, DB_TASK *task, int nDbn )
{
    
    LOCK_REQUEST *lr;
    int i, nLocks = 0;

    lr = (LOCK_REQUEST *)malloc( nCount * sizeof(LOCK_REQUEST) );
    
    for( i = 0; i < nCount; i++ )
    {
	/* if lock is an upgrade or record is not locked, need to pass
	 * on to d_lock
	 */
	char cCurrLock;		        /* lock currently on nRecType */
    
	if( dt_reclstat( lrPacket[i].item, &cCurrLock, task, nDbn ) != S_OKAY )
	{
	    ErrorOutput( "%s: Unable to get record's lock status.\n", FUNC );
	    ErrorOutput("    nRecType = %d, db_status = %d\n",
		    lrPacket[i].item, task->v.ptr->Db_status );
	    free( lr );
	    return( task->v.ptr->Db_status ); 	
	}
	
	if( (c_lock2_i(lrPacket[i].type) > c_lock2_i(cCurrLock)) ||
	    (GetRecLockCount( lrPacket[i].item, task, nDbn ) == 0) )
	{
	    memcpy( &(lr[nLocks]), &(lrPacket[i]), sizeof(LOCK_REQUEST) );
	    nLocks++;
	}
    }
    
    if( nLocks != 0 )
    {
 	if( dt_lock( nLocks, lr, task, nDbn ) != S_OKAY )
	{
	    ErrorOutput( "%s: d_lock failed\n", FUNC );
	    free( lr );
	    return( task->v.ptr->Db_status );
	}
    }
    else
	Output( 0, "%s: no locks to upgrade.\n", FUNC );

    for( i = 0; i < nCount; i++ )
    {
	if( lrPacket[i].item >= KEYMARK )
	{
	    DebugOutput( 2, "%s: Not handling key locks yet!\n", FUNC );
	}
	else if( lrPacket[i].item >= SETMARK )
	{
	    DebugOutput( 2, "%s: Not handling set locks yet!!\n", FUNC );	    
	}
	else if( lrPacket[i].item >= RECMARK )
	{
	    IncRecLock( lrPacket[i].item, task, nDbn );	
	}
	else
	{
	    DebugOutput( 2, "%s: Unknown lock request item\n", FUNC );
	}
    }
    
    free( lr );

    return( S_OKAY );
}
#undef FUNC

#define FUNC "rdmGroupFree"
/*FF*************************************************************************

    NAME
	rdmGroupFree

    DESCRIPTION
	
    INPUTS

    OUTPUTS
    
    NOTES
    
    BUGS
    
    REVISION HISTORY
	01/27/94 - jwb - Version 1.0.0

****************************************************************************/
int rdmGroupFree( int nCount, LOCK_REQUEST *lrPacket, DB_TASK *task, int nDbn )
{
    
    for( int i = 0; i < nCount; i++ )
    {
	if( lrPacket[i].item >= KEYMARK )
	{
	    DebugOutput( 2, "%s: Not handling key locks yet!\n", FUNC );
	}
	else if( lrPacket[i].item >= SETMARK )
	{
	    DebugOutput( 2, "%s: Not handling set locks yet!!\n", FUNC );	    
	}
	else if( lrPacket[i].item >= RECMARK )
	{
	    rdmRecFree( lrPacket[i].item, task, nDbn );	
	}
	else
	{
	    DebugOutput( 2, "%s: Unknown lock request item\n", FUNC );
	}
    }

    return( S_OKAY );
}
#undef FUNC

#define FUNC "rdmFreeAll"
/*FF*************************************************************************

    NAME
	rdmFreeAll

    DESCRIPTION

    INPUTS

    OUTPUTS
    
    NOTES
    
    BUGS
    
    REVISION HISTORY
	01/27/94 - jwb - Version 1.0.0

****************************************************************************/
int rdmFreeAll( DB_TASK *task, int nDbn )
{
    int i;
    
    for( i = 0; i < snNumRecs; i++ )
    {
	while( nRecLockTable[i] > 0 )
	    rdmRecFree( i + RECMARK, task, nDbn );
    }

    return( S_OKAY );
}
#undef FUNC

#define FUNC "rdmRecLock"
/*FF*************************************************************************

    NAME
	rdmRecLock

    DESCRIPTION
	Locks a record with the specified lock type.  If the record is
	already locked, the record will not be locked again.  If the
	requested lock type is an upgrade, the record's lock will be
	upgraded.
	
    INPUTS
	int nRecType - record type to be locked
	char *cpLockType - requested lock type
	int nDbn - database number of the database containing nRecType    

    OUTPUTS
	Returns S_OKAY if successful or the dbVista error code if
	unsuccessful.
    
    NOTES
    
    BUGS
    
    REVISION HISTORY
	01/18/94 - jwb - Version 1.0.0

****************************************************************************/
int rdmRecLock( int nRecType, char *cpLockType, DB_TASK *task, int nDbn )
{
    char cCurrLock;		        /* lock currently on nRecType */
    
    if( dt_reclstat( nRecType, &cCurrLock, task, nDbn ) != S_OKAY )
    {
	ErrorOutput( "%s: Unable to get record's lock status.\n", FUNC );
	ErrorOutput( "    nRecType = %d, db_status = %d\n",
		nRecType, task->v.ptr->Db_status );
	return( task->v.ptr->Db_status ); 	
    }
    
    /* The following order is used to determine if a lock is being
     * upgraded or not:
     *	    LOCK_FREE < LOCK_READ < LOCK_WRITE <
     *	    LOCK_EXCLUSIVE < LOCK_KEEP < LOCK_STATIC
     */
    if( c_lock2_i(*cpLockType) > c_lock2_i(cCurrLock) )
    {
	if( dt_reclock( nRecType, cpLockType, task, nDbn ) != S_OKAY )
	{
	    ErrorOutput( "%s: Unable to lock record.\n", FUNC );
	    ErrorOutput( "    nRecType = %d, db_status = %d\n",
		    nRecType, task->v.ptr->Db_status );
	    return( task->v.ptr->Db_status );
	}
    }

    IncRecLock( nRecType, task, nDbn );
    
    return( S_OKAY );
}
#undef FUNC

#define FUNC "rdmRecFree"
/*FF*************************************************************************

    NAME
	rdmRecFree

    DESCRIPTION
	
    INPUTS
	int nRecType - record type to be freed
	int nDbn - database number of the database containing nRecType    

    OUTPUTS
	Returns S_OKAY if successful or the dbVista error code if
	unsuccessful.
    
    NOTES
    
    BUGS
    
    REVISION HISTORY
	01/18/94 - jwb - Version 1.0.0

****************************************************************************/
int rdmRecFree( int nRecType, DB_TASK *task, int nDbn )
{
    char cCurrLock;

    /* If record is locked multiple times, just decrement the count
     * and return.
     */
    DecRecLock( nRecType, task, nDbn );
    if( GetRecLockCount( nRecType, task, nDbn ) != 0 )
	return( S_OKAY );
    if( (nRecLockTable != NULL) && (nRecLockTable[snNumRecs] == 0) )
	DestroyRecLockTable(); 
	
    /* Inside a transaction, you cannot legally free locks.  This implies
     * that either a transaction end or abort must be called.
     */
    if( IsTransActive(task) )
	return( S_OKAY );

    if( dt_reclstat( nRecType, &cCurrLock, task, nDbn ) != S_OKAY )
    {
	ErrorOutput( "%s: Unable to get record's lock status.\n", FUNC );
	ErrorOutput( "    nRecType = %d, db_status = %d\n",
		nRecType, task->v.ptr->Db_status );
	return( task->v.ptr->Db_status ); 	
    }

    if( cCurrLock == 'f' )
	return( S_OKAY );

    return( dt_recfree( nRecType, task, nDbn ) );    
}
#undef FUNC

/*FF*************************************************************************
    NAME
	IsTransActive
	
    DESCRIPTION

    INPUTS

    OUTPUTS

    CURRENCY CHANGES

    NOTES

    BUGS

    REVISION HISTORY
        01/17/94 - jwb - Version 1.0.0

****************************************************************************/
int IsTransActive(DB_TASK *task)
{
    return( task->v.ptr->Trans_id[0] ? TRUE : FALSE );
}

#define FUNC "rdmTransBegin"
/*FF*************************************************************************
    NAME
        rdmTransBegin

    DESCRIPTION
        Begins a transaction if one is not already started.

    INPUTS

    OUTPUTS

    CURRENCY CHANGES

    NOTES

    BUGS

    REVISION HISTORY
        02/04/93 - jwb - Version 1.0.0

****************************************************************************/
int rdmTransBegin( char *trans_name, DB_TASK *task )
{
    if( IsTransActive(task) )
	return( S_OKAY );
	
    return( dt_trbegin( trans_name, task ) );
}
#undef FUNC

#define FUNC "rdmTransEnd"
/*FF*************************************************************************
    NAME
        rdmTransEnd

    DESCRIPTION
        Ends a transaction if one is in progress.

    INPUTS

    OUTPUTS

    CURRENCY CHANGES

    NOTES

    BUGS

    REVISION HISTORY
        02/04/93 - jwb - Version 1.0.0

****************************************************************************/
int rdmTransEnd( DB_TASK *task )
{
    if( IsTransActive(task) )
    {
	if( dt_trend(task) != S_OKAY )
	    return( task->v.ptr->Db_status );
	ClearLockTables();
        return( S_OKAY );
    }
    
    return( S_OKAY );
}
#undef FUNC

#define FUNC "rdmTransAbort"
/*FF*************************************************************************
    NAME
        rdmTransAbort

    DESCRIPTION
        Aborts a transaction if one is in progrss.

    INPUTS

    OUTPUTS

    CURRENCY CHANGES

    NOTES

    BUGS

    REVISION HISTORY
        02/04/93 - jwb - Version 1.0.0

****************************************************************************/
int rdmTransAbort( DB_TASK *task )
{
    if( IsTransActive(task) )
    {
	if( dt_trabort(task) != S_OKAY )
	    return( task->v.ptr->Db_status );
	ClearLockTables();
	return( S_OKAY );
    }

    return( S_OKAY );
}
#undef FUNC

#define FUNC "c_lock2_i"
/*FF*************************************************************************
    NAME
        c_lock2_i

    DESCRIPTION
        Converts a character lock type to a BEK standard integer lock type.
        This conversion facilitates determining whether a lock is being
        upgraded or not.

    INPUTS
        char lock - the dbVista character lock type

    OUTPUTS
        Returns the BEK standard lock type.

    CURRENCY CHANGES
        None.

    NOTES

    BUGS

    REVISION HISTORY
        11/20/92 - jwb - Version 1.0.0

****************************************************************************/
int c_lock2_i( char lock )
{
    int rval;

    switch( lock )
    {
        case 'r':
            rval = LOCK_READ;
            break;

        case 'w':
            rval = LOCK_WRITE;
            break;

        case 'x':
            rval = LOCK_EXCLUSIVE;
            break;

        case 'k':
            rval = LOCK_KEEP;
            break;

        case 's':
            rval = LOCK_STATIC;
            break;

        case 'f':
            rval = LOCK_FREE;
            break;

        default:
            rval = LOCK_FREE;
            break;
    }
    return( rval );
}
#undef FUNC

#define FUNC "i_lock2_c"
/*FF*************************************************************************
    NAME
        i_lock2_c

    DESCRIPTION
        Converts a BEK standard lock type to a dbVista character based lock
        type.

    INPUTS
        int lock - BEK standard lock type

    OUTPUTS
        Returns the dbVista character based lock type.

    CURRENCY CHANGES
        None.

    NOTES

    BUGS

    REVISION HISTORY
        11/20/92 - jwb - Version 1.0.0

****************************************************************************/
char i_lock2_c( int lock )
{
    char rval;

    switch( lock )
    {
        case LOCK_READ:
            rval = 'r';
            break;

        case LOCK_WRITE:
            rval = 'w';
            break;

        case LOCK_EXCLUSIVE:
            rval = 'x';
            break;

        case LOCK_KEEP:
            rval = 'k';
            break;

        case LOCK_STATIC:
            rval = 's';
            break;

        case LOCK_FREE:
            rval = 'f';
            break;

        default:
            rval = '\0';
            break;
    }

    return( rval );
}
#undef FUNC

#define FUNC "CreateRecLockTable"
/*FF*************************************************************************
    NAME
        CreateRecLockTable

    DESCRIPTION

    INPUTS

    OUTPUTS

    CURRENCY CHANGES
        None.

    NOTES

    BUGS

    REVISION HISTORY
        01/27/93 - jwb - Version 1.0.0

****************************************************************************/
int CreateRecLockTable( DB_TASK *task )
{
    dt_internals( task, TOPIC_GLOBALS, ID_SIZE_RT, 0, &snNumRecs, sizeof(snNumRecs) );

    nRecLockTable = (int *)malloc( (snNumRecs + 1) * sizeof(int) );
    memset( nRecLockTable, 0, (snNumRecs + 1) * sizeof(int) );

    return( S_OKAY );
}
#undef FUNC

#define FUNC "DestroyRecLockTable"
/*FF*************************************************************************
    NAME
        DestroyRecLockTable

    DESCRIPTION

    INPUTS

    OUTPUTS

    CURRENCY CHANGES
        None.

    NOTES

    BUGS

    REVISION HISTORY
        01/27/93 - jwb - Version 1.0.0

****************************************************************************/
int DestroyRecLockTable( void )
{
    if( nRecLockTable != NULL )
    {
	free( nRecLockTable );
	nRecLockTable = NULL;
	snNumRecs = 0;
    }
    
    return( S_OKAY );
}
#undef FUNC

#define FUNC "IncRecLock"
/*FF*************************************************************************
    NAME
        IncRecLock

    DESCRIPTION

    INPUTS

    OUTPUTS

    CURRENCY CHANGES
        None.

    NOTES

    BUGS

    REVISION HISTORY
        01/27/93 - jwb - Version 1.0.0

****************************************************************************/
int IncRecLock( int nRecType, DB_TASK *task, int nDbn )
{
    if( nRecLockTable == NULL )
	CreateRecLockTable(task);
    
    nRecLockTable[nRecType - RECMARK]++;
    nRecLockTable[snNumRecs]++;

    return( S_OKAY );
}
#undef FUNC

#define FUNC "DecRecLock"
/*FF*************************************************************************
    NAME
        DecRecLock

    DESCRIPTION

    INPUTS

    OUTPUTS

    CURRENCY CHANGES
        None.

    NOTES

    BUGS

    REVISION HISTORY
        01/27/93 - jwb - Version 1.0.0

****************************************************************************/
int DecRecLock( int nRecType, DB_TASK *task, int nDbn )
{
    if( nRecLockTable == NULL )
    {
	DebugOutput( 2, "%s: nRecLockTable is NULL\n", FUNC );
	return( !S_OKAY );
    }
    
    if( nRecLockTable[nRecType - RECMARK] > 0 )
    {
	nRecLockTable[nRecType - RECMARK]--;
	nRecLockTable[snNumRecs]--;
    }

    return( S_OKAY );
}
#undef FUNC

#define FUNC "GetRecLockCount"
/*FF*************************************************************************
    NAME
        GetRecLockCount

    DESCRIPTION

    INPUTS

    OUTPUTS

    CURRENCY CHANGES
        None.

    NOTES

    BUGS

    REVISION HISTORY
        01/27/93 - jwb - Version 1.0.0

****************************************************************************/
int GetRecLockCount( int nRecType, DB_TASK *task, int nDbn )
{
    if( nRecLockTable == NULL )
    {
	DebugOutput( 2, "%s: nRecLockTable is NULL\n", FUNC );
	return( 0 );
    }
    
    return( nRecLockTable[nRecType - RECMARK] );
}
#undef FUNC

#define FUNC "ClearLockTables"
/*FF*************************************************************************
    NAME
        ClearLockTables

    DESCRIPTION

    INPUTS

    OUTPUTS

    CURRENCY CHANGES
        None.

    NOTES

    BUGS

    REVISION HISTORY
        01/27/93 - jwb - Version 1.0.0

****************************************************************************/
int ClearLockTables( void )
{
    
    if( nRecLockTable != NULL )
	memset( nRecLockTable, 0, (snNumRecs + 1)*sizeof(int) );

    if( nKeyLockTable != NULL )
	memset( nKeyLockTable, 0, (snNumKeys + 1)*sizeof(int) );

    if( nSetLockTable != NULL )
	memset( nSetLockTable, 0, (snNumSets + 1)*sizeof(int) );
    
    return( S_OKAY );
}
#undef FUNC

/*FF*************************************************************************
    NAME
        rdmSetFree

    DESCRIPTION
        Frees a set lock on the specified set type.  If the lock is already
        free no attempt is made to free the lock and S_OKAY is returned.
        If a transaction is in progress, the lock is not freed and
        S_OKAY is returned.  If the lock is successfully freed, S_OKAY is
        returned.  Otherwise, the dbVista error code is returned.

    INPUTS
        DB_OBJECT *dbo
            set_id - set id of the set whose lock is to be freed
            dbn    - database number of the database containing set_id
            task   -

    OUTPUTS
        Returns S_OKAY if successful or the dbVista error code if
        unsuccessful.

    CURRENCY CHANGES
        None.

    NOTES

    BUGS

    REVISION HISTORY
	12/08/94 - jwb - Version 1.1.0
			 Moved to SGI
        11/18/92 - jwb - Version 1.0.0

****************************************************************************/
#define FUNC "rdmSetFree()"
int rdmSetFree( int setID, DB_TASK *task, int dbn )
{
    char c_lock;

    dt_setlstat( setID, &c_lock, task, dbn );
    if( task->v.ptr->Db_status != S_OKAY )
    {
        ErrorOutput( "%s: Unable to get set's lock status.", FUNC );
        return( task->v.ptr->Db_status );
    }

    if( c_lock == 'f' )
        return( S_OKAY );

    if( task->v.ptr->Trans_id[0] )
        return( S_OKAY );

    dt_setfree( setID, task, dbn );
    return( task->v.ptr->Db_status );
}
#undef FUNC

/*FF*************************************************************************
    NAME
        set_lock

    DESCRIPTION
        Locks a set with the specified lock type.  If the set is already
        locked, the set will not be locked again.  If the requested lock
        type is an upgrade, the set's lock will be upgraded.

    INPUTS
        DB_OBJECT *dbo
            set_id - set id of the set whose lock is to be changed
            dbn    - database number of the database containing set_id
            task   -

        lock_type - identifier for the lock requested

    OUTPUTS
        Returns S_OKAY if successful or the dbVista error code if
        unsuccessful.  S_BADTYPE is returned if an invalid lock_type is
        encountered.

    CURRENCY CHANGES
        None.

    NOTES
        Lock ordering is determined by the values defined for the LOCK_
        constants.

    BUGS

    REVISION HISTORY
        11/18/92 - jwb - Version 1.0.0

****************************************************************************/
#define FUNC "rdmSetLock()"
int rdmSetLock( int setID, char *cpLockType, DB_TASK *task, int dbn )
{
    char cCurrLock;
    if( dt_setlstat( setID, &cCurrLock, task, dbn ) != S_OKAY )
    {
	ErrorOutput( "%s: Unable to get set's lock status.\n", FUNC );
	ErrorOutput( "    setID = %d, db_status = %d\n", setID, task->v.ptr->Db_status );
	return( task->v.ptr->Db_status );
    }

    /* The following order is used to determine if a lock is being
     * upgraded or not:
     *	    LOCK_FREE < LOCK_READ < LOCK_WRITE <
     *	    LOCK_EXCLUSIVE < LOCK_KEEP < LOCK_STATIC
     */
    if( c_lock2_i(*cpLockType) > c_lock2_i(cCurrLock) )
    {
	if( dt_setlock( setID, cpLockType, task, dbn ) != S_OKAY )
	{
	    ErrorOutput( "%s: Unable to lock set.\n", FUNC );
	    ErrorOutput( "    setID = %d, db_status = %d\n", setID, task->v.ptr->Db_status );
	    return( task->v.ptr->Db_status );
	}
    }
    
    return( S_OKAY );
}
#undef FUNC

#define FUNC "rdmRecWrite"
/*FF*************************************************************************
    NAME
        rdmRecWrite

    DESCRIPTION

    INPUTS

    OUTPUTS

    CURRENCY CHANGES

    NOTES

    BUGS

    REVISION HISTORY
        10/20/95 - jwb - Version 1.0.0

****************************************************************************/
int rdmRecWrite( void *value, DB_TASK *task, int dbn, char *file, int lineno )
{
    DB_ADDR dba;
    
    dt_crget( &dba, task, dbn );
//    WriteLogEntry( "%s:%d - dba = %d:%d  d_recwrite( %p, %d )\n", file, lineno, GET_FNO(dba), GET_RNO(dba), value, dbn );
    return( dt_recwrite( value, task, dbn ) );
}
#undef FUNC

#define FUNC "rdmFillNew"
/*FF*************************************************************************
    NAME
        rdmFillNew

    DESCRIPTION

    INPUTS

    OUTPUTS

    CURRENCY CHANGES

    NOTES

    BUGS

    REVISION HISTORY
        10/20/95 - jwb - Version 1.0.0

****************************************************************************/
int rdmFillNew( int rectype, void *value,DB_TASK *task,int dbn, char *file, int lineno )
{
//    WriteLogEntry( "%s:%d - d_fillnew( %d, %p, %d )\n", file, lineno, rectype, value, dbn );
    return( dt_fillnew( rectype, value, task, dbn ) );
}
#undef FUNC
