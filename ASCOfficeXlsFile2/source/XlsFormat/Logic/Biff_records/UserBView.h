#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;


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
private:
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
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(tabId)
		BO_ATTRIB_MARKUP_ATTRIB(guid)
		BO_ATTRIB_MARKUP_ATTRIB(x)
		BO_ATTRIB_MARKUP_ATTRIB(y)
		BO_ATTRIB_MARKUP_ATTRIB(dx)
		BO_ATTRIB_MARKUP_ATTRIB(dy)
		BO_ATTRIB_MARKUP_ATTRIB(wTabRatio)

		BO_ATTRIB_MARKUP_ATTRIB(fDspFmlaBar)
		BO_ATTRIB_MARKUP_ATTRIB(fDspStatus)
		BO_ATTRIB_MARKUP_ATTRIB(mdNoteDisp)
		BO_ATTRIB_MARKUP_ATTRIB(fDspHScroll)
		BO_ATTRIB_MARKUP_ATTRIB(fDspVScroll)
		BO_ATTRIB_MARKUP_ATTRIB(fBotAdornment)
		BO_ATTRIB_MARKUP_ATTRIB(fZoom)
		BO_ATTRIB_MARKUP_ATTRIB(fHideObj)
		BO_ATTRIB_MARKUP_ATTRIB(fPrintIncl)
		BO_ATTRIB_MARKUP_ATTRIB(fRowColIncl)
		BO_ATTRIB_MARKUP_ATTRIB(fInvalidTabId)
		BO_ATTRIB_MARKUP_ATTRIB(fTimedUpdate)
		BO_ATTRIB_MARKUP_ATTRIB(fAllMemChanges)
		BO_ATTRIB_MARKUP_ATTRIB(fOnlySync)
		BO_ATTRIB_MARKUP_ATTRIB(fPersonalView)
		BO_ATTRIB_MARKUP_ATTRIB(fIconic)

		if(fPersonalView && fTimedUpdate)
		{
			BO_ATTRIB_MARKUP_ATTRIB(wMergeInterval)
		}
		BO_ATTRIB_MARKUP_ATTRIB(st)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

