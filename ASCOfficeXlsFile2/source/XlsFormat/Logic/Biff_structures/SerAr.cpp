
#include "SerAr.h"
#include <Binary/CFRecord.h>
#include "SerBool.h"
#include "SerErr.h"
#include "SerNil.h"
#include "SerNum.h"
#include "SerStr.h"
//#include <Exception/WrongBiffRecord.h>

namespace XLS
{

// static 
SerArPtr SerAr::createSerAr(const unsigned char type)
{
	switch(type)
	{
    case typeSerBool:
		return SerArPtr(new SerBool);
		break;
    case typeSerErr:
		return SerArPtr(new SerErr);
		break;
    case typeSerNil:
		return SerArPtr(new SerNil);
		break;
    case typeSerNum:
		return SerArPtr(new SerNum);
		break;
    case typeSerStr:
		return SerArPtr(new SerStr);
		break;
	default:
		throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of SerAr.", "SerAr container");
	}

}



} // namespace XLS
