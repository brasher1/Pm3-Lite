/////////////////////////////////////////////////////////////
//
// Source file for EquipDocsListDialogDerived
//
//    This file generated by Builder Xcessory 3.1.
//
/////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stream.h>

#include <Xm/List.h>
#include <Xm/DialogS.h>

#include "msys.h"
#include "EquipDocsListDialogDerived.h"

#include <grlock.hpp>
#include <transact.hpp>

EquipDocsListDialogDerived::EquipDocsListDialogDerived(const char *name, Widget parent) : 
                   EquipDocsListDialog(name, parent) 
{
    initDialog();
}


EquipDocsListDialogDerived::EquipDocsListDialogDerived(const char *name) : 
                   EquipDocsListDialog(name) 
{ 
    initDialog();
}


EquipDocsListDialogDerived::~EquipDocsListDialogDerived()
{
  // Empty
}


const char *const EquipDocsListDialogDerived::className() // classname
{
    return ("EquipDocsListDialogDerived");
}

void EquipDocsListDialogDerived::equipDocsAddCB(Widget, XtPointer, XtPointer) 
{
    engDocsListDialog->manage();
    engDocsListDialog->updateList( theCurrEquip );
}

void EquipDocsListDialogDerived::equipDocsDismissCB(Widget, XtPointer, XtPointer) 
{
    engDocsListDialog->unmanage();
    unmanage();
}

#define FUNC "equipDocsRemoveCB()"
void EquipDocsListDialogDerived::equipDocsRemoveCB(Widget, XtPointer, XtPointer) 
{
    XmString *xmsp;
    int *npPositions;
    int nNumSelected;
    int nargs;
    Arg args[32];
    char *pszDoc;
    eq_docs_intObj theIntersect;
    engdocObj theEngDoc;
    equipObj theTempEquip;
    
    TransAction theTransAction( FUNC );
    
    if( theTransAction.GetStatus() != TrActive )
    {
	cout << "Unable to begin transaction." << endl;
	return;
    }
    
    GroupLock theLocks;
    theLocks.AddWrite( theIntersect );
    theLocks.AddWrite( theEngDoc );
    theLocks.AddWrite( theTempEquip );
    if( theLocks.Lock() != S_OKAY )
    {
	cout << "Unable to lock records." << endl;
	theTransAction.End();
	return;
    }

    nargs = 0;
    XtSetArg( args[nargs], XmNitems, &xmsp ); nargs++;
    XtGetValues( _equipDocsList, args, nargs );

    XmListGetSelectedPos( _equipDocsList, &npPositions, &nNumSelected );

    for( int i = 0; i < nNumSelected; i++ )
    {
	XmStringGetLtoR( xmsp[npPositions[i]-1], 
		XmSTRING_DEFAULT_CHARSET, &pszDoc );
	if( pszDoc != NULL )
	{
	    strcpy( theEngDoc.docno, pszDoc );
	    theEngDoc.Find( KeyObj(ENGDOC_DOCNO, &(theEngDoc.docno) ) );
	    if( theEngDoc.Okay() )
	    {
		for( theIntersect.GotoMember( theEngDoc, DOCS_EQ, FIRST );
			theIntersect.Okay();
			theIntersect.GotoMember( theEngDoc, DOCS_EQ, NEXT ) )
		{
		    theTempEquip.FromMember( &theIntersect, EQ_DOCS );
		    if( theTempEquip.GetDba() == theCurrEquip.GetDba() )
		    {
			theIntersect.DisConnect( theTempEquip );
			theIntersect.DisConnect( theEngDoc );
			if( !theIntersect.Okay() )
			{
			    XtFree( pszDoc );
			    theTransAction.Abort();
			    cout << "Unable to disconnect intersect record for " << theEngDoc.docno << endl;
			    return;
			}

			theIntersect.Delete();
		    }
		}
	    }
	    
	    XtFree( pszDoc ); 
	}
    }
    
    theTransAction.End();
    
    updateList( theCurrEquip );
}
#undef FUNC

#define FUNC "updateList()"
void EquipDocsListDialogDerived::updateList( equipObj theEquip )
{
    eq_docs_intObj theIntersect;
    engdocObj theEngDoc;
    XmString *xms = NULL;
    int n = 0;

    theCurrEquip = theEquip;
    
    if( !XtIsManaged(_w) )
	return;
    
    XmListDeleteAllItems( _equipDocsList );
    
    if( theCurrEquip.GetDba() == 0 )
	return;
    
    GroupLock theLocks;
    theLocks.AddRead( theCurrEquip );
    theLocks.AddRead( theEngDoc );
    theLocks.AddRead( theIntersect );

    if( theLocks.Lock() != S_OKAY )
    {
	cout << "Unable to lock records." << endl;
	return;
    }

    for( theIntersect.GotoMember( theCurrEquip, EQ_DOCS, FIRST );
	    theIntersect.Okay();
	    theIntersect.GotoMember( theCurrEquip, EQ_DOCS, NEXT ) )
    {
	theEngDoc.FromMember( &theIntersect, DOCS_EQ );
	if( theEngDoc.Okay() )
	{
	    xms = (XmString *)realloc( xms, ++n*sizeof(XmString) );
	    xms[n-1] = XmStringCreateSimple( theEngDoc.docno );
	}
    }

    theLocks.Unlock();        

    XmListAddItems( _equipDocsList, xms, n, 0 );
    
    for( int i = 0; i < n; i++ )
	XmStringFree( xms[i] );
    free( xms );
    
    engDocsListDialog->updateList( theCurrEquip );
}
#undef FUNC

void EquipDocsListDialogDerived::initDialog( void )
{
    int count;
    Arg args[32];

    count = 0;
    XtSetArg ( args[count], XmNx, 765 ); count++;
    XtSetArg ( args[count], XmNy, 486 ); count++;
    XtSetArg ( args[count], XmNwidth, 326 ); count++;
    XtSetArg ( args[count], XmNheight, 495 ); count++;
    engDocsListDialogShell = XtCreatePopupShell ( "engDocsListDialogShell",
                                        xmDialogShellWidgetClass,
                                        shell, args, count );

    engDocsListDialog =  new EngDocsListDialogDerived ( "engDocsListDialog" );
    engDocsListDialog->create( engDocsListDialogShell );
}
