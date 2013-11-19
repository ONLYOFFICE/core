#pragma once

#include "OfficeArtRecord.h"
#include "OfficeArtMetafileHeader.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

class OfficeArtBlip;
typedef boost::shared_ptr<OfficeArtBlip> OfficeArtBlipPtr;

class OfficeArtBlip : public OfficeArtRecord
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtBlip)
public:
	OfficeArtBlip(const WORD recType);

	static OfficeArtBlipPtr load_blip(XLS::CFRecord& record);

public:
	DWORD csp;
	DWORD spidCur;
};


typedef enum
{
	bpi_HasMetaData,
	bpi_HasInternalTag,
} BLIP_PICT_INFO;

template<BLIP_PICT_INFO pict_info, WORD recType>
class OfficeArtBlipCommon : public OfficeArtBlip
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtBlip)
public:
	OfficeArtBlipCommon() : OfficeArtBlip(recType) {}
	XLS::BiffStructurePtr clone()
	{
		return XLS::BiffStructurePtr(new OfficeArtBlipCommon<pict_info, recType>(*this));
	}

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
	{
		xml_tag->setAttribute(L"rgbUid", rgbUid.c_str());
		switch(pict_info)
		{
			case bpi_HasMetaData:
				metafileHeader.toXML(xml_tag);
				break;

			case bpi_HasInternalTag:
				xml_tag->setAttribute(L"tag", tag);
				break;
		}

	}
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
	{
		// We don't support xlsx to xls conversion
	}

	virtual void loadFields(XLS::CFRecord& record)
	{
		size_t end = record.getRdPtr() + rh_own.recLen;
		std::string rgbUid1 = ReadMD4Digest(record);
		std::string rgbUid2 = ReadMD4Digest(record);
		rgbUid = 0 != (rh_own.recInstance & 1) ? rgbUid2 : rgbUid1;

		switch(pict_info)
		{
			case bpi_HasMetaData:
				record >> metafileHeader;
				break;

			case bpi_HasInternalTag:
				record >> tag;
				break;
		}
		record.skipNBytes(end - record.getRdPtr());
	}

	virtual void storeFields(XLS::CFRecord& record)
	{
		// We don't support xlsx to xls conversion
	}

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
	std::string rgbUid;
	BYTE tag;
	OfficeArtMetafileHeader metafileHeader;
};


typedef OfficeArtBlipCommon<bpi_HasInternalTag, OfficeArtRecord::BlipJPEG> OfficeArtBlipJPEG;
typedef OfficeArtBlipCommon<bpi_HasInternalTag, OfficeArtRecord::BlipDIB> OfficeArtBlipDIB;
typedef OfficeArtBlipCommon<bpi_HasInternalTag, OfficeArtRecord::BlipTIFF> OfficeArtBlipTIFF;
typedef OfficeArtBlipCommon<bpi_HasInternalTag, OfficeArtRecord::BlipPNG> OfficeArtBlipPNG;
typedef OfficeArtBlipCommon<bpi_HasMetaData, OfficeArtRecord::BlipPICT> OfficeArtBlipPICT;
typedef OfficeArtBlipCommon<bpi_HasMetaData, OfficeArtRecord::BlipWMF> OfficeArtBlipWMF;
typedef OfficeArtBlipCommon<bpi_HasMetaData, OfficeArtRecord::BlipEMF> OfficeArtBlipEMF;



} // namespace ODRAW
