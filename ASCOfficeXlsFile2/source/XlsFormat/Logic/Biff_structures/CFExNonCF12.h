#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/DXFN12.h>
#include <Logic/Biff_structures/CFExTemplateParams.h>

namespace XLS
{;

class CFRecord;

class CFExNonCF12 : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CFExNonCF12)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned __int16 icf;
	unsigned char cp;
	unsigned char icfTemplate;
	unsigned __int16 ipriority;

	bool fActive;
	bool fStopIfTrue;

	Boolean<unsigned char> fHasDXF;

	DXFN12 dxf;
	CFExTemplateParams rgbTemplateParms;
};

} // namespace XLS

