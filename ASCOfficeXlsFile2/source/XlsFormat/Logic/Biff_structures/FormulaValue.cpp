
#include "FormulaValue.h"
#include <Binary/CFRecord.h>
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{


BiffStructurePtr FormulaValue::clone()
{
	return BiffStructurePtr(new FormulaValue(*this));
}


std::wstring FormulaValue::getValue()
{
	std::wstring result;

	if(0xffff != data.fExprO)
	{
		result = STR::double2str(data.xnum);
	}
	else
	{
		switch(data.Byte1)
		{
			case 0x00: // String
				result = L"";	// there must a String value follow this Formula record
				break;
			case 0x01: // Boolean
				result = data.Byte3 ? L"true" : L"false";
				break;
			case 0x02: // Error
			{
				BErr error;
				error.err = data.Byte3;
				result = error.toString();
				break;
			}
			case 0x03: // Blank
				result = L"";
				break;
		}
	}
	return result;
}

//

void FormulaValue::store(CFRecord& record)
{
	record << data.xnum;
}


void FormulaValue::load(CFRecord& record)
{
	record >> data.xnum;
}


unsigned char FormulaValue::getType() 
{
	if(0xffff != data.fExprO)
	{
		return 4; // Would mean that the value is numeric
	}
	else
	{
		return data.Byte1;
	}
}

} // namespace XLS

