#pragma once

#include "BiffStructure.h"
//#include <XLS_logic/Biff_structures/Phs.h>
//#include <XLS_logic/Biff_structures/BiffString.h>

namespace OSHARED
{;

class CFRecord;

class URICreateFlags : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(URICreateFlags)
public:
	URICreateFlags();
	URICreateFlags(XLS::CFRecord& record);
	XLS::BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
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

