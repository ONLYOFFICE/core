#include "precompiled_cpodf.h"
#include "oox_drawing_fills.h"

#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore {
namespace oox {

	oox_bitmap_fill::oox_bitmap_fill() : name_space(L"a"), bStretch(false), bCrop(false),bTile(false),isInternal(true)
	{
		memset(cropRect,0,sizeof(double)*4);
	}
	
	oox_bitmap_fill_ptr oox_bitmap_fill::create(){return boost::make_shared<oox_bitmap_fill>();}

	oox_hatch_fill_ptr oox_hatch_fill::create(){return boost::make_shared<oox_hatch_fill>();}
	
	oox_solid_fill_ptr oox_solid_fill::create(){return boost::make_shared<oox_solid_fill>();}
	
	oox_gradient_fill_ptr oox_gradient_fill::create(){return boost::make_shared<oox_gradient_fill>();}


//////////////////////////////////////////////////////////////////////////////////////////////////////

void oox_serialize_none_fill(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"a:noFill");
	}
}
void oox_serialize_srgb(std::wostream & strm,std::wstring color,_CP_OPT(double)  opacity)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"a:srgbClr")
		{
			CP_XML_ATTR(L"val",color);
			if (opacity)
			{
				CP_XML_NODE(L"a:alpha")
				{
					CP_XML_ATTR(L"val",boost::lexical_cast<std::wstring>((int)(*opacity)*1000));// + L"%");
				}
			}
		}
	}
}
void oox_serialize_srgb(std::wostream & strm,std::wstring color,_CP_OPT(odf::percent)  opacity)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"a:srgbClr")
		{
			CP_XML_ATTR(L"val",color);
			if (opacity)
			{
				CP_XML_NODE(L"a:alpha")
				{
					CP_XML_ATTR(L"val",boost::lexical_cast<std::wstring>((int)opacity->get_value()*1000));// + L"%");
				}
			}
		}
	}
}
void oox_serialize_solid_fill(std::wostream & strm, const _oox_fill & val)
{
	if (!val.solid)return;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"a:solidFill")
		{
			oox_serialize_srgb(CP_XML_STREAM(),val.solid->color,val.opacity);
		}

	}
}
void oox_serialize_bitmap_fill(std::wostream & strm, const _oox_fill & val)
{
	if (!val.bitmap)return;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(std::wstring(val.bitmap->name_space + L":blipFill"))
		{
			if (val.bitmap->rotate)	CP_XML_ATTR(L"a:rotWithShape",*(val.bitmap->rotate));
			else CP_XML_ATTR(L"a:rotWithShape",1);
			
			if (val.bitmap->dpi)	CP_XML_ATTR(L"a:dpi",*val.bitmap->dpi);
			
			CP_XML_NODE(L"a:blip")
			{
				if (val.bitmap->isInternal)
				{
					CP_XML_ATTR(L"r:embed",val.bitmap->rId );
					CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
				}
				else
					CP_XML_ATTR(L"r:link",val.bitmap->rId );

				if (val.opacity)
				{
					CP_XML_NODE(L"a:alphaModFix")
					{
						CP_XML_ATTR(L"amt",(int)(*val.opacity * 1000));
					}
				}		
			}
			if (val.bitmap->bStretch)
			{			
				CP_XML_NODE(L"a:stretch")
				{
					if (!val.bitmap->bCrop)
					{
						CP_XML_NODE(L"a:fillRect");
					}

				}
			}
			if (val.bitmap->bCrop)
			{
				CP_XML_NODE(L"a:srcRect")
				{
					CP_XML_ATTR(L"l", static_cast<size_t>(val.bitmap->cropRect[0]*1000));
					CP_XML_ATTR(L"t", static_cast<size_t>(val.bitmap->cropRect[1]*1000));
					CP_XML_ATTR(L"r", static_cast<size_t>(val.bitmap->cropRect[2]*1000));
					CP_XML_ATTR(L"b", static_cast<size_t>(val.bitmap->cropRect[3]*1000));
				}
			}
			if (val.bitmap->bTile)
			{
				CP_XML_NODE(L"a:tile")
				{
					//tx="0" ty="0" sx="100000" sy="100000"
					CP_XML_ATTR(L"flip","none");
					CP_XML_ATTR(L"algn",L"ctr");
				}
			}		

		}

	}
}
void oox_serialize_gradient_fill(std::wostream & strm, const _oox_fill & val)
{
	if (!val.gradient)return;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"a:gradFill")
		{
			if (val.gradient->colors.size()>0)
			{
				CP_XML_NODE(L"a:gsLst")
				{
					BOOST_FOREACH(oox_gradient_fill::_color_position & col, val.gradient->colors)
					{
						CP_XML_NODE(L"a:gs")
						{
							CP_XML_ATTR(L"pos",(int)(col.pos *1000));//%
							oox_serialize_srgb(CP_XML_STREAM(),col.color_ref,col.opacity);
						}
					}
				}
			}
			double angle =/*360 - */val.gradient->angle/* * 180./3.14159265358979323846*/;
			angle *= 60000; //60 000 per 1 gr - 19.5.5 oox 
			
			switch(val.gradient->style)
			{
			case 0:
				CP_XML_NODE(L"a:lin")
				{		
					CP_XML_ATTR(L"ang",(int)angle);//gr in rad
				}break;
			case 1:
			case 2:
			case 3:
				CP_XML_NODE(L"a:path")
				{
					if (val.gradient->style == 1) CP_XML_ATTR(L"path", L"rect");
					if (val.gradient->style == 2) CP_XML_ATTR(L"path", L"circle");
					if (val.gradient->style == 3) CP_XML_ATTR(L"path", L"shape");
					
					CP_XML_NODE(L"a:fillToRect")
					{
						CP_XML_ATTR(L"l", (int)(val.gradient->rect[0] * 1000));
						CP_XML_ATTR(L"t", (int)(val.gradient->rect[1] * 1000));
						CP_XML_ATTR(L"r", (int)(val.gradient->rect[2] * 1000));
						CP_XML_ATTR(L"b", (int)(val.gradient->rect[3] * 1000));
					}
				}break;
			}
		}
//tileRect (Tile Rectangle) §20.1.8.59
	}
}
void oox_serialize_hatch_fill(std::wostream & strm, const _oox_fill & val)
{
	if (!val.hatch)return;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"a:pattFill")
		{
			CP_XML_ATTR(L"prst",val.hatch->preset);
			CP_XML_NODE(L"a:fgClr")//опять для ms важно что этот цвет перед back
			{
				oox_serialize_srgb(CP_XML_STREAM(),val.hatch->color_ref,val.opacity);
			}	
			if (val.hatch->color_back_ref)
			{
				CP_XML_NODE(L"a:bgClr")
				{
					oox_serialize_srgb(CP_XML_STREAM(),*val.hatch->color_back_ref,val.opacity);
				}
			}

		}
	}
}
void oox_serialize_fill(std::wostream & strm, const _oox_fill & val)
{
	switch (val.type)
	{
		case 0:	
			oox_serialize_none_fill(strm);
			break;
		case 1:	
			oox_serialize_solid_fill(strm, val);
			break;
		case 2:	
			oox_serialize_bitmap_fill(strm, val);
			break;
		case 3:	
			oox_serialize_gradient_fill(strm, val);
			break;
		case 4:	oox_serialize_hatch_fill(strm, val);
			break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
}
}