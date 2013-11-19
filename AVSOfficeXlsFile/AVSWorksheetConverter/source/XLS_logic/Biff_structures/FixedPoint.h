#pragma once

#include "BiffAttribute.h"

namespace OSHARED
{;

class CFRecord;

class FixedPoint : public XLS::BiffAttribute
{
public:
	FixedPoint();
	FixedPoint(const long raw_data);
	XLS::BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	short Integral;
	WORD Fractional;
};

} // namespace OSHARED

