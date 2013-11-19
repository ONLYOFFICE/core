#include "stdafx.h"
#include "ChartNumNillable.h"
#include <XLS_bin/CFRecord.h>
//#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;


ChartNumNillable::ChartNumNillable(const WORD nil_type)
:	nil_type_(nil_type)
{
}


BiffStructurePtr ChartNumNillable::clone()
{
	return BiffStructurePtr(new ChartNumNillable(*this));
}


void ChartNumNillable::toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
{
	if(0xffff != data.fExprO)
	{
		parent->setAttribute(attrib_name, data.xnum);
	}
}


const bool ChartNumNillable::fromXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
{
	AUX::BetterVariantT num = getStructAttribute(parent, attrib_name);

	if(VT_NULL != num.vt)
	{
		data.xnum = num;
	}
	else
	{
		data.byte1 = 0;
		data.byte2 = 0;
		data.byte3 = 0;
		data.byte4 = 0;
		data.type = nil_type_;
		data.fExprO = 0xffff;
	}

	return true;
}


void ChartNumNillable::store(CFRecord& record)
{
	record << data.xnum;
}


void ChartNumNillable::load(CFRecord& record)
{
	record >> data.xnum;
}


const bool ChartNumNillable::isNil()
{
	return data.fExprO == 0xffff;
}


} // namespace XLS

