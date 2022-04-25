#ifndef ___Ptg3DNode_h___
#define ___Ptg3DNode_h___

#include "HierElem.h"
#include "PtgTransMatrix.h"

#define PTG_DEF_RED	1.0
#define PTG_DEF_GREEN	1.0
#define PTG_DEF_BLUE	1.0
#define PTG_DEF_ALPHA	1.0

/*
 * The transformation stored in the Node class is from child Node to parent Node.
 */

class Ptg3DNode : public HierElem {
	ID_TAG(Ptg3DNode);

    public:
	enum DrawStyle {
	    DS_NONE	    = 0x0000,
	    DS_INVISIBLE    = 0x0001,
	    DS_POINTS	    = 0x0002,
	    DS_WIREFRAME    = 0x0004,
	    DS_SOLIDS	    = 0x0008,
	    DS_MASK1	    = 0x000F,
	    DS_PICKING	    = 0x0100
	};

    public:
	Ptg3DNode();
	virtual ~Ptg3DNode();
	void setColor( float red = PTG_DEF_RED, float green = PTG_DEF_GREEN,
		float blue = PTG_DEF_BLUE, float alpha = PTG_DEF_ALPHA );
	void getColor( float retColors[4] );
	void clearColor() { setColor( -1.0, 0.0, 0.0, 0.0 ); }
	void setName( const char * newName );
	const char *getName() { return nodeName; }
	void draw();
	void setMatrix( VECTOR v );
	VECTOR getMatrix( Ptg3DNode *refNode = NULL );
	void setDrawStyle( DrawStyle ds, int bSetMembers = FALSE );
	DrawStyle getDrawStyle();
	int getUniqID(){ return nMyUniqID; }
	Ptg3DNode *findByID( int nNumIDS, unsigned int naIDS[] );
	Ptg3DNode *findByName( const char *findName, int bRecurse = FALSE );
	void setActive( int bNewVal ) { bIsActive = bNewVal; }
	int isActive() { return bIsActive; }

    protected:
	PtgTransMatrix transMatrix;
	int nMyUniqID;
	virtual void drawWireframe();
	virtual void drawSolid();

    private:
	static int nUniqID;
	float rgbColors[4];
	char *nodeName;
	int bIsActive;
	DrawStyle currDrawStyle;
	void initVars();
};

#endif	//#ifndef ___Ptg3DNode_h___
