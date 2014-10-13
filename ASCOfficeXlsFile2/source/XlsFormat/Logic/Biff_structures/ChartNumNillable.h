#pragma once

#include "BiffAttribute.h"

namespace XLS
{;

class CFRecord;

class ChartNumNillable : public BiffAttribute
{
public:

	ChartNumNillable(const unsigned __int16 nil_type);

	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent, const std::wstring & attrib_name);
	//virtual const bool fromXML(BiffStructurePtr & parent, const std::wstring & attrib_name);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	void setNilType(const unsigned __int16 type);
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
			unsigned __int16 type;
			unsigned __int16 fExprO;
		};
		double xnum;
	} data;
	unsigned __int16 nil_type_;
};

} // namespace XLS

