#include "stdafx.h"
#include "EncryptionVerifier.h"
#include <XLS_bin/CFRecord.h>

namespace CRYPTO
{;


XLS::BiffStructurePtr EncryptionVerifier::clone()
{
	return XLS::BiffStructurePtr(new EncryptionVerifier(*this));
}


void EncryptionVerifier::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"SaltSize", SaltSize);
	own_tag->setAttribute(L"Salt", (STR::int2hex_str(Salt.b1) + STR::int2hex_str(Salt.b2) + 
								STR::int2hex_str(Salt.b3) + STR::int2hex_str(Salt.b4)).c_str());
	own_tag->setAttribute(L"EncryptedVerifier", (STR::int2hex_str(EncryptedVerifier.b1) + STR::int2hex_str(EncryptedVerifier.b2) + 
							STR::int2hex_str(EncryptedVerifier.b3) + STR::int2hex_str(EncryptedVerifier.b4)).c_str());
	own_tag->setAttribute(L"VerifierHashSize", VerifierHashSize);
	own_tag->setAttribute(L"EncryptedVerifierHash", (STR::int2hex_str(EncryptedVerifierHash.b1) + STR::int2hex_str(EncryptedVerifierHash.b2) + 
							STR::int2hex_str(EncryptedVerifierHash.b3) + STR::int2hex_str(EncryptedVerifierHash.b4)).c_str());
}


const bool EncryptionVerifier::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### EncryptionVerifier record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! EncryptionVerifier record has no BiffStructure::fromXML() implemented.");
	return false;
}


void EncryptionVerifier::store(XLS::CFRecord& record)
{
#pragma message("####################### EncryptionVerifier record has no BiffStructure::store() implemented")
	Log::error(" Error!!! EncryptionVerifier record has no BiffStructure::store() implemented.");
	//record << something;
}


void EncryptionVerifier::load(XLS::CFRecord& record)
{
	record >> SaltSize;
	record.loadAnyData(Salt);
	record.loadAnyData(EncryptedVerifier);
	record >> VerifierHashSize;
	record.loadAnyData(EncryptedVerifierHash);
}


} // namespace CRYPTO

