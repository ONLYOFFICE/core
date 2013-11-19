#include "stdafx.h"
#include "LbsDropData.h"
#include <XLS_bin/CFRecord.h>
#include <Exception/AttributeDataWrong.h>

namespace XLS
{;


BiffStructurePtr LbsDropData::clone()
{
	return BiffStructurePtr(new LbsDropData(*this));
}


void LbsDropData::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	own_tag->setAttribute(L"wStyle", wStyle);
	own_tag->setAttribute(L"fFiltered", fFiltered);
	own_tag->setAttribute(L"cLine", cLine);
	own_tag->setAttribute(L"dxMin", dxMin);
	own_tag->setAttribute(L"str", str);
}


void LbsDropData::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	wStyle = getStructAttribute(own_tag, L"wStyle");
	fFiltered = getStructAttribute(own_tag, L"fFiltered");
	cLine = getStructAttribute(own_tag, L"cLine");
	dxMin = getStructAttribute(own_tag, L"dxMin");
	str = static_cast<_bstr_t>(getStructAttribute(own_tag, L"str"));
}


void LbsDropData::store(CFRecord& record)
{
	WORD flags = 0;
	SETBITS(flags, 0, 1, wStyle);
	SETBIT(flags, 3, fFiltered);

	record << flags << cLine << dxMin << str;

	if(0 != (str.getStructSize() & 1))
	{
		record.reserveNBytes(1); // padding
	}
}


void LbsDropData::load(CFRecord& record)
{
	WORD flags;
	record >> flags >> cLine >> dxMin >> str;
	wStyle = GETBITS(flags, 0, 1);
	fFiltered = GETBIT(flags, 3);

	if(0 != (str.getStructSize() & 1))
	{
		record.skipNBytes(1); // padding
	}

}


} // namespace XLS

