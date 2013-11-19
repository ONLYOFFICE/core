#pragma once

#include "../BiffStructTagged.h"
#include "OfficeArtCOLORREF.h"
#include "../FixedPoint.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;

class OfficeArtMetafileHeader : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtMetafileHeader)
public:
	XLS::BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void store(XLS::CFRecord& record);
	virtual void load(XLS::CFRecord& record);

public:
	DWORD cbSize;
	RECT rcBounds;
	POINT ptSize;
	DWORD cbSave;
	BYTE compression;
	BYTE filter;
};

typedef boost::shared_ptr<OfficeArtMetafileHeader> OfficeArtMetafileHeaderPtr;







} // namespace XLS
