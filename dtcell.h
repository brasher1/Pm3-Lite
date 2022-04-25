#ifndef __DTCELL_H__
#define __DTCELL_H__

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// CDTCell wrapper class

class CDTCell : public COleDispatchDriver
{
public:
	CDTCell() {}		// Calls COleDispatchDriver default constructor
	CDTCell(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDTCell(const CDTCell& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	COleFont GetFont();
	void SetFont(LPDISPATCH);
	unsigned long GetBackColor();
	void SetBackColor(unsigned long);
	unsigned long GetForeColor();
	void SetForeColor(unsigned long);
	unsigned long GetSelBackColor();
	void SetSelBackColor(unsigned long);
	unsigned long GetSelForeColor();
	void SetSelForeColor(unsigned long);
	BOOL GetNullFlag();
	void SetNullFlag(BOOL);
	VARIANT GetValue();
	void SetValue(const VARIANT&);
	BOOL GetSelected();
	void SetSelected(BOOL);

// Operations
public:
	void Invalidate();
	void MakeVisible();
	BOOL GetRect(long Pane, float* Left, float* Top, float* Right, float* Bottom);
};

#endif // __DTCELL_H__
