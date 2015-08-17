
#include "RC4CryptoAPIEncryptionHeader.h"
#include <Binary/CFRecord.h>

namespace CRYPTO
{


XLS::BiffStructurePtr RC4CryptoAPIEncryptionHeader::clone()
{
	return XLS::BiffStructurePtr(new RC4CryptoAPIEncryptionHeader(*this));
}

//
//void RC4CryptoAPIEncryptionHeader::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"vMajor", EncryptionVersionInfo.vMajor);
//	own_tag->setAttribute(L"vMinor", EncryptionVersionInfo.vMinor);
//	Flags.toXML(own_tag);
//	own_tag->setAttribute(L"EncryptionHeaderSize", EncryptionHeaderSize);
//	encryptionHeader.toXML(own_tag);
//	encryptionVerifier.toXML(own_tag);
//}
//
//
//const bool RC4CryptoAPIEncryptionHeader::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### RC4CryptoAPIEncryptionHeader record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! RC4CryptoAPIEncryptionHeader record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void RC4CryptoAPIEncryptionHeader::store(XLS::CFRecord& record)
{
#pragma message("####################### RC4CryptoAPIEncryptionHeader record has no BiffStructure::store() implemented")
	Log::error(" Error!!! RC4CryptoAPIEncryptionHeader record has no BiffStructure::store() implemented.");
	//record << something;
}


void RC4CryptoAPIEncryptionHeader::load(XLS::CFRecord& record)
{
	record >> EncryptionVersionInfo >> Flags >> EncryptionHeaderSize >> encryptionHeader >> encryptionVerifier;
}


} // namespace CRYPTO

