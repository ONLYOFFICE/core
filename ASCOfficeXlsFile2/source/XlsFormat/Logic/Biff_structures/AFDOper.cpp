
#include "AFDOper.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr AFDOper::clone()
{
	return BiffStructurePtr(new AFDOper(*this));
}

AFDOper::AFDOper()
{	
	m_bAutoFilter	= true;
	vt				= 0xff;
	grbitSign		= 0xff;
}

AFDOper::AFDOper(bool bAutoFilter)
{
	m_bAutoFilter = bAutoFilter;
}


AFDOper::~AFDOper()
{
}

void AFDOper::store(CFRecord& record)
{	
}


void AFDOper::load(CFRecord& record)
{
	record >> vt >> grbitSign;

	switch(vt)
	{
		case  0x02:
		{
			record >> vtValueRk;
		}break;
		case  0x04:
		{
			record >> vtValueNum;
		}break;
		case  0x06:
		{
			vtValueStr.m_bAutoFilter = m_bAutoFilter;
			record >> vtValueStr;
		}break;
		case  0x08:
		{
			record >> vtValueBool;
		}break;
		case 0x0C:	//All blanks are matched.
		case 0x0E:	//All non-blanks are matched.
		case 0x00:
		default:
		{
			record.skipNunBytes(8);
		}break;
	}
}


} // namespace XLS

