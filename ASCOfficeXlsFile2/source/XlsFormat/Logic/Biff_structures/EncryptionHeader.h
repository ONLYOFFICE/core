#pragma once

#include "BiffStructure.h"
#include "EncryptionHeaderFlags.h"

namespace CRYPTO
{;

class CFRecord;

class EncryptionHeader : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(EncryptionHeader)
public:
	XLS::BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	EncryptionHeaderFlags Flags;
	long AlgID;
	long AlgIDHash;
	unsigned int KeySize;
	unsigned int ProviderType;
	std::wstring CSPName;
};

} // namespace CRYPTO

