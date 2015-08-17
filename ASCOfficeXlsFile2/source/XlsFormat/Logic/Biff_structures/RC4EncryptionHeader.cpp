
#include "RC4EncryptionHeader.h"
#include <Binary/CFRecord.h>

namespace CRYPTO
{


XLS::BiffStructurePtr RC4EncryptionHeader::clone()
{
	return XLS::BiffStructurePtr(new RC4EncryptionHeader(*this));
}


//void RC4EncryptionHeader::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"vMajor", EncryptionVersionInfo.vMajor);
//	own_tag->setAttribute(L"vMinor", EncryptionVersionInfo.vMinor);
//	own_tag->setAttribute(L"Salt", (STR::int2hex_wstr(Salt.b1) + STR::int2hex_wstr(Salt.b2) + 
//							STR::int2hex_wstr(Salt.b3) + STR::int2hex_wstr(Salt.b4)).c_str());
//	own_tag->setAttribute(L"EncryptedVerifier", (STR::int2hex_wstr(EncryptedVerifier.b1) + STR::int2hex_wstr(EncryptedVerifier.b2) + 
//							STR::int2hex_wstr(EncryptedVerifier.b3) + STR::int2hex_wstr(EncryptedVerifier.b4)).c_str());
//	own_tag->setAttribute(L"EncryptedVerifierHash", (STR::int2hex_wstr(EncryptedVerifierHash.b1) + STR::int2hex_wstr(EncryptedVerifierHash.b2) + 
//							STR::int2hex_wstr(EncryptedVerifierHash.b3) + STR::int2hex_wstr(EncryptedVerifierHash.b4)).c_str());
//}
//
//
//const bool RC4EncryptionHeader::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### RC4EncryptionHeader record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! RC4EncryptionHeader record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void RC4EncryptionHeader::store(XLS::CFRecord& record)
{
#pragma message("####################### RC4EncryptionHeader record has no BiffStructure::store() implemented")
	Log::error(" Error!!! RC4EncryptionHeader record has no BiffStructure::store() implemented.");
	//record << something;
}


void RC4EncryptionHeader::load(XLS::CFRecord& record)
{
	record >> EncryptionVersionInfo;
	record.loadAnyData(Salt);
	record.loadAnyData(EncryptedVerifier);
	record.loadAnyData(EncryptedVerifierHash);
}


} // namespace CRYPTO

