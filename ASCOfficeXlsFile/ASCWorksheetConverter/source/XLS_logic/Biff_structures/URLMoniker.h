#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/URICreateFlags.h>

namespace OSHARED
{;

class CFRecord;

class URLMoniker : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(URLMoniker)
public:
	URLMoniker();
	URLMoniker(XLS::CFRecord& record);
	XLS::BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	std::wstring url;
	_bstr_t serialGUID;
	DWORD serialVersion;
	URICreateFlags uriFlags;
};

} // namespace OSHARED

