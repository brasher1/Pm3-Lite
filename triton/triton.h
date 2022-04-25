#ifndef TRITON_H
#define TRITON_H

/* Raima Database Manager Version 3.30 */


/* database triton record/key structure declarations */

struct Transform {
   double a[16];
};

struct Classes {
   long type;
   char name[32];
   char description[64];
};

struct Groups {
   char name[32];
   char description[64];
};

struct Properties {
   struct {
      float red;
      float green;
      float blue;
   } color;
   struct {
      double x1;
      double y1;
      double z1;
      double x2;
      double y2;
      double z2;
   } range;
};

struct Box {
   double xlen;
   double ylen;
   double zlen;
   long index;
};

struct CirTorus {
   double outerRadius;
   double innerRadius;
   double angle;
   long index;
};

struct Cone {
   double topRadius;
   double bottomRadius;
   double height;
   double xOffset;
   double yOffset;
   long index;
};

struct Cylinder {
   double radius;
   double height;
   long index;
};

struct RectTorus {
   double outerRadius;
   double innerRadius;
   double height;
   double angle;
   long index;
};

struct Sphere {
   double diameter;
   double height;
   double radius;
   long index;
};

struct Pyramid {
   double height;
   double baseWidth;
   double BaseDepth;
   double TopWidth;
   double TopDepth;
   double WidthOffset;
   double DepthOffset;
   long index;
};

struct Complex {
   long num_faces;
   long index;
};

struct Faces {
   long num_vertices;
};

struct Vertices {
   double xpos;
   double ypos;
   double zpos;
};

struct Point {
   double xpos;
   double ypos;
   double zpos;
   long index;
};

struct Line {
   double x1;
   double y1;
   double z1;
   double x2;
   double y2;
   double z2;
   long index;
};

/* record, field and set table entry definitions */

/* File Id Constants */
#define TRITON01 0
#define TRITON02 1
#define TRITON03 2
#define TRITON04 3
#define TRITON05 4
#define TRITON06 5
#define TRITON08 6
#define TRITON09 7
#define TRITON10 8
#define TRITON11 9
#define TRITON12 10
#define TRITON13 11
#define TRITON14 12
#define TRITON15 13
#define TRITON16 14
#define TRITON17 15
#define TRITON18 16
#define TRITON51 17
#define TRITON52 18
#define TRITON53 19

/* Record Name Constants */
#define TRANSFORM 10000
#define CLASSES 10001
#define GROUPS 10002
#define PROPERTIES 10003
#define BOX 10004
#define CIRTORUS 10005
#define CONE 10006
#define CYLINDER 10007
#define RECTTORUS 10008
#define SPHERE 10009
#define PYRAMID 10010
#define COMPLEX 10011
#define FACES 10012
#define VERTICES 10013
#define POINT_REC 10014
#define LINE_REC 10015

/* Field Name Constants */
#define TRANSFORM_A 0L
#define CLASSES_TYPE 1000L
#define CLASSES_NAME 1001L
#define CLASSES_DESCRIPTION 1002L
#define GROUPS_NAME 2000L
#define GROUPS_DESCRIPTION 2001L
#define PROPERTIES_COLOR 3000L
#define PROPERTIES_RED 3001L
#define PROPERTIES_GREEN 3002L
#define PROPERTIES_BLUE 3003L
#define PROPERTIES_RANGE 3004L
#define PROPERTIES_X1 3005L
#define PROPERTIES_Y1 3006L
#define PROPERTIES_Z1 3007L
#define PROPERTIES_X2 3008L
#define PROPERTIES_Y2 3009L
#define PROPERTIES_Z2 3010L
#define BOX_XLEN 4000L
#define BOX_YLEN 4001L
#define BOX_ZLEN 4002L
#define BOX_INDEX 4003L
#define CIRTORUS_OUTERRADIUS 5000L
#define CIRTORUS_INNERRADIUS 5001L
#define CIRTORUS_ANGLE 5002L
#define CIRTORUS_INDEX 5003L
#define CONE_TOPRADIUS 6000L
#define CONE_BOTTOMRADIUS 6001L
#define CONE_HEIGHT 6002L
#define CONE_XOFFSET 6003L
#define CONE_YOFFSET 6004L
#define CONE_INDEX 6005L
#define CYLINDER_RADIUS 7000L
#define CYLINDER_HEIGHT 7001L
#define CYLINDER_INDEX 7002L
#define RECTTORUS_OUTERRADIUS 8000L
#define RECTTORUS_INNERRADIUS 8001L
#define RECTTORUS_HEIGHT 8002L
#define RECTTORUS_ANGLE 8003L
#define RECTTORUS_INDEX 8004L
#define SPHERE_DIAMETER 9000L
#define SPHERE_HEIGHT 9001L
#define SPHERE_RADIUS 9002L
#define SPHERE_INDEX 9003L
#define PYRAMID_HEIGHT 10000L
#define PYRAMID_BASEWIDTH 10001L
#define PYRAMID_BASEDEPTH 10002L
#define PYRAMID_TOPWIDTH 10003L
#define PYRAMID_TOPDEPTH 10004L
#define PYRAMID_WIDTHOFFSET 10005L
#define PYRAMID_DEPTHOFFSET 10006L
#define PYRAMID_INDEX 10007L
#define COMPLEX_NUM_FACES 11000L
#define COMPLEX_INDEX 11001L
#define FACES_NUM_VERTICES 12000L
#define VERTICES_XPOS 13000L
#define VERTICES_YPOS 13001L
#define VERTICES_ZPOS 13002L
#define POINT_XPOS 14000L
#define POINT_YPOS 14001L
#define POINT_ZPOS 14002L
#define POINT_INDEX 14003L
#define LINE_X1 15000L
#define LINE_Y1 15001L
#define LINE_Z1 15002L
#define LINE_X2 15003L
#define LINE_Y2 15004L
#define LINE_Z2 15005L
#define LINE_INDEX 15006L

/* Set Name Constants */
#define WORLD_CLASSES 20000
#define CLASSES_CLASSES 20001
#define ELEMENT_CLASSES 20002
#define TRANSFORMS 20003
#define GROUPS_LIST 20004
#define PROPERTIES_SET 20005
#define COMP_FACES 20006
#define FACES_VERTICES 20007

/* Field Sizes */
#define SIZEOF_TRANSFORM_A 128
#define SIZEOF_CLASSES_TYPE 4
#define SIZEOF_CLASSES_NAME 32
#define SIZEOF_CLASSES_DESCRIPTION 64
#define SIZEOF_GROUPS_NAME 32
#define SIZEOF_GROUPS_DESCRIPTION 64
#define SIZEOF_PROPERTIES_RED 4
#define SIZEOF_PROPERTIES_GREEN 4
#define SIZEOF_PROPERTIES_BLUE 4
#define SIZEOF_PROPERTIES_X1 8
#define SIZEOF_PROPERTIES_Y1 8
#define SIZEOF_PROPERTIES_Z1 8
#define SIZEOF_PROPERTIES_X2 8
#define SIZEOF_PROPERTIES_Y2 8
#define SIZEOF_PROPERTIES_Z2 8
#define SIZEOF_BOX_XLEN 8
#define SIZEOF_BOX_YLEN 8
#define SIZEOF_BOX_ZLEN 8
#define SIZEOF_BOX_INDEX 4
#define SIZEOF_CIRTORUS_OUTERRADIUS 8
#define SIZEOF_CIRTORUS_INNERRADIUS 8
#define SIZEOF_CIRTORUS_ANGLE 8
#define SIZEOF_CIRTORUS_INDEX 4
#define SIZEOF_CONE_TOPRADIUS 8
#define SIZEOF_CONE_BOTTOMRADIUS 8
#define SIZEOF_CONE_HEIGHT 8
#define SIZEOF_CONE_XOFFSET 8
#define SIZEOF_CONE_YOFFSET 8
#define SIZEOF_CONE_INDEX 4
#define SIZEOF_CYLINDER_RADIUS 8
#define SIZEOF_CYLINDER_HEIGHT 8
#define SIZEOF_CYLINDER_INDEX 4
#define SIZEOF_RECTTORUS_OUTERRADIUS 8
#define SIZEOF_RECTTORUS_INNERRADIUS 8
#define SIZEOF_RECTTORUS_HEIGHT 8
#define SIZEOF_RECTTORUS_ANGLE 8
#define SIZEOF_RECTTORUS_INDEX 4
#define SIZEOF_SPHERE_DIAMETER 8
#define SIZEOF_SPHERE_HEIGHT 8
#define SIZEOF_SPHERE_RADIUS 8
#define SIZEOF_SPHERE_INDEX 4
#define SIZEOF_PYRAMID_HEIGHT 8
#define SIZEOF_PYRAMID_BASEWIDTH 8
#define SIZEOF_PYRAMID_BASEDEPTH 8
#define SIZEOF_PYRAMID_TOPWIDTH 8
#define SIZEOF_PYRAMID_TOPDEPTH 8
#define SIZEOF_PYRAMID_WIDTHOFFSET 8
#define SIZEOF_PYRAMID_DEPTHOFFSET 8
#define SIZEOF_PYRAMID_INDEX 4
#define SIZEOF_COMPLEX_NUM_FACES 4
#define SIZEOF_COMPLEX_INDEX 4
#define SIZEOF_FACES_NUM_VERTICES 4
#define SIZEOF_VERTICES_XPOS 8
#define SIZEOF_VERTICES_YPOS 8
#define SIZEOF_VERTICES_ZPOS 8
#define SIZEOF_POINT_XPOS 8
#define SIZEOF_POINT_YPOS 8
#define SIZEOF_POINT_ZPOS 8
#define SIZEOF_POINT_INDEX 4
#define SIZEOF_LINE_X1 8
#define SIZEOF_LINE_Y1 8
#define SIZEOF_LINE_Z1 8
#define SIZEOF_LINE_X2 8
#define SIZEOF_LINE_Y2 8
#define SIZEOF_LINE_Z2 8
#define SIZEOF_LINE_INDEX 4

#endif    /* TRITON_H */
