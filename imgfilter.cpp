#include "stdafx.h"

#include "photog.h"
#include "common.h"
//#include "ptgGlobals.h"
#include "output.h"
#include "imgfilter.h"

//void histogramEqualizeImage( unsigned long *ulnImage, long width, long height );
//void sharpenImage( unsigned long *ulnImage, long width, long height );
//void robertsEdgeFilterImage( unsigned long *ulnImage, long width, long height );
//void brightnessContrastFilterImage( unsigned long *ulnImage, long width, long height, int brightness, double contrast );

int brightness = 0;
double contrast = 1.0;

#define FUNC "filterZoomImage()"
void filterZoomImage( unsigned long *ulnImage, long width, long height, int color_depth )
{
    if( zoomImageFilterFlags & FILTER_BRIGHTNESS_CONTRAST )
		if( color_depth == 16 )
			brightnessContrastFilterImage( ulnImage, width, height, brightness, contrast );
//    if( zoomImageFilterFlags & FILTER_HISTEQ )
		if( color_depth == 16)
			histogramEqualizeImage( (unsigned short *)ulnImage, width, height );    
//    if( zoomImageFilterFlags & FILTER_SHARPEN )
//	sharpenImage( ulnImage, width, height );
  //  if( zoomImageFilterFlags & FILTER_ROBERTS_EDGE )
	//robertsEdgeFilterImage( ulnImage, width, height );
}
#undef FUNC

#define FUNC "histogramEqualizeImage()"
void histogramEqualizeImage( unsigned long *ulnImage, long width, long height )
{
    char minPixel = 255;
    char maxPixel = 0;
    char *currPos;
    int i, j;

    currPos = (char *)ulnImage;
    for( i = 0; i < height; i++ )
    {
		for( j = 0; j < width; j++ )
		{
		    if( minPixel > *currPos )
				minPixel = *currPos;
		    if( maxPixel < *currPos )
				maxPixel = *currPos;
		    currPos += 4;
		}
    }

    /* Make sure there is some variation in the image */
    if( maxPixel == minPixel )
		return;
	
    double scale = 255.0 / ((double)(maxPixel - minPixel));
    double offset = -scale*((double)minPixel);

    DebugOutput( 0, "pixel range = %d - %d\n", (int)minPixel, (int)maxPixel );
    DebugOutput( 0, "scale = %lf\n", scale );
    DebugOutput( 0, "offset = %lf\n", offset );

    currPos = (char *)ulnImage;
    for( i = 0; i < height; i++ )
    {
		for( j = 0; j < width; j++ )
		{
			char currPixel = (char)((double)(*currPos) * scale + offset);
			currPos[0] = currPixel;
			currPos[1] = currPixel;
			currPos[2] = currPixel;
			currPos += 4;
		}
    }    
    
}
#undef FUNC
void histogramEqualizeImage( unsigned short *ulnImage, long width, long height )
{
    char minPixel = 255;
    char maxPixel = 0;
    char *currPos;
    int i, j;

    currPos = (char *)ulnImage;
    for( i = 0; i < height; i++ )
    {
		for( j = 0; j < width; j++ )
		{
			DWORD p;
			p = PALETTEINDEX(*currPos);
			char q = ((char *)&p)[3];
			
		    if( minPixel > q)
				minPixel = q;
		    if( maxPixel < q)
				maxPixel = q;
		    currPos += 2;
		}
    }

    /* Make sure there is some variation in the image */
    if( maxPixel == minPixel )
		return;
	
    double scale = 255.0 / ((double)(maxPixel - minPixel));
    double offset = -scale*((double)minPixel);

    DebugOutput( 0, "pixel range = %d - %d\n", (int)minPixel, (int)maxPixel );
    DebugOutput( 0, "scale = %lf\n", scale );
    DebugOutput( 0, "offset = %lf\n", offset );

    currPos = (char *)ulnImage;
    for( i = 0; i < height; i++ )
    {
		for( j = 0; j < width; j++ )
		{
			DWORD p;
			p = PALETTEINDEX(*currPos);
			char q = ((char *)&p)[3];

			char currPixel = (char)((double)(q) * scale + offset);
			currPos[0] = RGB(currPixel,currPixel,currPixel);
			currPos[1] = currPixel<<4|currPos[1]&0x0F;
			//currPos[2] = currPixel;
			currPos += 2;
		}
    }    
    
}

#define FUNC "brightnessContrastFilterImage()"
void brightnessContrastFilterImage( unsigned long *ulnImage, long width, long height, int brightness, double contrast )
{
    char *currPos;
    int i, j;

    currPos = (char *)ulnImage;
    for( i = 0; i < height; i++ )
    {
		for( j = 0; j < width; j++ )
		{
			double pixmax = 255.0;
			
			double pixval = (double)(currPos[0]);
			pixval = (pixval + (double)brightness)*contrast + (pixmax/2.0)*(1 - contrast);
			
			if( pixval > pixmax )
				pixval = pixmax;
			else if( pixval < 0.0 )
				pixval = 0.0;
			currPos[0] = (unsigned char)pixval;
			currPos[1] = (unsigned char)pixval;
			currPos[2] = (unsigned char)pixval;

			currPos += 4;
		}
    }
}
#undef FUNC

