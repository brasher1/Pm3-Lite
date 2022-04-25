#include "PtgCamera.h"

PtgCamera::PtgCamera()
{
    initVars();
}

PtgCamera::~PtgCamera()
{
    free_vector( transform, 1, 12 );
}

void PtgCamera::initVars()
{
    focalLength = 65.0;
    aspectRatio = 1.0;
    fieldOfViewY = 50.0;
    nearClipDist = 1.0;
    farClipDist = 1000.0;
    viewType = VT_ORTHO;
    transform = vector( 1, 12 );
    
    lookx = looky = lookz = 0.0;
    alpha = beta = gamma = 0.0;
}

void PtgCamera::setPosition( const double x, const double y, const double z )
{
    transform[10] = x;
    transform[11] = y;
    transform[12] = z;
}

void PtgCamera::setLookAt( const double x, const double y, const double z )
{
    lookx = x;
    looky = y;
    lookz = z;
}

void PtgCamera::setAspectRatio( const double r )
{
    aspectRatio = r;
}

void PtgCamera::setFieldOfViewY( const double fovy )
{
    fieldOfViewY = fovy;
}

void PtgCamera::movePosBy( const double x, const double y, const double z )
{
    transform[10] += x;
    transform[11] += y;
    transform[12] += z;
}

void PtgCamera::setClipping( const double n, const double f )
{
    nearClipDist = n;
    farClipDist = f;
}

void PtgCamera::setOrthoClip( const double left, const double right, const double bottom, const double top )
{
    leftClip = left;
    rightClip = right;
    bottomClip = bottom;
    topClip = top;
}

/* Temporary solution to rotating the camera */
void PtgCamera::setDirection( const double x, const double y, const double z )
{
    transform[1] = x;
    transform[2] = y;
    transform[3] = z;
}

/* Temporary solution to rotating the camera */
void PtgCamera::setUpDirection( const double x, const double y, const double z )
{
    transform[4] = x;
    transform[5] = y;
    transform[6] = z;
}

void PtgCamera::setRotation( const double x, const double y, const double z )
{
    alpha = x;
    beta = y;
    gamma = z;
}
