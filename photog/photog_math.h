#include "mathsubs.h"

#if !defined _PHOTOG_MATH_

#if !defined DB_ADDR
#define DB_ADDR unsigned long
#define DB_ADDR_XXXX 1
#endif

typedef struct BUNDLEDATA
{
    DB_ADDR dbaBundle;	    /* dba of the bundle for which the data applies */
    int nNumPhotos;	    /* number of photos in bundle */
    int nNumPoints;	    /* number of points in the bundle */
    PNTLST plTMatrix;	    /* array of transformation matrix vectors */
    DB_ADDR *dbaTMatrix;    /* dba of each transformation matrix */
    PNTLST *plPixels;	    /* array of point lists containing the corrected film points */
    DB_ADDR *dbaPnts;	    /* array of arrays of the dba of the points record for each corrected film point */
    PNTLST plCalcPnts;	    /* point list for all calculated xyz's in the bundle */
    DB_ADDR *dbaCalcPnts;   /* dba of each calculated xyz in the master list */
    PNTLST plSurvPnts;	    /* point list for all surveyed xyz's in the master list */
    char *pszBundleName;
    char **pszPhotoNames;
    char **pszPointNames;
    VECTOR vfl;
} BundleData;
#if defined DB_ADDR_XXXX
#undef DB_ADDR_XXXX
#undef SB_ADDR
#endif

VECTOR matpnt(VECTOR m, VECTOR p);
PNT get3dpnt(int nf,PNTLST pnts, PHOTO_FLOAT **tmxs );
VECTOR idmatrix(void);
void forcesquare( PHOTO_FLOAT *L, PHOTO_FLOAT *R, PHOTO_FLOAT **A);
void forcesquare( PHOTO_FLOAT *L, PHOTO_FLOAT *R, PHOTO_FLOAT **A, PHOTO_FLOAT wf);

PHOTO_FLOAT srot(int np,PNTLST p,PNTLST xyz);
PHOTO_FLOAT *getfilmcorr(int np,PHOTO_FLOAT fl, PNTLST fud,PNTLST fp);
void radial_corr(int np ,PNTLST pnt,VECTOR rcf);
PNTLST corrfilmpnt(int npt,PHOTO_FLOAT fl, VECTOR R, PNTLST pnt, VECTOR rcf);
void bndlxx(int nf,int npt,int *npntArray, int **ndxAdrrArray, 
             PNTLST *pntAdrrArray, VECTOR *tmAdrrArray ,PHOTO_FLOAT ***xyz);

PHOTO_FLOAT rotx(int np,PNTLST p,PNTLST xyz, VECTOR R);
VECTOR srotx(int np,PNTLST p,PNTLST xyz);
VECTOR torealsys(int np, PNTLST cp, PNTLST mp, int npc,PNTLST xyz ,VECTOR tm, VECTOR itm);

int bundle( BundleData & bd );
VECTOR MergeBundles( VECTOR masterImageTMatrix, VECTOR mergeImageTMatrix,
	int numCalcPnts, PNTLST masterCalcPnts, PNTLST mergeCalcPnts );
PNTLST LUK(int nrk, int nck, int nre, PNTLST teq, PNTLST tkn);
int bundlecheck( BundleData & bd );

#define _PHOTOG_MATH_
#endif	//_PHOTOG_MATH_
