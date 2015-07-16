#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FontIndex.h>
#include <Logic/Biff_structures/CellXF.h>
#include <Logic/Biff_structures/StyleXF.h>

namespace XLS
{;


// Logical representation of XF record in BIFF8
class XF: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(XF)
	BASE_OBJECT_DEFINE_CLASS_NAME(XF)
public:
	XF(size_t& cell_xf_current_id, size_t& style_xf_current_id);
	~XF();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	int serialize(std::wostream & stream);

	static const ElementType	type = typeXF;

//-----------------------------
	FontIndex ifnt;
	BIFF_WORD ifmt;
	BIFF_BOOL fLocked;
	BIFF_BOOL fHidden;
	BIFF_BOOL fStyle;
	BIFF_BOOL f123Prefix;
	BIFF_WORD ixfParent;

	CellXF	cell;
	StyleXF	style;
};

} // namespace XLS

