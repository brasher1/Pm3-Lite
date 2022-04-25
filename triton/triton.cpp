// triton.cpp
// triton database code
//
//

#include "stdafx.h"

#include "triton.hpp"
#include "tritont.hpp"

DB_INIT(triton);

void TransformObj::Prune( T_F deleteChildren, T_F cleanOrphanedParents )
{
	// First, if we still own anything and we aren't deleting child nodes, we should do nothing.
	if( !deleteChildren && IsOwner(TRANSFORMS) )
		return;

	// And, if we are truly pruning (deleting all member records), we need to iterate on all our sets that we own and
	// delete the child records.
	if( deleteChildren )
	{
		ClassesObj childClass;
		for( childClass.GotoMember(*this,TRANSFORMS,FIRST); childClass.Okay(); childClass.GotoMember(*this,TRANSFORMS,FIRST) )
		{
			childClass.Prune(True, False);
		}
	}

	Delete();
}

void ClassesObj::Prune( T_F deleteChildren, T_F cleanOrphanedParents )
{
	// First, if we still own anything and we aren't deleting child nodes, we should do nothing.
	if( !deleteChildren && (IsOwner(CLASSES_CLASSES) || IsOwner(ELEMENT_CLASSES)) )
		return;

	// Next, we need to disconnect ourselves from all sets in which we are a member (and optionally,
	// clean up the parent nodes that exist solely as our owner.
	if( IsMember(GROUPS_LIST) )
	{
		GroupsObj go;
		go.FromMember( this, GROUPS_LIST);
		if( cleanOrphanedParents )
			go.Prune(False, True);
		else
			DisConnect(go, GROUPS_LIST);
	}

	if( IsMember(PROPERTIES_SET) )
	{
		PropertiesObj po;
		po.FromMember( this, PROPERTIES_SET);
		if( cleanOrphanedParents )
			po.Prune(False, True);
		else
			DisConnect(po, PROPERTIES_SET);
	}

	if( IsMember(TRANSFORMS) )
	{
		TransformObj to;
		to.FromMember( this, TRANSFORMS );
		if( cleanOrphanedParents )
			to.Prune(False, True);
		else
			DisConnect(to, TRANSFORMS);
	}

	if( IsMember(CLASSES_CLASSES) )
	{
		ClassesObj co;
		co.FromMember(this, CLASSES_CLASSES);
		if( cleanOrphanedParents )
			co.Prune(False, True);
		else
			DisConnect(co, CLASSES_CLASSES);
	}

	if( IsMember(WORLD_CLASSES) )
	{
		SystemObj so;
		so.FromMember( this, WORLD_CLASSES );
		DisConnect(so, WORLD_CLASSES);
	}

	// And finally, if we are truly pruning (deleting all member records), we need to iterate on all our sets that we own and
	// delete the child records.
	if( deleteChildren )
	{
		ClassesObj childClass;
		for( childClass.GotoMember(*this,CLASSES_CLASSES,FIRST); childClass.Okay(); childClass.GotoMember(*this,CLASSES_CLASSES,FIRST) )
		{
			childClass.Prune(True, False);
		}

		AllElements childElement;
		for( childElement.GotoMember(*this, ELEMENT_CLASSES,FIRST); childElement.Okay(); childElement.GotoMember(*this,ELEMENT_CLASSES,FIRST) )
		{
			childElement.Prune(True, False);
		}
	}

	Delete();
}

void GroupsObj::Prune( T_F deleteChildren, T_F cleanOrphanedParents )
{
	// First, if we still own anything and we aren't deleting child nodes, we should do nothing.
	if( !deleteChildren && IsOwner(GROUPS_LIST) )
		return;

	// And, if we are truly pruning (deleting all member records), we need to iterate on all our sets that we own and
	// delete the child records.
	if( deleteChildren )
	{
		ClassesObj childClass;
		for( childClass.GotoMember(*this,GROUPS_LIST,FIRST); childClass.Okay(); childClass.GotoMember(*this,GROUPS_LIST,FIRST) )
		{
			childClass.Prune(True, False);
		}
	}

	Delete();
}

void PropertiesObj::Prune( T_F deleteChildren, T_F cleanOrphanedParents )
{
	// First, if we still own anything and we aren't deleting child nodes, we should do nothing.
	if( !deleteChildren && IsOwner(PROPERTIES_SET) )
		return;

	// And, if we are truly pruning (deleting all member records), we need to iterate on all our sets that we own and
	// delete the child records.
	if( deleteChildren )
	{
		ClassesObj childClass;
		for( childClass.GotoMember(*this,PROPERTIES_SET,FIRST); childClass.Okay(); childClass.GotoMember(*this,PROPERTIES_SET,FIRST) )
		{
			childClass.Prune(True, False);
		}
	}

	Delete();
}

void AllElements::Prune( T_F deleteChildren, T_F cleanOrphanedParents )
{
	// First, if we still own anything and we aren't deleting child nodes, we should do nothing.
	if( !deleteChildren && IsOwner(ELEMENT_CLASSES) )
		return;

	// Next, we need to disconnect ourselves from all sets in which we are a member (and optionally,
	// clean up the parent nodes that exist solely as our owner.
	if( IsMember(ELEMENT_CLASSES) )
	{
		ClassesObj co;
		co.FromMember(this, ELEMENT_CLASSES);
		if( cleanOrphanedParents )
			co.Prune(False, True);
		else
			DisConnect(co, ELEMENT_CLASSES);
	}

	// And finally, if we are truly pruning (deleting all member records), we need to iterate on all our sets that we own and
	// delete the child records.
	if( deleteChildren )
	{
/***********************************************************
** NEED TO HANDLE COMPLEX ELEMENTS HERE!!!!!!!!!!!!!!!!!! **
***********************************************************/
#if FALSE
		ClassesObj childClass;
		for( childClass.GotoMember(*this,CLASSES_CLASSES,FIRST); childClass.Okay(); childClass.GotoMember(*this,CLASSES_CLASSES,FIRST) )
		{
			childClass.Prune(True, False);
		}

		AllElements childElement;
		for( childElement.GotoMember(*this, ELEMENT_CLASSES,FIRST); childElement.Okay(); childElement.GotoMember(*this,ELEMENT_CLASSES,FIRST) )
		{
			childElement.Prune(True, False);
		}
#endif
	}

	Delete();
}
