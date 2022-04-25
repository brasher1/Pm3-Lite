#ifndef __DTROW_H__
#define __DTROW_H__

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// CDTRow wrapper class

class CDTRow : public COleDispatchDriver
{
public:
	CDTRow() {}		// Calls COleDispatchDriver default constructor
	CDTRow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDTRow(const CDTRow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	unsigned long GetBackColor();
	void SetBackColor(unsigned long);
	unsigned long GetSelBackColor();
	void SetSelBackColor(unsigned long);
	unsigned long GetForeColor();
	void SetForeColor(unsigned long);
	unsigned long GetSelForeColor();
	void SetSelForeColor(unsigned long);
	COleFont GetFont();
	void SetFont(LPDISPATCH);
	CString GetLabel();
	void SetLabel(LPCTSTR);
	VARIANT GetValue();
	void SetValue(const VARIANT&);
	BOOL GetSelected();
	void SetSelected(BOOL);
	long GetResize();
	void SetResize(long);
	float GetHeight();
	void SetHeight(float);

// Operations
public:
	void Invalidate();
};

#endif // __DTROW_H__
