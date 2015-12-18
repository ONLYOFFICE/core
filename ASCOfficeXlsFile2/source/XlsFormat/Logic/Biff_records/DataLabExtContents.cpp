
#include "DataLabExtContents.h"
#include <Logic/Biff_structures/FrtHeader.h>

namespace XLS
{

DataLabExtContents::DataLabExtContents()
{
	is_area = false;
}


DataLabExtContents::~DataLabExtContents()
{
}


BaseObjectPtr DataLabExtContents::clone()
{
	return BaseObjectPtr(new DataLabExtContents(*this));
}


void DataLabExtContents::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_DataLabExtContents);
	record << frtHeader;

	unsigned short flags = 0;
	SETBIT(flags, 0, fSerName);
	SETBIT(flags, 1, fCatName);
	SETBIT(flags, 2, fValue);
	SETBIT(flags, 3, fPercent);
	SETBIT(flags, 4, fBubSizes);

	record << flags << rgchSep;
}


void DataLabExtContents::readFields(CFRecord& record)
{
	FrtHeader frtHeader;
	record >> frtHeader;

	unsigned short flags;
	record >> flags >> rgchSep;

	fSerName	= GETBIT(flags, 0);
	fCatName	= GETBIT(flags, 1);
	fValue		= GETBIT(flags, 2);
	fPercent	= GETBIT(flags, 3);
	fBubSizes	= GETBIT(flags, 4);
}

int DataLabExtContents::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:showVal")
		{
			CP_XML_ATTR (L"val" , fValue); 
		}
		CP_XML_NODE(L"c:showPercent")
		{
			CP_XML_ATTR (L"val" , fPercent); 
		}
		CP_XML_NODE(L"c:showBubbleSize")
		{
			CP_XML_ATTR (L"val" , fBubSizes); 
		}

		if (rgchSep.value().empty() == false)
		{
			CP_XML_NODE(L"c:separator")
			{
				CP_XML_STREAM() << rgchSep.value();
			}
		}
		if (is_area == false)
		{
			CP_XML_NODE(L"c:showCatName")
			{
				CP_XML_ATTR (L"val" , fCatName); 
			}		
			CP_XML_NODE(L"c:showSerName") {	CP_XML_ATTR (L"val" , 0); }	
		}
		else
		{
			CP_XML_NODE(L"c:showCatName") {	CP_XML_ATTR (L"val" , 0); }	
			CP_XML_NODE(L"c:showSerName")
			{
				CP_XML_ATTR (L"val" , fSerName); 
			}		
		}


	}
	return 0;
}

} // namespace XLS

