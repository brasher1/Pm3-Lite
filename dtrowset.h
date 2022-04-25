#ifndef __DTROWSET_H__
#define __DTROWSET_H__

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CDTRow;

/////////////////////////////////////////////////////////////////////////////
// CDTRowSet wrapper class

class CDTRowSet : public COleDispatchDriver
{
public:
	CDTRowSet() {}		// Calls COleDispatchDriver default constructor
	CDTRowSet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDTRowSet(const CDTRowSet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetCount();
	void SetCount(long);
	BOOL GetAllowResize();
	void SetAllowResize(BOOL);
	long GetSelectType();
	void SetSelectType(long);
	long GetVertAlign();
	void SetVertAlign(long);
	short GetHeadingType();
	void SetHeadingType(short);
	float GetHeight();
	void SetHeight(float);
	BOOL GetTrapDeleteKey();
	void SetTrapDeleteKey(BOOL);
	BOOL GetTrapInsertKey();
	void SetTrapInsertKey(BOOL);
	BOOL GetAllowAutoRowInsert();
	void SetAllowAutoRowInsert(BOOL);

// Operations
public:
	void Copy(long Source, long Dest);
	void Move(long Source, long Dest);
	void Remove(long Index);
	long GetSelectCount();
	void SelectRange(long Start, long End, BOOL SelectOn);
	long GetVisCount(long TBPane);
	long GetFirstVisible(long TBPane);
	void SetFirstVisible(long TBPane, long nNewValue);
	void Reset();
	long Add(long Index, const VARIANT& Data);
	CDTRow GetItem(long Index);
	void SetItem(long Index, LPDISPATCH newValue);
	CDTRow Get_Item(long Index);
	void Set_Item(long Index, LPDISPATCH newValue);
	long GetSelect(long After);
};

#endif // __DTROWSET_H__