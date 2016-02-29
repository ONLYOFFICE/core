#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of UserBView record in BIFF8
class UserBView: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(UserBView)
	BASE_OBJECT_DEFINE_CLASS_NAME(UserBView)
public:
	UserBView();
	~UserBView();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeUserBView;

	int serialize(std::wostream & stream);
//-----------------------------
	_UINT16			tabId;
	std::wstring	guid;
	
	BIFF_DWORD		x;
	BIFF_DWORD		y;
	BIFF_DWORD		dx;
	BIFF_DWORD		dy;

	_UINT16			wTabRatio;

	bool			fDspFmlaBar;
	bool			fDspStatus;
	
	std::wstring	mdNoteDisp;
	
	bool fDspHScroll;
	bool fDspVScroll;
	bool fBotAdornment;
	bool fZoom;
	
	std::wstring fHideObj;
	
	bool fPrintIncl;
	bool fRowColIncl;
	bool fInvalidTabId;
	bool fTimedUpdate;
	bool fAllMemChanges;
	bool fOnlySync;
	bool fPersonalView;
	bool fIconic;
	
	_UINT16			wMergeInterval;
	XLUnicodeString st;

};

} // namespace XLS

