#pragma once

#include "BiffAttribute.h"

namespace OSHARED
{

class CFRecord;

class FixedPoint : public XLS::BiffAttribute
{
public:
	FixedPoint();
    FixedPoint(const int raw_data);
	XLS::BiffStructurePtr clone();

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	short			Integral;
	unsigned short	Fractional;

	double			dVal;
};

} // namespace OSHARED

