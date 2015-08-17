
#include "CFMultistate.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr CFMultistate::clone()
{
	return BiffStructurePtr(new CFMultistate(*this));
}


//void CFMultistate::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"cStates", cStates);
//	xml_tag->setAttribute(L"iIconSet", iIconSet);
//	xml_tag->setAttribute(L"fIconOnly", fIconOnly);
//	xml_tag->setAttribute(L"fReverse", fReverse);
//	for(std::vector<CFMStateItemPtr>::iterator it = rgStates.begin(), itEnd = rgStates.end(); it != itEnd; ++it)
//	{
//		(*it)->toXML(xml_tag);
//	}
//}


//void CFMultistate::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	cStates = getStructAttribute(xml_tag, L"cStates");
//	iIconSet = getStructAttribute(xml_tag, L"iIconSet");
//	fIconOnly = getStructAttribute(xml_tag, L"fIconOnly");
//	fReverse = getStructAttribute(xml_tag, L"fReverse");
//	for(size_t i = 0; i <= 255; ++i)
//	{
//		CFMStateItemPtr item(new CFMStateItem);
//		if(!item->fromXML(xml_tag, i, false))
//		{
//			break;
//		}
//		rgStates.push_back(item);
//	}
//}


void CFMultistate::store(CFRecord& record)
{
	record.reserveNunBytes(2); // unused
	record.reserveNunBytes(1); // reserved
	record << cStates << iIconSet;
	unsigned char flags = 0;
	SETBIT(flags, 0, fIconOnly);
	SETBIT(flags, 2, fReverse);
	record << flags;

	for(std::vector<CFMStateItemPtr>::iterator it = rgStates.begin(), itEnd = rgStates.end(); it != itEnd; ++it)
	{
		(*it)->store(record);
	}
}


void CFMultistate::load(CFRecord& record)
{
	record.skipNunBytes(2); // unused
	record.skipNunBytes(1); // reserved
	record >> cStates >> iIconSet;
	unsigned char flags;
	record >> flags;
	fIconOnly = GETBIT(flags, 0);
	fReverse = GETBIT(flags, 2);

	for(unsigned char i = 0; i < cStates; ++i)
	{
		CFMStateItemPtr item(new CFMStateItem);
		item->load(record);
		rgStates.push_back(item);
	}
}


} // namespace XLS

