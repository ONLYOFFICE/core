
#include "MarkerFormat.h"

namespace XLS
{

MarkerFormat::MarkerFormat()
{
}


MarkerFormat::~MarkerFormat()
{
}


BaseObjectPtr MarkerFormat::clone()
{
	return BaseObjectPtr(new MarkerFormat(*this));
}


void MarkerFormat::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fAuto);
	SETBIT(flags, 4, fNotShowInt);
	SETBIT(flags, 5, fNotShowBrd);
	record << rgbFore << rgbBack << imk << flags << icvFore << icvBack << miSize;
}

void MarkerFormat::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> rgbFore >> rgbBack >> imk >> flags >> icvFore >> icvBack >> miSize;
	
	fAuto		= GETBIT(flags, 0);
	fNotShowInt = GETBIT(flags, 4);
	fNotShowBrd = GETBIT(flags, 5);
}

int MarkerFormat::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:marker")
		{
			CP_XML_NODE(L"c:symbol")
			{
				std::wstring strSymbol;
				switch (imk)
				{
				case 0x0000:	strSymbol = L"none";	break;//				No marker.
				case 0x0001:	strSymbol = L"square";	break;//				Square markers.
				case 0x0002:	strSymbol = L"diamond";	break;//				Diamond-shaped markers.
				case 0x0003:	strSymbol = L"triangle";break;//				Triangular markers.
				case 0x0004:	strSymbol = L"x";		break;//				Square markers with an X.
				case 0x0005:	strSymbol = L"star";	break;//				Square markers with an asterisk.
				case 0x0006:	strSymbol = L"dot";		break;//				Short bar markers.
				case 0x0007:	strSymbol = L"dash";	break;//				Long bar markers.
				case 0x0008:	strSymbol = L"circle";	break;//				Circular markers.
				case 0x0009:	strSymbol = L"plus";	break;//				Square markers with a plus sign.
				}
				CP_XML_ATTR(L"val",  strSymbol);	
			}
			if (imk > 0)
			{
				CP_XML_NODE(L"c:size") { CP_XML_ATTR(L"val",  miSize / 20);	}
				CP_XML_NODE(L"c:spPr")
				{
					CP_XML_NODE(L"a:solidFill")
					{
						CP_XML_NODE(L"a:srgbClr")
						{
							CP_XML_ATTR(L"val",  rgbBack.strRGB);		
						}
					}
					CP_XML_NODE(L"a:ln")
					{
						CP_XML_NODE(L"a:solidFill")
						{
							CP_XML_NODE(L"a:srgbClr")
							{
								CP_XML_ATTR(L"val",  rgbFore.strRGB);		
							}
						}
						CP_XML_NODE(L"a:prstDash") { CP_XML_ATTR(L"val",  L"solid"); }

					}
				}
			}
		}
	}
	
	return 0;
}

} // namespace XLS
