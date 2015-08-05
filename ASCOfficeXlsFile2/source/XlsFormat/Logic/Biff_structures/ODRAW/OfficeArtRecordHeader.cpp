
#include "OfficeArtRecordHeader.h"
#include <Binary/CFRecord.h>
#include <Binary/CFRecordType.h>

namespace ODRAW
{;

OfficeArtRecordHeader::OfficeArtRecordHeader()
{
	recVer = recInstance = recType = recLen = 0;
}

XLS::BiffStructurePtr OfficeArtRecordHeader::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtRecordHeader(*this));
}

void OfficeArtRecordHeader::store(XLS::CFRecord& record)
{
	unsigned short ver_inst;
	SETBITS(ver_inst, 0, 3, recVer);
	SETBITS(ver_inst, 4, 15, recInstance);
	record << ver_inst << recType;
	record.registerDelayedDataReceiver(NULL, sizeof(recLen), XLS::rt_MsoDrawing);
}


void OfficeArtRecordHeader::load(XLS::CFRecord& record)
{
	unsigned short ver_inst;
	record >> ver_inst >> recType >> recLen;
	recVer		= GETBITS(ver_inst, 0, 3);
	recInstance = GETBITS(ver_inst, 4, 15);
}


const size_t OfficeArtRecordHeader::size() const
{
	return sizeof(unsigned short) + sizeof(recType) + sizeof(recLen);
}

} // namespace XLS
