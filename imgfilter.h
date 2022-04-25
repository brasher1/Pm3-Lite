#ifndef __IMGFILTER_H__
#define __IMGFILTER_H__

#define FILTER_NONE		    0x00000000
#define FILTER_HISTEQ		    0x00000001
#define FILTER_SHARPEN		    0x00000002
#define FILTER_ROBERTS_EDGE	    0x00000004
#define FILTER_BRIGHTNESS_CONTRAST  0x00000008
#define FILTER_ALL		    0xffffffff

void filterZoomImage( unsigned long *ulnImage, long width, long height, int color_depth );
void histogramEqualizeImage( unsigned long *ulnImage, long width, long height );
void histogramEqualizeImage( unsigned short *ulnImage, long width, long height );
void brightnessContrastFilterImage( unsigned long *ulnImage, long width, long height, int brightness, double contrast );

#endif //__IMGFILTER_H__