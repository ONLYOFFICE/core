#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/HyperlinkMoniker.h>

namespace OSHARED
{;

class CFRecord;

class CompositeMoniker : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CompositeMoniker)
public:
	CompositeMoniker();
	CompositeMoniker(XLS::CFRecord& record);
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	std::vector<HyperlinkMoniker> monikerArray;
};

} // namespace OSHARED

