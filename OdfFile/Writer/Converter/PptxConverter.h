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

		void convert(PPTX::Logic::TcBdr *borders,	odf_writer::style_paragraph_properties	*para_properties);
		void convert(PPTX::Logic::TcStyle* style,	odf_writer::graphic_format_properties	*graphic_properties);
		void convert(PPTX::Logic::TcTxStyle* style,	odf_writer::style_text_properties		*text_properties);
		
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

		PPTX::Document									*pptx_document;
		PPTX::Presentation								*presentation;
		
		PPTX::Theme										*current_theme;		

		OOX::IFileContainer								*current_slide;
		PPTX::Logic::ClrMap								*current_clrMap;
		
		odf_writer::odp_conversion_context				*odp_context;

		std::map<std::wstring, std::wstring>			m_mapMasters;
		std::map<std::wstring, std::wstring>			m_mapLayouts;
		
		void convert_slides		();
		void convert_styles		();
		void convert_settings	();
		void convert_layouts	();
		void convert_common		();

		
//--------------------------------------------------------------------------------
	};
}
