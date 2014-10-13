#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/DXFN.h>
#include <Logic/Biff_structures/CFParsedFormulaNoCCE.h>

namespace XLS
{;


// Logical representation of CF record in BIFF8
class CF: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CF)
	BASE_OBJECT_DEFINE_CLASS_NAME(CF)
public:
	CF(const CellRef& cell_base_ref);
	~CF();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BYTE ct;
	BIFF_BYTE cp;
	DXFN rgbdxf;
	CFParsedFormulaNoCCE rgce1;
	CFParsedFormulaNoCCE rgce2;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(ct)
		BO_ATTRIB_MARKUP_ATTRIB(cp)
		BO_ATTRIB_MARKUP_COMPLEX(rgbdxf)
		BO_ATTRIB_MARKUP_COMPLEX(rgce1)
		BO_ATTRIB_MARKUP_COMPLEX(rgce2)
	BO_ATTRIB_MARKUP_END

};

typedef boost::shared_ptr<CF> CFPtr;

} // namespace XLS

