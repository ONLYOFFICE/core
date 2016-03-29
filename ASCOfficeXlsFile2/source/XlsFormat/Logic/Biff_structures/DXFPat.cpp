
#include "DXFN.h"
#include <Binary/CFRecord.h>


namespace XLS
{


BiffStructurePtr DXFPat::clone()
{
	return BiffStructurePtr(new DXFPat(*this));
}

void DXFPat::store(CFRecord& record)
{
	unsigned short flags = 0;
	SETBITS(flags, 10, 15, fls);
	record << flags;
	unsigned short flags2 = 0;
	SETBITS(flags2, 0, 6, icvForeground);
	SETBITS(flags2, 7, 13, icvBackground);
	record << flags2;
}


void DXFPat::load(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fls = GETBITS(flags, 10, 15);
	record >> flags;
	
	icvForeground = GETBITS(flags, 0, 6);
	icvBackground = GETBITS(flags, 7, 13);
}

int DXFPat::serialize(std::wostream & stream)
{
	if (parent->flsNinch && parent->icvBNinch && parent->icvFNinch) return 0;
	
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"fill")
		{	
			CP_XML_NODE(L"patternFill")
			{
				if (!parent->flsNinch)
				{
					switch(fls)
					{		
						case 2: CP_XML_ATTR(L"patternType", L"pct50");					break; //50% gray
						case 3: CP_XML_ATTR(L"patternType", L"pct75");					break; //75% gray
						case 4: CP_XML_ATTR(L"patternType", L"pct25");					break; //25% gray
						case 5: CP_XML_ATTR(L"patternType", L"horzStripe");				break; //Horizontal stripe
						case 6: CP_XML_ATTR(L"patternType", L"vertStripe");				break; //Vertical stripe
						case 7: CP_XML_ATTR(L"patternType", L"reverseDiagStripe");		break; //Reverse diagonal stripe
						case 8: CP_XML_ATTR(L"patternType", L"diagStripe");				break; //Diagonal stripe
						case 9: CP_XML_ATTR(L"patternType", L"diagCross");				break; //Diagonal crosshatch
						case 10: CP_XML_ATTR(L"patternType", L"trellis");				break; //Thick Diagonal crosshatch
						case 11: CP_XML_ATTR(L"patternType", L"thinHorzStripe");		break; //Thin horizontal stripe
						case 12: CP_XML_ATTR(L"patternType", L"thinVertStripe");		break; //Thin vertical stripe
						case 13: CP_XML_ATTR(L"patternType", L"thinReverseDiagStripe");	break; //Thin reverse diagonal stripe
						case 14: CP_XML_ATTR(L"patternType", L"thinDiagStripe");		break; //Thin diagonal stripe
						case 15: CP_XML_ATTR(L"patternType", L"thinHorzCross");			break; //Thin horizontal crosshatch
						case 16: CP_XML_ATTR(L"patternType", L"thinDiagCross");			break; //Thin diagonal crosshatch
						case 17: CP_XML_ATTR(L"patternType", L"gray125");				break; //12.5% gray
						case 18: CP_XML_ATTR(L"patternType", L"gray0625");				break; //6.25% gray
						default:
							CP_XML_ATTR(L"patternType", L"solid");
					}
				}
				if (!parent->icvFNinch)
				{

					CP_XML_NODE(L"fgColor")
					{				
						CP_XML_ATTR(L"indexed", icvForeground);
					}
				}	
				if (!parent->icvBNinch)
				{
					CP_XML_NODE(L"bgColor")
					{				
						CP_XML_ATTR(L"indexed", icvBackground);
					}
				}

			}
		}
	}
	return 0;
}



} // namespace XLS

