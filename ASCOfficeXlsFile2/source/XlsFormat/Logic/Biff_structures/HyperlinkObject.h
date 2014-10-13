#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/HyperlinkMoniker.h>
#include <Logic/Biff_structures/BiffString.h>

namespace OSHARED
{;

class CFRecord;

class HyperlinkObject : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(HyperlinkObject)
public:
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	unsigned int streamVersion;

	bool hlstmfHasMoniker;
	bool hlstmfIsAbsolute;
	bool hlstmfSiteGaveDisplayName;
	bool hlstmfHasLocationStr;
	bool hlstmfHasDisplayName;
	bool hlstmfHasGUID;
	bool hlstmfHasCreationTime;
	bool hlstmfHasFrameName;
	bool hlstmfMonikerSavedAsStr;
	bool hlstmfAbsFromGetdataRel;

	HyperlinkString displayName;
	HyperlinkString targetFrameName;
	HyperlinkString moniker;
	HyperlinkMoniker oleMoniker;
	HyperlinkString location;
	std::wstring  guid;
	FILETIME fileTime;

};

} // namespace OSHARED

