
#include "AxcExt.h"

namespace XLS
{

AxcExt::AxcExt()
{
}


AxcExt::~AxcExt()
{
}


BaseObjectPtr AxcExt::clone()
{
	return BaseObjectPtr(new AxcExt(*this));
}


void AxcExt::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fAutoMin);
	SETBIT(flags, 1, fAutoMax);
	SETBIT(flags, 2, fAutoMajor);
	SETBIT(flags, 3, fAutoMinor);
	SETBIT(flags, 4, fDateAxis);
	SETBIT(flags, 5, fAutoBase);
	SETBIT(flags, 6, fAutoCross);
	SETBIT(flags, 7, fAutoDate);
	record >> catMin >> catMax >> catMajor >> duMajor >> catMinor >> duMinor >> duBase >> catCrossDate >> flags;

}


void AxcExt::readFields(CFRecord& record)
{
	unsigned char flags, reserved;
	record >> catMin >> catMax >> catMajor >> duMajor >> catMinor >> duMinor >> duBase >> catCrossDate >> flags >> reserved;

	fAutoMin	= GETBIT(flags, 0);
	fAutoMax	= GETBIT(flags, 1);
	fAutoMajor	= GETBIT(flags, 2);
	fAutoMinor	= GETBIT(flags, 3);
	
	fDateAxis	= GETBIT(flags, 4);
	fAutoBase	= GETBIT(flags, 5);
	fAutoCross	= GETBIT(flags, 6);
	fAutoDate	= GETBIT(flags, 7);
}

int AxcExt::serialize(std::wostream & _stream)
{
	if (fDateAxis == false) return 0;
	
	CP_XML_WRITER(_stream)    
	{
		if (fAutoMax == false)
		{
			CP_XML_NODE(L"c:max") 
			{ 
				CP_XML_ATTR(L"val", catMax);
			}
		}			
		if (fAutoMin == false)
		{
			CP_XML_NODE(L"c:min") 
			{
				CP_XML_ATTR(L"val", catMin); 
			}
		}
		std::wstring DateUnit[] = {L"days", L"months", L"years"};
		
		if ( fAutoMajor == false )
		{
			CP_XML_NODE(L"c:majorUnit")
			{
				CP_XML_ATTR(L"val", catMajor);
			}
		} 
		if ( fAutoMinor == false )
		{
			CP_XML_NODE(L"c:minorUnit")
			{
				CP_XML_ATTR(L"val", catMinor);
			}
		}	
		if ( fAutoBase == false)
		{
			CP_XML_NODE(L"c:baseTimeUnit")
			{
				CP_XML_ATTR(L"val", DateUnit[duBase]);
			}
		}
		CP_XML_NODE(L"c:majorTimeUnit")
		{
			CP_XML_ATTR(L"val", DateUnit[duMajor]);
		}
		CP_XML_NODE(L"c:minorTimeUnit")
		{
			CP_XML_ATTR(L"val", DateUnit[duMinor]);
		}
	}
	return 0;
}

} // namespace XLS

