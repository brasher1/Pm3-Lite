//#ifdef WIN32
#include "stdafx.h"
//#endif

#include "rvqLite1.hpp"


void equipObj::Delete()
{
	StoreObj::Delete();
}

void image_equip_boxObj::Delete()
{
	equipObj e;
	image_ptrObj i;

	if( IsMember(EQUIP_BOX) )
	{
		e.FromMember(this, EQUIP_BOX );
		DisConnect(e);
	}
	if( IsMember(IMAGE_BOX) )
	{
		i.FromMember(this, IMAGE_BOX);
		DisConnect(i);
	}
	StoreObj::Delete();
}

void image_ptrObj::Delete()
{
#if FALSE
	equip_image_boxObj ei;
	if( IsOwner(STATION_POINTS) )
	{
		for( p.GotoMember(*this, STATION_POINTS, FIRST); p.Okay(); 
			p.GotoMember(*this, STATION_POINTS, LAST) )
		{                         
			TRACE("Calling point Delete with FALSE\n");
			p.Delete();
		}
	}
#endif
	StoreObj::Delete();
}


void eq_docs_intObj::Delete()
{
	equipObj e;
	engdocObj d;

	if( IsMember(EQ_DOCS) )
	{
		e.FromMember(this, EQ_DOCS );
		DisConnect(e);
	}
	if( IsMember(DOCS_EQ) )
	{
		d.FromMember(this, DOCS_EQ);
		DisConnect(d);
	}
	StoreObj::Delete();
}

void eq_mproc_intObj::Delete()
{
	equipObj e;
	mprocObj p;

	if( IsMember(EQ_MPROC) )
	{
		e.FromMember(this, EQ_MPROC );
		DisConnect(e);
	}
	if( IsMember(MPROC_EQ) )
	{
		p.FromMember(this, MPROC_EQ);
		DisConnect(p);
	}
	StoreObj::Delete();
}

void commsysObj::Delete()
{
	equipObj e;
	if( IsOwner(COMMSYS_EQUIP) )
	{
		for( e.GotoMember(*this, COMMSYS_EQUIP, FIRST); e.Okay(); 
			e.GotoMember(*this, COMMSYS_EQUIP, LAST) )
		{                         
			TRACE("Calling Disconnect for equip from Commsys\n");
			e.DisConnect(*this);
		}
	}
	StoreObj::Delete();
}

void vendorObj::Delete()
{
	equipObj e;
	if( IsOwner(VENDOR_EQUIP) )
		return;
	if( IsOwner(VENDOR_PARTS) )
		return;
	StoreObj::Delete();
}

void engdocObj::Delete()
{
	eq_docs_intObj ei;
	parts_docs_intObj pi;

	if( IsOwner(DOCS_EQ) )
	{
		for( ei.GotoMember(*this, DOCS_EQ, FIRST); ei.Okay(); 
			ei.GotoMember(*this, DOCS_EQ, LAST) )
		{                         
			TRACE("Calling Disconnect for equip from Mhist_mproc\n");
			ei.DisConnect(*this);
			ei.Delete();
		}
	}
	if( IsOwner(DOCS_PARTS) )
	{
		for( pi.GotoMember(*this, DOCS_PARTS, FIRST); pi.Okay(); 
			pi.GotoMember(*this, DOCS_PARTS, LAST) )
		{                         
			TRACE("Calling Disconnect for equip from Mhist_mproc\n");
			pi.DisConnect(*this);
			pi.Delete();
		}
	}
	StoreObj::Delete();
}

void progressObj::Delete()
{
	equipObj e;

	if( IsMember(EQ_PROGRESS) )
	{
		e.FromMember(this, EQ_PROGRESS );
		DisConnect(e);
	}
	StoreObj::Delete();
}

void mprocObj::Delete()
{
	mhist_mproc_intObj ph;
	eq_mproc_intObj pe;

	if( IsOwner(MPROC_MHIST) )
	{
		for( ph.GotoMember(*this, MPROC_MHIST, FIRST); ph.Okay(); 
			ph.GotoMember(*this, MPROC_MHIST, LAST) )
		{                         
			TRACE("Calling Disconnect for Proc from Mhist_mproc\n");
			ph.DisConnect(*this);
			ph.Delete();
		}
	}
	if( IsOwner(MPROC_EQ) )
	{
		for( pe.GotoMember(*this, MPROC_EQ, FIRST); pe.Okay(); 
			pe.GotoMember(*this, MPROC_EQ, LAST) )
		{                         
			TRACE("Calling Disconnect for mproc from Mhist_mproc\n");
			pe.DisConnect(*this);
			pe.Delete();
		}
	}
	StoreObj::Delete();
}

void mhistObj::Delete()
{
	equipObj e;
	mhist_mproc_intObj m;

	if( IsMember(EQ_MHIST) )
	{
		e.FromMember(this, EQ_MHIST );
		DisConnect(e);
	}
	if( IsOwner(MHIST_MPROC) )
	{
		for( m.GotoMember(*this, MHIST_MPROC, FIRST); m.Okay(); 
			m.GotoMember(*this, MHIST_MPROC, LAST) )
		{                         
			TRACE("Calling Disconnect for equip from Mhist_mproc\n");
			m.DisConnect(*this);
			m.Delete();
		}
	}
	StoreObj::Delete();
}

void mhist_mproc_intObj::Delete()
{
	mhistObj h;
	mprocObj p;

	if( IsMember(MHIST_MPROC) )
	{
		h.FromMember(this, MHIST_MPROC );
		DisConnect(h);
	}
	if( IsMember(MPROC_MHIST) )
	{
		p.FromMember(this, MPROC_MHIST);
		DisConnect(p);
	}
	StoreObj::Delete();
}

void partsObj::Delete()
{
	vendorObj v;
	eq_parts_intObj ep;
	parts_docs_intObj pd;

	if( IsMember(VENDOR_PARTS) )
	{
		v.FromMember(this, VENDOR_PARTS );
		DisConnect(v);
	}
	if( IsOwner(PARTS_EQ) )
	{
		for( ep.GotoMember(*this, PARTS_EQ, FIRST); ep.Okay(); 
			ep.GotoMember(*this, PARTS_EQ, LAST) )
		{                         
			TRACE("Calling Disconnect for Proc from Mhist_mproc\n");
			ep.DisConnect(*this);
			ep.Delete();
		}
	}
	if( IsOwner(PARTS_DOCS) )
	{
		for( pd.GotoMember(*this, PARTS_DOCS, FIRST); pd.Okay(); 
			pd.GotoMember(*this, PARTS_DOCS, LAST) )
		{                         
			TRACE("Calling Disconnect for mproc from Mhist_mproc\n");
			pd.DisConnect(*this);
			pd.Delete();
		}
	}
	StoreObj::Delete();
}

void eq_parts_intObj::Delete()
{
	equipObj e;
	partsObj p;

	if( IsMember(EQ_PARTS) )
	{
		e.FromMember(this, EQ_PARTS );
		DisConnect(e);
	}
	if( IsMember(PARTS_EQ) )
	{
		p.FromMember(this, PARTS_EQ );
		DisConnect(p);
	}
	StoreObj::Delete();
}

void parts_docs_intObj::Delete()
{
	engdocObj d;
	partsObj p;

	if( IsMember(DOCS_PARTS) )
	{
		d.FromMember(this, DOCS_PARTS );
		DisConnect(d);
	}
	if( IsMember(PARTS_DOCS) )
	{
		p.FromMember(this, PARTS_DOCS );
		DisConnect(p);
	}
	StoreObj::Delete();
}



