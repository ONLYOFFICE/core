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

#include "../../../Common/DocxFormat/Source/Common/SimpleTypes_Word.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Math/oMathContent.h"

namespace OOX 
{
	class CDocx;
	class CDocxFlat;
	class CTheme;
	class CDocDefaults;
	class CStyle;
	class CComment;
	class CStyles;

	namespace Logic
	{
		class CIns;
		class CDel;
		class CPPrChange;
		class CRPrChange;
		class CSectPrChange;
		class CTrPrChange;
		class CTcPrChange;
		class CTblPrChange;
		class CTblGridChange;
		class CParagraph;
		class CParagraphProperty;
		class CPTab;
		class CSmartTag;
		class CRun;
		class CSym;
		class CRunProperty;
		class CSectionProperty;
		class CText;
		class CAlternateContent;
		class CDrawing;	
//		class CGroupShape;
//		class CShape;
		class CControl;
		class CPicture;
		class CObject;
		class CPBdr;
		class CHyperlink;
		class CFldChar;
		class CFldSimple;
		class CInstrText;
		class CDelText;
		class CCommentRangeStart;
		class CCommentRangeEnd;
		class CCommentReference;
		class CFootnoteReference;
		class CEndnoteReference;
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
		class CSdtContent;
		class CBackground;
		class CLockedCanvas;
		class CBookmarkStart;
		class CBookmarkEnd;
		class CBgPict;
		/////////////////
		class CDeg;
		class CDen;
		class CElement;
		class CFName;
		class CLim;
		class CNum;
		class COMath;
		class CSub;
		class CSup;
		/////////////////
		class CCtrlPr;
		class CAcc;
		class CAccPr;
		class CArgPr;
		class CBar;
		class CBarPr;
		class CBorderBox;
		class CBorderBoxPr;
		class CBox;
		class CBoxPr;
		class CBrk;
		class CDelimiter;
		class CDelimiterPr;
		class CEqArr;
		class CEqArrPr;
		class CFPr;
		class CFraction;
		class CFunc;
		class CFuncPr;
		class CGroupChr;
		class CGroupChrPr;
		class CLimLow;
		class CLimLowPr;
		class CLimUpp;
		class CLimUppPr;
		class CMathFont;
		class CMathPr;
		class CMatrix;
		class CMc;
		class CMcPr;
		class CMcs;
		class CMDel;
		class CMIns;
		class CMPr;
		class CMr;
		class CMRPr;
		class CMRun;
		class CMText;
		class CNary;
		class CNaryPr;
		class COMathPara;
		class COMathParaPr;
		class CPhant;
		class CPhantPr;
		class CRad;
		class CRadPr;
		class CSPre;
		class CSPrePr;
		class CSSub;
		class CSSubPr;
		class CSSubSup;
		class CSSubSupPr;
		class CSSup;
		class CSSupPr;
	}
	namespace Numbering
	{
		class CAbstractNum;
		class CLvl;
		class CNumLvl;
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
		class CTrackChange;
		class CShading;
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
	}
}

using namespace cpdoccore;

namespace Oox2Odf
{
	class DocxConverter : public OoxConverter
	{
	public:
		DocxConverter(const std::wstring & path, bool bTemplate);
		~DocxConverter();

		virtual bool convertDocument();
		
		virtual OOX::IFileContainer					*current_document();
		virtual odf_writer::odf_conversion_context	*odf_context();
		virtual PPTX::Theme							*oox_theme();
		virtual PPTX::Logic::ClrMap					*oox_clrMap();
        virtual std::wstring						find_link_by_id (const std::wstring & sId, int t);
		virtual NSCommon::smart_ptr<OOX::File>		find_file_by_id (const std::wstring & sId);

		void			convert		(OOX::WritingElement *oox_unknown);		
		std::wstring	dump_text	(OOX::WritingElement *oox_unknown);
    private:
		struct _section
		{
            OOX::Logic::CSectionProperty    *props;
            size_t							start_para;
            size_t							end_para;

			bool							bContinue = false;
		}												*current_section_properties;
		OOX::CDocx										*docx_document;
		OOX::CDocxFlat									*docx_flat_document;
		
		odf_writer::odt_conversion_context				*odt_context;
        OOX::Logic::CSectionProperty					*last_section_properties;

		void apply_HF_from(OOX::Logic::CSectionProperty *props, OOX::Logic::CSectionProperty *other);
		
		void convert_document		();
		void convert_styles			();
		void convert_lists_styles	();
		void convert_settings		();
		
		void convert_footnote		(int oox_ref_id);
		void convert_endnote		(int oox_ref_id);
		void convert_comment		(int oox_comm_id);
        void convert_hdr_ftr		(std::wstring sId);

		void convert(OOX::Logic::CSdtContent			*oox_sdt);
		void convert(OOX::Logic::CBackground			*oox_background, int type);
		void convert(OOX::Logic::CBgPict				*oox_background, int type);
		void convert(OOX::Logic::CSdt					*oox_sdt);
		void convert(OOX::Logic::CSectionProperty		*oox_section_pr, bool bSection, const std::wstring & master_name = L"", bool bAlways = false);
		void convert(OOX::Logic::CParagraph				*oox_paragraph);
		void convert(OOX::Logic::CRun					*oox_run);
		void convert(OOX::Logic::CParagraphProperty		*oox_para_prop,	odf_writer::style_paragraph_properties	*paragraph_properties);
		void convert(ComplexTypes::Word::CFramePr		*oox_frame_pr,	odf_writer::style_paragraph_properties	*paragraph_properties);
		void convert(OOX::Logic::CRunProperty			*oox_run_prop,	odf_writer::style_text_properties		*text_properties, bool is_list_styles = false);
		void convert(OOX::Logic::CFldSimple				*oox_fld);
		void convert(OOX::Logic::CFldChar				*oox_fld);
		void convert(OOX::Logic::CInstrText				*oox_instrText);
		void convert(OOX::Logic::CDelText				*oox_delText);
		void convert(OOX::Logic::CText					*oox_text);
		void convert(OOX::Logic::CHyperlink				*oox_hyperlink);
		void convert(OOX::Logic::CSym					*oox_sym);
		void convert(OOX::Logic::CSmartTag				*oox_tag);		
		void convert(OOX::Logic::CPTab					*oox_ptab);
		void convert(OOX::Logic::CBookmarkStart			*oox_bookmark_start);
		void convert(OOX::Logic::CBookmarkEnd			*oox_bookmark_end);
		
		int convert(ComplexTypes::Word::CTrackChange	*oox_change, int type);
		void convert(OOX::Logic::CIns					*oox_ins);
		void convert(OOX::Logic::CDel					*oox_del);
		int  convert(OOX::Logic::CPPrChange				*oox_para_prop_change);
		int  convert(OOX::Logic::CRPrChange				*oox_run_prop_change);
		int  convert(OOX::Logic::CSectPrChange			*oox_sect_prop_change);
		int  convert(OOX::Logic::CTrPrChange			*oox_tr_prop_change);
		int  convert(OOX::Logic::CTcPrChange			*oox_tc_prop_change);
		int  convert(OOX::Logic::CTblPrChange			*oox_table_prop_change);		
		int  convert(OOX::Logic::CTblGridChange			*oox_table_grid_prop_change);		
		
		void convert(OOX::Logic::CAlternateContent		*oox_alt_content);
		void convert(OOX::Logic::CDrawing				*oox_drawing);
		void convert(OOX::Logic::CPicture				*oox_pict);
		void convert(OOX::Logic::CObject				*oox_obj);
		
		void convert(OOX::Drawing::CAnchor				*oox_anchor);
		void convert(OOX::Drawing::CInline				*oox_inline);
	
		void convert(SimpleTypes::CTheme<>				*oox_font_theme,_CP_OPT(std::wstring)				& odf_font_name);
		void convert(ComplexTypes::Word::CShading		*shading,		_CP_OPT(odf_types::color)			& odf_color);
		void convert(ComplexTypes::Word::CColor			*color,			_CP_OPT(odf_types::color)			& odf_color);
		void convert(SimpleTypes::CUniversalMeasure		*oox_size,		_CP_OPT(odf_types::length)			& odf_size);
		void convert(SimpleTypes::CUniversalMeasure		*oox_size,		_CP_OPT(odf_types::length_or_percent) & odf_size);
		void convert(ComplexTypes::Word::CTblWidth		*oox_size,		_CP_OPT(odf_types::length)			& odf_size);
		void convert(ComplexTypes::Word::CJc			*oox_jc,		_CP_OPT(odf_types::text_align)		& align);
		void convert(ComplexTypes::Word::CBorder		*borderProp,	std::wstring & odf_border_prop);
		void convert(ComplexTypes::Word::CPageBorder	*borderProp,	std::wstring & odf_border_prop);
		
		void convert(OOX::Logic::CPBdr					*oox_border,			odf_writer::style_paragraph_properties		*paragraph_properties);
		void convert(OOX::Logic::CTcBorders 			*oox_border,			odf_writer::style_table_cell_properties	*table_cell_properties);
		void convert(OOX::Logic::CTblBorders			*oox_border,			odf_writer::style_table_cell_properties	*table_cell_properties);
		//void convert(OOX::Logic::CTblBorders			*oox_border,			odf_writer::style_table_properties			*table_properties);

		void convert(SimpleTypes::CHexColor<>			*color, SimpleTypes::CThemeColor<>	*theme_color, 
														SimpleTypes::CUcharHexNumber<>* theme_tint,
														SimpleTypes::CUcharHexNumber<>* theme_shade, _CP_OPT(odf_types::color) & odf_color);
		void convert(OOX::CDocDefaults					*def_style, OOX::CStyles *styles);
		void convert(OOX::CStyle						*style);
		void convert_table_style(OOX::CStyle			*oox_style);
		void convert(OOX::Numbering::CLvl *oox_num_lvl, OOX::Numbering::CNumLvl *oox_num_lvl_over, int lvl);

		void convert(OOX::Logic::CCommentRangeStart		*oox_comm_start);
		void convert(OOX::Logic::CCommentRangeEnd		*oox_comm_end);
		void convert(OOX::Logic::CCommentReference		*oox_comm_ref);
		void convert(OOX::CComment						*oox_comm);

		void convert(OOX::Logic::CFootnoteReference		*oox_note_ref);
		void convert(OOX::Logic::CEndnoteReference		*oox_note_ref);

		void convert(OOX::Logic::CTbl					*oox_table);
		void convert(OOX::Logic::CTblGrid				*oox_table_grid);
		void convert(OOX::Logic::CTr					*oox_table_row);
		void convert(OOX::Logic::CTc					*oox_table_cell);
		
		bool convert(OOX::Logic::CTableCellProperties	*oox_table_cell_pr, int col = -1);
		bool convert(OOX::Logic::CTableCellProperties	*oox_table_cell_pr,	odf_writer::style_table_cell_properties	*table_cell_properties/*,odf_writer::style_table_cell_properties	* table_cell_properties = NULL*/);
		bool convert(OOX::Logic::CTableProperty			*oox_table_pr,		bool base_styled);
		bool convert(OOX::Logic::CTableProperty			*oox_table_pr,		odf_writer::style_table_properties		*table_properties);
		void convert(OOX::Logic::CTableProperty			*oox_table_pr,		odf_writer::style_table_cell_properties	*table_cell_properties);
		void convert(OOX::Logic::CTableRowProperties	*oox_table_row_pr);
		void convert(OOX::Logic::CTableRowProperties	*oox_table_row_pr,	odf_writer::style_table_row_properties	*table_row_properties);
		//------------------------------------------------------------------------
		void mrow();
		void endOfMrow();
		void resizeBrackets();
		void convert(OOX::Logic::COMath					*oox_math);
		void convert(OOX::Logic::CMathPr				*oox_math_pr);
		void convert(OOX::Logic::CCtrlPr				*oox_ctrl_pr);
		void convert(OOX::Logic::CAcc					*oox_acc);
		void convert(OOX::Logic::CAccPr					*oox_acc_pr);
		void convert(OOX::Logic::CArgPr					*oox_arg_pr);
		void convert(OOX::Logic::CBar					*oox_bar);
		void convert(OOX::Logic::CBarPr					*oox_bar_pr);
		void convert(OOX::Logic::CBorderBox				*oox_border_box);
		void convert(OOX::Logic::CBorderBoxPr			*oox_border_box_pr);
		void convert(OOX::Logic::CBox					*oox_box);
		void convert(OOX::Logic::CBoxPr					*oox_box_pr);
		void convert(OOX::Logic::CBrk					*oox_brk);
		void convert(OOX::Logic::CDelimiter				*oox_del);
		void convert(OOX::Logic::CDelimiterPr			*oox_del_pr);
		void convert(OOX::Logic::CEqArr					*oox_eq_arr);
		void convert(OOX::Logic::CEqArrPr				*oox_eq_arr_pr);
		void convert(OOX::Logic::CFPr					*oox_f_pr);
		void convert(OOX::Logic::CFraction				*oox_fraction);
		void convert(OOX::Logic::CFunc					*oox_func);
		void convert(OOX::Logic::CFuncPr				*oox_func_pr);
		void convert(OOX::Logic::CGroupChr				*oox_group_ch);
		void convert(OOX::Logic::CGroupChrPr			*oox_group_ch_pr);
		void convert(OOX::Logic::CLimLow				*oox_lim_low);
		void convert(OOX::Logic::CLimLowPr				*oox_lim_low_pr);
		void convert(OOX::Logic::CLimUpp				*oox_lim_upp);
		void convert(OOX::Logic::CLimUppPr				*oox_lim_upp_pr);
		void convert(OOX::Logic::CMathFont				*oox_math_font);		
		void convert(OOX::Logic::CMatrix				*oox_matrix);
		void convert(OOX::Logic::CMc					*oox_mc);
		void convert(OOX::Logic::CMcPr					*oox_mc_pr);
		void convert(OOX::Logic::CMcs					*oox_mcs);
		void convert(OOX::Logic::CMDel					*oox_m_del);
		void convert(OOX::Logic::CMIns					*oox_m_ins);
		void convert(OOX::Logic::CMPr					*oox_m_pr);
		void convert(OOX::Logic::CMr					*oox_mr);
		void convert(OOX::Logic::CMRun					*oox_run);
		void convert(OOX::Logic::CMText					*oox_text);
		void convert(OOX::Logic::CNary					*oox_nary);
		std::pair<bool,bool> convert(OOX::Logic::CNaryPr *oox_nary_pr);
		void convert(OOX::Logic::COMathPara				*oox_math_para);
		void convert(OOX::Logic::COMathParaPr			*oox_math_para_pr);
		void convert(OOX::Logic::CPhant					*oox_phant);
		void convert(OOX::Logic::CPhantPr				*oox_phant_pr);
		void convert(OOX::Logic::CRad					*oox_rad);
		void convert(OOX::Logic::CRadPr					*oox_rad_pr);
		void convert(OOX::Logic::CSPre					*oox_s_pre);
		void convert(OOX::Logic::CSPrePr				*oox_s_pre_pr);
		void convert(OOX::Logic::CSSub					*oox_ssub);
		void convert(OOX::Logic::CSSubPr				*oox_ssub_pr);
		void convert(OOX::Logic::CSSubSup				*oox_ssub_sup);
		void convert(OOX::Logic::CSSubSupPr				*oox_ssub_sup_pr);
		void convert(OOX::Logic::CSSup					*oox_ssup);
		void convert(OOX::Logic::CSSupPr				*oox_ssup_pr);		
		void convert(OOX::Logic::CNum					*oox_num);
		void convert(OOX::Logic::CDen					*oox_den);
		void convert(OOX::Logic::CBegChr				*oox_beg_chr);
		void convert(OOX::Logic::CEndChr				*oox_beg_chr);
		void convert(OOX::Logic::CElement				*oox_elm);
		void convert(OOX::Logic::CDegHide				*oox_deg_hide);
		void convert(OOX::Logic::CDeg *oox_deg, OOX::Logic::CElement *oox_elm);
		void convert(OOX::Logic::CSup *oox_sup, OOX::Logic::CElement *oox_elm);
		void convert(OOX::Logic::CSub *oox_sub, OOX::Logic::CElement *oox_elm);
		void convert(OOX::Logic::CFName					*oox_fname);
		void convert(OOX::Logic::CLim					*oox_lim);
		void convert(OOX::Logic::CChr					*oox_chr);
		void convert(OOX::Logic::CSup					*oox_csup);
		void convert(OOX::Logic::CSub					*oox_csub);
		bool convert(OOX::Logic::CSubHide				*oox_subHide);
		bool convert(OOX::Logic::CSupHide				*oox_supHide);
		void convert(OOX::Logic::CType					*oox_type);
//--------------------------------------------------------------------------------
	};
}
