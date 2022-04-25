#include "HierElem.h"

ID_NAME(HierElem);

HierElem::HierElem()
{
    initVars();
}

HierElem::~HierElem()
{
  // Disconnect from owner if have one
    if( elemOwner != NULL )
    {
	POSITION pos = elemOwner->getMemberList().Find( this );
	if( pos != NULL )
	    elemOwner->getMemberList().RemoveAt(pos);
    }

  // Delete all children
    while( getMemberList().GetCount() > 0 )
	delete getMemberList().RemoveTail();
}

void HierElem::initVars()
{
    elemOwner = NULL;
}

void HierElem::addMember( HierElem *newMember )
{
    elemMembers.AddTail( newMember );
    newMember->setOwner( this );
}
