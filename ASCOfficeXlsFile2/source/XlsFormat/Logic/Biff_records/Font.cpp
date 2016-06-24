/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

#include "Font.h"

namespace XLS
{
	const static std::wstring shemeColor[17] = 
	{L"accent1",L"accent2",L"accent3",L"accent4",L"accent5",L"accent6",L"bg1",L"bg2",L"dk1",L"dk2",L"folHlink",L"hlink",L"lt1",L"lt2",L"phClr",L"tx1",L"tx2"};

Font::Font()
{
}


Font::~Font()
{
}


BaseObjectPtr Font::clone()
{
	return BaseObjectPtr(new Font(*this));
}


void Font::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 1, fItalic);
	SETBIT(flags, 3, fStrikeOut);
	SETBIT(flags, 4, fOutline);
	SETBIT(flags, 5, fShadow);
	SETBIT(flags, 6, fCondense);
	SETBIT(flags, 7, fExtend);

	record << dyHeight << flags;
	record << icv << bls << sss << uls << bFamily << bCharSet;
	record.reserveNunBytes(1, static_cast<unsigned char>(0x5E));
	
	record << fontName;
}


void Font::readFields(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();

	unsigned short flags;
	record >> dyHeight >> flags;
	
	fItalic		= GETBIT(flags, 1);
	fStrikeOut	= GETBIT(flags, 3);
	fOutline	= GETBIT(flags, 4);
	fShadow		= GETBIT(flags, 5);
	fCondense	= GETBIT(flags, 6);
	fExtend		= GETBIT(flags, 7);

	record >> icv >> bls >> sss >> uls >> bFamily >> bCharSet;
	record.skipNunBytes(1);
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		ShortXLAnsiString name;
		record >> name;
		
		fontName = name;
	}
	else
	{
		record >> fontName;
	}
}

int Font::serialize(std::wostream & stream)
{
    CP_XML_WRITER(stream)    
    {
        CP_XML_NODE(L"font")
        {
			serialize_properties(CP_XML_STREAM());
		}
	}
	return 0;
}

void Font::set_color_ext(FillInfoExt & color_ext_)
{
	color_ext = color_ext_;
}
int Font::serialize_rPr(std::wostream & stream, bool rtl, bool defRPr, std::wstring namespace_)
{
    CP_XML_WRITER(stream)    
    {
		std::wstring	strRpr = namespace_ + L"rPr";
		if (defRPr)		strRpr = namespace_ + L"defRPr";
		
		CP_XML_NODE(strRpr)
		{
			if (dyHeight > 0)
			{
				CP_XML_ATTR(L"sz", (int)(dyHeight/20. * 100));

			}
			if (bls == 700)
			{
				CP_XML_ATTR(L"b", true);
			}
			else
				CP_XML_ATTR(L"b", false);

			if (fItalic)
			{
				CP_XML_ATTR(L"i", fItalic);
			}
			else 
				CP_XML_ATTR(L"i", false);
			//if (bCharSet.value())
			//{
			//    CP_XML_NODE(L"charset")
			//    {
			//        CP_XML_ATTR(L"val", bCharSet);
			//    }
			//}


			//if ((fCondense.value()) && (*fCondense.value()))
			//{
			//    CP_XML_NODE(L"condense")
			//    {
			//        CP_XML_ATTR(L"val", (int)(*fCondense.value()));
			//    }
			//}

			//if ((fExtend.value()) && (fExtend))
	  //      {
	  //          CP_XML_NODE(L"extend")
	  //          {
	  //              CP_XML_ATTR(L"val", fExtend);
	  //          }
	  //      }
			if ( (icv < 0x7fff) || color_ext.enabled )
			{
				if (color_ext.enabled )
				{
					CP_XML_NODE(namespace_ + L"solidFill")
					{
						switch(color_ext.xclrType)
							{
							case 0://auto
								/*CP_XML_ATTR(L"auto");*/ break;
							case 1://indexed
								CP_XML_NODE(namespace_ + L"schemeClr")
								{
									CP_XML_ATTR(L"val",  color_ext.icv); break;
								}
							case 2://rgb
								CP_XML_NODE(namespace_ + L"srgbClr")
								{
									CP_XML_ATTR(L"val", STR::toRGB(color_ext.xclrValue)); 
								}break;
							case 3://theme color
								CP_XML_NODE(namespace_ + L"schemeClr")
								{
									CP_XML_ATTR(L"val", color_ext.xclrValue + 1); 
									CP_XML_NODE(L"tint")
									{
										CP_XML_ATTR(L"val", color_ext.nTintShade / 32767.0);
									}							
								}break;
							case 4://not set
								break;
							}
						}
				}
				else
				{
					std::wstring strColor;

					std::map<int,  std::wstring>::iterator it = global_info->colors_palette.find(icv);
				
					if (it != global_info->colors_palette.end())	strColor = it->second;
					else if ( icv < 64)								strColor = DefaultPalette[icv].substr(2);

					if (!strColor.empty())
					{
						CP_XML_NODE(namespace_ + L"solidFill")
						{
							CP_XML_NODE(namespace_ + L"srgbClr")
							{
								CP_XML_ATTR(L"val", strColor); 
							}							
						}
					}
				}
			}
			if (!fontName.value().empty())
			{
				CP_XML_NODE(namespace_ + L"latin")
				{
					CP_XML_ATTR(L"typeface", fontName.value());
				}
				CP_XML_NODE(namespace_ + L"ea")
				{
					CP_XML_ATTR(L"typeface", fontName.value());
				}
				CP_XML_NODE(namespace_ + L"cs")
				{
					CP_XML_ATTR(L"typeface", fontName.value());
				}
			}
			if (rtl)
			{
				CP_XML_NODE(namespace_ + L"rtl");
			}
			//if ((fOutline.value()) && (fOutline))
	  //      {
	  //          CP_XML_NODE(L"outline")
	  //          {
	  //              CP_XML_ATTR(L"val", fOutline);
	  //          }
	  //      }

			//if (font.scheme)
			//{
			//    CP_XML_NODE(L"scheme")
			//    {
			//        CP_XML_ATTR(L"val", *font.scheme);
			//    }
			//}

			//if ((fShadow.value()) && (fShadow))
	  //      {
	  //          CP_XML_NODE(L"shadow")
	  //          {
			//		CP_XML_ATTR(L"val", fShadow);
	  //          }
	  //      }

			//if ((fStrikeOut.value()) && (fStrikeOut))
			//{
			//    CP_XML_NODE(L"strike")
			//    {
			//        CP_XML_ATTR(L"val", fStrikeOut);
			//    }
			//}



		//    if ((uls.value()) && (*uls.value() > 0))
		//    {
		//        CP_XML_NODE(L"u")
		//        {
					//switch(uls)
					//{
					//	case 1:		CP_XML_ATTR(L"val", "single");break;
					//	case 2:		CP_XML_ATTR(L"val", "double");break;
					//	case 33:	CP_XML_ATTR(L"val", "singleAccounting");break;
					//	case 34:	CP_XML_ATTR(L"val", "doubleAccounting");break;
					//}
		//        }
		//    }

		//    if ((sss.value()) && (*sss.value() > 0))
		//    {
		//        CP_XML_NODE(L"vertAlign")
		//        {
					//switch(*sss.value())
					//{
					//	case 1:	CP_XML_ATTR(L"val", L"superscript");break;
					//	case 2:	CP_XML_ATTR(L"val", L"subscript");break;
					//}
		//           
		//        }
		//    }
			}
	}
	return 0;
}


int Font::serialize_properties(std::wostream & stream, bool isRPr)
{
    CP_XML_WRITER(stream)    
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

        if (icv < 0x7fff || color_ext.enabled )
		{
            CP_XML_NODE(L"color")
            {
				if (color_ext.enabled )
				{
					switch(color_ext.xclrType)
					{
					case 0://auto
						/*CP_XML_ATTR(L"auto");*/ break;
					case 1://indexed
						CP_XML_ATTR(L"indexed",  color_ext.icv); break;
					case 2://rgb
						CP_XML_ATTR(L"rgb", STR::toARGB(color_ext.xclrValue)); break;
					case 3://theme color
						CP_XML_ATTR(L"theme", color_ext.xclrValue + 1); 
						CP_XML_ATTR(L"tint", color_ext.nTintShade / 32767.0); break;
					case 4://not set
						break;
					}
				}
				else
				{
					CP_XML_ATTR(L"indexed", icv);
				}
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

		if (!fontName.value().empty())
		{
			if (isRPr)
			{
				CP_XML_NODE(L"rFont")
				{
					CP_XML_ATTR(L"val", fontName.value());
				}
			}
			else
			{
				CP_XML_NODE(L"name")
				{
					CP_XML_ATTR(L"val", fontName.value());
				}
			}
		}

		if (fOutline)
        {
            CP_XML_NODE(L"outline")
            {
                CP_XML_ATTR(L"val", fOutline);
            }
        }

        //if (font.scheme)
        //{
        //    CP_XML_NODE(L"scheme")
        //    {
        //        CP_XML_ATTR(L"val", *font.scheme);
        //    }
        //}

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
                CP_XML_ATTR(L"val", dyHeight/20);
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
	return 0;
}

} // namespace XLS

