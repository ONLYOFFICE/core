
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "xlsx_drawing_context.h"

#include <simple_xml_writer.h>
#include <utils.h>
#include <Auxiliary/HelpFunc.h>

namespace oox {

	const static std::wstring shemeColor[17] = 
	{L"accent1",L"accent2",L"accent3",L"accent4",L"accent5",L"accent6",L"bg1",L"bg2",L"dk1",L"dk2",L"folHlink",L"hlink",L"lt1",L"lt2",L"phClr",L"tx1",L"tx2"};

class xlsx_drawing_context_handle::Impl
{
public:
    Impl(external_items & items)
        : items_(items), next_rId_(1), next_drawing_id_(1) 
    {
    }  

    external_items & get_mediaitems() { return items_; }

    size_t next_rId()
    {
        return next_rId_++;
    }

    std::pair<std::wstring, std::wstring> add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings)
    {
        const std::wstring id = boost::lexical_cast<std::wstring>(next_drawing_id_++);
        const std::wstring fileName = std::wstring(L"drawing") + id + L".xml";
       
		drawings_.push_back(drawing_elm(fileName, content, drawings));
       
		const std::wstring rId = std::wstring(L"rId") + id;//rDrId
        return std::pair<std::wstring, std::wstring>(fileName, rId);
    }
    const std::vector<drawing_elm> & content() const
    {
        return drawings_;
    }

	xlsx_drawings_ptr get_drawings();
private:
    external_items & items_;
    size_t next_rId_;
    
    std::vector<drawing_elm> drawings_;

    size_t next_drawing_id_;
};

xlsx_drawing_context_handle::xlsx_drawing_context_handle(external_items & items)
: impl_(new xlsx_drawing_context_handle::Impl(items))
{
}

xlsx_drawing_context_handle::~xlsx_drawing_context_handle()
{
}
std::pair<std::wstring, std::wstring> xlsx_drawing_context_handle::add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings )
{
    return impl_->add_drawing_xml(content, drawings);
}

const std::vector<drawing_elm> & xlsx_drawing_context_handle::content() const
{
    return impl_->content();
}

//---------------------------------------------------------------------------------------

xlsx_drawing_context::xlsx_drawing_context(xlsx_drawing_context_handle & h)
 : handle_(h), xlsx_drawings_(xlsx_drawings::create()), count_object(0)
{    
}

bool xlsx_drawing_context::start_drawing(int type)
{
	switch(type)
	{
	case 0x0000: // Group
	case 0x0001: // Line
	case 0x0002: // Rectangle
	case 0x0003: // Oval
	case 0x0004: // Arc
	case 0x0006: // Text
	case 0x001E: // OfficeArt object
	case 0x0009: // Polygon:
		start_shape(type); return true;
	case 0x0008: // Picture
		start_image(); return true;
	case 0x0005: // Chart  
	case 0x0007: // Button
	case 0x000B: // Checkbox
	case 0x000C: // Radio button
	case 0x000D: // Edit box
	case 0x000E: // Label
	case 0x000F: // Dialog box
	case 0x0010: // Spin control
	case 0x0011: // Scrollbar
	case 0x0012: // List
	case 0x0013: // Group box
	case 0x0014: // Dropdown list
	case 0x0019: // Note
		break;
	}
	return false;

}

void xlsx_drawing_context::start_image()
{
	_drawing_state st;
	drawing_state.push_back(st);

	drawing_state.back().type = external_items::typeImage;
	
	count_object++;
}

void xlsx_drawing_context::start_shape(int type)
{
	_drawing_state st;
	drawing_state.push_back(st);

	drawing_state.back().type = external_items::typeShape;
	drawing_state.back().shape_type = type;

	if (0x0006 == type)
	{
		drawing_state.back().shape_type = msosptTextBox;
		drawing_state.back().bTextBox = true;
	}
	if (0x001E == type)
	{
		drawing_state.back().shape_type = msosptTextBox;
		//drawing_state.back().bWordArt = true;
	}
	count_object++;
}

void xlsx_drawing_context::set_id(long id)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().id = id;
}
void xlsx_drawing_context::set_FlipH()
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().flipH = true;
}
void xlsx_drawing_context::set_FlipV()
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().flipV = true;
}
void xlsx_drawing_context::set_shape_id(int id)
{
	if (drawing_state.size() < 1 )		return;
	
	if (drawing_state.back().bTextBox)	return;

	drawing_state.back().shape_id = (MSOSPT)id;
}

void xlsx_drawing_context::end_drawing()
{
	if (drawing_state.size() < 1 )return;

	if (drawing_state.back().anchor.empty())
	{
		drawing_state.pop_back();
		return;
	}

	std::wstringstream strm;

	if ( drawing_state.back().type == external_items::typeImage ||
		drawing_state.back().shape_id == msosptPictureFrame )
	{
		drawing_state.back().type = external_items::typeImage;

		if (!drawing_state.back().image_target.empty())
		{
			bool isIternal = false;
			std::wstring rId = handle_.impl_->get_mediaitems().find_image( drawing_state.back().image_target, isIternal);
			
			serialize_pic(rId);
			serialize(strm);
			xlsx_drawings_->add(strm.str(), isIternal, 	rId , drawing_state.back().image_target, drawing_state.back().type);
		}
		else 
			drawing_state.back().type = external_items::typeShape;
	}
	if ( drawing_state.back().type == external_items::typeShape)
	{
		serialize_shape();
		serialize(strm);
		xlsx_drawings_->add(strm.str(), true, L"" , L"", drawing_state.back().type);
	}

	drawing_state.pop_back();

}
void xlsx_drawing_context::serialize_shape()
{
	std::wstringstream strm;

	std::wstring prstTxWarp;
	std::wstring prstGeom	= Spt2ShapeType(drawing_state.back().shape_id);

	if (prstGeom.empty())
	{
		prstTxWarp = Spt2WordArtShapeType(drawing_state.back().shape_id);
		if (prstTxWarp.empty() == false)
		{
			drawing_state.back().bWordArt = true;
			drawing_state.back().bTextBox = true;
			prstGeom = L"rect";
		}
		else if (drawing_state.back().path.empty())
		{
			//error !!!
			prstGeom = L"rect";
		}
	}
	else
	{
		if (drawing_state.back().shape_id == msosptTextBox)	drawing_state.back().bTextBox = true;
	}
	
	CP_XML_WRITER(strm)    
	{
		CP_XML_NODE(L"xdr:sp")
		{ 
			CP_XML_NODE(L"xdr:nvSpPr")
			{
				CP_XML_NODE(L"xdr:cNvPr")
				{
					if (drawing_state.back().id >= 0)	CP_XML_ATTR(L"id", drawing_state.back().id);
					if (drawing_state.back().name.empty())	drawing_state.back().name = L"Shape_" + count_object;

					CP_XML_ATTR(L"name", drawing_state.back().name);
					if (!drawing_state.back().description.empty())
					{
						CP_XML_ATTR(L"descr", drawing_state.back().description);
					}

					if (!drawing_state.back().hyperlink.empty())
					{
						CP_XML_NODE(L"a:hlinkClick")
						{
							CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
							
							CP_XML_ATTR(L"r:id", drawing_state.back().hyperlink);
						}
					}
				}
				CP_XML_NODE(L"xdr:cNvSpPr")
				{
					if (drawing_state.back().bTextBox)CP_XML_ATTR(L"txBox", 1);
				}
			}
			
			CP_XML_NODE(L"xdr:spPr")
			{
				serialize_xfrm(CP_XML_STREAM());

				if (prstGeom.empty() == false)
				{
					CP_XML_NODE(L"a:prstGeom")
					{
						CP_XML_ATTR(L"prst", prstGeom);
						if (!drawing_state.back().bWordArt)	CP_XML_NODE(L"a:avLst");
					}
				}
				else
				{
					CP_XML_NODE(L"a:custGeom")
					{        
						CP_XML_NODE(L"a:avLst");
						//oox_serialize_aLst(CP_XML_STREAM(),val.additional);
						CP_XML_NODE(L"a:ahLst");
						//CP_XML_NODE(L"a:gdLst");
						CP_XML_NODE(L"a:rect")
						{
							CP_XML_ATTR(L"b",L"b");
							CP_XML_ATTR(L"l",0);
							CP_XML_ATTR(L"r",L"r");
							CP_XML_ATTR(L"t",0);
						}
						CP_XML_NODE(L"a:pathLst")
						{ 	
							CP_XML_NODE(L"a:path")
							{
								CP_XML_ATTR(L"w", drawing_state.back().path_rect.right - drawing_state.back().path_rect.left);
								CP_XML_ATTR(L"h", drawing_state.back().path_rect.bottom- drawing_state.back().path_rect.top);
								
								CP_XML_STREAM() << drawing_state.back().path;
							}
						}
					}
				}
				//serialize_fill(CP_XML_STREAM());
				serialize_line(CP_XML_STREAM());		
			}
		}
	}

	drawing_state.back().shape = strm.str();
}

void xlsx_drawing_context::serialize_fill(std::wostream & stream)
{
	if (!drawing_state.back().image_target.empty())
	{
		bool  isIternal = false;
		std::wstring rId = handle_.impl_->get_mediaitems().find_image( drawing_state.back().image_target, isIternal);
		if (!rId.empty())
		{
			xlsx_drawings_->add(L"", isIternal, 	rId , drawing_state.back().image_target, drawing_state.back().type);
			serialize_bitmap_fill(stream, rId);
		}else 
			serialize_none_fill(stream);
	}
	else 
		serialize_none_fill(stream);
}

void xlsx_drawing_context::serialize_none_fill(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"a:noFill");
	}
}

void xlsx_drawing_context::serialize_xfrm(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"a:xfrm")
		{
			if (drawing_state.back().flipV)			CP_XML_ATTR(L"flipV", true);
			if (drawing_state.back().flipH)			CP_XML_ATTR(L"flipH", true);
			
			if (drawing_state.back().rotation != 0)	CP_XML_ATTR(L"rot", drawing_state.back().rotation);
			
			CP_XML_NODE(L"a:off")
			{
				CP_XML_ATTR(L"x", 0);
				CP_XML_ATTR(L"y", 0);
			}
			CP_XML_NODE(L"a:ext")
			{
				CP_XML_ATTR(L"cx", 0);
				CP_XML_ATTR(L"cy", 0);
			}
		}
	}
}

void xlsx_drawing_context::serialize_color	(std::wostream & stream, const _color &color)
{
	CP_XML_WRITER(stream)    
	{
		if (!color.rgb.empty())
		{
			CP_XML_NODE(L"a:srgbClr")
			{
				CP_XML_ATTR(L"val", color.rgb);		
			}
		}
		else if (color.bPalette){CP_XML_NODE(L"a:prstClr")	{	CP_XML_ATTR(L"val",L"black");}}
		else if (color.bScheme)
		//else
		{
			CP_XML_NODE(L"a:schemeClr")
			{
				CP_XML_ATTR(L"val", (color.index >=0 && color.index < 17) ? shemeColor[color.index] : L"tx1");		
			}
		}
		else{CP_XML_NODE(L"a:sysClr")	{	CP_XML_ATTR(L"val",L"windowText");}}
	}
}
void xlsx_drawing_context::serialize_line(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"a:ln")
		{
			if (drawing_state.back().line.width > 0)
				CP_XML_ATTR(L"w", drawing_state.back().line.width);//in emu (1 pt = 12700) ??? 
			CP_XML_NODE(L"a:" + drawing_state.back().line.type)
			{
				serialize_color(CP_XML_STREAM(), drawing_state.back().line.color);			
			}
		}
	}
}
void xlsx_drawing_context::serialize_bitmap_fill(std::wostream & stream, std::wstring rId, const std::wstring ns)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(ns + L"blipFill")
		{
			CP_XML_NODE(L"a:blip")
			{
				CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
				CP_XML_ATTR(L"r:embed", rId);
			}

			CP_XML_NODE(L"a:srcRect")
			{
				if (drawing_state.back().image_crop_enabled)
				{		
					CP_XML_ATTR(L"l", static_cast<size_t>(drawing_state.back().image_crop[0]));
					CP_XML_ATTR(L"t", static_cast<size_t>(drawing_state.back().image_crop[1]));
					CP_XML_ATTR(L"r", static_cast<size_t>(drawing_state.back().image_crop[2]));
					CP_XML_ATTR(L"b", static_cast<size_t>(drawing_state.back().image_crop[3]));
				}
			}
			CP_XML_NODE(L"a:stretch")
			{
				CP_XML_NODE(L"a:fillRect");
			}
		}
	}
}

void xlsx_drawing_context::serialize_pic(std::wstring rId)
{
	std::wstringstream strm;

	CP_XML_WRITER(strm)    
	{
		CP_XML_NODE(L"xdr:pic")
		{ 
			CP_XML_NODE(L"xdr:nvPicPr")
			{
				CP_XML_NODE(L"xdr:cNvPr")
				{
					if (drawing_state.back().id >= 0)	CP_XML_ATTR(L"id", drawing_state.back().id);
					if (drawing_state.back().name.empty())	drawing_state.back().name = L"Picture_" + rId.substr(5);

					CP_XML_ATTR(L"name", drawing_state.back().name);
					if (!drawing_state.back().description.empty())
					{
						CP_XML_ATTR(L"descr", drawing_state.back().description);
					}

					if (!drawing_state.back().hyperlink.empty())
					{
						CP_XML_NODE(L"a:hlinkClick")
						{
							CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
							//CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
							
							CP_XML_ATTR(L"r:id", drawing_state.back().hyperlink);
						}
					}
				}
				CP_XML_NODE(L"xdr:cNvPicPr")
				{
					CP_XML_NODE(L"a:picLocks")
					{
						CP_XML_ATTR(L"noChangeAspect", 1);
					}
				}
			}
			serialize_bitmap_fill(CP_XML_STREAM(), rId, L"xdr:");

			CP_XML_NODE(L"xdr:spPr")
			{
				serialize_xfrm(CP_XML_STREAM());

				CP_XML_NODE(L"a:prstGeom")
				{
					CP_XML_ATTR(L"prst", "rect");
					CP_XML_NODE(L"a:avLst");
				}

				CP_XML_NODE(L"a:noFill");
			}
		}
	}

	drawing_state.back().shape = strm.str();
}

void xlsx_drawing_context::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"xdr:twoCellAnchor")
		{ 
			//CP_XML_ATTR(L"editAs", L"oneCell");

			CP_XML_STREAM() << drawing_state.back().anchor;
			CP_XML_STREAM() << drawing_state.back().shape;
			
			CP_XML_NODE(L"xdr:clientData");
		}
	}
}
void xlsx_drawing_context::set_name(std::wstring & str)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().name = str;
}
void xlsx_drawing_context::set_description(std::wstring & str)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().description = str;
}
void xlsx_drawing_context::set_anchor(std::wstring & str)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().anchor = str;
}
void xlsx_drawing_context::set_image(std::wstring & str)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().image_target = str;
}

void xlsx_drawing_context::set_crop_top	(long val)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().image_crop[1] = val * 1.5;
}
void xlsx_drawing_context::set_crop_bottom(long val)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().image_crop[3] = val* 1.5;
}
void xlsx_drawing_context::set_crop_left (long val)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().image_crop[0]= val* 1.5;
}
void xlsx_drawing_context::set_crop_right (long val)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().image_crop[2] = val* 1.5;
}
void xlsx_drawing_context::set_rotation (long val)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().rotation = val;
}
void xlsx_drawing_context::set_line_color (std::wstring color)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().line.color.rgb = color;
}
void xlsx_drawing_context::set_line_color (int index, int type)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().line.color.index = index;
	
	if (type == 1)		drawing_state.back().line.color.bScheme = true;
	else if (type == 2) drawing_state.back().line.color.bPalette = true;

}
void xlsx_drawing_context::set_line_type (long val)
{
	if (drawing_state.size() < 1 )return;
	switch(val)
	{
	case 0: drawing_state.back().line.type = L"solidFill";		break;
	case 1: drawing_state.back().line.type = L"patternFill";	break;
	case 2: drawing_state.back().line.type = L"blipFill";		break;
	}
}
void xlsx_drawing_context::set_line_style (long val)
{
	if (drawing_state.size() < 1 )return;
	switch(val)
	{
	case 0: drawing_state.back().line.style = L"simple"; break;
	case 1: drawing_state.back().line.style = L"double"; break;
	case 2: drawing_state.back().line.style = L"thickThin"; break;
	case 3: drawing_state.back().line.style = L"thinThick"; break;
	case 4: drawing_state.back().line.style = L"triple"; break;
	}
}

void xlsx_drawing_context::set_line_width (long val)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().line.width = val;
}

void xlsx_drawing_context::set_hyperlink(std::wstring & str)
{
	if (drawing_state.size() < 1 )return;

	std::wstring hId=std::wstring(L"hId") + boost::lexical_cast<std::wstring>(hlinks_.size()+1);
	
	std::wstring href_correct = xml::utils::replace_text_to_xml(str);

	_hlink_desc desc = {hId, href_correct};

	hlinks_.push_back(desc);
	drawing_state.back().hyperlink = hId;

	xlsx_drawings_->add( false, hId , href_correct, external_items::typeHyperlink);
}

void xlsx_drawing_context::set_path (const std::wstring & path)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().path = path;
}

void xlsx_drawing_context::set_path_rect(_rect & rect)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().path_rect = rect;
}

void xlsx_drawing_context::set_properties(std::wstring & str)
{
	if (drawing_state.size() < 1 )return;
	drawing_state.back().shape = str;
}

bool xlsx_drawing_context::empty()
{
	return xlsx_drawings_->empty();
}

xlsx_drawings_ptr xlsx_drawing_context::get_drawings()
{
    return xlsx_drawings_;
}

}

