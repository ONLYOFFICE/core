
#include "LineFormat.h"

namespace XLS
{

LineFormat::LineFormat()
{
}


LineFormat::~LineFormat()
{
}


BaseObjectPtr LineFormat::clone()
{
	return BaseObjectPtr(new LineFormat(*this));
}


void LineFormat::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	
	SETBIT(flags, 0, fAuto);
	SETBIT(flags, 2, fAxisOn);
	SETBIT(flags, 3, fAutoCo);
	
	record << rgb << lns << we << flags << icv;
}


void LineFormat::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> rgb >> lns >> we >> flags >> icv;
	
	fAuto = GETBIT(flags, 0);
	fAxisOn = GETBIT(flags, 2);
	fAutoCo = GETBIT(flags, 3);
}

int LineFormat::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"a:ln")
		{
			switch(we)
			{
				case 0xFFFF:	CP_XML_ATTR(L"w", 3175);	break; //Hairline
				case 0:			CP_XML_ATTR(L"w", 12700);	break; //single
				case 1:			CP_XML_ATTR(L"w", 25400);	break; //double
				case 2:			CP_XML_ATTR(L"w", 38100);	break; //triple
			}
			if (lns == (_UINT16) 5)
			{
				CP_XML_NODE(L"a:noFill");
			}
			else
			{
				CP_XML_NODE(L"a:solidFill")
				{
					CP_XML_NODE(L"a:srgbClr")
					{
						CP_XML_ATTR(L"val",  rgb.strRGB);		
					}
				}

				CP_XML_NODE(L"a:prstDash")
				{			
					switch(lns)
					{
						case 1: CP_XML_ATTR(L"val", L"dash");			break;  //Dash
						case 2: CP_XML_ATTR(L"val", L"dot");			break;  //Dot
						case 3: CP_XML_ATTR(L"val", L"dashDot");		break;  //Dash-dot
						case 4: CP_XML_ATTR(L"val", L"sysDashDotDot");	break;  //Dash dot-dot
						case 0: 
						default:CP_XML_ATTR(L"val", L"solid");			break;  //Solid
					}
				}			
				switch(lns)
				{
					case 6: break; //Dark gray pattern
					case 7: break; //Medium gray pattern
					case 8: break; //Light gray patternn
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

