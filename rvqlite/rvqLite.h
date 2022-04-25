#ifndef RVQLITE_H
#define RVQLITE_H

/* Raima Database Manager Version 3.30 */

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
*	rvq.ddl								    *
*									    *
*   DESCRIPTION								    *
*	Database for review query maintenance system.			    *
*									    *
*   NOTES								    *
*									    *
*   BUGS								    *
*									    *
*   REVISION HISTORY							    *
*	07/28/94 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/

#define VISTACALLS
/* database rvqlite record/key structure declarations */
#pragma pack (1)

struct equip {
   char name[60];
   char type[24];
   char area[6];
   char po_no[16];
   char descrip[64];
};

struct engdoc {
   char docno[16];
   char descrip[64];
   char assoc_appl[32];
   char datafile[120];
};

struct progress {
   char discip[32];
   char status[32];
};

struct mproc {
   char name[24];
   char descrip[64];
   char assoc_appl[32];
   char datafile[120];
   long interval;
   long last_done;
   long sched;
   long due;
};

struct mhist {
   char report_no[24];
   long servicedate;
   char assoc_appl[32];
   char datafile[120];
};

struct parts {
   char cat_no[16];
   char descrip[64];
   char millstoreloc[16];
   int in_stock;
   int reorder_at;
   int reorder_qty;
};

struct vendor {
   int vendcode;
   char add_1[32];
   char add_2[32];
   char city[26];
   char cnt_in[30];
   char em_phone[12];
   char ico_name[32];
   char maj_prod[48];
   char name_cde[6];
   char phone[12];
   char state[4];
   char vnd_name[32];
   char zip[12];
   char cat_code[2];
   char sta_code[4];
};

struct commsys {
   char name[32];
   long sch_flush;
   long prj_flush;
   long act_flush;
   long sch_turnkey;
   long prj_turnkey;
   long act_turnkey;
};

struct image_ptr {
   long dba;
};

struct image_equip_box {
   long left;
   long bottom;
   long right;
   long top;
};
#pragma pack ()
/* record, field and set table entry definitions */

/* File Id Constants */
#define DATA001 0
#define DATA002 1
#define DATA003 2
#define DATA004 3
#define DATA005 4
#define DATA006 5
#define DATA007 6
#define DATA008 7
#define DATA009 8
#define DATA010 9
#define DATA011 10
#define DATA012 11
#define DATA013 12
#define DATA014 13
#define DATA015 14
#define KEY001 15
#define KEY002 16
#define KEY003 17
#define KEY004 18
#define KEY005 19
#define KEY006 20
#define KEY007 21
#define KEY008 22

/* Record Name Constants */
#define EQUIP 10000
#define ENGDOC 10001
#define PROGRESS 10002
#define MPROC 10003
#define MHIST 10004
#define MHIST_MPROC_INT 10005
#define PARTS 10006
#define EQ_PARTS_INT 10007
#define PARTS_DOCS_INT 10008
#define VENDOR 10009
#define COMMSYS 10010
#define EQ_DOCS_INT 10011
#define EQ_MPROC_INT 10012
#define IMAGE_PTR 10013
#define IMAGE_EQUIP_BOX 10014

/* Field Name Constants */
#define EQUIP_NAME 0L
#define EQUIP_TYPE 1L
#define EQUIP_AREA 2L
#define EQUIP_PO_NO 3L
#define EQUIP_DESCRIP 4L
#define ENGDOC_DOCNO 1000L
#define ENGDOC_DESCRIP 1001L
#define ENGDOC_ASSOC_APPL 1002L
#define ENGDOC_DATAFILE 1003L
#define PROGRESS_DISCIP 2000L
#define PROGRESS_STATUS 2001L
#define MPROC_NAME 3000L
#define MPROC_DESCRIP 3001L
#define MPROC_ASSOC_APPL 3002L
#define MPROC_DATAFILE 3003L
#define MPROC_INTERVAL 3004L
#define MPROC_LAST_DONE 3005L
#define MPROC_SCHED 3006L
#define MPROC_DUE 3007L
#define MHIST_REPORT_NO 4000L
#define MHIST_SERVICEDATE 4001L
#define MHIST_ASSOC_APPL 4002L
#define MHIST_DATAFILE 4003L
#define PARTS_CAT_NO 6000L
#define PARTS_DESCRIP 6001L
#define PARTS_MILLSTORELOC 6002L
#define PARTS_IN_STOCK 6003L
#define PARTS_REORDER_AT 6004L
#define PARTS_REORDER_QTY 6005L
#define VENDOR_VENDCODE 9000L
#define VENDOR_ADD_1 9001L
#define VENDOR_ADD_2 9002L
#define VENDOR_CITY 9003L
#define VENDOR_CNT_IN 9004L
#define VENDOR_EM_PHONE 9005L
#define VENDOR_ICO_NAME 9006L
#define VENDOR_MAJ_PROD 9007L
#define VENDOR_NAME_CDE 9008L
#define VENDOR_PHONE 9009L
#define VENDOR_STATE 9010L
#define VENDOR_VND_NAME 9011L
#define VENDOR_ZIP 9012L
#define VENDOR_CAT_CODE 9013L
#define VENDOR_STA_CODE 9014L
#define COMMSYS_NAME 10000L
#define COMMSYS_SCH_FLUSH 10001L
#define COMMSYS_PRJ_FLUSH 10002L
#define COMMSYS_ACT_FLUSH 10003L
#define COMMSYS_SCH_TURNKEY 10004L
#define COMMSYS_PRJ_TURNKEY 10005L
#define COMMSYS_ACT_TURNKEY 10006L
#define IMAGE_PTR_DBA 13000L
#define IMAGE_EQUIP_BOX_LEFT 14000L
#define IMAGE_EQUIP_BOX_BOTTOM 14001L
#define IMAGE_EQUIP_BOX_RIGHT 14002L
#define IMAGE_EQUIP_BOX_TOP 14003L

/* Set Name Constants */
#define EQ_DOCS 20000
#define DOCS_EQ 20001
#define EQ_PROGRESS 20002
#define EQ_MPROC 20003
#define MPROC_EQ 20004
#define EQ_MHIST 20005
#define MPROC_MHIST 20006
#define MHIST_MPROC 20007
#define EQ_PARTS 20008
#define PARTS_EQ 20009
#define PARTS_DOCS 20010
#define DOCS_PARTS 20011
#define VENDOR_PARTS 20012
#define VENDOR_EQUIP 20013
#define COMMSYS_EQUIP 20014
#define ISOL_LIST 20015
#define IMAGE_BOX 20016
#define EQUIP_BOX 20017

/* Field Sizes */
#define SIZEOF_EQUIP_NAME 60
#define SIZEOF_EQUIP_TYPE 24
#define SIZEOF_EQUIP_AREA 6
#define SIZEOF_EQUIP_PO_NO 16
#define SIZEOF_EQUIP_DESCRIP 64
#define SIZEOF_ENGDOC_DOCNO 16
#define SIZEOF_ENGDOC_DESCRIP 64
#define SIZEOF_ENGDOC_ASSOC_APPL 32
#define SIZEOF_ENGDOC_DATAFILE 120
#define SIZEOF_PROGRESS_DISCIP 32
#define SIZEOF_PROGRESS_STATUS 32
#define SIZEOF_MPROC_NAME 24
#define SIZEOF_MPROC_DESCRIP 64
#define SIZEOF_MPROC_ASSOC_APPL 32
#define SIZEOF_MPROC_DATAFILE 120
#define SIZEOF_MPROC_INTERVAL 4
#define SIZEOF_MPROC_LAST_DONE 4
#define SIZEOF_MPROC_SCHED 4
#define SIZEOF_MPROC_DUE 4
#define SIZEOF_MHIST_REPORT_NO 24
#define SIZEOF_MHIST_SERVICEDATE 4
#define SIZEOF_MHIST_ASSOC_APPL 32
#define SIZEOF_MHIST_DATAFILE 120
#define SIZEOF_PARTS_CAT_NO 16
#define SIZEOF_PARTS_DESCRIP 64
#define SIZEOF_PARTS_MILLSTORELOC 16
#define SIZEOF_PARTS_IN_STOCK 4
#define SIZEOF_PARTS_REORDER_AT 4
#define SIZEOF_PARTS_REORDER_QTY 4
#define SIZEOF_VENDOR_VENDCODE 4
#define SIZEOF_VENDOR_ADD_1 32
#define SIZEOF_VENDOR_ADD_2 32
#define SIZEOF_VENDOR_CITY 26
#define SIZEOF_VENDOR_CNT_IN 30
#define SIZEOF_VENDOR_EM_PHONE 12
#define SIZEOF_VENDOR_ICO_NAME 32
#define SIZEOF_VENDOR_MAJ_PROD 48
#define SIZEOF_VENDOR_NAME_CDE 6
#define SIZEOF_VENDOR_PHONE 12
#define SIZEOF_VENDOR_STATE 4
#define SIZEOF_VENDOR_VND_NAME 32
#define SIZEOF_VENDOR_ZIP 12
#define SIZEOF_VENDOR_CAT_CODE 2
#define SIZEOF_VENDOR_STA_CODE 4
#define SIZEOF_COMMSYS_NAME 32
#define SIZEOF_COMMSYS_SCH_FLUSH 4
#define SIZEOF_COMMSYS_PRJ_FLUSH 4
#define SIZEOF_COMMSYS_ACT_FLUSH 4
#define SIZEOF_COMMSYS_SCH_TURNKEY 4
#define SIZEOF_COMMSYS_PRJ_TURNKEY 4
#define SIZEOF_COMMSYS_ACT_TURNKEY 4
#define SIZEOF_IMAGE_PTR_DBA 4
#define SIZEOF_IMAGE_EQUIP_BOX_LEFT 4
#define SIZEOF_IMAGE_EQUIP_BOX_BOTTOM 4
#define SIZEOF_IMAGE_EQUIP_BOX_RIGHT 4
#define SIZEOF_IMAGE_EQUIP_BOX_TOP 4

#endif    /* RVQLITE_H */
