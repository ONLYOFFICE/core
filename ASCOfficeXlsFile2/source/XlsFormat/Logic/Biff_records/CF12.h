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

	int serialize(std::wostream & stream);
	FrtRefHeader			frtRefHeader;

	unsigned char			ct;
	unsigned char			cp;

	DXFN12					dxf;

	CFParsedFormulaNoCCE	rgce1;
	CFParsedFormulaNoCCE	rgce2;

	CFParsedFormula			fmlaActive;
	_UINT16					ipriority;
	_UINT16					icfTemplate;
	CFExTemplateParams		rgbTemplateParms;

	BiffStructurePtr		rgbCT;

//-----------------------------
	bool fStopIfTrue;

	int ipriority_;

	int dxfId_;
};

typedef boost::shared_ptr<CF12> CF12Ptr;

} // namespace XLS

