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
	_INT16 xWn;
	_INT16 yWn;
	_INT16 dxWn;
	_INT16 dyWn;
	bool fHidden;
	bool fIconic;
	bool fVeryHidden;
	bool fDspHScroll;
	bool fDspVScroll;
	bool fBotAdornment;
	bool fNoAFDateGroup;
	_UINT16 itabCur;
	_UINT16 itabFirst;
	_UINT16 ctabSel;
	_UINT16 wTabRatio;

};

} // namespace XLS

