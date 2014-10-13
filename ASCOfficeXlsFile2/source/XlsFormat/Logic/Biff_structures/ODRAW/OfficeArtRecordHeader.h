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
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtRecordHeader)
public:
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	const size_t size() const;

public:
	unsigned char recVer;
	unsigned __int16 recInstance;
	unsigned __int16 recType;
	unsigned int recLen;
};

typedef boost::shared_ptr<OfficeArtRecordHeader> OfficeArtRecordHeaderPtr;


} // namespace ODRAW
