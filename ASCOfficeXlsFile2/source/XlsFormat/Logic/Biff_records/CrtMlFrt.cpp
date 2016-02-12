
#include "CrtMlFrt.h"

namespace XLS
{

CrtMlFrt::CrtMlFrt()
{
}


CrtMlFrt::~CrtMlFrt()
{
}


BaseObjectPtr CrtMlFrt::clone()
{
	return BaseObjectPtr(new CrtMlFrt(*this));
}


void CrtMlFrt::writeFields(CFRecord& record)
{

}


void CrtMlFrt::readFields(CFRecord& record)
{
#pragma message("####################### CrtMlFrt record is not implemented")
	Log::error("CrtMlFrt record is not implemented.");
	
	record.skipNunBytes(record.getDataSize() - record.getRdPtr());
//
//	if (cb > 0)
//	{
//		//XmlTkChain
//	}
}

} // namespace XLS

