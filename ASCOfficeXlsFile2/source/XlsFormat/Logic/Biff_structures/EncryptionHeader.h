#pragma once

#include "BiffStructure.h"
#include "EncryptionHeaderFlags.h"

namespace CRYPTO
{

class CFRecord;

class EncryptionHeader : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(EncryptionHeader)
public:
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeEncryptionHeader;

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	EncryptionHeaderFlags Flags;
    int AlgID;
    int AlgIDHash;
	unsigned int KeySize;
	unsigned int ProviderType;
	std::wstring CSPName;
};

} // namespace CRYPTO

