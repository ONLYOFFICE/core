
#include "Bes.h"

#include <boost/algorithm/string.hpp>

namespace XLS
{;


BiffStructurePtr Bes::clone()
{
	return BiffStructurePtr(new Bes(*this));
}


//void Bes::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"fError", fError);
//	xml_tag->setAttribute(L"bBoolErr", bBoolErr);
//	xml_tag->setAttribute(L"value", toString().c_str());
//}
//
//
//void Bes::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	std::wstring  val = getStructAttribute(xml_tag, L"value");
//	fromString(static_cast<wchar_t*>(val));
//}


void Bes::store(CFRecord& record)
{
	record << bBoolErr << fError;
}


void Bes::load(CFRecord& record)
{
	record >> bBoolErr >> fError;
}


const std::wstring Bes::toString()
{
	if(fError || bBoolErr > 0x01)
	{
		switch(bBoolErr)
		{
		case 0x00:
			return L"#NULL!";
		case 0x07:
			return L"#DIV/0!";
		case 0x0F:
			return L"#VALUE!";
		case 0x17:
			return L"#REF!";
		case 0x1D:
			return L"#NAME?";
		case 0x24:
			return L"#NUM!";
		case 0x2A:
			return L"#N/A";
		case 0x2B:
			return L"#GETTING_DATA";
		default:
			throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of Bes.", "unknown");
		}
	}
	else
	{
		return Boolean<unsigned char>(bBoolErr);
	}
}


void Bes::fromString(const std::wstring str)
{
	const std::wstring upper_str = boost::algorithm::to_upper_copy(str);
	if(L"#NULL!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x00;
	}
	else if(L"#DIV/0!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x07;
	}
	else if(L"#VALUE!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x0F;
	}
	else if(L"#REF!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x17;
	}
	else if(L"#NAME?" == upper_str)
	{
		fError = true;
		bBoolErr = 0x1D;
	}
	else if(L"#NUM!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x24;
	}
	else if(L"#N/A" == upper_str)
	{
		fError = true;
		bBoolErr = 0x2A;
	}
	else if(L"#GETTING_DATA" == upper_str)
	{
		fError = true;
		bBoolErr = 0x2B;
	}
	else if(L"TRUE" == upper_str || L"1" == str)
	{
		fError = false;
		bBoolErr = 0x01;
	}
	else if(L"FALSE" == upper_str || L"0" == str)
	{
		fError = false;
		bBoolErr = 0x00;
	}
	else
	{
		throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of Bes.", "unknown");
	}
}


} // namespace XLS

