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
#pragma once

#include <xml/simple_xml_writer.h>

namespace cpdoccore { 

namespace oox {
	static void pptx_serialize_size(std::wostream & strm, int cx, int cy,std::wstring node)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(node)
			{
				CP_XML_ATTR(L"cx", cx); 
				CP_XML_ATTR(L"cy", cy); 
			}
		}
	}
	static void pptx_serialize_clrScheme(std::wstringstream & strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"a:dk1")
			{
				CP_XML_NODE(L"a:sysClr")
				{
					CP_XML_ATTR(L"val",L"windowText");
					CP_XML_ATTR(L"lastClr",L"000000");
				}
			}
			CP_XML_NODE(L"a:lt1")
			{
				CP_XML_NODE(L"a:sysClr")
				{
					CP_XML_ATTR(L"val",L"window");
					CP_XML_ATTR(L"lastClr",L"FFFFFF");
				}
			}			
			CP_XML_NODE(L"a:dk2")
			{
				CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val",L"1F497D");}
			}		
			CP_XML_NODE(L"a:lt2")
			{
				CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val",L"EEECE1");}
			}			
			CP_XML_NODE(L"a:accent1")
			{
				CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val",L"4F81BD");}
			}			
			CP_XML_NODE(L"a:accent2")
			{
				CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val",L"C0504D");}
			}			
			CP_XML_NODE(L"a:accent3")
			{
				CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val",L"9BBB59");}
			}			
			CP_XML_NODE(L"a:accent4")
			{
				CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val",L"8064A2");}
			}			
			CP_XML_NODE(L"a:accent5")
			{
				CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val",L"4BACC6");}
			}			
			CP_XML_NODE(L"a:accent6")
			{
				CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val",L"F79646");}
			}			
			CP_XML_NODE(L"a:hlink")
			{
				CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val",L"0000FF");}
			}			
			CP_XML_NODE(L"a:folHlink")
			{
				CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val",L"800080");}
			}
		}
	}

	static void pptx_serialize_fontScheme(std::wstringstream & strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"a:majorFont")
			{
				CP_XML_NODE(L"a:latin"){CP_XML_ATTR(L"typeface",L"Arial");}
				CP_XML_NODE(L"a:ea"){CP_XML_ATTR(L"typeface",L"DejaVu Sans");}
				CP_XML_NODE(L"a:cs"){CP_XML_ATTR(L"typeface",L"DejaVu Sans");}
			}
 			CP_XML_NODE(L"a:minorFont")
			{
				CP_XML_NODE(L"a:latin"){CP_XML_ATTR(L"typeface",L"Arial");}
				CP_XML_NODE(L"a:ea"){CP_XML_ATTR(L"typeface",L"DejaVu Sans");}
				CP_XML_NODE(L"a:cs"){CP_XML_ATTR(L"typeface",L"DejaVu Sans");}
			}
		}
	}
	static void pptx_serialize_fmtScheme(std::wstringstream & strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"a:fillStyleLst")
			{
				CP_XML_NODE(L"a:solidFill"){CP_XML_NODE(L"a:schemeClr"){CP_XML_ATTR(L"val",L"phClr");}}
				CP_XML_NODE(L"a:gradFill")
				{
					CP_XML_NODE(L"a:gsLst")
					{
						CP_XML_NODE(L"a:gs")
						{
							CP_XML_ATTR(L"pos",0);
							CP_XML_NODE(L"a:schemeClr")
							{
								CP_XML_ATTR(L"val","phClr");
								CP_XML_NODE(L"a:lumMod"){CP_XML_ATTR(L"val",110000);}
								CP_XML_NODE(L"a:satMod"){CP_XML_ATTR(L"val",105000);}
								CP_XML_NODE(L"a:tint")	{CP_XML_ATTR(L"val",67000);}
							}
						}
						CP_XML_NODE(L"a:gs")
						{
							CP_XML_ATTR(L"pos",50000);
							CP_XML_NODE(L"a:schemeClr")
							{
								CP_XML_ATTR(L"val","phClr");
								CP_XML_NODE(L"a:lumMod"){CP_XML_ATTR(L"val",105000);}
								CP_XML_NODE(L"a:satMod"){CP_XML_ATTR(L"val",103000);}
								CP_XML_NODE(L"a:tint")	{CP_XML_ATTR(L"val",73000);}
							}
						}						
						CP_XML_NODE(L"a:gs")
						{
							CP_XML_ATTR(L"pos",100000);
							CP_XML_NODE(L"a:schemeClr")
							{
								CP_XML_ATTR(L"val","phClr");
								CP_XML_NODE(L"a:lumMod"){CP_XML_ATTR(L"val",105000);}
								CP_XML_NODE(L"a:satMod"){CP_XML_ATTR(L"val",109000);}
								CP_XML_NODE(L"a:tint")	{CP_XML_ATTR(L"val",81000);}
							}
						}					
					}
					CP_XML_NODE(L"a:lin"){CP_XML_ATTR(L"ang",5400000);CP_XML_ATTR(L"scaled",0);}
				}
				CP_XML_NODE(L"a:gradFill")
				{
					CP_XML_NODE(L"a:gsLst")
					{
						CP_XML_NODE(L"a:gs")
						{
							CP_XML_ATTR(L"pos",0);
							CP_XML_NODE(L"a:schemeClr")
							{
								CP_XML_ATTR(L"val","phClr");
								CP_XML_NODE(L"a:lumMod"){CP_XML_ATTR(L"val",102000);}
								CP_XML_NODE(L"a:satMod"){CP_XML_ATTR(L"val",103000);}
								CP_XML_NODE(L"a:tint")	{CP_XML_ATTR(L"val",94000);}
							}
						}
						CP_XML_NODE(L"a:gs")
						{
							CP_XML_ATTR(L"pos",50000);
							CP_XML_NODE(L"a:schemeClr")
							{
								CP_XML_ATTR(L"val","phClr");
								CP_XML_NODE(L"a:lumMod"){CP_XML_ATTR(L"val",100000);}
								CP_XML_NODE(L"a:satMod"){CP_XML_ATTR(L"val",110000);}
								CP_XML_NODE(L"a:shade")	{CP_XML_ATTR(L"val",100000);}
							}
						}						
						CP_XML_NODE(L"a:gs")
						{
							CP_XML_ATTR(L"pos",100000);
							CP_XML_NODE(L"a:schemeClr")
							{
								CP_XML_ATTR(L"val","phClr");
								CP_XML_NODE(L"a:lumMod"){CP_XML_ATTR(L"val",120000);}
								CP_XML_NODE(L"a:satMod"){CP_XML_ATTR(L"val",99000);}
								CP_XML_NODE(L"a:shade")	{CP_XML_ATTR(L"val",78000);}
							}
						}					
					}
					CP_XML_NODE(L"a:lin"){CP_XML_ATTR(L"ang",5400000);CP_XML_ATTR(L"scaled",0);}
				}
			}
			CP_XML_NODE(L"a:lnStyleLst")
			{
				CP_XML_NODE(L"a:ln")
				{
					CP_XML_ATTR(L"w",6350);
					CP_XML_ATTR(L"cap",L"flat");CP_XML_ATTR(L"cmpd",L"sng"); CP_XML_ATTR(L"algn",L"ctr");
					CP_XML_NODE(L"a:solidFill"){CP_XML_NODE(L"a:schemeClr"){CP_XML_ATTR(L"val",L"phClr");}}
					CP_XML_NODE(L"a:miter"){CP_XML_ATTR(L"lim",800000);}
				}
				CP_XML_NODE(L"a:ln")
				{
					CP_XML_ATTR(L"w",12700);
					CP_XML_ATTR(L"cap",L"flat");CP_XML_ATTR(L"cmpd",L"sng"); CP_XML_ATTR(L"algn",L"ctr");
					CP_XML_NODE(L"a:solidFill"){CP_XML_NODE(L"a:schemeClr"){CP_XML_ATTR(L"val",L"phClr");}}
					CP_XML_NODE(L"a:miter"){CP_XML_ATTR(L"lim",800000);}
				}
				CP_XML_NODE(L"a:ln")
				{
					CP_XML_ATTR(L"w",19050);
					CP_XML_ATTR(L"cap",L"flat");CP_XML_ATTR(L"cmpd",L"sng"); CP_XML_ATTR(L"algn",L"ctr");
				  	CP_XML_NODE(L"a:solidFill"){CP_XML_NODE(L"a:schemeClr"){CP_XML_ATTR(L"val",L"phClr");}}
					CP_XML_NODE(L"a:miter"){CP_XML_ATTR(L"lim",800000);}
				}
			}
			CP_XML_NODE(L"a:effectStyleLst")
			{
				CP_XML_NODE(L"a:effectStyle")
				{
					CP_XML_NODE(L"a:effectLst")
					{
						CP_XML_NODE(L"a:outerShdw")
						{
							CP_XML_ATTR(L"dist",20000);
							CP_XML_ATTR(L"blurRad",40000);
							CP_XML_ATTR(L"dir",5400000);
							CP_XML_ATTR(L"rotWithShape",0);
							CP_XML_NODE(L"a:srgbClr")
							{
								CP_XML_ATTR(L"val",L"000000");
								CP_XML_NODE(L"a:alpha"){CP_XML_ATTR(L"val", 38000);}
							}
						}
					}
				}
				CP_XML_NODE(L"a:effectStyle")
				{
					CP_XML_NODE(L"a:effectLst")
					{
						CP_XML_NODE(L"a:outerShdw")
						{
							CP_XML_ATTR(L"dist",23000);
							CP_XML_ATTR(L"blurRad",40000);
							CP_XML_ATTR(L"dir",5400000);
							CP_XML_ATTR(L"rotWithShape",0);
							CP_XML_NODE(L"a:srgbClr")
							{
								CP_XML_ATTR(L"val",L"000000");
								CP_XML_NODE(L"a:alpha"){CP_XML_ATTR(L"val", 35000);}
							}
						}
					}
				}
				CP_XML_NODE(L"a:effectStyle")
				{
					CP_XML_NODE(L"a:effectLst")
					{
						CP_XML_NODE(L"a:outerShdw")
						{
							CP_XML_ATTR(L"dist",23000);
							CP_XML_ATTR(L"blurRad",40000);
							CP_XML_ATTR(L"dir",5400000);
							CP_XML_ATTR(L"rotWithShape",0);
							CP_XML_NODE(L"a:srgbClr")
							{
								CP_XML_ATTR(L"val",L"000000");
								CP_XML_NODE(L"a:alpha"){CP_XML_ATTR(L"val", 35000);}
							}
						}
					}
				}
			}
			CP_XML_NODE(L"a:bgFillStyleLst")
			{
				CP_XML_NODE(L"a:solidFill"){CP_XML_NODE(L"a:schemeClr"){CP_XML_ATTR(L"val",L"phClr");}}
				CP_XML_NODE(L"a:solidFill")
				{
					CP_XML_NODE(L"a:schemeClr")
					{
						CP_XML_ATTR(L"val",L"phClr");
						CP_XML_NODE(L"a:tint")	{CP_XML_ATTR(L"val",95000);}
						CP_XML_NODE(L"a:satMod"){CP_XML_ATTR(L"val",170000);}
					}
				}
				CP_XML_NODE(L"a:gradFill")
				{
					CP_XML_NODE(L"a:gsLst")
					{
						CP_XML_NODE(L"a:gs")
						{
							CP_XML_ATTR(L"pos",0);
							CP_XML_NODE(L"a:schemeClr")
							{
								CP_XML_ATTR(L"val","phClr");
								CP_XML_NODE(L"a:lumMod"){CP_XML_ATTR(L"val",102000);}
								CP_XML_NODE(L"a:satMod"){CP_XML_ATTR(L"val",150000);}
								CP_XML_NODE(L"a:tint")	{CP_XML_ATTR(L"val",93000);}
								CP_XML_NODE(L"a:shade") {CP_XML_ATTR(L"val",98000);}
							}
						}
						
						CP_XML_NODE(L"a:gs")
						{
							CP_XML_ATTR(L"pos",50000);
							CP_XML_NODE(L"a:schemeClr")
							{
								CP_XML_ATTR(L"val","phClr");
								CP_XML_NODE(L"a:lumMod"){CP_XML_ATTR(L"val",103000);}
								CP_XML_NODE(L"a:satMod"){CP_XML_ATTR(L"val",130000);}
								CP_XML_NODE(L"a:tint")	{CP_XML_ATTR(L"val",98000);}
								CP_XML_NODE(L"a:shade") {CP_XML_ATTR(L"val",90000);}
							}
						}
						CP_XML_NODE(L"a:gs")
						{
							CP_XML_ATTR(L"pos",100000);
							CP_XML_NODE(L"a:schemeClr")
							{
								CP_XML_ATTR(L"val","phClr");
								CP_XML_NODE(L"a:satMod"){CP_XML_ATTR(L"val",120000);}
								CP_XML_NODE(L"a:shade") {CP_XML_ATTR(L"val",63000);}
							}		
						}					
					}
					CP_XML_NODE(L"a:lin"){CP_XML_ATTR(L"ang",5400000);CP_XML_ATTR(L"scaled",0);}
				}
			}
		}
	}

}
}