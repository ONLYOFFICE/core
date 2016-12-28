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

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "xlsx_conversion_context.h"

#include <simple_xml_writer.h>
#include <utils.h>
#include <Auxiliary/HelpFunc.h>

#include "../../../DesktopEditor/raster/BgraFrame.h"

#include "ConvertShapes/CustomShape.h"
#include "ConvertShapes/CustomShapeConvert.h"

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
    Impl(size_t & next_vml_file_id, external_items & items)
				: next_vml_file_id_(next_vml_file_id), items_(items), next_rId_(1), next_drawing_id_(1) 
    {
    }  

    external_items & get_mediaitems() { return items_; }

    size_t next_rId()
    {
        return next_rId_++;
    }

    std::pair<std::wstring, std::wstring> add_drawing_xml(std::wstring const & content, xlsx_drawings_rels_ptr rels)
    {//todooo отсчеты номеров файлов отдельно
        const std::wstring id = boost::lexical_cast<std::wstring>(next_drawing_id_++);
        const std::wstring fileName = std::wstring(L"drawing") + id + L".xml";
       
		drawings_.push_back(drawing_elm(fileName, content, rels));
       
		const std::wstring rId = std::wstring(L"rId") + id;
        return std::pair<std::wstring, std::wstring>(fileName, rId);
    }

    std::pair<std::wstring, std::wstring> add_drawing_vml(std::wstring const & content, xlsx_drawings_rels_ptr rels)
    {
        const std::wstring drawing_id	= boost::lexical_cast<std::wstring>(next_drawing_id_++);
		const std::wstring file_vml_id	= boost::lexical_cast<std::wstring>(next_vml_file_id_++);
		const std::wstring fileName		= std::wstring(L"vmlDrawing") + file_vml_id + L".vml";
       
		drawings_vml_.push_back(drawing_elm(fileName, content, rels));
       
		const std::wstring rId = std::wstring(L"rId") + drawing_id;
        return std::pair<std::wstring, std::wstring>(fileName, rId);
    }
	
    const std::vector<drawing_elm> & content() const
    {
        return drawings_;
    }
	 const std::vector<drawing_elm> & content_vml() const
	 {
        return drawings_vml_;
	 }

	xlsx_drawings_rels_ptr get_drawings_rels();
private:
    external_items				& items_;
    size_t						next_rId_;
    
    std::vector<drawing_elm>	drawings_;
    std::vector<drawing_elm>	drawings_vml_;

    size_t						next_drawing_id_;
	size_t						& next_vml_file_id_;
};

xlsx_drawing_context_handle::xlsx_drawing_context_handle(size_t & next_vml_file_id, external_items & items)
									: impl_(new xlsx_drawing_context_handle::Impl(next_vml_file_id, items))
{
}

xlsx_drawing_context_handle::~xlsx_drawing_context_handle()
{
}

std::pair<std::wstring, std::wstring> xlsx_drawing_context_handle::add_drawing_xml(std::wstring const & content, xlsx_drawings_rels_ptr rels )
{
    return impl_->add_drawing_xml(content, rels);
}

std::pair<std::wstring, std::wstring> xlsx_drawing_context_handle::add_drawing_vml(std::wstring const & content, xlsx_drawings_rels_ptr rels )
{
    return impl_->add_drawing_vml(content, rels);
}

const std::vector<drawing_elm> & xlsx_drawing_context_handle::content() const
{
    return impl_->content();
}
const std::vector<drawing_elm> & xlsx_drawing_context_handle::content_vml() const
{
    return impl_->content_vml();
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
		if (drawing_state->line.fill.type != fillNone)	color = drawing_state->line.fill.color;
		else											color = drawing_state->fill.color;
	}break;
	case 0xF2:	color = drawing_state->line.fill.color;		break;
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

	//if (color.sRGB.empty())
	//{неверно
	//	color.nRGB = shemeDefaultColor[nColorIndex];
	//	color.index = -1;
	//	color.sRGB = STR::toRGB(color.nRGB);
	//}
	return color;
}

xlsx_drawing_context::xlsx_drawing_context(xlsx_conversion_context & Context) : context_(Context), handle_(Context.get_drawing_context_handle())
	, rels_				(xlsx_drawings_rels::create())
	, vml_HF_rels_		(xlsx_drawings_rels::create())
	, vml_comments_rels_(xlsx_drawings_rels::create())
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
	case 0x0012: // List
	case 0x0013: // Group box
		start_shape(0x0002); return true;
	case 0x0011: // Scrollbar
	case 0x0014: // Dropdown list
		break;
	case 0x0019: // Note
		start_comment(); return true;
		break;
	}
	return false;

}

external_items::Type xlsx_drawing_context::getType()
{
	if (current_drawing_states == NULL)		return external_items::typeUnknown;
	if (current_drawing_states->empty()) return external_items::typeUnknown;

	return current_drawing_states->back()->type;
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

void xlsx_drawing_context::start_comment()
{
	if (current_drawing_states == NULL) return;

	current_drawing_states->push_back(create_drawing_state());

	current_drawing_states->back()->type			= external_items::typeComment;
	//current_drawing_states->back()->vmlwrite_mode_	= true; это только для HF !!!
	
	count_object++;

	context_.get_comments_context().start_comment();
}


void xlsx_drawing_context::start_group()
{
	_drawing_state_ptr newState = create_drawing_state();
	
	current_drawing_states->push_back(newState);
	current_level++;

	current_drawing_states->back()->parent_drawing_states = current_drawing_states;
	current_drawing_states = &newState->drawing_states;
	
	current_drawing_states->push_back(newState);

	current_drawing_states->back()->type = external_items::typeGroup;
}
void xlsx_drawing_context::end_group()
{
	if (current_level < 1) return;
	
	serialize_group();
	current_drawing_states = current_drawing_states->front()->parent_drawing_states;

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

	switch(type)
	{
		case 0x0001: current_drawing_states->back()->shape_id = MSOSPT::msosptLine;			break;
		case 0x0002: current_drawing_states->back()->shape_id = MSOSPT::msosptRectangle;	break;
		case 0x0003: current_drawing_states->back()->shape_id = MSOSPT::msosptEllipse;		break;
		case 0x0004: current_drawing_states->back()->shape_id = MSOSPT::msosptArc;			break;
		case 0x0006: current_drawing_states->back()->shape_id = MSOSPT::msosptTextBox;		break;
		case 0x001E: current_drawing_states->back()->shape_id = MSOSPT::msosptRectangle;	break;// OfficeArt object
		case 0x0009: current_drawing_states->back()->shape_id = MSOSPT::msosptNotPrimitive;	break;// Polygon:
	}

	count_object++;
}

void xlsx_drawing_context::set_id(int id)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;
	
	current_drawing_states->back()->id = id;
}
void xlsx_drawing_context::set_FlipH()
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;
	
	current_drawing_states->back()->flipH = true;
}
void xlsx_drawing_context::set_FlipV()
{
	if (current_drawing_states == NULL)		return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->flipV = true;
}
void xlsx_drawing_context::set_shape_id(int id)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;
	
	if (current_drawing_states->back()->bTextBox)	return;

	current_drawing_states->back()->shape_id = (MSOSPT)id;
}
void xlsx_drawing_context::set_object_visible	(bool val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.visible = val;
}
void xlsx_drawing_context::set_object_anchor	(int col, int row)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.col = col;
	current_drawing_states->back()->object.row = row;

}
void xlsx_drawing_context::set_object_id(int val)
{
}


void xlsx_drawing_context::end_drawing()
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	if (current_drawing_states->back()->type == external_items::typeGroup)  return;
	
	end_drawing(current_drawing_states->back());

	if ( current_drawing_states->back()->type == external_items::typeComment )
	{
		drawing_states_vml_comments.push_back(current_drawing_states->back());
		current_drawing_states->pop_back();
	}
	else 
		if (current_drawing_states->back()->vmlwrite_mode_)
		{
			drawing_states_vml_HF.push_back(current_drawing_states->back());
			current_drawing_states->pop_back();
		}
}

void xlsx_drawing_context::end_drawing(_drawing_state_ptr & drawing_state)
{
	if (!drawing_state) return;

	if (drawing_state->id < 0)	
		drawing_state->id = count_object + 0x20000;

	if ( drawing_state->type	== external_items::typeImage ||
		drawing_state->shape_id	== msosptPictureFrame )
	{
		drawing_state->type = external_items::typeImage;

		if (!drawing_state->fill.texture_target.empty())
		{
			bool isIternal = false;
			std::wstring rId = handle_.impl_->get_mediaitems().find_image( drawing_state->fill.texture_target, isIternal);
			
			serialize_pic(drawing_state, rId);
		
			if (drawing_state->vmlwrite_mode_)
				vml_HF_rels_->add(isIternal, rId , drawing_state->fill.texture_target, drawing_state->type);
			else
				rels_->add(isIternal, rId , drawing_state->fill.texture_target, drawing_state->type);
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
		
		rels_->add( isIternal, 	rId, target, drawing_state->type);
	}
	if ( drawing_state->type == external_items::typeComment )
	{
		context_.get_comments_context().set_content(drawing_state->text.content);
		
		serialize_shape_comment(drawing_state);
	
		//context_.get_comments_context().set_shape_drawing(drawing_state->shape); 
		context_.get_comments_context().end_comment();
	}
	if ( drawing_state->type == external_items::typeShape)
	{
		serialize_shape(drawing_state);
	}
}

void xlsx_drawing_context::serialize_group()
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	_drawing_state_ptr & drawing_state = current_drawing_states->front();
	std::wstringstream strm;

	CP_XML_WRITER(strm)    
	{
		CP_XML_NODE(L"xdr:grpSp")
		{ 
			CP_XML_NODE(L"xdr:nvGrpSpPr")
			{
				CP_XML_NODE(L"xdr:cNvPr")
				{
					CP_XML_ATTR(L"id", drawing_state->id);
					
					if (drawing_state->name.empty())
					{
						drawing_state->name = L"Group_" + boost::lexical_cast<std::wstring>(count_object);
					}
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
				CP_XML_NODE(L"xdr:cNvGrpSpPr")
				{
					CP_XML_NODE(L"a:grpSpLocks")
					{
						CP_XML_ATTR(L"noChangeAspect", 1);
					}
				}
			}
			
			CP_XML_NODE(L"xdr:grpSpPr")
			{
				serialize_xfrm(CP_XML_STREAM(), drawing_state);
				//serialize_fill(CP_XML_STREAM(), drawing_state);
				//serialize_line(CP_XML_STREAM(), drawing_state);		
			}

			for (int i = 1; i < current_drawing_states->size(); i++)
			{
				CP_XML_STREAM() << current_drawing_states->at(i)->shape;
				//todooo current_drawing_states->at(i).shape.erase(); // память поэкономить
			}
		}
	}

	drawing_state->shape = strm.str();
}
void xlsx_drawing_context::serialize_shape_comment(_drawing_state_ptr & drawing_state)
{
	std::wstringstream strm;
	
	CP_XML_WRITER(strm)    
	{		
		std::wstringstream strmStyle;

		strmStyle << L"position:absolute;";	
		strmStyle << L"margin-left:" << boost::lexical_cast<std::wstring>(drawing_state->child_anchor.x / 12700.)	<< L"pt;";	//in pt (1 pt = 12700 emu)
		strmStyle << L"margin-top:"	<< boost::lexical_cast<std::wstring>(drawing_state->child_anchor.y / 12700.)	<< L"pt;";
		strmStyle << L"width:"		<< boost::lexical_cast<std::wstring>(drawing_state->child_anchor.cx / 12700.)	<< L"pt;";
		strmStyle << L"height:"		<< boost::lexical_cast<std::wstring>(drawing_state->child_anchor.cy / 12700.)	<< L"pt;";
		strmStyle << L"z-index:"		<< boost::lexical_cast<std::wstring>(drawing_state->id)	 << L";";
		
		if (drawing_state->object.visible == false) 
			strmStyle << L"visibility:hidden;";	

		CP_XML_NODE(L"v:shape")
		{
			//CP_XML_ATTR(L"id"			, boost::lexical_cast<std::wstring>(drawing_state->object.id));
			CP_XML_ATTR(L"type"			, L"_x0000_t202");
			CP_XML_ATTR(L"fillcolor"	, L"#" + drawing_state->fill.color.sRGB);

			if (drawing_state->line.width > 0)
			{
				CP_XML_ATTR(L"strokeweight",  boost::lexical_cast<std::wstring>(drawing_state->line.width) +L"pt");
			}
			CP_XML_ATTR(L"style", strmStyle.str());

			CP_XML_NODE(L"v:shadow")
			{
				CP_XML_ATTR(L"color", L"black");
				CP_XML_ATTR(L"obscured", L"t");
				CP_XML_ATTR(L"on", L"t");
			}

			CP_XML_NODE(L"v:wrap")
			{
				CP_XML_ATTR(L"v:type", L"none");
			}
		
			CP_XML_NODE(L"v:fill")
			{
				CP_XML_ATTR(L"color", std::wstring(L"#") + drawing_state->fill.color.sRGB);
				if (drawing_state->fill.opacity > 0.00001)
				{
					CP_XML_ATTR(L"opacity", drawing_state->fill.opacity * 65536);
				}			
				bool  isIternal = false;
				std::wstring rId = handle_.impl_->get_mediaitems().find_image( drawing_state->fill.texture_target, isIternal);
				if (!rId.empty())
				{	
					CP_XML_ATTR(L"o:relid"	, rId);
					CP_XML_ATTR(L"type"		, L"tile");

					vml_comments_rels_->add(isIternal, rId , drawing_state->fill.texture_target, external_items::typeImage);
				}
				else if (drawing_state->fill.type == fillGradient || drawing_state->fill.type == fillGradientOne)
				{
					CP_XML_ATTR(L"color2", std::wstring(L"#") + drawing_state->fill.color2.sRGB);
					if (drawing_state->fill.opacity2 > 0.00001)
					{
						CP_XML_ATTR(L"opacity2", drawing_state->fill.opacity2 * 65536);
					}
					CP_XML_ATTR(L"type", L"gradient");
				}
				else
				{
					CP_XML_ATTR(L"type", L"solid");
				}
			}

			CP_XML_NODE(L"v:stroke")
			{
				CP_XML_ATTR(L"color", std::wstring(L"#") + drawing_state->line.fill.color.sRGB);
				//CP_XML_ATTR(L"opacity",(100.-dVal.get())/100.);

				switch(drawing_state->line.typeDash)
				{	
				case lineDash:		CP_XML_ATTR(L"dashstyle", L"dash");			break;
				case lineDot:		CP_XML_ATTR(L"dashstyle", L"dot");			break;
				case lineDashDot:	CP_XML_ATTR(L"dashstyle", L"dashDot");		break;
				case lineDashDotDot:CP_XML_ATTR(L"dashstyle", L"lgDashDotDot"); break;
				}
			}	
			CP_XML_NODE(L"x:ClientData")
			{
				CP_XML_ATTR(L"ObjectType", L"Note");
				CP_XML_NODE(L"x:MoveWithCells"){}
				CP_XML_NODE(L"x:SizeWithCells"){}

				if (drawing_state->sheet_anchor.xFrom >= 0 && drawing_state->sheet_anchor.yFrom > 0)
				{//old xls don't have this
					std::wstringstream strmAnchor;
					strmAnchor	
							<< drawing_state->sheet_anchor.colFrom	<< L", " << (int)(2 * drawing_state->sheet_anchor.xFrom / 12700.)	<< L", " 
							<< drawing_state->sheet_anchor.rwFrom	<< L", " << (int)(2 * drawing_state->sheet_anchor.yFrom / 12700.)	<< L", " 
							<< drawing_state->sheet_anchor.colTo	<< L", " << (int)(2 * drawing_state->sheet_anchor.xTo / 12700.)		<< L", " 
							<< drawing_state->sheet_anchor.rwTo		<< L", " << (int)(2 * drawing_state->sheet_anchor.yTo / 12700.); 
					if (!strmAnchor.str().empty())
					{
						CP_XML_NODE(L"x:Anchor"){CP_XML_CONTENT(strmAnchor.str());}
					}
				}
				CP_XML_NODE(L"x:AutoFill")	{CP_XML_CONTENT("False");}
				CP_XML_NODE(L"x:Row")		{CP_XML_CONTENT(drawing_state->object.row);}
				CP_XML_NODE(L"x:Column")	{CP_XML_CONTENT(drawing_state->object.col);}
				
				if (drawing_state->object.visible) CP_XML_NODE(L"x:Visible");
			}
		}	
	
	}
	drawing_state->shape = strm.str();
}
void xlsx_drawing_context::serialize_vml_shape(_drawing_state_ptr & drawing_state)
{
}
void xlsx_drawing_context::serialize_vml_pic(_drawing_state_ptr & drawing_state, std::wstring rId)
{
	std::wstringstream strm;

	CP_XML_WRITER(strm)    
	{
		CP_XML_NODE(L"v:shape")
		{ 
			CP_XML_ATTR( L"id"				, drawing_state->name);
			CP_XML_ATTR( L"o:spt"			, L"75" );
			CP_XML_ATTR( L"o:preferrelative", "t" );
			CP_XML_ATTR( L"filled"			, "f" );
			CP_XML_ATTR( L"stroked"			, "f" );
			CP_XML_ATTR( L"type"			, L"#_x0000_t75" );
			CP_XML_ATTR( L"coordsize"		, L"21600,21600" );

			std::wstring style = std::wstring(L"position:absolute;margin-left:0;margin-top:0;");
			
			//todooo сделать "покороче" значения .. достаточно 2 знаков после запятой
			style += std::wstring(L"width:")	+ boost::lexical_cast<std::wstring>(drawing_state->child_anchor.cx)	+ std::wstring(L"pt;");
			style += std::wstring(L"height:")	+ boost::lexical_cast<std::wstring>(drawing_state->child_anchor.cy)	+ std::wstring(L"pt;");
			style += std::wstring(L"z-index:")	+ boost::lexical_cast<std::wstring>(drawing_state->id) + std::wstring(L";");

			CP_XML_ATTR(L"style",style);

			CP_XML_NODE(L"v:imagedata")
			{
				CP_XML_ATTR(L"o:relid", rId);
			}
		}
	}
	drawing_state->shape = strm.str();
}

void xlsx_drawing_context::serialize_pic(_drawing_state_ptr & drawing_state, std::wstring rId)
{
	if (drawing_state->vmlwrite_mode_) 
		return serialize_vml_pic(drawing_state, rId);

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
			drawing_state->fill.texture_mode = textureStretch;
			serialize_bitmap_fill(CP_XML_STREAM(), drawing_state->fill, rId, L"xdr:");

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
	if (drawing_state->vmlwrite_mode_) 
		return serialize_vml_shape(drawing_state);

	std::wstringstream strm;

	std::wstring prstTxWarp;
	std::wstring prstGeom	= Spt2ShapeType(drawing_state->shape_id);
	std::wstring customGeom;

	if (prstGeom.empty())
	{
		prstTxWarp = Spt2WordArtShapeType(drawing_state->shape_id);
		if (prstTxWarp.empty() == false)
		{
			drawing_state->wordart.is = true;
			drawing_state->bTextBox = true;
			prstGeom = L"rect";
		}
		else
		{
			customGeom	= convert_custom_shape(drawing_state);
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
					{
						if (drawing_state->wordart.is)
							drawing_state->name = L"WordArt_" + boost::lexical_cast<std::wstring>(count_object);
						else
							drawing_state->name = L"Shape_" + boost::lexical_cast<std::wstring>(count_object);
					}
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
					CP_XML_NODE(L"a:spLocks")
					{
						CP_XML_ATTR(L"noGrp", 1); 
						CP_XML_ATTR(L"noChangeArrowheads", 1);
					}
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
						if (!drawing_state->wordart.is)	CP_XML_NODE(L"a:avLst");
					}
				}
				else
				{
					CP_XML_NODE(L"a:custGeom")
					{
						CP_XML_STREAM() << customGeom;
					}
				}
				if (!is_lined_shape(drawing_state))
				{
					serialize_fill(CP_XML_STREAM(), drawing_state);
				}
				serialize_line(CP_XML_STREAM(), drawing_state);		
			}
			serialize_text(CP_XML_STREAM(), drawing_state);
		}
	}

	drawing_state->shape = strm.str();
}

bool xlsx_drawing_context::is_lined_shape	(_drawing_state_ptr & drawing_state)
{
	if (drawing_state->shape_id == msosptStraightConnector1 ||
		drawing_state->shape_id == msosptLine				||
		drawing_state->shape_id == msosptBentConnector2		||
		drawing_state->shape_id == msosptBentConnector3		||
		drawing_state->shape_id == msosptBentConnector4		||
		drawing_state->shape_id == msosptBentConnector5		||
		drawing_state->shape_id == msosptCurvedConnector2	||
		drawing_state->shape_id == msosptCurvedConnector3	||
		drawing_state->shape_id == msosptCurvedConnector4	||
		drawing_state->shape_id == msosptCurvedConnector5	)return true;

	return false;
}

std::wstring xlsx_drawing_context::convert_custom_shape(_drawing_state_ptr & drawing_state)
{
	NSCustomShapesConvert::CCustomShape * shape = NSCustomShapesConvert::CCustomShape::CreateByType(drawing_state->shape_id);
	if (shape == NULL) return L"";

	std::wstring strResult;
	std::wstringstream strm;

	shape->m_oCustomVML.SetAdjusts(&shape->m_arAdjustments);
	
	shape->m_oCustomVML.m_bIsVerticesPresent = drawing_state->custom_verticles.empty() ? false : true;
	
	for (int i = 0 ; i < drawing_state->custom_verticles.size(); i++)
	{
		 Aggplus::POINT p;
		
		 p.x = drawing_state->custom_verticles[i].x;
		 p.y = drawing_state->custom_verticles[i].y;
		
		 shape->m_oCustomVML.m_arVertices.push_back(p);
	}
	
	for (int i = 0 ; i < drawing_state->custom_guides.size(); i++)
	{//todooo объеденить/срастить !!
		NSCustomShapesConvert::CGuide guid;
		
		guid.m_eType		= drawing_state->custom_guides[i].m_eType;
		guid.m_param_type1	= drawing_state->custom_guides[i].m_param_type1;
		guid.m_param_type2	= drawing_state->custom_guides[i].m_param_type2;
		guid.m_param_type3	= drawing_state->custom_guides[i].m_param_type3;
		guid.m_param_value1 = drawing_state->custom_guides[i].m_param_value1;
		guid.m_param_value2 = drawing_state->custom_guides[i].m_param_value2;
		guid.m_param_value3 = drawing_state->custom_guides[i].m_param_value3;
		
		shape->m_oCustomVML.addGuide(guid);
	}	
	
	for (int i = 0 ; i < drawing_state->custom_segments.size(); i++)
	{
		if (0 == drawing_state->custom_segments[i].m_nCount)
		{
			if ((NSCustomShapesConvert::rtEnd		!= drawing_state->custom_segments[i].m_eRuler) &&
				(NSCustomShapesConvert::rtNoFill	!= drawing_state->custom_segments[i].m_eRuler) &&
				(NSCustomShapesConvert::rtNoStroke	!= drawing_state->custom_segments[i].m_eRuler) &&
				(NSCustomShapesConvert::rtClose		!= drawing_state->custom_segments[i].m_eRuler))
			{
				continue;
			}
		}
		shape->m_oCustomVML.addSegment(drawing_state->custom_segments[i].m_eRuler , drawing_state->custom_segments[i].m_nCount);
	}	
	//for (int i = 0; i < drawing_state->custom_adjustHandles.size(); i++)
	//{//todooo - ранее этого не было ?????
	//	shape->m_oCustomVML.addHandle(i, *drawing_state->custom_adjustHandles[i]);
	//}
	for (int i = 0; i < drawing_state->custom_adjustValues.size(); i++)
	{
		if (drawing_state->custom_adjustValues[i])
		{
			shape->m_oCustomVML.addAdjust(i, *drawing_state->custom_adjustValues[i]);
		}
	}
	if (drawing_state->custom_path >=0)
		shape->m_oCustomVML.SetPath((NSCustomShapesConvert::RulesType)drawing_state->custom_path);
	
	shape->m_oCustomVML.ToCustomShape(shape, shape->m_oManager);
	shape->ReCalculate();
//-------------------------------------------------------------------------------------
	if (drawing_state->custom_rect.cx > 0 && drawing_state->custom_rect.cy > 0)
	{
		shape->m_oPath.SetCoordsize(drawing_state->custom_rect.cx, drawing_state->custom_rect.cy);
	}

	NSCustomShapesConvert::CFormParam pParamCoef;
	pParamCoef.m_eType	= NSCustomShapesConvert::ptValue;
	pParamCoef.m_lParam = 65536;
	pParamCoef.m_lCoef	= 65536;
	
	NSCustomShapesConvert::CFormulaConverter pFormulaConverter;

	//coeff
	pFormulaConverter.ConvertCoef(pParamCoef);

	//guids----------------------------------------
	int nGuidCount = shape->m_oManager.m_arFormulas.size();
	if (0 != nGuidCount)
	{
		pFormulaConverter.ConvertFormula(shape->m_oManager.m_arFormulas);
	}				

	//path------------------------------------------
	int nPathCount = shape->m_strPath.length();
	
	if (0 != nPathCount && shape->m_eType != 1)
	{
		pFormulaConverter.ConvertPath(shape->m_strPath, shape->m_oPath);

		//string rect
		int nRectCount = (int)shape->m_arStringTextRects.size();
		if (0 != nRectCount)
		{
			pFormulaConverter.ConvertTextRect(shape->m_arStringTextRects[0]);
		}

		int nHandlesCount = shape->m_arHandles.size();
		int nAdjCount = shape->m_arAdjustments.size();

		//handles
		if (0 != nHandlesCount || 0 != nAdjCount)
		{
			pFormulaConverter.ConvertHandle(shape->m_arHandles, shape->m_arAdjustments, shape->m_eType);
		}
		CP_XML_WRITER(strm)    
		{
			CP_XML_NODE(L"a:avLst")
			{
				CP_XML_STREAM() << pFormulaConverter.m_oAdjRes.GetXmlString();
			}
			CP_XML_NODE(L"a:gdLst")
			{
				CP_XML_STREAM() << pFormulaConverter.m_oCoef.GetXmlString();
				CP_XML_STREAM() << pFormulaConverter.m_oGuidsRes.GetXmlString();
			}
			CP_XML_NODE(L"a:ahLst")
			{
				CP_XML_STREAM() << pFormulaConverter.m_oHandleRes.GetXmlString();
			}
				
			//connectors-------------------------
			CP_XML_NODE(L"a:cxnLst");
			
			////textRect---------------------------
			//if (pFormulaConverter.m_oTextRect.GetSize() != 0)
			strm << pFormulaConverter.m_oTextRect.GetXmlString();

			CP_XML_NODE(L"a:pathLst")
			{
				CP_XML_STREAM() << pFormulaConverter.m_oPathRes.GetXmlString();
			}	
		}
		strResult = strm.str();
	}

	delete shape;

	return strResult;
}

void xlsx_drawing_context::reset_fill_pattern (_drawing_state_ptr & drawing_state)
{
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
}

void xlsx_drawing_context::serialize_fill(std::wostream & stream, _drawing_state::_fill & fill)
{
	if (fill.type == fillNone)
	{
		serialize_none_fill(stream);
		return;
	}
	if (!fill.texture_target.empty() && fill.type == fillTexture)
	{
		bool  isIternal = false;
		std::wstring rId = handle_.impl_->get_mediaitems().find_image( fill.texture_target, isIternal);
		if (!rId.empty())
		{
			if (in_chart_ == true)
			{
				context_.current_chart().add_rels( isIternal, rId , fill.texture_target, external_items::typeImage);
			}
			else 
			{
				rels_->add(isIternal, rId , fill.texture_target, external_items::typeImage);
			}
			serialize_bitmap_fill(stream, fill, rId);
			return;
		}
	}
	if (fill.type == fillGradient || fill.type == fillGradientOne)
	{
		serialize_gradient_fill(stream, fill);
		return;
	}
//------------ 
	if (fill.color.index >= 0 || !fill.color.sRGB.empty())
	{
		serialize_solid_fill(stream, fill.color, fill.opacity);
	}
	else serialize_none_fill(stream);
}
void xlsx_drawing_context::serialize_fill(std::wostream & stream)
{
	serialize_fill(stream, current_drawing_states->back());
}
void xlsx_drawing_context::serialize_fill(std::wostream & stream, _drawing_state_ptr & drawing_state)
{
	if (drawing_state->fill.type == fillPattern) 
		reset_fill_pattern(drawing_state);

	if (drawing_state->wordart.is)
		serialize_none_fill(stream);
	else
		serialize_fill(stream, drawing_state->fill);
}
void xlsx_drawing_context::serialize_line(std::wostream & stream, _drawing_state_ptr & drawing_state)
{
	if (drawing_state->wordart.is)					return;
	
	serialize_line(stream, drawing_state->line);
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

void xlsx_drawing_context::serialize_gradient_fill(std::wostream & stream, _drawing_state::_fill & fill)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"a:gradFill")
		{
			CP_XML_ATTR(L"rotWithShape", 0);

			CP_XML_NODE(L"a:gsLst")
			{
				if (fill.colorsPosition.empty() && fill.focus != 0)
				{
					if (fill.focus > 0)
					{
						fill.colorsPosition.push_back(std::pair<double, _color>(0., fill.color));
						fill.colorsPosition.push_back(std::pair<double, _color>(fill.focus/100., fill.color2));
						fill.colorsPosition.push_back(std::pair<double, _color>(1., fill.color));
					}
					else if (fill.focus < 0)
					{
						fill.colorsPosition.push_back(std::pair<double, _color>(0., fill.color2));
						fill.colorsPosition.push_back(std::pair<double, _color>(-fill.focus/100., fill.color));
						fill.colorsPosition.push_back(std::pair<double, _color>(1., fill.color2));
					}
				}

				if ( !fill.colorsPosition.empty() )
				{
					for (int i = 0; i < fill.colorsPosition.size(); i++)
					{
						CP_XML_NODE(L"a:gs")
						{
							CP_XML_ATTR(L"pos",  (int)(fill.colorsPosition[i].first * 100000));
							serialize_color(CP_XML_STREAM(), fill.colorsPosition[i].second);
							//проверить что если тут индексы то они берутся с программных а не с юзерских (см как ниже)
						}
					}
				}
				else
				{
					CP_XML_NODE(L"a:gs")
					{
						fill.color.bScheme = false; // по общим индексам
						CP_XML_ATTR(L"pos", 0);
						serialize_color(CP_XML_STREAM(), fill.color, fill.opacity);
					}
					CP_XML_NODE(L"a:gs")
					{
						fill.color2.bScheme = false; // по общим индексам
						CP_XML_ATTR(L"pos", 100000);
						serialize_color(CP_XML_STREAM(), fill.color2, fill.opacity2);
					}
				}
			}
			CP_XML_NODE(L"a:lin")
			{
				int val = 90 - fill.angle;
				if (val < 0) val = 0;
				CP_XML_ATTR(L"ang", val * 60000);
				CP_XML_ATTR(L"scaled", 1);
			}
		}
	}
}

void xlsx_drawing_context::serialize_anchor (std::wostream & stream, _drawing_state_ptr & drawing_state)
{
	CP_XML_WRITER(stream)    
	{
		if (drawing_state->type_anchor == 1)
		{
			CP_XML_NODE(L"xdr:from")
			{ 
				CP_XML_NODE(L"xdr:col")		{ CP_XML_CONTENT (drawing_state->sheet_anchor.colFrom);	}
				CP_XML_NODE(L"xdr:colOff")	{ CP_XML_CONTENT (drawing_state->sheet_anchor.xFrom) ;	}
				CP_XML_NODE(L"xdr:row")		{ CP_XML_CONTENT (drawing_state->sheet_anchor.rwFrom);	}
				CP_XML_NODE(L"xdr:rowOff")	{ CP_XML_CONTENT (drawing_state->sheet_anchor.yFrom) ;	}
			}
			CP_XML_NODE(L"xdr:to")
			{  		
				CP_XML_NODE(L"xdr:col")		{ CP_XML_CONTENT (drawing_state->sheet_anchor.colTo);	}
				CP_XML_NODE(L"xdr:colOff")	{ CP_XML_CONTENT (drawing_state->sheet_anchor.xTo);	}
				CP_XML_NODE(L"xdr:row")		{ CP_XML_CONTENT (drawing_state->sheet_anchor.rwTo);	}
				CP_XML_NODE(L"xdr:rowOff")	{ CP_XML_CONTENT (drawing_state->sheet_anchor.yTo);	}
			}
		}
		if (drawing_state->type_anchor == 3)
		{
			CP_XML_NODE(L"xdr:pos")//in emu (1 pt = 12700)
			{ 
				CP_XML_ATTR(L"x", (int)(drawing_state->absolute_anchor.x * 12700));
				CP_XML_ATTR(L"y", (int)(drawing_state->absolute_anchor.y * 12700));
			}
			CP_XML_NODE(L"xdr:ext") //in emu (1 pt = 12700)
			{ 
				CP_XML_ATTR(L"cx", (int)(drawing_state->absolute_anchor.cx * 12700));
				CP_XML_ATTR(L"cy", (int)(drawing_state->absolute_anchor.cy * 12700)); 
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
				CP_XML_ATTR(L"x", drawing_state->child_anchor.x);
				CP_XML_ATTR(L"y", drawing_state->child_anchor.y);
			}
			CP_XML_NODE(L"a:ext")
			{
				CP_XML_ATTR(L"cx", drawing_state->child_anchor.cx);
				CP_XML_ATTR(L"cy", drawing_state->child_anchor.cy);
			}

			if (drawing_state->type == external_items::typeGroup)
			{
				CP_XML_NODE(L"a:chOff")
				{
					CP_XML_ATTR(L"x", drawing_state->group_anchor.x);
					CP_XML_ATTR(L"y", drawing_state->group_anchor.y);
				}
				CP_XML_NODE(L"a:chExt")
				{
					CP_XML_ATTR(L"cx", drawing_state->group_anchor.cx);
					CP_XML_ATTR(L"cy", drawing_state->group_anchor.cy);
				}				
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
	if (drawing_state->text.content.empty() && drawing_state->wordart.text.empty()) return;

	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"xdr:txBody")
		{  
			CP_XML_NODE(L"a:bodyPr")//todooo rtl
			{
				if (drawing_state->text.wrap == 2 || drawing_state->wordart.is)
					CP_XML_ATTR(L"wrap", L"none" ); 

				if (drawing_state->wordart.is)
				{
					CP_XML_ATTR(L"fromWordArt", 1);					

					if (drawing_state->wordart.vertical)
						CP_XML_ATTR(L"vert", L"wordArtVert"); 
					
					std::wstring prstTxWarp = Spt2WordArtShapeType(drawing_state->shape_id);				
					CP_XML_NODE(L"a:prstTxWarp")
					{
						CP_XML_ATTR(L"prst", prstTxWarp);
						CP_XML_NODE(L"a:avLst");//модификаторы
					}
				}
				else
				{
					CP_XML_ATTR(L"lIns", drawing_state->text.margins.left);	
					CP_XML_ATTR(L"tIns", drawing_state->text.margins.top);	
					CP_XML_ATTR(L"rIns", drawing_state->text.margins.right);
					CP_XML_ATTR(L"bIns", drawing_state->text.margins.bottom);

					switch (drawing_state->text.vert_align)
					{
						case 1: CP_XML_ATTR(L"anchor", L"t");	break;
						case 2: CP_XML_ATTR(L"anchor", L"ctr"); break;
						case 3: CP_XML_ATTR(L"anchor", L"b");	break;
					}
					switch(drawing_state->text.vertical)
					{
						case 1: 
						case 3: CP_XML_ATTR(L"vert", L"vert");			break;
						case 2: CP_XML_ATTR(L"vert", L"vert270");		break;
						case 5: CP_XML_ATTR(L"vert", L"wordArtVert");	break;
					}
				}
				//text margins

			}
			CP_XML_NODE(L"a:lstStyle");		
			
			CP_XML_NODE(L"a:p")
			{
				CP_XML_NODE(L"a:pPr")
				{
					switch(drawing_state->text.align)
					{
						case 1: CP_XML_ATTR(L"algn", L"l");		break;
						case 2: CP_XML_ATTR(L"algn", L"ctr"); 	break;
						case 3: CP_XML_ATTR(L"algn", L"r"); 	break;
						case 4: CP_XML_ATTR(L"algn", L"just"); 	break;
						case 5: CP_XML_ATTR(L"algn", L"dist"); 	break;
					}
				}
				if (drawing_state->wordart.is)
				{		
					CP_XML_NODE(L"a:r")
					{
						CP_XML_NODE(L"a:rPr")
						{	
							if (drawing_state->wordart.size < 1) drawing_state->wordart.size = 36;
							CP_XML_ATTR(L"sz", (int)(drawing_state->wordart.size * 100)); 
							
							if (drawing_state->wordart.bold)		CP_XML_ATTR(L"b", 1);
							if (drawing_state->wordart.italic)		CP_XML_ATTR(L"i", 1);
							if (drawing_state->wordart.underline)	CP_XML_ATTR(L"u", L"sng");
							if (drawing_state->wordart.strike)		CP_XML_ATTR(L"strike", L"sngStrike");
						
							CP_XML_ATTR(L"kern", 10);
							
							if (drawing_state->wordart.spacing > 1) 
								CP_XML_ATTR(L"spc", (int) ((drawing_state->wordart.spacing - 1 ) * 3600.));
							else if (drawing_state->wordart.spacing < 1) 
								CP_XML_ATTR(L"spc", (int) ((drawing_state->wordart.spacing - 1 ) * 1800.));
							else
								CP_XML_ATTR(L"spc", 0);
						
							serialize_line(CP_XML_STREAM(), drawing_state->line);
							serialize_fill(CP_XML_STREAM(), drawing_state->fill);
							if (!drawing_state->wordart.font.empty())
							{
								CP_XML_NODE(L"a:latin")	{CP_XML_ATTR(L"typeface", drawing_state->wordart.font);}
								CP_XML_NODE(L"a:cs")	{CP_XML_ATTR(L"typeface", drawing_state->wordart.font);}
							}			
						}
						CP_XML_NODE(L"a:t")
						{		
							CP_XML_STREAM() << xml::utils::replace_text_to_xml(drawing_state->wordart.text);
						}
					}
				}
				else
				{
					CP_XML_STREAM() << drawing_state->text.content;
				}
			}
		}
	}
}

void xlsx_drawing_context::serialize_line(std::wostream & stream, _drawing_state::_line & line)
{
	if (line.fill.type == fillNone)	return;
	
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"a:ln")
		{
			if (line.width > 0)
				CP_XML_ATTR(L"w", line.width);//in emu (1 pt = 12700) ??? 
			else
				CP_XML_ATTR(L"w", 9525); //default

			serialize_fill(CP_XML_STREAM(), line.fill);

			CP_XML_NODE(L"a:prstDash")
			{
				switch(line.typeDash)
				{	
				case lineSolid:		CP_XML_ATTR(L"val", L"solid");		break;
				case lineDash:		CP_XML_ATTR(L"val", L"dash");		break;
				case lineDot:		CP_XML_ATTR(L"val", L"dot");		break;
				case lineDashDot:	CP_XML_ATTR(L"val", L"dashDot");	break;
				case lineDashDotDot:CP_XML_ATTR(L"val", L"lgDashDotDot"); break;
				}
			}
			if (line.arrow.enabled)
			{
				serialize_arrow(CP_XML_STREAM(), L"a:headEnd", line.arrow.start, line.arrow.start_width, line.arrow.start_length);

				serialize_arrow(CP_XML_STREAM(), L"a:tailEnd", line.arrow.end, line.arrow.end_width, line.arrow.end_length);
			}
		}
	}
}
void xlsx_drawing_context::serialize_arrow(std::wostream & stream, std::wstring name, int type, int width, int length)
{
	if (type < 1) return;

	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(name)
		{
			switch(type)
			{
				case 1:	CP_XML_ATTR(L"type", L"triangle");	break;
				case 2:	CP_XML_ATTR(L"type", L"stealth");	break;
				case 3:	CP_XML_ATTR(L"type", L"diamond");	break;
				case 4:	CP_XML_ATTR(L"type", L"oval");		break;
				case 5:	CP_XML_ATTR(L"type", L"arrow");		break;
			}
			switch(width)
			{
				case 1:	CP_XML_ATTR(L"w", L"lg");	break;
				case 2:	CP_XML_ATTR(L"w", L"med");	break;
				case 3:	CP_XML_ATTR(L"w", L"sm");	break;
			}
			switch(length)
			{
				case 1:	CP_XML_ATTR(L"len", L"lg");	break;
				case 2:	CP_XML_ATTR(L"len", L"med");break;
				case 3:	CP_XML_ATTR(L"len", L"sm");	break;
			}			
		}
	}
}
void xlsx_drawing_context::serialize_bitmap_fill(std::wostream & stream, _drawing_state::_fill & fill, std::wstring rId, const std::wstring ns)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(ns + L"blipFill")
		{
			CP_XML_NODE(L"a:blip")
			{
				CP_XML_ATTR(L"r:embed", rId);
			}

			CP_XML_NODE(L"a:srcRect")
			{
				if (fill.texture_crop_enabled)
				{		
					CP_XML_ATTR(L"l", static_cast<size_t>(fill.texture_crop[0]));
					CP_XML_ATTR(L"t", static_cast<size_t>(fill.texture_crop[1]));
					CP_XML_ATTR(L"r", static_cast<size_t>(fill.texture_crop[2]));
					CP_XML_ATTR(L"b", static_cast<size_t>(fill.texture_crop[3]));
				}
			}
			if (fill.texture_mode == textureStretch)
			{
				CP_XML_NODE(L"a:stretch")
				{
					CP_XML_NODE(L"a:fillRect");
				}
			}
			if (fill.texture_mode == textureTill)
			{
				CP_XML_NODE(L"a:tile");
			}
		}
	}
}

void xlsx_drawing_context::serialize_vml(std::wostream & stream, _drawing_state_ptr & drawing_state)
{
	stream << drawing_state->shape;
}

void xlsx_drawing_context::serialize(std::wostream & stream, _drawing_state_ptr & drawing_state)
{
	if		(drawing_state->type_anchor == 0) return;

	std::wstring sNodeAnchor;

	if		(drawing_state->type_anchor == 1)	sNodeAnchor = L"xdr:twoCellAnchor";
	else if (drawing_state->type_anchor == 2)	sNodeAnchor = L"xdr:oneCellAnchor";
	else										sNodeAnchor = L"xdr:absoluteAnchor";

	if (sNodeAnchor.empty()) return;
	
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(sNodeAnchor)
		{ 
			//if (drawing_state->anchor.type == 1) CP_XML_ATTR(L"editAs", L"oneCell");

			serialize_anchor(CP_XML_STREAM(), drawing_state);

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
void xlsx_drawing_context::set_sheet_anchor(int colFrom, int xFrom, int rwFrom, int yFrom, int colTo, int xTo, int rwTo,int yTo)
{
	if (current_drawing_states == NULL) return;	
	
	current_drawing_states->back()->sheet_anchor.colFrom	= colFrom;
	current_drawing_states->back()->sheet_anchor.colTo		= colTo;
	current_drawing_states->back()->sheet_anchor.rwFrom		= rwFrom;
	current_drawing_states->back()->sheet_anchor.rwTo		= rwTo;
	current_drawing_states->back()->sheet_anchor.xFrom		= xFrom;
	current_drawing_states->back()->sheet_anchor.yFrom		= yFrom;
	current_drawing_states->back()->sheet_anchor.xTo		= xTo;
	current_drawing_states->back()->sheet_anchor.yTo		= yTo;

	current_drawing_states->back()->type_anchor	= 1;
}
void xlsx_drawing_context::set_child_anchor(int x, int y, int cx, int cy)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->child_anchor.x	= x;
	current_drawing_states->back()->child_anchor.y	= y;
	current_drawing_states->back()->child_anchor.cx	= cx;
	current_drawing_states->back()->child_anchor.cy	= cy;	
}
void xlsx_drawing_context::set_group_anchor(int x, int y, int cx, int cy)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->group_anchor.x	= x;
	current_drawing_states->back()->group_anchor.y	= y;
	current_drawing_states->back()->group_anchor.cx	= cx;
	current_drawing_states->back()->group_anchor.cy	= cy;	
}
void xlsx_drawing_context::set_absolute_anchor(double x, double y, double cx, double cy)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	if (current_drawing_states->back()->type_anchor != 0) return;	

	current_drawing_states->back()->absolute_anchor.x	= x;
	current_drawing_states->back()->absolute_anchor.y	= y;
	current_drawing_states->back()->absolute_anchor.cx	= cx;
	current_drawing_states->back()->absolute_anchor.cy	= cy;

	current_drawing_states->back()->type_anchor = 3;
}

void xlsx_drawing_context::set_fill_texture_mode(int val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;
		
	current_drawing_states->back()->fill.texture_mode = (_texture_mode)val;
}
void xlsx_drawing_context::set_fill_texture(const std::wstring & str)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;
	
	if (str.empty()) return;

	current_drawing_states->back()->fill.texture_target = str;
}

void xlsx_drawing_context::set_crop_top	(double val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;
	
	current_drawing_states->back()->fill.texture_crop[1] = val;
    current_drawing_states->back()->fill.texture_crop_enabled = true;
}
void xlsx_drawing_context::set_crop_bottom(double val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->fill.texture_crop[3] = val;
    current_drawing_states->back()->fill.texture_crop_enabled = true;
}
void xlsx_drawing_context::set_crop_left (double val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->fill.texture_crop[0] = val;
    current_drawing_states->back()->fill.texture_crop_enabled = true;
}
void xlsx_drawing_context::set_crop_right (double val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->fill.texture_crop[2] = val;
    current_drawing_states->back()->fill.texture_crop_enabled = true;
}
void xlsx_drawing_context::set_rotation (double val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->rotation = val;
}
void xlsx_drawing_context::set_line_color (int nColor, const std::wstring & sColor)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->line.fill.color.sRGB = sColor;
	current_drawing_states->back()->line.fill.color.nRGB = nColor;
}
void xlsx_drawing_context::set_line_color (int index, int type)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;
	
	_color color;
	if (type == 3)
	{
		color = CorrectSysColor(index, current_drawing_states->back());
	}
	else
	{	
		if (index < 64)
		{
			color.nRGB = shemeDefaultColor[index];
			color.sRGB = STR::toRGB(color.nRGB);
		}
		color.index = -1;
	}

	if (color.sRGB.empty())
		return;
		
	current_drawing_states->back()->line.fill.color = color;
}
void xlsx_drawing_context::set_fill_angle (double val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->fill.angle = val;
}
void xlsx_drawing_context::set_fill_focus (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->fill.focus = val;
}
void xlsx_drawing_context::set_fill_color (int nColor, const std::wstring & sColor, bool background)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

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
	if (current_drawing_states->empty()) return;
	
	_color color;
	if (type == 3)
	{
		color = CorrectSysColor(index, current_drawing_states->back());
	}
	else
	{	
		if (index < 64)
		{
			color.nRGB = shemeDefaultColor[index];
			color.sRGB = STR::toRGB(color.nRGB);
		}
		color.index = -1;
	}
	if (color.sRGB.empty())
		return;

	if (background)	current_drawing_states->back()->fill.color2	= color;
	else			current_drawing_states->back()->fill.color	= color;
}
void xlsx_drawing_context::set_fill_opacity	(double val, bool background)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;
	
	if (background)	current_drawing_states->back()->fill.opacity2	= val;
	else			current_drawing_states->back()->fill.opacity	= val;
}
void xlsx_drawing_context::add_fill_colors(double position, const std::wstring & col)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	_color color;
	color.sRGB = col;
	current_drawing_states->back()->fill.colorsPosition.push_back(std::pair<double, _color>(position, color));
}
void xlsx_drawing_context::add_fill_colors(double position, int index, int type)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	_color color;
	color.index = index;
	if (type == 1)	color.bScheme = true;

	current_drawing_states->back()->fill.colorsPosition.push_back(std::pair<double, _color>(position, color));
}
void xlsx_drawing_context::set_fill_type (int val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;
	
	current_drawing_states->back()->fill.type = (_fill_type) val;
}
//---------------------------------------------------------
void xlsx_drawing_context::set_line_dash(int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

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
	if (current_drawing_states->empty()) return;

	switch(val)
	{
	case 0: current_drawing_states->back()->line.fill.type = fillSolid;		break;
	case 1: current_drawing_states->back()->line.fill.type = fillPattern;	break;
	case 2: current_drawing_states->back()->line.fill.type = fillTexture;	break;
	case 5: current_drawing_states->back()->line.fill.type = fillNone;		break;
	}
}
void xlsx_drawing_context::set_line_style (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	switch(val)
	{
	case 0: current_drawing_states->back()->line.style = L"simple";		break;
	case 1: current_drawing_states->back()->line.style = L"double";		break;
	case 2: current_drawing_states->back()->line.style = L"thickThin";	break;
	case 3: current_drawing_states->back()->line.style = L"thinThick";	break;
	case 4: current_drawing_states->back()->line.style = L"triple";		break;
	}
}

void xlsx_drawing_context::set_line_width (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->line.width = val;
}

void xlsx_drawing_context::set_line_arrow(bool val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->line.arrow.enabled = val;
}
void xlsx_drawing_context::set_arrow_start (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->line.arrow.start = val;
}
void xlsx_drawing_context::set_arrow_end (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->line.arrow.end = val;
}
void xlsx_drawing_context::set_arrow_start_width (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->line.arrow.start_width = val;
}
void xlsx_drawing_context::set_arrow_end_width (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->line.arrow.end_width = val;
}
void xlsx_drawing_context::set_arrow_start_length (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->line.arrow.start_length = val;
}
void xlsx_drawing_context::set_arrow_end_length (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->line.arrow.end_length = val;
}
//----------------------------------------------------------------------
void xlsx_drawing_context::set_fill_old_version (_UINT32 val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	unsigned char backColorIdx		= GETBITS(val, 0, 7);
	unsigned char patternColorIdx	= GETBITS(val, 8,15);
	unsigned char pattern			= GETBITS(val,16,23);
	unsigned char auto_				= GETBITS(val,24,31);

	if (auto_)
	{
		if (current_drawing_states->back()->shape_id == MSOSPT::msosptTextBox)
			set_fill_type(0);
	}
	else if (pattern)
	{
		set_fill_color(patternColorIdx, 2);
	}
	else
	{
		set_fill_color(backColorIdx, 2);
	}


}
void xlsx_drawing_context::set_line_old_version (_UINT32 val)
{
	unsigned char colorIdx	= GETBITS(val,  0,  7);
	unsigned char style		= GETBITS(val,  8, 15);
	unsigned char width		= GETBITS(val, 16, 23);
	unsigned char auto_		= GETBITS(val, 24, 31);

	if (auto_)
		return;

	set_line_color(colorIdx, 2);
	
	if (style < 5) set_line_dash(style);
	else if (style == 0xff)
		set_line_type(5);  

    //switch( style )
    //{
        //case 5:	med;
        //case 6:	dark;
        //case 7:	light;
    //}
    switch( width )
    {
        case 0:		break; //hair
        case 1:     break; //single
        case 2:		set_line_style(1);	break;
        case 3:		set_line_style(44);	break;
    }	
}
void xlsx_drawing_context::set_flag_old_version(_UINT16 val, _UINT16 val2)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	unsigned short flips_flag = 0;
	if (current_drawing_states->back()->shape_id == MSOSPT::msosptArc)
	{
		flips_flag = val;
	}
	if (current_drawing_states->back()->shape_id == MSOSPT::msosptTextBox)
	{
	}
	if (current_drawing_states->back()->shape_id == MSOSPT::msosptLine)
	{
		unsigned char nArrowType = GETBITS(val,  0,  3);
		bool bLineStart = false;
		bool bLineEnd = false;
		bool bFilled = false;
		switch( nArrowType )
		{
			case 0:	bLineStart = false; bLineEnd = true;  bFilled = false;  break;
			case 1:	bLineStart = true;  bLineEnd = true;  bFilled = false;  break;
			case 2: bLineStart = false; bLineEnd = true;  bFilled = true;   break;
			case 3:	bLineStart = true;  bLineEnd = true;  bFilled = true;   break;
		}
		if( bLineStart || bLineEnd )
		{
			unsigned char nArrowWidth = GETBITS(val,  4,  7);
			double fArrowWidth = 3.0;
			switch( nArrowWidth )
			{
				case 0:	fArrowWidth = 2.0;  break;
				case 1:	fArrowWidth = 3.0;  break;
				case 2:	fArrowWidth = 5.0;  break;
			}

			unsigned char nArrowLength = GETBITS(val, 8, 11 );
			double fArrowLength = 3.0;
			switch( nArrowLength )
			{
				case 0:	fArrowLength = 2.5; break;
				case 1:	fArrowLength = 3.5; break;
				case 2:	fArrowLength = 6.0; break;
			}
		}
		flips_flag = val2;
	}
	switch( flips_flag )
	{
		case 1:	
			current_drawing_states->back()->flipH = true; 
			current_drawing_states->back()->flipV = true;	break;
		case 2:
			current_drawing_states->back()->flipH = true;	break;
		case 3:
			current_drawing_states->back()->flipV = true;	break;
	}
}
//-------------------------------------------------------------------------------------------------------------------
void xlsx_drawing_context::set_hyperlink(const std::wstring & link, const std::wstring & display, bool is_external)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	std::wstring sId			= std::wstring(L"hId") + boost::lexical_cast<std::wstring>(hlinks_.size()+1);
	std::wstring link_correct	= link;

	if (!is_external) link_correct = std::wstring(L"#") + link_correct;
	
	_hlink_desc desc = {sId, link_correct, display, is_external};

	hlinks_.push_back(desc);
	current_drawing_states->back()->hyperlink = sId;

	rels_->add( !is_external, sId , link_correct, external_items::typeHyperlink);
}

void xlsx_drawing_context::set_wordart_text(const std::wstring & text)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->wordart.text = text;
}
void xlsx_drawing_context::set_wordart_font(const std::wstring & val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->wordart.font = val;
}
void xlsx_drawing_context::set_wordart_size	(int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->wordart.size = val;
}
void xlsx_drawing_context::set_text_align (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->text.align = val;
}
void xlsx_drawing_context::set_text_vert_align (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->text.vert_align = val;
}
void xlsx_drawing_context::set_text_vertical (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->text.vertical = val;
}
void xlsx_drawing_context::set_text_margin(RECT & val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->text.margins = val;
}
void xlsx_drawing_context::set_text_fit_shape(bool val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->text.fit_shape = val;
}
void xlsx_drawing_context::set_wordart_bold	(bool val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->wordart.bold = val;
}
void xlsx_drawing_context::set_wordart_italic (bool val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->wordart.italic = val;
}
void xlsx_drawing_context::set_wordart_underline (bool val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->wordart.underline = val;
}
void xlsx_drawing_context::set_wordart_strike(bool val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->wordart.strike = val;
}

void xlsx_drawing_context::set_wordart_vertical	(bool val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->wordart.vertical = val;
}
void xlsx_drawing_context::set_wordart_spacing	(double val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->wordart.spacing = val;
}
void xlsx_drawing_context::set_text (const std::wstring & text)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->text.content = text;
}
void xlsx_drawing_context::set_text_wrap (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->text.wrap = val;
}
void xlsx_drawing_context::set_custom_rect(_rect & rect)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->custom_rect = rect;
}
void xlsx_drawing_context::set_properties(const std::wstring & str)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;
	
	current_drawing_states->back()->shape = str;
}
void xlsx_drawing_context::set_custom_verticles (std::vector<ODRAW::MSOPOINT> & points)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->custom_verticles = points;
}
void xlsx_drawing_context::set_custom_guides (std::vector<ODRAW::MSOSG> & guides)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	if (current_drawing_states->back()->shape_id == MSOSPT::msosptNotchedCircularArrow) return;
	
	current_drawing_states->back()->custom_guides = guides;
}
void xlsx_drawing_context::set_custom_segments (std::vector<ODRAW::MSOPATHINFO> & segments)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;
	
	current_drawing_states->back()->custom_segments = segments;
}
void xlsx_drawing_context::set_custom_adjustHandles(std::vector<ODRAW::ADJH> & handles)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;
	
	current_drawing_states->back()->custom_adjustHandles = handles;
}
void xlsx_drawing_context::set_custom_adjustValues(std::vector<_CP_OPT(int)> & values)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->custom_adjustValues = values;
}
void xlsx_drawing_context::set_custom_path (int type_path)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->custom_path = type_path;
}
//----------------------------------------------------------------------------------------------------------
bool xlsx_drawing_context::get_mode_vmlwrite ()
{//comment, shapes in header/footer, ....
	if (current_drawing_states == NULL) return false;	
	if (current_drawing_states->empty()) return false;

	return current_drawing_states->back()->vmlwrite_mode_;
}
void xlsx_drawing_context::set_mode_vmlwrite (bool val)
{//comment, shapes in header/footer, ....
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->vmlwrite_mode_ = val;
}
//----------------------------------------------------------------------------------------------------------
bool xlsx_drawing_context::empty()
{
	return drawing_states.empty();
}
bool xlsx_drawing_context::empty_vml_HF()
{
	return drawing_states_vml_HF.empty();
}
xlsx_drawings_rels_ptr xlsx_drawing_context::get_rels()
{
    return rels_;
}
xlsx_drawings_rels_ptr xlsx_drawing_context::get_vml_HF_rels()
{
    return vml_HF_rels_;
}
xlsx_drawings_rels_ptr xlsx_drawing_context::get_vml_comments_rels()
{
    return vml_comments_rels_;
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

void xlsx_drawing_context::serialize_vml_HF(std::wostream & strm) 
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"xml")
		{
            CP_XML_ATTR(L"xmlns:v"	, L"urn:schemas-microsoft-com:vml");
            CP_XML_ATTR(L"xmlns:o"	, L"urn:schemas-microsoft-com:office:office");
            CP_XML_ATTR(L"xmlns:x"	, L"urn:schemas-microsoft-com:office:excel");

			for (int i = 0 ; i < drawing_states_vml_HF.size(); i++)
			{
				serialize_vml(CP_XML_STREAM(), drawing_states_vml_HF[i]);
			}

		}
    }
}
void xlsx_drawing_context::serialize_vml_comments(std::wostream & strm) 
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"xml")
		{
            CP_XML_ATTR(L"xmlns:v"	, L"urn:schemas-microsoft-com:vml");
            CP_XML_ATTR(L"xmlns:o"	, L"urn:schemas-microsoft-com:office:office");
            CP_XML_ATTR(L"xmlns:x"	, L"urn:schemas-microsoft-com:office:excel");

			CP_XML_NODE(L"v:shapetype")
			{
				CP_XML_ATTR(L"id"		, L"_x0000_t202");
				CP_XML_ATTR(L"coordsize", L"21600,21600");
				CP_XML_ATTR(L"o:spt"	, L"202");
				CP_XML_ATTR(L"path"		, L"m,l,21600r21600,l21600,xe");

				CP_XML_NODE(L"v:stroke")
				{
					CP_XML_ATTR(L"joinstyle", L"miter");
				}
				CP_XML_NODE(L"v:path")
				{
					CP_XML_ATTR(L"gradientshapeok", L"t");				
					CP_XML_ATTR(L"o:connecttype", L"rect");
				}
			}				

			for (int i = 0 ; i < drawing_states_vml_comments.size(); i++)
			{
				serialize_vml(CP_XML_STREAM(), drawing_states_vml_comments[i]);
			}

		}
    }
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
			CP_XML_ATTR(L"xmlns:mc"	, L"http://schemas.openxmlformats.org/markup-compatibility/2006");

			for (int i = 0 ; i < drawing_states.size(); i++)
			{
				serialize(CP_XML_STREAM(), drawing_states[i]);
			}

		}
    }
}
}