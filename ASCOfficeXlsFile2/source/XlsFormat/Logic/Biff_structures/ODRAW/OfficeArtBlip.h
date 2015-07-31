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
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtBlip)
public:
	OfficeArtBlip(const unsigned short recType);

	static OfficeArtBlipPtr load_blip(XLS::CFRecord& record);
	
	static const XLS::ElementType	type = XLS::typeOfficeArtBlip;

	unsigned int csp;
	unsigned int spidCur;
};


typedef enum
{
	bpi_HasMetaData,
	bpi_HasInternalTag,
} BLIP_PICT_INFO;

template<BLIP_PICT_INFO pict_info, unsigned short recType>
class OfficeArtBlipCommon : public OfficeArtBlip
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtBlip)
public:
	OfficeArtBlipCommon() : OfficeArtBlip(recType) {}
	XLS::BiffStructurePtr clone()
	{
		return XLS::BiffStructurePtr(new OfficeArtBlipCommon<pict_info, recType>(*this));
	}



	virtual void loadFields(XLS::CFRecord& record)
	{
		size_t end = record.getRdPtr() + rh_own.recLen;
		std::wstring rgbUid1 = ReadMD4Digest(record);
		std::wstring rgbUid2 = ReadMD4Digest(record);
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
		record.skipNunBytes(end - record.getRdPtr());
	}

	virtual void storeFields(XLS::CFRecord& record)
	{
		// We don't support xlsx to xls conversion
	}

private:
	const std::wstring ReadMD4Digest(XLS::CFRecord& record)
	{
		std::wstring result;
		for(int i = 0; i < 16; i++)
		{
			unsigned char hex_data;
			record >> hex_data;
			result += STR::int2hex_wstr(hex_data, sizeof(hex_data));
		}
		return result;
	}

private:
	std::wstring rgbUid;
	unsigned char tag;
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
