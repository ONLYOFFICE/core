
#include "ChartNumNillable.h"
#include <Binary/CFRecord.h>
//#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;


ChartNumNillable::ChartNumNillable(const unsigned short nil_type)
:	nil_type_(nil_type)
{
}


BiffStructurePtr ChartNumNillable::clone()
{
	return BiffStructurePtr(new ChartNumNillable(*this));
}


//void ChartNumNillable::toXML(BiffStructurePtr & parent, const std::wstring & attrib_name)
//{
//	if(0xffff != data.fExprO)
//	{
//		parent->setAttribute(attrib_name, data.xnum);
//	}
//}


//const bool ChartNumNillable::fromXML(BiffStructurePtr & parent, const std::wstring & attrib_name)
//{
//	AUX::BetterVariantT num = getStructAttribute(parent, attrib_name);
//
//	if(VT_NULL != num.vt)
//	{
//		data.xnum = num;
//	}
//	else
//	{
//		data.Byte1 = 0;
//		data.Byte2 = 0;
//		data.Byte3 = 0;
//		data.Byte4 = 0;
//		data.type = nil_type_;
//		data.fExprO = 0xffff;
//	}
//
//	return true;
//}
//
//
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

