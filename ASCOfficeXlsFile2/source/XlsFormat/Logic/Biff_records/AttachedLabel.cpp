//
#include "AttachedLabel.h"

namespace XLS
{

AttachedLabel::AttachedLabel()
{
	is_area = false;
}


AttachedLabel::~AttachedLabel()
{
}


BaseObjectPtr AttachedLabel::clone()
{
	return BaseObjectPtr(new AttachedLabel(*this));
}


void AttachedLabel::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fShowValue);
	SETBIT(flags, 1, fShowPercent);
	SETBIT(flags, 2, fShowLabelAndPerc);
	SETBIT(flags, 4, fShowLabel);
	SETBIT(flags, 5, fShowBubbleSizes);
	SETBIT(flags, 6, fShowSeriesName);
	record << flags;
}


void AttachedLabel::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fShowValue			= GETBIT(flags, 0);
	fShowPercent		= GETBIT(flags, 1);
	fShowLabelAndPerc	= GETBIT(flags, 2);
	fShowLabel			= GETBIT(flags, 4);
	fShowBubbleSizes	= GETBIT(flags, 5);
	fShowSeriesName		= GETBIT(flags, 6);
}

int AttachedLabel::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:showVal")
		{
			CP_XML_ATTR (L"val" , fShowValue); 
		}
		CP_XML_NODE(L"c:showPercent")
		{
			CP_XML_ATTR (L"val" , fShowPercent); 
		}
		CP_XML_NODE(L"c:showBubbleSize")
		{
			CP_XML_ATTR (L"val" , fShowBubbleSizes); 
		}

		if (is_area == false)
		{
			CP_XML_NODE(L"c:showCatName")	
			{
				CP_XML_ATTR (L"val" ,fShowLabel); 
			}
			CP_XML_NODE(L"c:showSerName") {	CP_XML_ATTR (L"val" , 0); }	
		}
		else
		{
			CP_XML_NODE(L"c:showCatName") {	CP_XML_ATTR (L"val" , 0); }	
			CP_XML_NODE(L"c:showSerName")
			{
				CP_XML_ATTR (L"val" , fShowSeriesName || fShowLabel); 
			}			
		}
	}
	return 0;
}

} // namespace XLS

