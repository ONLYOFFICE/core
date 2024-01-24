/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "PptxConverter.h"
#include "../../Common/utils.h"

#include "../../../OOXML/PPTXFormat/Folder.h"
#include "../../../OOXML/PPTXFormat/Presentation.h"
#include "../../../OOXML/PPTXFormat/Slide.h"
#include "../../../OOXML/PPTXFormat/SlideMaster.h"
#include "../../../OOXML/PPTXFormat/SlideLayout.h"
#include "../../../OOXML/PPTXFormat/NotesMaster.h"
#include "../../../OOXML/PPTXFormat/NotesSlide.h"
#include "../../../OOXML/PPTXFormat/TableStyles.h"
#include "../../../OOXML/PPTXFormat/App.h"
#include "../../../OOXML/PPTXFormat/Core.h"

#include "../../../OOXML/PPTXFormat/Logic/Table/Table.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/Par.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/Seq.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/CTn.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/Set.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/CBhvr.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/AnimEffect.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/AnimMotion.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/AnimClr.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/AnimRot.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/AnimScale.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/Anim.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/Audio.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/Timing.h"

#include "../../../OOXML/PPTXFormat/Logic/TcBdr.h"
#include "../../../OOXML/PPTXFormat/Logic/TablePartStyle.h"
#include "../../../OOXML/PPTXFormat/Logic/CxnSp.h"
#include "../../../OOXML/PPTXFormat/Logic/Shape.h"
#include "../../../OOXML/PPTXFormat/Logic/TxStyles.h"
#include "../../../OOXML/PPTXFormat/Logic/ClrMapOvr.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/Transition.h"

#include "../../../OOXML/PPTXFormat/Logic/Transitions/EmptyTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/OrientationTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/EightDirectionTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/OptionalBlackTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/SideDirectionTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/CornerDirectionTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/WheelTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/SplitTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/ZoomTransition.h"

#include "../../../OOXML/PPTXFormat/Presentation/SldSz.h"
#include "../../../OOXML/PPTXFormat/Presentation/NotesSz.h"

#include "../Format/odp_conversion_context.h"

#include "../Format/odf_text_context.h"
#include "../Format/odf_drawing_context.h"
#include "../Format/office_event_listeners.h"

#include "../Format/styles.h"
#include "../Format/style_presentation.h"
#include "../Format/style_page_layout_properties.h"
#include "../Format/style_table_properties.h"
#include "../Format/style_text_properties.h"
#include "../Format/style_paragraph_properties.h"
#include "../Format/style_graphic_properties.h"
#include "../Format/styles_list.h"

using namespace cpdoccore;

namespace Oox2Odf
{

PptxConverter::PptxConverter(const std::wstring & path, bool bTemplate)
{
 	current_clrMap		= NULL;
	current_slide		= NULL;
	current_theme		= NULL;

	presentation		= NULL;
	output_document		= NULL;
	odp_context			= NULL;

	const OOX::CPath oox_path(std::wstring(path.c_str()));

	pptx_document = new PPTX::Document();
	if (!pptx_document->isValid(oox_path.GetPath())) // true ???
	{
		delete pptx_document; pptx_document = NULL;
		return;
	}

	pptx_document->read(oox_path.GetPath() + FILE_SEPARATOR_STR);

	smart_ptr<PPTX::Presentation> presentation_ptr = pptx_document->Get(OOX::Presentation::FileTypes::Presentation).smart_dynamic_cast<PPTX::Presentation>();
	if (!presentation_ptr.is_init())
	{
		delete pptx_document;  pptx_document = NULL;
		return;
	}
	presentation = presentation_ptr.GetPointer();
	
	output_document = new odf_writer::package::odf_document(L"presentation", bTemplate);
    odp_context     = new odf_writer::odp_conversion_context(output_document);
}
PptxConverter::~PptxConverter()
{
	current_clrMap		= NULL;
	current_slide		= NULL;
	current_theme		= NULL;
	presentation		= NULL;

	if (odp_context)		delete odp_context;		odp_context		= NULL;
	if (pptx_document)		delete pptx_document;	pptx_document	= NULL;
	if (output_document)	delete output_document;	output_document = NULL;
}
odf_writer::odf_conversion_context* PptxConverter::odf_context()
{
	return odp_context;
}
PPTX::Theme* PptxConverter::oox_theme()
{
	return current_theme;
}
PPTX::Logic::ClrMap* PptxConverter::oox_clrMap()
{
	return current_clrMap;
}
OOX::IFileContainer* PptxConverter::current_document()
{
	if (oox_current_child_document)
		return oox_current_child_document;
	else if (current_slide)
		return current_slide;
	else
		return pptx_document;
}
NSCommon::smart_ptr<OOX::File> PptxConverter::find_file_by_id(const std::wstring & sId)
{
	smart_ptr<OOX::File> oFile;
	if (pptx_document)
	{
		if (oox_current_child_document)
			oFile = oox_current_child_document->Find(sId);
		else if (current_slide)
			oFile = current_slide->Find(sId);
		else
			oFile = pptx_document->Find(sId);
	}
		
	return oFile;
}

std::wstring PptxConverter::find_link_by_id (const std::wstring & sId, int type, bool & bExternal)
{
	bExternal = false;

	if(!pptx_document) return L"";

    std::wstring			ref;
	smart_ptr<OOX::File>	oFile;

	if (oox_current_child_document)
	{
		oFile	= oox_current_child_document->Find(sId);
		ref		= OoxConverter::find_link_by(oFile, type, bExternal);
	}
	if (!ref.empty()) return ref;

	oFile	= current_slide ? current_slide->Find(sId) : pptx_document->Find(sId);
	ref		= OoxConverter::find_link_by(oFile, type, bExternal);

	return ref;
}

bool PptxConverter::convertDocument()
{
    if (!pptx_document) return false;
    if (!odp_context)   return false;
		
	odp_context->start_document();

	convert_styles();
	convert_settings(); 
	
	smart_ptr<PPTX::App> app_ptr = pptx_document->Get(OOX::FileTypes::App).smart_dynamic_cast<PPTX::App>();
	smart_ptr<PPTX::Core> core_ptr = pptx_document->Get(OOX::FileTypes::Core).smart_dynamic_cast<PPTX::Core>();

	//convert_meta(app_ptr.GetPointer(), core_ptr.GetPointer()); -> привести к OOX::...

	convert_masters_and_layouts();
	convert_slides();

	//удалим уже ненужный документ pptx 
	delete pptx_document; pptx_document = NULL;

	odp_context->end_document();

	return true;
}
void PptxConverter::convert_styles()
{
	if (presentation->sldIdLst.empty()) return;

	std::wstring rId = presentation->sldIdLst[0].rid.get();
	smart_ptr<PPTX::Slide> slide = ((*presentation)[rId]).smart_dynamic_cast<PPTX::Slide>();

	if (slide.IsInit() == false) return;

	odp_context->styles_context()->create_default_style(odf_types::style_family::Graphic);					
	
	odf_writer::paragraph_format_properties	* paragraph_properties	= odp_context->styles_context()->last_state()->get_paragraph_properties();
	odf_writer::text_format_properties		* text_properties		= odp_context->styles_context()->last_state()->get_text_properties();

	if (presentation->defaultTextStyle.IsInit())
	{
		OoxConverter::convert(presentation->defaultTextStyle->levels[9].GetPointer(), paragraph_properties, text_properties); //defPPr
		OoxConverter::convert(presentation->defaultTextStyle->levels[0].GetPointer(), paragraph_properties, text_properties); //default text
		
		odp_context->styles_context()->create_default_style(odf_types::style_family::Paragraph);		
		paragraph_properties	= odp_context->styles_context()->last_state()->get_paragraph_properties();
		
		OoxConverter::convert(presentation->defaultTextStyle->levels[0].GetPointer(), paragraph_properties, text_properties); //default text
		
		odp_context->styles_context()->create_default_style(odf_types::style_family::Text);
		text_properties			= odp_context->styles_context()->last_state()->get_text_properties();

		OoxConverter::convert(presentation->defaultTextStyle->levels[0].GetPointer(), paragraph_properties, text_properties); //default text
	}
	//convert(presentation->defaultTextStyle.GetPointer()); //стили дефалтовых списков
	
///////////////////////////////////////////////////////////////////////////

	odp_context->styles_context()->create_default_style(odf_types::style_family::Table);					
	odf_writer::style_table_properties	* table_properties	= odp_context->styles_context()->last_state()->get_table_properties();
	//для красивой отрисовки в редакторах - разрешим объеденить стили пересекающихся обрамлений 
	table_properties->content_.table_border_model_ = odf_types::border_model(odf_types::border_model::Collapsing);

	odp_context->styles_context()->create_default_style(odf_types::style_family::TableRow);					
	odf_writer::style_table_row_properties	* row_properties	= odp_context->styles_context()->last_state()->get_table_row_properties();
	row_properties->style_table_row_properties_attlist_.fo_keep_together_ = odf_types::keep_together(odf_types::keep_together::Auto);

/////////////////////////////////////////////////////////////////////////////////////////////////
	//зачемто ?! для OpenOffice для врезок/фреймов нужен базовый стиль - без него другой тип геометрии oO !!!
	odp_context->styles_context()->create_style(L"Frame", odf_types::style_family::Graphic, false, true);		
	
	odf_writer::graphic_format_properties	* frame_graphic_properties	= odp_context->styles_context()->last_state()->get_graphic_properties();
	odf_writer::text_format_properties		* frame_text_properties		= odp_context->styles_context()->last_state()->get_text_properties();

	if (frame_text_properties && text_properties)
		frame_text_properties->apply_from(*text_properties);

	odp_context->page_layout_context()->create_layer_sets();
}



void PptxConverter::convert_settings()
{
	OoxConverter::convert(presentation->m_pJsaProject.operator ->());
}
void PptxConverter::convert(PPTX::TableStyles *oox_table_styles)
{
	if (!oox_table_styles) return;

	for (std::map<std::wstring, PPTX::Logic::TableStyle>::iterator it = oox_table_styles->Styles.begin(); it != oox_table_styles->Styles.end(); ++it)
	{
		convert(it->first, &it->second);
	}
}
void PptxConverter::convert(const std::wstring &id, PPTX::Logic::TableStyle *oox_table_style)
{
	if (!oox_table_style) return;
	if (id.empty()) return;

	odf_writer::table_style_state state;

	state.default_		= convert(oox_table_style->wholeTbl.GetPointer());
	state.first_row_	= convert(oox_table_style->firstRow.GetPointer(), state.default_);
	state.band_row_		= convert(oox_table_style->band1H.GetPointer(), state.default_);
	state.last_row_		= convert(oox_table_style->lastRow.GetPointer(), state.default_);
	
	state.first_col_	= convert(oox_table_style->firstCol.GetPointer());
	state.band_col_		= convert(oox_table_style->band1V.GetPointer());
	state.last_col_		= convert(oox_table_style->lastCol.GetPointer());

	odp_context->map_table_styles_.insert(std::make_pair(id, state));
}

std::wstring PptxConverter::convert(PPTX::Logic::TablePartStyle* style, const std::wstring &base)
{
	if (!style) return L"";
	
	odp_context->styles_context()->create_style(L"", odf_types::style_family::TableCell, true, false, -1);
	odf_writer::odf_style_state_ptr style_state = odp_context->styles_context()->last_state(style_family::TableCell);
	if (!style_state) return L"";

	if (false == base.empty())
	{
		odf_writer::odf_style_state_ptr style_state_base;
		if (odp_context->styles_context()->find_odf_style_state(base, style_family::TableCell, style_state_base))
		{
			odf_writer::graphic_format_properties *gr = style_state->get_graphic_properties();
			odf_writer::graphic_format_properties *gr_base = style_state_base->get_graphic_properties();

			if (gr && gr_base) gr->apply_from(*gr_base);

			odf_writer::paragraph_format_properties *para = style_state->get_paragraph_properties();
			odf_writer::paragraph_format_properties *para_base = style_state_base->get_paragraph_properties();
			
			if (para && para_base) para->apply_from(*para_base);
		}
	}

	if (style->tcStyle.IsInit())
	{
		odp_context->drawing_context()->start_area_properties();
			convert(style->tcStyle.GetPointer(), style_state->get_graphic_properties());
		odp_context->drawing_context()->end_area_properties();
		
		odp_context->drawing_context()->start_line_properties();
			convert(style->tcStyle->tcBdr.GetPointer(), style_state->get_paragraph_properties());
		odp_context->drawing_context()->end_line_properties();		

		convert(style_state->get_graphic_properties(), style_state->get_table_cell_properties());
		convert(style_state->get_paragraph_properties(), style_state->get_table_cell_properties());
	}
	convert(style->tcTxStyle.GetPointer(), style_state->get_text_properties());

	return style_state->get_name();
}
void PptxConverter::convert(PPTX::Logic::TcStyle* style, odf_writer::graphic_format_properties *properties)
{
	if (!style) return;
	if (!properties) return;

	odp_context->drawing_context()->set_no_fill();

	if (style->fill.IsInit())
	{
		OoxConverter::convert(&style->fill->Fill);
	}
	else if (style->fillRef.IsInit())
	{
		OoxConverter::convert(style->fillRef.GetPointer());
	}

	if (style->cell3D.IsInit())
	{
	}
	odf_writer::graphic_format_properties *temp_props = odp_context->drawing_context()->get_graphic_properties();
	
	if (temp_props)
		properties->apply_from(*temp_props);

}

void PptxConverter::convert(PPTX::Logic::TcTxStyle* style, odf_writer::text_format_properties *text_properties)
{
	if (!style) return;
	if (!text_properties) return;
	
//nullable_limit<Limit::OnOff> i;
//nullable_limit<Limit::OnOff> b;
//nullable<FontRef> fontRef;
//UniColor Color;
}

void PptxConverter::convert(const PPTX::Limit::TLNodeType& oox_note_type)
{
	odf_types::presentation_node_type odfNodeType;

	switch (oox_note_type.GetBYTECode())
	{
	case 0:
	case 1: 
		odfNodeType = odf_types::presentation_node_type::after_previous; 
		break;
	case 2:
	case 3:
		odfNodeType = odf_types::presentation_node_type::on_click;
		break;
	case 4:
		odfNodeType = odf_types::presentation_node_type::interactive_sequence;
		break;
	case 5:
		odfNodeType = odf_types::presentation_node_type::main_sequence;
		break;
	case 6:
		odfNodeType = odf_types::presentation_node_type::timing_root;
		break;
	case 7:
	case 8:
		odfNodeType = odf_types::presentation_node_type::with_previous;
		break;
	default:
		odfNodeType = odf_types::presentation_node_type::on_click;
	}

	odp_context->current_slide().set_anim_type(odfNodeType);
}

void PptxConverter::convert(const PPTX::Limit::TLPresetClass& oox_preset_class)
{
	odf_types::preset_class odfPresetClass;

	switch (oox_preset_class.GetBYTECode())
	{
	case 0: odfPresetClass = odf_types::preset_class::emphasis; break;
	case 1: odfPresetClass = odf_types::preset_class::entrance; break;
	case 2: odfPresetClass = odf_types::preset_class::exit; break;
	case 3: odfPresetClass = odf_types::preset_class::media_call; break;
	case 4: odfPresetClass = odf_types::preset_class::motion_path; break;
	default: odfPresetClass = odf_types::preset_class::custom; break;
	}

	odp_context->current_slide().set_anim_preset_class(odfPresetClass);
}

void PptxConverter::convert(const PPTX::Limit::TLPresetClass& oox_preset_class, int preset_id)
{
	odf_types::preset_id odfPresetId = preset_id::type::none;

	switch (oox_preset_class.GetBYTECode())
	{
		case 0: // "emph"
		{
			switch (preset_id)
			{
			case 1: 	odfPresetId = preset_id::type::ooo_emphasis_fill_color				; break;
			case 2: 	odfPresetId = preset_id::type::ooo_emphasis_font					; break;
			case 3: 	odfPresetId = preset_id::type::ooo_emphasis_font_color				; break;
			case 4: 	odfPresetId = preset_id::type::ooo_emphasis_font_size				; break;
			case 5: 	odfPresetId = preset_id::type::ooo_emphasis_font_style				; break;
			case 6: 	odfPresetId = preset_id::type::ooo_emphasis_grow_and_shrink			; break;
			case 7: 	odfPresetId = preset_id::type::ooo_emphasis_line_color				; break;
			case 8: 	odfPresetId = preset_id::type::ooo_emphasis_spin					; break;
			case 9: 	odfPresetId = preset_id::type::ooo_emphasis_transparency			; break;
			case 10: 	odfPresetId = preset_id::type::ooo_emphasis_bold_flash				; break;
			case 14: 	odfPresetId = preset_id::type::ooo_emphasis_blast					; break;
			case 15: 	odfPresetId = preset_id::type::ooo_emphasis_bold_reveal				; break;
			case 16: 	odfPresetId = preset_id::type::ooo_emphasis_color_over_by_word		; break;
			case 18: 	odfPresetId = preset_id::type::ooo_emphasis_reveal_underline		; break;
			case 19: 	odfPresetId = preset_id::type::ooo_emphasis_color_blend				; break;
			case 20: 	odfPresetId = preset_id::type::ooo_emphasis_color_over_by_letter	; break;
			case 21: 	odfPresetId = preset_id::type::ooo_emphasis_complementary_color		; break;
			case 22: 	odfPresetId = preset_id::type::ooo_emphasis_complementary_color_2	; break;
			case 23: 	odfPresetId = preset_id::type::ooo_emphasis_contrasting_color		; break;
			case 24: 	odfPresetId = preset_id::type::ooo_emphasis_darken					; break;
			case 25: 	odfPresetId = preset_id::type::ooo_emphasis_desaturate				; break;
			case 26: 	odfPresetId = preset_id::type::ooo_emphasis_flash_bulb				; break;
			case 27: 	odfPresetId = preset_id::type::ooo_emphasis_flicker					; break;
			case 28: 	odfPresetId = preset_id::type::ooo_emphasis_grow_with_color			; break;
			case 30: 	odfPresetId = preset_id::type::ooo_emphasis_lighten					; break;
			case 31: 	odfPresetId = preset_id::type::ooo_emphasis_style_emphasis			; break;
			case 32: 	odfPresetId = preset_id::type::ooo_emphasis_teeter					; break;
			case 33: 	odfPresetId = preset_id::type::ooo_emphasis_vertical_highlight		; break;
			case 34: 	odfPresetId = preset_id::type::ooo_emphasis_wave					; break;
			case 35: 	odfPresetId = preset_id::type::ooo_emphasis_blink					; break;
			case 36: 	odfPresetId = preset_id::type::ooo_emphasis_shimmer					; break;
			}
		} break;
		case 1: // "entrance"
		{
			switch (preset_id)
			{
			case 1:		odfPresetId = preset_id::type::ooo_entrance_appear			; break;
			case 2:		odfPresetId = preset_id::type::ooo_entrance_fly_in			; break;
			case 3:		odfPresetId = preset_id::type::ooo_entrance_venetian_blinds	; break;
			case 4:		odfPresetId = preset_id::type::ooo_entrance_box				; break;
			case 5:		odfPresetId = preset_id::type::ooo_entrance_checkerboard	; break;
			case 6:		odfPresetId = preset_id::type::ooo_entrance_circle			; break;
			case 7:		odfPresetId = preset_id::type::ooo_entrance_fly_in_slow		; break;
			case 8:		odfPresetId = preset_id::type::ooo_entrance_diamond			; break;
			case 9:		odfPresetId = preset_id::type::ooo_entrance_dissolve_in		; break;
			case 10:	odfPresetId = preset_id::type::ooo_entrance_fade_in			; break;
			case 11:	odfPresetId = preset_id::type::ooo_entrance_flash_once		; break;
			case 12:	odfPresetId = preset_id::type::ooo_entrance_peek_in			; break;
			case 13:	odfPresetId = preset_id::type::ooo_entrance_plus			; break;
			case 14:	odfPresetId = preset_id::type::ooo_entrance_random_bars		; break;
			case 15:	odfPresetId = preset_id::type::ooo_entrance_spiral_in		; break;
			case 16:	odfPresetId = preset_id::type::ooo_entrance_split			; break;
			case 17:	odfPresetId = preset_id::type::ooo_entrance_stretchy		; break;
			case 18:	odfPresetId = preset_id::type::ooo_entrance_diagonal_squares; break;
			case 19:	odfPresetId = preset_id::type::ooo_entrance_swivel			; break;
			case 20:	odfPresetId = preset_id::type::ooo_entrance_wedge			; break;
			case 21:	odfPresetId = preset_id::type::ooo_entrance_wheel			; break;
			case 22:	odfPresetId = preset_id::type::ooo_entrance_wipe			; break;
			case 23:	odfPresetId = preset_id::type::ooo_entrance_zoom			; break;
			case 24:	odfPresetId = preset_id::type::ooo_entrance_random			; break;
			case 25:	odfPresetId = preset_id::type::ooo_entrance_boomerang		; break;
			case 26:	odfPresetId = preset_id::type::ooo_entrance_bounce			; break;
			case 27:	odfPresetId = preset_id::type::ooo_entrance_colored_lettering; break;
			case 28:	odfPresetId = preset_id::type::ooo_entrance_movie_credits	; break;
			case 29:	odfPresetId = preset_id::type::ooo_entrance_ease_in			; break;
			case 30:	odfPresetId = preset_id::type::ooo_entrance_float			; break;
			case 31:	odfPresetId = preset_id::type::ooo_entrance_turn_and_grow	; break;
			case 34:	odfPresetId = preset_id::type::ooo_entrance_breaks			; break;
			case 35:	odfPresetId = preset_id::type::ooo_entrance_pinwheel		; break;
			case 37:	odfPresetId = preset_id::type::ooo_entrance_rise_up			; break;
			case 38:	odfPresetId = preset_id::type::ooo_entrance_falling_in		; break;
			case 39:	odfPresetId = preset_id::type::ooo_entrance_thread			; break;
			case 40:	odfPresetId = preset_id::type::ooo_entrance_unfold			; break;
			case 41:	odfPresetId = preset_id::type::ooo_entrance_whip			; break;
			case 42:	odfPresetId = preset_id::type::ooo_entrance_ascend			; break;
			case 43:	odfPresetId = preset_id::type::ooo_entrance_center_revolve	; break;
			case 45:	odfPresetId = preset_id::type::ooo_entrance_fade_in_and_swivel; break;
			case 47:	odfPresetId = preset_id::type::ooo_entrance_descend			; break;
			case 48:	odfPresetId = preset_id::type::ooo_entrance_sling			; break;
			case 49:	odfPresetId = preset_id::type::ooo_entrance_spin_in			; break;
			case 50:	odfPresetId = preset_id::type::ooo_entrance_compress		; break;
			case 51:	odfPresetId = preset_id::type::ooo_entrance_magnify			; break;
			case 52:	odfPresetId = preset_id::type::ooo_entrance_curve_up		; break;
			case 53:	odfPresetId = preset_id::type::ooo_entrance_fade_in_and_zoom; break;
			case 54:	odfPresetId = preset_id::type::ooo_entrance_glide			; break;
			case 55:	odfPresetId = preset_id::type::ooo_entrance_expand			; break;
			case 56:	odfPresetId = preset_id::type::ooo_entrance_flip			; break;
			case 58:	odfPresetId = preset_id::type::ooo_entrance_fold			; break;
			}
		} break;
		case 2: // "exit"
		{
			switch (preset_id)
			{
			case 1: odfPresetId = preset_id::type::ooo_exit_disappear			; break;
			case 2: odfPresetId = preset_id::type::ooo_exit_fly_out				; break;
			case 3: odfPresetId = preset_id::type::ooo_exit_venetian_blinds		; break;
			case 4: odfPresetId = preset_id::type::ooo_exit_box					; break;
			case 5: odfPresetId = preset_id::type::ooo_exit_checkerboard		; break;
			case 6: odfPresetId = preset_id::type::ooo_exit_circle				; break;
			case 7: odfPresetId = preset_id::type::ooo_exit_crawl_out			; break;
			case 8: odfPresetId = preset_id::type::ooo_exit_diamond				; break;
			case 9: odfPresetId = preset_id::type::ooo_exit_dissolve			; break;
			case 10: odfPresetId = preset_id::type::ooo_exit_fade_out			; break;
			case 11: odfPresetId = preset_id::type::ooo_exit_flash_once			; break;
			case 12: odfPresetId = preset_id::type::ooo_exit_peek_out			; break;
			case 13: odfPresetId = preset_id::type::ooo_exit_plus				; break;
			case 14: odfPresetId = preset_id::type::ooo_exit_random_bars		; break;
			case 15: odfPresetId = preset_id::type::ooo_exit_spiral_out			; break;
			case 16: odfPresetId = preset_id::type::ooo_exit_split				; break;
			case 17: odfPresetId = preset_id::type::ooo_exit_collapse			; break;
			case 18: odfPresetId = preset_id::type::ooo_exit_diagonal_squares	; break;
			case 19: odfPresetId = preset_id::type::ooo_exit_swivel				; break;
			case 20: odfPresetId = preset_id::type::ooo_exit_wedge				; break;
			case 21: odfPresetId = preset_id::type::ooo_exit_wheel				; break;
			case 22: odfPresetId = preset_id::type::ooo_exit_wipe				; break;
			case 23: odfPresetId = preset_id::type::ooo_exit_zoom				; break;
			case 24: odfPresetId = preset_id::type::ooo_exit_random				; break;
			case 25: odfPresetId = preset_id::type::ooo_exit_boomerang			; break;
			case 26: odfPresetId = preset_id::type::ooo_exit_bounce				; break;
			case 27: odfPresetId = preset_id::type::ooo_exit_colored_lettering	; break;
			case 28: odfPresetId = preset_id::type::ooo_exit_movie_credits		; break;
			case 29: odfPresetId = preset_id::type::ooo_exit_ease_out			; break;
			case 30: odfPresetId = preset_id::type::ooo_exit_float				; break;
			case 31: odfPresetId = preset_id::type::ooo_exit_turn_and_grow		; break;
			case 34: odfPresetId = preset_id::type::ooo_exit_breaks				; break;
			case 35: odfPresetId = preset_id::type::ooo_exit_pinwheel			; break;
			case 37: odfPresetId = preset_id::type::ooo_exit_sink_down			; break;
			case 38: odfPresetId = preset_id::type::ooo_exit_swish				; break;
			case 39: odfPresetId = preset_id::type::ooo_exit_thread				; break;
			case 40: odfPresetId = preset_id::type::ooo_exit_unfold				; break;
			case 41: odfPresetId = preset_id::type::ooo_exit_whip				; break;
			case 42: odfPresetId = preset_id::type::ooo_exit_descend			; break;
			case 43: odfPresetId = preset_id::type::ooo_exit_center_revolve		; break;
			case 45: odfPresetId = preset_id::type::ooo_exit_fade_out_and_swivel; break;
			case 47: odfPresetId = preset_id::type::ooo_exit_ascend				; break;
			case 48: odfPresetId = preset_id::type::ooo_exit_sling				; break;
			case 53: odfPresetId = preset_id::type::ooo_exit_fade_out_and_zoom	; break;
			case 55: odfPresetId = preset_id::type::ooo_exit_contract			; break;
			case 49: odfPresetId = preset_id::type::ooo_exit_spin_out			; break;
			case 50: odfPresetId = preset_id::type::ooo_exit_stretchy			; break;
			case 51: odfPresetId = preset_id::type::ooo_exit_magnify			; break;
			case 52: odfPresetId = preset_id::type::ooo_exit_curve_down			; break;
			case 54: odfPresetId = preset_id::type::ooo_exit_glide				; break;
			case 56: odfPresetId = preset_id::type::ooo_exit_flip				; break;
			case 58: odfPresetId = preset_id::type::ooo_exit_fold				; break;
			}
		} break;
		case 4: // motion-path
		{
			switch (preset_id)
			{
			case 16:	odfPresetId = preset_id::type::ooo_motionpath_4_point_star		; break;
			case 5:		odfPresetId = preset_id::type::ooo_motionpath_5_point_star		; break;
			case 11:	odfPresetId = preset_id::type::ooo_motionpath_6_point_star		; break;
			case 17:	odfPresetId = preset_id::type::ooo_motionpath_8_point_star		; break;
			case 1:		odfPresetId = preset_id::type::ooo_motionpath_circle			; break;
			case 6:		odfPresetId = preset_id::type::ooo_motionpath_crescent_moon		; break;
			case 3:		odfPresetId = preset_id::type::ooo_motionpath_diamond			; break;
			case 13:	odfPresetId = preset_id::type::ooo_motionpath_equal_triangle	; break;
			case 12:	odfPresetId = preset_id::type::ooo_motionpath_oval				; break;
			case 9:		odfPresetId = preset_id::type::ooo_motionpath_heart				; break;
			case 4:		odfPresetId = preset_id::type::ooo_motionpath_hexagon			; break;
			case 10:	odfPresetId = preset_id::type::ooo_motionpath_octagon			; break;
			case 14:	odfPresetId = preset_id::type::ooo_motionpath_parallelogram		; break;
			case 15:	odfPresetId = preset_id::type::ooo_motionpath_pentagon			; break;
			case 2:		odfPresetId = preset_id::type::ooo_motionpath_right_triangle	; break;
			case 7:		odfPresetId = preset_id::type::ooo_motionpath_square			; break;
			case 18:	odfPresetId = preset_id::type::ooo_motionpath_teardrop			; break;
			case 8:		odfPresetId = preset_id::type::ooo_motionpath_trapezoid			; break;
			case 37:	odfPresetId = preset_id::type::ooo_motionpath_arc_down			; break;
			case 51:	odfPresetId = preset_id::type::ooo_motionpath_arc_left			; break;
			case 58:	odfPresetId = preset_id::type::ooo_motionpath_arc_right			; break;
			case 44:	odfPresetId = preset_id::type::ooo_motionpath_arc_up			; break;
			case 41:	odfPresetId = preset_id::type::ooo_motionpath_bounce_left		; break;
			case 54:	odfPresetId = preset_id::type::ooo_motionpath_bounce_right		; break;
			case 48:	odfPresetId = preset_id::type::ooo_motionpath_curvy_left		; break;
			case 61:	odfPresetId = preset_id::type::ooo_motionpath_curvy_right		; break;
			case 60:	odfPresetId = preset_id::type::ooo_motionpath_decaying_wave		; break;
			case 49:	odfPresetId = preset_id::type::ooo_motionpath_diagonal_down_right; break;
			case 56:	odfPresetId = preset_id::type::ooo_motionpath_diagonal_up_right	; break;
			case 42:	odfPresetId = preset_id::type::ooo_motionpath_down				; break;
			case 52:	odfPresetId = preset_id::type::ooo_motionpath_funnel			; break;
			case 53:	odfPresetId = preset_id::type::ooo_motionpath_spring			; break;
			case 62:	odfPresetId = preset_id::type::ooo_motionpath_stairs_down		; break;
			case 50:	odfPresetId = preset_id::type::ooo_motionpath_turn_down			; break;
			case 36:	odfPresetId = preset_id::type::ooo_motionpath_turn_down_right	; break;
			case 43:	odfPresetId = preset_id::type::ooo_motionpath_turn_up			; break;
			case 57:	odfPresetId = preset_id::type::ooo_motionpath_turn_up_right		; break;
			case 64:	odfPresetId = preset_id::type::ooo_motionpath_up				; break;
			case 47:	odfPresetId = preset_id::type::ooo_motionpath_wave				; break;
			case 38:	odfPresetId = preset_id::type::ooo_motionpath_zigzag			; break;
			case 31:	odfPresetId = preset_id::type::ooo_motionpath_bean				; break;
			case 25:	odfPresetId = preset_id::type::ooo_motionpath_buzz_saw			; break;
			case 20:	odfPresetId = preset_id::type::ooo_motionpath_curved_square		; break;
			case 21:	odfPresetId = preset_id::type::ooo_motionpath_curved_x			; break;
			case 23:	odfPresetId = preset_id::type::ooo_motionpath_curvy_star		; break;
			case 28:	odfPresetId = preset_id::type::ooo_motionpath_figure_8_four		; break;
			case 26:	odfPresetId = preset_id::type::ooo_motionpath_horizontal_figure_8; break;
			case 34:	odfPresetId = preset_id::type::ooo_motionpath_inverted_square	; break;
			case 33:	odfPresetId = preset_id::type::ooo_motionpath_inverted_triangle	; break;
			case 24:	odfPresetId = preset_id::type::ooo_motionpath_loop_de_loop		; break;
			case 29:	odfPresetId = preset_id::type::ooo_motionpath_neutron			; break;
			case 27:	odfPresetId = preset_id::type::ooo_motionpath_peanut			; break;
			case 32:	odfPresetId = preset_id::type::ooo_motionpath_clover			; break;
			case 19:	odfPresetId = preset_id::type::ooo_motionpath_pointy_star		; break;
			case 30:	odfPresetId = preset_id::type::ooo_motionpath_swoosh			; break;
			case 22:	odfPresetId = preset_id::type::ooo_motionpath_vertical_figure_8	; break;
			case 35:	odfPresetId = preset_id::type::ooo_motionpath_left				; break;
			case 63:	odfPresetId = preset_id::type::ooo_motionpath_right				; break;
			case 55:	odfPresetId = preset_id::type::ooo_motionpath_spiral_left		; break;
			case 46:	odfPresetId = preset_id::type::ooo_motionpath_spiral_right		; break;
			case 40:	odfPresetId = preset_id::type::ooo_motionpath_sine_wave			; break;
			case 59:	odfPresetId = preset_id::type::ooo_motionpath_s_curve_1			; break;
			case 39:	odfPresetId = preset_id::type::ooo_motionpath_s_curve_2			; break;
			case 45:	odfPresetId = preset_id::type::ooo_motionpath_heartbeat			; break;
			default:	odfPresetId = preset_id::type::libo_motionpath_curve			; break;
			}
		} break;
	}

	if(odfPresetId.get_type() != preset_id::type::none)
		odp_context->current_slide().set_anim_preset_id(odfPresetId);
	else 
		odp_context->current_slide().set_anim_preset_id(odfPresetId);
}

void PptxConverter::convert(PPTX::Logic::CBhvr* oox_cbhvr)
{
	if (!oox_cbhvr)
		return;

	convert(&oox_cbhvr->cTn);
	convert(&oox_cbhvr->tgtEl);

	if (oox_cbhvr->attrNameLst.IsInit())
	{
		if (oox_cbhvr->attrNameLst->list.size() == 1)
		{
			convert(&oox_cbhvr->attrNameLst->list[0]);
		}
		else
			_CP_LOG << L"[warning] : multiple attribute list elements not supported\n";
	}
		
}

void PptxConverter::convert(PPTX::Logic::TgtEl* oox_tgt_el)
{
	if (!oox_tgt_el)
		return;

	if (oox_tgt_el->spTgt.IsInit())
	{
		const std::wstring& odfId = odp_context->get_mapped_identifier(oox_tgt_el->spTgt->spid);

		if(!odfId.empty())
			odp_context->current_slide().set_anim_target_element(odfId);
	}
}

void PptxConverter::convert(PPTX::Logic::AnimVariant* oox_anim_variant)
{
	if (!oox_anim_variant)
		return;

	std::wstring val;

	if (oox_anim_variant->boolVal.IsInit())
		val = std::to_wstring(*oox_anim_variant->boolVal);
	else if (oox_anim_variant->strVal.IsInit())
		val = *oox_anim_variant->strVal;
	else if (oox_anim_variant->intVal.IsInit())
		val = std::to_wstring(*oox_anim_variant->intVal);
	else if (oox_anim_variant->fltVal.IsInit())
		val = std::to_wstring(*oox_anim_variant->fltVal);

	odp_context->current_slide().set_anim_to(val);
}

void PptxConverter::convert(PPTX::Logic::AttrName* oox_attr_name)
{
	if (!oox_attr_name)
		return;

	smil_attribute_name attrName = smil_attribute_name::none;
	const std::wstring& val = oox_attr_name->text;

	if (val == L"style.color")
		attrName = smil_attribute_name::color;
	else if(val == L"stroke.color")
		attrName = smil_attribute_name::strokeColor;
	else if (val == L"fill.type")
		attrName = smil_attribute_name::fill;
	else if (val == L"fillcolor")
		attrName = smil_attribute_name::fillColor;
	else if (val == L"fill.on")
		attrName = smil_attribute_name::fillOn;
	else if (val == L"style.opacity")
		attrName = smil_attribute_name::opacity;
	else if (val == L"stroke.on")
		attrName = smil_attribute_name::stroke;
	else if (val == L"r" || val == L"style.rotation")
		attrName = smil_attribute_name::rotate;
	else if (val == L"xshear")
		attrName = smil_attribute_name::skewX;
	else if (val == L"style.visibility")
		attrName = smil_attribute_name::visibility;
	else if (val == L"ppt_x")
		attrName = smil_attribute_name::x;
	else if (val == L"ppt_y")
		attrName = smil_attribute_name::y;
	else if (val == L"ppt_w")
		attrName = smil_attribute_name::width;
	else if (val == L"ppt_h")
		attrName = smil_attribute_name::height;
	else if (val == L"ppt_c")
		attrName = smil_attribute_name::dim;

	odp_context->current_slide().set_anim_attribute_name(attrName);
}

void PptxConverter::convert(PPTX::Logic::AnimEffect* oox_anim_effect)
{
	if (!oox_anim_effect)
		return;

	odp_context->current_slide().start_timing_transition_filter();

	if (oox_anim_effect->transition.IsInit())
	{
		odp_context->current_slide().set_anim_transition_filter_mode(oox_anim_effect->transition->get());
	}
	if (oox_anim_effect->filter.IsInit())
	{
		std::wstring filter = *oox_anim_effect->filter;
		std::wstring subtype = L"";

		_CP_OPT(smil_transition_type) odfType = boost::none;
		std::wstring odfSubtype = L"";
		bool odfReversed = false;

		std::string::size_type openBracket = filter.find('(');
		std::string::size_type closeBracket = filter.find(')');

		if (openBracket != std::wstring::npos &&
			closeBracket != std::wstring::npos)
		{
			filter = oox_anim_effect->filter->substr(0, openBracket);
			subtype = oox_anim_effect->filter->substr(openBracket + 1, closeBracket - openBracket - 1);
		}

		std::transform(filter.begin(), filter.end(), filter.begin(), ::towlower);
		if (filter == L"blinds")
		{
			odfType = smil_transition_type::blindsWipe;
			odfReversed = false;
			if (subtype == L"horizontal")			odfSubtype = L"horizontal";
			else if (subtype == L"vertical")		odfSubtype = L"vertical";
		}
		else if (filter == L"box")
		{
			odfType = smil_transition_type::irisWipe;
			odfSubtype = L"rectangle";
			if (subtype == L"in")			odfReversed = true;
			else if (subtype == L"out")		odfReversed = false;
		}
		else if (filter == L"checkerboard")
		{
			odfType = smil_transition_type::checkerBoardWipe;
			odfReversed = false;
			if (subtype == L"across")			odfSubtype = L"across";
			else if (subtype == L"down")		odfSubtype = L"down";
		}
		else if (filter == L"circle")
		{
			odfType = smil_transition_type::ellipseWipe;
			odfSubtype = L"horizontal";
			if (subtype == L"in")			odfReversed = true;
			else if (subtype == L"out")		odfReversed = false;
		}
		else if (filter == L"diamond")
		{
			odfType = smil_transition_type::irisWipe;
			odfSubtype = L"diamond";
			if (subtype == L"in")			odfReversed = true;
			else if (subtype == L"out")		odfReversed = false;
		}
		else if (filter == L"dissolve")
		{
			odfType = smil_transition_type::dissolve;
			odfSubtype = L"";
			odfReversed = false;
		}
		else if (filter == L"fade")
		{
			odfType = smil_transition_type::fade;
			odfSubtype = L"crossfade";
			odfReversed = false;
		}
		else if (filter == L"slide")
		{
			odfType = smil_transition_type::slideWipe;
			odfReversed = false;
			odfSubtype = subtype;
		}
		else if (filter == L"plus")
		{
			odfType = smil_transition_type::fourBoxWipe;
			odfSubtype = L"cornersIn";
			if (subtype == L"in")			odfReversed = false;
			else if (subtype == L"out")		odfReversed = true;
		}
		else if (filter == L"barn")
		{
			odfType = smil_transition_type::barnDoorWipe;
			if (subtype == L"inVertical")			{ odfReversed = true;	odfSubtype = L"vertical"; }
			else if (subtype == L"inHorizontal")	{ odfReversed = true;	odfSubtype = L"horizontal"; }
			else if (subtype == L"outVertical")		{ odfReversed = false;	odfSubtype = L"vertical"; }
			else if (subtype == L"outHorizontal")	{ odfReversed = false;	odfSubtype = L"horizontal"; }
		}
		else if (boost::starts_with(filter, L"randombar"))
		{
			odfType = smil_transition_type::randomBarWipe;
			odfReversed = false;
			odfSubtype = subtype;
		}
		else if (filter == L"strips")
		{
			odfType = smil_transition_type::waterfallWipe;
			if (subtype == L"downLeft")			{ odfReversed = false;	odfSubtype = L"horizontalRight"; }
			else if (subtype == L"upLeft")		{ odfReversed = true;	odfSubtype = L"horizontalLeft"; }
			else if (subtype == L"downRight")	{ odfReversed = false;	odfSubtype = L"horizontalLeft"; }
			else if (subtype == L"upRight")		{ odfReversed = true;	odfSubtype = L"horizontalRight"; }
		}
		else if (filter == L"wedge")
		{
			odfType = smil_transition_type::fanWipe;
			odfReversed = false;
			odfSubtype = L"centerTop";
		}
		else if (filter == L"wheel")
		{
			odfType = smil_transition_type::pinWheelWipe;
			odfReversed = false;
			if (subtype == L"1")		odfSubtype = L"oneBlade";
			else if (subtype == L"2")	odfSubtype = L"twoBladeVertical"; 
			else if (subtype == L"3")	odfSubtype = L"threeBlade"; 
			else if (subtype == L"4")	odfSubtype = L"fourBlade";
			else if (subtype == L"8")	odfSubtype = L"eightBlade";
		}
		else if (filter == L"wipe")
		{
			odfType = smil_transition_type::slideWipe;
			if (subtype == L"right")		{ odfReversed = true;	odfSubtype = L"fromRight"; }
			else if (subtype == L"left")	{ odfReversed = false;	odfSubtype = L"fromLeft"; }
			else if (subtype == L"down")	{ odfReversed = true;	odfSubtype = L"fromBottom"; }
			else if (subtype == L"up")		{ odfReversed = false;	odfSubtype = L"fromTop"; }
		}
		else if (filter == L"image")
		{
			odfType = boost::none;
		}
		else
		{
			odfType = smil_transition_type::fade;
			odfReversed = false;
		}

		if(odfType)
			odp_context->current_slide().set_anim_transition_filter_type(odfType.value());

		if(!odfSubtype.empty())
			odp_context->current_slide().set_anim_transition_filter_subtype(odfSubtype);
		if(odfReversed)
			odp_context->current_slide().set_anim_transition_filter_direction(L"reverse");
	}

	convert(&oox_anim_effect->cBhvr);
	
	odp_context->current_slide().end_timing_transition_filter();
}

void PptxConverter::convert(PPTX::Logic::Anim* oox_anim)
{
	if (!oox_anim)
		return;

	odp_context->current_slide().start_timing_anim();

	convert(&oox_anim->cBhvr);

	if (oox_anim->tavLst.IsInit())
	{
		std::wstringstream ss_tm;
		std::wstringstream ss_val;

		for (size_t i = 0; i < oox_anim->tavLst->list.size(); i++)
		{
			if (i > 0) 
				ss_tm << L";";

			if (oox_anim->tavLst->list[i].tm.IsInit())
				ss_tm << boost::lexical_cast<double>(*oox_anim->tavLst->list[i].tm) / 100000.0;
			else
				ss_tm << 0;
			
			if (oox_anim->tavLst->list[i].val.IsInit())
			{
				if (i > 0) 
					ss_val << L";";

				if(oox_anim->tavLst->list[i].val->boolVal.IsInit())
					ss_val << *oox_anim->tavLst->list[i].val->boolVal;
				else if (oox_anim->tavLst->list[i].val->intVal.IsInit())
					ss_val << *oox_anim->tavLst->list[i].val->intVal;
				else if (oox_anim->tavLst->list[i].val->fltVal.IsInit())
					ss_val << *oox_anim->tavLst->list[i].val->fltVal;
				else if (oox_anim->tavLst->list[i].val->strVal.IsInit())
					ss_val << *oox_anim->tavLst->list[i].val->strVal;
			}
		}

		if (oox_anim->tavLst->list.size() > 1)
		{
			if (oox_anim->tavLst->list[0].fmla.IsInit())
			{
				std::wstring formula = convert_animation_formula(*oox_anim->tavLst->list[0].fmla);
				odp_context->current_slide().set_anim_animation_formula(formula);
			}
		}
		
		odp_context->current_slide().set_anim_animation_keytimes(odf_types::smil_key_times::parse(convert_animation_formula(ss_tm.str())));
		odp_context->current_slide().set_anim_animation_values(odf_types::smil_values::parse(convert_animation_formula(ss_val.str())));
	}

	if (oox_anim->by.IsInit())
	{
		std::wstring by = convert_animation_formula(*oox_anim->by);
		odp_context->current_slide().set_anim_animation_by(by);
	}
	if (oox_anim->from.IsInit())
	{
		std::wstring from = convert_animation_formula(*oox_anim->from);
		odp_context->current_slide().set_anim_animation_from(from);
	}
	if (oox_anim->to.IsInit())
	{
		std::wstring to = convert_animation_formula(*oox_anim->to);
		odp_context->current_slide().set_anim_animation_to(to);
	}

	odp_context->current_slide().end_timing_anim();
}

void PptxConverter::convert(PPTX::Logic::AnimMotion* oox_anim_motion)
{
	if (!oox_anim_motion)
		return;

	odp_context->current_slide().start_timing_motion();
	odp_context->current_slide().set_anim_attribute_name(odf_types::smil_attribute_name::x);

	convert(&oox_anim_motion->cBhvr);

	if (oox_anim_motion->path.IsInit())
		odp_context->current_slide().set_anim_motion_path(*oox_anim_motion->path);
		
	// TODO: Convert "from", "to", "by"
	// TODO: Convert "rCtr"

	odp_context->current_slide().end_timing_motion();
}

void PptxConverter::convert(PPTX::Logic::AnimClr* oox_anim_color)
{
	if (!oox_anim_color)
		return;

	odp_context->current_slide().start_timing_anim_clr();

	convert(&oox_anim_color->cBhvr);

	if (oox_anim_color->to.is_init())
	{
		std::wstringstream ss;
		ss << L"#" << std::hex << (oox_anim_color->to.GetRGBA() >> 8);

		odp_context->current_slide().set_anim_color_to(ss.str());
	}

	if (oox_anim_color->clrSpc.IsInit())
	{
		odp_context->current_slide().set_anim_color_interpolation(oox_anim_color->clrSpc->get());
	}

	if (oox_anim_color->dir.IsInit())
	{
		if(oox_anim_color->dir->get() == L"cw")
			odp_context->current_slide().set_anim_color_direction(L"clockwise");
		else if (oox_anim_color->dir->get() == L"ccw")
			odp_context->current_slide().set_anim_color_direction(L"counter-clockwise");
	}

	if (oox_anim_color->byR.IsInit() ||
		oox_anim_color->byG.IsInit() ||
		oox_anim_color->byB.IsInit())
	{
		const int r = oox_anim_color->byR.get_value_or(0);
		const int g = oox_anim_color->byG.get_value_or(0);
		const int b = oox_anim_color->byB.get_value_or(0);
		const float multiplyer = 1000.0f;

		std::wstringstream ss;
		ss << L"#"
			<< std::setfill(L'0')
			<< std::setw(2)
			<< std::hex
			<< ((int)(r / multiplyer) & 0xff)
			<< ((int)(g / multiplyer) & 0xff)
			<< ((int)(b / multiplyer) & 0xff);

		odp_context->current_slide().set_anim_color_by(ss.str());
	}
	else if (oox_anim_color->byH.IsInit() ||
		oox_anim_color->byS.IsInit() ||
		oox_anim_color->byL.IsInit())
	{
		const int h = oox_anim_color->byH.get_value_or(0);
		const int s = oox_anim_color->byS.get_value_or(0);
		const int l = oox_anim_color->byL.get_value_or(0);

		std::wstringstream ss;
		ss << L"hsl("
			<< (h / 100000) << L","
			<< (s / 1000  ) << L"%,"
			<< (l / 1000  ) << L"%)";
		odp_context->current_slide().set_anim_color_by(ss.str());
	}

	odp_context->current_slide().end_timing_anim_clr();
}

void PptxConverter::convert(PPTX::Logic::AnimRot* oox_anim_rot)
{
	if (!oox_anim_rot)
		return;

	odp_context->current_slide().start_timing_anim();
	odp_context->current_slide().set_anim_animation_type(odf_types::svg_type::rotate);

	convert(&oox_anim_rot->cBhvr);

	const double odp_mulipyer = 60000.0;
	if (oox_anim_rot->from.IsInit())
	{
		odp_context->current_slide().set_anim_animation_from(std::to_wstring(*oox_anim_rot->from / odp_mulipyer));
	}
	if (oox_anim_rot->to.IsInit())
	{
		odp_context->current_slide().set_anim_animation_to(std::to_wstring(*oox_anim_rot->to / odp_mulipyer));
	}
	if (oox_anim_rot->by.IsInit())
	{
		odp_context->current_slide().set_anim_animation_by(std::to_wstring(*oox_anim_rot->by / odp_mulipyer));
	}

	odp_context->current_slide().end_timing_anim();
}

void PptxConverter::convert(PPTX::Logic::AnimScale* oox_anim_scale)
{
	if (!oox_anim_scale)
		return;

	odp_context->current_slide().start_timing_transform();
	odp_context->current_slide().set_anim_transform_type(odf_types::svg_type::scale);
	odp_context->current_slide().set_anim_attribute_name(odf_types::smil_attribute_name::transform);
	
	convert(&oox_anim_scale->cBhvr);

	if (oox_anim_scale->fromX.IsInit() && oox_anim_scale->fromY.IsInit())
	{
		std::wstring from = convert_animation_scale_values(*oox_anim_scale->fromX, *oox_anim_scale->fromY);
		odp_context->current_slide().set_anim_transform_from(from);
	}

	if (oox_anim_scale->toX.IsInit() && oox_anim_scale->toY.IsInit())
	{
		std::wstring to = convert_animation_scale_values(*oox_anim_scale->toX, *oox_anim_scale->toY);
		odp_context->current_slide().set_anim_transform_to(to);
	}

	if (oox_anim_scale->byX.IsInit() && oox_anim_scale->byY.IsInit())
	{
		const int pptx_multiplyer = 100000;
		std::wstring by = convert_animation_scale_values(*oox_anim_scale->byX - pptx_multiplyer, *oox_anim_scale->byY - pptx_multiplyer);
		odp_context->current_slide().set_anim_transform_by(by);
	}

	odp_context->current_slide().end_timing_transform();
}

void PptxConverter::convert(PPTX::Logic::Audio* oox_audio)
{
	if (!oox_audio)
		return;

	odp_context->current_slide().start_anim_audio();

	if (oox_audio->cMediaNode.tgtEl.name.IsInit())
	{
		bool isExternal;
		const std::wstring aID = oox_audio->cMediaNode.tgtEl.embed->get();
		const std::wstring pathAudio = find_link_by_id(aID, 3, isExternal);

		const std::wstring xlink = odp_context->add_media(pathAudio);
		
		odp_context->current_slide().set_anim_audio_xlink(xlink);
	}

	odp_context->current_slide().end_anim_audio();
}

void PptxConverter::convert(odf_writer::graphic_format_properties* graphic_props, odf_writer::style_table_cell_properties* table_cell_props)
{
	if (!graphic_props)
		return;
	if (!table_cell_props)
		return;

	using namespace odf_types;

	if(graphic_props->common_draw_fill_attlist_.draw_fill_color_)
		table_cell_props->content_.common_background_color_attlist_.fo_background_color_ = background_color(*graphic_props->common_draw_fill_attlist_.draw_fill_color_);

	odf_types::color color_ = graphic_props->svg_stroke_color_.get_value_or(odf_types::color(L"#FFFFFF"));
	// odf_types::draw_fill draw_fill_ = graphic_props->common_draw_fill_attlist_.draw_fill_.get_value_or(draw_fill(draw_fill::solid));
	odf_types::length length_ = graphic_props->svg_stroke_width_.get_value_or(length(1, length::pt)).get_length();

	table_cell_props->content_.common_border_attlist_.fo_border_bottom_ = odf_types::border_style(color_, border_style::solid, length_);
}

void PptxConverter::convert(odf_writer::paragraph_format_properties* paragraph_props, odf_writer::style_table_cell_properties* table_cell_props)
{
	if (!paragraph_props)
		return;
	if (!table_cell_props)
		return;

	if (paragraph_props->common_border_attlist_.fo_border_)
	{
		table_cell_props->content_.common_border_attlist_.fo_border_left_	= *paragraph_props->common_border_attlist_.fo_border_;
		table_cell_props->content_.common_border_attlist_.fo_border_top_	= *paragraph_props->common_border_attlist_.fo_border_;
		table_cell_props->content_.common_border_attlist_.fo_border_right_	= *paragraph_props->common_border_attlist_.fo_border_;
		table_cell_props->content_.common_border_attlist_.fo_border_bottom_ = *paragraph_props->common_border_attlist_.fo_border_;
	}
	else 
		table_cell_props->content_.common_border_attlist_ = paragraph_props->common_border_attlist_;		
}

void PptxConverter::convert_common()
{
	if (presentation->sldSz.IsInit())
	{
		_CP_OPT(odf_types::length) width	= odf_types::length(presentation->sldSz->cx / 12700., odf_types::length::pt);
		_CP_OPT(odf_types::length) height	= odf_types::length(presentation->sldSz->cy / 12700., odf_types::length::pt);
		odf_context()->page_layout_context()->set_page_size(width, height);
		if (presentation->sldSz->type.IsInit())
		{
			switch(presentation->sldSz->type->GetBYTECode())
			{
			default:
				break;
			}
			//odf_context()->page_layout_context()->set_page_orientation
		}
	}
}

std::wstring PptxConverter::convert_animation_formula(std::wstring formula)
{
	boost::erase_all(formula, L"#");
	boost::replace_all(formula, L"ppt_x", L"x");
	boost::replace_all(formula, L"ppt_y", L"y");
	boost::replace_all(formula, L"ppt_w", L"width");
	boost::replace_all(formula, L"ppt_h", L"height");

	return formula;
}

std::wstring PptxConverter::convert_animation_scale_values(int x, int y)
{
	std::wstringstream ss;
	ss.setf(std::ios::fixed);
	ss.precision(2);
	const double odp_multipyer = 100000;
	double _x = x / odp_multipyer;
	double _y = y / odp_multipyer;
	ss << _x << L"," << _y;

	return ss.str();
}

std::wstring PptxConverter::get_page_name(PPTX::Logic::CSld* oox_slide, _typePages type)
{
	if (!oox_slide)
		return std::wstring();

	std::wstring page_name;
	if (oox_slide->attrName.IsInit())
		page_name = oox_slide->attrName.get();

	if (page_name.empty())
	{
		if (type == Slide)
			page_name = L"Slide_" + std::to_wstring((int)odp_context->get_pages_count());
	}

	return page_name;
}

void PptxConverter::fill_in_deferred_hyperlinks()
{
	for (auto hyperlink : odp_context->get_deferred_hyperlinks())
	{
		cpdoccore::odf_writer::presentation_event_listener* event_listener = dynamic_cast<cpdoccore::odf_writer::presentation_event_listener*>(hyperlink.first.get());
		const std::wstring& slidename = hyperlink.second;

		if (!event_listener)
			continue;

		auto hrefIt = odp_context->map_slidenames_.find(slidename);
		if (hrefIt == odp_context->map_slidenames_.end())
			continue;

		event_listener->attlist_.common_xlink_attlist_.href_ = std::wstring(L"#") + hrefIt->second;
		event_listener->attlist_.common_xlink_attlist_.type_ = xlink_type::Simple;
		event_listener->attlist_.common_xlink_attlist_.show_ = xlink_show::Embed;
		event_listener->attlist_.common_xlink_attlist_.actuate_ = xlink_actuate::OnRequest;
	}
}


void PptxConverter::convert_masters_and_layouts()
{
	for (size_t iMaster = 0; iMaster < presentation->sldMasterIdLst.size(); ++iMaster)
	{
		smart_ptr<PPTX::SlideMaster> slideMaster = ((*presentation)[presentation->sldMasterIdLst[iMaster].rid.get()]).smart_dynamic_cast<PPTX::SlideMaster>();

		if (slideMaster.IsInit() == false)
			continue;

		for (size_t iLayout = 0; iLayout < slideMaster->sldLayoutIdLst.size(); ++iLayout)
		{
			std::wstring rId = slideMaster->sldLayoutIdLst[iLayout].rid.get();
			smart_ptr<PPTX::SlideLayout> slideLayout = ((*slideMaster)[rId]).smart_dynamic_cast<PPTX::SlideLayout>();

			if (false == slideLayout.IsInit()) continue;
			
			std::map<std::wstring, std::wstring>::iterator pFind = m_mapLayouts.find(slideLayout->m_sOutputFilename);
			if (pFind == m_mapLayouts.end())
			{
				odp_context->start_layout_slide();
					convert_layout(&slideLayout->cSld);
				odp_context->end_layout_slide();

				std::wstring layout_style_name = odp_context->page_layout_context()->get_local_styles_context()->last_state(odf_types::style_family::PresentationPageLayout)->get_name();

				m_mapLayouts.insert(std::make_pair(slideLayout->m_sOutputFilename, layout_style_name));
			}
		}
	}
}

void PptxConverter::convert_slides()
{
	for (size_t i = 0; i < presentation->sldIdLst.size(); ++i)
	{
		odp_context->map_table_styles_.clear();// todooo - для одинаковых тем не чистить

        std::wstring rId = presentation->sldIdLst[i].rid.get();
        smart_ptr<PPTX::Slide> slide = ((*presentation)[rId]).smart_dynamic_cast<PPTX::Slide>();
		
		if (slide.IsInit() == false)
        {
            continue;// странное ... слайд 38 в FY10_September_Partner_Call.pptx
        }

		current_theme	= slide->theme.GetPointer();
		current_clrMap	= NULL;

		std::wstring master_style_name;
		std::wstring layout_style_name;

		bool bShow				= slide->show.get_value_or(true);
		bool bShowMasterAnim	= slide->showMasterPhAnim.get_value_or(true);
		bool bShowMasterSp		= slide->showMasterSp.get_value_or(true);

		PPTX::Logic::TxStyles* current_txStyles = NULL;
		if (slide->Master.IsInit())
		{
			current_clrMap		= &slide->Master->clrMap;
			current_txStyles	= slide->Master->txStyles.GetPointer();

			std::wstring master_name = (bShowMasterSp ? slide->Master->m_sOutputFilename : L"") + slide->Layout->m_sOutputFilename;

			std::map<std::wstring, std::wstring>::iterator pFind = m_mapMasters.find(master_name);
			if (pFind == m_mapMasters.end())
			{
				master_style_name = L"MasterPage";
				
				bool bShowLayoutMasterAnim	= slide->Layout->showMasterPhAnim.get_value_or(true);
				bool bShowLayoutMasterSp	= slide->Layout->showMasterSp.get_value_or(true);
				
				if (slide->Master->cSld.attrName.IsInit())					master_style_name = slide->Master->cSld.attrName.get();
				else if ((current_theme) && (current_theme->name.IsInit()))	master_style_name = current_theme->name.get();

				master_style_name += L"_" ;
				
				if (slide->Layout->cSld.attrName.IsInit())	master_style_name += slide->Layout->cSld.attrName.get();
				else if (slide->Layout->attrType.IsInit())	master_style_name += slide->Layout->attrType->get();
				else
					master_style_name += std::to_wstring((int)m_mapMasters.size());
				
				odp_context->start_master_slide(master_style_name);
					convert_common();

					current_slide = slide->Master.GetPointer();
					
					if (bShowLayoutMasterSp && bShowMasterSp)
						convert_slide(&slide->Master->cSld, current_txStyles, false, true, Master);
					else
						convert(slide->Master->cSld.bg.GetPointer());

					if (slide->Layout->clrMapOvr.IsInit() && slide->Layout->clrMapOvr->overrideClrMapping.IsInit())
						current_clrMap	= slide->Layout->clrMapOvr->overrideClrMapping.GetPointer();
					current_slide = slide->Layout.GetPointer();
					
					convert_slide(&slide->Layout->cSld, current_txStyles, true, bShowLayoutMasterSp, Layout);	

					if (slide->Layout->transition.IsInit())	convert	(slide->Layout->transition.GetPointer());
					else									convert	(slide->Master->transition.GetPointer());
					
					//if (slide->Layout->timing.IsInit())	convert	(slide->Layout->timing.GetPointer());
					//else									convert	(slide->Master->timing.GetPointer());

					if (!presentation->notesMasterIdLst.empty())
					{
						rId = presentation->notesMasterIdLst[0].rid.get();
						smart_ptr<PPTX::NotesMaster> notes_master = ((*presentation)[rId]).smart_dynamic_cast<PPTX::NotesMaster>();
						convert(notes_master.GetPointer());
					}
				odp_context->end_master_slide();
				
				m_mapMasters.insert(std::make_pair(master_name, master_style_name));
			}
			else
			{
				master_style_name = pFind->second;
			}
			pFind = m_mapLayouts.find(slide->Layout->m_sOutputFilename);
			if (pFind == m_mapLayouts.end())
			{//сюда уже не попадет - выше
				odp_context->start_layout_slide();
					convert_layout(&slide->Layout->cSld);
				odp_context->end_layout_slide();

				layout_style_name = odp_context->page_layout_context()->get_local_styles_context()->last_state(odf_types::style_family::PresentationPageLayout)->get_name();
				
				m_mapLayouts.insert(std::make_pair(slide->Layout->m_sOutputFilename, layout_style_name));
			}
			else
			{
				layout_style_name = pFind->second;
			}
		}
//--------------------------------------------------------------------------------------------------------------------
		current_clrMap	= &slide->Master->clrMap; //after layout					
		
		if (slide->clrMapOvr.IsInit() && slide->clrMapOvr->overrideClrMapping.IsInit())
			current_clrMap	= slide->clrMapOvr->overrideClrMapping.GetPointer();

		current_slide		= slide.GetPointer();

		odp_context->start_slide();
		
		odp_context->current_slide().set_master_page (master_style_name);
		odp_context->current_slide().set_layout_page (layout_style_name);

		odp_context->add_page_name(get_page_name(slide->cSld.GetPointer(), Slide));
		convert_slide	(slide->cSld.GetPointer(), current_txStyles, true, bShowMasterSp, Slide);
		convert			(slide->timing.GetPointer());
		convert			(slide->comments.GetPointer());
		convert			(slide->Note.GetPointer());
		
		convert			(slide->transition.GetPointer());

		if (!bShow)
			odp_context->hide_slide();


		odp_context->end_slide();
	}

	fill_in_deferred_hyperlinks();
}
void PptxConverter::convert(PPTX::NotesMaster *oox_notes)
{
	if (!oox_notes) return;
	
	odp_context->start_note(true);
	
	PPTX::Theme*			old_theme	= current_theme;
	PPTX::Logic::ClrMap*	old_clrMap	= current_clrMap;

	current_theme	= oox_notes->theme_.GetPointer();
	current_clrMap	= &oox_notes->clrMap;
	
	current_slide	= dynamic_cast<OOX::IFileContainer*>(oox_notes);

	NSCommon::nullable<PPTX::Logic::TxStyles> current_txStyles;
	if (oox_notes->notesStyle.IsInit())
	{
		current_txStyles.Init();
		current_txStyles->otherStyle = oox_notes->notesStyle;
		
		_CP_OPT(int) inStyles = odf_context()->drawing_context()->get_presentation();

		odf_context()->styles_context()->lists_styles().start_style(inStyles && *inStyles > 0);
		for (int i = 0; i < 10; i++)
		{
			if (oox_notes->notesStyle->levels[i].IsInit())
			{
				convert_list_level(oox_notes->notesStyle->levels[i].GetPointer(), i /*- 1*/);
			}
		}
		odf_context()->styles_context()->lists_styles().end_style();
	}
	if (presentation->notesSz.IsInit())
	{
		_CP_OPT(odf_types::length) width	= odf_types::length(presentation->notesSz->cx / 12700., odf_types::length::pt);
		_CP_OPT(odf_types::length) height	= odf_types::length(presentation->notesSz->cy / 12700., odf_types::length::pt);
		
		odf_context()->page_layout_context()->set_page_size(width, height);
	}
	convert_slide(&oox_notes->cSld, current_txStyles.GetPointer(), true, true, NotesMaster);
	
	odp_context->end_note();

	current_clrMap	= old_clrMap;
	current_theme	= old_theme;
}
void PptxConverter::convert(PPTX::NotesSlide *oox_notes)
{
	if (!oox_notes) return;

	PPTX::Theme*			old_theme	= current_theme;
	PPTX::Logic::ClrMap*	old_clrMap	= current_clrMap;
	
	odp_context->start_note();
	
	if (oox_notes->master_.IsInit())
	{
		current_theme	= oox_notes->master_->theme_.GetPointer();
		current_clrMap	= &oox_notes->master_->clrMap;
	}

	current_slide	= dynamic_cast<OOX::IFileContainer*>(oox_notes);

	if (oox_notes->clrMapOvr.IsInit() && oox_notes->clrMapOvr->overrideClrMapping.IsInit())
	{
		current_clrMap	= oox_notes->clrMapOvr->overrideClrMapping.GetPointer();
	}
	
	convert_slide(&oox_notes->cSld, NULL, true, true, Notes);
	
	odp_context->end_note();
	
	current_clrMap	= old_clrMap;
	current_theme	= old_theme;
}

void PptxConverter::convert(OOX::WritingElement  *oox_unknown)
{
	if (oox_unknown == NULL)return;

	switch(oox_unknown->getType())
	{
		case OOX::et_p_EmptyTransition:
			convert(dynamic_cast<PPTX::Logic::EmptyTransition*>(oox_unknown)); break;
		case OOX::et_p_OrientationTransition:
			convert(dynamic_cast<PPTX::Logic::OrientationTransition*>(oox_unknown)); break;
		case OOX::et_p_EightDirectionTransition:
			convert(dynamic_cast<PPTX::Logic::EightDirectionTransition*>(oox_unknown)); break;
		case OOX::et_p_OptionalBlackTransition:
			convert(dynamic_cast<PPTX::Logic::OptionalBlackTransition*>(oox_unknown)); break;
		case OOX::et_p_CornerDirectionTransition:
			convert(dynamic_cast<PPTX::Logic::CornerDirectionTransition*>(oox_unknown)); break;
		case OOX::et_p_SideDirectionTransition:
			convert(dynamic_cast<PPTX::Logic::SideDirectionTransition*>(oox_unknown)); break;
		case OOX::et_p_WheelTransition:
			convert(dynamic_cast<PPTX::Logic::WheelTransition*>(oox_unknown)); break;
		case OOX::et_p_SplitTransition:
			convert(dynamic_cast<PPTX::Logic::SplitTransition*>(oox_unknown)); break;
		case OOX::et_p_ZoomTransition:
			convert(dynamic_cast<PPTX::Logic::ZoomTransition*>(oox_unknown)); break;
		default:
		{
			OoxConverter::convert(oox_unknown);
		}break;
	}
}
void PptxConverter::convert(PPTX::Comments *oox_comments)
{
	if (!oox_comments) return;

	for (size_t i = 0; i < oox_comments->m_arComments.size(); i++)
	{
		PPTX::Logic::Comment & oox_comment = oox_comments->m_arComments[i];

		odp_context->start_comment(oox_comment.idx.get_value_or(-1));
		odp_context->start_comment_content();

			if (oox_comment.pos_x.IsInit() && oox_comment.pos_y.IsInit())
				odp_context->comment_context()->set_position (*oox_comment.pos_x / 10., *oox_comment.pos_y / 10.); //pt
		
			if (oox_comment.authorId.IsInit() && presentation->commentAuthors.IsInit())
			{
				for (size_t a = 0; a < presentation->commentAuthors->m_arAuthors.size(); a++)
				{
					PPTX::Logic::CommentAuthor & autor = presentation->commentAuthors->m_arAuthors[a];

					if (autor.id.IsInit() && autor.id.get() == oox_comment.authorId.get())
					{
						odp_context->comment_context()->set_author(autor.name.get_value_or(L""));
						odp_context->comment_context()->set_initials(autor.initials.get_value_or(L""));
						break;
					}
				}				
			}
			if (oox_comment.dt.IsInit())	odp_context->comment_context()->set_date(*oox_comment.dt);
			if (oox_comment.text.IsInit())	odp_context->text_context()->add_text_content(*oox_comment.text);

		odp_context->end_comment_content();
		odp_context->end_comment();
	}
}
void PptxConverter::convert( PPTX::Logic::Transition *oox_transition )
{
	if (!oox_transition) return;

	if (oox_transition->advTm.is_init())
	{
		odp_context->current_slide().set_page_duration(*oox_transition->advTm);
	}	

	if (oox_transition->base.is_init() == false) return;
	
	odp_context->current_slide().start_transition();
	if (oox_transition->spd.is_init())
	{
		odp_context->current_slide().set_transition_speed(oox_transition->spd->GetBYTECode());
	}
	if (oox_transition->dur.is_init())
	{
		odp_context->current_slide().set_transition_duration(*oox_transition->dur);
	}

	convert(oox_transition->base.base.GetPointer());
	
	if (oox_transition->sndAc.is_init() && oox_transition->sndAc->stSnd.is_init())
	{
		std::wstring sID = oox_transition->sndAc->stSnd->embed.get();
		
		bool bExternal = false;
		std::wstring pathAudio = find_link_by_id(sID, 3, bExternal);
		
		std::wstring odf_ref = odf_context()->add_media(pathAudio, bExternal);

		odp_context->current_slide().set_transition_sound(odf_ref, oox_transition->sndAc->stSnd->loop.get_value_or(false));
	}

	odp_context->current_slide().end_transition();
}
void PptxConverter::convert(PPTX::Logic::Timing *oox_timing)
{
	if (!oox_timing) return;
	if (!oox_timing->tnLst.IsInit()) return;
	
	odp_context->current_slide().start_timing();
	for (size_t i = 0; i < oox_timing->tnLst->list.size(); i++)
	{
		if (oox_timing->tnLst->list[i].is_init() == false) continue;

		convert(&oox_timing->tnLst->list[i]);
	}
	odp_context->current_slide().end_timing();
}
void PptxConverter::convert(PPTX::Logic::TimeNodeBase *oox_time_base)
{
	if (!oox_time_base) return;

	if (oox_time_base->is<PPTX::Logic::Par>()) //Parallel Time
	{
		PPTX::Logic::Par & par = oox_time_base->as<PPTX::Logic::Par>();

		odp_context->current_slide().start_timing_par();
			convert(&par.cTn);
		odp_context->current_slide().end_timing_par();
	}
	else if (oox_time_base->is<PPTX::Logic::Seq>()) //Sequence Time
	{
		PPTX::Logic::Seq & seq = oox_time_base->as<PPTX::Logic::Seq>();
		odp_context->current_slide().start_timing_seq();
			convert(&seq.cTn);
		odp_context->current_slide().end_timing_seq();
	}	
	else if (oox_time_base->is<PPTX::Logic::Set>())
	{
		PPTX::Logic::Set& set = oox_time_base->as<PPTX::Logic::Set>();
		convert(&set);
	}
	else if (oox_time_base->is<PPTX::Logic::AnimEffect>())
	{
		PPTX::Logic::AnimEffect& animEffect = oox_time_base->as<PPTX::Logic::AnimEffect>();
		convert(&animEffect);
	}
	else if (oox_time_base->is<PPTX::Logic::Anim>())
	{
		PPTX::Logic::Anim& animate = oox_time_base->as<PPTX::Logic::Anim>();
		convert(&animate);
	}
	else if (oox_time_base->is<PPTX::Logic::AnimMotion>())
	{
		PPTX::Logic::AnimMotion& motion = oox_time_base->as<PPTX::Logic::AnimMotion>();
		convert(&motion);
	}
	else if (oox_time_base->is<PPTX::Logic::AnimClr>())
	{
		PPTX::Logic::AnimClr& color = oox_time_base->as<PPTX::Logic::AnimClr>();
		convert(&color);
	}
	else if (oox_time_base->is<PPTX::Logic::AnimRot>())
	{
		PPTX::Logic::AnimRot& rotate = oox_time_base->as<PPTX::Logic::AnimRot>();
		convert(&rotate);
	}
	else if (oox_time_base->is<PPTX::Logic::AnimScale>())
	{
		PPTX::Logic::AnimScale& rotate = oox_time_base->as<PPTX::Logic::AnimScale>();
		convert(&rotate);
	}
	else if (oox_time_base->is<PPTX::Logic::Audio>())
	{
		PPTX::Logic::Audio& audio = oox_time_base->as<PPTX::Logic::Audio>();
		convert(&audio);
	}
}
void PptxConverter::convert(PPTX::Logic::EmptyTransition *oox_transition)
{
	if (!oox_transition) return;
	
	if (oox_transition->name == L"random")
		odp_context->current_slide().set_transition_type(40);
	else if (oox_transition->name == L"circle")
	{
		odp_context->current_slide().set_transition_type(16);
		odp_context->current_slide().set_transition_subtype(L"circle");
	}
	else if (oox_transition->name == L"dissolve")
		odp_context->current_slide().set_transition_type(39);
	else if (oox_transition->name == L"diamond")
	{
		odp_context->current_slide().set_transition_type(11);
		odp_context->current_slide().set_transition_subtype(L"diamond");
	}
	else if (oox_transition->name == L"newsflash")
		odp_context->current_slide().set_transition_type(24);	//fanWipe
	else if (oox_transition->name == L"plus")
	{
		odp_context->current_slide().set_transition_type(19);	//starWipe
		odp_context->current_slide().set_transition_subtype(L"fourPoint");
	}
	else if (oox_transition->name == L"wedge")
		odp_context->current_slide().set_transition_type(24);	//fanWipe
	else if (oox_transition->name == L"vortex")
	{
		odp_context->current_slide().set_transition_type(20);	//miscShapeWipe
		odp_context->current_slide().set_transition_subtype(L"vertical");
	}
	else if (oox_transition->name == L"doors")
	{
		odp_context->current_slide().set_transition_type(0);	//barWipe	
		odp_context->current_slide().set_transition_subtype(L"topToBottom");
		odp_context->current_slide().set_transition_direction(L"reverse");
	}
	else if (oox_transition->name == L"prism")
	{
		odp_context->current_slide().set_transition_type(20);	//miscShapeWipe
		odp_context->current_slide().set_transition_subtype(L"cornersOut");
	}
	else if (oox_transition->name == L"switch")
	{
		odp_context->current_slide().set_transition_type(20);	//miscShapeWipe
		odp_context->current_slide().set_transition_subtype(L"topToBottom");
	}
	else 
	{
		odp_context->current_slide().set_transition_type(36);	//fade
	}
}
void PptxConverter::convert(PPTX::Logic::OrientationTransition *oox_transition)
{
	if (!oox_transition) return;

	if (oox_transition->name == L"comb")
	{
		odp_context->current_slide().set_transition_type(34);

		if (oox_transition->dir.IsInit())
		{
			if (oox_transition->dir->get() == L"horz")
				odp_context->current_slide().set_transition_subtype(L"combHorizontal");
			if (oox_transition->dir->get() == L"vert")
				odp_context->current_slide().set_transition_subtype(L"combVertical");
		}
	}
	else
	{
		if (oox_transition->name == L"blinds")
			odp_context->current_slide().set_transition_type(38);
		else if (oox_transition->name == L"checker")
			odp_context->current_slide().set_transition_type(37);
		else if (oox_transition->name == L"randomBar")
			odp_context->current_slide().set_transition_type(40);
		
		//if (oox_transition->dir.IsInit())
		//{
		//	if (oox_transition->dir->get() == L"horz")
		//		odp_context->current_slide().set_transition_subtype(L"combHorizontal");
		//	if (oox_transition->dir->get() == L"vert")
		//		odp_context->current_slide().set_transition_subtype(L"combVertical");
		//}
	}
}
void PptxConverter::convert(PPTX::Logic::EightDirectionTransition	*oox_transition)
{
	if (!oox_transition) return;

	if (oox_transition->name == L"cover")
		odp_context->current_slide().set_transition_type(1);
	if (oox_transition->name == L"pull")
		odp_context->current_slide().set_transition_type(35);

	if (oox_transition->dir.IsInit())
	{
			 if (oox_transition->dir->get() == L"d")	odp_context->current_slide().set_transition_subtype(L"fromTop");
		else if (oox_transition->dir->get() == L"l")	odp_context->current_slide().set_transition_subtype(L"fromRight");
		else if (oox_transition->dir->get() == L"r")	odp_context->current_slide().set_transition_subtype(L"fromLeft");
		else if (oox_transition->dir->get() == L"u")	odp_context->current_slide().set_transition_subtype(L"fromBottom");

		else if (oox_transition->dir->get() == L"rd")	odp_context->current_slide().set_transition_subtype(L"horizontalLeft");
		else if (oox_transition->dir->get() == L"lu")	odp_context->current_slide().set_transition_subtype(L"horizontalRight");
		else if (oox_transition->dir->get() == L"ld")	odp_context->current_slide().set_transition_subtype(L"verticalRight");
		else if (oox_transition->dir->get() == L"ru")	odp_context->current_slide().set_transition_subtype(L"verticalLeft");
	}
}
void PptxConverter::convert(PPTX::Logic::OptionalBlackTransition *oox_transition)
{
	if (!oox_transition) return;

	if (oox_transition->name == L"cut")
		odp_context->current_slide().set_transition_type(36);
	if (oox_transition->name == L"fade")
		odp_context->current_slide().set_transition_type(36);
}
void PptxConverter::convert(PPTX::Logic::SideDirectionTransition *oox_transition)
{
	if (!oox_transition) return;

	if (oox_transition->name == L"push")
		odp_context->current_slide().set_transition_type(34);
	if (oox_transition->name == L"wipe")
		odp_context->current_slide().set_transition_type(0);
	
	if (oox_transition->dir.IsInit())
	{
			 if (oox_transition->dir->get() == L"d")	odp_context->current_slide().set_transition_subtype(L"fromTop");
		else if (oox_transition->dir->get() == L"l")	odp_context->current_slide().set_transition_subtype(L"fromRight");
		else if (oox_transition->dir->get() == L"r")	odp_context->current_slide().set_transition_subtype(L"fromLeft");
		else if (oox_transition->dir->get() == L"u")	odp_context->current_slide().set_transition_subtype(L"fromBottom");
	}
}
void PptxConverter::convert(PPTX::Logic::CornerDirectionTransition	*oox_transition)
{
	if (!oox_transition) return;
	//name == strips
	odp_context->current_slide().set_transition_type(4);

	if (oox_transition->dir.IsInit())
	{
			 if (oox_transition->dir->get() == L"rd") odp_context->current_slide().set_transition_subtype(L"horizontalLeft");
		else if (oox_transition->dir->get() == L"lu") odp_context->current_slide().set_transition_subtype(L"horizontalRight");
		else if (oox_transition->dir->get() == L"ld") odp_context->current_slide().set_transition_subtype(L"verticalRight");
		else if (oox_transition->dir->get() == L"ru") odp_context->current_slide().set_transition_subtype(L"verticalLeft");
	}
}
void PptxConverter::convert(PPTX::Logic::WheelTransition *oox_transition)
{
	if (!oox_transition) return;
	//name == wheel
	odp_context->current_slide().set_transition_type(22);

	switch (oox_transition->spokes.get_value_or(0))
	{
	case 1: odp_context->current_slide().set_transition_subtype(L"oneBlade");	break;
	case 3: odp_context->current_slide().set_transition_subtype(L"threeBlade");	break;
	case 4: odp_context->current_slide().set_transition_subtype(L"fourBlade");	break;
	case 8: odp_context->current_slide().set_transition_subtype(L"eightBlade");	break;
	}
}
void PptxConverter::convert(PPTX::Logic::SplitTransition *oox_transition)
{
	if (!oox_transition) return;
	//name == split
	odp_context->current_slide().set_transition_type(3);
	odp_context->current_slide().set_transition_subtype(L"vertical");
}
void PptxConverter::convert(PPTX::Logic::ZoomTransition *oox_transition)
{
	if (!oox_transition) return;
	//name == zoom
	odp_context->current_slide().set_transition_type(11);
}

struct preset_subtype_maping
{
	int							OOX_PresetSubtype;
	std::wstring				ODF_PresetSubtype;
};

static const preset_subtype_maping s_preset_subtype_maping[] =
{
	{   1,  L"from-top" },
	{   2,  L"from-right" },
	{   3,  L"from-top-right" },
	{   4,  L"from-bottom" },
	{   5,  L"horizontal" },
	{   6,  L"from-bottom-right" },
	{   8,  L"from-left" },
	{   9,  L"from-top-left" },
	{  10,  L"vertical" },
	{  12,  L"from-bottom-left" },
	{  16,  L"in" },
	{  21,  L"vertical-in" },
	{  26,  L"horizontal-in" },
	{  32,  L"out" },
	{  36,  L"out-from-screen-center" },
	{  37,  L"vertical-out" },
	{  42,  L"horizontal-out" },
	{  272, L"in-slightly" },
	{  288, L"out-slightly" },
	{  528, L"in-from-screen-center" },
	{  0,   L"" }
};

static std::wstring convert_subtype(PPTX::Limit::TLPresetClass preset_class_, int preset_id_, int preset_subtype_)
{
	_CP_OPT(std::wstring) subtype;

	const unsigned char entrance_bytecode	= 1;
	const unsigned char exit_bytecode		= 2;

	if ((preset_class_.GetBYTECode() == entrance_bytecode) || (preset_class_.GetBYTECode() == exit_bytecode))
	{
		// skip wheel effect
		if (preset_id_ != 21)
		{
			if (preset_id_ == 5)
			{
				// checkerboard
				switch (preset_subtype_)
				{
				case  5: subtype = L"downward"; break;
				case 10: subtype = L"across"; break;
				default: subtype = boost::none;
				}
			}
			else if (preset_id_ == 17)
			{
				// stretch
				if (preset_subtype_ == 10)
					subtype = L"across";
			}
			else if (preset_id_ == 18)
			{
				// strips
				switch (preset_subtype_)
				{
				case 3: subtype = L"right-to-top"; break;
				case 6: subtype = L"right-to-bottom"; break;
				case 9: subtype = L"left-to-top"; break;
				case 12: subtype = L"left-to-bottom"; break;
				default: subtype = boost::none;
				}
			}
			else if (preset_id_ == 6)
			{
				switch (preset_subtype_)
				{
				case 16: subtype = L"out"; break;
				case 32: subtype = L"in"; break;
				default: subtype = boost::none;
				}
			}

			if (!subtype)
			{
				const preset_subtype_maping* p = s_preset_subtype_maping;

				while (p->OOX_PresetSubtype != 0)
				{
					if (p->OOX_PresetSubtype == preset_subtype_)
					{
						subtype = p->ODF_PresetSubtype;
						break;
					}
					p++;
				}
			}
		}
	}

	if (!subtype && preset_subtype_ != 0)
		return std::to_wstring(preset_subtype_);

	return subtype.get_value_or(std::to_wstring(preset_subtype_));
}

static double convert_acceleration(int acceleration)
{
	const double odf_multiplier = 100000.0;
	return acceleration / odf_multiplier;
}

void PptxConverter::convert(PPTX::Logic::CTn *oox_time_common)
{
	if (!oox_time_common) return;

	if (oox_time_common->id.IsInit())
	{
		odp_context->current_slide().set_anim_id(*oox_time_common->id);
	}
	if (oox_time_common->nodeType.IsInit())
	{
		convert(*oox_time_common->nodeType);
	}
	if (oox_time_common->dur.IsInit())
	{
		if (*oox_time_common->dur == L"indefinite")
			odp_context->current_slide().set_anim_duration(-1);
		else
			odp_context->current_slide().set_anim_duration(XmlUtils::GetInteger(*oox_time_common->dur));
	}
	if (oox_time_common->restart.IsInit())
	{
		odp_context->current_slide().set_anim_restart(oox_time_common->restart->get());
	}
	if (oox_time_common->fill.IsInit())
	{
		odp_context->current_slide().set_anim_fill(odf_types::smil_fill::parse(oox_time_common->fill->get()));
	}
	if (oox_time_common->autoRev.IsInit())
	{
		odp_context->current_slide().set_anim_auto_reverse(*oox_time_common->autoRev);
	}
	if (oox_time_common->presetClass.IsInit())
	{
		convert(*oox_time_common->presetClass);
		if (oox_time_common->presetID.IsInit())
		{
			convert(*oox_time_common->presetClass, *oox_time_common->presetID);

			if (oox_time_common->presetSubtype.IsInit())
			{
				const std::wstring odf_subtype = convert_subtype(
					*oox_time_common->presetClass,
					*oox_time_common->presetID,
					*oox_time_common->presetSubtype);
				if(!odf_subtype.empty())
					odp_context->current_slide().set_anim_subtype(odf_subtype);
			}
		}
	}
	if (oox_time_common->accel.IsInit())
		odp_context->current_slide().set_anim_accelerate(convert_acceleration(*oox_time_common->accel));
	if (oox_time_common->decel.IsInit())
		odp_context->current_slide().set_anim_decelerate(convert_acceleration(*oox_time_common->decel));

	

	//nullable<CondLst>			stCondLst;
	//nullable<CondLst>			endCondLst;
	//nullable<Cond>			endSync;
	//nullable<Iterate>			iterate;

    // TODO
    //for (auto& child : oox_time_common->childTnLst)
    //{
    //    for (size_t i = 0; i <child.m_node. .list.size(); i++)
    //    {
    //        if (tnLst.list[i].is_init() == false) continue;

    //        convert(&oox_time_common->childTnLst->list[i]);
    //    }
    //}
	
	if (oox_time_common->stCondLst.IsInit())
	{
		for (size_t i = 0; i < oox_time_common->stCondLst->list.size(); i++)
		{
			PPTX::Logic::Cond& cond = oox_time_common->stCondLst->list[i];
			convert(&cond);
		}
	}

	if (oox_time_common->childTnLst.IsInit())
	{
		for (size_t i = 0; i < oox_time_common->childTnLst->list.size(); i++)
		{
			PPTX::Logic::TimeNodeBase& child = oox_time_common->childTnLst->list[i];
			if(child.is_init())
				convert(&child);
		}
	}
	

//	if (oox_time_common->childTnLst.IsInit())
//	{
//		for (size_t i = 0; i < oox_time_common->childTnLst->list.size(); i++)
//		{
//			if (oox_time_common->childTnLst->list[i].is_init() == false) continue;

//			convert(&oox_time_common->childTnLst->list[i]);
//		}
//	}
	if (oox_time_common->subTnLst.IsInit())
	{
		for (size_t i = 0; i < oox_time_common->subTnLst->list.size(); i++)
		{
			if (oox_time_common->subTnLst->list[i].is_init() == false) continue;

			convert(&oox_time_common->subTnLst->list[i]);
		}
	}
}


void PptxConverter::convert(PPTX::Logic::Cond* oox_condition)
{
	if (!oox_condition)
		return;

	std::wstring begin = L"0s";

	if (oox_condition->delay.IsInit())
	{
		std::wstring delay;
		if (*oox_condition->delay == L"indefinite")
			delay = L"next";
		else
		{
			int ms = XmlUtils::GetInteger(*oox_condition->delay);
			std::wstringstream ss;
			ss << ms / 1000.0 << L"s";
			delay = ss.str();
		}

		if (!interactive_animation_element_id.empty())
		{
			delay = interactive_animation_element_id + L".click+" + delay;
			interactive_animation_element_id = std::wstring();
		}
			

		begin = delay;
	}
	
	if (oox_condition->tgtEl.IsInit())
	{
		if (oox_condition->tgtEl->spTgt.IsInit())
		{
			std::wstring id = odp_context->get_mapped_identifier(oox_condition->tgtEl->spTgt->spid);
			interactive_animation_element_id = id;
		}	
	}

	odp_context->current_slide().set_anim_begin(begin);

	//else if(oox_condition->evt.IsInit())
	//	odp_context->current_slide().set_anim_evt();
}

void PptxConverter::convert(PPTX::Logic::Set* oox_set)
{
	if (!oox_set)
		return;

	odp_context->current_slide().start_timing_set();

	convert(&oox_set->cBhvr);
	if(oox_set->to.IsInit())
		convert(oox_set->to.GetPointer());

	odp_context->current_slide().end_timing_set();
}

void PptxConverter::convert(PPTX::Logic::TableProperties *oox_table_pr)
{
	if (!oox_table_pr) return;

	odp_context->drawing_context()->start_area_properties();
		odp_context->drawing_context()->set_no_fill();
	odp_context->drawing_context()->end_area_properties();
	
	if (oox_table_pr->TableStyleId.IsInit())
	{
		std::map<std::wstring, odf_writer::table_style_state>::iterator pFind;
		pFind = odp_context->map_table_styles_.find(oox_table_pr->TableStyleId.get());
		
		if (pFind == odp_context->map_table_styles_.end())
		{
			smart_ptr<PPTX::TableStyles> table_styles_ptr = presentation->Get(OOX::Presentation::FileTypes::TableStyles).smart_dynamic_cast<PPTX::TableStyles>();
			std::map<std::wstring, PPTX::Logic::TableStyle>::iterator pFind1 = table_styles_ptr->Styles.find(oox_table_pr->TableStyleId.get());		
			
			if (pFind1 != table_styles_ptr->Styles.end())
				convert(pFind1->first, dynamic_cast<PPTX::Logic::TableStyle*>(&pFind1->second));

			pFind = odp_context->map_table_styles_.find(oox_table_pr->TableStyleId.get());
		}
		
		if (pFind != odp_context->map_table_styles_.end())
		{
			odp_context->slide_context()->table_context()->set_table_styled(true);

			odf_writer::table_style_state & table_style = pFind->second;

			if (!table_style.default_.empty())
				odp_context->slide_context()->table_context()->set_default_cell_properties(table_style.default_);
			
			if (oox_table_pr->FirstRow.get_value_or(false) && !table_style.first_row_.empty())
			{
				odp_context->slide_context()->table_context()->set_first_row_cell_properties(table_style.first_row_);
			}
			if (oox_table_pr->FirstCol.get_value_or(false) && !table_style.first_col_.empty())
			{
				odp_context->slide_context()->table_context()->set_first_col_cell_properties(table_style.first_col_);
			}
			if (oox_table_pr->BandRow.get_value_or(false) && !table_style.band_row_.empty())
			{
				odp_context->slide_context()->table_context()->set_band_row_cell_properties(table_style.band_row_);
			}
			if (oox_table_pr->BandCol.get_value_or(false) && !table_style.band_col_.empty())
			{
				odp_context->slide_context()->table_context()->set_band_col_cell_properties(table_style.band_col_);
			}
			if (oox_table_pr->LastRow.get_value_or(false) && !table_style.last_row_.empty())
			{
				odp_context->slide_context()->table_context()->set_last_row_cell_properties(table_style.last_row_);
			}
			if (oox_table_pr->LastCol.get_value_or(false) && !table_style.last_col_.empty())
			{
				odp_context->slide_context()->table_context()->set_last_col_cell_properties(table_style.last_col_);
			}
		}
	}

	if (oox_table_pr->Fill.is_init())
	{
		odp_context->styles_context()->create_style(L"", odf_types::style_family::TableCell, true, false, -1);
		odf_writer::odf_style_state_ptr style_state = odp_context->styles_context()->last_state(style_family::TableCell);
		if (style_state)
		{
			odp_context->drawing_context()->start_area_properties();
				OoxConverter::convert(&oox_table_pr->Fill);
			odp_context->drawing_context()->end_area_properties();

			odf_writer::graphic_format_properties *temp_props = odp_context->drawing_context()->get_graphic_properties();
	
			style_state->get_graphic_properties()->apply_from(*temp_props);
		}
		odp_context->slide_context()->table_context()->set_default_cell_properties(style_state->get_name());
	}
	//EffectProperties			Effects;
	//nullable_bool				Rtl;
}

void PptxConverter::convert(PPTX::Logic::Table *oox_table)
{
	if (!oox_table) return;

	odp_context->slide_context()->start_table();
	
	convert(oox_table->tableProperties.GetPointer());

	for (size_t i = 0; i < oox_table->TableCols.size(); i++)
	{
		double width = -1;

		if (oox_table->TableCols[i].Width.IsInit())
			width = oox_table->TableCols[i].Width.get() / 12700.;

		odp_context->slide_context()->add_table_column(width);

	}

	odp_context->slide_context()->table_context()->set_table_size(oox_table->TableCols.size(), oox_table->TableRows.size());

	for (size_t i = 0; i < oox_table->TableRows.size(); i++)
	{
		convert(&oox_table->TableRows[i]);
	}

	odp_context->slide_context()->end_table();
}
void PptxConverter::convert(PPTX::Logic::TableRow *oox_table_row)
{
	if (!oox_table_row) return;

	odp_context->slide_context()->start_table_row(oox_table_row->Height.IsInit());

	if (oox_table_row->Height.IsInit())
	{
		odf_writer::style_table_row_properties	*table_row_properties = odp_context->styles_context()->last_state()->get_table_row_properties();
		table_row_properties->style_table_row_properties_attlist_.style_row_height_ = odf_types::length(oox_table_row->Height.get() / 12700., odf_types::length::pt); 
//		table_row_properties->style_table_row_properties_attlist_.style_min_row_height_
	}

	for (size_t i = 0; i < oox_table_row->Cells.size(); i++)
	{
		convert(&oox_table_row->Cells[i], (int)i);
	}
	odp_context->slide_context()->end_table_row();
}
void PptxConverter::convert(PPTX::Logic::TableCell *oox_table_cell, int numCol)
{
	if (!oox_table_cell) return;

	bool covered = false, styled = false;

	if (oox_table_cell->VMerge.IsInit() || oox_table_cell->HMerge.IsInit())
		covered = true; 

	if (oox_table_cell->CellProperties.IsInit())
	{
		styled = (oox_table_cell->CellProperties->is_empty == false);
	}
	
	odp_context->slide_context()->start_table_cell(numCol, covered, styled);
	
	if (oox_table_cell->RowSpan.IsInit())
		odp_context->slide_context()->table_context()->set_cell_row_span(oox_table_cell->RowSpan.get());

	if (oox_table_cell->GridSpan.IsInit())
		odp_context->slide_context()->table_context()->set_cell_column_span(oox_table_cell->GridSpan.get());
	
	convert(oox_table_cell->CellProperties.GetPointer(), numCol + 1);
	OoxConverter::convert(oox_table_cell->txBody.GetPointer());

	//nullable_string					Id;
	odp_context->slide_context()->end_table_cell();
}
bool PptxConverter::convert(PPTX::Logic::TableCellProperties *oox_table_cell_pr, int col)
{
	if (!oox_table_cell_pr)				return false;
	if (oox_table_cell_pr->is_empty)	return false;

	bool is_base_styled = odp_context->slide_context()->table_context()->is_styled();
	
	if (col < 0)	col = odp_context->slide_context()->table_context()->current_column() + 1;
	int				row = odp_context->slide_context()->table_context()->current_row();
		
	odf_writer::style_table_cell_properties * parent_cell_properties = NULL;

	std::wstring parent_name = odp_context->slide_context()->table_context()->get_default_cell_properties();
	
	if (false == parent_name.empty()) 
	{
		odf_writer::style * style_ = NULL;		
		if (odp_context->styles_context()->find_odf_style(parent_name, odf_types::style_family::TableCell, style_))
		{
			parent_cell_properties = style_->content_.add_get_style_table_cell_properties();
		}
	}
	
	if (oox_table_cell_pr == NULL && is_base_styled == false && parent_cell_properties == NULL) return false;
	

	odf_writer::style_table_cell_properties *cell_properties = odp_context->styles_context()->last_state()->get_table_cell_properties();

	if (cell_properties == NULL) return false;

	cell_properties->apply_from(parent_cell_properties);

//check for inside cell or not

	_CP_OPT(std::wstring) border_inside_v = odp_context->slide_context()->table_context()->get_table_inside_v();
	_CP_OPT(std::wstring) border_inside_h = odp_context->slide_context()->table_context()->get_table_inside_h();
	
	if ((border_inside_v || border_inside_h))
	{
		if (cell_properties->content_.common_border_attlist_.fo_border_)//раскидаем по сторонам
		{
			if (cell_properties->content_.common_border_attlist_.fo_border_->is_none() == false)
			{
				cell_properties->content_.common_border_attlist_.fo_border_left_ = 
				cell_properties->content_.common_border_attlist_.fo_border_right_ =
				cell_properties->content_.common_border_attlist_.fo_border_bottom_ = 
				cell_properties->content_.common_border_attlist_.fo_border_top_ =
												cell_properties->content_.common_border_attlist_.fo_border_;
			}		
			cell_properties->content_.common_border_attlist_.fo_border_ = boost::none;
		}	
		//если нет убрать, если да - добавить
		if (border_inside_h)
		{
			bool del_border = (std::wstring::npos != border_inside_h->find(L"none"));
			if (row != 1)
			{
				if (cell_properties->content_.common_border_attlist_.fo_border_top_ && del_border)
					cell_properties->content_.common_border_attlist_.fo_border_top_ = boost::none;

				else if (border_inside_h && !del_border)
					cell_properties->content_.common_border_attlist_.fo_border_top_ = *border_inside_h;
			}
			if (row != odp_context->slide_context()->table_context()->count_rows())
			{
				if (cell_properties->content_.common_border_attlist_.fo_border_bottom_ && del_border)
					cell_properties->content_.common_border_attlist_.fo_border_bottom_ = boost::none;

				else if (border_inside_h && !del_border)
					cell_properties->content_.common_border_attlist_.fo_border_bottom_ = *border_inside_h;
			}
		}
		if (border_inside_v)
		{
			bool del_border = (std::wstring::npos != border_inside_v->find(L"none"));
			if (col != 1)
			{
				if (cell_properties->content_.common_border_attlist_.fo_border_left_ && del_border)
					cell_properties->content_.common_border_attlist_.fo_border_left_ = boost::none;

				else if (border_inside_h && !del_border)
					cell_properties->content_.common_border_attlist_.fo_border_left_ = *border_inside_h;
			}
			if (col != odp_context->slide_context()->table_context()->count_columns())
			{
				if (cell_properties->content_.common_border_attlist_.fo_border_right_ && del_border)
					cell_properties->content_.common_border_attlist_.fo_border_right_ = boost::none;

				else if (border_inside_h && !del_border)
					cell_properties->content_.common_border_attlist_.fo_border_right_ = *border_inside_h;
			}
		}
	}
	bool res = convert(oox_table_cell_pr); //last state

	return true;
}
void PptxConverter::convert(PPTX::Logic::Ln *oox_border, std::wstring & odf_border_prop)
{
	odf_border_prop = L"";
	if (!oox_border) return;

	odp_context->drawing_context()->start_line_properties();
		OoxConverter::convert(oox_border);
	odp_context->drawing_context()->end_line_properties();
//-----------------------------------------------------------------------------------------------------
	std::wstringstream	border_style;
	std::wstring		border_color;

	odf_types::length length = odf_types::length(oox_border->w.IsInit() ? oox_border->w.get() / 12700. : 1,  odf_types::length::pt);
	border_style << length;
	
	_CP_OPT(odf_types::color) color = odp_context->drawing_context()->get_line_color();

	if (oox_border->prstDash.IsInit() && oox_border->prstDash->val.IsInit())
	{
		switch(oox_border->prstDash->val->GetBYTECode())
		{
			case 0:		
			case 3:		
			case 7:		
			case 1:		border_style << L" dashed";		break;
			case 10:
			case 2:		border_style << L" dotted";		break;
			case 4:
			case 8:		border_style << L" dash-dot";	break;
			case 5:		
			case 9:		border_style << L" dash-dot-dot";break;
			case 6:
			default:	border_style << L" solid";
		}
	}else border_style << L" solid";
	
	if (color)	border_style << L" " << color.get();
	else		border_style << L" #000000";

	odf_border_prop = border_style.str();
}
void PptxConverter::convert(PPTX::Logic::TcBdr *oox_table_borders)
{
	if (!oox_table_borders) return;

	//НИ ГРАФИКА НИ СВОЙСТВА ЯЧЕЕК .. ПАРАГРАФ блять !! - идиетский odf !!!
	//odf_writer::style_table_cell_properties *odf_cell_props = odp_context->styles_context()->last_state(odf_types::style_family::TableCell)->get_table_cell_properties();
	odf_writer::paragraph_format_properties *odf_para_props = odp_context->styles_context()->last_state(odf_types::style_family::TableCell)->get_paragraph_properties();

	convert(oox_table_borders, odf_para_props);
}


void PptxConverter::convert(PPTX::Logic::TcBdr *oox_table_borders, odf_writer::paragraph_format_properties *odf_para_props)
{
	if (!oox_table_borders) return;
	if (!odf_para_props) return;

	std::wstring left, right, top, bottom, other2BR, other2BL;
	
	if (oox_table_borders->left.IsInit())
	{
		convert(oox_table_borders->left->ln.GetPointer(), left);
		//if (left.empty())
		//	convert(oox_table_borders->left->lnRef.GetPointer(), left); //todooo
	}
	if (oox_table_borders->right.IsInit())
	{
		convert(oox_table_borders->right->ln.GetPointer(), right);
	}
	if (oox_table_borders->top.IsInit())
	{
		convert(oox_table_borders->top->ln.GetPointer(), top);
	}
	if (oox_table_borders->bottom.IsInit())
	{
		convert(oox_table_borders->bottom->ln.GetPointer(), bottom);
	}
	if (oox_table_borders->tl2br.IsInit())
	{
		convert(oox_table_borders->tl2br->ln.GetPointer(), other2BR);
	}
	if (oox_table_borders->tr2bl.IsInit())
	{
		convert(oox_table_borders->tr2bl->ln.GetPointer(), other2BL);
	}

	if (bottom == top && top == left && left== right && bottom.length() > 0)
	{
		odf_para_props->common_border_attlist_.fo_border_ = left;

		odf_para_props->common_border_attlist_.fo_border_bottom_ =
		odf_para_props->common_border_attlist_.fo_border_top_	= 
		odf_para_props->common_border_attlist_.fo_border_left_ = 
		odf_para_props->common_border_attlist_.fo_border_right_ = boost::none;
	}
	else
	{
		if (odf_para_props->common_border_attlist_.fo_border_)
		{
			odf_para_props->common_border_attlist_.fo_border_bottom_	= 
			odf_para_props->common_border_attlist_.fo_border_top_		= 
			odf_para_props->common_border_attlist_.fo_border_left_		= 
			odf_para_props->common_border_attlist_.fo_border_right_	= odf_para_props->common_border_attlist_.fo_border_;
		}
		odf_para_props->common_border_attlist_.fo_border_ = boost::none;

		if ( !bottom.empty() )	odf_para_props->common_border_attlist_.fo_border_bottom_	= bottom;
		if ( !top.empty() )		odf_para_props->common_border_attlist_.fo_border_top_		= top;
		if ( !left.empty() )	odf_para_props->common_border_attlist_.fo_border_left_		= left;
		if ( !right.empty() )	odf_para_props->common_border_attlist_.fo_border_right_	= right;
	}
	//if (other2BR.empty() == false)	odf_para_props->style_diagonal_tl_br_ = other2BR;
	//if (other2BL.empty() == false)	odf_para_props->style_diagonal_bl_tr_ = other2BL;
}

bool PptxConverter::convert(PPTX::Logic::TableCellProperties *oox_table_cell_pr)
{
	if (!oox_table_cell_pr)	return false;
	
	odf_writer::style_table_cell_properties *odf_cell_props = odp_context->slide_context()->get_styles_context()->last_state()->get_table_cell_properties();
	odf_writer::paragraph_format_properties *odf_para_props	= odp_context->slide_context()->get_styles_context()->last_state(odf_types::style_family::TableCell)->get_paragraph_properties();
	
	if (!odf_para_props)	return false;

	if (oox_table_cell_pr->Fill.is_init())
	{
		odp_context->drawing_context()->start_area_properties();
			OoxConverter::convert(&oox_table_cell_pr->Fill);
		odp_context->drawing_context()->end_area_properties();
	}

	//if (oox_table_cell_pr->Vert.IsInit())
	//{
	//	switch(oox_table_cell_pr->Vert->GetBYTECode())
	//	{
	//	case 1  :
	//		odf_para_props->style_direction_ = odf_types::direction(odf_types::direction::Ltr);break;
	//	case 6 ://rtl vert
	//		break;
	//	default:
	//		odf_para_props->style_direction_ = odf_types::direction(odf_types::direction::Ttb);break;
	//	}
	//}
	if (oox_table_cell_pr->MarL.IsInit())
	{
		odf_cell_props->content_.common_padding_attlist_.fo_padding_left_ = odf_types::length(oox_table_cell_pr->MarL.get() / 12700., odf_types::length::pt);
	}
	if (oox_table_cell_pr->MarR.IsInit())
	{
		odf_cell_props->content_.common_padding_attlist_.fo_padding_right_ = odf_types::length(oox_table_cell_pr->MarR.get() / 12700., odf_types::length::pt);
	}
	if (oox_table_cell_pr->MarT.IsInit())
	{
		odf_cell_props->content_.common_padding_attlist_.fo_padding_top_ = odf_types::length(oox_table_cell_pr->MarT.get() / 12700., odf_types::length::pt);
	}
	if (oox_table_cell_pr->MarB.IsInit())
	{
		odf_cell_props->content_.common_padding_attlist_.fo_padding_bottom_ = odf_types::length(oox_table_cell_pr->MarB.get() / 12700., odf_types::length::pt);
	}
	if (oox_table_cell_pr->Vert.IsInit())
	{
		switch(oox_table_cell_pr->Vert->GetBYTECode())
		{
		default:
			break;
		//case SimpleTypes::verticaljcBoth   : //??????
		//	odf_para_props->style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Justify); break;
		//case SimpleTypes::verticaljcBottom :
		//	odf_para_props->style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Bottom); break;
		//case SimpleTypes::verticaljcCenter :
		//	odf_para_props->style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Middle); break;
		//case SimpleTypes::verticaljcTop    :
		//	odf_para_props->style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Top); break;
		}
	}
//borders
	std::wstring left, right, top, bottom, other;

	convert(oox_table_cell_pr->LnB.GetPointer(), bottom);
	convert(oox_table_cell_pr->LnT.GetPointer(), top);
	convert(oox_table_cell_pr->LnL.GetPointer(), left);
	convert(oox_table_cell_pr->LnR.GetPointer(), right);

	if (bottom == top && top == left && left== right && bottom.length() > 0)
	{
		odf_para_props->common_border_attlist_.fo_border_ = left;

		odf_para_props->common_border_attlist_.fo_border_bottom_ =
		odf_para_props->common_border_attlist_.fo_border_top_	= 
		odf_para_props->common_border_attlist_.fo_border_left_ = 
		odf_para_props->common_border_attlist_.fo_border_right_ = boost::none;
	}
	else
	{
		if (odf_para_props->common_border_attlist_.fo_border_)
		{
			odf_para_props->common_border_attlist_.fo_border_bottom_	= 
			odf_para_props->common_border_attlist_.fo_border_top_		= 
			odf_para_props->common_border_attlist_.fo_border_left_		= 
			odf_para_props->common_border_attlist_.fo_border_right_	= odf_para_props->common_border_attlist_.fo_border_;
		}
		odf_para_props->common_border_attlist_.fo_border_ = boost::none;

		if ( !bottom.empty() )	odf_para_props->common_border_attlist_.fo_border_bottom_	= bottom;
		if ( !top.empty() )		odf_para_props->common_border_attlist_.fo_border_top_		= top;
		if ( !left.empty() )	odf_para_props->common_border_attlist_.fo_border_left_		= left;
		if ( !right.empty() )	odf_para_props->common_border_attlist_.fo_border_right_	= right;
	}
	//convert(oox_border->m_oTL2BR.GetPointer()	, other);
	//if (other.empty() == false) odf_para_props->style_diagonal_tl_br_ = other;
	//
	//convert(oox_border->m_oTR2BL.GetPointer()	, other);
	//if (other.empty() == false)
	//{
	//	odf_para_props->style_diagonal_bl_tr_ = other;
	//}	
	return true;
}

void PptxConverter::convert(PPTX::Logic::Bg *oox_background)
{
	if (!oox_background) return;
	if (!oox_background->bgPr.IsInit() && !oox_background->bgRef.IsInit())return;
	
	odp_context->drawing_context()->start_drawing();
	odp_context->drawing_context()->set_background_state(true);
		if (oox_background->bgPr.IsInit())
		{
			OoxConverter::convert(&oox_background->bgPr->Fill);
	
			convert(oox_background->bgPr->EffectList.List.GetPointer());
	//nullable_bool			shadeToTitle;
		}
		else if (oox_background->bgRef.IsInit())
		{
			OoxConverter::convert(oox_background->bgRef.GetPointer(), 1);
		}
	odf_writer::style* page_style_ = dynamic_cast<odf_writer::style*>(odp_context->current_slide().page_style_elm_.get());
	odf_writer::style_drawing_page_properties* page_props = page_style_->content_.add_get_style_drawing_page_properties();
	
	//необязательно
	//if (page_props->content_.common_draw_fill_attlist_.draw_fill_image_name_)
	//{
	//	page_props->content_.draw_background_size_ = L"border";
	//}

	odp_context->drawing_context()->end_drawing_background(page_props->content_.common_draw_fill_attlist_);
	odp_context->drawing_context()->set_background_state(false);

	odp_context->drawing_context()->clear();
}

void PptxConverter::convert_slide(PPTX::Logic::CSld *oox_slide, PPTX::Logic::TxStyles* txStyles, bool bPlaceholders, bool bFillUp, _typePages type)
{
	if (oox_slide == NULL) return;

	if (current_theme && current_clrMap)
		current_theme->SetColorMap(*current_clrMap);

	std::wstring page_name = get_page_name(oox_slide, type);
	odp_context->current_slide().set_page_name(page_name);

	if (type != Notes && type != NotesMaster)
	{
		convert(oox_slide->bg.GetPointer());
	}
		
	for (size_t i = 0 ; i < oox_slide->spTree.SpTreeElems.size(); i++)
	{
		smart_ptr<PPTX::WrapperWritingElement>	pElem = oox_slide->spTree.SpTreeElems[i].GetElem();
		
		smart_ptr<PPTX::Logic::Shape>			pShape	= pElem.smart_dynamic_cast<PPTX::Logic::Shape>();
		smart_ptr<PPTX::Logic::Pic>				pPic	= pElem.smart_dynamic_cast<PPTX::Logic::Pic>();

		odf_context()->drawing_context()->start_drawing();

		PPTX::Logic::NvPr *pNvPr = NULL;
		
		if (pShape.IsInit())	pNvPr = &pShape->nvSpPr.nvPr;
		if (pPic.IsInit())		pNvPr = &pPic->nvPicPr.nvPr;

		bool bConvert = false;
		bool bPlaceholder = false;

		if ((pNvPr) && (pNvPr->ph.is_init()))
		{
			bPlaceholder = true;
			
			if (type == Notes || type == NotesMaster)
			{
				if (pShape.IsInit())	pShape->nvSpPr.nvPr.ph->idx.reset();
				if (pPic.IsInit())		pPic->nvPicPr.nvPr.ph->idx.reset();
			}				
			if (bFillUp)
			{
				if (pShape.IsInit())	pShape->FillLevelUp();
				if (pPic.IsInit())		pPic->FillLevelUp();
			}
			
			int ph_type = 0;
			if (pNvPr->ph->type.IsInit())
			{
				ph_type = pNvPr->ph->type->GetBYTECode();

				if (type == Layout && (ph_type == 5 || ph_type == 6 || ph_type == 7 || ph_type == 12))
					continue;
			}

			if (!bPlaceholders)
				continue;

			odf_context()->drawing_context()->set_placeholder_type(ph_type);
			
			if (pNvPr->ph->idx.IsInit())
				odf_context()->drawing_context()->set_placeholder_id(pNvPr->ph->idx.get());


			PPTX::Logic::TextListStyle * listMasterStyle = NULL;
			
			if (txStyles)
			{
				std::wstring type = pNvPr->ph->type.get_value_or(_T("body"));
				
				if ((type == L"title") || (type == L"ctrTitle"))
					listMasterStyle = txStyles->titleStyle.GetPointer();
				else if ((type == L"body") || (type == L"subTitle") || (type == L"obj"))
					listMasterStyle = txStyles->bodyStyle.GetPointer();
				else if (type != L"")
					listMasterStyle = txStyles->otherStyle.GetPointer();
			}

			if (pShape.IsInit())
			{
				std::wstring bOriginText;
				PPTX::Logic::TextListStyle* shapeListStyle = NULL;

				if (pShape->txBody.IsInit())
				{
					bOriginText = pShape->txBody->GetText(false);
					shapeListStyle = pShape->txBody->lstStyle.GetPointer();
				}
				bool bTextPresent = (bOriginText.empty() == false); 

				PPTX::Logic::Shape update_shape;
				
				if (listMasterStyle)
				{
					update_shape.txBody = new PPTX::Logic::TxBody();
					
					PPTX::Logic::TextListStyle *newListStyle = new PPTX::Logic::TextListStyle();

					for (int i = 0; i < 10; i++)
					{
						if(listMasterStyle->levels[i].is_init())
							listMasterStyle->levels[i]->Merge(newListStyle->levels[i]);
					}
					if (shapeListStyle)
					{
						for (int i = 0; i < 10; i++)
						{
							if (shapeListStyle->levels[i].is_init())
								shapeListStyle->levels[i]->Merge(newListStyle->levels[i]);
						}
					}
					update_shape.txBody->lstStyle.reset(newListStyle);
				}
				pShape->Merge(update_shape);

				if (type == Slide && bPlaceholder && false == bTextPresent && update_shape.txBody.IsInit())
				// спец. для либры - чтобы она отображала плейсхолдеры на слайдах нормально! бл...
				// изменение форматирования в плейсхолдере для данного слайда похерется
				{
					update_shape.txBody->Paragrs.clear();
				}
				OoxConverter::convert(&update_shape);
			}
			if (pPic.IsInit())
			{
				PPTX::Logic::Pic update_shape;
				
				pPic->Merge(update_shape);
				OoxConverter::convert(&update_shape);
			}
			bConvert = true;
		}
		
		if (!bConvert && (pShape.IsInit()) && (pShape->txBody.IsInit() && presentation->defaultTextStyle.IsInit()))
		{//default text style with master clrScheme
			PPTX::Logic::Shape update_shape;
			
			update_shape.txBody = new PPTX::Logic::TxBody();

			presentation->defaultTextStyle->Merge(update_shape.txBody->lstStyle);

			pShape->Merge(update_shape);
			OoxConverter::convert(&update_shape);

			bConvert = true;
		}


		if (!bConvert)
		{
			OoxConverter::convert(pElem.GetPointer());
		}

		{
			// TODO: Move to NvPicPr.cNvPr conversion
			int id = -1;
			if (pPic.IsInit()) 
				id = pPic->nvPicPr.cNvPr.id;

			if (id != -1)
			{
				const std::wstring xml_id = odp_context->map_indentifier(std::to_wstring(id));
				odf_context()->drawing_context()->set_xml_id(xml_id);
			}
		}
		
		odf_context()->drawing_context()->end_drawing();
	}
	convert(oox_slide->controls.GetPointer());
}

void PptxConverter::convert_layout(PPTX::Logic::CSld *oox_slide)
{
	if (oox_slide == NULL) return;

	for (size_t i = 0 ; i < oox_slide->spTree.SpTreeElems.size(); i++)
	{
		smart_ptr<PPTX::WrapperWritingElement>	pElem = oox_slide->spTree.SpTreeElems[i].GetElem();
		smart_ptr<PPTX::Logic::Shape>			pShape = pElem.smart_dynamic_cast<PPTX::Logic::Shape>();
		
		if (pShape.IsInit() && pShape->nvSpPr.nvPr.ph.is_init())
		{
			pShape->FillLevelUp();
			
			int type = 0;
			if (pShape->nvSpPr.nvPr.ph->type.IsInit())
				type = pShape->nvSpPr.nvPr.ph->type->GetBYTECode();

			if (type != 5 && type != 6 && type != 7 && type != 12)
			{
				odf_writer::office_element_ptr elm;
				create_element(L"presentation", L"placeholder", elm, odp_context);

				odf_context()->drawing_context()->start_drawing();
				odf_context()->drawing_context()->start_element(elm);
					
				odf_context()->drawing_context()->set_placeholder_type(type);

				if (pShape->nvSpPr.nvPr.ph->idx.IsInit())
					odf_context()->drawing_context()->set_placeholder_id(*pShape->nvSpPr.nvPr.ph->idx);

				OoxConverter::convert(pShape->spPr.xfrm.GetPointer());
				OoxConverter::convert(pShape->txBody.GetPointer());

				odf_context()->drawing_context()->end_element();			
				odf_context()->drawing_context()->end_drawing();
			}
		}
	}
}

} 

