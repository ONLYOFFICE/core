
#include "HLink.h"
//#include <Exception/AttributeDataWrong.h>

namespace XLS
{;

HLink::HLink()
{
}


HLink::~HLink()
{
}


BaseObjectPtr HLink::clone()
{
	return BaseObjectPtr(new HLink(*this));
}


void HLink::writeFields(CFRecord& record)
{
	static _GUID_ StdLink_CLSID = {0x79EAC9D0, 0xBAF9, 0x11CE, {0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B}};
	record << ref8 << StdLink_CLSID << hyperlink;
}


void HLink::readFields(CFRecord& record)
{
	_GUID_ clsid;
	record >> ref8 >> clsid >> hyperlink;
	hlinkClsid = STR::guid2bstr(clsid);
}

} // namespace XLS

