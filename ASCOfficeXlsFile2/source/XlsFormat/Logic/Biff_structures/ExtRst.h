#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/Phs.h>
#include <Logic/Biff_structures/RPHSSub.h>
#include <Logic/Biff_structures/PhRuns.h>

namespace XLS
{;

class CFRecord;

class ExtRst : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ExtRst)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const size_t getSize() const;

private:
	unsigned __int16 cb;
	Phs phs;
	RPHSSub rphssub;
	std::vector<PhRuns> rgphruns;
};

} // namespace XLS
