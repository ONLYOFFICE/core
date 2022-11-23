/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#include <boost/functional/hash/hash.hpp>

#include "BorderFillInfo.h"
#include "../../Auxiliary/HelpFunc.h"

#include "../../../../Common/Utils/simple_xml_writer.h"

namespace XLS
{
	std::size_t hash_value(FillInfoExt const & val)
	{
		std::size_t seed = 0;

		boost::hash_combine(seed, val.enabled);
		if (val.enabled)
		{
			boost::hash_combine(seed, val.icv);
			boost::hash_combine(seed, val.xclrType);
			boost::hash_combine(seed, val.xclrValue);
		}
		return seed;
	}
	std::size_t hash_value(FillInfo const & val)
	{
		std::size_t seed = 0;

		boost::hash_combine(seed, val.backFillInfo_);
		boost::hash_combine(seed, val.foreFillInfo_);

		if (!val.foreFillInfo_.enabled)
			boost::hash_combine(seed, val.icvFore);
		if (!val.backFillInfo_.enabled)
			boost::hash_combine(seed, val.icvBack);
		
		boost::hash_combine(seed, val.fls);
		
		return seed;
	}
	std::size_t hash_value(BorderInfo const & val)
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, val.bottomFillInfo_);
		boost::hash_combine(seed, val.leftFillInfo_);
		boost::hash_combine(seed, val.rightFillInfo_);
		boost::hash_combine(seed, val.topFillInfo_);

		boost::hash_combine(seed, val.dgBottom);
		boost::hash_combine(seed, val.dgLeft);
		boost::hash_combine(seed, val.dgRight);
		boost::hash_combine(seed, val.dgTop);
		boost::hash_combine(seed, val.dgDiag);
		boost::hash_combine(seed, val.grbitDiag);

		boost::hash_combine(seed, val.icvBottom);
		boost::hash_combine(seed, val.icvLeft);
		boost::hash_combine(seed, val.icvRight);
		boost::hash_combine(seed, val.icvTop);
		boost::hash_combine(seed, val.icvDiag);

		return seed;
	}
	std::size_t hash_value(FontInfo const & val)
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, val.color);
		boost::hash_combine(seed, val.dyHeight);
		boost::hash_combine(seed, val.fItalic);
		boost::hash_combine(seed, val.fStrikeOut);
		boost::hash_combine(seed, val.fOutline);
		boost::hash_combine(seed, val.fShadow);
		boost::hash_combine(seed, val.fCondense);
		boost::hash_combine(seed, val.fExtend);
		boost::hash_combine(seed, val.icv);
		boost::hash_combine(seed, val.bls);
		boost::hash_combine(seed, val.sss);
		boost::hash_combine(seed, val.uls);
		boost::hash_combine(seed, val.bFamily);
		boost::hash_combine(seed, val.bCharSet);
		boost::hash_combine(seed, val.scheme);
		boost::hash_combine(seed, val.name);

		return seed;
	}
	bool FillInfoExt::operator == (const FillInfoExt & rVal) const
	{
		return  (enabled == false && rVal.enabled == false)
	||
		(	enabled == rVal.enabled &&
			icv == rVal.icv &&
			nTintShade == rVal.nTintShade &&
			xclrType == rVal.xclrType &&
			xclrValue == rVal.xclrValue);
}
bool BorderInfo::operator < (const BorderInfo& right) const
{
	return  (dgLeft < right.dgLeft || dgLeft == right.dgLeft && 
			(dgRight < right.dgRight || dgRight == right.dgRight &&
			(dgTop < right.dgTop || dgTop == right.dgTop &&
			(dgBottom < right.dgBottom || dgBottom == right.dgBottom &&
			(dgDiag < right.dgDiag || dgDiag == right.dgDiag &&
			(grbitDiag < right.grbitDiag || grbitDiag == right.grbitDiag &&
			(icvLeft < right.icvLeft || icvLeft == right.icvLeft &&
			(icvRight < right.icvRight || icvRight == right.icvRight &&
			(icvTop < right.icvTop || icvTop == right.icvTop &&
			(icvBottom < right.icvBottom || icvBottom == right.icvBottom &&
			(icvDiag < right.icvDiag)))))))))));
}
bool FontInfo::operator == (const FontInfo & rVal) const
{
	const bool res =
		dyHeight== rVal.dyHeight &&
		name	== rVal.name &&
		scheme	== rVal.scheme &&
		fItalic == rVal.fItalic &&
		fStrikeOut == rVal.fStrikeOut &&
		fOutline == rVal.fOutline &&
		fShadow == rVal.fShadow &&
		fCondense == rVal.fCondense &&
		fExtend == rVal.fExtend &&
		bls	== rVal.bls &&
		sss	== rVal.sss &&
		uls	== rVal.uls &&
		bFamily	== rVal.bFamily &&
		bCharSet	== rVal.bCharSet &&
		icv == rVal.icv &&
		color == rVal.color
		;

    return res;
}

bool FontInfo::operator != (const FontInfo & rVal) const
{
	return !(this->operator ==(rVal));
}

int FontInfo::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
        CP_XML_NODE(L"font")
        {
		   if ( bls== 700)
			{
				CP_XML_NODE(L"b")
				{
					CP_XML_ATTR(L"val", true);
				}            
			}

			if (bCharSet)
			{
				CP_XML_NODE(L"charset")
				{
					CP_XML_ATTR(L"val", bCharSet);
				}
			}

			if (icv < 0x7fff || color.enabled )
			{
				CP_XML_NODE(L"color")
				{
					if (color.enabled)
					{
						switch(color.xclrType)
						{
						case 0://auto
							/*CP_XML_ATTR(L"auto");*/ break;
						case 1://indexed
							if (color.icv < 64) 
								CP_XML_ATTR(L"indexed",  color.icv); break;
						case 2://rgb
							CP_XML_ATTR(L"rgb", STR::toARGB(color.xclrValue)); break;
						case 3://theme color
							CP_XML_ATTR(L"theme", color.xclrValue/* + 1*/); 
							CP_XML_ATTR(L"tint", color.nTintShade / 32767.0); break;
						case 4://not set
							break;
						}
					}else
						CP_XML_ATTR(L"indexed", icv);
				}
			}

			if (fCondense)
			{
				CP_XML_NODE(L"condense")
				{
					CP_XML_ATTR(L"val", 1);
				}
			}

			if (fExtend)
			{
				CP_XML_NODE(L"extend")
				{
					CP_XML_ATTR(L"val", fExtend);
				}
			}

			if (bFamily)
			{
				CP_XML_NODE(L"family")
				{
					CP_XML_ATTR(L"val", bFamily);
				}
			}

			if (fItalic)
			{
				CP_XML_NODE(L"i")
				{
					CP_XML_ATTR(L"val", fItalic);
				}
			}

			if (!name.empty())
			{
				CP_XML_NODE(L"name")
				{
					CP_XML_ATTR(L"val", name);
				}
			}

			if (fOutline)
			{
				CP_XML_NODE(L"outline")
				{
					CP_XML_ATTR(L"val", fOutline);
				}
			}

			if (scheme == 1 || scheme == 2)
			{
			    CP_XML_NODE(L"scheme")
			    {
					CP_XML_ATTR(L"val", (scheme == 1 ? L"major" : L"minor"));
			    }
			}

			if (fShadow)
			{
				CP_XML_NODE(L"shadow")
				{
					CP_XML_ATTR(L"val", fShadow);
				}
			}

			if (fStrikeOut)
			{
				CP_XML_NODE(L"strike")
				{
					CP_XML_ATTR(L"val", fStrikeOut);
				}
			}

			if (dyHeight > 0)
			{
				CP_XML_NODE(L"sz")
				{
					CP_XML_ATTR(L"val", dyHeight/20.f);
				}
			}

			if (uls > 0)
			{
				CP_XML_NODE(L"u")
				{
					switch(uls)
					{
						case 0:		CP_XML_ATTR(L"val", L"none");break;
						case 1:		CP_XML_ATTR(L"val", L"single");break;
						case 2:		CP_XML_ATTR(L"val", L"double");break;
						case 33:	CP_XML_ATTR(L"val", L"singleAccounting");break;
						case 34:	CP_XML_ATTR(L"val", L"doubleAccounting");break;
					}
				}
			}

			if (sss > 0)
			{
				CP_XML_NODE(L"vertAlign")
				{
					switch(sss)
					{
						case 1:	CP_XML_ATTR(L"val", L"superscript");break;
						case 2:	CP_XML_ATTR(L"val", L"subscript");break;
					}
	               
				}
			}
		}
	}

	return 0;
}


//------------------------------------------------------------------------------------------
bool FillInfo::operator == (const FillInfo & rVal) const
{
	const bool res =
		fls == rVal.fls &&
		icvBack == rVal.icvBack &&
		icvFore == rVal.icvFore;

	const bool res_back_ex = (backFillInfo_ == rVal.backFillInfo_);

	const bool res_fore_ex = (foreFillInfo_ == rVal.foreFillInfo_);

    return res && res_back_ex && res_fore_ex;
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
		if (!ext.empty())
		{
			stream << ext;
		}
		else
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
												(dgDiag	== 0 && rVal.dgDiag == 0 && grbitDiag == 0 && rVal.grbitDiag== 0)) &&
	

		(bottomFillInfo_ == rVal.bottomFillInfo_ && leftFillInfo_ == rVal.leftFillInfo_ && topFillInfo_ == rVal.topFillInfo_
			&& rightFillInfo_ == rVal.rightFillInfo_)
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
						CP_XML_ATTR(L"theme", color.xclrValue); 
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
