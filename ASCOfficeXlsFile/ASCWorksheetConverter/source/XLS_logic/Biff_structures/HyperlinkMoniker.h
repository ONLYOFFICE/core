#pragma once

#include "BiffStructure.h"

namespace OSHARED
{;

class CFRecord;

class HyperlinkMoniker : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(HyperlinkMoniker)
public:
	XLS::BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	_bstr_t monikerClsid;
	boost::shared_ptr<BiffStructure> data;
};

} // namespace OSHARED

