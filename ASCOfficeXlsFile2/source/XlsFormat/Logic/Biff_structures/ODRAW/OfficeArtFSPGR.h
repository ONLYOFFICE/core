#pragma once

#include "OfficeArtRecord.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

class OfficeArtFSPGR : public OfficeArtRecord
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtFSPGR)
public:
	OfficeArtFSPGR();
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

private:
	long xLeft;
	long yTop;
	long xRight;
	long yBottom;
};

typedef boost::shared_ptr<OfficeArtFSPGR> OfficeArtFSPGRPtr;


} // namespace XLS
