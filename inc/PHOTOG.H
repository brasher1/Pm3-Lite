#ifndef PHOTOG_H
#define PHOTOG_H

/* Raima Database Manager Version 3.30 */


/* database photog record/key structure declarations */
#pragma pack (8)

struct points {
   char szLabel[24];
   char cType;
};

struct images {
   char szName[24];
   char szStoreageLocation[256];
   char szFileName[128];
   long lPixHeight;
   long lPixWidth;
   long lFileSize;
   long lUpToDate;
   long lStatus;
   long lLastUserMod;
   long lLastTimeMod;
   double dfUpVector[2][2];
   double dfFilmCorr[12];
   double dfCameraPos[12];
   double dfAdjFocalLength;
};

struct pixels {
   double dfImgCoor[2];
   double dfSigma[3];
   double dfCorr[3];
   long lUpToDate;
};

struct xyz {
   double dfCoor[3];
   double dfSigma[3];
   long lUpToDate;
};

struct bundle {
   char szTag[128];
   long lStatus;
   long lLastUserMod;
   long lLastTimeMod;
   long lUpToDate;
};

struct camera_info {
   char szID[24];
   char szDesc[64];
   double dfFocalLength;
   double dfRadCorrFact[9];
   long lUpToDate;
};

struct fiducial {
   char szTag[64];
   double dfIdealCoor[2];
   long lUpToDate;
};

struct tmatrix {
   double dfCameraPos[12];
   double dfAdjFocalLength;
   long lStatus;
   long lUpToDate;
};

#pragma pack ()
/* record, field and set table entry definitions */

/* File Id Constants */
#define DATA01 0
#define DATA02 1
#define DATA03 2
#define DATA04 3
#define DATA05 4
#define DATA06 5
#define DATA07 6
#define DATA08 7
#define KEY01 8
#define KEY02 9
#define KEY03 10
#define KEY04 11

/* Record Name Constants */
#define POINTS 10000
#define IMAGES 10001
#define PIXELS 10002
#define XYZ 10003
#define BUNDLE 10004
#define CAMERA_INFO 10005
#define FIDUCIAL 10006
#define TMATRIX 10007

/* Field Name Constants */
#define POINTS_SZLABEL 0L
#define POINTS_CTYPE 1L
#define IMAGES_SZNAME 1000L
#define IMAGES_SZSTOREAGELOCATION 1001L
#define IMAGES_SZFILENAME 1002L
#define IMAGES_LPIXHEIGHT 1003L
#define IMAGES_LPIXWIDTH 1004L
#define IMAGES_LFILESIZE 1005L
#define IMAGES_LUPTODATE 1006L
#define IMAGES_LSTATUS 1007L
#define IMAGES_LLASTUSERMOD 1008L
#define IMAGES_LLASTTIMEMOD 1009L
#define IMAGES_DFUPVECTOR 1010L
#define IMAGES_DFFILMCORR 1011L
#define IMAGES_DFCAMERAPOS 1012L
#define IMAGES_DFADJFOCALLENGTH 1013L
#define PIXELS_DFIMGCOOR 2000L
#define PIXELS_DFSIGMA 2001L
#define PIXELS_DFCORR 2002L
#define PIXELS_LUPTODATE 2003L
#define XYZ_DFCOOR 3000L
#define XYZ_DFSIGMA 3001L
#define XYZ_LUPTODATE 3002L
#define BUNDLE_SZTAG 4000L
#define BUNDLE_LSTATUS 4001L
#define BUNDLE_LLASTUSERMOD 4002L
#define BUNDLE_LLASTTIMEMOD 4003L
#define BUNDLE_LUPTODATE 4004L
#define CAMERA_INFO_SZID 5000L
#define CAMERA_INFO_SZDESC 5001L
#define CAMERA_INFO_DFFOCALLENGTH 5002L
#define CAMERA_INFO_DFRADCORRFACT 5003L
#define CAMERA_INFO_LUPTODATE 5004L
#define FIDUCIAL_SZTAG 6000L
#define FIDUCIAL_DFIDEALCOOR 6001L
#define FIDUCIAL_LUPTODATE 6002L
#define TMATRIX_DFCAMERAPOS 7000L
#define TMATRIX_DFADJFOCALLENGTH 7001L
#define TMATRIX_LSTATUS 7002L
#define TMATRIX_LUPTODATE 7003L

/* Set Name Constants */
#define POINTS_XYZ 20000
#define POINTS_PIXELS 20001
#define IMAGES_PIXELS 20002
#define BUNDLE_XYZ 20003
#define CAMERA_INFO_IMAGES 20004
#define IMAGES_TMATRIX 20005
#define BUNDLE_TMATRIX 20006
#define POINTS_SURVEY_XYZ 20007
#define CAMERA_INFO_FIDUCIAL 20008
#define FIDUCIAL_PIXELS 20009
#define IMAGES_FID_PIXELS 20010

/* Field Sizes */
#define SIZEOF_POINTS_SZLABEL 24
#define SIZEOF_POINTS_CTYPE 1
#define SIZEOF_IMAGES_SZNAME 24
#define SIZEOF_IMAGES_SZSTOREAGELOCATION 256
#define SIZEOF_IMAGES_SZFILENAME 128
#define SIZEOF_IMAGES_LPIXHEIGHT 4
#define SIZEOF_IMAGES_LPIXWIDTH 4
#define SIZEOF_IMAGES_LFILESIZE 4
#define SIZEOF_IMAGES_LUPTODATE 4
#define SIZEOF_IMAGES_LSTATUS 4
#define SIZEOF_IMAGES_LLASTUSERMOD 4
#define SIZEOF_IMAGES_LLASTTIMEMOD 4
#define SIZEOF_IMAGES_DFUPVECTOR 32
#define SIZEOF_IMAGES_DFFILMCORR 96
#define SIZEOF_IMAGES_DFCAMERAPOS 96
#define SIZEOF_IMAGES_DFADJFOCALLENGTH 8
#define SIZEOF_PIXELS_DFIMGCOOR 16
#define SIZEOF_PIXELS_DFSIGMA 24
#define SIZEOF_PIXELS_DFCORR 24
#define SIZEOF_PIXELS_LUPTODATE 4
#define SIZEOF_XYZ_DFCOOR 24
#define SIZEOF_XYZ_DFSIGMA 24
#define SIZEOF_XYZ_LUPTODATE 4
#define SIZEOF_BUNDLE_SZTAG 128
#define SIZEOF_BUNDLE_LSTATUS 4
#define SIZEOF_BUNDLE_LLASTUSERMOD 4
#define SIZEOF_BUNDLE_LLASTTIMEMOD 4
#define SIZEOF_BUNDLE_LUPTODATE 4
#define SIZEOF_CAMERA_INFO_SZID 24
#define SIZEOF_CAMERA_INFO_SZDESC 64
#define SIZEOF_CAMERA_INFO_DFFOCALLENGTH 8
#define SIZEOF_CAMERA_INFO_DFRADCORRFACT 72
#define SIZEOF_CAMERA_INFO_LUPTODATE 4
#define SIZEOF_FIDUCIAL_SZTAG 64
#define SIZEOF_FIDUCIAL_DFIDEALCOOR 16
#define SIZEOF_FIDUCIAL_LUPTODATE 4
#define SIZEOF_TMATRIX_DFCAMERAPOS 96
#define SIZEOF_TMATRIX_DFADJFOCALLENGTH 8
#define SIZEOF_TMATRIX_LSTATUS 4
#define SIZEOF_TMATRIX_LUPTODATE 4

#endif    /* PHOTOG_H */
