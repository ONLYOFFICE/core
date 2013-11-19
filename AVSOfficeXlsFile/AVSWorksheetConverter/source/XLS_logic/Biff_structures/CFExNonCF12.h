#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/DXFN12.h>
#include <XLS_logic/Biff_structures/CFExTemplateParams.h>

namespace XLS
{;

class CFRecord;

class CFExNonCF12 : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CFExNonCF12)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	WORD icf;
	BYTE cp;
	BYTE icfTemplate;
	WORD ipriority;

	bool fActive;
	bool fStopIfTrue;

	Boolean<BYTE> fHasDXF;

	DXFN12 dxf;
	CFExTemplateParams rgbTemplateParms;
};

} // namespace XLS

