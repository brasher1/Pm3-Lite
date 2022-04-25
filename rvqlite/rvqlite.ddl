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

database rvqlite {
    data file data001="rvqlite.d01" contains equip;
    data file data002="rvqlite.d02" contains engdoc;
    data file data003="rvqlite.d03" contains progress;
    data file data004="rvqlite.d04" contains mproc;
    data file data005="rvqlite.d05" contains mhist;
    data file data006="rvqlite.d06" contains mhist_mproc_int;
    data file data007="rvqlite.d07" contains parts;
    data file data008="rvqlite.d08" contains eq_parts_int;
    data file data009="rvqlite.d09" contains parts_docs_int;
    data file data010="rvqlite.d10" contains vendor;
    data file data011="rvqlite.d11" contains eq_docs_int;
    data file data012="rvqlite.d12" contains eq_mproc_int;
    data file data013="rvqlite.d13" contains commsys;
    data file data014="rvqlite.d14" contains image_ptr;
    data file data015="rvqlite.d15" contains image_equip_box;

    key file key001="rvqlite.k01" contains equip.name;
    key file key002="rvqlite.k02" contains engdoc.docno;
    key file key003="rvqlite.k03" contains mproc.name;
    key file key004="rvqlite.k04" contains parts.cat_no;
    key file key005="rvqlite.k05" contains vendor.vendcode;
    key file key006="rvqlite.k06" contains commsys.name;
    key file key007="rvqlite.k07" contains mhist.report_no;
    key file key008="rvqlite.k08" contains image_ptr.dba;

    record equip {
	unique key char name[60];       /* component name in pdms */
	char type[24];		        /* component type (vessel, pipe, etc.) */
	char area[6];		        /* mill area component belongs to */
	char po_no[16];		        /* purchase order for componnent */
	char descrip[64];	        /* description of equipment */
    }

    record engdoc {
	unique key char docno[16];	/* document number */
	char descrip[64];	        /* description of document */
	char assoc_appl[32];	        /* application to be run for datafile */
	char datafile[120];	        /* data file representing the document */
    }

    record progress {
	char discip[32];	        /* discipline for a piece of equipment */
	char status[32];	        /* what state the equipment is in */
    }

    record mproc {		        /* maintenance procedure record */
	unique key char name[24];	/* procedure name */
	char descrip[64];	        /* description of maintenance procedure */
	char assoc_appl[32];	        /* application to be run for datafile */
	char datafile[120];	        /* data file describing the procedure */
	long interval;		        /* interval over which the procedure is regularly scheduled */
	long last_done;			/* date the procedure was last performed */
	long sched;		        /* date the procedure is next scheduled for */
	long due;		        /* date the procedure is due to be done by */
    }
    
    record mhist {		        /* maintenance history record */
	unique key char report_no[24];
	long servicedate;		/* date service was performed */
	char assoc_appl[32];	        /* application to be run for datafile */
	char datafile[120];	        /* data file representing the document */
    }

    record mhist_mproc_int {		/* maintenance history intersection */
    }

    record parts {
	key char cat_no[16];		/* catalog number of spare part */
	char descrip[64];	        /* description of part */
	char millstoreloc[16];	        /* mill storeage location */
	int in_stock;		        /* quantity of part in stock */
	int reorder_at;		        /* quantity at which to reorder */
	int reorder_qty;	        /* quantity to reorder when necessary */
    }

    record eq_parts_int {
    }

    record parts_docs_int {
    }

    record vendor {
	unique key int vendcode;
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
    }

    record commsys {
	unique key char name[32];
	long sch_flush;
	long prj_flush;
	long act_flush;
	long sch_turnkey;
	long prj_turnkey;
	long act_turnkey;
    }

    record eq_docs_int {
    }

    record eq_mproc_int {
    }

    record image_ptr {
		unique key long dba;
    }

    record image_equip_box {
	long left;
	long bottom;
	long right;
	long top;
    }	

    set eq_docs {
	order last;
	owner equip;
	member eq_docs_int;
    }

    set docs_eq {
	order last;
	owner engdoc;
	member eq_docs_int;
    }

    set eq_progress {
	order last;
	owner equip;
	member progress;
    }

    set eq_mproc {
	order last;
	owner equip;
	member eq_mproc_int;
    }

    set mproc_eq {
	order last;
	owner mproc;
	member eq_mproc_int;
    }

    set eq_mhist {
	order last;
	owner equip;
	member mhist;
    }

    set mproc_mhist {
	order last;
	owner mproc;
	member mhist_mproc_int;
    }

    set mhist_mproc {
	order last;
	owner mhist;
	member mhist_mproc_int;
    }

    set eq_parts {
	order last;
	owner equip;
	member eq_parts_int;
    }

    set parts_eq {
	order last;
	owner parts;
	member eq_parts_int;
    }

    set parts_docs {
	order last;
	owner parts;
	member parts_docs_int;
    }

    set docs_parts {
	order last;
	owner engdoc;
	member parts_docs_int;
    }

    set vendor_parts {
	order last;
	owner vendor;
	member parts;
    }

    set vendor_equip {
	order last;
	owner vendor;
	member equip;
    }

    set commsys_equip {
	order last;
	owner commsys;
	member equip;
    }

    set isol_list {
	order last;
	owner equip;
	member equip;
    }

    set image_box {
	order last;
	owner image_ptr;
	member image_equip_box;
    }

    set equip_box {
	order last;
	owner equip;
	member image_equip_box;
    }
}
