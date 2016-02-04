#pragma once

#include "BiffRecord.h"

namespace XLS
{


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

	static const ElementType	type = typeWindow1;

	int serialize(std::wostream & stream);
//-----------------------------
	BIFF_SHORT xWn;
	BIFF_SHORT yWn;
	BIFF_SHORT dxWn;
	BIFF_SHORT dyWn;
	bool fHidden;
	bool fIconic;
	bool fVeryHidden;
	bool fDspHScroll;
	bool fDspVScroll;
	bool fBotAdornment;
	bool fNoAFDateGroup;
	BIFF_WORD itabCur;
	BIFF_WORD itabFirst;
	BIFF_WORD ctabSel;
	BIFF_WORD wTabRatio;

};

} // namespace XLS

