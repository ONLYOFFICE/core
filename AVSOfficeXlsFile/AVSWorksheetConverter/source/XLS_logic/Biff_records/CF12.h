#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/FrtRefHeader.h>
#include <XLS_logic/Biff_structures/DXFN12.h>
#include <XLS_logic/Biff_structures/CFParsedFormulaNoCCE.h>
#include <XLS_logic/Biff_structures/CFParsedFormula.h>
#include <XLS_logic/Biff_structures/CFExTemplateParams.h>

namespace XLS
{;


// Logical representation of CF12 record in BIFF8
class CF12: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CF12)
	BASE_OBJECT_DEFINE_CLASS_NAME(CF12)
public:
	CF12(const CellRef& cell_base_ref);
	~CF12();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	FrtRefHeader frtRefHeader;
	BIFF_BYTE ct;
	BIFF_BYTE cp;
	DXFN12 dxf;
	CFParsedFormulaNoCCE rgce1;
	CFParsedFormulaNoCCE rgce2;
	CFParsedFormula fmlaActive;
	BIFF_BOOL fStopIfTrue;
	BIFF_WORD ipriority;
	BIFF_WORD icfTemplate;
	CFExTemplateParams rgbTemplateParms;
	BiffStructurePtr rgbCT;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(frtRefHeader)
		BO_ATTRIB_MARKUP_ATTRIB(ct)
		BO_ATTRIB_MARKUP_ATTRIB(cp)
		BO_ATTRIB_MARKUP_COMPLEX(dxf)
		BO_ATTRIB_MARKUP_COMPLEX(rgce1)
		BO_ATTRIB_MARKUP_COMPLEX(rgce2)
		BO_ATTRIB_MARKUP_COMPLEX(fmlaActive)
		BO_ATTRIB_MARKUP_ATTRIB(fStopIfTrue)
		BO_ATTRIB_MARKUP_ATTRIB(ipriority)
		BO_ATTRIB_MARKUP_ATTRIB(icfTemplate)
		rgbTemplateParms.toXML(proc.getParent(), static_cast<BYTE>(icfTemplate));
		if(rgbCT)
		{
			BO_ATTRIB_MARKUP_COMPLEX(*rgbCT)
		}
	BO_ATTRIB_MARKUP_END

};

typedef boost::shared_ptr<CF12> CF12Ptr;

} // namespace XLS

