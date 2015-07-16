#pragma once

#include "BiffAttribute.h"

namespace XLS
{;

class CFRecord;

class FormulaValue : public BiffAttribute
{
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent, const std::wstring & attrib_name);
	//virtual const bool fromXML(BiffStructurePtr & parent, const std::wstring & attrib_name);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const unsigned char getType() const;

private:
	union
	{
		struct  
		{
			unsigned char Byte1;
			unsigned char Byte2;
			unsigned char Byte3;
			unsigned char Byte4;
			unsigned char Byte5;
			unsigned char Byte6;
			unsigned short fExprO;
		};
		double xnum;
	} data;
};

} // namespace XLS

