#ifndef TRITON_HPP
#define TRITON_HPP

#include <storedb.hpp>
#include <storeobj.hpp>
#include <keyobj.hpp>
#include "poly.hpp"

#include "triton_defines.h"
#include "triton.h"

#ifndef REC_TYPE
#define REC_TYPE int    /* Required to support RDM 3.21A */
#endif /* REC_TYPE */

/* Raima Database Manager Version 3.30 */


/* record, field and set table entry definitions */
#ifdef _BOBOBOBOB_
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
#define POINT 10014
#define LINE 10015

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
#define PROPERTIES 20005
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
#endif

/* database triton class declarations */
#ifdef TRITON_DLL
#define TRITON_PORTING		__declspec(dllexport)
#else
#define TRITON_PORTING		__declspec(dllimport)
#endif

class OM_EXPORT triton : public StoreDb
{
public:
   triton() : StoreDb("triton", PDB_LOCATOR) {}

   void OpenMe()
   {
	Open();
   }
  
   DEFINE_DB_LOCATOR;
};

OMCLASS(TransformObj)
OMCLASS(ClassesObj)
OMCLASS(GroupsObj)
OMCLASS(PropertiesObj)
OMCLASS(BoxObj)
OMCLASS(CirTorusObj)
OMCLASS(ConeObj)
OMCLASS(CylinderObj)
OMCLASS(RectTorusObj)
OMCLASS(SphereObj)
OMCLASS(PyramidObj)
OMCLASS(ComplexObj)
OMCLASS(FacesObj)
OMCLASS(VerticesObj)
OMCLASS(PointObj)
OMCLASS(LineObj)
OMCLASS(SystemObj)
OMCLASS(Classes_type_KeyObj)
OMCLASS(Classes_name_KeyObj)
OMCLASS(Groups_name_KeyObj)

class OM_EXPORT TransformObj : public StoreObj, public Transform
{
public:
	TransformObj() {}
	virtual ~TransformObj() {}
	REC_TYPE RecType() { return TRANSFORM; }
	Pchar OMLOADDS RecName() { return "Transform"; }

	Pvoid RecData() { return StoreObj::RecData(); }
	long RecDataSize() { return sizeof(struct Transform); }
	void Prune( T_F deleteChildren, T_F cleanOrphanedParents );

	STOREDIN(triton);
	CUSTOMOPS(TransformObj);
	OWNEROF(ClassesObj, TRANSFORMS);
};


class OM_EXPORT ClassesObj : public StoreObj, public Classes
{
public:
   ClassesObj() {}
   ClassesObj(RSetObj set) : StoreObj(set) {}
   ClassesObj(RKeyObj key) : StoreObj(key) {}
   ClassesObj(RSystemObj) : StoreObj(SetObj(WORLD_CLASSES)) {}
   ClassesObj(RClassesObj) : StoreObj(SetObj(CLASSES_CLASSES)) {}
   ClassesObj(RTransformObj) : StoreObj(SetObj(TRANSFORMS)) {}
   ClassesObj(RGroupsObj) : StoreObj(SetObj(GROUPS_LIST)) {}
   ClassesObj(RPropertiesObj) : StoreObj(SetObj(PROPERTIES_SET)) {}
   virtual ~ClassesObj() {}
   REC_TYPE RecType() { return CLASSES; }
   Pchar OMLOADDS RecName() { return "Classes"; }

   Pvoid RecData() { return StoreObj::RecData(); }
   long RecDataSize() { return sizeof(struct Classes); }
   void Prune(T_F deleteChildren, T_F cleanOrphanedParents);

   STOREDIN(triton);
   CUSTOMOPS(ClassesObj);
   OWNEROF(ClassesObj, CLASSES_CLASSES);
   OWNEROF(ConeObj, ELEMENT_CLASSES);
   OWNEROF(CylinderObj, ELEMENT_CLASSES);
   OWNEROF(SphereObj, ELEMENT_CLASSES);
   OWNEROF(BoxObj, ELEMENT_CLASSES);
   OWNEROF(ComplexObj, ELEMENT_CLASSES);
   OWNEROF(CirTorusObj, ELEMENT_CLASSES);
   OWNEROF(RectTorusObj, ELEMENT_CLASSES);
   OWNEROF(PyramidObj, ELEMENT_CLASSES);
   OWNEROF(LineObj, ELEMENT_CLASSES);
   OWNEROF(PointObj, ELEMENT_CLASSES);
   MEMBEROF(SystemObj, WORLD_CLASSES);
   MEMBEROF(ClassesObj, CLASSES_CLASSES);
   MEMBEROF(TransformObj, TRANSFORMS);
   MEMBEROF(GroupsObj, GROUPS_LIST);
   MEMBEROF(PropertiesObj, PROPERTIES_SET);
};


class OM_EXPORT GroupsObj : public StoreObj, public Groups
{
public:
	GroupsObj() {}
	GroupsObj(RKeyObj key) : StoreObj(key) {}
	virtual ~GroupsObj() {}
	REC_TYPE RecType() { return GROUPS; }
	Pchar OMLOADDS RecName() { return "Groups"; }

	Pvoid RecData() { return StoreObj::RecData(); }
	long RecDataSize() { return sizeof(struct Groups); }
	void Prune( T_F deleteChildren, T_F cleanOrphanedParents );

	STOREDIN(triton);
	CUSTOMOPS(GroupsObj);
	OWNEROF(ClassesObj, GROUPS_LIST);
};


class OM_EXPORT PropertiesObj : public StoreObj, public Properties
{
public:
	PropertiesObj() {}
	virtual ~PropertiesObj() {}
	REC_TYPE RecType() { return PROPERTIES; }
	Pchar OMLOADDS RecName() { return "Properties"; }

	Pvoid RecData() { return StoreObj::RecData(); }
	long RecDataSize() { return sizeof(struct Properties); }
	void Prune( T_F deleteChildren, T_F cleanOrphanedParents );

	STOREDIN(triton);
	CUSTOMOPS(PropertiesObj);
	OWNEROF(ClassesObj, PROPERTIES_SET);
};

class OM_EXPORT ElementObj: public StoreObj
{
private:
	int mode;
	char dbastr[25];
public:
	ElementObj():StoreObj(SetObj(ELEMENT_CLASSES)){}

	virtual long RecDataSize() = 0;

	Pvoid RecData() {return (void *)((char *)this+sizeof(*this));}

	STOREDIN(triton);
};

class OM_EXPORT BoxObj : public ElementObj, public Box
{
public:
   BoxObj() {}
//   BoxObj(RSetObj set) : StoreObj(set) {}
   virtual ~BoxObj() {}
   REC_TYPE RecType() { return BOX; }
   Pchar OMLOADDS RecName() { return "Box"; }

//   Pvoid RecData() { return StoreObj::RecData(); }
   long RecDataSize() { return sizeof(struct Box); }

   STOREDIN(triton);
   CUSTOMOPS(BoxObj);
   MEMBEROF(ClassesObj, ELEMENT_CLASSES);
};


class OM_EXPORT CirTorusObj : public ElementObj, public CirTorus
{
public:
   CirTorusObj() {}
//   CirTorusObj(RSetObj set) : StoreObj(set) {}
   virtual ~CirTorusObj() {}
   REC_TYPE RecType() { return CIRTORUS; }
   Pchar OMLOADDS RecName() { return "CirTorus"; }

//   Pvoid RecData() { return StoreObj::RecData(); }
   long RecDataSize() { return sizeof(struct CirTorus); }

   STOREDIN(triton);
   CUSTOMOPS(CirTorusObj);
   MEMBEROF(ClassesObj, ELEMENT_CLASSES);
};


class OM_EXPORT ConeObj : public ElementObj, public Cone
{
public:
   ConeObj() {}
//   ConeObj(RSetObj set) : StoreObj(set) {}
   virtual ~ConeObj() {}
   REC_TYPE RecType() { return CONE; }
   Pchar OMLOADDS RecName() { return "Cone"; }

//   Pvoid RecData() { return StoreObj::RecData(); }
   long RecDataSize() { return sizeof(struct Cone); }

   STOREDIN(triton);
   CUSTOMOPS(ConeObj);
   MEMBEROF(ClassesObj, ELEMENT_CLASSES);
};


class OM_EXPORT CylinderObj : public ElementObj, public Cylinder
{
public:
   CylinderObj() {}
//   CylinderObj(RSetObj set) : StoreObj(set) {}
   virtual ~CylinderObj() {}
   REC_TYPE RecType() { return CYLINDER; }
   Pchar OMLOADDS RecName() { return "Cylinder"; }

//   Pvoid RecData() { return StoreObj::RecData(); }
   long RecDataSize() { return sizeof(struct Cylinder); }

   STOREDIN(triton);
   CUSTOMOPS(CylinderObj);
   MEMBEROF(ClassesObj, ELEMENT_CLASSES);
};


class OM_EXPORT RectTorusObj : public ElementObj, public RectTorus
{
public:
   RectTorusObj() {}
//   RectTorusObj(RSetObj set) : StoreObj(set) {}
   virtual ~RectTorusObj() {}
   REC_TYPE RecType() { return RECTTORUS; }
   Pchar OMLOADDS RecName() { return "RectTorus"; }

//   Pvoid RecData() { return StoreObj::RecData(); }
   long RecDataSize() { return sizeof(struct RectTorus); }

   STOREDIN(triton);
   CUSTOMOPS(RectTorusObj);
   MEMBEROF(ClassesObj, ELEMENT_CLASSES);
};


class OM_EXPORT SphereObj : public ElementObj, public Sphere
{
public:
   SphereObj() {}
//   SphereObj(RSetObj set) : StoreObj(set) {}
   virtual ~SphereObj() {}
   REC_TYPE RecType() { return SPHERE; }
   Pchar OMLOADDS RecName() { return "Sphere"; }

//   Pvoid RecData() { return StoreObj::RecData(); }
   long RecDataSize() { return sizeof(struct Sphere); }

   STOREDIN(triton);
   CUSTOMOPS(SphereObj);
   MEMBEROF(ClassesObj, ELEMENT_CLASSES);
};


class OM_EXPORT PyramidObj : public ElementObj, public Pyramid
{
public:
   PyramidObj() {}
//   PyramidObj(RSetObj set) : StoreObj(set) {}
   virtual ~PyramidObj() {}
   REC_TYPE RecType() { return PYRAMID; }
   Pchar OMLOADDS RecName() { return "Pyramid"; }

//   Pvoid RecData() { return StoreObj::RecData(); }
   long RecDataSize() { return sizeof(struct Pyramid); }

   STOREDIN(triton);
   CUSTOMOPS(PyramidObj);
   MEMBEROF(ClassesObj, ELEMENT_CLASSES);
};


class OM_EXPORT ComplexObj : public ElementObj, public Complex
{
public:
   ComplexObj() {}
//   ComplexObj(RSetObj set) : StoreObj(set) {}
   virtual ~ComplexObj() {}
   REC_TYPE RecType() { return COMPLEX; }
   Pchar OMLOADDS RecName() { return "Complex"; }

//   Pvoid RecData() { return StoreObj::RecData(); }
   long RecDataSize() { return sizeof(struct Complex); }

   STOREDIN(triton);
   CUSTOMOPS(ComplexObj);
   OWNEROF(FacesObj, COMP_FACES);
   MEMBEROF(ClassesObj, ELEMENT_CLASSES);
};


class OM_EXPORT FacesObj : public ElementObj, public Faces
{
public:
   FacesObj() {}
//   FacesObj(RSetObj set) : StoreObj(set) {}
   virtual ~FacesObj() {}
   REC_TYPE RecType() { return FACES; }
   Pchar OMLOADDS RecName() { return "Faces"; }

//   Pvoid RecData() { return StoreObj::RecData(); }
   long RecDataSize() { return sizeof(struct Faces); }

   STOREDIN(triton);
   CUSTOMOPS(FacesObj);
   OWNEROF(VerticesObj, FACES_VERTICES);
   MEMBEROF(ComplexObj, COMP_FACES);
};


class OM_EXPORT VerticesObj : public ElementObj, public Vertices
{
public:
   VerticesObj() {}
//   VerticesObj(RSetObj set) : StoreObj(set) {}
   virtual ~VerticesObj() {}
   REC_TYPE RecType() { return VERTICES; }
   Pchar OMLOADDS RecName() { return "Vertices"; }

//   Pvoid RecData() { return StoreObj::RecData(); }
   long RecDataSize() { return sizeof(struct Vertices); }

   STOREDIN(triton);
   CUSTOMOPS(VerticesObj);
   MEMBEROF(FacesObj, FACES_VERTICES);
};


class OM_EXPORT PointObj : public ElementObj, public Point
{
public:
   PointObj() {}
//   PointObj(RSetObj set) : StoreObj(set) {}
   virtual ~PointObj() {}
   REC_TYPE RecType() { return POINT_REC; }
   Pchar OMLOADDS RecName() { return "Point"; }

//   Pvoid RecData() { return StoreObj::RecData(); }
   long RecDataSize() { return sizeof(struct Point); }

   STOREDIN(triton);
   CUSTOMOPS(PointObj);
   MEMBEROF(ClassesObj, ELEMENT_CLASSES);
};


class OM_EXPORT LineObj : public ElementObj, public Line
{
public:
   LineObj() {}
//   LineObj(RSetObj set) : StoreObj(set) {}
   virtual ~LineObj() {}
    REC_TYPE RecType() { return LINE_REC; }
   Pchar OMLOADDS RecName() { return "Line"; }

//   Pvoid RecData() { return StoreObj::RecData(); }
   long RecDataSize() { return sizeof(struct Line); }

   STOREDIN(triton);
   CUSTOMOPS(LineObj);
   MEMBEROF(ClassesObj, ELEMENT_CLASSES);
};


class OM_EXPORT SystemObj : public StoreObj
{
public:
   SystemObj() {}
   REC_TYPE RecType() { return SYSTEM; }
   Pchar OMLOADDS RecName() { return "System"; }

   STOREDIN(triton);
   OWNEROF(ClassesObj, WORLD_CLASSES);
};

class AllElements:public Polymorph
{
    private:
	 BoxObj		Box;
	 CirTorusObj	CirTorus;
	 ConeObj	Cone;
	 CylinderObj	Cylinder;
	 RectTorusObj	RectTorus;
	 SphereObj	Sphere;
	 PyramidObj	Pyramid;
	 ComplexObj	Complex;
	 PointObj	Point;
	 LineObj	Line;
	 
	 int		*rtn;
    public:
	AllElements();
	~AllElements(); // Functions are found in tritondraw.h
//	AllElements & operator=(AllElements &as);
	ElementObj *GetElement(){ return (ElementObj *)GetObj(); }
	
	long RecDataSize() { return GetElement()->RecDataSize(); }
	Pvoid RecData() { return GetElement()->RecData(); }
	void Prune( T_F deleteChildren, T_F cleanOrphanedParents );

/*	AllElements & operator=(SoNode &node);
	
	int GetIndex(int RecordNo) { return rtn[RecordNo-10000];}
	int GetNum() { return Num; }
	PObjInfo GetInfo(int i=0) { return (PObjInfo)&(Polymorph::GetInfo())[i]; }
*/
	T_F Select(int i) { return Polymorph::Select(i); }
/*	
	void SetNode(SoSeparator *n) { GetElement()->SetNode(n); }
	void SetNode(){ GetElement()->SetNode(); }
	
	SoSeparator *GetNode(){ return GetElement()->GetNode(); }
	
	void Draw() { GetElement()->Draw(); }
	void Print() { GetElement()->Print(); }
	void WritePDMS(char *s) { GetElement()->WritePDMS(s); }

	long getPrimitiveType() { return GetElement()->getPrimitiveType(); }
*/	
	STOREDIN(triton);
	MEMBEROF(ClassesObj, ELEMENT_CLASSES);
};

class OM_EXPORT Classes_type_KeyObj : public KeyObj
{
private:
   long type;

public:
   Classes_type_KeyObj() : KeyObj(CLASSES_TYPE, (Pvoid) &type) {}
   virtual ~Classes_type_KeyObj() {}
};


class OM_EXPORT Classes_name_KeyObj : public KeyObj
{
private:
   char name[32];

public:
   Classes_name_KeyObj() : KeyObj(CLASSES_NAME, (Pvoid) &name[0]) {}
   virtual ~Classes_name_KeyObj() {}
};


class OM_EXPORT Groups_name_KeyObj : public KeyObj
{
private:
   char name[32];

public:
   Groups_name_KeyObj() : KeyObj(GROUPS_NAME, (Pvoid) &name[0]) {}
   virtual ~Groups_name_KeyObj() {}
};



#endif    /* TRITON_HPP */
