#ifndef __DTCELLSET_H__
#define __DTCELLSET_H__

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CDTCell;

/////////////////////////////////////////////////////////////////////////////
// CDTCellSet wrapper class

class CDTCellSet : public COleDispatchDriver
{
public:
	CDTCellSet() {}		// Calls COleDispatchDriver default constructor
	CDTCellSet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDTCellSet(const CDTCellSet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetSelectType();
	void SetSelectType(long);
	CString GetNullString();
	void SetNullString(LPCTSTR);

// Operations
public:
	CDTCell GetItem(long Row, long Column);
	void SetItem(long Row, long Column, LPDISPATCH newValue);
	void SelectBlock(long LeftCol, long TopRow, long RightCol, long BottomRow, BOOL bSelect);
	BOOL GetSelect(long* LeftCol, long* TopRow, long* RightCol, long* BottomRow);
	long GetCellAtPoint(float X, float Y, short* Pane, long* Row, long* Column);
	CDTCell Get_Item(long Column, long Row);
	void Set_Item(long Column, long Row, LPDISPATCH newValue);
};

#endif // __DTCELLSET_H__