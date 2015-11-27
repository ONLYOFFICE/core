#pragma once

#include "OfficeArtRecord.h"
#include "OfficeArtMetafileHeader.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{

class OfficeArtBlip;
typedef boost::shared_ptr<OfficeArtBlip> OfficeArtBlipPtr;

class OfficeArtBlip : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtBlip)
public:
	OfficeArtBlip(const unsigned short recType);
	~OfficeArtBlip();

	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record){}
	
	XLS::BiffStructurePtr clone()
	{
		return XLS::BiffStructurePtr(new OfficeArtBlip(recType));
	}

	static const XLS::ElementType	type = XLS::typeOfficeArtBlip;

	size_t			pict_size;
	std::wstring	pict_type;
	char			*pict_data;

	void	readCompressedData(XLS::CFRecord& record, OfficeArtMetafileHeader & metafileHeader);
	
	const	std::wstring ReadMD4Digest(XLS::CFRecord& record)
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
	size_t			recType;	
	std::wstring	rgbUid1;
	std::wstring	rgbUid2;
	bool			result;
};


} // namespace ODRAW
