
#include "XFCRC.h"

namespace XLS
{

XFCRC::XFCRC()
{
}


XFCRC::~XFCRC()
{
}


BaseObjectPtr XFCRC::clone()
{
	return BaseObjectPtr(new XFCRC(*this));
}


void XFCRC::writeFields(CFRecord& record)
{
#pragma message("####################### XFCRC record is not implemented")
	Log::error("XFCRC record is not implemented.");
	//record << some_value;
}


void XFCRC::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	record.skipNunBytes(2); // reserved
	record >> cxfs >> crc;
}

} // namespace XLS

