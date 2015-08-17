#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FrtRefHeader.h>
#include <Logic/Biff_structures/DXFN12.h>
#include <Logic/Biff_structures/CFParsedFormulaNoCCE.h>
#include <Logic/Biff_structures/CFParsedFormula.h>
#include <Logic/Biff_structures/CFExTemplateParams.h>

namespace XLS
{


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

	static const ElementType	type = typeCF12;

//-----------------------------
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
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_COMPLEX(frtRefHeader)
		//BO_ATTRIB_MARKUP_ATTRIB(ct)
		//BO_ATTRIB_MARKUP_ATTRIB(cp)
		//BO_ATTRIB_MARKUP_COMPLEX(dxf)
		//BO_ATTRIB_MARKUP_COMPLEX(rgce1)
		//BO_ATTRIB_MARKUP_COMPLEX(rgce2)
		//BO_ATTRIB_MARKUP_COMPLEX(fmlaActive)
		//BO_ATTRIB_MARKUP_ATTRIB(fStopIfTrue)
		//BO_ATTRIB_MARKUP_ATTRIB(ipriority)
		//BO_ATTRIB_MARKUP_ATTRIB(icfTemplate)
		//rgbTemplateParms.toXML(proc.getParent(), static_cast<unsigned char>(icfTemplate));
		//if(rgbCT)
		//{
		//	//BO_ATTRIB_MARKUP_COMPLEX(*rgbCT)
		//}
	//BO_ATTRIB_MARKUP_END

};

typedef boost::shared_ptr<CF12> CF12Ptr;

} // namespace XLS

