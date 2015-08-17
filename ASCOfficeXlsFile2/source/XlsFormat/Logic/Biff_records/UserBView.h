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

//-----------------------------
	BIFF_WORD tabId;
	BIFF_BSTR guid;
	BIFF_DWORD x;
	BIFF_DWORD y;
	BIFF_DWORD dx;
	BIFF_DWORD dy;
	BIFF_WORD wTabRatio;

	BIFF_BOOL fDspFmlaBar;
	BIFF_BOOL fDspStatus;
	BIFF_BSTR mdNoteDisp;
	BIFF_BOOL fDspHScroll;
	BIFF_BOOL fDspVScroll;
	BIFF_BOOL fBotAdornment;
	BIFF_BOOL fZoom;
	BIFF_BSTR fHideObj;
	BIFF_BOOL fPrintIncl;
	BIFF_BOOL fRowColIncl;
	BIFF_BOOL fInvalidTabId;
	BIFF_BOOL fTimedUpdate;
	BIFF_BOOL fAllMemChanges;
	BIFF_BOOL fOnlySync;
	BIFF_BOOL fPersonalView;
	BIFF_BOOL fIconic;
	BIFF_WORD wMergeInterval;
	XLUnicodeString st;

		//if(fPersonalView && fTimedUpdate)
		//{
		//	//BO_ATTRIB_MARKUP_ATTRIB(wMergeInterval)
		//}

};

} // namespace XLS

