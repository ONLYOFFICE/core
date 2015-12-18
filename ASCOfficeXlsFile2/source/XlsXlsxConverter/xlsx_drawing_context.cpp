
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "xlsx_conversion_context.h"

#include <simple_xml_writer.h>
#include <utils.h>
#include <Auxiliary/HelpFunc.h>

#include "../../../DesktopEditor/raster/BgraFrame.h"

namespace oox {

	const static std::wstring shemeColor[18] = 
	{L"accent1",L"accent2",L"accent3",L"accent4",L"accent5",L"accent6",L"bk1",L"bk2",L"dk1",L"dk2",L"folHlink",L"hlink",L"lt1",L"lt2",L"none", L"tx1",L"tx2",L"phClr"};

	const static int shemeDefaultColor[] = 
	{
		0x00000000,	0x00FFFFFF,	0x00FF0000,	0x0000FF00,	0x000000FF,	0x00FFFF00,	0x00FF00FF,	0x0000FFFF,	
		0x00000000,	0x00FFFFFF,	0x00FF0000,	0x0000FF00,	0x000000FF,	0x00FFFF00,	0x00FF00FF,	0x0000FFFF,	
		0x00800000,	0x00008000,	0x00000080,	0x00808000,	0x00800080,	0x00008080,	0x00C0C0C0,	0x00808080,	
		0x009999FF,	0x00993366,	0x00FFFFCC,	0x00CCFFFF,	0x00660066,	0x00FF8080,	0x000066CC,	0x00CCCCFF,	
		0x00000080,	0x00FF00FF,	0x00FFFF00,	0x0000FFFF,	0x00800080,	0x00800000,	0x00008080,	0x000000FF,	
		0x0000CCFF,	0x00CCFFFF,	0x00CCFFCC,	0x00FFFF99,	0x0099CCFF,	0x00FF99CC,	0x00CC99FF,	0x00FFCC99,	
		0x003366FF,	0x0033CCCC,	0x0099CC00,	0x00FFCC00,	0x00FF9900,	0x00FF6600,	0x00666699,	0x00969696,	
		0x00003366,	0x00339966,	0x00003300,	0x00333300,	0x00993300,	0x00993366,	0x00333399,	0x00333333
	};

	void _color::SetRGB(unsigned char nR, unsigned char  nG, unsigned char  nB)
	{
		nRGB = (nR<<16) | (nG<<8) | nB;
		sRGB = STR::toRGB(nR, nG, nB);

		index = -1;
	}
//-----------------------------------------------------------------------------------------------------
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
_color xlsx_drawing_context::CorrectSysColor(int nColorCode, _drawing_state_ptr & drawing_state)
{
	_color  color;

	unsigned short nParameter		= (unsigned short)(( nColorCode >> 16 ) & 0x00ff);  // the HiByte of nParameter is not zero, an exclusive AND is helping :o
	unsigned short nFunctionBits	= (unsigned short)(( nColorCode	& 0x00000f00 ) >> 8 );
	unsigned short nAdditionalFlags = (unsigned short)(( nColorCode	& 0x0000f000) >> 8 );
	unsigned short nColorIndex		= (unsigned short) ( nColorCode	& 0x00ff);
	unsigned short nPropColor		= 0;

	switch (nColorIndex)
	{
	case 0xF0:	color = drawing_state->fill.color;	break;
	case 0xF1:
	{
		if (drawing_state->line.type != fillNone)	color = drawing_state->line.color;
		else											color = drawing_state->fill.color;
	}break;
	case 0xF2:	color = drawing_state->line.color;		break;
	case 0xF3:	color = drawing_state->shadow.color;	break;
	case 0xF4:	break; ///this
	case 0xF5:	color = drawing_state->fill.color2;	break; 
	case 0xF6:	break; //lineBackColor
	case 0xF7:	//FillThenLine
	case 0xF8:	//colorIndexMask
		color = drawing_state->shadow.color;	break;
	default:
		//from table 
		break;
	}

	if (color.index != -1)
	{
		//вытащить цвет (
		//todoooo сдлать темы !!!!!
		color.nRGB = shemeDefaultColor[color.index];
		color.index = -1;
		color.sRGB = STR::toRGB(color.nRGB);
	}

    //if ( nCProp && ( nPropColor & 0x10000000 ) == 0 )       // beware of looping recursive
    //    color = CorrectSysColor( nPropColor, pElement);

    if( nAdditionalFlags & 0x80 )           // make color gray
    {
        BYTE nZwi = 0x7f;//= aColor.GetLuminance();
		color.SetRGB(nZwi, nZwi, nZwi);
    }
    switch( nFunctionBits )
    {
        case 0x01 :     // darken color by parameter
        {
            BYTE R = static_cast<BYTE>	( ( nParameter * color.GetR() ) >> 8 );
            BYTE G = static_cast<BYTE>	( ( nParameter * color.GetG() ) >> 8 );
            BYTE B = static_cast<BYTE> ( ( nParameter * color.GetB() ) >> 8 );
		
			color.SetRGB(R, G, B);
        }
        break;
        case 0x02 :     // lighten color by parameter
        {
            unsigned short nInvParameter = ( 0x00ff - nParameter ) * 0xff;
            BYTE R = static_cast<BYTE>( ( nInvParameter + ( nParameter * color.GetR() ) ) >> 8 );
            BYTE G = static_cast<BYTE>( ( nInvParameter + ( nParameter * color.GetG() ) ) >> 8 );
            BYTE B = static_cast<BYTE>( ( nInvParameter + ( nParameter * color.GetB() ) ) >> 8 );

			color.SetRGB(R, G, B);
        }
        break;
        case 0x03 :     // add grey level RGB(p,p,p)
        {
            short nR = (short)color.GetR()	+ (short)nParameter;
            short nG = (short)color.GetG()	+ (short)nParameter;
            short nB = (short)color.GetB()	+ (short)nParameter;

            if ( nR > 0x00ff )	nR = 0x00ff;
            if ( nG > 0x00ff )	nG = 0x00ff;
            if ( nB > 0x00ff )	nB = 0x00ff;

           color.SetRGB( (BYTE)nR, (BYTE)nG, (BYTE)nB );
        }
        break;
        case 0x04 :     // substract grey level RGB(p,p,p)
        {
            short nR = (short)color.GetR() - (short)nParameter;
            short nG = (short)color.GetG() - (short)nParameter;
            short nB = (short)color.GetB() - (short)nParameter;
            if ( nR < 0 ) nR = 0;
            if ( nG < 0 ) nG = 0;
            if ( nB < 0 ) nB = 0;
            color.SetRGB( (BYTE)nR, (BYTE)nG, (BYTE)nB );
        }
        break;
        case 0x05 :     // substract from gray level RGB(p,p,p)
        {
            short nR = (short)nParameter - (short)color.GetR();
            short nG = (short)nParameter - (short)color.GetG();
            short nB = (short)nParameter - (short)color.GetB();
            if ( nR < 0 ) nR = 0;
            if ( nG < 0 ) nG = 0;
            if ( nB < 0 ) nB = 0;
            color.SetRGB( (BYTE)nR, (BYTE)nG, (BYTE)nB );
        }
        break;
        case 0x06 :     // per component: black if < p, white if >= p
        {
			BYTE R = color.GetR() < nParameter ? 0x00 : 0xff ;
            BYTE G = color.GetG() < nParameter ? 0x00 : 0xff ;
            BYTE B = color.GetB() < nParameter ? 0x00 : 0xff ;

			color.SetRGB(R, G, B);
        }
        break;
    }
    if ( nAdditionalFlags & 0x40 )                  // top-bit invert
        color.SetRGB( color.GetR() ^ 0x80, color.GetG() ^ 0x80, color.GetB() ^ 0x80 );

    if ( nAdditionalFlags & 0x20 )                  // invert color
        color.SetRGB(0xff - color.GetR(), 0xff - color.GetG(), 0xff - color.GetB());

	return color;
}

xlsx_drawing_context::xlsx_drawing_context(xlsx_conversion_context & Context)
 : context_(Context), handle_(Context.get_drawing_context_handle()), xlsx_drawings_(xlsx_drawings::create())
{    
	in_chart_		= false;
	count_object	= 0;

	current_level	= 0;

	current_drawing_states = &drawing_states ;
}
bool xlsx_drawing_context::start_drawing(int type)
{
	switch(type)
	{
	case 0x0000: // Group
		start_group();		return true;
	case 0x0001: // Line
	case 0x0002: // Rectangle
	case 0x0003: // Oval
	case 0x0004: // Arc
	case 0x0006: // Text
	case 0x001E: // OfficeArt object
	case 0x0009: // Polygon:
		start_shape(type);	return true;
	case 0x0008: // Picture
		start_image();		return true;
	case 0x0005: // Chart  
		//start_shape(2);	return true;
		start_chart();		return true;
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
		start_shape(0x0002); return true;
	case 0x0019: // Note
		break;
	}
	return false;

}
_drawing_state_ptr create_drawing_state()
{
	return _drawing_state_ptr(new _drawing_state());
}

void xlsx_drawing_context::start_image()
{
	if (current_drawing_states == NULL) return;

	current_drawing_states->push_back(create_drawing_state());

	current_drawing_states->back()->type = external_items::typeImage;
	
	count_object++;
}


void xlsx_drawing_context::start_group()
{
	current_drawing_states->push_back(create_drawing_state());
	current_level++;

	current_drawing_states->back()->parent_drawing_states = current_drawing_states;
	current_drawing_states = &current_drawing_states->back()->drawing_states;
	
	current_drawing_states->push_back(create_drawing_state());
}
void xlsx_drawing_context::end_group()
{
	current_drawing_states = current_drawing_states->back()->parent_drawing_states;

	current_level--;
}
void xlsx_drawing_context::start_chart()
{
	if (current_drawing_states == NULL) return;

	context_.start_chart();
	
	current_drawing_states->push_back(create_drawing_state());

	current_drawing_states->back()->type = external_items::typeChart;
	
	in_chart_ = true;
	count_object++;
}

void xlsx_drawing_context::start_shape(int type)
{
	if (current_drawing_states == NULL) return;

	current_drawing_states->push_back(create_drawing_state());

	current_drawing_states->back()->type = external_items::typeShape;
	current_drawing_states->back()->shape_type = type;

	if (0x0006 == type)
	{
		current_drawing_states->back()->shape_type = msosptTextBox;
		//drawing_state.back().bTextBox = true;
	}
	if (0x001E == type)
	{
		current_drawing_states->back()->shape_type = msosptTextBox;
		//drawing_state.back().bWordArt = true;
	}
	count_object++;
}

void xlsx_drawing_context::set_id(int id)
{
	if (current_drawing_states == NULL)		return;
	if (current_drawing_states->size() < 1) return;
	
	current_drawing_states->back()->id = id;
}
void xlsx_drawing_context::set_FlipH()
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->size() < 1) return;
	
	current_drawing_states->back()->flipH = true;
}
void xlsx_drawing_context::set_FlipV()
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->size() < 1) return;

	current_drawing_states->back()->flipV = true;
}
void xlsx_drawing_context::set_shape_id(int id)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->size() < 1) return;
	
	if (current_drawing_states->back()->bTextBox)	return;

	current_drawing_states->back()->shape_id = (MSOSPT)id;
}
void xlsx_drawing_context::end_drawing()
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->size() < 1) return;
	
	if (!current_drawing_states->back()->anchor.empty())
	{
		end_drawing(current_drawing_states->back());
	}
}

void xlsx_drawing_context::end_drawing(_drawing_state_ptr & drawing_state)
{
	if (!drawing_state) return;

	if (drawing_state->id < 0)	
		drawing_state->id = count_object + 0x20000;

	if ( drawing_state->type		== external_items::typeImage ||
		drawing_state->shape_id	== msosptPictureFrame )
	{
		drawing_state->type = external_items::typeImage;

		if (!drawing_state->fill.texture_target.empty())
		{
			bool isIternal = false;
			std::wstring rId = handle_.impl_->get_mediaitems().find_image( drawing_state->fill.texture_target, isIternal);
			
			serialize_pic(drawing_state, rId);
		
			//std::wstringstream strm;
			//serialize(strm, drawing_state);

			xlsx_drawings_->add(/*strm.str(), */isIternal, 	rId , drawing_state->fill.texture_target, drawing_state->type);
		}
		else 
			drawing_state->type = external_items::typeShape;
	}
	if ( drawing_state->type == external_items::typeChart )
	{
 //функциональная часть		
		context_.end_chart();
		
		in_chart_ = false;
	
//отобразительная часть
		drawing_state->type = external_items::typeChart;

		bool			isIternal = true;
		std::wstring	target;
		std::wstring rId = handle_.impl_->get_mediaitems().add_chart(target);
		
		serialize_chart(drawing_state, rId); 
		//
		//std::wstringstream strm;
		//serialize(strm, drawing_state);
		
		xlsx_drawings_->add(/*strm.str(),*/ isIternal, 	rId , target, drawing_state->type);
	}
	if ( drawing_state->type == external_items::typeShape)
	{
		serialize_shape(drawing_state);

		//std::wstringstream strm;
		//serialize(strm, drawing_state);
		xlsx_drawings_->add(/*strm.str(), */true, L"" , L"", drawing_state->type);
	}


}
void xlsx_drawing_context::serialize_pic(_drawing_state_ptr & drawing_state, std::wstring rId)
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
					CP_XML_ATTR(L"id", drawing_state->id);
					
					if (drawing_state->name.empty())	
						drawing_state->name = L"Picture_" + rId.substr(5);
					CP_XML_ATTR(L"name", drawing_state->name);

					if (!drawing_state->description.empty())
					{
						CP_XML_ATTR(L"descr", drawing_state->description);
					}

					if (!drawing_state->hyperlink.empty())
					{
						CP_XML_NODE(L"a:hlinkClick")
						{
							CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
							//CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
							
							CP_XML_ATTR(L"r:id", drawing_state->hyperlink);
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
			serialize_bitmap_fill(CP_XML_STREAM(), drawing_state, rId, L"xdr:");

			CP_XML_NODE(L"xdr:spPr")
			{
				serialize_xfrm(CP_XML_STREAM(), drawing_state);

				CP_XML_NODE(L"a:prstGeom")
				{
					CP_XML_ATTR(L"prst", "rect");
					CP_XML_NODE(L"a:avLst");
				}

				CP_XML_NODE(L"a:noFill");
			}
		}
	}

	drawing_state->shape = strm.str();
}

void xlsx_drawing_context::serialize_chart(_drawing_state_ptr & drawing_state, std::wstring rId)
{
	std::wstringstream strm;

    CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"xdr:graphicFrame")
        {                  
            CP_XML_NODE(L"xdr:nvGraphicFramePr")
            {
                CP_XML_NODE(L"xdr:cNvPr")
                {
					CP_XML_ATTR(L"id", drawing_state->id);
					
					if (drawing_state->name.empty())	
						drawing_state->name = L"Chart_" + boost::lexical_cast<std::wstring>(count_object);

					CP_XML_ATTR(L"name", drawing_state->name);
					if (!drawing_state->description.empty())
					{
						CP_XML_ATTR(L"descr", drawing_state->description);
					}
                }

                CP_XML_NODE(L"xdr:cNvGraphicFramePr");
            } 
			serialize_xfrm(CP_XML_STREAM(), drawing_state);

			//serialize_fill(CP_XML_STREAM());
			//serialize_line(CP_XML_STREAM());	

            CP_XML_NODE(L"a:graphic")
            {                   
                CP_XML_NODE(L"a:graphicData")
				{
					CP_XML_ATTR(L"uri", L"http://schemas.openxmlformats.org/drawingml/2006/chart");
					CP_XML_NODE(L"c:chart")
					{
						CP_XML_ATTR(L"xmlns:c", L"http://schemas.openxmlformats.org/drawingml/2006/chart");
						CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
						CP_XML_ATTR(L"r:id", rId);
					}
				}
			}               
		} 
    }  
	drawing_state->shape = strm.str();
}

void xlsx_drawing_context::serialize_shape(_drawing_state_ptr & drawing_state)
{
	std::wstringstream strm;

	std::wstring prstTxWarp;
	std::wstring prstGeom	= Spt2ShapeType(drawing_state->shape_id);

	if (prstGeom.empty())
	{
		prstTxWarp = Spt2WordArtShapeType(drawing_state->shape_id);
		if (prstTxWarp.empty() == false)
		{
			drawing_state->bWordArt = true;
			drawing_state->bTextBox = true;
			prstGeom = L"rect";
		}
		else if (drawing_state->path.empty())
		{
			//error !!!
			prstGeom = L"rect";
		}
	}
	else
	{
		if (drawing_state->shape_id == msosptTextBox)	drawing_state->bTextBox = true;
	}
	
	CP_XML_WRITER(strm)    
	{
		CP_XML_NODE(L"xdr:sp")
		{ 
			CP_XML_NODE(L"xdr:nvSpPr")
			{
				CP_XML_NODE(L"xdr:cNvPr")
				{
					CP_XML_ATTR(L"id", drawing_state->id);
					
					if (drawing_state->name.empty())	
						drawing_state->name = L"Shape_" + boost::lexical_cast<std::wstring>(count_object);
					CP_XML_ATTR(L"name", drawing_state->name);
					
					if (!drawing_state->description.empty())
					{
						CP_XML_ATTR(L"descr", drawing_state->description);
					}

					if (!drawing_state->hyperlink.empty())
					{
						CP_XML_NODE(L"a:hlinkClick")
						{
							CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
							
							CP_XML_ATTR(L"r:id", drawing_state->hyperlink);
						}
					}
				}
				CP_XML_NODE(L"xdr:cNvSpPr")
				{
					if (drawing_state->bTextBox)CP_XML_ATTR(L"txBox", 1);
				}
			}
			
			CP_XML_NODE(L"xdr:spPr")
			{
				serialize_xfrm(CP_XML_STREAM(), drawing_state);

				if (prstGeom.empty() == false)
				{
					CP_XML_NODE(L"a:prstGeom")
					{
						CP_XML_ATTR(L"prst", prstGeom);
						if (!drawing_state->bWordArt)	CP_XML_NODE(L"a:avLst");
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
							int w = drawing_state->path_rect.right - drawing_state->path_rect.left;
							int h = drawing_state->path_rect.bottom- drawing_state->path_rect.top;

							if (w > 0 && h > 0)
							{
								CP_XML_NODE(L"a:path")
								{
									CP_XML_ATTR(L"w", w);
									CP_XML_ATTR(L"h", h);
									
									CP_XML_STREAM() << drawing_state->path;
								}
							}
						}
					}
				}
				serialize_fill(CP_XML_STREAM(), drawing_state);
				serialize_line(CP_XML_STREAM(), drawing_state);		
			}
			serialize_text(CP_XML_STREAM(), drawing_state);
		}
	}

	drawing_state->shape = strm.str();
}
void xlsx_drawing_context::serialize_fill(std::wostream & stream)
{
	serialize_fill(stream, current_drawing_states->back());
}
void xlsx_drawing_context::serialize_fill(std::wostream & stream, _drawing_state_ptr & drawing_state)
{
	if (drawing_state->fill.type == fillNone)
	{
		serialize_none_fill(stream);
		return;
	}
	if (drawing_state->fill.type == fillPattern)
	{
		std::wstring rgbColor1 = drawing_state->fill.color.sRGB;
		std::wstring rgbColor2 = drawing_state->fill.color2.sRGB;

		if (rgbColor1.empty())
		{
			if (drawing_state->fill.color.index >= 8)
				rgbColor1 = standart_color[drawing_state->fill.color.index - 8];
			else 
				rgbColor1 = L"0";
		}
		if (rgbColor2.empty() )
		{
			if ( drawing_state->fill.color2.index >= 8)
				rgbColor2 = standart_color[drawing_state->fill.color2.index - 8];
			else
				rgbColor2 = L"0";
		}
		ChangeBlack2ColorImage( rgbColor2, rgbColor1, drawing_state );
		
		drawing_state->fill.type			= fillTexture;
		drawing_state->fill.texture_mode	= textureTill;
	}
	if (!drawing_state->fill.texture_target.empty() && drawing_state->fill.type == fillTexture)
	{
		bool  isIternal = false;
		std::wstring rId = handle_.impl_->get_mediaitems().find_image( drawing_state->fill.texture_target, isIternal);
		if (!rId.empty())
		{
			if (in_chart_ == true)
			{
				context_.current_chart().add_rels( isIternal, rId , drawing_state->fill.texture_target, external_items::typeImage);
			}
			else 
			{
				xlsx_drawings_->add(isIternal, 	rId , drawing_state->fill.texture_target, external_items::typeImage);
			}
			serialize_bitmap_fill(stream, drawing_state, rId);
			return;
		}
	}
	if (drawing_state->fill.type == fillGradient || drawing_state->fill.type == fillGradientOne)
	{
		serialize_gradient_fill(stream, drawing_state);
		return;
	}
//------------ 
	if (drawing_state->fill.color.index>=0 || !drawing_state->fill.color.sRGB.empty())
	{
		serialize_solid_fill(stream, drawing_state->fill.color, drawing_state->fill.opacity);
	}
	else serialize_none_fill(stream);
}

void xlsx_drawing_context::serialize_none_fill(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"a:noFill");
	}
}

void xlsx_drawing_context::serialize_solid_fill(std::wostream & stream, const _color & color, double opacity)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"a:solidFill")
		{
			serialize_color(CP_XML_STREAM(), color, opacity);	
		}
	}
}

void xlsx_drawing_context::serialize_gradient_fill(std::wostream & stream, _drawing_state_ptr & drawing_state)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"a:gradFill")
		{
			CP_XML_NODE(L"a:gsLst")
			{
				if ( !drawing_state->fill.colorsPosition.empty() )
				{
					for (int i = 0; i < drawing_state->fill.colorsPosition.size(); i++)
					{
						CP_XML_NODE(L"a:gs")
						{
							CP_XML_ATTR(L"pos",  (int)(drawing_state->fill.colorsPosition[i].first * 100000));
							serialize_color(CP_XML_STREAM(), drawing_state->fill.colorsPosition[i].second);
							//проверить что если тут индексы то они берутся с программных а не с юзерских (см как ниже)
						}
					}
				}
				else
				{
					CP_XML_NODE(L"a:gs")
					{
						drawing_state->fill.color.bScheme = false; // по общим индексам
						CP_XML_ATTR(L"pos", 0);
						serialize_color(CP_XML_STREAM(), drawing_state->fill.color, drawing_state->fill.opacity);
					}
					CP_XML_NODE(L"a:gs")
					{
						drawing_state->fill.color2.bScheme = false; // по общим индексам
						CP_XML_ATTR(L"pos", 100000);
						serialize_color(CP_XML_STREAM(), drawing_state->fill.color2, drawing_state->fill.opacity2);
					}
				}
			}
			CP_XML_NODE(L"a:lin")
			{
				CP_XML_ATTR(L"ang", (int)( 90 - drawing_state->fill.angle) * 60000);
				CP_XML_ATTR(L"scaled", 1);
			}
		}
	}
}

void xlsx_drawing_context::serialize_xfrm(std::wostream & stream, _drawing_state_ptr & drawing_state)
{
	std::wstring nameXfrm = L"a:xfrm";
	
	if (drawing_state->type == external_items::typeChart)	
		nameXfrm = L"xdr:xfrm";

	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(nameXfrm)
		{
			if (drawing_state->flipV)			CP_XML_ATTR(L"flipV", true);
			if (drawing_state->flipH)			CP_XML_ATTR(L"flipH", true);
			
			if (drawing_state->rotation != 0)	CP_XML_ATTR(L"rot", drawing_state->rotation);
			
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

void xlsx_drawing_context::serialize_color	(std::wostream & stream, const _color &color, double opacity)
{
	CP_XML_WRITER(stream)    
	{
		if (!color.sRGB.empty())
		{
			CP_XML_NODE(L"a:srgbClr")
			{
				CP_XML_ATTR(L"val", color.sRGB);		
				if (opacity > 0.00001)
				{
					CP_XML_NODE(L"a:alpha")
					{
						CP_XML_ATTR(L"val", (int)(opacity * 100000));
					}
				}
			}
		}
		else if (color.index >=0 && color.index < 17 && color.bScheme && color.index != 14)
		{
			CP_XML_NODE(L"a:schemeClr")
			{
				CP_XML_ATTR(L"val",  shemeColor[color.index]);		
				if (opacity > 0.00001)
				{
					CP_XML_NODE(L"a:alpha")
					{
						CP_XML_ATTR(L"val", (int)(opacity * 100000));
					}
				}
			}
		}
		else if (color.index >= 8 && color.index < 57)
		{
			CP_XML_NODE(L"a:srgbClr")
			{
				CP_XML_ATTR(L"val", standart_color[color.index - 8]);		
				if (opacity > 0.00001)
				{
					CP_XML_NODE(L"a:alpha")
					{
						CP_XML_ATTR(L"val", (int)(opacity * 100000));
					}
				}
			}	
		}
		else
		{
			CP_XML_NODE(L"a:schemeClr")
			{
				CP_XML_ATTR(L"val",  L"lt1");		
			}
		}
	}
}

void xlsx_drawing_context::serialize_text(std::wostream & stream, _drawing_state_ptr & drawing_state)
{
	if (drawing_state->text_content.empty()) return;

	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"xdr:txBody")
		{  
			CP_XML_NODE(L"a:bodyPr")
			{
				if (drawing_state->bWordArt)
				{
					std::wstring prstTxWarp = Spt2WordArtShapeType(drawing_state->shape_id);				
					CP_XML_NODE(L"a:prstTxWarp")
					{
						CP_XML_ATTR(L"prst", prstTxWarp);
						CP_XML_NODE(L"a:avLst");//модификаторы
					}
				}
			}
			CP_XML_NODE(L"a:lstStyle");		
			
			CP_XML_NODE(L"a:p")
			{
				CP_XML_STREAM() << drawing_state->text_content;
			}
		}
	}
}

void xlsx_drawing_context::serialize_line(std::wostream & stream, _drawing_state_ptr & drawing_state)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"a:ln")
		{
			if (drawing_state->line.width > 0)
				CP_XML_ATTR(L"w", drawing_state->line.width);//in emu (1 pt = 12700) ??? 

			if (drawing_state->line.type == fillSolid)
			{
				serialize_solid_fill(CP_XML_STREAM(), drawing_state->line.color, drawing_state->line.opacity );
			}
			CP_XML_NODE(L"a:prstDash")
			{
				switch(drawing_state->line.typeDash)
				{	
				case lineSolid:		CP_XML_ATTR(L"val", L"solid");		break;
				case lineDash:		CP_XML_ATTR(L"val", L"dash");		break;
				case lineDot:		CP_XML_ATTR(L"val", L"dot");		break;
				case lineDashDot:	CP_XML_ATTR(L"val", L"dashDot");	break;
				case lineDashDotDot:CP_XML_ATTR(L"val", L"lgDashDotDot"); break;
				}
			}
		}
	}
}
void xlsx_drawing_context::serialize_bitmap_fill(std::wostream & stream, _drawing_state_ptr & drawing_state, std::wstring rId, const std::wstring ns)
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
				if (drawing_state->fill.texture_crop_enabled)
				{		
					CP_XML_ATTR(L"l", static_cast<size_t>(drawing_state->fill.texture_crop[0]));
					CP_XML_ATTR(L"t", static_cast<size_t>(drawing_state->fill.texture_crop[1]));
					CP_XML_ATTR(L"r", static_cast<size_t>(drawing_state->fill.texture_crop[2]));
					CP_XML_ATTR(L"b", static_cast<size_t>(drawing_state->fill.texture_crop[3]));
				}
			}
			if (drawing_state->fill.texture_mode == textureStretch)
			{
				CP_XML_NODE(L"a:stretch")
				{
					CP_XML_NODE(L"a:fillRect");
				}
			}
			if (drawing_state->fill.texture_mode == textureTill)
			{
				CP_XML_NODE(L"a:tile");
			}
		}
	}
}


void xlsx_drawing_context::serialize(std::wostream & stream, _drawing_state_ptr & drawing_state)
{
	std::wstring sNodeAnchor;

	if		(drawing_state->type_anchor == 1)	sNodeAnchor = L"xdr:twoCellAnchor";
	else if (drawing_state->type_anchor == 2)	sNodeAnchor = L"xdr:oneCellAnchor";
	else										sNodeAnchor = L"xdr:absoluteAnchor";

	if (sNodeAnchor.empty()) return;
	
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(sNodeAnchor)
		{ 
			//CP_XML_ATTR(L"editAs", L"oneCell");

			CP_XML_STREAM() << drawing_state->anchor;
			CP_XML_STREAM() << drawing_state->shape;
			
			CP_XML_NODE(L"xdr:clientData");
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
void xlsx_drawing_context::set_name(const std::wstring & str)
{
	if (current_drawing_states == NULL) return;
	
	current_drawing_states->back()->name = str;
}
void xlsx_drawing_context::set_description(const std::wstring & str)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->description = str;
}
void xlsx_drawing_context::set_anchor(const std::wstring & str)
{
	if (str.empty())				return;
	if (current_drawing_states == NULL) return;	
	
	current_drawing_states->back()->anchor = str;
}

bool xlsx_drawing_context::is_anchor()
{
	if (current_drawing_states == NULL) return false;

	return !current_drawing_states->back()->anchor.empty();
}
void xlsx_drawing_context::set_fill_texture_mode(int val)
{
	if (current_drawing_states == NULL) return;
		
	current_drawing_states->back()->fill.texture_mode = (_texture_mode)val;
}
void xlsx_drawing_context::set_fill_texture(const std::wstring & str)
{
	if (current_drawing_states == NULL) return;
	
	if (str.empty()) return;

	current_drawing_states->back()->fill.texture_target = str;
}

void xlsx_drawing_context::set_crop_top	(double val)
{
	if (current_drawing_states == NULL) return;	
	
	current_drawing_states->back()->fill.texture_crop[1] = val;
    current_drawing_states->back()->fill.texture_crop_enabled = true;
}
void xlsx_drawing_context::set_crop_bottom(double val)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->fill.texture_crop[3] = val;
    current_drawing_states->back()->fill.texture_crop_enabled = true;
}
void xlsx_drawing_context::set_crop_left (double val)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->fill.texture_crop[0] = val;
    current_drawing_states->back()->fill.texture_crop_enabled = true;
}
void xlsx_drawing_context::set_crop_right (double val)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->fill.texture_crop[2] = val;
    current_drawing_states->back()->fill.texture_crop_enabled = true;
}
void xlsx_drawing_context::set_rotation (double val)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->rotation = val;
}
void xlsx_drawing_context::set_line_color (int nColor, const std::wstring & sColor)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->line.color.sRGB = sColor;
	current_drawing_states->back()->line.color.nRGB = nColor;
}
void xlsx_drawing_context::set_line_color (int index, int type)
{
	if (current_drawing_states == NULL) return;
	
	//drawing_state.back().line.color.index	= index;	
	//drawing_state.back().line.color.sRGB		= L"";

	current_drawing_states->back()->line.color.nRGB = shemeDefaultColor[index];
	current_drawing_states->back()->line.color.index = -1;
	current_drawing_states->back()->line.color.sRGB = STR::toRGB(shemeDefaultColor[index]);

	if (type == 1)		current_drawing_states->back()->line.color.bScheme = true;
}
void xlsx_drawing_context::set_fill_angle (double val)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->fill.angle = val;
}
void xlsx_drawing_context::set_fill_color (int nColor, const std::wstring & sColor, bool background)
{
	if (current_drawing_states == NULL) return;	

	if (background)	
	{
		current_drawing_states->back()->fill.color2.sRGB = sColor;
		current_drawing_states->back()->fill.color2.nRGB = nColor;
	}
	else			
	{
		current_drawing_states->back()->fill.color.sRGB = sColor;
		current_drawing_states->back()->fill.color.nRGB = nColor;
	}
}
void xlsx_drawing_context::set_fill_color (int index, int type, bool background)
{
	if (current_drawing_states == NULL) return;	
	
	_color color;
	if (type == 3)
	{
		color = CorrectSysColor(index, current_drawing_states->back());
	}
	else
	{	
	//	color.index		= index;	
	//	color.sRGB		= L"";
	//	color.bScheme = true;
		color.nRGB = shemeDefaultColor[index];
		color.index = -1;
		color.sRGB = STR::toRGB(color.nRGB);
	}

	if (background)	current_drawing_states->back()->fill.color2	= color;
	else			current_drawing_states->back()->fill.color		= color;
}
void xlsx_drawing_context::set_fill_opacity	(double val, bool background)
{
	if (current_drawing_states == NULL) return;
	
	
	if (background)	current_drawing_states->back()->fill.opacity2	= val;
	else			current_drawing_states->back()->fill.opacity	= val;
}
void xlsx_drawing_context::add_fill_colors(double position, const std::wstring & col)
{
	if (current_drawing_states == NULL) return;
	

	_color color;
	color.sRGB = col;
	current_drawing_states->back()->fill.colorsPosition.push_back(std::pair<double, _color>(position, color));
}
void xlsx_drawing_context::add_fill_colors(double position, int index, int type)
{
	if (current_drawing_states == NULL) return;	

	_color color;
	color.index = index;
	if (type == 1)	color.bScheme = true;

	current_drawing_states->back()->fill.colorsPosition.push_back(std::pair<double, _color>(position, color));
}
void xlsx_drawing_context::set_fill_type (int val)
{
	if (current_drawing_states == NULL) return;
	

	current_drawing_states->back()->fill.type = (_fill_type) val;
}
void xlsx_drawing_context::set_line_dash(int val)
{
	if (current_drawing_states == NULL) return;	

	switch (val)
	{
		case 0:	current_drawing_states->back()->line.typeDash = lineSolid; break;
		case 1: 
		case 6:
		case 7:	current_drawing_states->back()->line.typeDash = lineDash; break;
		case 2:
		case 5: current_drawing_states->back()->line.typeDash = lineDot; break;
		case 3:
		case 8:
		case 9:	current_drawing_states->back()->line.typeDash = lineDashDot; break;
		case 4:
		case 10:current_drawing_states->back()->line.typeDash = lineDashDotDot; break;
	}
}
void xlsx_drawing_context::set_line_type (int val)
{
	if (current_drawing_states == NULL) return;	

	switch(val)
	{
	case 0: current_drawing_states->back()->line.type = fillSolid;		break;
	case 1: current_drawing_states->back()->line.type = fillPattern;	break;
	case 2: current_drawing_states->back()->line.type = fillTexture;	break;
	}
}
void xlsx_drawing_context::set_line_style (int val)
{
	if (current_drawing_states == NULL) return;	

	switch(val)
	{
	case 0: current_drawing_states->back()->line.style = L"simple"; break;
	case 1: current_drawing_states->back()->line.style = L"double"; break;
	case 2: current_drawing_states->back()->line.style = L"thickThin"; break;
	case 3: current_drawing_states->back()->line.style = L"thinThick"; break;
	case 4: current_drawing_states->back()->line.style = L"triple"; break;
	}
}

void xlsx_drawing_context::set_line_width (int val)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->line.width = val;
}

void xlsx_drawing_context::set_hyperlink(const std::wstring & str)
{
	if (current_drawing_states == NULL) return;	

	std::wstring hId=std::wstring(L"hId") + boost::lexical_cast<std::wstring>(hlinks_.size()+1);
	
	std::wstring href_correct = xml::utils::replace_text_to_xml(str);

	_hlink_desc desc = {hId, href_correct};

	hlinks_.push_back(desc);
	current_drawing_states->back()->hyperlink = hId;

	xlsx_drawings_->add( false, hId , href_correct, external_items::typeHyperlink);
}

void xlsx_drawing_context::set_path (const std::wstring & path)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->path = path;
}

void xlsx_drawing_context::set_text (const std::wstring & text)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->text_content = text;
}

void xlsx_drawing_context::set_path_rect(_rect & rect)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->path_rect = rect;
}

void xlsx_drawing_context::set_properties(const std::wstring & str)
{
	if (current_drawing_states == NULL) return;
	
	current_drawing_states->back()->shape = str;
}

bool xlsx_drawing_context::empty()
{
	return drawing_states.empty();
}

xlsx_drawings_ptr xlsx_drawing_context::get_drawings()
{
    return xlsx_drawings_;
}

bool xlsx_drawing_context::ChangeBlack2ColorImage(std::wstring sRgbColor1, std::wstring sRgbColor2, _drawing_state_ptr & drawing_state)
{
	if (drawing_state->fill.texture_target.length() < 6) return false;

	std::wstring image_path = context_.get_mediaitems().media_path() + drawing_state->fill.texture_target.substr(6);

	int rgbColor1 = STR::hex_str2int(sRgbColor1);
	int rgbColor2 = STR::hex_str2int(sRgbColor2);

	CBgraFrame bgraFrame;

	if (bgraFrame.OpenFile(image_path))
	{
		int smpl = abs(bgraFrame.get_Stride() / bgraFrame.get_Width());

		BYTE * rgb = bgraFrame.get_Data();
		
		BYTE R1 = (BYTE)(rgbColor1);
		BYTE G1 = (BYTE)(rgbColor1 >> 8);
		BYTE B1 = (BYTE)(rgbColor1 >> 16);

		BYTE R2 = (BYTE)(rgbColor2);
		BYTE G2 = (BYTE)(rgbColor2 >> 8);
		BYTE B2 = (BYTE)(rgbColor2 >> 16);

		for (int i = 0 ; i < bgraFrame.get_Width() * bgraFrame.get_Height(); i++)
		{
			if (rgb[i * smpl + 0 ] == 0x00 && rgb[i * smpl + 1 ] == 0x00 && rgb[i * smpl + 2 ] == 0x00)
			{
				rgb[i * smpl + 0 ] = R1;
				rgb[i * smpl + 1 ] = G1;
				rgb[i * smpl + 2 ] = B1;
			}
			else
			{
				rgb[i * smpl + 0 ] = R2;
				rgb[i * smpl + 1 ] = G2;
				rgb[i * smpl + 2 ] = B2;
			}
		}
		bgraFrame.SaveFile(image_path, 1); 
		return true;
	}
	return false;
}
void xlsx_drawing_context::set_chart_sheet_anchor(double width, double height)
{
	if (current_drawing_states == NULL) return;	

	std::wstringstream stream;

	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"xdr:pos")
		{ 
			CP_XML_ATTR(L"x", 0);
			CP_XML_ATTR(L"y", 0);
		}
		CP_XML_NODE(L"xdr:ext")
		{ 
			CP_XML_ATTR(L"cx", (int)(width * 12700)); //in emu (1 pt = 12700)
			CP_XML_ATTR(L"cy", (int)(height * 12700)); //in emu (1 pt = 12700)
		}
	}

	set_anchor(stream.str());

	current_drawing_states->back()->type_anchor = 2;
}

void xlsx_drawing_context::serialize(std::wostream & strm) 
{
    CP_XML_WRITER(strm)
    {
        CP_XML_NODE(L"xdr:wsDr")
        {
            CP_XML_ATTR(L"xmlns:xdr", L"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing");
            CP_XML_ATTR(L"xmlns:a"	, L"http://schemas.openxmlformats.org/drawingml/2006/main");
            CP_XML_ATTR(L"xmlns:r"	, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");

			for (int i = 0 ; i < drawing_states.size(); i++)
			{
				if (drawing_states[i]->drawing_states.size() > 1)
				{
					//группа
				}
				else
				{
					serialize(CP_XML_STREAM(), drawing_states[i]);
				}
			}

		}
    }
}
}