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
	OfficeArtContainer(const unsigned char recVer, const unsigned short recType, const OfficeArtClientAnchorType anchor_type);

	static const XLS::ElementType	type = XLS::typeOfficeArtContainer;

	// overridden:
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	static OfficeArtRecordPtr loadAnyArtRecord(XLS::CFRecord& record);

	OfficeArtClientAnchorType			anchor_type_;
	std::vector<OfficeArtRecordPtr>		child_records;

};

typedef boost::shared_ptr<OfficeArtContainer> OfficeArtContainerPtr;


} // namespace ODRAW
