#ifndef ___DRAWUTILS_H___
#define ___DRAWUTILS_H___
 

struct _Point3d {
    double x;
    double y;
    double z;
};

struct Line2p {
    _Point3d pt1;
    _Point3d pt2;
};

struct Plane3p {
    _Point3d pt1;
    _Point3d pt2;
    _Point3d pt3;
};

struct Ray {
    _Point3d pt1;
    _Point3d pt2;
};

int drawLine(struct Line2p *);
int drawPlane(struct Plane3p *);
int drawPoint(struct _Point3d *);




#endif //___DRAWUTILS_H___
