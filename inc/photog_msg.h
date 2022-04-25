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
#ifndef ___PHOTOG_MSG_H___
#define ___PHOTOG_MSG_H___

#ifdef MESSAGE_PASSING
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/signal.h>
#include <unistd.h>

#include "triton.h"
#include "linklist.h"
#include "drawutils.h"

#define PTG_EXIT_NORMAL 1
#define PTG_EXIT_FATAL  2

#define PGM_MSG_PROCESS_TIME 1

#define PGM_ATOM	"photog_msg"
#define PGM_TYPE_ATOM	"photog_msg_type"

#define PGM_BASE	100L

#define PGM_INVALID	    ((long)(-1L))

#define PGM_QUIT	    ((long)(PGM_BASE+0L))
#define PGM_CHILD_QUIT	    ((long)(PGM_BASE+2L))

#define PGM_ACTIVATE	    ((long)(PGM_BASE+1L))
#define PGM_CHILD_ACTIVATED ((long)(PGM_BASE+3L))
#define PGM_USTN_INIT	    ((long)(PGM_BASE+4L))
#define PGM_REQUEST_OUTPUT  ((long)(PGM_BASE+5L))
#define PGM_REFUSE_OUTPUT   ((long)(PGM_BASE+6L))
#define PGM_CANCEL	    ((long)(PGM_BASE+7L))

#define PGM_DRAW_BOX	    ((long)(PGM_BASE+100L))
#define PGM_DRAW_CONE	    ((long)(PGM_BASE+101L))
#define PGM_DRAW_CYL	    ((long)(PGM_BASE+102L))
#define PGM_DRAW_CIRTORUS   ((long)(PGM_BASE+103L))
#define PGM_DRAW_RECTTORUS  ((long)(PGM_BASE+104L))
#define PGM_DRAW_SPHERE	    ((long)(PGM_BASE+105L))
#define PGM_DRAW_PYRAMID    ((long)(PGM_BASE+106L))
#define PGM_DRAW_SNOUT	    ((long)(PGM_BASE+107L))  
#define PGM_DRAW_COMPLEX    ((long)(PGM_BASE+108L))

#define PGM_DRAW_TRANSFORM  ((long)(PGM_BASE+109L))  
#define PGM_DRAW_PROPERTY   ((long)(PGM_BASE+110L))

#define PGM_DRAW_LINE2P	    ((long)(PGM_BASE+111L))
#define PGM_DRAW_PLANE3P    ((long)(PGM_BASE+112L))
#define PGM_DRAW_POINT	    ((long)(PGM_BASE+113L))

#define PGM_DRAW_CAMERA	    ((long)(PGM_BASE+114L))
#define PGM_DRAW_RAY	    ((long)(PGM_BASE+115L))

#define PGM_3DPOINT	    ((long)(PGM_BASE+200L))
#define PGM_LINE	    ((long)(PGM_BASE+201L))
#define PGM_LINE2	    ((long)(PGM_BASE+202L))
#define PGM_CIRC	    ((long)(PGM_BASE+203L))
#define PGM_CIRC2	    ((long)(PGM_BASE+204L))
#define PGM_CYL		    ((long)(PGM_BASE+205L))
#define PGM_CYL2	    ((long)(PGM_BASE+206L))
#define PGM_BOX		    ((long)(PGM_BASE+207L))
#define PGM_BOX2	    ((long)(PGM_BASE+208L))
#define PGM_CONE	    ((long)(PGM_BASE+209L))
#define PGM_RAY		    ((long)(PGM_BASE+210L))

#define PGM_GETCAMERAS	    ((long)(PGM_BASE+211L))
#define PGM_OPENIMAGE	    ((long)(PGM_BASE+212L))

#define PGM_ADD		    ((long)(PGM_BASE+213L))
#endif //MESSAGE_PASSING

struct _transform {
   struct {
      double  pos_x;
      double  pos_y;
      double  pos_z;
   } pozition;
   struct {
      double  r[16];  
   } rotation;
   struct {
      double  sca_x;
      double  sca_y;
      double  sca_z;
   } scale;
   struct {
      double  ori_x;
      double  ori_y;
      double  ori_z;
      double  ori_beta;
   } orientation;
   struct {
      double  cen_x;
      double  cen_y;
      double  cen_z;
   } center;
};

struct CameraLoc {
    char theBundleName[128];
    char theImageName[24];
    double theMatrix[12];
};
#ifdef MESSAGE_PASSING
struct structPhotogMsg {
    long mtype;
    long msglength;
    pid_t pidFrom;
    long lnMsgID;
    pid_t pidTo;
    union {
	union PGMData{
	    struct box BoxData;
	    struct cir_torus CirTorusData;
	    struct cone ConeData;
	    struct cylinder CylData;
	    struct rect_torus RectTorusData;
	    struct sphere SphereData;
	    struct pyramid PyramidData;
	    struct snout SnoutData;
	    struct complex ComplexData;
	    struct _transform TransformData;
	    struct properties PropertyData;
	    struct Line2p line2p;
	    struct Plane3p plane3p;
	    struct _Point3d point3d;
	    struct Ray ray2p;
	    struct CameraLoc CameraData;
	}Data;	
	char s[sizeof(union PGMData)];
    };
};
typedef struct structPhotogMsg PhotogMsg;

struct structPGM_MsgPath {
    pid_t pidFrom;
    pid_t pidTo;
    int nFromIPCMsgID;
    int nToIPCMsgID;
};
typedef struct structPGM_MsgPath PGM_MsgPath;


#ifndef _NO_MSG_PROTOS_  //to eliminate X params from RDM

int PhotogMsgInit( int (*msgFunc)(PhotogMsg *msg), int (*errorFunc)(int exitType) = NULL );
int PhotogMsgAddPath( pid_t pidFrom = 0, pid_t pidTo = 0 );
int PhotogMsgDelPath( pid_t pidFrom = 0, pid_t pidTo = 0);
PGM_MsgPath *PhotogMsgFindPath( pid_t pidFrom = 0, pid_t pidTo = 0 );
node_t *PhotogMsgFindPathNode( pid_t pidFrom = 0, pid_t pidTo = 0 );
int PhotogMsgDelAllPaths( void );
int PhotogSendMessage( PhotogMsg *msg );
void PhotogMsgMainLoop( int (*UserFunc)(PhotogMsg *) = NULL );
void PhotogMsgPathsPrint( void );
void DumpPhotogMsgHeader( PhotogMsg *msg );


#endif // _NO_MSG_PROTOS_
#endif //#MESSAGE_PASSING
#endif // ___PHOTOG_MSG_H___
