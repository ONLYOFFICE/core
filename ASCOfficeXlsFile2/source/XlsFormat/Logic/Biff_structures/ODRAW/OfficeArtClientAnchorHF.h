#pragma once

#include "OfficeArtRecord.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

class OfficeArtClientAnchorHF : public OfficeArtRecord
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtClientAnchorHF)
public:
	OfficeArtClientAnchorHF();
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

private:
	long width;
	long height;
};

typedef boost::shared_ptr<OfficeArtClientAnchorHF> OfficeArtClientAnchorHFPtr;


} // namespace XLS
