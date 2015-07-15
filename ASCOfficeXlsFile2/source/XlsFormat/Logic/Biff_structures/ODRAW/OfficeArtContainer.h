#pragma once

#include "OfficeArtRecord.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

class OfficeArtContainer : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtContainer)
public:
	OfficeArtContainer(const unsigned char recVer, const unsigned __int16 recType, const OfficeArtClientAnchorType anchor_type);

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	// overridden:
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	static OfficeArtRecordPtr loadAnyArtRecord(XLS::CFRecord& record);

private:

	std::vector<OfficeArtRecordPtr> child_records;
	OfficeArtClientAnchorType anchor_type_;
};

typedef boost::shared_ptr<OfficeArtContainer> OfficeArtContainerPtr;


} // namespace ODRAW
