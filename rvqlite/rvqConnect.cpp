//#ifdef WIN32
#include "stdafx.h"
//#endif

#include "rvqLite1.hpp"

int equipObj::IsInUse()
{
	if( IsOwner(EQUIP_BOX) ||
		IsOwner(EQ_MPROC) ||
		IsOwner(ISOL_LIST) ||
		IsOwner(EQ_MHIST) ||
		IsOwner(EQ_DOCS) ||
		IsOwner(EQ_PARTS) ||
		IsOwner(EQ_PROGRESS) ||
		IsMember(COMMSYS_EQUIP) ||
		IsMember(ISOL_LIST) )
			return TRUE;
	return FALSE;
}
