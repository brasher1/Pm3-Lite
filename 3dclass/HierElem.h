#ifndef ___HierElem_h___
#define ___HierElem_h___

//#include "CList.h"
#include "afxtempl.h"

#ifdef WIN32
#pragma warning (disable : 4244)

#define ID_TAG(class_name) \
	public: \
		static char *className; \
		virtual const char *getType(){return className;} \
	private:

#define ID_NAME(class_name) \
		char *##class_name::className=#class_name;

#define IS_TYPE_OF(s1, class_name) \
	(strcmp((s1)->getType(), ##class_name::className)?0:1)

#endif //WIN32

#ifdef WIN32
#pragma warning ( disable :  )
#endif

#ifndef FALSE
#define FALSE 0
#endif	//#ifndef FALSE

#ifndef TRUE
#define TRUE (!FALSE)
#endif	//#ifndef TRUE

class HierElem {
    ID_TAG(HierElem);
    
    public:
	HierElem();
	virtual ~HierElem();
	void setOwner( HierElem *owner ) { elemOwner = owner; }
	HierElem *getOwner() { return elemOwner; }
	void addMember( HierElem *newMember );
	int getMemberCount() { return elemMembers.GetCount(); }
	CList<HierElem *, HierElem *>&getMemberList() { return elemMembers; }

    private:
	HierElem *elemOwner;
	CList<HierElem *, HierElem *> elemMembers;
	
	void initVars();
};

#endif	//#ifndef ___HierElem_h___
