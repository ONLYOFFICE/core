/*
 * (c) Copyright Ascensio System SIA 2010-2017
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "BorderFillInfo.h"
#include <Auxiliary/HelpFunc.h>
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
				if (fls < 0 || fls > 18) 
					fls = 1;

				CP_XML_ATTR(L"patternType", PatternType[fls]);

				if (fls > 0)
				{
					CP_XML_NODE(L"fgColor")
					{
						if (foreFillInfo_.enabled)
						{
							switch(foreFillInfo_.xclrType)
							{
							case 0://auto
								/*CP_XML_ATTR(L"auto");*/ break;
							case 1://indexed
								if (foreFillInfo_.icv < 64) 
									CP_XML_ATTR(L"indexed",  foreFillInfo_.icv); break;
							case 2://rgb
								CP_XML_ATTR(L"rgb", STR::toARGB(foreFillInfo_.xclrValue)); break;
							case 3://theme color
								CP_XML_ATTR(L"theme", foreFillInfo_.xclrValue/* + 1*/); 
								CP_XML_ATTR(L"tint", foreFillInfo_.nTintShade / 32767.0); break;
							case 4://not set
								break;
							}
						}else
							CP_XML_ATTR(L"indexed", icvFore);

					}
					CP_XML_NODE(L"bgColor")
					{
						if (backFillInfo_.enabled)
						{
							switch(backFillInfo_.xclrType)
							{
							case 0://auto
								/*CP_XML_ATTR(L"auto");*/ break;
							case 1://indexed
								if (backFillInfo_.icv < 64)
									CP_XML_ATTR(L"indexed",  backFillInfo_.icv); break;
							case 2://rgb
								CP_XML_ATTR(L"rgb", STR::toARGB(backFillInfo_.xclrValue)); break;
							case 3://theme color
								CP_XML_ATTR(L"theme", backFillInfo_.xclrValue /*+ 1*/); 
								CP_XML_ATTR(L"tint", backFillInfo_.nTintShade / 32767.0); break;
							case 4://not set
								break;
							}
						}
						else
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
		((dgBottom	== rVal.dgBottom	&& icvBottom	== rVal.icvBottom)	|| (dgBottom	== 0 && rVal.dgBottom == 0))&&
		((dgLeft	== rVal.dgLeft		&& icvLeft		== rVal.icvLeft)	|| (dgLeft		== 0 && rVal.dgLeft == 0))	&&
		((dgRight	== rVal.dgRight		&& icvRight		== rVal.icvRight)	|| (dgRight		== 0 && rVal.dgRight == 0))	&&
		((dgTop		== rVal.dgTop		&& icvTop		== rVal.icvTop)		|| (dgTop		== 0 && rVal.dgTop == 0))	&&
		
		((	dgDiag		== rVal.dgDiag		&&
			grbitDiag	== rVal.grbitDiag	&& icvDiag	== rVal.icvDiag) || 
												(dgDiag	== 0 && rVal.dgDiag == 0 && grbitDiag == 0 && rVal.grbitDiag== 0))
	
		;

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
    if (type < 0 || type > 13) return;
    
	CP_XML_WRITER(_stream)
    {
        CP_XML_NODE(name)
        {
			if (type > 0) CP_XML_ATTR(L"style", border_type[type]);

            if (color > 0 && type > 0)
			{
				CP_XML_NODE(L"color")
				{              
					CP_XML_ATTR(L"indexed", color);
				}
			}
        }
    }
}
void serialize1(std::wostream & _stream, unsigned char type, FillInfoExt & color, const std::wstring & name)
{
    if (type < 0 || type > 13) return;
    
	CP_XML_WRITER(_stream)
    {
        CP_XML_NODE(name)
        {
			if (type > 0) CP_XML_ATTR(L"style", border_type[type]);

            if (color.enabled && type > 0)
			{
				CP_XML_NODE(L"color")
				{              
					switch(color.xclrType)
					{
					case 0://auto
						/*CP_XML_ATTR(L"auto");*/ break;
					case 1://indexed
						CP_XML_ATTR(L"indexed",  color.icv); break;
					case 2://rgb
						CP_XML_ATTR(L"rgb", STR::toARGB(color.xclrValue)); break;
					case 3://theme color
						CP_XML_ATTR(L"theme", color.xclrValue + 1); 
						CP_XML_ATTR(L"tint", color.nTintShade / 32767.0); break;
					case 4://not set
						break;
					}
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
            
			if (leftFillInfo_.enabled)	serialize1(CP_XML_STREAM(), dgLeft	, leftFillInfo_	, L"left");
			else						serialize1(CP_XML_STREAM(), dgLeft	, icvLeft		, L"left");
          
			if (rightFillInfo_.enabled)	serialize1(CP_XML_STREAM(), dgRight	, rightFillInfo_, L"right");
			else						serialize1(CP_XML_STREAM(), dgRight	, icvRight		, L"right");
            
			if (topFillInfo_.enabled)	serialize1(CP_XML_STREAM(), dgTop	, topFillInfo_	, L"top");
			else						serialize1(CP_XML_STREAM(), dgTop	, icvTop		, L"top");

            if (bottomFillInfo_.enabled)serialize1(CP_XML_STREAM(), dgBottom, bottomFillInfo_, L"bottom");
			else						serialize1(CP_XML_STREAM(), dgBottom, icvBottom		, L"bottom");
            
			serialize1(CP_XML_STREAM(), dgDiag	, icvDiag	, L"diagonal");
 		}
	}
	return 0;
}

}