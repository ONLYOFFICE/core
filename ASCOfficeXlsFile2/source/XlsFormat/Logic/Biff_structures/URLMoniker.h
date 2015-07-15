#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/URICreateFlags.h>

namespace OSHARED
{;

class CFRecord;

class URLMoniker : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(URLMoniker)
public:
	URLMoniker();
	URLMoniker(XLS::CFRecord& record);
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	std::wstring url;
	std::wstring  serialGUID;
	unsigned int serialVersion;
	URICreateFlags uriFlags;
};

} // namespace OSHARED

