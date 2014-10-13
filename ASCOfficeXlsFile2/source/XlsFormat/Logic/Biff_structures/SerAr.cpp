#include "precompiled_xls.h"
#include "SerAr.h"
#include <Binary/CFRecord.h>
#include "SerBool.h"
#include "SerErr.h"
#include "SerNil.h"
#include "SerNum.h"
#include "SerStr.h"
//#include <Exception/WrongBiffRecord.h>

namespace XLS
{;


//void SerAr::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->Puttext(toString().c_str());
//}


// static 
SerArPtr SerAr::createSerAr(const unsigned char type)
{
	switch(type)
	{
	case SerBool::fixed_type:
		return SerArPtr(new SerBool);
		break;
	case SerErr::fixed_type:
		return SerArPtr(new SerErr);
		break;
	case SerNil::fixed_type:
		return SerArPtr(new SerNil);
		break;
	case SerNum::fixed_type:
		return SerArPtr(new SerNum);
		break;
	case SerStr::fixed_type:
		return SerArPtr(new SerStr);
		break;
	default:
		throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of SerAr.", "SerAr container");
	}

}



} // namespace XLS
