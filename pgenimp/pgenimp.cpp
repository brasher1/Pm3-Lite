#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <afx.h>

#include <vista.h>

#include "TextToColumns.h"
#include "..\inc\photog.h"
#include "..\photog\mathsubs.h"
#include "..\photog\photog_math.h"

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

typedef struct {
	char *fileName;
	int (*function)( int dbn, CStdioFile &file );
} FileDispatch;

typedef struct {
	char name[64];
	double focalLength;
	double ppac[2];
	double ppbs[2];
	double filmFormat[2];
	double filmFormatScale;
	BOOL lensDistortionFlag;
	char inputMode[32];
	double fiducials[8][2];
	double distortionParams[10][2];
} PGenCamera;

typedef struct {
	char name[64];
	char cameraName[64];
	int cameraOrientation;
	char imageID[256];
	char typeOfIO[64];
	double IOParams[6];
	double IONumIters;
	double IONumDOF;
	double IOApostStdDev;
	double EOParams[6];
	double computedStdDevs[6];
} PGenPhoto;

int PGenCamToPM3Cam( PGenCamera pgenCam );
int PGenPhotoToPM3Photo( PGenPhoto pgenPhoto );

int cameraFunc( int dbn, CStdioFile &file );
int controlFunc( int dbn, CStdioFile &file );
int modelFunc( int dbn, CStdioFile &file );
int photoFunc( int dbn, CStdioFile &file );
int projectFunc( int dbn, CStdioFile &file );
int triangFunc( int dbn, CStdioFile &file );

FileDispatch fdl[] = {
	{ "project",	projectFunc },
	{ "camera",		cameraFunc },
	{ "control",	controlFunc },
	{ "photo",		photoFunc },
	{ "triang",		triangFunc },
	{ "model",		modelFunc },
};

// Global variables
DB_TASK dbTask;
int dbn;

int main( int argc, char *argv[] )
{
	if( argc != 3 )
	{
		printf( "USAGE: pgenimp pgendir pm3dir\n" );
		return 0;
	}

	// Get DB paths and standardize to have \ on end
	char pgenDBPath[_MAX_PATH];
	char pm3DBPath[_MAX_PATH];

	strcpy( pgenDBPath, argv[1] );
	strcpy( pm3DBPath, argv[2] );

	if( pgenDBPath[strlen(pgenDBPath)-1] != '\\' )
		strcat( pgenDBPath, "\\" );
	if( pm3DBPath[strlen(pm3DBPath)-1] != '\\' )
		strcat( pm3DBPath, "\\" );

	printf( "PlantGen DB Path = '%s'\n", pgenDBPath );
	printf( "PM3 DB Path = '%s'\n", pm3DBPath );

	// Attempt to open PM3 DB
	char pm3DBName[_MAX_PATH];
	int dbStatus = S_OKAY;

	sprintf( pm3DBName, "%s%s", pm3DBPath, "photog" );

	dt_opentask(&dbTask);
	dt_lockcomm(LMC_INTERNAL,&dbTask);
	
	dt_dbuserid("pgenimp",&dbTask);

	if( (dbStatus=dt_open((char *)(const char *)pm3DBName,"x", &dbTask)) != S_OKAY)
	{
		printf( "Error(%d) opening '%s'\n", dbStatus, pm3DBName );
		dt_closetask( &dbTask );
		return 0;
	}

	dbn = dt_dbnum( "photog", &dbTask );
	dt_initialize( &dbTask, dbn );

	printf( "PM3 DB opened successfully(%d).\n", dbn );

	// Handle data files from PlantGen database
	for( int i = 0; i < ARRAYSIZE(fdl); i++ )
	{
		if( fdl[i].function )
		{
			char pgenDBFile[_MAX_PATH];
			sprintf( pgenDBFile, "%s%s", pgenDBPath, fdl[i].fileName );

			CStdioFile pgenData( pgenDBFile, CFile::modeRead|CFile::typeText );

			fdl[i].function( dbn, pgenData );
		}
	}

	dt_close(&dbTask);
	dt_closetask(&dbTask);

	return 0;
}

int cameraFunc( int dbn, CStdioFile &file )
{
	CString pgenLine;
	PGenCamera pgenCam;

	printf( "Importing cameras" );

	while( file.ReadString( pgenLine ) != FALSE )
	{
		printf( "." );
		char **fields = NULL;
		int numFields = 0;

		TextToColumns( (char *)(LPCSTR)pgenLine, &fields, &numFields, " \t:", TRUE );

		if( numFields > 0 )
		{
			if( strcmp( fields[0], "begin" ) == 0 && strcmp( fields[1], "camera_parameters" ) == 0 )
			{
				memset( &pgenCam, 0, sizeof(pgenCam) );
				strcpy( pgenCam.name, _strlwr(fields[2]) );
			}
			else if( strcmp( fields[0], "focal_length" ) == 0 )
			{
				pgenCam.focalLength = atof(fields[1]);
			}
			else if( strcmp( fields[0], "ppac" ) == 0 )
			{
				pgenCam.ppac[0] = atof(fields[1]);
				pgenCam.ppac[1] = atof(fields[2]);
			}
			else if( strcmp( fields[0], "distortion_spacing" ) == 0 )
			{
				for( int j = 0; j < 10; j++ )
					pgenCam.distortionParams[j][0] = atof(fields[j+1]);
			}
			else if( strcmp( fields[0], "distortions" ) == 0 )
			{
				for( int j = 0; j < 10; j++ )
					pgenCam.distortionParams[j][1] = atof(fields[j+1]);
			}
			else if( strcmp( fields[0], "end" ) == 0 )
			{
				PGenCamToPM3Cam( pgenCam );
				memset( &pgenCam, 0, sizeof(pgenCam) );
			}
//			else
//				printf( "Camera: %s - Field %s not handled\n", pgenCam.name, fields[0] );
		}

		FreeTextToColumns( fields, numFields );
	}
	printf( "\n" );

	return TRUE;
}

int PGenCamToPM3Cam( PGenCamera pgenCam )
{
	struct camera_info camInfo;
	memset( &camInfo, 0, sizeof(camInfo) );

	strcpy( camInfo.szID, pgenCam.name );
	strcpy( camInfo.szDesc, pgenCam.name );
	camInfo.dfFocalLength = -pgenCam.focalLength;

	// Calculate radial correction factors
	MATRIX *m1 = matrix( 1, 3, 1, 3 );
	int i, j;
	for( i = 0; i < 9; i++ )
	{
		int row = i/3;
		int col = i%3;
		int power = 6 + 2*row + 2*col;
		row++;
		col++;

		(*m1)[row][col] = 0.0;
		for( j = 0; j < 10; j++ )
			(*m1)[row][col] += pow(pgenCam.distortionParams[j][0], power);
	}

	VECTOR v1 = vector( 1, 3 );
	for( i = 0; i < 3; i++ )
	{
		v1[i+1] = 0.0;
		for( j = 0; j < 10; j++ )
			v1[i+1] += pgenCam.distortionParams[j][1]/1000.0 * pow(pgenCam.distortionParams[j][0],3+i*2);
	}

	VECTOR v2 = solve( m1, v1, 3 );

	VECTOR v3 = matrix_vector( m1, 1, 3, 1, 3, v2, 1, 3 );

	camInfo.dfRadCorrFact[0] = -v2[1];
	camInfo.dfRadCorrFact[1] = -v2[2];
	camInfo.dfRadCorrFact[2] = -v2[3];

	free_matrix( m1, 1, 3, 1, 3 );
	free_vector( v1, 1, 3 );
	free_vector( v2, 1, 3 );
	free_vector( v3, 1, 3 );

	int dbStatus = dt_fillnew( CAMERA_INFO, &camInfo, &dbTask, dbn );
	if( dbStatus != S_OKAY )
		printf( "Error(%d) creating camera '%s'.\n", dbStatus, camInfo.szID );

	return TRUE;
}

int controlFunc( int dbn, CStdioFile &file )
{
	CString pgenLine;
	int i = 0;
	int insideControlPoints = FALSE;

	printf( "Importing control" );

	while( file.ReadString( pgenLine ) != FALSE )
	{
		char **fields = NULL;
		int numFields = 0;

		printf( "." );
		TextToColumns( (char *)(LPCSTR)pgenLine, &fields, &numFields, " \t:", TRUE );

		if( numFields > 0 )
		{
			if( strcmp( fields[0], "begin" ) == 0 && strcmp( fields[1], "control_points" ) == 0 )
			{
				insideControlPoints = TRUE;
			}
			else if( insideControlPoints )
			{
				if( strcmp( fields[0], "end" ) == 0 )
					insideControlPoints = FALSE;
				else
				{
					struct points thePoint;
					struct xyz theXYZ;

					memset( &thePoint, 0, sizeof(thePoint) );
					memset( &theXYZ, 0, sizeof(theXYZ) );

					strcpy( thePoint.szLabel, fields[0] );
					thePoint.cType = 'S';
					int dbStatus = dt_fillnew( POINTS, &thePoint, &dbTask, dbn );
					if( dbStatus == S_OKAY || dbStatus == S_DUPLICATE )
					{
						dt_setor( POINTS_SURVEY_XYZ, &dbTask, dbn );
						theXYZ.dfCoor[0] = atof(fields[1]) * 1000.0;
						theXYZ.dfCoor[1] = atof(fields[3]) * 1000.0;
						theXYZ.dfCoor[2] = -atof(fields[2]) * 1000.0;

						dt_fillnew( XYZ, &theXYZ, &dbTask, dbn );
						dt_connect( POINTS_SURVEY_XYZ, &dbTask, dbn );
					}
				}
			}
			else
				printf( "Field %s not handled\n", fields[0] );
		}

		FreeTextToColumns( fields, numFields );
	}
	printf( "\n" );

	return TRUE;
}

int modelFunc( int dbn, CStdioFile &file )
{
	return TRUE;
}

int photoFunc( int dbn, CStdioFile &file )
{
	CString pgenLine;
	int i = 0;
	PGenPhoto pgenPhoto;
	int insidePhotoParams = FALSE;

	printf( "Importing photo" );
	while( file.ReadString( pgenLine ) != FALSE )
	{
		char **fields = NULL;
		int numFields = 0;

		printf( "." );
		TextToColumns( (char *)(LPCSTR)pgenLine, &fields, &numFields, " \t:", TRUE );

		if( numFields > 0 )
		{
			if( strcmp( fields[0], "begin" ) == 0 && strcmp( fields[1], "photo_parameters" ) == 0 )
				insidePhotoParams = TRUE;

			if( insidePhotoParams )
			{
				if( strcmp( fields[0], "begin" ) == 0 && strcmp( fields[1], "photo_parameters" ) == 0 )
				{
					memset( &pgenPhoto, 0, sizeof(pgenPhoto) );
					strcpy( pgenPhoto.name, fields[2] );
				}
				else if( strcmp( fields[0], "end" ) == 0 )
				{
					PGenPhotoToPM3Photo( pgenPhoto );
					memset( &pgenPhoto, 0, sizeof(pgenPhoto) );
				}
				else if( strcmp( fields[0], "camera_name" ) == 0 )
				{
					strcpy( pgenPhoto.cameraName, _strlwr(fields[1]) );
				}
				else if( strcmp( fields[0], "camera_orientation" ) == 0 )
				{
					pgenPhoto.cameraOrientation = atoi(fields[1]);
				}
				else if( strcmp( fields[0], "image_id" ) == 0 )
				{
					strcpy( pgenPhoto.imageID, fields[1] );
				}
				else if( strcmp( fields[0], "type_of_IO" ) == 0 )
				{
					strcpy( pgenPhoto.typeOfIO, fields[1] );
				}
				else if( strcmp( fields[0], "IO_parameters" ) == 0 )
				{
					for( int i = 0; i < 6; i++ )
						pgenPhoto.IOParams[i] = atof(fields[i+1]);
				}
				else if( strcmp( fields[0], "IO_num_iters" ) == 0 )
				{
					pgenPhoto.IONumIters = atof(fields[1]);
				}
				else if( strcmp( fields[0], "IO_num_DOF" ) == 0 )
				{
					pgenPhoto.IONumDOF = atof(fields[1]);
				}
				else if( strcmp( fields[0], "IO_apost_std_dev" ) == 0 )
				{
					pgenPhoto.IOApostStdDev = atof(fields[1]);
				}
				else if( strcmp( fields[0], "EO_parameters" ) == 0 )
				{
					for( int i = 0; i < 6; i++ )
						pgenPhoto.EOParams[i] = atof(fields[i+1]);
				}
				else if( strcmp( fields[0], "COMPUTED_std_devs" ) == 0 )
				{
					for( int i = 0; i < 6; i++ )
						pgenPhoto.computedStdDevs[i] = atof(fields[i+1]);
				}
				else
					printf( "Photo: %s - Field %s not handled\n", pgenPhoto.name, fields[0] );
			}
		}

		FreeTextToColumns( fields, numFields );
	}

	printf( "\n" );
	return TRUE;
}

int PGenPhotoToPM3Photo( PGenPhoto pgenPhoto )
{
	struct images theImage;
	memset( &theImage, 0, sizeof(theImage) );
	strcpy( theImage.szName, pgenPhoto.name );
	CString imageFile(pgenPhoto.imageID);
	int fnIndex = imageFile.ReverseFind('\\');
	if( fnIndex != -1 )
		imageFile = imageFile.Right(imageFile.GetLength() - fnIndex - 1);

	fnIndex = imageFile.ReverseFind('.');
	if( fnIndex != -1 )
		imageFile = imageFile.Left(fnIndex);
	imageFile += ".bmp";

	strcpy( theImage.szFileName, (LPCSTR)imageFile );

	// Calculate film correction parameters from the Interior Orientation
	double sx = sqrt(pgenPhoto.IOParams[2]*pgenPhoto.IOParams[2] + pgenPhoto.IOParams[3]*pgenPhoto.IOParams[3]);
	double sy = sqrt(pgenPhoto.IOParams[4]*pgenPhoto.IOParams[4] + pgenPhoto.IOParams[5]*pgenPhoto.IOParams[5]);

	theImage.dfFilmCorr[0] = pgenPhoto.IOParams[2] / (sx*sx);
	theImage.dfFilmCorr[1] = pgenPhoto.IOParams[4] / (sx*sy);
	theImage.dfFilmCorr[2] = -(theImage.dfFilmCorr[0]*pgenPhoto.IOParams[0] + theImage.dfFilmCorr[1]*pgenPhoto.IOParams[1]);

	theImage.dfFilmCorr[3] = pgenPhoto.IOParams[3] / (sx*sy);
	theImage.dfFilmCorr[4] = pgenPhoto.IOParams[5] / (sy*sy);
	theImage.dfFilmCorr[5] = -(theImage.dfFilmCorr[3]*pgenPhoto.IOParams[0] + theImage.dfFilmCorr[4]*pgenPhoto.IOParams[1]);

	// Calculate the image transformation matrix from the Exterior Orientation
	double pi = 3.1415926535;
	double RadPerDeg = pi / 180;
	double theta1 = pgenPhoto.EOParams[3] * RadPerDeg;
	double theta2 = pgenPhoto.EOParams[4] * RadPerDeg;
	double theta3 = pgenPhoto.EOParams[5] * RadPerDeg;

	VECTOR v1 = vector( 1, 12 );
	v1[1] = 1.0;		v1[2] = 0.0;			v1[3] = 0.0;
	v1[4] = 0.0;		v1[5] =  cos(theta1);	v1[6] = sin(theta1);
	v1[7] = 0.0;		v1[8] = -sin(theta1);	v1[9] = cos(theta1);
	v1[10] = 0.0;		v1[11] = 0.0;							v1[12] = 0.0;

	VECTOR v2 = vector( 1, 12 );
	v2[1] = cos(theta2);	v2[2] = 0.0;	v2[3] = -sin(theta2);
	v2[4] = 0.0;			v2[5] = 1.0;	v2[6] = 0.0;
	v2[7] = sin(theta2);	v2[8] = 0.0;	v2[9] = cos(theta2);
	v2[10] = 0.0;			v2[11] = 0.0;	v2[12] = 0.0;

	VECTOR v3 = vector( 1, 12 );
	v3[1] =  cos(theta3);	v3[2] = sin(theta3);	v3[3] = 0.0;
	v3[4] = -sin(theta3);	v3[5] = cos(theta3);	v3[6] = 0.0;
	v3[7] = 0.0;			v3[8] = 0.0;			v3[9] = 1.0;
	v3[10] = 0.0;			v3[11] = 0.0;			v3[12] = 0.0;

	VECTOR v4 = matmat( v2,v1 );
	VECTOR v5 = matmat( v3,v4 );

	VECTOR v6 = vector( 1, 3 );
	v6[1] = 1000.0 * pgenPhoto.EOParams[0];
	v6[2] = 1000.0 * pgenPhoto.EOParams[1];
	v6[3] = 1000.0 * pgenPhoto.EOParams[2];

	VECTOR v7 = matpnt( v5, v6 );

	v5[10] = -v7[1];
	v5[11] = -v7[2];
	v5[12] = -v7[3];

	VECTOR v8 = vector( 1, 12 );
	v8[1] = 1.0;		v8[2] = 0.0;		v8[3] = 0.0;
	v8[4] = 0.0;		v8[5] = 0.0;		v8[6] = -1.0;
	v8[7] = 0.0;		v8[8] = 1.0;		v8[9] = 0.0;
	v8[10] = 1.0;		v8[11] = 0.0;		v8[12] = 0.0;

	VECTOR v9 = matmat( v5, v8 );

	// Find our camera first
	dt_keyfind( CAMERA_INFO_SZID, &(pgenPhoto.cameraName), &dbTask, dbn );
	dt_setor( CAMERA_INFO_IMAGES, &dbTask, dbn );
	// Save the image
	int dbStatus = dt_fillnew( IMAGES, &theImage, &dbTask, dbn );
	if( dbStatus == S_OKAY )
	{
		dt_connect( CAMERA_INFO_IMAGES, &dbTask, dbn );
		struct bundle theBundle;
		memset( &theBundle, 0, sizeof(theBundle) );
		strcpy( theBundle.szTag, "BUNDLE" );

		dt_setor( IMAGES_TMATRIX, &dbTask, dbn );

		dbStatus = dt_keyfind( BUNDLE_SZTAG, &(theBundle.szTag), &dbTask, dbn );
		if( dbStatus == S_NOTFOUND )
			dt_fillnew( BUNDLE, &theBundle, &dbTask, dbn );
		dt_setor( BUNDLE_TMATRIX, &dbTask, dbn );
		struct tmatrix theMatrix;
		memset( &theMatrix, 0, sizeof(theMatrix) );
		for( int i = 0; i < 12; i++ )
			theMatrix.dfCameraPos[i] = v9[i+1];

		dt_fillnew( TMATRIX, &theMatrix, &dbTask, dbn );
		dt_connect( IMAGES_TMATRIX, &dbTask, dbn );
		dt_connect( BUNDLE_TMATRIX, &dbTask, dbn );
	}
	else
		printf( "Error(%d) creating image '%s'.\n", dbStatus, theImage.szName );

	return TRUE;
}

int projectFunc( int dbn, CStdioFile &file )
{
	return TRUE;
}

int triangFunc( int dbn, CStdioFile &file )
{
	return TRUE;
}
