#pragma once

#include "OfficeArtRecord.h"
#include <XLS_bin/CFRecord.h>
#include "OfficeArtRecordHeader.h"
#include "OfficeArtBlip.h"


namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;

class OfficeArtBStoreContainerFileBlock : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtBStoreContainerFileBlock)
public:
	XLS::BiffStructurePtr clone();

	OfficeArtBStoreContainerFileBlock()
	{
		data_size = 0;
		pict_data = NULL;
		pict_type = L"jpeg";
		rgbUid1 = "";
		rgbUid2 = "";
		result = false;
	}

	~OfficeArtBStoreContainerFileBlock()
	{
		if (pict_data)
		{
			delete[] pict_data;
			pict_data = NULL;
		}
	}

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	void readCompressedData(XLS::CFRecord& record, OfficeArtMetafileHeader metafileHeader);

private:
	const std::string ReadMD4Digest(XLS::CFRecord& record)
	{
		std::string result;
		for(int i = 0; i < 16; i++)
		{
			BYTE hex_data;
			record >> hex_data;
			result += STR::int2hex_str(hex_data, sizeof(hex_data));
		}
		return result;
	}

private:
	int bin_data_id;

	size_t data_size;
	char* pict_data;
	_bstr_t pict_type;

public:
	size_t recType;	
	std::string rgbUid1;
	std::string rgbUid2;
	bool result;
};

typedef boost::shared_ptr<OfficeArtBStoreContainerFileBlock> OfficeArtBStoreContainerFileBlockPtr;


} // namespace XLS
