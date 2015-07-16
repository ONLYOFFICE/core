#include "precompiled_xls.h"
#include "EncryptionHeaderFlags.h"
#include <Binary/CFRecord.h>

namespace CRYPTO
{;


XLS::BiffStructurePtr EncryptionHeaderFlags::clone()
{
	return XLS::BiffStructurePtr(new EncryptionHeaderFlags(*this));
}

//
//void EncryptionHeaderFlags::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"fCryptoAPI", fCryptoAPI);
//	own_tag->setAttribute(L"fDocProps", fDocProps);
//	own_tag->setAttribute(L"fExternal", fExternal);
//	own_tag->setAttribute(L"fAES", fAES);
//}
//
//
//const bool EncryptionHeaderFlags::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### EncryptionHeaderFlags record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! EncryptionHeaderFlags record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void EncryptionHeaderFlags::store(XLS::CFRecord& record)
{
#pragma message("####################### EncryptionHeaderFlags record has no BiffStructure::store() implemented")
	Log::error(" Error!!! EncryptionHeaderFlags record has no BiffStructure::store() implemented.");
	//record << something;
}


void EncryptionHeaderFlags::load(XLS::CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fCryptoAPI = GETBIT(flags, 2);
	fDocProps = GETBIT(flags, 3);
	fExternal = GETBIT(flags, 4);
	fAES = GETBIT(flags, 5);
}


} // namespace CRYPTO

