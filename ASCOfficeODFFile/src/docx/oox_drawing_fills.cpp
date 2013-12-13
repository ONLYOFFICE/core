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
					CP_XML_ATTR(L"val",boost::lexical_cast<std::wstring>(*opacity) + L"%");
				}
			}
		}
	}
}
void oox_serialize_srgb(std::wostream & strm,std::wstring color,_CP_OPT(odf::length_or_percent)  opacity)
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
					CP_XML_ATTR(L"val",boost::lexical_cast<std::wstring>(opacity->get_percent().get_value()) + L"%");
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
					CP_XML_NODE(L"a:alpha")
					{
						CP_XML_ATTR(L"val",boost::lexical_cast<std::wstring>(*val.opacity) + L"%");
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
		//case 3:	oox_serialize_gradient_fill(strm, val);break;
		//case 4:	oox_serialize_hatch_fill(strm, val);break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
}
}