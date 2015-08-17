
#include "EncryptionHeader.h"
#include <Binary/CFRecord.h>

namespace CRYPTO
{;


XLS::BiffStructurePtr EncryptionHeader::clone()
{
	return XLS::BiffStructurePtr(new EncryptionHeader(*this));
}



void EncryptionHeader::store(XLS::CFRecord& record)
{
#pragma message("####################### EncryptionHeader record has no BiffStructure::store() implemented")
	Log::error(" Error!!! EncryptionHeader record has no BiffStructure::store() implemented.");
	//record << something;
}


void EncryptionHeader::load(XLS::CFRecord& record)
{
	record.skipNunBytes(2); // reserved
	unsigned short flags;
	record >> flags;
	record >> Flags;
	record. skipNunBytes(4); // reserved (SizeExtra)
	record >> AlgID >> AlgIDHash >> KeySize >> ProviderType;
	record. skipNunBytes(8); // reserved
	record >> CSPName;
}


} // namespace CRYPTO

