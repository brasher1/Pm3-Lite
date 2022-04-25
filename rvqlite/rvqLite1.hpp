/*FF*************************************************************************
*									    *
*   Copyright (c) 1994 BE&K, Inc.  These coded instructions,                *
*   statements, computer programs and source code contain unpublished	    *
*   trade secrets of BE&K, Inc. and are protected by Federal                *
*   Copyright Law.  They may not be disclosed or copied, duplicated,	    *
*   distributed, demonstrated or licensed in any form, in whole or in	    *
*   part, without the prior written consent of BE&K, Inc.  Any              *
*   permitted use shall be considered as a personal license to the	    *
*   authorized recipient only.  Any such permitted use is provided on a	    *
*   "as is" basis without any warranty of any kind, express or implied,	    *
*   and specifically without any warranty of merchantibility or fitness	    *
*   for a particular purpose.  In no event shall BE&K, Inc. be              *
*   liable for any special, incidental, direct or indirect damages of	    *
*   any kind, regardless of legal theory or negligence of		    *
*   BE&K, Inc.                                                              *
*									    *
****************************************************************************/

/*FF*************************************************************************
*									    *
*   FILE								    *
*	rvqLite.h							    *
*									    *
*   DESCRIPTION								    *
*									    *
*   NOTES								    *
*									    *
*   BUGS								    *
*									    *
*   REVISION HISTORY							    *
*	08/03/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/

#ifndef ___rvqLite_h___
#define ___rvqLite_h___

#define VISTACALLS
#include <storedb.hpp>
#include <storeobj.hpp>
#include <keyobj.hpp>

typedef struct RECORD_ENTRY_S
{
   DB_SHORT rt_file;                   /* file table entry of file
                                        * containing record */
   DB_SHORT rt_len;                    /* total length of record */
   DB_SHORT rt_data;                   /* offset to start of data in record */
   DB_SHORT rt_fields;                 /* first field def in field_table */
   DB_SHORT rt_fdtot;                  /* total number of fields in record */
   DB_SHORT rt_flags;                  /* 0x0001 is set if record is
                                        * timestamped 0x0002 is set if
                                        * record is static */
} RECORD_ENTRY;

#include "rvqLite.h"

#ifndef REC_TYPE
#define REC_TYPE int    /* Required to support RDM 3.21A */
#endif /* REC_TYPE */

#define FIXTotalSize(x) \
long TotalSize() { return( sizeof(x) ); } \
void MakeEmpty() { memset( RecData(), 0, (unsigned int)TotalSize() );} \
Pvoid RecData() { return StoreObj::RecData(); }

/* database rvq class declarations */

class OM_EXPORT rvqLiteDb : public StoreDb
{
public:
   rvqLiteDb() : StoreDb(PDB_LOCATOR) {}
   DEFINE_DB_LOCATOR;
};

class rvqLiteTask : public StoreTask
{
public:
    rvqLiteDb *thervqLiteDb;
public:
    rvqLiteTask();
	rvqLiteTask(char *user);
    ~rvqLiteTask();
    
    T_F openDatabase( Pchar name );
    T_F closeDatabase();
    T_F initializeDatabase(Pchar name);
};

OMCLASS(equipObj)
OMCLASS(engdocObj)
OMCLASS(progressObj)
OMCLASS(mprocObj)
OMCLASS(mhistObj)
OMCLASS(mhist_mproc_intObj)
OMCLASS(partsObj)
OMCLASS(eq_parts_intObj)
OMCLASS(parts_docs_intObj)
OMCLASS(vendorObj)
OMCLASS(commsysObj)
OMCLASS(eq_docs_intObj)
OMCLASS(eq_mproc_intObj)
OMCLASS(equip_name_KeyObj)
OMCLASS(engdoc_docno_KeyObj)
OMCLASS(image_ptrObj)
OMCLASS(image_equip_boxObj)
OMCLASS(mproc_name_KeyObj)
OMCLASS(parts_cat_no_KeyObj)
OMCLASS(vendor_vendcode_KeyObj)
OMCLASS(commsys_name_KeyObj)
OMCLASS(image_ptr_dba_KeyObj)

class OM_EXPORT equipObj : public StoreObj, public equip
{
public:
   equipObj() : StoreObj(KeyObj(EQUIP_NAME, name)) {}
   virtual ~equipObj() {}
   REC_TYPE RecType() { return EQUIP; }
   Pchar OMLOADDS RecName() { return "equip"; }

   virtual void Delete();
   int IsInUse();
   FIXTotalSize(equip);
   
   STOREDIN(rvqLiteDb);
   CUSTOMOPS(equipObj);
   OWNEROF(eq_docs_intObj, EQ_DOCS);
   OWNEROF(progressObj, EQ_PROGRESS);
   OWNEROF(eq_mproc_intObj, EQ_MPROC);
   OWNEROF(mhistObj, EQ_MHIST);
   OWNEROF(eq_parts_intObj, EQ_PARTS);
   OWNEROF(equipObj, ISOL_LIST);
   OWNEROF(image_equip_boxObj, EQUIP_BOX);
   MEMBEROF(commsysObj, COMMSYS_EQUIP);
   MEMBEROF(equipObj, ISOL_LIST);
   MEMBEROF(vendorObj, VENDOR_EQUIP);
};


class OM_EXPORT engdocObj : public StoreObj, public engdoc
{
public:
   engdocObj() : StoreObj(KeyObj(ENGDOC_DOCNO, docno)) {}
   virtual ~engdocObj() {}
   REC_TYPE RecType() { return ENGDOC; }
   Pchar OMLOADDS RecName() { return "engdoc"; }

   FIXTotalSize(engdoc);
   
   virtual void Delete();
   STOREDIN(rvqLiteDb);
   CUSTOMOPS(engdocObj);
   OWNEROF(eq_docs_intObj, DOCS_EQ);
   OWNEROF(parts_docs_intObj, DOCS_PARTS);
};


class OM_EXPORT progressObj : public StoreObj, public progress
{
public:
   progressObj() : StoreObj(SetObj(EQ_PROGRESS)) {}
   virtual ~progressObj() {}
   REC_TYPE RecType() { return PROGRESS; }
   Pchar OMLOADDS RecName() { return "progress"; }

   FIXTotalSize(progress);
   
   virtual void Delete();
   STOREDIN(rvqLiteDb);
   CUSTOMOPS(progressObj);
   MEMBEROF(equipObj, EQ_PROGRESS);
};


class OM_EXPORT mprocObj : public StoreObj, public mproc
{
public:
   mprocObj() : StoreObj(KeyObj(MPROC_NAME, name)) {}
   virtual ~mprocObj() {}
   REC_TYPE RecType() { return MPROC; }
   Pchar OMLOADDS RecName() { return "mproc"; }

   FIXTotalSize(mproc);
   
   virtual void Delete();
   STOREDIN(rvqLiteDb);
   CUSTOMOPS(mprocObj);
   OWNEROF(eq_mproc_intObj, MPROC_EQ);
   OWNEROF(mhist_mproc_intObj, MPROC_MHIST);
};


class OM_EXPORT mhistObj : public StoreObj, public mhist
{
public:
   mhistObj() : StoreObj(KeyObj(MHIST_REPORT_NO, report_no)) {}
   virtual ~mhistObj() {}
   REC_TYPE RecType() { return MHIST; }
   Pchar OMLOADDS RecName() { return "mhist"; }

   FIXTotalSize(mhist);
   
   virtual void Delete();
   STOREDIN(rvqLiteDb);
   CUSTOMOPS(mhistObj);
   OWNEROF(mhist_mproc_intObj, MHIST_MPROC);
   MEMBEROF(equipObj, EQ_MHIST);
};


class OM_EXPORT mhist_mproc_intObj : public StoreObj//, public mhist_mproc_int
{
public:
   mhist_mproc_intObj() {}
   mhist_mproc_intObj(RSetObj set) : StoreObj(set) {}
   mhist_mproc_intObj(RmprocObj) : StoreObj(SetObj(MPROC_MHIST)) {}
   mhist_mproc_intObj(RmhistObj) : StoreObj(SetObj(MHIST_MPROC)) {}
   virtual ~mhist_mproc_intObj() {}
   REC_TYPE RecType() { return MHIST_MPROC_INT; }
   Pchar OMLOADDS RecName() { return "mhist_mproc_int"; }

//   FIXTotalSize(mhist_mproc_int);
   
   virtual void Delete();
   STOREDIN(rvqLiteDb);
   CUSTOMOPS(mhist_mproc_intObj);
   MEMBEROF(mprocObj, MPROC_MHIST);
   MEMBEROF(mhistObj, MHIST_MPROC);
};


class OM_EXPORT partsObj : public StoreObj, public parts
{
public:
   partsObj() : StoreObj(KeyObj(PARTS_CAT_NO, cat_no)) {}
   partsObj(RvendorObj) : StoreObj(SetObj(VENDOR_PARTS)) {}
   virtual ~partsObj() {}
   REC_TYPE RecType() { return PARTS; }
   Pchar OMLOADDS RecName() { return "parts"; }

   FIXTotalSize(parts);
   
   virtual void Delete();
   STOREDIN(rvqLiteDb);
   CUSTOMOPS(partsObj);
   OWNEROF(eq_parts_intObj, PARTS_EQ);
   OWNEROF(parts_docs_intObj, PARTS_DOCS);
   MEMBEROF(vendorObj, VENDOR_PARTS);
};


class OM_EXPORT eq_parts_intObj : public StoreObj//, public eq_parts_int
{
public:
   eq_parts_intObj() {}
   eq_parts_intObj(RSetObj set) : StoreObj(set) {}
   eq_parts_intObj(RequipObj) : StoreObj(SetObj(EQ_PARTS)) {}
   eq_parts_intObj(RpartsObj) : StoreObj(SetObj(PARTS_EQ)) {}
   virtual ~eq_parts_intObj() {}
   REC_TYPE RecType() { return EQ_PARTS_INT; }
   Pchar OMLOADDS RecName() { return "eq_parts_int"; }

//   FIXTotalSize(eq_part_int);
   
   virtual void Delete();
   STOREDIN(rvqLiteDb);
   CUSTOMOPS(eq_parts_intObj);
   MEMBEROF(equipObj, EQ_PARTS);
   MEMBEROF(partsObj, PARTS_EQ);
};


class OM_EXPORT parts_docs_intObj : public StoreObj//, parts_docs_int
{
public:
   parts_docs_intObj() {}
   parts_docs_intObj(RSetObj set) : StoreObj(set) {}
   parts_docs_intObj(RpartsObj) : StoreObj(SetObj(PARTS_DOCS)) {}
   parts_docs_intObj(RengdocObj) : StoreObj(SetObj(DOCS_PARTS)) {}
   virtual ~parts_docs_intObj() {}
   REC_TYPE RecType() { return PARTS_DOCS_INT; }
   Pchar OMLOADDS RecName() { return "parts_docs_int"; }

//   FIXTotalSize(parts_docs_int);
   
   virtual void Delete();
   STOREDIN(rvqLiteDb);
   CUSTOMOPS(parts_docs_intObj);
   MEMBEROF(partsObj, PARTS_DOCS);
   MEMBEROF(engdocObj, DOCS_PARTS);
};


class OM_EXPORT vendorObj : public StoreObj, public vendor
{
public:
   vendorObj() : StoreObj(KeyObj(VENDOR_VENDCODE, vendcode)) {}
   virtual ~vendorObj() {}
   REC_TYPE RecType() { return VENDOR; }
   Pchar OMLOADDS RecName() { return "vendor"; }

   FIXTotalSize(vendor);
   
   virtual void Delete();
   STOREDIN(rvqLiteDb);
   CUSTOMOPS(vendorObj);
   OWNEROF(partsObj, VENDOR_PARTS);
   OWNEROF(equipObj, VENDOR_EQUIP);
};

class OM_EXPORT commsysObj : public StoreObj, public commsys
{
public:
    commsysObj() : StoreObj(KeyObj(COMMSYS_NAME, name)) {}
    virtual ~commsysObj() {}
    REC_TYPE RecType() { return COMMSYS; }
    Pchar OMLOADDS RecName() { return "commsys"; }
    
    FIXTotalSize(commsys);
    
    virtual void Delete();
    STOREDIN(rvqLiteDb);
    CUSTOMOPS(commsysObj);
    OWNEROF(equipObj, COMMSYS_EQUIP);
};


class OM_EXPORT eq_docs_intObj : public StoreObj//, public eq_docs_int
{
public:
   eq_docs_intObj() {}
   eq_docs_intObj(RSetObj set) : StoreObj(set) {}
   eq_docs_intObj(RequipObj) : StoreObj(SetObj(EQ_DOCS)) {}
   eq_docs_intObj(RengdocObj) : StoreObj(SetObj(DOCS_EQ)) {}
   virtual ~eq_docs_intObj() {}
   REC_TYPE RecType() { return EQ_DOCS_INT; }
   Pchar OMLOADDS RecName() { return "eq_docs_int"; }

//   FIXTotalSize(eq_docs_int);
   
   virtual void Delete();
   STOREDIN(rvqLiteDb);
   CUSTOMOPS(eq_docs_intObj);
   MEMBEROF(equipObj, EQ_DOCS);
   MEMBEROF(engdocObj, DOCS_EQ);
};


class OM_EXPORT eq_mproc_intObj : public StoreObj//, public eq_mproc_int
{
public:
   eq_mproc_intObj() {}
   eq_mproc_intObj(RSetObj set) : StoreObj(set) {}
   eq_mproc_intObj(RequipObj) : StoreObj(SetObj(EQ_MPROC)) {}
   eq_mproc_intObj(RmprocObj) : StoreObj(SetObj(MPROC_EQ)) {}
   virtual ~eq_mproc_intObj() {}
   REC_TYPE RecType() { return EQ_MPROC_INT; }
   Pchar OMLOADDS RecName() { return "eq_mproc_int"; }

//   FIXTotalSize(eq_mproc_int);
   
   virtual void Delete();
   STOREDIN(rvqLiteDb);
   CUSTOMOPS(eq_mproc_intObj);
   MEMBEROF(equipObj, EQ_MPROC);
   MEMBEROF(mprocObj, MPROC_EQ);
};

class OM_EXPORT image_ptrObj : public StoreObj, public image_ptr
{
/*
private:
   long dba;
*/
public:
   image_ptrObj() : StoreObj(KeyObj(IMAGE_PTR_DBA, (Pvoid )&(image_ptr::dba)))  {}
   virtual ~image_ptrObj() {}
   REC_TYPE RecType() { return IMAGE_PTR; }

   FIXTotalSize(image_ptr);

   virtual void Delete();

   STOREDIN(rvqLiteDb);
   CUSTOMOPS(image_ptrObj);
   OWNEROF(image_equip_boxObj, IMAGE_BOX);
};


class OM_EXPORT image_equip_boxObj : public StoreObj, public image_equip_box
{
/*
private:
   long left;
   long bottom;
   long right;
   long top;
*/
public:
   image_equip_boxObj() {}
   image_equip_boxObj(RSetObj set) : StoreObj(set) {}
   virtual ~image_equip_boxObj() {}
   REC_TYPE RecType() { return IMAGE_EQUIP_BOX; }

   FIXTotalSize(image_equip_box);

   virtual void Delete();

   STOREDIN(rvqLiteDb);
   CUSTOMOPS(image_equip_boxObj);
   MEMBEROF(image_ptrObj, IMAGE_BOX);
   MEMBEROF(equipObj, EQUIP_BOX);
};



class OM_EXPORT equip_name_KeyObj : public KeyObj
{
public:
   char name[60];

   equip_name_KeyObj() : KeyObj(EQUIP_NAME, (Pvoid) &name[0]) {}
   virtual ~equip_name_KeyObj() {}
};

class OM_EXPORT engdoc_docno_KeyObj : public KeyObj
{
public:
    char docno[16];
    
    engdoc_docno_KeyObj() : KeyObj(ENGDOC_DOCNO, (Pvoid) &docno[0]) {}
    virtual ~engdoc_docno_KeyObj() {}
};

class OM_EXPORT mproc_name_KeyObj : public KeyObj
{
public:
   char name[24];

   mproc_name_KeyObj() : KeyObj(MPROC_NAME, (Pvoid) &name[0]) {}
   virtual ~mproc_name_KeyObj() {}
};


class OM_EXPORT parts_cat_no_KeyObj : public KeyObj
{
public:
   char cat_no[16];

   parts_cat_no_KeyObj() : KeyObj(PARTS_CAT_NO, (Pvoid) &cat_no[0]) {}
   virtual ~parts_cat_no_KeyObj() {}
};


class OM_EXPORT vendor_vendcode_KeyObj : public KeyObj
{
public:
   int vendcode;

   vendor_vendcode_KeyObj() : KeyObj(VENDOR_VENDCODE, (Pvoid) &vendcode) {}
   virtual ~vendor_vendcode_KeyObj() {}
};

class OM_EXPORT commsys_name_KeyObj : public KeyObj
{
public:
    char name[32];
    
    commsys_name_KeyObj() : KeyObj(COMMSYS_NAME, (Pvoid) &name[0]) {}
    virtual ~commsys_name_KeyObj() {}
};

class OM_EXPORT image_ptr_dba_KeyObj : public KeyObj
{
public:
    long dba;
    
    image_ptr_dba_KeyObj() : KeyObj(IMAGE_PTR_DBA, (Pvoid) &dba) {}
    virtual ~image_ptr_dba_KeyObj() {}
};
#endif    /* ___rvqLite_h___ */
