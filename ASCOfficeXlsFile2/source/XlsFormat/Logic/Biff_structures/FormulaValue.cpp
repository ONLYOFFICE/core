#include "precompiled_xls.h"
#include "FormulaValue.h"
#include <Binary/CFRecord.h>
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;


BiffStructurePtr FormulaValue::clone()
{
	return BiffStructurePtr(new FormulaValue(*this));
}


//void FormulaValue::toXML(BiffStructurePtr & parent, const std::wstring & attrib_name)
//{
//	_variant_t result;
//	if(0xffff != data.fExprO)
//	{
//		result = data.xnum;
//	}
//	else
//	{
//		switch(data.Byte1)
//		{
//			case 0x00: // String
//				result = "";	// there must a String value follow this Formula record
//				break;
//			case 0x01: // Boolean
//				result = data.Byte3 ? "true" : "false";
//				break;
//			case 0x02: // Error
//			{
//				BErr error;
//				error.err = data.Byte3;
//				result = error;
//				break;
//			}
//			case 0x03: // Blank
//				result = "";
//				break;
//			default:
//				throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of FormulaValue.", "unknown");
//		}
//	}
//	parent->setAttribute(attrib_name, result);
//	parent->setAttribute(L"val_type", getType());
//}
//
//
//const bool FormulaValue::fromXML(BiffStructurePtr & parent, const std::wstring & attrib_name)
//{
//	unsigned char val_type = getStructAttribute(parent, L"val_type");
//
//	switch(val_type)
//	{
//		case 4:
//			data.xnum = getStructAttribute(parent, attrib_name);
//			break;
//		case 0:
//		case 3:
//			data.Byte1 = val_type;
//			data.fExprO = 0xffff;
//			break;
//		case 1:
//		{
//			data.Byte1 = 1;
//			Boolean<unsigned char> bool_val;
//			bool_val.fromXML(parent, attrib_name);
//			data.Byte3 = static_cast<unsigned char>(bool_val);
//			data.fExprO = 0xffff;
//			break;
//		}
//		case 2:
//		{
//			std::wstring  str_error = getStructAttribute(parent, attrib_name);
//			data.Byte1 = 2;
//			BErr error(str_error);
//			data.Byte3 = error.err;
//			data.fExprO = 0xffff;
//			break;
//		}
//		default:
//			throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of FormulaValue.", "unknown");
//
//	}
//	return true;
//}
//
//
void FormulaValue::store(CFRecord& record)
{
	record << data.xnum;
}


void FormulaValue::load(CFRecord& record)
{
	record >> data.xnum;
}


const unsigned char FormulaValue::getType() const
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

