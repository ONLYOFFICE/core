#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class PictFmlaEmbedInfo : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PictFmlaEmbedInfo)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	XLUnicodeStringNoCch strClass;
};

} // namespace XLS

