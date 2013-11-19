#include "stdafx.h"
#include "FormulaValue.h"
#include <XLS_bin/CFRecord.h>
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;


BiffStructurePtr FormulaValue::clone()
{
	return BiffStructurePtr(new FormulaValue(*this));
}


void FormulaValue::toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
{
	_variant_t result;
	if(0xffff != data.fExprO)
	{
		result = data.xnum;
	}
	else
	{
		switch(data.byte1)
		{
			case 0x00: // String
				result = "";	// there must a String value follow this Formula record
				break;
			case 0x01: // Boolean
				result = data.byte3 ? "true" : "false";
				break;
			case 0x02: // Error
			{
				BErr error;
				error.err = data.byte3;
				result = error;
				break;
			}
			case 0x03: // Blank
				result = "";
				break;
			default:
				throw EXCEPT::RT::WrongBiffRecord("Unsupported type of FormulaValue.", "unknown");
		}
	}
	parent->setAttribute(attrib_name, result);
	parent->setAttribute(L"val_type", getType());
}


const bool FormulaValue::fromXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
{
	BYTE val_type = getStructAttribute(parent, L"val_type");

	switch(val_type)
	{
		case 4:
			data.xnum = getStructAttribute(parent, attrib_name);
			break;
		case 0:
		case 3:
			data.byte1 = val_type;
			data.fExprO = 0xffff;
			break;
		case 1:
		{
			data.byte1 = 1;
			Boolean<BYTE> bool_val;
			bool_val.fromXML(parent, attrib_name);
			data.byte3 = static_cast<BYTE>(bool_val);
			data.fExprO = 0xffff;
			break;
		}
		case 2:
		{
			_bstr_t str_error = getStructAttribute(parent, attrib_name);
			data.byte1 = 2;
			BErr error(str_error);
			data.byte3 = error.err;
			data.fExprO = 0xffff;
			break;
		}
		default:
			throw EXCEPT::RT::WrongBiffRecord("Unsupported type of FormulaValue.", "unknown");

	}
	return true;
}


void FormulaValue::store(CFRecord& record)
{
	record << data.xnum;
}


void FormulaValue::load(CFRecord& record)
{
	record >> data.xnum;
}


const BYTE FormulaValue::getType() const
{
	if(0xffff != data.fExprO)
	{
		return 4; // Would mean that the value is numeric
	}
	else
	{
		return data.byte1;
	}
}

} // namespace XLS

