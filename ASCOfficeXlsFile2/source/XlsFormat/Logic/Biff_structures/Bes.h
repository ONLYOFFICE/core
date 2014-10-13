#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/Boolean.h>

namespace XLS
{;

class CFRecord;

class Bes : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Bes)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const std::wstring toString();
	void fromString(const std::wstring str);

private:
	unsigned char bBoolErr;
	Boolean<unsigned char> fError;
};

} // namespace XLS

