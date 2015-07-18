#include "BorderFillInfo.h"
#include <boost/functional/hash/hash.hpp>

#include <simple_xml_writer.h>

namespace XLS
{

std::size_t hash_value(FillInfo const & val)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, val.icvFore);
    boost::hash_combine(seed, val.icvBack);
	boost::hash_combine(seed, val.fls);

	return seed;
}
std::size_t hash_value(BorderInfo const & val)
{
    std::size_t seed = 0;
	boost::hash_combine(seed, val.dgBottom);
    boost::hash_combine(seed, val.dgDiag);
    boost::hash_combine(seed, val.dgLeft);
    boost::hash_combine(seed, val.dgRight);
    boost::hash_combine(seed, val.dgTop);
    boost::hash_combine(seed, val.grbitDiag);
    boost::hash_combine(seed, val.icvBottom);
    boost::hash_combine(seed, val.icvDiag);
    boost::hash_combine(seed, val.icvLeft);
    boost::hash_combine(seed, val.icvRight);
    boost::hash_combine(seed, val.icvTop);

	return seed;
}

bool FillInfo::operator == (const FillInfo & rVal) const
{
    const bool res =     
		fls		== rVal.fls &&
		icvBack == rVal.icvBack &&
		icvFore	== rVal.icvFore;

    return res;
}

bool FillInfo::operator != (const FillInfo & rVal) const
{
	return !(this->operator ==(rVal));
}


static std::wstring PatternType [] = 
{
	L"none",
	L"solid",
	L"mediumGray",
	L"darkGray",
	L"lightGray",
	L"darkHorizontal",
	L"darkVertical",
	L"darkDown",
	L"darkUp",
	L"darkGrid",
	L"darkTrellis",
	L"lightHorizontal",
	L"lightVertical",
	L"lightDown",
	L"lightUp",
	L"lightGrid",
	L"lightTrellis",
	L"gray125",
	L"gray0625"
};

int FillInfo::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"fill")
		{
			CP_XML_NODE(L"patternFill")
			{
				CP_XML_ATTR(L"patternType", PatternType[fls]);

				if (fls > 0)
				{
					CP_XML_NODE(L"fgColor")
					{
						//if (icvFore < 65 ) 
							CP_XML_ATTR(L"indexed", icvFore);
					}
					CP_XML_NODE(L"bgColor")
					{
						//if (icvBack < 65 )	
							CP_XML_ATTR(L"indexed", icvBack);
					}
				}
			}
		}
	}
	return 0;
}

bool BorderInfo::operator == (const BorderInfo & rVal) const
{
    const bool res =     
		dgBottom	== rVal.dgBottom	&&
		dgDiag		== rVal.dgDiag		&&
		dgLeft		== rVal.dgLeft		&&
		dgRight		== rVal.dgRight		&&
		dgTop		== rVal.dgTop		&&
		grbitDiag	== rVal.grbitDiag	&&
		icvBottom	== rVal.icvBottom	&&
		icvDiag		== rVal.icvDiag		&&
		icvLeft		== rVal.icvLeft		&&
		icvRight	== rVal.icvRight	&&
		icvTop		== rVal.icvTop;

    return res;
}

bool BorderInfo::operator != (const BorderInfo & rVal) const
{
	return !(this->operator ==(rVal));
}
static std::wstring border_type[]=
{
	L"none",
	L"thin",
	L"medium",
	L"dashed",
	L"dotted",
	L"thick",
	L"double",
	L"hair",
	L"mediumDashed",
	L"dashDot",
	L"mediumDashDot",
	L"dashDotDot",
	L"mediumDashDotDot",
	L"slantDashDot"
};

void serialize1(std::wostream & _stream, unsigned char type, unsigned color, const std::wstring & name)
{
    if (type < 1 || type > 13) return;
    
	CP_XML_WRITER(_stream)
    {
        CP_XML_NODE(name)
        {
			CP_XML_ATTR(L"style", border_type[type]);

            if (color > 0)
			{
				CP_XML_NODE(L"color")
				{              
					CP_XML_ATTR(L"indexed", color);
				}
			}
        }
    }
}

int BorderInfo::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"border")
		{
            if (grbitDiag == 2 || grbitDiag == 3)
                CP_XML_ATTR(L"diagonalUp", true);
            if (grbitDiag == 1 || grbitDiag == 3) 
                CP_XML_ATTR(L"diagonalDown", true);
            
            serialize1(CP_XML_STREAM(), dgLeft	, icvLeft	, L"left");
            serialize1(CP_XML_STREAM(), dgRight	, icvRight	, L"right");
            serialize1(CP_XML_STREAM(), dgTop	, icvTop	, L"top");
            serialize1(CP_XML_STREAM(), dgBottom, icvBottom	, L"bottom");
            serialize1(CP_XML_STREAM(), dgDiag	, icvDiag	, L"diagonal");
 		}
	}
	return 0;
}

}