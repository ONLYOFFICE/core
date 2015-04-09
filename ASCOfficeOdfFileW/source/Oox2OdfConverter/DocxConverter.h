#pragma once

#include "Converter.h"

#include <boost/foreach.hpp>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPOptional.h>

#include "../../../Common/DocxFormat/source/Common/SimpleTypes_Word.h"

namespace OOX 
{
	class CDocx;
	class CTheme;
	class CDocDefaults;
	class CStyle;

	namespace Logic
	{
		class CSmartTag;
		class CIns;
		class CParagraph;
		class CParagraphProperty;
		class CRun;
		class CSym;
		class CRunProperty;
		class CSectionProperty;
		class CText;
		class CAlternateContent;
		class CDrawing;	
		class CGroupShape;
		class CShape;
		class CPicture;
		class CObject;
		class CPBdr;
		class CHyperlink;
		class CFldChar;
		class CFldSimple;
		class CInstrText;
		class CCommentRangeStart;
		class CCommentRangeEnd;
		class CCommentReference;
		class CTbl;
		class CTblGrid;
		class CTr;
		class CTc;
		class CTblGridCol;
		class CTableCellProperties;
		class CTableProperty;
		class CTableRowProperties;
		class CTcBorders;
		class CTblBorders;
		class CSdt;
		class CBackground;
		class CLockedCanvas;

	}
	namespace Numbering
	{
		class CAbstractNum;
		class CLvl;
	}
}
namespace ComplexTypes
{
	namespace Word
	{
		class CColor;
		class CJc;
		class CBorder;
		class CShading;
		class CFramePr;
		class CTblWidth;
		class CPageBorder;
	}
}
namespace cpdoccore 
{
	namespace odf 
	{
		namespace package 
		{		
			class odf_document;
		}
		class ods_conversion_context;
		class odf_conversion_context;
		class odt_conversion_context;
	}
}

using namespace cpdoccore;

namespace Oox2Odf
{
	class DocxConverter : public OoxConverter
	{
	public:
		DocxConverter(const std::wstring & path, const ProgressCallback* ffCallBack);

		virtual void convertDocument();
		virtual void write(const std::wstring & path);
		
		virtual odf::odf_conversion_context		*odf_context();
		virtual OOX::CTheme						*oox_theme();
		virtual CString							 find_link_by_id (CString sId, int t);

		void convert(OOX::WritingElement		*oox_unknown);
    private:
		OOX::CDocx								*docx_document;
		cpdoccore::odf::package::odf_document	*output_document;
		
		odf::odt_conversion_context				*odt_context;
		OOX::Logic::CSectionProperty			*last_seсtion_properties; // для режима "продолжение" раздела с обновленными свойствами

		void apply_from(OOX::Logic::CSectionProperty *props, OOX::Logic::CSectionProperty *other);
		
		void convert_document();
		void convert_styles();
		void convert_lists_styles();
		void convert_settings();
		
		void convert_comment	(int oox_comm_id);
		void convert_hdr_ftr	(CString sId);

		void convert(OOX::Logic::CBackground			*oox_background, int type);
		void convert(OOX::Logic::CSdt					*oox_sdt);
		void convert(OOX::Logic::CSectionProperty		*oox_section_pr, bool root = false);
		void convert(OOX::Logic::CParagraph				*oox_paragraph);
		void convert(OOX::Logic::CRun					*oox_run);
		void convert(OOX::Logic::CParagraphProperty		*oox_paragraph_prop,	odf::style_paragraph_properties *paragraph_properties);
		void convert(ComplexTypes::Word::CFramePr		*oox_frame_pr,			odf::style_paragraph_properties *paragraph_properties);
		void convert(OOX::Logic::CRunProperty			*oox_run_prop,			odf::style_text_properties		*text_properties);
		void convert(ComplexTypes::Word::CShading		*oox_shading,			odf::style_text_properties		*text_properties );
		void convert(OOX::Logic::CFldSimple				*oox_fld);
		void convert(OOX::Logic::CFldChar				*oox_fld);
		void convert(OOX::Logic::CInstrText				*oox_instr);
		void convert(OOX::Logic::CText					*oox_text);
		void convert(OOX::Logic::CHyperlink				*oox_hyperlink);
		void convert(OOX::Logic::CSym					*oox_sym);
		void convert(OOX::Logic::CIns					*oox_ins);
		void convert(OOX::Logic::CSmartTag				*oox_tag);		
		
		void convert(OOX::Logic::CAlternateContent		*oox_alt_content);
		void convert(OOX::Logic::CDrawing				*oox_drawing);
		void convert(OOX::Logic::CGroupShape			*oox_group_shape);
		void convert(OOX::Logic::CShape					*oox_shape);
		void convert(OOX::Logic::CPicture				*oox_picture);
		void convert(OOX::Logic::CLockedCanvas			*oox_canvas);
		void convert(OOX::Logic::CObject				*oox_obj);
		
		void convert(OOX::Drawing::CAnchor				*oox_anchor);
		void convert(OOX::Drawing::CInline				*oox_inline);
		void convert(OOX::Drawing::CGraphic				*oox_graphic);		
		void convert(OOX::Drawing::CChart				*oox_chart);
		void convert(OOX::Drawing::CPicture				*oox_picture);
		void convert(OOX::Drawing::CDiagrammParts		*oox_diagramm);
		
		void convert(SimpleTypes::CTheme<>				*oox_font_theme,_CP_OPT(std::wstring)			& odf_font_name);
		void convert(ComplexTypes::Word::CColor			*color,			_CP_OPT(odf::color)				& odf_color);
		void convert(SimpleTypes::CUniversalMeasure		*oox_size,		_CP_OPT(odf::length)			& odf_size);
		void convert(SimpleTypes::CUniversalMeasure		*oox_size,		_CP_OPT(odf::length_or_percent) & odf_size);
		void convert(ComplexTypes::Word::CTblWidth		*oox_size,		_CP_OPT(odf::length)			& odf_size);
		void convert(ComplexTypes::Word::CJc			*oox_jc,		_CP_OPT(odf::text_align)		& align);
		void convert(ComplexTypes::Word::CBorder		*borderProp,	std::wstring & odf_border_prop);
		void convert(ComplexTypes::Word::CPageBorder	*borderProp,	std::wstring & odf_border_prop);
		
		void convert(OOX::Logic::CPBdr					*oox_border,			odf::style_paragraph_properties		*paragraph_properties);
		void convert(OOX::Logic::CTcBorders 			*oox_border,			odf::style_table_cell_properties	*table_cell_properties);
		void convert(OOX::Logic::CTblBorders			*oox_border,			odf::style_table_cell_properties	*table_cell_properties);
		//void convert(OOX::Logic::CTblBorders			*oox_border,			odf::style_table_properties			*table_properties);

		void convert(SimpleTypes::CHexColor<>			*color, SimpleTypes::CThemeColor<>	*theme_color, 
														SimpleTypes::CUcharHexNumber<>* theme_tint,
														SimpleTypes::CUcharHexNumber<>* theme_shade, _CP_OPT(odf::color) & odf_color);
		void convert(OOX::CDocDefaults					*def_style);
		void convert(OOX::CStyle						*style);
		void convert_table_style(OOX::CStyle			*oox_style);
		void convert(OOX::Numbering::CAbstractNum		*oox_num_style);
		void convert(OOX::Numbering::CLvl				*oox_num_lvl);

		void convert(OOX::Logic::CCommentRangeStart		*oox_comm_start);
		void convert(OOX::Logic::CCommentRangeEnd		*oox_comm_end);
		void convert(OOX::Logic::CCommentReference		*oox_comm_ref);

		void convert(OOX::Logic::CTbl					*oox_table);
		void convert(OOX::Logic::CTblGrid				*oox_table_grid);
		void convert(OOX::Logic::CTr					*oox_table_row);
		void convert(OOX::Logic::CTc					*oox_table_cell);
		
		bool convert(OOX::Logic::CTableCellProperties	*oox_table_cell_pr, int col = -1);
		bool convert(OOX::Logic::CTableCellProperties	*oox_table_cell_pr,	odf::style_table_cell_properties	*table_cell_properties/*,odf::style_table_cell_properties	* table_cell_properties = NULL*/);
		bool convert(OOX::Logic::CTableProperty			*oox_table_pr, bool base_styled);
		bool convert(OOX::Logic::CTableProperty			*oox_table_pr,		odf::style_table_properties		*table_properties);
		void convert(OOX::Logic::CTableProperty			*oox_table_pr,		odf::style_table_cell_properties	*table_cell_properties);
		void convert(OOX::Logic::CTableRowProperties	*oox_table_row_pr);
		void convert(OOX::Logic::CTableRowProperties	*oox_table_row_pr,	odf::style_table_row_properties	*table_row_properties);
//--------------------------------------------------------------------------------
	};
}