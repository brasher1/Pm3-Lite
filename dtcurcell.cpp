// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "dtcurcell.h"

// Dispatch interfaces referenced by this interface
#include "Font.h"


/////////////////////////////////////////////////////////////////////////////
// CDTCurCell properties

long CDTCurCell::GetColumn()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void CDTCurCell::SetColumn(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

long CDTCurCell::GetRow()
{
	long result;
	GetProperty(0xf, VT_I4, (void*)&result);
	return result;
}

void CDTCurCell::SetRow(long propVal)
{
	SetProperty(0xf, VT_I4, propVal);
}

long CDTCurCell::GetPane()
{
	long result;
	GetProperty(0x10, VT_I4, (void*)&result);
	return result;
}

void CDTCurCell::SetPane(long propVal)
{
	SetProperty(0x10, VT_I4, propVal);
}

COleFont CDTCurCell::GetFont()
{
	LPDISPATCH pDispatch;
	GetProperty(0x11, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CDTCurCell::SetFont(LPDISPATCH propVal)
{
	SetProperty(0x11, VT_DISPATCH, propVal);
}

VARIANT CDTCurCell::GetEditValue()
{
	VARIANT result;
	GetProperty(0x12, VT_VARIANT, (void*)&result);
	return result;
}

void CDTCurCell::SetEditValue(const VARIANT& propVal)
{
	SetProperty(0x12, VT_VARIANT, &propVal);
}

unsigned long CDTCurCell::GetBackColor()
{
	unsigned long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}

void CDTCurCell::SetBackColor(unsigned long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}

unsigned long CDTCurCell::GetForeColor()
{
	unsigned long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}

void CDTCurCell::SetForeColor(unsigned long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}

unsigned long CDTCurCell::GetSelBackColor()
{
	unsigned long result;
	GetProperty(0x15, VT_I4, (void*)&result);
	return result;
}

void CDTCurCell::SetSelBackColor(unsigned long propVal)
{
	SetProperty(0x15, VT_I4, propVal);
}

unsigned long CDTCurCell::GetSelForeColor()
{
	unsigned long result;
	GetProperty(0x16, VT_I4, (void*)&result);
	return result;
}

void CDTCurCell::SetSelForeColor(unsigned long propVal)
{
	SetProperty(0x16, VT_I4, propVal);
}

unsigned long CDTCurCell::GetFocusPenColor()
{
	unsigned long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void CDTCurCell::SetFocusPenColor(unsigned long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

short CDTCurCell::GetFocusPenWidth()
{
	short result;
	GetProperty(0x2, VT_I2, (void*)&result);
	return result;
}

void CDTCurCell::SetFocusPenWidth(short propVal)
{
	SetProperty(0x2, VT_I2, propVal);
}

short CDTCurCell::GetFocusPenStyle()
{
	short result;
	GetProperty(0x3, VT_I2, (void*)&result);
	return result;
}

void CDTCurCell::SetFocusPenStyle(short propVal)
{
	SetProperty(0x3, VT_I2, propVal);
}

short CDTCurCell::GetKeyForAbortEdit()
{
	short result;
	GetProperty(0x4, VT_I2, (void*)&result);
	return result;
}

void CDTCurCell::SetKeyForAbortEdit(short propVal)
{
	SetProperty(0x4, VT_I2, propVal);
}

short CDTCurCell::GetKeyForBeginEdit()
{
	short result;
	GetProperty(0x5, VT_I2, (void*)&result);
	return result;
}

void CDTCurCell::SetKeyForBeginEdit(short propVal)
{
	SetProperty(0x5, VT_I2, propVal);
}

short CDTCurCell::GetKeyForEndEdit()
{
	short result;
	GetProperty(0x6, VT_I2, (void*)&result);
	return result;
}

void CDTCurCell::SetKeyForEndEdit(short propVal)
{
	SetProperty(0x6, VT_I2, propVal);
}

BOOL CDTCurCell::GetEndEditTab()
{
	BOOL result;
	GetProperty(0x7, VT_BOOL, (void*)&result);
	return result;
}

void CDTCurCell::SetEndEditTab(BOOL propVal)
{
	SetProperty(0x7, VT_BOOL, propVal);
}

BOOL CDTCurCell::GetEndEditCtrlTab()
{
	BOOL result;
	GetProperty(0x8, VT_BOOL, (void*)&result);
	return result;
}

void CDTCurCell::SetEndEditCtrlTab(BOOL propVal)
{
	SetProperty(0x8, VT_BOOL, propVal);
}

BOOL CDTCurCell::GetEndEditLfRtArrows()
{
	BOOL result;
	GetProperty(0x9, VT_BOOL, (void*)&result);
	return result;
}

void CDTCurCell::SetEndEditLfRtArrows(BOOL propVal)
{
	SetProperty(0x9, VT_BOOL, propVal);
}

BOOL CDTCurCell::GetEndEditUpDnArrows()
{
	BOOL result;
	GetProperty(0xa, VT_BOOL, (void*)&result);
	return result;
}

void CDTCurCell::SetEndEditUpDnArrows(BOOL propVal)
{
	SetProperty(0xa, VT_BOOL, propVal);
}

BOOL CDTCurCell::GetEndEditClick()
{
	BOOL result;
	GetProperty(0xb, VT_BOOL, (void*)&result);
	return result;
}

void CDTCurCell::SetEndEditClick(BOOL propVal)
{
	SetProperty(0xb, VT_BOOL, propVal);
}

VARIANT CDTCurCell::GetValue()
{
	VARIANT result;
	GetProperty(0x17, VT_VARIANT, (void*)&result);
	return result;
}

void CDTCurCell::SetValue(const VARIANT& propVal)
{
	SetProperty(0x17, VT_VARIANT, &propVal);
}

BOOL CDTCurCell::GetAllowCellEdit()
{
	BOOL result;
	GetProperty(0x18, VT_BOOL, (void*)&result);
	return result;
}

void CDTCurCell::SetAllowCellEdit(BOOL propVal)
{
	SetProperty(0x18, VT_BOOL, propVal);
}

BOOL CDTCurCell::GetTabToNextCell()
{
	BOOL result;
	GetProperty(0xc, VT_BOOL, (void*)&result);
	return result;
}

void CDTCurCell::SetTabToNextCell(BOOL propVal)
{
	SetProperty(0xc, VT_BOOL, propVal);
}

BOOL CDTCurCell::GetMoveColMajor()
{
	BOOL result;
	GetProperty(0xd, VT_BOOL, (void*)&result);
	return result;
}

void CDTCurCell::SetMoveColMajor(BOOL propVal)
{
	SetProperty(0xd, VT_BOOL, propVal);
}

long CDTCurCell::GetHWndListBox()
{
	long result;
	GetProperty(0x19, VT_I4, (void*)&result);
	return result;
}

void CDTCurCell::SetHWndListBox(long propVal)
{
	SetProperty(0x19, VT_I4, propVal);
}

long CDTCurCell::GetHWndEditBox()
{
	long result;
	GetProperty(0x1a, VT_I4, (void*)&result);
	return result;
}

void CDTCurCell::SetHWndEditBox(long propVal)
{
	SetProperty(0x1a, VT_I4, propVal);
}

long CDTCurCell::GetSelStart()
{
	long result;
	GetProperty(0x1b, VT_I4, (void*)&result);
	return result;
}

void CDTCurCell::SetSelStart(long propVal)
{
	SetProperty(0x1b, VT_I4, propVal);
}

long CDTCurCell::GetSelLength()
{
	long result;
	GetProperty(0x1c, VT_I4, (void*)&result);
	return result;
}

void CDTCurCell::SetSelLength(long propVal)
{
	SetProperty(0x1c, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CDTCurCell operations

void CDTCurCell::MoveToNext()
{
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDTCurCell::MoveToPrev()
{
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDTCurCell::BeginEdit()
{
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDTCurCell::EndEdit()
{
	InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDTCurCell::AbortEdit()
{
	InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}