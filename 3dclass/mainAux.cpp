#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

extern "C" {
#include "aux.h"
}

GLfloat aspectRatio = 1.0;
double latitude = 0.0;
double longitude = 0.0;

void handleResize( int w, int h )
{
    glViewport( 0, 0, w, h );
    aspectRatio = (GLfloat)w/(GLfloat)h;
}

void handleUpKey( void )
{
    latitude -= 5.0;
}

void handleDownKey( void )
{
    latitude += 5.0;
}

void handleLeftKey( void )
{
    longitude -= 5.0;
}

void handleRightKey( void )
{
    longitude += 5.0;
}

void drawScene( void )
{
    GLfloat m[16];
fprintf( stderr, "latitude, longitude = %lf, %lf\n", latitude, longitude );
    glEnable( GL_DEPTH_TEST );

    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    /* Load camera parameters */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -500.0, 500.0, -500.0, 500.0, 0.0, 10000.0 );
    glMatrixMode( GL_MODELVIEW );

    glTranslated( 0.0, 0.0, -1000.0 );
    glRotated(-latitude, 1.0, 0.0, 0.0);
    glRotated(longitude, 0.0, 0.0, 1.0);	 

glGetFloatv( GL_MODELVIEW_MATRIX, m );
for( int i = 0; i < 16; i++ )
    fprintf( stderr, "m[%d] = %f\n", i, m[i] );
    /* Draw objects */
    glColor3f( 1.0, 0.0, 0.0 );
    auxSolidBox( 100.0, 100.0, 100.0 );

    glColor3f( 0.0, 1.0, 0.0 );
    glTranslatef( 200.0, 0.0, 0.0 );
    auxSolidBox( 200.0, 200.0, 200.0 );

    glFlush();
}

int main(int argc, char** argv)
{
    auxInitDisplayMode( AUX_SINGLE | AUX_RGB | AUX_DIRECT | AUX_DEPTH );
    auxInitPosition( 0, 0, 400, 400 );
    auxInitWindow( argv[0] );

    auxReshapeFunc( handleResize );

    auxKeyFunc( AUX_UP, handleUpKey );
    auxKeyFunc( AUX_DOWN, handleDownKey );
    auxKeyFunc( AUX_LEFT, handleLeftKey );
    auxKeyFunc( AUX_RIGHT, handleRightKey );

    auxMainLoop( drawScene );
}
