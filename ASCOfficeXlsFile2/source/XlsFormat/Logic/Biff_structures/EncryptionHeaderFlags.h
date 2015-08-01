#pragma once

#include "BiffStructure.h"
//#include <Logic/Biff_structures/Phs.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace CRYPTO
{;

class CFRecord;

class EncryptionHeaderFlags : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(EncryptionHeaderFlags)
public:
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeEncryptionHeaderFlags;

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	bool fCryptoAPI;
	bool fDocProps;
	bool fExternal;
	bool fAES;
};

} // namespace CRYPTO

