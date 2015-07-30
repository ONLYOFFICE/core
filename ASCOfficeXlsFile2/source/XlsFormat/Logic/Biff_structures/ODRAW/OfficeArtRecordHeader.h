#pragma once

#include "../BiffStructure.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

class OfficeArtRecordHeader : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtRecordHeader)
public:
	XLS::BiffStructurePtr clone();
	
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	const size_t size() const;

	unsigned char	recVer;
	unsigned short	recInstance;
	unsigned short	recType;
	unsigned int	recLen;
};

typedef boost::shared_ptr<OfficeArtRecordHeader> OfficeArtRecordHeaderPtr;


} // namespace ODRAW
