#pragma once

#include "BiffAttribute.h"

namespace XLS
{;

class CFRecord;

class ChartNumNillable : public BiffAttribute
{
public:

	ChartNumNillable(const unsigned short nil_type);

	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent, const std::wstring & attrib_name);
	//virtual const bool fromXML(BiffStructurePtr & parent, const std::wstring & attrib_name);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	void setNilType(const unsigned short type);
	const bool isNil();

private:
	union
	{
		struct  
		{
			unsigned char Byte1;
			unsigned char Byte2;
			unsigned char Byte3;
			unsigned char Byte4;
			unsigned short type;
			unsigned short fExprO;
		};
		double xnum;
	} data;
	unsigned short nil_type_;
};

} // namespace XLS

