#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/FontIndex.h>
#include <XLS_logic/Biff_structures/CellXF.h>
#include <XLS_logic/Biff_structures/StyleXF.h>

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

private:
	FontIndex ifnt;
	BIFF_WORD ifmt;
	BIFF_BOOL fLocked;
	BIFF_BOOL fHidden;
	BIFF_BOOL fStyle;
	BIFF_BOOL f123Prefix;
	BIFF_WORD ixfParent;

	CellXF	cell;
	StyleXF	style;
	
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(ifnt)
		BO_ATTRIB_MARKUP_ATTRIB(ifmt)
		BO_ATTRIB_MARKUP_ATTRIB(fLocked)
		BO_ATTRIB_MARKUP_ATTRIB(fHidden)
		BO_ATTRIB_MARKUP_ATTRIB(fStyle)
		BO_ATTRIB_MARKUP_ATTRIB(f123Prefix)
		BO_ATTRIB_MARKUP_ATTRIB(ixfParent)
		if(fStyle)
		{
			BO_ATTRIB_MARKUP_COMPLEX(style)
		}
		else
		{
			BO_ATTRIB_MARKUP_COMPLEX(cell)
		}
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

