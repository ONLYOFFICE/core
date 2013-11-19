#pragma once

#include "BiffStructTagged.h"

namespace OSHARED
{;

class CFRecord;

class ItemMoniker : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ItemMoniker)
public:
	ItemMoniker();
	ItemMoniker(XLS::CFRecord& record);
	XLS::BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	std::string delimiterAnsi;
	std::wstring delimiterUnicode;
	std::string itemAnsi;
	std::wstring itemUnicode;
};

} // namespace OSHARED

