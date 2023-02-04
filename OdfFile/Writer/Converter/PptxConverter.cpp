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
#include "PptxConverter.h"
#include "../../Common/utils.h"

#include "../../../OOXML/PPTXFormat/Folder.h"
#include "../../../OOXML/PPTXFormat/Presentation.h"
#include "../../../OOXML/PPTXFormat/Slide.h"
#include "../../../OOXML/PPTXFormat/NotesMaster.h"

#include "../../../OOXML/PPTXFormat/Logic/Table/Table.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/Par.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/Seq.h"
#include "../../../OOXML/PPTXFormat/Logic/Timing/CTn.h"

#include "../../../OOXML/PPTXFormat/Logic/CxnSp.h"

#include "../../../OOXML/PPTXFormat/Logic/Transitions/EmptyTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/OrientationTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/EightDirectionTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/OptionalBlackTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/SideDirectionTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/CornerDirectionTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/WheelTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/SplitTransition.h"
#include "../../../OOXML/PPTXFormat/Logic/Transitions/ZoomTransition.h"

#include "../Format/odp_conversion_context.h"

#include "../Format/odf_text_context.h"
#include "../Format/odf_drawing_context.h"

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

std::wstring PptxConverter::find_link_by_id (const std::wstring & sId, int type)
{
	if(!pptx_document) return L"";

    std::wstring			ref;
	smart_ptr<OOX::File>	oFile;

	if (oox_current_child_document)
	{
		oFile	= oox_current_child_document->Find(sId);
		ref		= OoxConverter::find_link_by(oFile, type);
	}
	if (!ref.empty()) return ref;

	oFile	= current_slide ? current_slide->Find(sId) : pptx_document->Find(sId);
	ref		= OoxConverter::find_link_by(oFile, type);

	return ref;
}

bool PptxConverter::convertDocument()
{
    if (!pptx_document) return false;
    if (!odp_context)   return false;
		
	odp_context->start_document();

	convert_styles();
	convert_settings(); 
	
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
	
	odf_writer::style_paragraph_properties	* paragraph_properties	= odp_context->styles_context()->last_state()->get_paragraph_properties();
	odf_writer::style_text_properties		* text_properties		= odp_context->styles_context()->last_state()->get_text_properties();

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
	odf_writer::style_text_properties		* frame_text_properties		= odp_context->styles_context()->last_state()->get_text_properties();

	if (frame_text_properties && text_properties)
		frame_text_properties->apply_from(text_properties);

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

			odf_writer::style_paragraph_properties *para = style_state->get_paragraph_properties();
			odf_writer::style_paragraph_properties *para_base = style_state_base->get_paragraph_properties();
			
			if (para && para_base) para->apply_from(para_base);
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

void PptxConverter::convert(PPTX::Logic::TcTxStyle* style, odf_writer::style_text_properties *text_properties)
{
	if (!style) return;
	if (!text_properties) return;
	
//nullable_limit<Limit::OnOff> i;
//nullable_limit<Limit::OnOff> b;
//nullable<FontRef> fontRef;
//UniColor Color;
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
			{
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
		
		convert_slide	(slide->cSld.GetPointer(), current_txStyles, true, bShowMasterSp, Slide);
		convert			(slide->comments.GetPointer());
		convert			(slide->Note.GetPointer());
		
		convert			(slide->transition.GetPointer());
		//convert		(slide->timing.GetPointer());


		odp_context->end_slide();
	}
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
		
		std::wstring pathAudio = find_link_by_id(sID, 3);
		
		std::wstring odf_ref = odf_context()->add_media(pathAudio);

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

void PptxConverter::convert(PPTX::Logic::CTn *oox_time_common)
{
	if (!oox_time_common) return;

	if (oox_time_common->id.IsInit())
	{
		odp_context->current_slide().set_anim_id(*oox_time_common->id);
	}
	if (oox_time_common->nodeType.IsInit())
	{
		odp_context->current_slide().set_anim_type(oox_time_common->nodeType->get());
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

	//nullable<CondLst>			stCondLst;
	//nullable<CondLst>			endCondLst;
	//nullable<Cond>			endSync;
	//nullable<Iterate>			iterate;

    // TODO
//    for (auto& child : oox_time_common->childTnLst)
//    {
//        for (size_t i = 0; i <child.m_node. .list.size(); i++)
//        {
//            if (tnLst.list[i].is_init() == false) continue;

//            convert(&oox_time_common->childTnLst->list[i]);
//        }
//    }
//	if (oox_time_common->childTnLst.IsInit())
//	{
//		for (size_t i = 0; i < oox_time_common->childTnLst->list.size(); i++)
//		{
//			if (oox_time_common->childTnLst->list[i].is_init() == false) continue;

//			convert(&oox_time_common->childTnLst->list[i]);
//		}
//	}
	//if (oox_time_common->subTnLst.IsInit())
	//{
	//	for (size_t i = 0; i < oox_time_common->subTnLst->list.size(); i++)
	//	{
	//		if (oox_time_common->subTnLst->list[i].is_init() == false) continue;

	//		convert(&oox_time_common->subTnLst->list[i]);
	//	}
	//}
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
			
			if (oox_table_pr->FirstRow.is_init() && !table_style.first_row_.empty())
			{
				odp_context->slide_context()->table_context()->set_first_row_cell_properties(table_style.first_row_);
			}
			if (oox_table_pr->FirstCol.is_init() && !table_style.first_col_.empty())
			{
				odp_context->slide_context()->table_context()->set_first_col_cell_properties(table_style.first_col_);
			}
			if (oox_table_pr->BandRow.is_init() && !table_style.band_row_.empty())
			{
				odp_context->slide_context()->table_context()->set_band_row_cell_properties(table_style.band_row_);
			}
			if (oox_table_pr->BandCol.is_init() && !table_style.band_col_.empty())
			{
				odp_context->slide_context()->table_context()->set_band_col_cell_properties(table_style.band_col_);
			}
			if (oox_table_pr->LastRow.is_init() && !table_style.last_row_.empty())
			{
				odp_context->slide_context()->table_context()->set_last_row_cell_properties(table_style.last_row_);
			}
			if (oox_table_pr->LastCol.is_init() && !table_style.last_col_.empty())
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

	odp_context->slide_context()->start_table_columns();

	for (size_t i = 0; i < oox_table->TableCols.size(); i++)
	{
		double width = -1;

		if (oox_table->TableCols[i].Width.IsInit())
			width = oox_table->TableCols[i].Width.get() / 12700.;

		odp_context->slide_context()->add_table_column(width);

	}
	odp_context->slide_context()->end_table_columns();	

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
			parent_cell_properties = style_->content_.get_style_table_cell_properties();
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
	odf_writer::style_paragraph_properties *odf_para_props = odp_context->styles_context()->last_state(odf_types::style_family::TableCell)->get_paragraph_properties();

	convert(oox_table_borders, odf_para_props);
}


void PptxConverter::convert(PPTX::Logic::TcBdr *oox_table_borders, odf_writer::style_paragraph_properties *odf_para_props)
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
		odf_para_props->content_.common_border_attlist_.fo_border_ = left;

		odf_para_props->content_.common_border_attlist_.fo_border_bottom_ =
		odf_para_props->content_.common_border_attlist_.fo_border_top_	= 
		odf_para_props->content_.common_border_attlist_.fo_border_left_ = 
		odf_para_props->content_.common_border_attlist_.fo_border_right_ = boost::none;
	}
	else
	{
		if (odf_para_props->content_.common_border_attlist_.fo_border_)
		{
			odf_para_props->content_.common_border_attlist_.fo_border_bottom_	= 
			odf_para_props->content_.common_border_attlist_.fo_border_top_		= 
			odf_para_props->content_.common_border_attlist_.fo_border_left_		= 
			odf_para_props->content_.common_border_attlist_.fo_border_right_	= odf_para_props->content_.common_border_attlist_.fo_border_;
		}
		odf_para_props->content_.common_border_attlist_.fo_border_ = boost::none;

		if ( !bottom.empty() )	odf_para_props->content_.common_border_attlist_.fo_border_bottom_	= bottom;
		if ( !top.empty() )		odf_para_props->content_.common_border_attlist_.fo_border_top_		= top;
		if ( !left.empty() )	odf_para_props->content_.common_border_attlist_.fo_border_left_		= left;
		if ( !right.empty() )	odf_para_props->content_.common_border_attlist_.fo_border_right_	= right;
	}
	//if (other2BR.empty() == false)	odf_para_props->content_.style_diagonal_tl_br_ = other2BR;
	//if (other2BL.empty() == false)	odf_para_props->content_.style_diagonal_bl_tr_ = other2BL;
}

bool PptxConverter::convert(PPTX::Logic::TableCellProperties *oox_table_cell_pr)
{
	if (!oox_table_cell_pr)	return false;
	
	odf_writer::style_table_cell_properties *odf_cell_props = odp_context->slide_context()->get_styles_context()->last_state()->get_table_cell_properties();
	odf_writer::style_paragraph_properties *odf_para_props	= odp_context->slide_context()->get_styles_context()->last_state(odf_types::style_family::TableCell)->get_paragraph_properties();
	
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
	//		odf_para_props->content_.style_direction_ = odf_types::direction(odf_types::direction::Ltr);break;
	//	case 6 ://rtl vert
	//		break;
	//	default:
	//		odf_para_props->content_.style_direction_ = odf_types::direction(odf_types::direction::Ttb);break;
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
		//	odf_para_props->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Justify); break;
		//case SimpleTypes::verticaljcBottom :
		//	odf_para_props->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Bottom); break;
		//case SimpleTypes::verticaljcCenter :
		//	odf_para_props->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Middle); break;
		//case SimpleTypes::verticaljcTop    :
		//	odf_para_props->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Top); break;
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
		odf_para_props->content_.common_border_attlist_.fo_border_ = left;

		odf_para_props->content_.common_border_attlist_.fo_border_bottom_ =
		odf_para_props->content_.common_border_attlist_.fo_border_top_	= 
		odf_para_props->content_.common_border_attlist_.fo_border_left_ = 
		odf_para_props->content_.common_border_attlist_.fo_border_right_ = boost::none;
	}
	else
	{
		if (odf_para_props->content_.common_border_attlist_.fo_border_)
		{
			odf_para_props->content_.common_border_attlist_.fo_border_bottom_	= 
			odf_para_props->content_.common_border_attlist_.fo_border_top_		= 
			odf_para_props->content_.common_border_attlist_.fo_border_left_		= 
			odf_para_props->content_.common_border_attlist_.fo_border_right_	= odf_para_props->content_.common_border_attlist_.fo_border_;
		}
		odf_para_props->content_.common_border_attlist_.fo_border_ = boost::none;

		if ( !bottom.empty() )	odf_para_props->content_.common_border_attlist_.fo_border_bottom_	= bottom;
		if ( !top.empty() )		odf_para_props->content_.common_border_attlist_.fo_border_top_		= top;
		if ( !left.empty() )	odf_para_props->content_.common_border_attlist_.fo_border_left_		= left;
		if ( !right.empty() )	odf_para_props->content_.common_border_attlist_.fo_border_right_	= right;
	}
	//convert(oox_border->m_oTL2BR.GetPointer()	, other);
	//if (other.empty() == false) odf_para_props->content_.style_diagonal_tl_br_ = other;
	//
	//convert(oox_border->m_oTR2BL.GetPointer()	, other);
	//if (other.empty() == false)
	//{
	//	odf_para_props->content_.style_diagonal_bl_tr_ = other;
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
	odf_writer::style_drawing_page_properties* page_props = page_style_->content_.get_style_drawing_page_properties();
	
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

	std::wstring page_name;
	if (oox_slide->attrName.IsInit())
		page_name = oox_slide->attrName.get();
	

	if (page_name.empty())
	{
		if (type == Slide)
			page_name = L"Slide_" + std::to_wstring((int)odp_context->get_pages_count());
	}
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
				std::wstring bOriginText = pShape->txBody.IsInit() ? pShape->txBody->GetText(false) : L"";
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

				odf_context()->drawing_context()->end_element();			
				odf_context()->drawing_context()->end_drawing();
			}
		}
	}
}

} 

