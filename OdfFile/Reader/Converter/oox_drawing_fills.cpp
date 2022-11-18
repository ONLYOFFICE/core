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

#include "oox_drawing_fills.h"

#include <xml/simple_xml_writer.h>

namespace cpdoccore {
namespace oox {

	oox_bitmap_fill::oox_bitmap_fill() : name_space(L"a"), bStretch(false), bCrop(false), bTile(false), isInternal(true), bGrayscale(false)
	{
		memset(cropRect, 0, sizeof(double)*4);
	}
	
	oox_bitmap_fill_ptr oox_bitmap_fill::create(){return boost::make_shared<oox_bitmap_fill>();}

	oox_hatch_fill_ptr oox_hatch_fill::create(){return boost::make_shared<oox_hatch_fill>();}
	
	oox_solid_fill_ptr oox_solid_fill::create(){return boost::make_shared<oox_solid_fill>();}
	
	oox_gradient_fill_ptr oox_gradient_fill::create(){return boost::make_shared<oox_gradient_fill>();}


//////////////////////////////////////////////////////////////////////////////////////////////////////

void oox_serialize_none_fill(std::wostream & strm, const std::wstring &ns)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(ns + L":noFill");
	}
}
void oox_serialize_srgb(std::wostream & strm, const std::wstring &color,_CP_OPT(double) opacity, const std::wstring &ns)
{
	std::wstring ns_att = (ns == L"a" ? L"" : ns + L":");

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(ns + L":srgbClr")
		{
			CP_XML_ATTR2(ns_att + L"val", color);
			if (opacity)
			{
				CP_XML_NODE(ns + L":alpha")
				{
					CP_XML_ATTR2(ns_att + L"val", std::to_wstring((int)(*opacity)*1000));// + L"%");
				}
			}
		}
	}
}
void oox_serialize_srgb(std::wostream & strm, const std::wstring &color, _CP_OPT(odf_types::percent) opacity, const std::wstring &ns)
{
	std::wstring ns_att = (ns == L"a" ? L"" : ns + L":");

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(ns + L":srgbClr")
		{
			CP_XML_ATTR2(ns_att + L"val",color);
			if (opacity)
			{
				CP_XML_NODE(ns + L":alpha")
				{
					CP_XML_ATTR2(ns_att + L"val", std::to_wstring((int)opacity->get_value() * 1000));// + L"%");
				}
			}
		}
	}
}
void oox_serialize_solid_fill(std::wostream & strm, const _oox_fill & val, const std::wstring &ns)
{
	if (!val.solid)return;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(ns + L":solidFill")
		{
			oox_serialize_srgb(CP_XML_STREAM(), val.solid->color, val.opacity, ns);
		}

	}
}
void vml_serialize_solid_fill(std::wostream & strm, const _oox_fill & val)
{
	if (!val.solid)	return;
	if (!val.opacity) return;
	
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"v:fill")
		{
			if (val.opacity)
			{
				CP_XML_ATTR(L"opacity",	(int)(*val.opacity));
			}
		}

	}
}
void vml_serialize_bitmap_fill(std::wostream & strm, const _oox_fill & val)
{
	if (!val.bitmap) return;
	if (!val.bitmap->isInternal) return;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"v:fill")
		{	
			CP_XML_ATTR(L"r:id", val.bitmap->rId );

			if (val.opacity)
			{
				CP_XML_ATTR(L"opacity",(int)(*val.opacity));
			}		
			
			if (val.bitmap->bTile)
			{
				CP_XML_ATTR(L"type", L"pattern");
			}
			else
			{
				CP_XML_ATTR(L"type", L"frame");
			}
			CP_XML_ATTR(L"recolor", L"t");
		}
	}
}
void oox_serialize_bitmap_fill(std::wostream & strm, const _oox_fill & val, const std::wstring &ns)
{
	if (!val.bitmap) return;

	std::wstring ns_att = (ns == L"a" ? L"" : ns + L":");

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(std::wstring(val.bitmap->name_space + L":blipFill"))
		{
			//if (val.bitmap->rotate)	CP_XML_ATTR(ns + L":rotWithShape",*(val.bitmap->rotate));
			//else CP_XML_ATTR(ns + L":rotWithShape",1);

			if (val.bitmap->dpi)	
			{
				CP_XML_ATTR2(ns + L":dpi", *val.bitmap->dpi);
			}
			
			CP_XML_NODE(ns + L":blip")
			{
				if (val.bitmap->isInternal) 
				{
					CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
					CP_XML_ATTR(L"r:embed", val.bitmap->rId );
				}
				else
					CP_XML_ATTR(L"r:link", val.bitmap->rId );

				if (val.opacity)
				{
					CP_XML_NODE(ns + L":alphaModFix")
					{
						CP_XML_ATTR2(ns_att + L"amt", (int)(*val.opacity * 1000));
					}
				}
				if (val.bitmap->bGrayscale)
				{
					CP_XML_NODE(ns + L":grayscl");
				}
				if (val.bitmap->luminance || val.bitmap->contrast)
				{
					CP_XML_NODE(ns + L":lum")
					{
						if (val.bitmap->luminance)	CP_XML_ATTR2(L"bright", (int)(*val.bitmap->luminance * 1000));
						if (val.bitmap->contrast)	CP_XML_ATTR2(L"contrast", (int)(*val.bitmap->contrast * 1000));
					}
				}
			}
			if (val.bitmap->bCrop)
			{
				CP_XML_NODE(ns + L":srcRect")
				{
					CP_XML_ATTR2(ns_att + L"l", static_cast<int>(val.bitmap->cropRect[0]*1000));
					CP_XML_ATTR2(ns_att + L"t", static_cast<int>(val.bitmap->cropRect[1]*1000));
					CP_XML_ATTR2(ns_att + L"r", static_cast<int>(val.bitmap->cropRect[2]*1000));
					CP_XML_ATTR2(ns_att + L"b", static_cast<int>(val.bitmap->cropRect[3]*1000));
				}
			}
			if (val.bitmap->bTile)
			{
				CP_XML_NODE(ns + L":tile")
				{
					//tx="0" ty="0" sx="100000" sy="100000"
					CP_XML_ATTR2(ns_att + L"flip", "none");
					CP_XML_ATTR2(ns_att + L"algn", L"ctr");
				}
			}
			else if (val.bitmap->bStretch)
			{			
				CP_XML_NODE(ns + L":stretch")
				{
					if (!val.bitmap->bCrop)
					{
						CP_XML_NODE(ns + L":fillRect");
					}
				}
			}	
		}
	}
}
void vml_serialize_gradient_fill(std::wostream & strm, const _oox_fill & val)
{
	if (!val.gradient) return;
	
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"v:fill")
		{
			if (!val.gradient->colors.empty())
			{
				CP_XML_ATTR(L"color2", L"#" + val.gradient->colors[val.gradient->colors.size() - 1].color_ref);

				std::wstring colors_value;
				for (size_t i = 0; i < val.gradient->colors.size(); i++)
				{
					colors_value += std::to_wstring(val.gradient->colors[i].pos) + L"% #" + 
													val.gradient->colors[i].color_ref + L",";
				}

				if (!colors_value.empty())
				{
					CP_XML_ATTR(L"colors", colors_value.substr(0, colors_value.length() - 1));
				}
			}
			double angle =/*360 - */val.gradient->angle/* * 180./3.14159265358979323846*/;
			
			CP_XML_ATTR(L"focus", L"100%");
			switch(val.gradient->style)
			{
			case 0:	
				CP_XML_ATTR(L"type", L"gradient");	
				CP_XML_ATTR(L"method", L"linear");
				CP_XML_ATTR(L"angle", angle);
				break;
			case 1:	
				//CP_XML_ATTR(L"type", L"gradientRadial");
				//break;
			case 2:	
				//CP_XML_ATTR(L"type", L"gradientCenter");	
				//break;
			case 3:	
				//CP_XML_ATTR(L"type", L"gradientUnscaled");
				//break;
				CP_XML_ATTR(L"type", L"gradientRadial");
			}
			if (val.gradient->style > 0)
			{
				double focus_x = ((val.gradient->rect[2] - val.gradient->rect[0]) /2. + val.gradient->rect[0]) / 100.;
				double focus_y = ((val.gradient->rect[3] - val.gradient->rect[1]) /2. + val.gradient->rect[1]) / 100.;
				
				CP_XML_ATTR(L"focusposition", XmlUtils::ToString(focus_x, L"%.2f") + L"," + XmlUtils::ToString(focus_y, L"%.2f"));
			}
		}
	}
}
void oox_serialize_gradient_fill(std::wostream & strm, const _oox_fill & val, const std::wstring &ns)
{
	if (!val.gradient) return;
	
	std::wstring ns_att = (ns == L"a" ? L"" : ns + L":");

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(ns + L":gradFill")
		{
			if (val.gradient->colors.size()>0)
			{
				CP_XML_NODE(ns + L":gsLst")
				{
					for (size_t i = 0; i < val.gradient->colors.size(); i++)
					{
						oox_gradient_fill::_color_position & col = val.gradient->colors[i];
						CP_XML_NODE(ns + L":gs")
						{
							CP_XML_ATTR2(ns_att + L"pos", (int)(col.pos * 1000));//%
							oox_serialize_srgb(CP_XML_STREAM(), col.color_ref, col.opacity, ns);
						}
					}
				}
			}
			double angle =/*360 - */val.gradient->angle/* * 180./3.14159265358979323846*/;
			angle *= 60000; //60 000 per 1 gr - 19.5.5 oox 
			
			switch(val.gradient->style)
			{
			case 0:
				CP_XML_NODE(ns + L":lin")
				{		
					CP_XML_ATTR2(ns_att + L"ang",(int)angle);//gr in rad
				}break;
			case 1:
			case 2:
			case 3:
				CP_XML_NODE(ns + L":path")
				{
					if (val.gradient->style == 1) CP_XML_ATTR2(ns_att + L"path", L"rect");
					if (val.gradient->style == 2) CP_XML_ATTR2(ns_att + L"path", L"circle");
					if (val.gradient->style == 3) CP_XML_ATTR2(ns_att + L"path", L"shape");
					
					CP_XML_NODE(ns + L":fillToRect")
					{
						CP_XML_ATTR2(ns_att + L"l", (int)(val.gradient->rect[0] * 1000));
						CP_XML_ATTR2(ns_att + L"t", (int)(val.gradient->rect[1] * 1000));
						CP_XML_ATTR2(ns_att + L"r", (int)(val.gradient->rect[2] * 1000));
						CP_XML_ATTR2(ns_att + L"b", (int)(val.gradient->rect[3] * 1000));
					}
				}break;
			}
		}
//tileRect (Tile Rectangle) §20.1.8.59
	}
}
void oox_serialize_hatch_fill(std::wostream & strm, const _oox_fill & val, const std::wstring &ns)
{
	if (!val.hatch)return;

	std::wstring ns_att = (ns == L"a" ? L"" : ns + L":");

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(ns + L":pattFill")
		{
			CP_XML_ATTR2(ns_att + L"prst", val.hatch->preset);
			CP_XML_NODE(ns + L":fgClr")//опять для ms важно что этот цвет перед back
			{
				oox_serialize_srgb(CP_XML_STREAM(), val.hatch->color_ref, val.opacity);
			}	
			if (val.hatch->color_back_ref)
			{
				CP_XML_NODE(ns + L":bgClr")
				{
					oox_serialize_srgb(CP_XML_STREAM(), *val.hatch->color_back_ref ,val.opacity);
				}
			}

		}
	}
}

void vml_serialize_background (std::wostream & strm, const _oox_fill & val, const std::wstring & color, int id)
{
	if (val.type == 0) return;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"v:background")
		{
			CP_XML_ATTR(L"id",			L"_x0000_s" + std::to_wstring(1024 + id));
			CP_XML_ATTR(L"o:bwmode",	L"white");
			CP_XML_ATTR(L"fillcolor",	L"#" + color);
			CP_XML_ATTR(L"o:targetscreensize", L"1024,768");
			switch (val.type)
			{
				case 1:	
					vml_serialize_solid_fill(CP_XML_STREAM(), val);
					break;
				case 2:	
					vml_serialize_bitmap_fill(CP_XML_STREAM(), val);
					break;
				case 3:	
					vml_serialize_gradient_fill(CP_XML_STREAM(), val);
					break;
				case 4:	
					//vml_serialize_hatch_fill(CP_XML_STREAM(), val);
					break;
			}
		}
	}
}

void vml_serialize_fill (std::wostream & strm, const _oox_fill & val)
{
	switch (val.type)
	{
		case 0:	
			break;
		case 1:	
			vml_serialize_solid_fill(strm, val);
			break;
		case 2:	
			vml_serialize_bitmap_fill(strm, val);
			break;
		case 3:	
			vml_serialize_gradient_fill(strm, val);
			break;
		case 4:	
			//vml_serialize_hatch_fill(strm, val);
			break;			
	}
}

void oox_serialize_fill (std::wostream & strm, const _oox_fill & val, const std::wstring &ns)
{
	switch (val.type)
	{
		case 0:	
			oox_serialize_none_fill(strm, ns);
			break;
		case 1:	
			oox_serialize_solid_fill(strm, val, ns);
			break;
		case 2:	
			oox_serialize_bitmap_fill(strm, val, ns);
			break;
		case 3:	
			oox_serialize_gradient_fill(strm, val, ns);
			break;
		case 4:	oox_serialize_hatch_fill(strm, val, ns);
			break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
}
}