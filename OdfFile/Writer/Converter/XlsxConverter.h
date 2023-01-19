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

namespace OOX 
{
	class CDocx;
	class CTheme;

	namespace Spreadsheet
	{
		class CXlsx;
		class CXlsxFlat;
		class WritingElement;

		class CWorksheet;
		class CTable;
		class CTableColumns;
		class CCol;
		class CRow;
		class CCell;
		class CRun;
		class CRPr;
		class CText;
		class CFormula;
		class CHyperlink;
		class CSheetFormatPr;
		class CSheetPr;		
		class CFont;
		class CBorder;
		class CFill;		
		class CColor;
		class CBorderProp;
		class CAligment;
		class CXfs;
		class CDxf;
		class CCellStyle;
		class CNumFmt;
		class COleObjects;
		class CControls;
		class CDrawing;
		class CCellAnchor;
		class CFromTo;
		class CCommentItem;
		class CDefinedName;
		class CConditionalFormatting;
		class CConditionalFormattingRule;
		class CDataBar;
		class CColorScale;
		class CIconSet;
		class CFormulaCF;
		class CConditionalFormatValueObject;
		class CAutofilter;
		class CFilterColumn;
		class CSheetViews;
		class CPageSetup;
		class CPageMargins;
		class CSi;
		class CWorkbookView;
		class CPictureWorksheet;
		class CHeaderFooter;		
		class CSparklineGroups;
		class CAltTextTable;
		class CExternalLink;
		class CDataValidations;
		class CSheetProtection;
		class CDataValidation;
		class CHeaderFooterElement;
		class CLegacyDrawingHFWorksheet;
		class CPrintOptions;
		class CProtection;

		class CPivotTableFile;
		class CPivotTableDefinition;
		class CPivotCacheDefinition;
	}
}

namespace cpdoccore
{
	struct oox_table_position;

	namespace odf_types
	{
		class background_color;
		class font_size;
	}

	namespace odf_writer 
	{
		class office_element;
		typedef shared_ptr<office_element>::Type office_element_ptr;

		namespace package 
		{		
			class odf_document;
		}
		class odf_conversion_context;
		class ods_conversion_context;

	}
}

using namespace cpdoccore;

namespace Oox2Odf
{

	class XlsxConverter : public OoxConverter
	{
	public:
		friend class OoxConverter;

		XlsxConverter(const std::wstring & path, bool bTemplate);
		~XlsxConverter();

		virtual bool convertDocument();
		
		virtual OOX::IFileContainer					*current_document();
		virtual odf_writer::odf_conversion_context	*odf_context();		
		virtual PPTX::Theme							*oox_theme();
        virtual std::wstring						find_link_by_id (const std::wstring & sId, int t);
		virtual NSCommon::smart_ptr<OOX::File>		find_file_by_id (const std::wstring & sId);

		void convert(OOX::Spreadsheet::WritingElement	*oox_unknown);
    private:		
		OOX::Spreadsheet::CXlsxFlat				*xlsx_flat_document;
		OOX::Spreadsheet::CXlsx					*xlsx_document;
		OOX::IFileContainer						*xlsx_current_container; 
		
		odf_writer::ods_conversion_context		*ods_context;

		void convert_sheets();
		void convert_styles();		
		void convert();
		
		void convert(OOX::Spreadsheet::CWorksheet			*oox_sheet);
		void convert(OOX::Spreadsheet::CDefinedName			*oox_defined);
		void convert(OOX::Spreadsheet::CTable				*oox_table_part);
		void convert(OOX::Spreadsheet::CTableColumns		*oox_table_part_columns);
		void convert(OOX::Spreadsheet::CPictureWorksheet	*oox_background);
		void convert(OOX::Spreadsheet::CHeaderFooter		*oox_header_footer);
		void convert(OOX::Spreadsheet::CLegacyDrawingHFWorksheet *oox_background);
		void convert(OOX::Spreadsheet::CHeaderFooterElement	*oox_header_footer);
		void convert(OOX::Spreadsheet::CPivotTableFile		*oox_pivot_table);
		void convert(OOX::Spreadsheet::CPivotTableDefinition*oox_pivot_table, OOX::Spreadsheet::CPivotCacheDefinition* oox_pivot_cache);

		void convert(OOX::Spreadsheet::CCol					*oox_column);
		void convert(OOX::Spreadsheet::CRow					*oox_row, OOX::Spreadsheet::CRow *oox_row_prev);
		void convert(OOX::Spreadsheet::CCell				*oox_cell);

		void convert(OOX::Spreadsheet::CRun					*oox_text_run);
		void convert(OOX::Spreadsheet::CRPr					*oox_text_pr);
		void convert(OOX::Spreadsheet::CText				*oox_text);

		void convert(OOX::Spreadsheet::CFormula				*oox_formula);
		void convert(OOX::Spreadsheet::CHyperlink			*oox_hyperlink,	OOX::Spreadsheet::CWorksheet *oox_sheet);
		void convert(OOX::Spreadsheet::CCommentItem			*oox_comment);

		void convert(OOX::Spreadsheet::CSheetFormatPr		*oox_sheet_format_pr);
		void convert(OOX::Spreadsheet::CSheetPr				*oox_sheet_pr);
		void convert(OOX::Spreadsheet::CSheetViews			*oox_sheet_views);
		void convert(OOX::Spreadsheet::CPageSetup			*oox_page);
		void convert(OOX::Spreadsheet::CPageMargins			*oox_page);
		void convert(OOX::Spreadsheet::CPrintOptions		*oox_print_options);
		void convert(OOX::Spreadsheet::CWorkbookView		*oox_book_views);
		void convert(OOX::Spreadsheet::CExternalLink		*oox_external_link);

		void convert(OOX::Spreadsheet::CFont				*font,		odf_writer::style_text_properties		*text_properties);		
		void convert(OOX::Spreadsheet::CBorder				*border,	odf_writer::style_table_cell_properties *cell_properties);
		void convert(OOX::Spreadsheet::CFill				*fill,		odf_writer::style_table_cell_properties *cell_properties);
		void convert(OOX::Spreadsheet::CProtection			*protection,odf_writer::style_table_cell_properties *cell_properties);
		
		void convert(OOX::Spreadsheet::CColor				*color,		_CP_OPT(odf_types::color) & odf_color);
		void convert(OOX::Spreadsheet::CColor				*color,		_CP_OPT(odf_types::background_color) & odf_bckgrd_color);
		void convert(OOX::Spreadsheet::CBorderProp			*borderProp, std::wstring & odf_border_prop);
		void convert(OOX::Spreadsheet::CAligment			*aligment,	odf_writer::style_paragraph_properties	*paragraph_properties,
																		odf_writer::style_table_cell_properties *cell_properties);

		void convert(OOX::Spreadsheet::CXfs					*cell_style, int oox_id, bool automatic = true, bool root = false);
		void convert(OOX::Spreadsheet::CCellStyle			*cell_style);
		void convert(OOX::Spreadsheet::CNumFmt				*numFmt);
		void convert(OOX::Spreadsheet::CDxf					*dxFmt, int oox_id);

		void convert(OOX::Spreadsheet::CCellAnchor					*oox_anchor);
		void convert(OOX::Spreadsheet::CDrawing						*oox_drawing, OOX::Spreadsheet::CWorksheet *oox_sheet);
		void convert(OOX::Spreadsheet::COleObjects					*oox_objects, OOX::Spreadsheet::CWorksheet *oox_sheet);
		void convert(OOX::Spreadsheet::CControls					*oox_control, OOX::Spreadsheet::CWorksheet *oox_sheet);

		void convert(OOX::Spreadsheet::CFromTo						*oox_from_to, oox_table_position *pos);

		void convert(OOX::Spreadsheet::CSheetProtection				*oox_prot);
		void convert(OOX::Spreadsheet::CDataValidations				*oox_validations);
		void convert(OOX::Spreadsheet::CDataValidation				*oox_validation);
		void convert(OOX::Spreadsheet::CConditionalFormatting		*oox_cond_fmt);
		void convert(OOX::Spreadsheet::CConditionalFormattingRule	*oox_cond_rule);
		void convert(OOX::Spreadsheet::CAutofilter					*oox_filter);
		void convert(OOX::Spreadsheet::CFilterColumn				*oox_filter_column);
		void convert(OOX::Spreadsheet::CDataBar						*oox_cond_databar);
		void convert(OOX::Spreadsheet::CColorScale					*oox_cond_colorscale);
		void convert(OOX::Spreadsheet::CIconSet						*oox_cond_iconset);
		void convert(OOX::Spreadsheet::CConditionalFormatValueObject*oox_cond_value);
		void convert(OOX::Spreadsheet::CFormulaCF					*oox_cond_formula);
		void convert(OOX::Spreadsheet::CSi							*oox_rtf_text);

		void convert(OOX::Spreadsheet::CSparklineGroups				*sparkline);
		void convert(OOX::Spreadsheet::CAltTextTable				*alt_text);

		void convert(double oox_size,						_CP_OPT(odf_types::length) & odf_size);
		void convert_sharing_string(int number);
	};

}
