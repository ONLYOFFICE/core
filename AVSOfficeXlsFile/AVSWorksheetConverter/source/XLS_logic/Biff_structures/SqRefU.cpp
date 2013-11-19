#include "stdafx.h"
#include "SqRefU.h"
#include "CellRangeRef.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr SqRefU::clone()
{
	return BiffStructurePtr(new SqRefU(*this));
}


void SqRefU::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"sqref", sqref);
}


void SqRefU::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	sqref = getStructAttribute(xml_tag, L"sqref");
}


void SqRefU::store(CFRecord& record)
{
	std::vector<CellRangeRef> refs;
	AUX::str2refs(static_cast<wchar_t*>(static_cast<_bstr_t>(sqref)), refs);

	WORD cref = refs.size();
	record << cref;
	for(std::vector<CellRangeRef>::const_iterator it = refs.begin(), itEnd = refs.end(); it != itEnd ; ++it)
	{
		Ref8U ref8u(*it);
		record << ref8u;
	}
}


void SqRefU::load(CFRecord& record)
{
	WORD cref;
	record >> cref;
	_bstr_t sqref_str;
	for (size_t i = 0; i < cref ; ++i)
	{
		Ref8U ref8;
		record >> ref8;
		sqref_str += _bstr_t(ref8.toString(false).c_str()) + ((i == cref - 1) ? L"" : L" ");
	}
	sqref = sqref_str;
}


const CellRef SqRefU::getLocationFirstCell() const
{
	std::vector<CellRangeRef> refs;
	AUX::str2refs(static_cast<wchar_t*>(static_cast<_bstr_t>(sqref)), refs);
	if(!refs.size())
	{
		return CellRef();
	}
	else
	{
		return refs[0].getTopLeftCell();
	}
}

} // namespace XLS

