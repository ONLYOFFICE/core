#pragma once

#include "BiffAttribute.h"

namespace OSHARED
{

class CFRecord;

class FixedPoint : public XLS::BiffAttribute
{
public:
	FixedPoint();
	FixedPoint(const long raw_data);
	XLS::BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent, const std::wstring & attrib_name);
	//virtual const bool fromXML(BiffStructurePtr & parent, const std::wstring & attrib_name);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	short Integral;
	unsigned short Fractional;
};

} // namespace OSHARED

