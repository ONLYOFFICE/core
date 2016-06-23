
#include "Scatter.h"

namespace XLS
{

Scatter::Scatter()
{
}


Scatter::~Scatter()
{
}


BaseObjectPtr Scatter::clone()
{
	return BaseObjectPtr(new Scatter(*this));
}


void Scatter::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fBubbles);
	SETBIT(flags, 1, fShowNegBubbles);
	SETBIT(flags, 2, fHasShadow);
	record >> pcBubbleSizeRatio >> wBubbleSize >> flags;
}


void Scatter::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> pcBubbleSizeRatio >> wBubbleSize >> flags;
	
	fBubbles		= GETBIT(flags, 0);
	fShowNegBubbles = GETBIT(flags, 1);
	fHasShadow		= GETBIT(flags, 2);
}

int	Scatter::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		if (fBubbles)
		{
			CP_XML_NODE(L"c:bubbleScale")
			{
				CP_XML_ATTR (L"val" , pcBubbleSizeRatio);
			}
			if (fShowNegBubbles)
			{
				CP_XML_NODE(L"c:showNegBubbles") { CP_XML_ATTR (L"val" , L"1"); }
			}
			if (wBubbleSize == (_UINT16)2)
			{
				CP_XML_NODE(L"c:sizeRepresents") { CP_XML_ATTR (L"val" , L"2"); }
			}
		}
		else
		{
			//CP_XML_NODE(L"c:scatterStyle") - уровнем выше
		}
	}
	
	return 0;
}

} // namespace XLS

