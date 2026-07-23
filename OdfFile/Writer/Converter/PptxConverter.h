/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "Converter.h"

#include <CPSharedPtr.h>
#include <CPOptional.h>

#include "../../../OOXML/Common/SimpleTypes_Word.h"

namespace OOX 
{
	class CTheme;

}
namespace PPTX
{
	class Document;
	class TableStyles;
	class NotesSlide;
	class NotesMaster;
	class Presentation;
	class Comments;

	namespace Logic
	{
		class CSld;
		class Bg;
		class Transition;
		class Timing;
		class TablePartStyle;
		class Table;
		class TableStyle;
		class TableProperties;
		class TableRow;
		class TableCell;
		class TableCellProperties;
		class TcBdr;
		class TxStyles;
		class TcStyle;
		class TcTxStyle;
		class TimeNodeBase;
		class CTn;
		class Cond;
		class Anim;
		class AnimClr;
		class AnimEffect;
		class AnimMotion;
		class AnimRot;
		class AnimScale;
		class Audio;
		class Cmd;
		class Excl;
		class Set;
		class Video;
		class CBhvr;
		class TgtEl;
		class AnimVariant;
		class AttrName;
		class EmptyTransition;
		class OrientationTransition;
		class EightDirectionTransition;
		class OptionalBlackTransition;
		class CornerDirectionTransition;
		class SideDirectionTransition;
		class WheelTransition;
		class SplitTransition;
		class ZoomTransition;
	}

	namespace Limit
	{
		class TLNodeType;
		class TLPresetClass;
	}
}

namespace cpdoccore 
{
	namespace odf_writer 
	{
		namespace package 
		{		
			class odf_document;
		}
		class ods_conversion_context;
		class odf_conversion_context;
		class odt_conversion_context;
		class odp_conversion_context;

		class graphic_format_properties;
		class style_table_cell_properties;
		class paragraph_format_properties;
	}
}

using namespace cpdoccore;

namespace Oox2Odf
{
	enum _typePages
	{
		Slide,
		Master,
		Layout,
		NotesMaster,
		Notes
	};
	class PptxConverter : public OoxConverter
	{
	public:
		PptxConverter(const std::wstring & path, bool bTemplate);
		~PptxConverter();

		virtual bool convertDocument();
		
		virtual OOX::IFileContainer						*current_document();
		virtual odf_writer::odf_conversion_context		*odf_context();		
		virtual PPTX::Theme								*oox_theme();
		virtual PPTX::Logic::ClrMap						*oox_clrMap();
       
		virtual std::wstring							find_link_by_id (const std::wstring & sId, int t, bool & bExternal);
		virtual NSCommon::smart_ptr<OOX::File>			find_file_by_id	(const std::wstring & sId);

		void convert(OOX::WritingElement *oox_unknown);
 		
		void convert_slide	(PPTX::Logic::CSld			*oox_slide, PPTX::Logic::TxStyles* txStyles, bool bPlaceholders, bool bFillUp, _typePages type);
		void convert_layout	(PPTX::Logic::CSld			*oox_slide);
		void convert		(PPTX::Comments				*oox_comments);
		void convert		(PPTX::NotesSlide			*oox_notes);
		void convert		(PPTX::NotesMaster			*oox_notes);

		void convert(PPTX::Logic::Bg					*oox_background);
		void convert(PPTX::Logic::Timing				*oox_timing);
		void convert(PPTX::Logic::Transition			*oox_transition);
		void convert(PPTX::Logic::TimeNodeBase			*oox_base_time);
		void convert(PPTX::Logic::CTn					*oox_common_time);
		void convert(PPTX::Logic::Cond					*oox_condition); 
		void convert(PPTX::Logic::Anim					*oox_anim);
		void convert(PPTX::Logic::AnimClr				*oox_anim_color);
		void convert(PPTX::Logic::AnimEffect			*oox_anim_effect);
		void convert(PPTX::Logic::AnimMotion			*oox_anim_motion);
		void convert(PPTX::Logic::AnimRot				*oox_anim_rot);
		void convert(PPTX::Logic::AnimScale				*oox_anim_scale);
		void convert(PPTX::Logic::Audio					*oox_audio);
		void convert(PPTX::Logic::Cmd					*oox_cmd);
		void convert(PPTX::Logic::Excl					*oox_excl);
		void convert(PPTX::Logic::Set					*oox_set);
		void convert(PPTX::Logic::Video					*oox_video);
		void convert(PPTX::Logic::CBhvr					*oox_cbhvr);
		void convert(PPTX::Logic::TgtEl					*oox_tgt_el);
		void convert(PPTX::Logic::AnimVariant			*oox_anim_variant);	
		void convert(PPTX::Logic::AttrName				*oox_attr_name);
		
		void convert(PPTX::Logic::Table					*oox_table);
		void convert(PPTX::Logic::TableRow				*oox_table_row);
		void convert(PPTX::Logic::TableCell				*oox_table_cell, int numCol);
		
		bool convert(PPTX::Logic::TableCellProperties	*oox_table_cell_pr, int col);
		bool convert(PPTX::Logic::TableCellProperties	*oox_table_cell_pr); 
		
		void convert(PPTX::Logic::TableProperties		*oox_table_pr);
		void convert(PPTX::Logic::TcBdr					*oox_table_borders);

		void convert(PPTX::TableStyles					*oox_table_styles);

		void convert(const std::wstring &id, PPTX::Logic::TableStyle *oox_table_style);
 		std::wstring convert(PPTX::Logic::TablePartStyle *oox_table_part_style, const std::wstring &base = L"");

		void convert(PPTX::Logic::TcBdr *borders,	odf_writer::paragraph_format_properties	*para_properties);
		void convert(PPTX::Logic::TcStyle* style,	odf_writer::graphic_format_properties	*graphic_properties);
		void convert(PPTX::Logic::TcTxStyle* style,	odf_writer::text_format_properties		*text_properties);
		
		void convert(PPTX::Logic::Ln *oox_ln_border, std::wstring & odf_border);
private:
		void convert(PPTX::Logic::EmptyTransition			*oox_transition);
		void convert(PPTX::Logic::OrientationTransition		*oox_transition);
		void convert(PPTX::Logic::EightDirectionTransition	*oox_transition);
		void convert(PPTX::Logic::OptionalBlackTransition	*oox_transition);
		void convert(PPTX::Logic::CornerDirectionTransition	*oox_transition);
		void convert(PPTX::Logic::SideDirectionTransition	*oox_transition);
		void convert(PPTX::Logic::WheelTransition			*oox_transition);
		void convert(PPTX::Logic::SplitTransition			*oox_transition);
		void convert(PPTX::Logic::ZoomTransition			*oox_transition);

		void convert(const PPTX::Limit::TLNodeType&			oox_note_type);
		void convert(const PPTX::Limit::TLPresetClass&		oox_preset_class);
		void convert(const PPTX::Limit::TLPresetClass&		oox_preset_class, int preset_id);
		void convert(odf_writer::graphic_format_properties* graphic_props, odf_writer::style_table_cell_properties* table_cell_props);
		void convert(odf_writer::paragraph_format_properties* paragraph_props, odf_writer::style_table_cell_properties* table_cell_props);

		PPTX::Document									*pptx_document;
		PPTX::Presentation								*presentation;
		
		PPTX::Theme										*current_theme;		

		OOX::IFileContainer								*current_slide;
		PPTX::Logic::ClrMap								*current_clrMap;
		
		odf_writer::odp_conversion_context				*odp_context;

		std::map<std::wstring, std::wstring>			m_mapMasters;
		std::map<std::wstring, std::wstring>			m_mapLayouts;

		std::wstring									interactive_animation_element_id;
		
		void convert_slides ();
		void convert_styles ();
		void convert_settings ();
		void convert_layouts ();
		void convert_common	 ();
		void convert_masters_and_layouts();

		std::wstring convert_animation_formula(std::wstring formula);
		std::wstring convert_animation_scale_values(int x, int y);

		std::wstring get_page_name(PPTX::Logic::CSld* oox_slide, _typePages type);
		void fill_in_deferred_hyperlinks();
		
//--------------------------------------------------------------------------------
	};
}
