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

#include "xlsx_conversion_context.h"

#include "../../Common/Utils/simple_xml_writer.h"
#include "../../../OOXML/Base/Unit.h"
#include "../Format/Auxiliary/HelpFunc.h"

#include "../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../OOXML/PPTXFormat/Logic/SpTreeElem.h"
#include "../../../OOXML/PPTXFormat/Logic/Shape.h"
#include "../../../OOXML/PPTXFormat/Logic/SpTree.h"

#include "../../Common/ODraw/CustomShape.h"
#include "../../Common/ODraw/CustomShapeConvert.h"


namespace oox {

//-----------------------------------------------------------------------------------------------------------
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
	const static int controlPanelColors2[] = 
	{
		0x00000000,	0x00FFFFFF,	0x00000000,	0x00FFFFFF,
		0x00000000,	0x00000000,	0x00000000,	0x00FFFFFF,	
		0x00FFFFFF,	0x00000000,	0x00FFFFFF,	0x00FFFFFF,	
		0x00000000,	0x00000000,	0x00000000,	0x00000000,	
		0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00000000,	
		0x00FFFFFF,	0x00000000,	0x00000000,	0x00000000,	
		0x00000000,	0x00000000,	0x00FFFFFF,	0x00FFFFFF
	};
	const static int controlPanelColors1[] = 
	{
		0x00FFFFFF,	0x00CCCCCC,	0x00FFFFFF,	0x006363CE,
		0x00DDDDDD,	0x00DDDDDD,	0x00888888,	0x00000000,	
		0x00000000,	0x00808080,	0x00B5D5FF,	0x00000000,	
		0x00FFFFFF,	0x00FFFFFF,	0x007F7F7F,	0x00FBFCC5,	
		0x00000000,	0x00F7F7F7,	0x00000000,	0x00FFFFFF,	
		0x00666666,	0x00C0C0C0,	0x00DDDDDD,	0x00C0C0C0,	
		0x00888888,	0x00FFFFFF,	0x00CCCCCC,	0x00000000
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
        const std::wstring id = std::to_wstring(next_drawing_id_++);
        const std::wstring fileName = std::wstring(L"drawing") + id + L".xml";
       
		drawings_.push_back(drawing_elm(fileName, content, rels));
       
		const std::wstring rId = std::wstring(L"rId") + id;
        return std::pair<std::wstring, std::wstring>(fileName, rId);
    }

    std::pair<std::wstring, std::wstring> add_drawing_vml(std::wstring const & content, xlsx_drawings_rels_ptr rels)
    {
        const std::wstring drawing_id	= std::to_wstring(next_drawing_id_++);
		const std::wstring file_vml_id	= std::to_wstring(next_vml_file_id_++);
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
//-----------------------------------------------------------------------------------------------------------
xlsx_drawing_context::xlsx_drawing_context(xlsx_conversion_context & Context) : context_(Context), handle_(Context.get_drawing_context_handle())
	, rels_				(xlsx_drawings_rels::create())
	, vml_HF_rels_		(xlsx_drawings_rels::create())
	, vml_rels_(xlsx_drawings_rels::create())
	, sheet_rels_		(xlsx_drawings_rels::create())
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
	case 0x0011: // Scrollbar
	case 0x0014: // Dropdown list
		start_control(type); return true;
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
int xlsx_drawing_context::getLevel()
{
	if (current_drawing_states == NULL)		return -1;
	if (current_drawing_states->empty())	return -1;

	return current_level;
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
	current_drawing_states->back()->type_control = 0x0008;
	
	count_object++;
}

void xlsx_drawing_context::start_comment()
{
	if (current_drawing_states == NULL) return;

	current_drawing_states->push_back(create_drawing_state());

	current_drawing_states->back()->type = external_items::typeComment;
	
	current_drawing_states->back()->shape_id = MSOSPT::msosptRectangle;
	current_drawing_states->back()->type_control = 0x0019;
	current_drawing_states->back()->object.bVisible = false;

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
	current_drawing_states->back()->type_control = 0x0000;
}
void xlsx_drawing_context::end_group()
{
	if (current_level < 1) return;
	
	//std::vector<_drawing_state_ptr>* cur_states = NULL;
	//for (size_t i = 0; i < current_drawing_states->size(); i++)
	//{
	//	if (!current_drawing_states->empty())
	//	{	
	//		int level = current_level;
	//		cur_states = current_drawing_states;
	//		while (level > 0)
	//		{
	//			_drawing_state_ptr & drawing_state = cur_states->front();

	//			if (i != 0 || level != current_level) // группа сама себя
	//			{
	//				double kf_x = (double)drawing_state->child_anchor.cx / drawing_state->group_anchor.cx;
	//				double kf_y = (double)drawing_state->child_anchor.cy / drawing_state->group_anchor.cy;

	//				current_drawing_states->at(i)->child_anchor.cx *= kf_x;
	//				current_drawing_states->at(i)->child_anchor.cy *= kf_y;
	//			
	//				current_drawing_states->at(i)->child_anchor.x = current_drawing_states->at(i)->child_anchor.x * kf_x + drawing_state->child_anchor.x;
	//				current_drawing_states->at(i)->child_anchor.y = current_drawing_states->at(i)->child_anchor.y * kf_y + drawing_state->child_anchor.y;
	//			}
	//			level--;
	//			cur_states = cur_states->front()->parent_drawing_states;
	//		}
	//	}
	//}
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
void xlsx_drawing_context::start_control(int type)
{
	if (current_drawing_states == NULL) return;

	current_drawing_states->push_back(create_drawing_state());

	current_drawing_states->back()->type = external_items::typeControl;

	current_drawing_states->back()->shape_id = MSOSPT::msosptRectangle;
	current_drawing_states->back()->type_control = type;

	count_object++;
}
void xlsx_drawing_context::start_shape(int type)
{
	if (current_drawing_states == NULL) return;

	current_drawing_states->push_back(create_drawing_state());

	current_drawing_states->back()->type = external_items::typeShape;
	current_drawing_states->back()->type_control = type;

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
void xlsx_drawing_context::set_alternative_drawing(const std::wstring & xml_data)
{
	if (xml_data.empty()) return;
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->xmlAlternative = xml_data;
}

void xlsx_drawing_context::reset_alternative_drawing()
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	std::wstring xml_data = current_drawing_states->back()->xmlAlternative;
	current_drawing_states->back()->xmlAlternative.clear();
	
	XmlUtils::CXmlLiteReader oReader;

	if ( !oReader.FromString( xml_data ) )
		return;
	if ( !oReader.ReadNextNode() ) //anchor or object(sp in group)
		return;

	std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

	nullable<PPTX::Logic::SpTreeElem> oElement;

	int nCurDepth = oReader.GetDepth();
	while ( oReader.ReadNextSiblingNode( nCurDepth ) )
	{
		sName = XmlUtils::GetNameNoNS(oReader.GetName());
		
		if (L"AlternateContent" == sName || L"Choice" == sName || 
			L"twoCellAnchor" == sName || L"oneCellAnchor" == sName || L"absoluteAnchor" == sName)
		{
			nCurDepth++;
			continue;
		}
		else if ( L"graphicFrame" == sName || L"pic" == sName || L"sp" == sName || L"grpSp" == sName || L"cxnSp" == sName || L"spTree" == sName)
		{
			oElement = oReader;
		}
	}

	if (oElement.IsInit())
	{
		NSBinPptxRW::CXmlWriter writer(XMLWRITER_DOC_TYPE_XLSX);

		smart_ptr<PPTX::Logic::Shape> shape = oElement->GetElem().smart_dynamic_cast<PPTX::Logic::Shape>();
		if (shape.IsInit())
		{
			shape->spPr.Geometry.toXmlWriter(&writer);

			if (shape->spPr.scene3d.IsInit())
				shape->spPr.scene3d->toXmlWriter(&writer);
			
			if (shape->spPr.sp3d.IsInit())
				shape->spPr.sp3d->toXmlWriter(&writer);

			current_drawing_states->back()->xmlGeomAlternative = writer.GetXmlString();
			writer.ClearNoAttack();
			
			if (shape->style.IsInit())
				shape->style->toXmlWriter(&writer);

			current_drawing_states->back()->xmlStyleAlternative = writer.GetXmlString();
			writer.ClearNoAttack();

			if ((shape->txBody.IsInit()) && (shape->txBody->bodyPr.IsInit()))
			{
				bool bWordArt = shape->txBody->bodyPr->prstTxWarp.IsInit();

				if (bWordArt || (false == shape->txBody->Paragrs.empty()) && (false == shape->txBody->Paragrs[0].RunElems.empty()))
				{
					shape->txBody->toXmlWriter(&writer);
					current_drawing_states->back()->xmlTxBodyAlternative = writer.GetXmlString();
					writer.ClearNoAttack();
				}
			}
			if (shape->spPr.Fill.is_init())
			{
				shape->spPr.Fill.toXmlWriter(&writer);

				current_drawing_states->back()->xmlFillAlternative = writer.GetXmlString();
				writer.ClearNoAttack();
			}
			if (shape->spPr.EffectList.is_init())
			{
				shape->spPr.EffectList.toXmlWriter(&writer);

				current_drawing_states->back()->xmlEffectAlternative = writer.GetXmlString();
				writer.ClearNoAttack();
			}
		}
		smart_ptr<PPTX::Logic::SpTree> groupShape = oElement->GetElem().smart_dynamic_cast<PPTX::Logic::SpTree>();
		if (groupShape.IsInit())
		{//smartArt	or group	

			if (groupShape->grpSpPr.Fill.is_init())
			{
				groupShape->grpSpPr.Fill.toXmlWriter(&writer);
				current_drawing_states->back()->xmlFillAlternative = writer.GetXmlString();
				writer.ClearNoAttack();
			}

			if (false == groupShape->SpTreeElems.empty()) // smartArt
			{
				if (false == groupShape->grpSpPr.xfrm.IsInit())
				{
					groupShape->grpSpPr.xfrm.Init();
					set_xfrm_from_anchor(groupShape->grpSpPr.xfrm.GetPointer(), current_drawing_states->back());
				}
				groupShape->toXmlWriter(&writer);
				current_drawing_states->back()->xmlAlternative = writer.GetXmlString();
				writer.ClearNoAttack();
			}
		}
	}

	//std::wstring sName = oReader.GetName();
	//if ( (L"xdr:anchor" == sName || L"xdr:object" == sName) && !oReader.IsEmptyNode() ) //object - 
	//{
	//	current_drawing_states->back()->xmlAlternative = oReader.GetInnerXml();
	//	//fill(color index???) ??? prst ???
	//}
}
void xlsx_drawing_context::set_xfrm_from_anchor(PPTX::Logic::Xfrm *xfrm, _drawing_state_ptr state)
{
	if (!xfrm) return;
	if (!state) return;

	if (state->type_anchor == 1)
	{
		xfrm->offX = (int)(state->sheet_anchor.absolute.x);
		xfrm->offY = (int)(state->sheet_anchor.absolute.y);
		xfrm->extX = (int)(state->sheet_anchor.absolute.cx);
		xfrm->extY = (int)(state->sheet_anchor.absolute.cy);
		
		xfrm->chExtX = (int)(state->sheet_anchor.absolute.cx);
		xfrm->chExtY = (int)(state->sheet_anchor.absolute.cy);
	}
	if (state->type_anchor == 3)
	{
		xfrm->offX = (int)(state->absolute_anchor.x * 12700);
		xfrm->offY = (int)(state->absolute_anchor.y * 12700);
		xfrm->extX = (int)(state->absolute_anchor.cx * 12700);
		xfrm->extY = (int)(state->absolute_anchor.cy * 12700); 
		
		xfrm->chExtX = (int)(state->absolute_anchor.cx * 12700);
		xfrm->chExtY = (int)(state->absolute_anchor.cy * 12700); 
	}
}
void xlsx_drawing_context::set_hidden(bool val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;
	
	current_drawing_states->back()->hidden = val;
}
void xlsx_drawing_context::set_object_visible(bool val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.bVisible = val;
}
void xlsx_drawing_context::set_object_anchor(int col, int row)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.col = col;
	current_drawing_states->back()->object.row = row;

}
void xlsx_drawing_context::set_object_id(int val)
{
}
void xlsx_drawing_context::set_object_link(const std::wstring & formula)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.link = formula;
}
void xlsx_drawing_context::set_object_x_val(int val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.x_val = val;
}
void xlsx_drawing_context::set_object_x_min(int val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.x_min = val;
}
void xlsx_drawing_context::set_object_x_max(int val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.x_max = val;
}
void xlsx_drawing_context::set_object_x_inc(int val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.x_inc = val;
}
void xlsx_drawing_context::set_object_x_page(int val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.x_page = val;
}
void xlsx_drawing_context::set_object_x_sel(int val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.x_sel = val;
}
void xlsx_drawing_context::set_object_x_sel_type(int val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.x_sel_type = val;
}
void xlsx_drawing_context::set_object_lct(int val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.lct = val;
}
void xlsx_drawing_context::set_object_fmlaRange(const std::wstring & fmla)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.fmlaRange = fmla;
}
void xlsx_drawing_context::set_object_drop_style(int val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.drop_style = val;
}
void xlsx_drawing_context::set_object_drop_lines(int val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->object.drop_lines = val;
}
void xlsx_drawing_context::end_drawing()
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	reset_alternative_drawing();

	if (current_drawing_states->back()->type == external_items::typeGroup)  return;
	
	//std::vector<_drawing_state_ptr>* cur_states = current_drawing_states;;
	//
	//int level = current_level;

	//while (level > 0)
	//{
	//	_drawing_state_ptr & drawing_state = cur_states->front();

	//	double kf_x = (double)drawing_state->child_anchor.cx / drawing_state->group_anchor.cx;
	//	double kf_y = (double)drawing_state->child_anchor.cy / drawing_state->group_anchor.cy;

	//	current_drawing_states->back()->child_anchor.cx *= kf_x;
	//	current_drawing_states->back()->child_anchor.cy *= kf_y;
	//
	//	current_drawing_states->back()->child_anchor.x = drawing_state->group_anchor.x	+ 
	//		(long)(kf_x * (current_drawing_states->back()->child_anchor.x - drawing_state->group_anchor.x));
	//	current_drawing_states->back()->child_anchor.y = drawing_state->group_anchor.y	+ 
	//		(long)(kf_y * (current_drawing_states->back()->child_anchor.y - drawing_state->group_anchor.y));

	//	level--;
	//	cur_states = cur_states->front()->parent_drawing_states;
	//}
	
	end_drawing(current_drawing_states->back());

	if ( current_drawing_states->back()->type == external_items::typeComment ||
		 current_drawing_states->back()->type == external_items::typeControl )
	{
		drawing_states_vml.push_back(current_drawing_states->back());

		if (current_drawing_states->back()->type == external_items::typeComment)
			current_drawing_states->pop_back();
	}
	else if (!current_drawing_states->back()->vml_shape.empty())
	{ // && current_drawing_states->back()->vml_HF_mode_
		drawing_states_vml_HF.push_back(current_drawing_states->back());
		current_drawing_states->pop_back();
	}
}

void xlsx_drawing_context::end_drawing(_drawing_state_ptr & drawing_state)
{
	if (!drawing_state) return;

	if (drawing_state->id < 0)	
		drawing_state->id = count_object + 0x20000;

	if (((drawing_state->rotation >= 45.0 && drawing_state->rotation <= 135.0) || 
		(drawing_state->rotation >= 225.0 && drawing_state->rotation <= 315.0) || 
		(drawing_state->rotation <= -45.0 && drawing_state->rotation >= -135.0) ||
		(drawing_state->rotation <= -225.0 && drawing_state->rotation >= -315.0))	&& current_level > 0)
	{
		int left = current_drawing_states->back()->child_anchor.x;
		int top	= current_drawing_states->back()->child_anchor.y;
		
		int right	= current_drawing_states->back()->child_anchor.x + current_drawing_states->back()->child_anchor.cx;
		int bottom	= current_drawing_states->back()->child_anchor.y + current_drawing_states->back()->child_anchor.cy;
		
		left =	(right	+	current_drawing_states->back()->child_anchor.x) * 0.5 - (bottom	-	current_drawing_states->back()->child_anchor.y)  * 0.5;
		top	=	(bottom	+	current_drawing_states->back()->child_anchor.y) * 0.5 - (right	-	current_drawing_states->back()->child_anchor.x) * 0.5;

		right	= left + current_drawing_states->back()->child_anchor.cy;
		bottom	= top + current_drawing_states->back()->child_anchor.cx;

		current_drawing_states->back()->child_anchor.x = left;
		current_drawing_states->back()->child_anchor.y = top;
		
		current_drawing_states->back()->child_anchor.cx = right - left;
		current_drawing_states->back()->child_anchor.cy = bottom - top;
	}

	if (  drawing_state->type == external_items::typeImage ||
		( drawing_state->type == external_items::typeShape && drawing_state->shape_id == msosptPictureFrame ))
	{
		drawing_state->type = external_items::typeImage;

		if (!drawing_state->fill.picture_target.empty())
			drawing_state->fill.texture_target = drawing_state->fill.picture_target;

		if (!drawing_state->fill.texture_target.empty())
		{
			bool isIternal = false;
			drawing_state->objectId = handle_.impl_->get_mediaitems().find_image( drawing_state->fill.texture_target, isIternal);
			
			serialize_pic(drawing_state);
		
			if (drawing_state->vml_HF_mode_)
			{
				vml_HF_rels_->add(isIternal, drawing_state->objectId , drawing_state->fill.texture_target, drawing_state->type);
			}
			else
			{
				rels_->add(isIternal, drawing_state->objectId , drawing_state->fill.texture_target, drawing_state->type);
			}
		}
		else 
			drawing_state->type = external_items::typeShape;
	}
	if ( drawing_state->type == external_items::typeChart )
	{
		context_.end_chart();		
		in_chart_ = false;
	
		std::wstring	target;
		drawing_state->objectId = handle_.impl_->get_mediaitems().add_chart(target);
		
		serialize_chart(drawing_state); 
		
		rels_->add( true, drawing_state->objectId, target, drawing_state->type);
	}
	if ( drawing_state->type == external_items::typeComment )
	{
		context_.get_comments_context().set_content(drawing_state->text.content);
		
		serialize_vml_shape(drawing_state);
	
		context_.get_comments_context().end_comment();
	}
	if ( drawing_state->type == external_items::typeShape)
	{
		serialize_shape(drawing_state);
	}
	if ( drawing_state->type == external_items::typeOleObject )
	{
		serialize_shape(drawing_state);
		drawing_states_objects.push_back(drawing_state); // for serialize in sheet
	}
	if ( drawing_state->type == external_items::typeActiveX) // объекты управления с бинарными свойствами
	{		
		context_.end_activeX();

		serialize_shape(drawing_state);
		drawing_states_controls.push_back(drawing_state); // for serialize in sheet
	}
	if ( drawing_state->type == external_items::typeControl)// объекты управления с xml свойствами
	{
		serialize_control(drawing_state);
		if (drawing_state->objectId.empty())
		{
			std::wstring target;
			drawing_state->objectId = context_.get_mediaitems().add_control_props(target);

			sheet_rels_->add(true, drawing_state->objectId, L"ctrlProps/" + target, external_items::typeControlProps);
			
			std::wstringstream strm;		
			serialize_control_props(strm, drawing_state);

			context_.add_control_props(target, strm.str());
		}
		drawing_states_controls.push_back(drawing_state); // for serialize in sheet
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
						drawing_state->name = L"Group_" + std::to_wstring(count_object);
					}
					CP_XML_ATTR(L"name", drawing_state->name);
					
					if (!drawing_state->description.empty())
					{
						CP_XML_ATTR(L"descr", drawing_state->description);
					}
					if (drawing_state->hidden)
					{
						CP_XML_ATTR(L"hidden", 1);
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
				CP_XML_ATTR(L"bwMode", L"auto");
				serialize_xfrm(CP_XML_STREAM(), drawing_state);
				
				if (false == drawing_state->xmlFillAlternative.empty()) //Family budget (monthly)1.xls
				{
					CP_XML_STREAM() << drawing_state->xmlFillAlternative;
				}
				//serialize_fill(CP_XML_STREAM(), drawing_state); ???? белый //Family budget (monthly)1.xls
				//serialize_line(CP_XML_STREAM(), drawing_state);		
			}

			for (size_t i = 1; i < current_drawing_states->size(); i++)
			{
				CP_XML_STREAM() << current_drawing_states->at(i)->shape;
				//todooo current_drawing_states->at(i).shape.erase(); // память поэкономить
			}
		}
	}

	drawing_state->shape = strm.str();
}
void xlsx_drawing_context::serialize_vml_shape(_drawing_state_ptr & drawing_state)
{
	std::wstringstream strm;
	
	CP_XML_WRITER(strm)    
	{		
		std::wstringstream strmStyle;

		strmStyle << L"position:absolute;";	
		strmStyle << L"margin-left:" << std::to_wstring(drawing_state->sheet_anchor.absolute.x / 12700.) << L"pt;";	//in pt (1 pt = 12700 emu)
		strmStyle << L"margin-top:"	<< std::to_wstring(drawing_state->sheet_anchor.absolute.y / 12700.)	<< L"pt;";
		strmStyle << L"width:" << std::to_wstring(drawing_state->sheet_anchor.absolute.cx / 12700.)	<< L"pt;";
		strmStyle << L"height:"	<< std::to_wstring(drawing_state->sheet_anchor.absolute.cy / 12700.) << L"pt;";
		strmStyle << L"z-index:" << std::to_wstring(drawing_state->id)	 << L";";
		
		if (drawing_state->object.bVisible == false) 
			strmStyle << L"visibility:hidden;";	

		CP_XML_NODE(L"v:shape")
		{
			CP_XML_ATTR(L"id", L"_x0000_s" + std::to_wstring(drawing_state->id));

			if (drawing_state->shape_id != msosptNotPrimitive)
			{
				CP_XML_ATTR(L"type", L"#_x0000_t" + std::to_wstring(drawing_state->shape_id));
				CP_XML_ATTR(L"o:spt", drawing_state->shape_id);
			}
			CP_XML_ATTR(L"fillcolor", L"#" + drawing_state->fill.color.sRGB);

			if (drawing_state->line.width > 0)
			{
				CP_XML_ATTR(L"strokeweight",  std::to_wstring(drawing_state->line.width) +L"pt");
			}
			CP_XML_ATTR(L"style", strmStyle.str());

			if (!current_drawing_states->back()->custom_verticles.empty() &&
				!current_drawing_states->back()->custom_segments.empty())
			{		
				ODRAW::PathParser oParser (	current_drawing_states->back()->custom_segments, 
											current_drawing_states->back()->custom_verticles, 
											current_drawing_states->back()->custom_guides,
											current_drawing_states->back()->custom_x_limo,
											current_drawing_states->back()->custom_y_limo);
				std::wstring path = oParser.GetVmlPath();

				if (false == path.empty())
					CP_XML_ATTR(L"path", path);
			}
			if (drawing_state->type == external_items::typeControl)
			{
				CP_XML_ATTR(L"o:button", L"t");
			}
			if (drawing_state->line.fill.type == fillNone)
			{
				CP_XML_ATTR(L"stroked", L"f");
			}
			if (drawing_state->fill.type == fillNone)
			{
				CP_XML_ATTR(L"filled", L"f");
			}
//----------------------------------------------------------------------------------------------
			CP_XML_NODE(L"v:shadow")
			{
				CP_XML_ATTR(L"color", L"black");
				CP_XML_ATTR(L"obscured", L"t");
				CP_XML_ATTR(L"on", L"t");
			}

			//CP_XML_NODE(L"v:wrap")
			//{
			//	CP_XML_ATTR(L"v:type", L"none");
			//}
		
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

					if (current_drawing_states->back()->vml_HF_mode_ )
					{
						vml_HF_rels_->add(isIternal, rId , drawing_state->fill.texture_target, external_items::typeImage);
					}
					else
					{
						vml_rels_->add(isIternal, rId , drawing_state->fill.texture_target, external_items::typeImage);
					}
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
			if (!drawing_state->text.vml_content.empty())
			{
				CP_XML_NODE(L"v:textbox")
				{
					//style='mso-direction-alt:auto' 
					//o:singleclick="f"
					CP_XML_STREAM() << drawing_state->text.vml_content;
				}
			}
			if (drawing_state->type_control == 0x000b)
			{
				CP_XML_NODE(L"v:textbox")
				{
					CP_XML_ATTR(L"o:singleclick", L"f");
					CP_XML_NODE(L"div")
					{
						CP_XML_ATTR(L"style", L"text-align:left");
					}
				}
			}
			CP_XML_NODE(L"x:ClientData")
			{
				switch(drawing_state->type_control)
				{
					case 0x0000: CP_XML_ATTR(L"ObjectType", L"Group");		break;
					case 0x0001: CP_XML_ATTR(L"ObjectType", L"Shape");		break; // Line
					case 0x0002: CP_XML_ATTR(L"ObjectType", L"Rect");		break;
					case 0x0003: CP_XML_ATTR(L"ObjectType", L"Shape");		break; // Oval
					case 0x0004: CP_XML_ATTR(L"ObjectType", L"Shape");		break; // Arc
					case 0x0006: CP_XML_ATTR(L"ObjectType", L"Shape");		break; // Text
					case 0x0007: CP_XML_ATTR(L"ObjectType", L"Button");		break;
					case 0x0008: CP_XML_ATTR(L"ObjectType", L"Pict");		break;
					case 0x0009: CP_XML_ATTR(L"ObjectType", L"Shape");		break; // Polygon:			
					case 0x000B: CP_XML_ATTR(L"ObjectType", L"Checkbox");	break;
					case 0x000C: CP_XML_ATTR(L"ObjectType", L"Radio");		break;
					case 0x000D: CP_XML_ATTR(L"ObjectType", L"Edit");		break;
					case 0x000E: CP_XML_ATTR(L"ObjectType", L"Label");		break;
					case 0x000F: CP_XML_ATTR(L"ObjectType", L"Dialog");		break;
					case 0x0010: CP_XML_ATTR(L"ObjectType", L"Spin");		break;
					case 0x0012: CP_XML_ATTR(L"ObjectType", L"List");		break;
					case 0x0013: CP_XML_ATTR(L"ObjectType", L"GBox");		break;
					case 0x0011: CP_XML_ATTR(L"ObjectType", L"Scroll");		break;
					case 0x0014: CP_XML_ATTR(L"ObjectType", L"Drop");		break;
					case 0x001E: CP_XML_ATTR(L"ObjectType", L"Shape");		break; // OfficeArt object
					case 0x0019: CP_XML_ATTR(L"ObjectType", L"Note");		break;
					default: break;
				}
				CP_XML_NODE(L"x:MoveWithCells"){}
				CP_XML_NODE(L"x:SizeWithCells"){}

				if (drawing_state->sheet_anchor.xFrom >= 0 && drawing_state->sheet_anchor.xTo > 0)
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

				//if (drawing_state->type_control != 0x000b)
				{
					CP_XML_NODE(L"x:Row")		{CP_XML_CONTENT(drawing_state->object.row);}
					CP_XML_NODE(L"x:Column")	{CP_XML_CONTENT(drawing_state->object.col);}
				}

				if (drawing_state->object.bVisible) CP_XML_NODE(L"x:Visible");

				if (!drawing_state->object.macro.empty())
				{
					CP_XML_NODE(L"x:FmlaMacro"){CP_XML_CONTENT(drawing_state->object.macro);}
				}
				if (!drawing_state->object.link.empty() )
				{
					CP_XML_NODE(L"x:FmlaLink"){CP_XML_CONTENT(drawing_state->object.link);}
				}
				if (drawing_state->object.x_val)
				{
					CP_XML_NODE(L"x:Val"){CP_XML_CONTENT(*drawing_state->object.x_val);}
				}
				if (drawing_state->object.x_min)
				{
					CP_XML_NODE(L"x:Min"){CP_XML_CONTENT(*drawing_state->object.x_min);}
				}
				if (drawing_state->object.x_max)
				{
					CP_XML_NODE(L"x:Max"){CP_XML_CONTENT(*drawing_state->object.x_max);}
				}
				if (drawing_state->object.x_inc)
				{
					CP_XML_NODE(L"x:Inc"){CP_XML_CONTENT(*drawing_state->object.x_inc);}
				}
				if (drawing_state->object.x_page)
				{
					CP_XML_NODE(L"x:Page"){CP_XML_CONTENT(*drawing_state->object.x_page);}
				}
				if (!drawing_state->object.fmlaRange.empty() )
				{
					CP_XML_NODE(L"x:FmlaRange"){CP_XML_CONTENT(drawing_state->object.fmlaRange);}
				}			
				if (drawing_state->object.x_sel)
				{
					CP_XML_NODE(L"x:Sel"){CP_XML_CONTENT(*drawing_state->object.x_sel);}
				}
				if (drawing_state->object.x_sel_type)
				{
					switch(*drawing_state->object.x_sel_type)
					{
					case 0:
					default:
						CP_XML_NODE(L"x:SelType"){CP_XML_CONTENT(L"Single");}
					}
				}
				if (drawing_state->object.lct)
				{
					switch(*drawing_state->object.lct)
					{
					case 0:
					default:
						CP_XML_NODE(L"x:LCT"){CP_XML_CONTENT(L"Normal");}
					}
				}
				if (drawing_state->object.drop_style)
				{
					switch(*drawing_state->object.drop_style)
					{
					case 0:
					default:
						CP_XML_NODE(L"x:DropStyle"){CP_XML_CONTENT(L"Combo");}
					}
				}
				if (drawing_state->object.drop_lines)
				{
					CP_XML_NODE(L"x:DropLines"){CP_XML_CONTENT(*drawing_state->object.drop_lines);}
				}
			}
		}	
	
	}
	drawing_state->vml_shape = strm.str();
}
void xlsx_drawing_context::serialize_vml_pic(_drawing_state_ptr & drawing_state)
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
			style += std::wstring(L"width:")	+ std::to_wstring(drawing_state->child_anchor.cx)	+ std::wstring(L"pt;");
			style += std::wstring(L"height:")	+ std::to_wstring(drawing_state->child_anchor.cy)	+ std::wstring(L"pt;");
			style += std::wstring(L"z-index:")	+ std::to_wstring(drawing_state->id) + std::wstring(L";");

			CP_XML_ATTR(L"style",style);

			CP_XML_NODE(L"v:imagedata")
			{
				CP_XML_ATTR(L"o:relid", drawing_state->objectId);
			}
		}
	}
	drawing_state->vml_shape = strm.str();
}

void xlsx_drawing_context::serialize_pic(_drawing_state_ptr & drawing_state)
{
	if (drawing_state->vml_HF_mode_) 
		return serialize_vml_pic(drawing_state);

	std::wstringstream strm;

	CP_XML_WRITER(strm)    
	{
		CP_XML_NODE(L"xdr:pic")
		{ 
			CP_XML_ATTR(L"macro", drawing_state->object.macro);

			CP_XML_NODE(L"xdr:nvPicPr")
			{
				CP_XML_NODE(L"xdr:cNvPr")
				{
					CP_XML_ATTR(L"id", drawing_state->id);
					if (drawing_state->name.empty())	
						drawing_state->name = L"Picture_" + drawing_state->objectId.substr(5);
					CP_XML_ATTR(L"name", drawing_state->name);		

					if (!drawing_state->description.empty())
					{
						CP_XML_ATTR(L"descr", drawing_state->description);
					}
					if (drawing_state->hidden)
					{
						CP_XML_ATTR(L"hidden", 1);
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
			serialize_bitmap_fill(CP_XML_STREAM(), drawing_state->fill, drawing_state->objectId, L"xdr:");

			CP_XML_NODE(L"xdr:spPr")
			{
				serialize_xfrm(CP_XML_STREAM(), drawing_state);

				CP_XML_NODE(L"a:prstGeom")
				{
					CP_XML_ATTR(L"prst", "rect");
					CP_XML_NODE(L"a:avLst");
				}

				CP_XML_NODE(L"a:noFill");
				
				if (false == drawing_state->xmlEffectAlternative.empty())
				{
					CP_XML_STREAM() << drawing_state->xmlEffectAlternative;
				}
				else
				{
					//serialize_effect(CP_XML_STREAM(), drawing_state);
				}
			}
		}
	}

	drawing_state->shape = strm.str();
}

void xlsx_drawing_context::serialize_chart(_drawing_state_ptr & drawing_state)
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
						drawing_state->name = L"Chart_" + std::to_wstring(count_object);

					CP_XML_ATTR(L"name", drawing_state->name);
					if (!drawing_state->description.empty())
					{
						CP_XML_ATTR(L"descr", drawing_state->description);
					}
					if (drawing_state->hidden)
					{
						CP_XML_ATTR(L"hidden", 1);
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
						CP_XML_ATTR(L"r:id", drawing_state->objectId);
					}
				}
			}               
		} 
    }  
	drawing_state->shape = strm.str();
}
void xlsx_drawing_context::serialize_control(_drawing_state_ptr & drawing_state)
{
	std::wstringstream strm;
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
						drawing_state->name = L"Control_" + std::to_wstring(count_object);
					}
					CP_XML_ATTR(L"name", drawing_state->name);
					
					if (!drawing_state->description.empty())
					{
						CP_XML_ATTR(L"descr", drawing_state->description);
					}
					CP_XML_ATTR(L"hidden", 1);

					CP_XML_NODE(L"a:extLst")
					{
						CP_XML_NODE(L"a:ext")
						{
							CP_XML_ATTR(L"uri", L"{63B3BB69-23CF-44E3-9099-C40C66FF867C}");

							CP_XML_NODE(L"a14:compatExt")
							{
								CP_XML_ATTR(L"xmlns:a14", L"http://schemas.microsoft.com/office/drawing/2010/main");
								CP_XML_ATTR(L"spid", L"_x0000_s" + std::to_wstring(drawing_state->id));
							}
						}
					}
				}
				CP_XML_NODE(L"xdr:cNvSpPr");
			}
			
			CP_XML_NODE(L"xdr:spPr")
			{
				serialize_xfrm(CP_XML_STREAM(), drawing_state);

				CP_XML_NODE(L"a:prstGeom")
				{
					CP_XML_ATTR(L"prst", L"rect");
					if (!drawing_state->wordart.is)	CP_XML_NODE(L"a:avLst");
				}
				CP_XML_NODE(L"a:noFill");
				CP_XML_NODE(L"a:ln")
				{
					CP_XML_NODE(L"a:noFill");
				}
			}
			serialize_text(CP_XML_STREAM(), drawing_state);
		}
	}
	drawing_state->shape = strm.str();

	serialize_vml_shape(drawing_state);
}
void xlsx_drawing_context::serialize_shape(_drawing_state_ptr & drawing_state)
{
	if (drawing_state->vml_HF_mode_) 
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
			drawing_state->bCustom = true;
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
			CP_XML_ATTR(L"macro", drawing_state->object.macro);

			CP_XML_NODE(L"xdr:nvSpPr")
			{
				CP_XML_NODE(L"xdr:cNvPr")
				{
					CP_XML_ATTR(L"id", drawing_state->id);
					
					if (drawing_state->name.empty())
					{
						if (drawing_state->wordart.is)
							drawing_state->name = L"WordArt_" + std::to_wstring(count_object);
						else if ( drawing_state->type == external_items::typeOleObject )
							drawing_state->name = L"Object_" + std::to_wstring(count_object);
						else if ( drawing_state->type == external_items::typeActiveX )
							drawing_state->name = L"ActiveX_" + std::to_wstring(count_object);
						else
							drawing_state->name = L"Shape_" + std::to_wstring(count_object);
					}
					CP_XML_ATTR(L"name", drawing_state->name);
					
					if (!drawing_state->description.empty())
					{
						CP_XML_ATTR(L"descr", drawing_state->description);
					}
					if (drawing_state->hidden)
					{
						CP_XML_ATTR(L"hidden", 1);
					}
					if (!drawing_state->hyperlink.empty())
					{
						CP_XML_NODE(L"a:hlinkClick")
						{
							CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
							
							CP_XML_ATTR(L"r:id", drawing_state->hyperlink);
						}
					}
					//if ( drawing_state->type == external_items::typeOleObject ) + VmlDrawing
					//{
					//	CP_XML_NODE(L"a:extLst")
					//	{
					//		CP_XML_NODE(L"a:ext")
					//		{
					//			CP_XML_ATTR(L"uri", L"{63B3BB69-23CF-44E3-9099-C40C66FF867C}");
					//			CP_XML_ATTR(L"xmlns:a14", L"http://schemas.microsoft.com/office/drawing/2010/main");

					//			CP_XML_NODE(L"a14:compatExt")
					//			{
					//				CP_XML_ATTR(L"spid", L"_x0000_s" + std::to_wstring(drawing_state->id));
					//			}
					//		}
					//	}
					//}
				}
				CP_XML_NODE(L"xdr:cNvSpPr")
				{
					if (drawing_state->bTextBox)CP_XML_ATTR(L"txBox", 1);
					//CP_XML_NODE(L"a:spLocks")
					//{
					//	CP_XML_ATTR(L"noGrp", 1); 
					//	CP_XML_ATTR(L"noChangeArrowheads", 1);
					//}
				}
			}
			
			CP_XML_NODE(L"xdr:spPr")
			{
				serialize_xfrm(CP_XML_STREAM(), drawing_state);

				if (false == drawing_state->xmlGeomAlternative.empty())
				{
					CP_XML_STREAM() << drawing_state->xmlGeomAlternative;
				}
				else
				{
					if (prstGeom.empty() == false)
					{
						CP_XML_NODE(L"a:prstGeom")
						{
							CP_XML_ATTR(L"prst", prstGeom);
							if (false == drawing_state->wordart.is)
							{
								CP_XML_NODE(L"a:avLst")
								{
									// нужен перерасчет
									//for (size_t i = 0; i < drawing_state->custom_adjustValues.size(); i++)
									//{
									//	if (drawing_state->custom_adjustValues[i])
									//	{
									//		CP_XML_NODE(L"a:gd")
									//		{
									//			CP_XML_ATTR(L"name", L"adj" + std::to_wstring(i + 1));
									//			CP_XML_ATTR(L"fmla", L"val " + std::to_wstring(*drawing_state->custom_adjustValues[i]));
									//		}
									//	}
									//}
								}
							}
						}
					}
					else if (customGeom.empty() == false)
					{
						CP_XML_NODE(L"a:custGeom")
						{
							CP_XML_STREAM() << customGeom;
						}
					}
					else
					{
						CP_XML_NODE(L"a:prstGeom")
						{
							CP_XML_ATTR(L"prst", L"rect");
						}
					}
				}
				if (!is_lined_shape(drawing_state))
				{
					if (false == drawing_state->xmlFillAlternative.empty()) //Family budget (monthly)1.xls
					{
						CP_XML_STREAM() << drawing_state->xmlFillAlternative;
					}
					else
					{
						serialize_fill(CP_XML_STREAM(), drawing_state);
					}
				}
				serialize_line(CP_XML_STREAM(), drawing_state);	
				
				if (false == drawing_state->xmlEffectAlternative.empty())
				{
					CP_XML_STREAM() << drawing_state->xmlEffectAlternative;
				}
				else
				{
					if (drawing_state->shadow.enabled && drawing_state->shadow.offsetX != 0)
					{
						double dist = sqrt(drawing_state->shadow.offsetX * drawing_state->shadow.offsetX +
							drawing_state->shadow.offsetY * drawing_state->shadow.offsetY);
						double dir = atan(abs(drawing_state->shadow.offsetY / drawing_state->shadow.offsetX)) * 180. / 3.1415926;

						if (drawing_state->shadow.offsetX < 0 && drawing_state->shadow.offsetY < 0)
							dir += 180;
						else if (drawing_state->shadow.offsetX < 0)
							dir = 180 - dir;
						else if (drawing_state->shadow.offsetY < 0)
							dir = 360 - dir;

						CP_XML_NODE(L"a:effectLst")
						{
							CP_XML_NODE(L"a:outerShdw")
							{
								CP_XML_ATTR(L"dist", (int)(dist)); 
								CP_XML_ATTR(L"dir", (int)(dir * 60000)); 		
								
								CP_XML_ATTR(L"sx", (int)(drawing_state->shadow.scaleX2X * 100000 + 0.5)); 
								CP_XML_ATTR(L"sy", (int)(drawing_state->shadow.scaleY2Y * 100000 + 0.5)); 		
								
								CP_XML_ATTR(L"rotWithShape", 0);

								serialize_color(CP_XML_STREAM(), drawing_state->shadow.color, drawing_state->shadow.opacity);	
							}
						}
					}
					//serialize_effect(CP_XML_STREAM(), drawing_state);
				}
			}
			if (false == drawing_state->xmlStyleAlternative.empty())
			{
				CP_XML_STREAM() << drawing_state->xmlStyleAlternative;
			}
			serialize_text(CP_XML_STREAM(), drawing_state);
		}
	}

	drawing_state->shape = strm.str();
}

bool xlsx_drawing_context::is_lined_shape(_drawing_state_ptr & drawing_state)
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
	
	for (size_t i = 0 ; i < drawing_state->custom_verticles.size(); i++)
	{
		 Aggplus::POINT p;
		
		 p.x = drawing_state->custom_verticles[i].x;
		 p.y = drawing_state->custom_verticles[i].y;
		
		 shape->m_oCustomVML.m_arVertices.push_back(p);
	}
	
	for (size_t i = 0 ; i < drawing_state->custom_guides.size(); i++)
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
	
	for (size_t i = 0 ; i < drawing_state->custom_segments.size(); i++)
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
	for (size_t i = 0; i < drawing_state->custom_adjustValues.size(); i++)
	{
		if (drawing_state->custom_adjustValues[i])
		{
			shape->m_oCustomVML.addAdjust(i, *drawing_state->custom_adjustValues[i]);
		}
	}
	if (drawing_state->custom_path >= 0)
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
void xlsx_drawing_context::clear_fill()
{
	if (!current_drawing_states) return;
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->clear_fill();
}
int xlsx_drawing_context::get_fill_type()
{
	if (!current_drawing_states) return 0;
	if (current_drawing_states->empty()) return 0;

	return current_drawing_states->back()->fill.type;
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
					for (size_t i = 0; i < fill.colorsPosition.size(); i++)
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

void xlsx_drawing_context::serialize_anchor (std::wostream & stream, _drawing_state_ptr & drawing_state, std::wstring ns)
{
	CP_XML_WRITER(stream)    
	{
		if (drawing_state->type_anchor == 1)
		{
			if (drawing_state->sheet_anchor.colFrom < 0) drawing_state->sheet_anchor.colFrom = 0;
			if (drawing_state->sheet_anchor.rwFrom < 0) drawing_state->sheet_anchor.rwFrom = 0;

			CP_XML_NODE(ns + L"from")
			{ 
				CP_XML_NODE(L"xdr:col")		{ CP_XML_CONTENT (drawing_state->sheet_anchor.colFrom);	}
				CP_XML_NODE(L"xdr:colOff")	{ CP_XML_CONTENT (drawing_state->sheet_anchor.xFrom) ;	}
				CP_XML_NODE(L"xdr:row")		{ CP_XML_CONTENT (drawing_state->sheet_anchor.rwFrom);	}
				CP_XML_NODE(L"xdr:rowOff")	{ CP_XML_CONTENT (drawing_state->sheet_anchor.yFrom) ;	}
			}
			CP_XML_NODE(ns + L"to")
			{  		
				CP_XML_NODE(L"xdr:col")		{ CP_XML_CONTENT (drawing_state->sheet_anchor.colTo);	}
				CP_XML_NODE(L"xdr:colOff")	{ CP_XML_CONTENT (drawing_state->sheet_anchor.xTo);	}
				CP_XML_NODE(L"xdr:row")		{ CP_XML_CONTENT (drawing_state->sheet_anchor.rwTo);	}
				CP_XML_NODE(L"xdr:rowOff")	{ CP_XML_CONTENT (drawing_state->sheet_anchor.yTo);	}
			}
		}
		if (drawing_state->type_anchor == 3)
		{
			CP_XML_NODE(ns + L"pos")//in emu (1 pt = 12700)
			{ 
				CP_XML_ATTR(L"x", (int)(drawing_state->absolute_anchor.x * 12700));
				CP_XML_ATTR(L"y", (int)(drawing_state->absolute_anchor.y * 12700));
			}
			CP_XML_NODE(ns + L"ext") //in emu (1 pt = 12700)
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
			
			if (drawing_state->rotation != 0)
			{
				if (drawing_state->rotation > 180)
				{
					drawing_state->rotation -= 360.;
				}
				CP_XML_ATTR(L"rot", (int)(drawing_state->rotation * 60000));
			}
			
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
	if (!drawing_state) return;
	if (drawing_state->text.content.empty() && drawing_state->wordart.text.empty()) return;
	
	if (false == drawing_state->xmlTxBodyAlternative.empty())
	{
		stream << drawing_state->xmlTxBodyAlternative;
		return;
	}
	
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"xdr:txBody")
		{  
			CP_XML_NODE(L"a:bodyPr")//todooo rtl
			{
				if (drawing_state->text.wrap == 2 || drawing_state->wordart.is)
					CP_XML_ATTR(L"wrap", L"none" );
				else if (drawing_state->bCustom)
					CP_XML_ATTR(L"wrap", L"square");

				if (false == drawing_state->text.fit_shape)
				{
					CP_XML_ATTR(L"horzOverflow", L"clip");
					CP_XML_ATTR(L"vertOverflow", L"clip");
				}			

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
							//if (drawing_state->wordart.cap)			CP_XML_ATTR(L"cap", *cap);
						
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
							CP_XML_STREAM() << drawing_state->wordart.text;
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

			switch(line.endcap)
			{	
			case 0:		CP_XML_ATTR(L"cap", L"rnd");	break;
			case 1:		CP_XML_ATTR(L"cap", L"sq");		break;
			case 2:		CP_XML_ATTR(L"cap", L"flat");	break;
			}

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
			switch(line.join)
			{	
			case 0:		CP_XML_NODE(L"a:bevel");	break;				
			case 1:
						CP_XML_NODE(L"a:miter")
						{		
							if (line.miter > 0)
								CP_XML_ATTR(L"lim", line.miter * 1000);
						}break;				
			case 2:		CP_XML_NODE(L"a:round");	break;
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

				if (fill.grayscale)
				{
					CP_XML_NODE(L"a:grayscl");
				}
				if (fill.brightness || fill.contrast)
				{
					CP_XML_NODE(L"a:lum")
					{
						if (fill.brightness)CP_XML_ATTR(L"bright", *fill.brightness * 1000);
						if (fill.contrast)	CP_XML_ATTR(L"contrast", *fill.contrast * 1000);
					}
				}
				if (fill.biLevel)
				{
					CP_XML_NODE(L"a:biLevel")
					{
						CP_XML_ATTR(L"thresh", *fill.biLevel * 1000);
					}
				}
			}

			CP_XML_NODE(L"a:srcRect")
			{
				if (fill.texture_crop_enabled)
				{		
					CP_XML_ATTR(L"l", static_cast<long>(fill.texture_crop[0] * 10000));
					CP_XML_ATTR(L"t", static_cast<long>(fill.texture_crop[1] * 10000));
					CP_XML_ATTR(L"r", static_cast<long>(fill.texture_crop[2] * 10000));
					CP_XML_ATTR(L"b", static_cast<long>(fill.texture_crop[3] * 10000));
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

void xlsx_drawing_context::serialize(std::wostream & stream, _drawing_state_ptr & drawing_state)
{
	if		(drawing_state->type_anchor == 0) return;

	std::wstring sNodeAnchor;

	if		(drawing_state->type_anchor == 1)	sNodeAnchor = L"xdr:twoCellAnchor";
	else if (drawing_state->type_anchor == 2)	sNodeAnchor = L"xdr:oneCellAnchor";
	else if (drawing_state->type_anchor == 3)	sNodeAnchor = L"xdr:absoluteAnchor";

	if (sNodeAnchor.empty()) return;
	
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(sNodeAnchor)
		{ 
			//if (drawing_state->anchor.type == 1) CP_XML_ATTR(L"editAs", L"oneCell");

			serialize_anchor(CP_XML_STREAM(), drawing_state);

			if (!drawing_state->xmlAlternative.empty())
			{
				CP_XML_STREAM() << drawing_state->xmlAlternative;
			}
			else
			{
				CP_XML_STREAM() << drawing_state->shape;
			}
			
			CP_XML_NODE(L"xdr:clientData");
		}
	}
}
void xlsx_drawing_context::serialize_control(std::wostream & strm, _drawing_state_ptr & drawing_state)
{
	if (drawing_state->type != external_items::typeActiveX && 
		drawing_state->type != external_items::typeControl) return;

	if (drawing_state->type_anchor == 3)
	{//absolute
		drawing_state->sheet_anchor.colFrom	= 1;
		drawing_state->sheet_anchor.colTo	= 1;
		drawing_state->sheet_anchor.rwFrom	= 0;
		drawing_state->sheet_anchor.rwTo	= 0;
		drawing_state->sheet_anchor.xFrom	= drawing_state->absolute_anchor.x;
		drawing_state->sheet_anchor.yFrom	= drawing_state->absolute_anchor.y;
		drawing_state->sheet_anchor.xTo		= drawing_state->absolute_anchor.x + drawing_state->absolute_anchor.cx;
		drawing_state->sheet_anchor.yTo		= drawing_state->absolute_anchor.y + drawing_state->absolute_anchor.cy;

		drawing_state->type_anchor = 1;
	}
	else if (drawing_state->type_anchor == 0)
	{//child in group
		drawing_state->sheet_anchor.colFrom	= 1;
		drawing_state->sheet_anchor.colTo	= 1;
		drawing_state->sheet_anchor.rwFrom	= 0;
		drawing_state->sheet_anchor.rwTo	= 0;
		drawing_state->sheet_anchor.xFrom	= drawing_state->child_anchor.x;
		drawing_state->sheet_anchor.yFrom	= drawing_state->child_anchor.y;
		drawing_state->sheet_anchor.xTo		= drawing_state->child_anchor.x + drawing_state->child_anchor.cx;
		drawing_state->sheet_anchor.yTo		= drawing_state->child_anchor.y + drawing_state->child_anchor.cy;

		drawing_state->type_anchor = 1;
	}

	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"control")
		{
			CP_XML_ATTR(L"shapeId", drawing_state->id);
			CP_XML_ATTR(L"r:id",	drawing_state->objectId);

			if (!drawing_state->name.empty())
			{
				CP_XML_ATTR(L"name", drawing_state->name);
			}			

			CP_XML_NODE(L"controlPr")
			{
				CP_XML_ATTR(L"defaultSize", 0);
				CP_XML_ATTR(L"autoLine", drawing_state->object.bAutoLine);
				CP_XML_ATTR(L"autoPict", drawing_state->object.bAutoPict);
				
				if (!drawing_state->object.link.empty())
				{
					CP_XML_ATTR(L"linkedCell", drawing_state->object.link);
				}
				if (!drawing_state->object.fmlaRange.empty())
				{
					CP_XML_ATTR(L"listFillRange", drawing_state->object.fmlaRange);
				}				
				if (!drawing_state->fill.picture_target.empty())
					drawing_state->fill.texture_target = drawing_state->fill.picture_target;
				if (!drawing_state->fill.texture_target.empty())
				{
					bool isIternal = false;
					std::wstring rId = handle_.impl_->get_mediaitems().find_image( drawing_state->fill.texture_target, isIternal);
					
					CP_XML_ATTR(L"r:id", rId);
		
					sheet_rels_->add(isIternal, rId , drawing_state->fill.texture_target, external_items::typeImage);
				}

				CP_XML_NODE(L"anchor")
				{
					CP_XML_ATTR(L"moveWithCells", 1);

					serialize_anchor(CP_XML_STREAM(), drawing_state, L"");
				}
			}
		}
	}
}
void xlsx_drawing_context::serialize_control_props(std::wostream & strm, _drawing_state_ptr & drawing_state)
{
	if (drawing_state == NULL) return;
	
	if (drawing_state->type != external_items::typeActiveX && 
		drawing_state->type != external_items::typeControl) return;

	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"formControlPr")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main" );

			switch(drawing_state->type_control)
			{
				case 0x0000: CP_XML_ATTR(L"objectType", L"Group");		break;
				case 0x0001: CP_XML_ATTR(L"objectType", L"Shape");		break; // Line
				case 0x0002: CP_XML_ATTR(L"objectType", L"Rect");		break;
				case 0x0003: CP_XML_ATTR(L"objectType", L"Shape");		break; // Oval
				case 0x0004: CP_XML_ATTR(L"objectType", L"Shape");		break; // Arc
				case 0x0006: CP_XML_ATTR(L"objectType", L"Shape");		break; // Text
				case 0x0007: CP_XML_ATTR(L"objectType", L"Button");		break;
				case 0x0008: CP_XML_ATTR(L"objectType", L"Pict");		break;
				case 0x0009: CP_XML_ATTR(L"objectType", L"Shape");		break; // Polygon:			
				case 0x000B: CP_XML_ATTR(L"objectType", L"CheckBox");	break;
				case 0x000C: CP_XML_ATTR(L"objectType", L"Radio");		break;
				case 0x000D: CP_XML_ATTR(L"objectType", L"Edit");		break;
				case 0x000E: CP_XML_ATTR(L"objectType", L"Label");		break;
				case 0x000F: CP_XML_ATTR(L"objectType", L"Dialog");		break;
				case 0x0010: CP_XML_ATTR(L"objectType", L"Spin");		break;
				case 0x0012: CP_XML_ATTR(L"objectType", L"List");		break;
				case 0x0013: CP_XML_ATTR(L"objectType", L"GBox");		break;
				case 0x0011: CP_XML_ATTR(L"objectType", L"Scroll");		break;
				case 0x0014: CP_XML_ATTR(L"objectType", L"Drop");		break;
				case 0x001E: CP_XML_ATTR(L"objectType", L"Shape");		break; // OfficeArt object
				case 0x0019: CP_XML_ATTR(L"objectType", L"Note");		break;
				default: break;
			}
			if (drawing_state->object.drop_style)
			{
				switch(*drawing_state->object.drop_style)
				{
				case 0:
				default:
					CP_XML_ATTR(L"dropStyle", L"combo");
				}
			}
			if (!drawing_state->object.link.empty() )
			{
				CP_XML_ATTR(L"fmlaLink", drawing_state->object.link);
			}
			if (!drawing_state->object.fmlaRange.empty() )
			{
				CP_XML_ATTR(L"fmlaRange", drawing_state->object.fmlaRange);
			}	
			if (drawing_state->object.x_min)
			{
				CP_XML_ATTR(L"min", *drawing_state->object.x_min);
			}
			if (drawing_state->object.x_max)
			{
				CP_XML_ATTR(L"max", *drawing_state->object.x_max);
			}
			if (drawing_state->object.x_inc)
			{
				CP_XML_ATTR(L"inc", *drawing_state->object.x_inc);
			}
			if (drawing_state->object.x_page)
			{
				CP_XML_ATTR(L"page", *drawing_state->object.x_page);
			}
			if (drawing_state->object.x_sel)
			{
				CP_XML_ATTR(L"sel", *drawing_state->object.x_sel);
			}
			if (drawing_state->object.x_val)
			{
				CP_XML_ATTR(L"val", *drawing_state->object.x_val);
			}
			if (drawing_state->object.drop_lines)
			{
				CP_XML_ATTR(L"dropLines", *drawing_state->object.drop_lines);
			}
		}
	}
}

void xlsx_drawing_context::serialize_object(std::wostream & stream, _drawing_state_ptr & drawing_state)
{
	if (drawing_state->type != external_items::typeOleObject) return;

	if (drawing_state->type_anchor == 3)
	{//absolute
		drawing_state->sheet_anchor.colFrom	= 1;
		drawing_state->sheet_anchor.colTo	= 1;
		drawing_state->sheet_anchor.rwFrom	= 0;
		drawing_state->sheet_anchor.rwTo	= 0;
		drawing_state->sheet_anchor.xFrom	= drawing_state->absolute_anchor.x;
		drawing_state->sheet_anchor.yFrom	= drawing_state->absolute_anchor.y;
		drawing_state->sheet_anchor.xTo		= drawing_state->absolute_anchor.x + drawing_state->absolute_anchor.cx;
		drawing_state->sheet_anchor.yTo		= drawing_state->absolute_anchor.y + drawing_state->absolute_anchor.cy;

		drawing_state->type_anchor = 1;
	}
	else if (drawing_state->type_anchor == 0)
	{//child in group
		drawing_state->sheet_anchor.colFrom	= 1;
		drawing_state->sheet_anchor.colTo	= 1;
		drawing_state->sheet_anchor.rwFrom	= 0;
		drawing_state->sheet_anchor.rwTo	= 0;
		drawing_state->sheet_anchor.xFrom	= drawing_state->child_anchor.x;
		drawing_state->sheet_anchor.yFrom	= drawing_state->child_anchor.y;
		drawing_state->sheet_anchor.xTo		= drawing_state->child_anchor.x + drawing_state->child_anchor.cx;
		drawing_state->sheet_anchor.yTo		= drawing_state->child_anchor.y + drawing_state->child_anchor.cy;

		drawing_state->type_anchor = 1;
	}

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"oleObject")
		{
			if (!drawing_state->objectProgId.empty())
			{
				CP_XML_ATTR(L"progId",	drawing_state->objectProgId);
			}			
			CP_XML_ATTR(L"shapeId", drawing_state->id);
			CP_XML_ATTR(L"r:id",	drawing_state->objectId);

			CP_XML_NODE(L"objectPr")
			{
				CP_XML_ATTR(L"defaultSize", 0);
				//CP_XML_ATTR(L"autoPict", 0);
				if (!drawing_state->fill.picture_target.empty())
					drawing_state->fill.texture_target = drawing_state->fill.picture_target;
				
				if (!drawing_state->fill.texture_target.empty())
				{
					bool isIternal = false;
					std::wstring rId = handle_.impl_->get_mediaitems().find_image( drawing_state->fill.texture_target, isIternal);
					
					CP_XML_ATTR(L"r:id", rId);
		
					sheet_rels_->add(isIternal, rId , drawing_state->fill.texture_target, external_items::typeImage);
				}

				CP_XML_NODE(L"anchor")
				{
					CP_XML_ATTR(L"moveWithCells", 1);

					serialize_anchor(CP_XML_STREAM(), drawing_state, L"");
				}
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
void xlsx_drawing_context::set_name(const std::wstring & str)
{
	if (current_drawing_states == NULL) return;
	if (str.empty()) return;
	
	current_drawing_states->back()->name = str;
}
void xlsx_drawing_context::set_description(const std::wstring & str)
{
	if (current_drawing_states == NULL) return;	
	if (str.empty()) return;

	current_drawing_states->back()->description = str;
}
void xlsx_drawing_context::set_macro(const std::wstring & str)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->object.macro = str;
}
void xlsx_drawing_context::set_ole_object(const std::wstring & id, const std::wstring & info)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->type = external_items::typeOleObject;
	
	current_drawing_states->back()->objectId		= id;
	current_drawing_states->back()->objectProgId	= info;
}
void xlsx_drawing_context::set_control_activeX(const std::wstring & rid)
{
	if (current_drawing_states == NULL) return;	

	current_drawing_states->back()->type = external_items::typeActiveX;
	current_drawing_states->back()->objectId = rid;
}
void xlsx_drawing_context::set_sheet_anchor(int colFrom, int xFrom, int rwFrom, int yFrom, int colTo, int xTo, int rwTo, int yTo,
											double x, double y, double cx, double cy)
{
	if (current_drawing_states == NULL) return;	
	
	if (colTo > 0 || xTo > 0 || rwTo > 0 || yTo > 0)
	{//  0  in comment old versions
		current_drawing_states->back()->sheet_anchor.colFrom = colFrom;
		current_drawing_states->back()->sheet_anchor.colTo = colTo;
		current_drawing_states->back()->sheet_anchor.rwFrom = rwFrom;
		current_drawing_states->back()->sheet_anchor.rwTo = rwTo;
		current_drawing_states->back()->sheet_anchor.xFrom = (std::min)(xFrom, xTo);
		current_drawing_states->back()->sheet_anchor.yFrom = (std::min)(yFrom, yTo);
		current_drawing_states->back()->sheet_anchor.xTo = (std::max)(xFrom, xTo);
		current_drawing_states->back()->sheet_anchor.yTo = (std::max)(yFrom, yTo);
	}

	current_drawing_states->back()->sheet_anchor.absolute.x = x;
	current_drawing_states->back()->sheet_anchor.absolute.y = y;
	current_drawing_states->back()->sheet_anchor.absolute.cx = cx;
	current_drawing_states->back()->sheet_anchor.absolute.cy = cy;

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

	current_drawing_states->back()->absolute_anchor.x	= (int)x;
	current_drawing_states->back()->absolute_anchor.y	= (int)y;
	current_drawing_states->back()->absolute_anchor.cx	= (int)cx;
	current_drawing_states->back()->absolute_anchor.cy	= (int)cy;

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
	if (false == current_drawing_states->back()->fill.texture_target.empty()) return;

	current_drawing_states->back()->fill.texture_target = str;

	if (current_drawing_states->back()->fill.type == fillUndefined)
	{
		current_drawing_states->back()->fill.type = fillTexture;
		current_drawing_states->back()->fill.texture_mode = textureStretch;
	}
}
void xlsx_drawing_context::set_picture(const std::wstring & str)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;

	if (str.empty()) return;
	if (false == current_drawing_states->back()->fill.picture_target.empty()) return;

	current_drawing_states->back()->fill.picture_target = str;

	if (current_drawing_states->back()->fill.type == fillUndefined)
	{
		current_drawing_states->back()->fill.type = fillTexture;
		current_drawing_states->back()->fill.texture_mode = textureStretch;
	}
}
void xlsx_drawing_context::set_picture_crop_top	(double val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;
	
	current_drawing_states->back()->fill.texture_crop[1] = val;
    current_drawing_states->back()->fill.texture_crop_enabled = true;
}
void xlsx_drawing_context::set_picture_crop_bottom(double val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->fill.texture_crop[3] = val;
    current_drawing_states->back()->fill.texture_crop_enabled = true;
}
void xlsx_drawing_context::set_picture_crop_left (double val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->fill.texture_crop[0] = val;
    current_drawing_states->back()->fill.texture_crop_enabled = true;
}
void xlsx_drawing_context::set_picture_crop_right (double val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->fill.texture_crop[2] = val;
    current_drawing_states->back()->fill.texture_crop_enabled = true;
}
void xlsx_drawing_context::set_picture_name(const std::wstring & str)
{
	//....
}
void xlsx_drawing_context::set_picture_grayscale(bool val)
{
    current_drawing_states->back()->fill.grayscale = val;
}
void xlsx_drawing_context::set_picture_brightness(int val)
{
    current_drawing_states->back()->fill.brightness = val;
}
void xlsx_drawing_context::set_picture_contrast(int val)
{
    current_drawing_states->back()->fill.contrast = val;
}
void xlsx_drawing_context::set_picture_biLevel(int val)
{
    current_drawing_states->back()->fill.biLevel = val;
}
void xlsx_drawing_context::set_picture_transparent(int nColor, const std::wstring & sColor)
{
	//....
}
void xlsx_drawing_context::set_shadow_enabled(bool val)
{
	current_drawing_states->back()->shadow.enabled = val;
}
void xlsx_drawing_context::set_shadow_type(int val)
{
    current_drawing_states->back()->shadow.type = val;
}
void xlsx_drawing_context::set_shadow_opacity(double val)
{
    current_drawing_states->back()->shadow.opacity = val;
}
void xlsx_drawing_context::set_shadow_color(int nColor, const std::wstring & sColor)
{
	current_drawing_states->back()->shadow.color.sRGB = sColor;
	current_drawing_states->back()->shadow.color.nRGB = nColor;
}
void xlsx_drawing_context::set_shadow_highlight(int nColor, const std::wstring & sColor)
{
	current_drawing_states->back()->shadow.highlight.sRGB = sColor;
	current_drawing_states->back()->shadow.highlight.nRGB = nColor;
}
void xlsx_drawing_context::set_shadow_originX(double val)
{
    current_drawing_states->back()->shadow.originX = val;
}
void xlsx_drawing_context::set_shadow_originY(double val)
{
    current_drawing_states->back()->shadow.originY = val;
}
void xlsx_drawing_context::set_shadow_offsetX(int val)
{
    current_drawing_states->back()->shadow.offsetX = val;
}
void xlsx_drawing_context::set_shadow_offsetY(int val)
{
    current_drawing_states->back()->shadow.offsetY = val;
}
void xlsx_drawing_context::set_shadow_scaleX2X(double val)
{
    current_drawing_states->back()->shadow.scaleX2X = val;
}
void xlsx_drawing_context::set_shadow_scaleY2Y(double val)
{
    current_drawing_states->back()->shadow.scaleY2Y = val;
}
void xlsx_drawing_context::set_rotation (double val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	val -= (int)(val / 360.) * 360;

	if (current_drawing_states->back()->flipH && current_drawing_states->back()->flipV/* && current_level > 0*/)
	{
		val = 180 - val;
	}
	else if (current_drawing_states->back()->flipH/* && current_level > 0*/)
	{
		val = 180 + val;
	}
	else if (current_drawing_states->back()->flipV/* && current_level > 0*/)
	{
		val = 360 - val;
	}
	val -= (int)(val / 360.) * 360;
	//if (val > 360) val -= 360;

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
		else if (index > 64 && index < 92)
		{
			color.nRGB = controlPanelColors1[index - 65];
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
		else if (index > 64 && index < 92)
		{
			color.nRGB = background ? controlPanelColors2[index - 65] : controlPanelColors1[index - 65];
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
void xlsx_drawing_context::set_fill_type (_fill_type val)
{
	if (current_drawing_states == NULL) return;
	if (current_drawing_states->empty()) return;
	
	current_drawing_states->back()->fill.type = val;
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
void xlsx_drawing_context::set_line_miter(int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->line.miter = val;
}
void xlsx_drawing_context::set_line_join(int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->line.join = val;
}
void xlsx_drawing_context::set_line_endcap(int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->line.endcap = val;
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

	current_drawing_states->back()->line.arrow.start	= val;
	current_drawing_states->back()->line.arrow.enabled	= true;
}
void xlsx_drawing_context::set_arrow_end (int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->line.arrow.end = val;
	current_drawing_states->back()->line.arrow.enabled	= true;
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
			set_fill_type(oox::fillNone);
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

	std::wstring sId			= std::wstring(L"hId") + std::to_wstring(hlinks_.size()+1);
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
void xlsx_drawing_context::set_text_vml (const std::wstring & text)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->text.vml_content = text;
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
void xlsx_drawing_context::set_custom_connection(std::vector<ODRAW::MSOPOINT>& points)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->custom_connection = points;
}
void xlsx_drawing_context::set_custom_connectionDir(std::vector<OSHARED::FixedPoint>& points)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->custom_connectionDir = points;
}
void xlsx_drawing_context::set_custom_inscribe(std::vector<ODRAW::MSORECT>& rects)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->custom_inscribe = rects;
}
void xlsx_drawing_context::set_custom_x_limo(int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->custom_x_limo = val;
}
void xlsx_drawing_context::set_custom_y_limo(int val)
{
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->custom_y_limo = val;
}
//----------------------------------------------------------------------------------------------------------
bool xlsx_drawing_context::get_mode_HF ()
{//comment, shapes in header/footer, ....
	if (current_drawing_states == NULL) return false;	
	if (current_drawing_states->empty()) return false;

	return current_drawing_states->back()->vml_HF_mode_;
}
void xlsx_drawing_context::set_mode_HF (bool val)
{//comment, shapes in header/footer, ....
	if (current_drawing_states == NULL) return;	
	if (current_drawing_states->empty()) return;

	current_drawing_states->back()->vml_HF_mode_ = val;
}
//----------------------------------------------------------------------------------------------------------
bool xlsx_drawing_context::empty()
{
	return drawing_states.empty();
}
bool xlsx_drawing_context::empty_vml()
{
	return drawing_states_vml.empty();
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
xlsx_drawings_rels_ptr xlsx_drawing_context::get_vml_rels()
{
    return vml_rels_;
}
xlsx_drawings_rels_ptr xlsx_drawing_context::get_sheet_rels()
{
    return sheet_rels_;
}
bool xlsx_drawing_context::ChangeBlack2ColorImage(std::wstring sRgbColor1, std::wstring sRgbColor2, _drawing_state_ptr & drawing_state)
{
	if (drawing_state->fill.texture_target.length() < 6) return false;

	std::wstring image_path = context_.get_mediaitems().media_path() + drawing_state->fill.texture_target.substr(6);

	size_t rgbColor1 = STR::hex_str2int(sRgbColor1);
	size_t rgbColor2 = STR::hex_str2int(sRgbColor2);

	CBgraFrame bgraFrame;

	return bgraFrame.ReColorPatternImage(image_path, rgbColor1, rgbColor2);
}
void xlsx_drawing_context::serialize_objects(std::wostream & strm) 
{
	for (size_t i = 0; i < drawing_states_objects.size(); i++)
	{
		serialize_object(strm, drawing_states_objects[i]);
	}
	drawing_states_objects.clear();
}
void xlsx_drawing_context::serialize_controls(std::wostream & strm) 
{
	for (size_t i = 0; i < drawing_states_controls.size(); i++)
	{
		serialize_control(strm, drawing_states_controls[i]);
	}
	drawing_states_controls.clear();
}
//-------------------------------------------------------------------------------------------------------------------
void xlsx_drawing_context::serialize_vml_HF(std::wostream & strm) 
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"xml")
		{
            CP_XML_ATTR(L"xmlns:v"	, L"urn:schemas-microsoft-com:vml");
            CP_XML_ATTR(L"xmlns:o"	, L"urn:schemas-microsoft-com:office:office");
            CP_XML_ATTR(L"xmlns:x"	, L"urn:schemas-microsoft-com:office:excel");

			for (size_t i = 0 ; i < drawing_states_vml_HF.size(); i++)
			{
				CP_XML_STREAM() << drawing_states_vml_HF[i]->vml_shape;
			}

		}
    }
}
void xlsx_drawing_context::serialize_vml(std::wostream & strm) 
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"xml")
		{
            CP_XML_ATTR(L"xmlns:v"	, L"urn:schemas-microsoft-com:vml");
            CP_XML_ATTR(L"xmlns:o"	, L"urn:schemas-microsoft-com:office:office");
            CP_XML_ATTR(L"xmlns:x"	, L"urn:schemas-microsoft-com:office:excel");

			for (size_t i = 0 ; i < drawing_states_vml.size(); i++)
			{
				CP_XML_STREAM() << drawing_states_vml[i]->vml_shape;
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
			CP_XML_ATTR(L"xmlns:a14", L"http://schemas.microsoft.com/office/drawing/2010/main");

			for (size_t i = 0 ; i < drawing_states.size(); i++)
			{
				serialize(CP_XML_STREAM(), drawing_states[i]);
			}

		}
    }
}
}
