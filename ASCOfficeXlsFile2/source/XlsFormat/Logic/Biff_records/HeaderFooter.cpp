
#include "HeaderFooter.h"
#include <Logic/Biff_structures/FrtHeader.h>
//#include <Exception/AttributeDataWrong.h>

namespace XLS
{

HeaderFooter::HeaderFooter()
{
}


HeaderFooter::~HeaderFooter()
{
}


BaseObjectPtr HeaderFooter::clone()
{
	return BaseObjectPtr(new HeaderFooter(*this));
}


void HeaderFooter::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_HeaderFooter);
	record << frtHeader;
	_GUID_ guid_num;
	if(!STR::bstr2guid(guidSView, guid_num))
	{
		throw;// EXCEPT::LE::AttributeDataWrong(L"guid", L"HeaderFooter", guidSView);
	}
	record << guid_num;
	unsigned short flags = 0;
	SETBIT(flags, 0, fHFDiffOddEven);
	SETBIT(flags, 1, fHFDiffFirst);
	SETBIT(flags, 2, fHFScaleWithDoc);
	SETBIT(flags, 3, fHFAlignMargins);
	record << flags;
	cchHeaderEven = strHeaderEven.getSize();
	cchFooterEven = strFooterEven.getSize();
	cchHeaderFirst = strHeaderFirst.getSize();
	cchFooterFirst = strFooterFirst.getSize();
	record << cchHeaderEven << cchFooterEven << cchHeaderFirst << cchFooterFirst;
	if(cchHeaderEven)
	{
		record << strHeaderEven;
	}
	if(cchFooterEven)
	{
		record << strFooterEven;
	}
	if(cchHeaderFirst)
	{
		record << strHeaderFirst;
	}
	if(cchFooterFirst)
	{
		record << strFooterFirst;
	}
}


void HeaderFooter::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_HeaderFooter);
	record >> frtHeader;
	unsigned short flags;
	_GUID_ guid_num;
	record >> guid_num >> flags;
	guidSView = STR::guid2bstr(guid_num);
	fHFDiffOddEven = GETBIT(flags, 0);
	fHFDiffFirst = GETBIT(flags, 1);
	fHFScaleWithDoc = GETBIT(flags, 2);
	fHFAlignMargins = GETBIT(flags, 3);

	record >> cchHeaderEven >> cchFooterEven >> cchHeaderFirst >> cchFooterFirst;
	if(cchHeaderEven)
	{
		record >> strHeaderEven;
	}
	if(cchFooterEven)
	{
		record >> strFooterEven;
	}
	if(cchHeaderFirst)
	{
		record >> strHeaderFirst;
	}
	if(cchFooterFirst)
	{
		record >> strFooterFirst;
	}
}

} // namespace XLS

