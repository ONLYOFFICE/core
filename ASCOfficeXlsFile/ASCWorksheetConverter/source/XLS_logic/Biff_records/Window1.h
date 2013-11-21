#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Window1 record in BIFF8
class Window1: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Window1)
	BASE_OBJECT_DEFINE_CLASS_NAME(Window1)
public:
	Window1();
	~Window1();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_SHORT xWn;
	BIFF_SHORT yWn;
	BIFF_SHORT dxWn;
	BIFF_SHORT dyWn;
	BIFF_BOOL fHidden;
	BIFF_BOOL fIconic;
	BIFF_BOOL fVeryHidden;
	BIFF_BOOL fDspHScroll;
	BIFF_BOOL fDspVScroll;
	BIFF_BOOL fBotAdornment;
	BIFF_BOOL fNoAFDateGroup;
	BIFF_WORD itabCur;
	BIFF_WORD itabFirst;
	BIFF_WORD ctabSel;
	BIFF_WORD wTabRatio;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(xWn)
		BO_ATTRIB_MARKUP_ATTRIB(yWn)
		BO_ATTRIB_MARKUP_ATTRIB(dxWn)
		BO_ATTRIB_MARKUP_ATTRIB(dyWn)
		BO_ATTRIB_MARKUP_ATTRIB(fHidden)
		BO_ATTRIB_MARKUP_ATTRIB(fIconic)
		BO_ATTRIB_MARKUP_ATTRIB(fVeryHidden)
		BO_ATTRIB_MARKUP_ATTRIB(fDspHScroll)
		BO_ATTRIB_MARKUP_ATTRIB(fDspVScroll)
		BO_ATTRIB_MARKUP_ATTRIB(fBotAdornment)
		BO_ATTRIB_MARKUP_ATTRIB(fNoAFDateGroup)
		BO_ATTRIB_MARKUP_ATTRIB(itabCur)
		BO_ATTRIB_MARKUP_ATTRIB(itabFirst)
		BO_ATTRIB_MARKUP_ATTRIB(ctabSel)
		BO_ATTRIB_MARKUP_ATTRIB(wTabRatio)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

