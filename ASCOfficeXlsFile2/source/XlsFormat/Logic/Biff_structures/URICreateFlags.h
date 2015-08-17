#pragma once

#include "BiffStructure.h"
//#include <Logic/Biff_structures/Phs.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace OSHARED
{

class CFRecord;

class URICreateFlags : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(URICreateFlags)
public:
	URICreateFlags();
	URICreateFlags(XLS::CFRecord& record);
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeURICreateFlags;

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	bool createAllowRelative;
	bool createAllowImplicitWildcardScheme;
	bool createAllowImplicitFileScheme;
	bool createNoFrag;
	bool createNoCanonicalize;
	bool createCanonicalize;
	bool createFileUseDosPath;
	bool createDecodeExtraInfo;
	bool createNoDecodeExtraInfo;
	bool createCrackUnknownSchemes;
	bool createNoCrackUnknownSchemes;
	bool createPreProcessHtmlUri;
	bool createNoPreProcessHtmlUri;
	bool createIESettings;
	bool createNoIESettings;
	bool createNoEncodeForbiddenCharacters;
};

} // namespace OSHARED

