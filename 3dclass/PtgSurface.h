#ifndef ___PtgSurface_h___
#define ___PtgSurface_h___

#define rad .7071067811865 /*  square root of 2 all divided by 2  */

#define PI          3.1415926535897932384626433
#define DEGRAD(a)   (a*(PI/180))
#define RADDEG(a)   (a*(180/PI))

static GLfloat knots1[] = { 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4 };
static GLfloat knots2[] = { 0, 0, 0, 1, 1, 2, 2, 3, 3, 3 };
static GLfloat knots3[] = { 0, 0, 0, 1, 1, 2, 2, 2 };
static GLfloat knots4[] = { 0, 0, 0, 1, 1, 1 };
static GLfloat knots5[] = { 0, 0, 1, 1 };
static GLfloat knots6[] = { 0, 0, 1, 2, 3, 3 };

static GLfloat vknots[]={ 0, 0, 1, 2, 3, 4, 4 };

static double w1[9] = { 1, rad, 0, -rad, -1, -rad, 0, rad, 1 };
static double w2[9] = { 0, rad, 1, rad, 0, -rad, -1, -rad, 0 };

static double wx[9] = { 1, 1, 0, -1, -1, -1, 0, 1, 1 };  
static double wy[9] = { 0, 1, 1, 1, 0, -1, -1, -1, 0 };

/**********************************************************************************/

#endif	//#ifndef ___PtgSurface_h___
