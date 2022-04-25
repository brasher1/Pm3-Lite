#ifndef __COMMON_H__
#define __COMMON_H__

#pragma warning (disable : 4244)
#pragma pack (4)
struct EnvStrings
{
	char dbPath[256];
	char imgPath[256];
	char projectID[256];
	char imageReductionFactor[256];
};

typedef struct {
	char name[60];
	double x,y,z;
	double dx,dy,dz,dist;
	char sz_x[30], sz_y[30], sz_z[30];
	char sz_dx[30], sz_dy[30], sz_dz[30];
	char sz_dist[30];
	double std_dev_x, std_dev_y, std_dev_z;
	double ray_miss_dist;
//	long dba;
} GridRowData;

typedef struct {
	char name[60];
	double east_begin, north_begin, height_begin;
	double east_end, north_end, height_end;
	char sz_east_begin[30], sz_north_begin[30], sz_height_begin[30];
	char sz_east_end[30], sz_north_end[30], sz_height_end[30];
	long dba;
} ColumnData;

typedef struct {
	char name[60];
	double SW_east, SW_north, SW_height;
	double NE_east, NE_north, NE_height;
	char szSW_east[30], szSW_north[30], szSW_height[30];
	char szNE_east[30], szNE_north[30], szNE_height[30];
	long dba;
} ZoneData;
#pragma pack ()

//#include <vista.h>

#include "../triton/tritont.hpp"
#include "../rvqlite/rvqlite1.hpp"
#include "rdm_utils.h"

BOOL OpenImageByName( char *name );

#define TASKER		&Mytask,dbn

#ifdef _FOTOBROW_CPP_
	DB_TASK	Mytask;
	TritonTask *t;
	int dbn;
	rvqLiteTask *rvqTask=NULL;
	char *reg_strings[] = {"SOFTWARE", "AsBuiltData", "Proj" };
//	PtgCamera theCamera;
//	Ptg3DNode *theObjects;
//	Ptg3DNode *theCameraNode;
//	Ptg3DNode *theRays;
//	Ptg3DNode *theModel;
#else
	extern DB_TASK	Mytask;
	extern TritonTask *t;
	extern int dbn;
	extern rvqLiteTask *rvqTask;
	extern char *reg_strings[];
//	extern PtgCamera theCamera;
//	extern Ptg3DNode *theObjects;
//	extern Ptg3DNode *theCameraNode;
//	extern Ptg3DNode *theRays;
//	extern Ptg3DNode *theModel;
#endif

typedef struct {
	double x,y,z;
} _Point3d;

void ConvertPoint(_Point3d *in_ptg, _Point3d *out_ptg);

int Calc3DPoint( char *pszBundle, _Point3d *ptr3d=NULL );
char *SearchForBundle( );
char *SearchForBundleSpecific( char **ImageNames, int count );
int getSurveyModelTransform(double *transform, const char *projectID);
int	saveSurveyModelTransform(double *transform, const char *m_ProjectID);
int getMeasurementTransform(double *transform, const char *projectID);
int	saveMeasurementTransform(double *transform, const char *m_ProjectID);
char *ConvertUnits(char *s, int size, double number);
BOOL EnglishToMetric( char *source, double *destination);

//// DATATABLE JUNK ////
#include <datatbl.h>
//#include <cdatatbl.h>
//#include <dtmfc.h>
////////////////////
#define INI_BITMAPS			"Bitmaps"
#define INI_UNITS			"Units"
#define INI_METRIC			1
#define INI_ENGLISH			0
#define INI_IMAGEPATH		"ImagePath"
#define INI_IMAGEPATHDEF	"d:\\"
#define INI_DATABASE		"Database"
#define INI_PROJECTS		"Projects"
#define INI_IMAGEPATH		"ImagePath"
#define INI_IMAGEPATHDEF	"d:\\"
#define INI_TRANSFORM		"Transform"
#define INI_TRANSFORM0		"Transform0"
#define INI_TRANSFORM1		"Transform1"
#define INI_TRANSFORM2		"Transform2"
#define INI_TRANSFORM3		"Transform3"
#define INI_TRANSFORM4		"Transform4"
#define INI_TRANSFORM5		"Transform5"
#define INI_TRANSFORM6		"Transform6"
#define INI_TRANSFORM7		"Transform7"
#define INI_TRANSFORM8		"Transform8"
#define INI_TRANSFORM9		"Transform9"
#define INI_TRANSFORM10		"Transform10"
#define INI_TRANSFORM11		"Transform11"


////////////////////
// USER DEFINED MESSAGES ///
enum MyMSG 
{
	UM_NEW_CLIP_VOLUME = WM_USER+1722,
	UM_CURRENT_ELEMENT_CHANGE,			// Current element is changing
	UM_NEW_TREE_NODE,					// Add a new node to treeview
	UM_UNITS_CHANGE,					// Current units have changed
	UM_NEW_3D_ELEMENT					// New element added to 3D hierarchy
};


#define GETWORLD	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->ptg3DWorld
#define GETSYSTEM	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->ptg3DWorld.getSystem()
#define GETREVIEW	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->ptg3DWorld.getReview()
//#define GETAXES 	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->ptg3DWorld.getAxes()
#define GETRAYS		((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->ptg3DWorld.getRays()
#define GETMODEL	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->ptg3DWorld.getModel()
//#define GETOVERLAY	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->ptg3DWorld.getCalculated()
#define GETCAMERAS	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->ptg3DWorld.getCameras()
#define GETTARGETS	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->ptg3DWorld.getTargets()
#define GETMEASUREREFS	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->ptg3DWorld.getMeasurementRefs()
#define GETCURRENTELEM ((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->getCurrentElem()
#endif //__COMMON_H__
