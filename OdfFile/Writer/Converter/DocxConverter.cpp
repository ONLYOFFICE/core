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
#include "DocxConverter.h"
#include "../../Common/utils.h"

#include "../../../OOXML/DocxFormat/Docx.h"
#include "../../../OOXML/DocxFormat/DocxFlat.h"
#include "../../../OOXML/DocxFormat/Document.h"
#include "../../../OOXML/DocxFormat/Endnote.h"
#include "../../../OOXML/DocxFormat/Footnote.h"
#include "../../../OOXML/DocxFormat/FontTable.h"
#include "../../../OOXML/DocxFormat/Numbering.h"
#include "../../../OOXML/DocxFormat/Styles.h"
#include "../../../OOXML/DocxFormat/Comments.h"
#include "../../../OOXML/DocxFormat/Settings/WebSettings.h"
#include "../../../OOXML/DocxFormat/Settings/Settings.h"
#include "../../../OOXML/DocxFormat/External/HyperLink.h"
#include "../../../OOXML/DocxFormat/HeaderFooter.h"

#include "../../../OOXML/DocxFormat/External/HyperLink.h"
#include "../../../OOXML/XlsxFormat/Chart/Chart.h"
#include "../../../OOXML/DocxFormat/Logic/Sdt.h"
#include "../../../OOXML/DocxFormat/Logic/Pict.h"
#include "../../../OOXML/DocxFormat/Logic/Table.h"
#include "../../../OOXML/DocxFormat/Logic/Hyperlink.h"
#include "../../../OOXML/DocxFormat/Logic/AlternateContent.h"
#include "../../../OOXML/DocxFormat/Logic/Paragraph.h"
#include "../../../OOXML/DocxFormat/Logic/Bdo.h"
#include "../../../OOXML/DocxFormat/Logic/Dir.h"
#include "../../../OOXML/DocxFormat/Logic/SmartTag.h"
#include "../../../OOXML/DocxFormat/Logic/ParagraphProperty.h"
#include "../../../OOXML/DocxFormat/Logic/FldSimple.h"
#include "../../../OOXML/DocxFormat/Logic/Run.h"
#include "../../../OOXML/DocxFormat/Logic/RunProperty.h"

#include "VmlShapeTypes2Oox.h"

#include "../Format/odt_conversion_context.h"

#include "../Format/odf_text_context.h"
#include "../Format/odf_drawing_context.h"

#include "../Format/styles.h"

#include "../Format/style_page_layout_properties.h"
#include "../Format/style_table_properties.h"
#include "../Format/style_text_properties.h"
#include "../Format/style_paragraph_properties.h"
#include "../Format/style_graphic_properties.h"
#include "../Format/styles_list.h"

using namespace cpdoccore;

std::vector<double> current_font_size;

namespace Oox2Odf
{

	bool compare (OOX::Logic::CSectionProperty* props1, OOX::Logic::CSectionProperty* props2)
	{
		size_t cols_1 = 1, cols_2 = 1;
		if (props1 && props1->m_oCols.IsInit()) 
		{
			cols_1 = props1->m_oCols->m_oNum.IsInit() ? props1->m_oCols->m_oNum->GetValue() : 1;

			if (!props1->m_oCols->m_arrColumns.empty())
                cols_1 = (std::min)(cols_1, props1->m_oCols->m_arrColumns.size());
		}
		if (props2)
		{
			if (props2->m_oCols.IsInit()) 
			{
				cols_2 = props2->m_oCols->m_oNum.IsInit() ? props2->m_oCols->m_oNum->GetValue() : 1;
				
				if (!props2->m_oCols->m_arrColumns.empty())
                    cols_2 = (std::min)(cols_2, props2->m_oCols->m_arrColumns.size());
			}
			else
			{
				if (props2->m_oType.IsInit() && props2->m_oType->m_oVal.IsInit()) 
				{
					if (props2->m_oType->m_oVal->GetValue() == SimpleTypes::sectionmarkContinious)
						cols_2 = cols_1;
				}
			}
		}
		return cols_1 == cols_2;
	}
DocxConverter::DocxConverter(const std::wstring & path, bool bTemplate) : docx_flat_document(NULL), docx_document(NULL)
{
    const OOX::CPath oox_path(std::wstring(path.c_str()));

    docx_document   = new OOX::CDocx(oox_path);

	if (docx_document && !docx_document->m_oMain.document)
	{
		delete docx_document; docx_document = NULL;
		docx_flat_document = new OOX::CDocxFlat(oox_path);
	}

	output_document = new odf_writer::package::odf_document(L"text", bTemplate);
    odt_context     = new odf_writer::odt_conversion_context(output_document);

//set flags to default
	current_section_properties	= NULL;
    last_section_properties		= NULL;
}
DocxConverter::~DocxConverter()
{
	if (odt_context)			delete odt_context;			odt_context		= NULL;
	if (output_document)		delete output_document;		output_document = NULL;
	if (docx_document)			delete docx_document;		docx_document	= NULL;
	if (docx_flat_document)		delete docx_flat_document;	docx_flat_document	= NULL;
}
odf_writer::odf_conversion_context* DocxConverter::odf_context()
{
	return odt_context;
}
PPTX::Theme* DocxConverter::oox_theme()
{
	if (docx_document)
		return docx_document->m_pTheme;
	else
		return NULL;
}
OOX::IFileContainer* DocxConverter::current_document()
{
	if (oox_current_child_document)
		return oox_current_child_document;
	else if (docx_document)
		return dynamic_cast<OOX::IFileContainer*>(docx_document->m_oMain.document);

	return NULL;
}
NSCommon::smart_ptr<OOX::File> DocxConverter::find_file_by_id(const std::wstring &sId)
{
	smart_ptr<OOX::File> oFile;

	if (oox_current_child_document)
		oFile = oox_current_child_document->Find(sId);
	else if ((docx_document) && (docx_document->m_oMain.document))
		oFile = docx_document->m_oMain.document->Find(sId);
		
	return oFile;
}

std::wstring DocxConverter::find_link_by_id (const std::wstring & sId, int type, bool & bExternal)
{
	bExternal = false;

    std::wstring			ref;
	smart_ptr<OOX::File>	oFile;

    if (oox_current_child_document)
	{
		oFile	= oox_current_child_document->Find(sId);
		ref		= OoxConverter::find_link_by(oFile, type, bExternal);
	}
	if (!ref.empty()) return ref;

	if (!docx_document) return L"";
	if (docx_document->m_oMain.document == NULL) return L"";
	
	oFile	= docx_document->m_oMain.document->Find(sId);
	ref		= OoxConverter::find_link_by(oFile, type, bExternal);

	return ref;
}

bool DocxConverter::convertDocument()
{
    if (!odt_context)   return false;
    if (!docx_document && !docx_flat_document) return false;
		
	odt_context->start_document();

	convert_lists_styles();
	convert_styles();

	convert_settings(); 
	
	convert_document();

	//удалим уже ненужный документ docx 
	if (docx_document)		delete docx_document; docx_document = NULL;
	if (docx_flat_document)	delete docx_flat_document; docx_flat_document = NULL;

	odt_context->end_document();
	return true;
}

void DocxConverter::convert_document()
{
	OOX::CDocument *doc = docx_document ? docx_document->m_oMain.document : (docx_flat_document ? docx_flat_document->m_pDocument.GetPointer() : NULL);
	
	if (!doc)return;

	std::vector<_section> sections;
//----------------------------------------------------------------------------------------------------------

	OOX::Logic::CSectionProperty* prev = NULL;

	if (false == doc->m_arrSections.empty())
	{
		for (size_t i = 0; i < doc->m_arrSections.size(); ++i)
		{
			_section section;

			section.props = dynamic_cast<OOX::Logic::CSectionProperty*>(doc->m_arrSections[i].sect);
			section.start_para = doc->m_arrSections[i].start_elm;
			section.end_para = doc->m_arrSections[i].end_elm;
			section.bContinue = compare(prev, section.props);

			sections.push_back(section);

			prev = section.props;
		}
	}
	else
	{
		//считаем количесво секций и запоминаем их свойства .. 
		size_t last_section_start = 0;
		for (size_t i = 0; i < doc->m_arrItems.size(); ++i)
		{
			if ((doc->m_arrItems[i]) == NULL) continue;

			if (doc->m_arrItems[i]->getType() == OOX::et_w_p)
			{
				OOX::Logic::CParagraph * para = dynamic_cast<OOX::Logic::CParagraph *>(doc->m_arrItems[i]);

				if ((para) && (para->m_oParagraphProperty))
				{
					if (para->m_oParagraphProperty->m_oSectPr.IsInit())
					{
						_section section;

						section.props = para->m_oParagraphProperty->m_oSectPr.GetPointer();
						section.start_para = last_section_start;
						section.end_para = i + 1;
						section.bContinue = compare(prev, section.props);

						sections.push_back(section);

						last_section_start = i + 1;
						prev = section.props;
					}
				}
			}
		}
		_section section;

		section.props = doc->m_oSectPr.GetPointer();
		section.start_para = last_section_start;
		section.end_para = doc->m_arrItems.size();
		section.bContinue = compare(prev, section.props);

		sections.push_back(section);
	}					
//----------------------------------------------------------------------------------------------------------

	convert(doc->m_oSectPr.GetPointer(), false, L"Standard", true);
	
	odt_context->text_context()->clear_params();

	for (size_t sect = 0; sect < sections.size(); sect++)
	{
		current_section_properties = &sections[sect];

        for (size_t i = sections[sect].start_para; i < sections[sect].end_para; ++i)
		{
			convert(doc->m_arrItems[i]);
		}
	}
//-----------------------------------------------------------------------------------------------------------------
	if (docx_document)
	{
		OoxConverter::convert (docx_document->m_pJsaProject);
	}
}
std::wstring DocxConverter::dump_text(OOX::WritingElement *oox_unknown)
{
	std::wstring result;

	if (oox_unknown == NULL)return result;

	switch(oox_unknown->getType())
	{
		case OOX::et_w_t:
		{
			OOX::Logic::CText* text = dynamic_cast<OOX::Logic::CText*>(oox_unknown);

			if (text)
				result += text->m_sText;
		}break;
		case OOX::et_w_r:
		{
			OOX::Logic::CRun* run = dynamic_cast<OOX::Logic::CRun*>(oox_unknown);

			for (size_t i = 0; (run) && (i < run->m_arrItems.size()); i++)
			{
				result += dump_text(run->m_arrItems[i]);
			}
		}break;
		case OOX::et_w_rPr:
		{
			odf_writer::style_text_properties text_properties;
			convert(dynamic_cast<OOX::Logic::CRunProperty*>(oox_unknown), &text_properties);
			odt_context->drawing_context()->set_text_properties(&text_properties);
		}break;
		case OOX::et_w_p:
		{
			OOX::Logic::CParagraph* para = dynamic_cast<OOX::Logic::CParagraph*>(oox_unknown);
			for (size_t i = 0; (para) && (i < para->m_arrItems.size()); i++)
			{
				result += dump_text(para->m_arrItems[i]);
			}
			result += L"\\n";
		}break;
		default:
		{
		}break;
	}
	return result;
}

void DocxConverter::convert(OOX::WritingElement  *oox_unknown)
{
	if (oox_unknown == NULL)return;

	switch(oox_unknown->getType())
	{
		case OOX::et_w_ptab:
		{
			convert(dynamic_cast<OOX::Logic::CPTab*>(oox_unknown));
		}break;
		case OOX::et_w_sdt:
		{
			convert(dynamic_cast<OOX::Logic::CSdt*>(oox_unknown));
		}break;
		case OOX::et_w_fldSimple:	
		{
			convert(dynamic_cast<OOX::Logic::CFldSimple*>(oox_unknown));
		}break;
		case OOX::et_w_bookmarkStart:
		{
			convert(dynamic_cast<OOX::Logic::CBookmarkStart*>(oox_unknown));
		}break;
		case OOX::et_w_bookmarkEnd:
		{
			convert(dynamic_cast<OOX::Logic::CBookmarkEnd*>(oox_unknown));
		}break;
		case OOX::et_w_r:
		{
			convert(dynamic_cast<OOX::Logic::CRun*>(oox_unknown));
		}break;
		case OOX::et_w_p:
		{
			convert(dynamic_cast<OOX::Logic::CParagraph*>(oox_unknown));
		}break;
		case OOX::et_w_ins:
		{
			convert(dynamic_cast<OOX::Logic::CIns*>(oox_unknown));
		}break;
		case OOX::et_w_del:
		{
			convert(dynamic_cast<OOX::Logic::CDel*>(oox_unknown));
		}break;
		case OOX::et_w_smartTag:
		{
			convert(dynamic_cast<OOX::Logic::CSmartTag*>(oox_unknown));
		}break;
		case OOX::et_w_pPr:
		{
			convert(dynamic_cast<OOX::Logic::CParagraphProperty*>(oox_unknown));
		}break;
		case OOX::et_w_hyperlink:
		{
			convert(dynamic_cast<OOX::Logic::CHyperlink*>(oox_unknown));
		}break;
		case OOX::et_w_rPr:
		{
			convert(dynamic_cast<OOX::Logic::CRunProperty*>(oox_unknown));
		}break;
		case OOX::et_mc_alternateContent:
		{
			convert(dynamic_cast<OOX::Logic::CAlternateContent*>(oox_unknown));
		}break;
		case OOX::et_w_drawing:
		{
			convert(dynamic_cast<OOX::Logic::CDrawing*>(oox_unknown));
		}break;
		case OOX::et_w_pict:
		{
			convert(dynamic_cast<OOX::Logic::CPicture*>(oox_unknown));
		}break;
		case OOX::et_w_object:
		{
			convert(dynamic_cast<OOX::Logic::CObject*>(oox_unknown));
		}break;
		case OOX::et_pic_pic:
		case OOX::et_pic:
		case OOX::et_p_pic:
		{
			convert(dynamic_cast<PPTX::Logic::Pic*>(oox_unknown));
		}break;
		case OOX::et_w_GroupShape:
		case OOX::et_p_ShapeTree:
		case OOX::et_lc_LockedCanvas:
		{
			convert(dynamic_cast<PPTX::Logic::SpTree*>(oox_unknown));
		}break;

		case OOX::et_w_commentRangeEnd:
		{
			convert(dynamic_cast<OOX::Logic::CCommentRangeEnd*>(oox_unknown));
		}break;
		case OOX::et_w_commentRangeStart:
		{
			convert(dynamic_cast<OOX::Logic::CCommentRangeStart*>(oox_unknown));
		}break;
		case OOX::et_w_comment:
		{
			convert(dynamic_cast<OOX::CComment*>(oox_unknown));
		}break;
		case OOX::et_w_commentReference:
		{
			convert(dynamic_cast<OOX::Logic::CCommentReference*>(oox_unknown));		
					//если нет Start - означает начало с предыдущего Run
		}break;
		case OOX::et_w_footnoteReference:
		{
			convert(dynamic_cast<OOX::Logic::CFootnoteReference*>(oox_unknown));
		}break;
		case OOX::et_w_endnoteReference:
		{
			convert(dynamic_cast<OOX::Logic::CEndnoteReference*>(oox_unknown));
		}break;
		case OOX::et_w_endnoteRef:
		{
			//add ref ??? todoooo
		}break;
		case OOX::et_w_footnoteRef:
		{
			//add ref 
		}break;
		case OOX::et_w_tbl:
		{
			convert(dynamic_cast<OOX::Logic::CTbl*>(oox_unknown));
		}break;
		case OOX::et_w_tr:
		{
			convert(dynamic_cast<OOX::Logic::CTr*>(oox_unknown));
		}break;
		case OOX::et_w_tc:
		{
			convert(dynamic_cast<OOX::Logic::CTc*>(oox_unknown));
		}break;		
		default:
		{
			OoxConverter::convert(oox_unknown);
		}break;
	}
}

void DocxConverter::convert(OOX::Logic::CSdt *oox_sdt)
{
	if (oox_sdt == NULL) return;

	bool bField = false, bForm = false;
	
	_CP_OPT(double) x, y, width = 20, height = 20; 

	if (oox_sdt->m_oSdtPr.IsInit())
	{
		switch(oox_sdt->m_oSdtPr->m_eType)
		{
			case OOX::Logic::sdttypeBibliography:
			case OOX::Logic::sdttypeDocPartList:
			case OOX::Logic::sdttypeDocPartObj:
			case OOX::Logic::sdttypeCitation:
			{
				bField = true;
				odt_context->start_field(false);
			}break;
			case OOX::Logic::sdttypeComboBox:
			case OOX::Logic::sdttypeDropDownList:
			case OOX::Logic::sdttypeDate:
			case OOX::Logic::sdttypeCheckBox:
			//case OOX::Logic::sdttypePicture:
			//case OOX::Logic::sdttypeEquation:
			//case OOX::Logic::sdttypeGroup:
			case OOX::Logic::sdttypeRichText:
			case OOX::Logic::sdttypeText:
			{
				std::wstring id = odt_context->controls_context()->start_control_sdt(oox_sdt->m_oSdtPr->m_eType);

				if (false == id.empty())
				{				
					bForm = true;
					odt_context->start_drawing_context();
					odt_context->drawing_context()->set_anchor(odf_types::anchor_type::AsChar);
					odt_context->drawing_context()->set_drawings_rect(x, y, width, height); //default
					
					width = boost::none;
					
					odt_context->drawing_context()->start_drawing();
					odt_context->drawing_context()->start_control(id);
				}
			}break;
		}
		if (bForm)
		{
			odt_context->drawing_context()->set_vertical_rel(0); //baseline
			odt_context->drawing_context()->set_textarea_vertical_align(1);//middle
		}
		if (bForm && oox_sdt->m_oSdtPr->m_oDate.IsInit())
		{
			odt_context->controls_context()->add_property(L"Dropdown", odf_types::office_value_type::Boolean, L"true");
			
			if (oox_sdt->m_oSdtPr->m_oDate->m_oFullDate.IsInit())
			{
				odt_context->controls_context()->add_property(L"Dropdown", odf_types::office_value_type::Boolean, L"true");
				//odt_context->controls_context()->set_value(oox_sdt->m_oSdtPr->m_oDate->m_oFullDate->ToString());
			}
			if ((oox_sdt->m_oSdtPr->m_oDate->m_oDateFormat.IsInit()) && 
				(oox_sdt->m_oSdtPr->m_oDate->m_oDateFormat->m_sVal.IsInit()))
			{
				//odt_context->controls_context()->set_format(oox_sdt->m_oSdtPr->m_oDate->m_oDateFormat->m_sVal.get2());
			}
		}
		if (bForm && oox_sdt->m_oSdtPr->m_oDropDownList.IsInit())
		{
			odt_context->controls_context()->set_drop_down(true);

			size_t size = 0;
			for ( size_t i = 0; i < oox_sdt->m_oSdtPr->m_oDropDownList->m_arrListItem.size(); i++ )
			{
				if ( oox_sdt->m_oSdtPr->m_oDropDownList->m_arrListItem[i] )
				{
					std::wstring val = oox_sdt->m_oSdtPr->m_oDropDownList->m_arrListItem[i]->m_sValue.get_value_or(L"");

					if (val.length() > size) size = val.length();
					odt_context->controls_context()->add_item(val);
				}
			}		
			width = 10. * size; //todooo sizefont
			odt_context->drawing_context()->set_size(width, height, true);			
		}
		if (bForm && oox_sdt->m_oSdtPr->m_oComboBox.IsInit())
		{
			size_t size = 0;
			for ( size_t i = 0; i < oox_sdt->m_oSdtPr->m_oComboBox->m_arrListItem.size(); i++ )
			{
				if ( oox_sdt->m_oSdtPr->m_oComboBox->m_arrListItem[i] )
				{
					std::wstring val = oox_sdt->m_oSdtPr->m_oComboBox->m_arrListItem[i]->m_sValue.get_value_or(L"");

					if (val.length() > size) size = val.length();
					odt_context->controls_context()->add_item(val);
				}
			}
			width = 10. * size; //todooo sizefont

			odt_context->drawing_context()->set_size(width, height, true);
		}
		if (bForm && oox_sdt->m_oSdtPr->m_oCheckbox.IsInit())
		{
			if (oox_sdt->m_oSdtPr->m_oCheckbox->m_oChecked.IsInit())
			{
				odt_context->controls_context()->set_check_state(oox_sdt->m_oSdtPr->m_oCheckbox->m_oChecked->m_oVal.ToBool() ? 1 : 0);
			}
		}

//-----------------------------------------------
		std::wstring name;
		if (oox_sdt->m_oSdtPr->m_oAlias.IsInit())
		{
			name = oox_sdt->m_oSdtPr->m_oAlias->ToString2();
		}
		if (name.empty() && (oox_sdt->m_oSdtPr->m_oId.IsInit()) && (oox_sdt->m_oSdtPr->m_oId->m_oVal.IsInit()))
		{
			name = std::to_wstring(*oox_sdt->m_oSdtPr->m_oId->m_oVal);
		}
		if (bField)
			odt_context->set_field_name(name);
		if (bForm)
			odt_context->controls_context()->set_name(name);
//-----------------------------------------------
		if (oox_sdt->m_oSdtPr->m_oColor.IsInit())
		{
			_CP_OPT(odf_types::color) color;
			convert (	oox_sdt->m_oSdtPr->m_oColor->m_oVal.GetPointer(), 
						oox_sdt->m_oSdtPr->m_oColor->m_oThemeColor.GetPointer(), 
						oox_sdt->m_oSdtPr->m_oColor->m_oThemeTint.GetPointer(), 
						oox_sdt->m_oSdtPr->m_oColor->m_oThemeShade.GetPointer(), color);
			
			if (color)
			{
				if (bField)
					odt_context->set_field_color(color);
				if (bForm)
				{
					odf_context()->drawing_context()->start_area_properties();
						odf_context()->drawing_context()->set_solid_fill(color->get_hex_value());
					odf_context()->drawing_context()->end_area_properties();
				}
			}
		}
	}
//-----------------------------------------------
	if (bForm && oox_sdt->m_oSdtContent.IsInit())
	{
		std::wstring value;
		for (size_t i = 0; i < oox_sdt->m_oSdtContent->m_arrItems.size(); i++)
		{
			value += dump_text(oox_sdt->m_oSdtContent->m_arrItems[i]);
		}
		if ((false == value.empty()) && (value.back() == L'\n'))
		{
			value.erase(value.back());
		}
		odt_context->controls_context()->set_value(value);

		if (!width)
		{
			width = 10. * value.length(); //todooo sizefont
			odt_context->drawing_context()->set_size(width, height, true);
		}
	}
	else
	{
		convert(oox_sdt->m_oSdtContent.GetPointer());
	}

	if (bField)
	{
		odt_context->end_field();
	}
	if (bForm)
	{
		odt_context->drawing_context()->end_control();
		odt_context->drawing_context()->end_drawing();

		odt_context->end_drawing_context();
		odt_context->controls_context()->end_control();
	}
}
void DocxConverter::convert(OOX::Logic::CSdtContent *oox_sdt)
{
	if (oox_sdt == NULL) return;

    for (size_t i = 0; i < oox_sdt->m_arrItems.size(); ++i)
	{
		convert(oox_sdt->m_arrItems[i]);
	}
}
void DocxConverter::convert(OOX::Logic::CParagraph *oox_paragraph)
{
	if (oox_paragraph == NULL) return; 
	
//---------------------------------------------------------------------------------------------------------------------
	if (current_section_properties)
	{
		if (odt_context->text_context()->get_KeepNextParagraph())
			odt_context->end_paragraph();
		odt_context->text_context()->set_KeepNextParagraph(false);
	}
//---------------------------------------------------------------------------------------------------------------------
	if (false == current_font_size.empty())
	{
		current_font_size.erase(current_font_size.begin() + 1, current_font_size.end());
	}

	bool bStyled			= false;
	bool bStartNewParagraph = !odt_context->text_context()->get_KeepNextParagraph();
	
	bool		 list_present = false;
	std::wstring list_style_name;

	int			 list_level		= -1;
	int			 list_style_id	= -1;

//---------------------------------------------------------------------------------------------------------------------
	std::vector<std::pair<int, int>> id_change_properties;
	
	if (oox_paragraph->m_oParagraphProperty)
	{//цепочка изменений форматов в удаленных кусках либрой (и оо) не поддерживается - 
		int id;

		if (oox_paragraph->m_oParagraphProperty->m_oRPr.IsInit())
		{
			//удаление знака абзаца - объединение со следующим - в либре нету
			//id = convert(oox_paragraph->m_oParagraphProperty->m_oRPr->m_oDel.GetPointer(), 2);
			//if (id >= 0)	
			//	id_change_properties.push_back(std::pair<int, int> (2, id)); 

			//вставка знака абзаца - разделение текущего параграфа - в либре нету
			//if (oox_paragraph->m_arrItems.size() < 2)//только для пустых 
			{
				id = convert(oox_paragraph->m_oParagraphProperty->m_oRPr->m_oIns.GetPointer(), 1); 
				if (id >= 0)	
					id_change_properties.push_back(std::pair<int, int> (1, id));
			}

			id = convert(oox_paragraph->m_oParagraphProperty->m_oRPr->m_oRPrChange.GetPointer());
			if (id >= 0)	
				id_change_properties.push_back(std::pair<int, int> (3, id));
		}			
		
		//if (oox_paragraph->m_oParagraphProperty->m_oSectPr.IsInit())
		//{
		//	id = convert(oox_paragraph->m_oParagraphProperty->m_oSectPr->m_oSectPrChange.GetPointer());
		//	if (id >= 0)	id_change_properties.push_back(std::pair<int, int> (3, id));
		//}

		id = convert(oox_paragraph->m_oParagraphProperty->m_oPPrChange.GetPointer());
		if (id >= 0)	
			id_change_properties.push_back(std::pair<int, int> (3, id));
	}	
//---------------------------------------------------------------------------------------------------------------------
	if(oox_paragraph->m_oParagraphProperty && odt_context->text_context()->get_list_item_state() == false)
	{
		if (oox_paragraph->m_oParagraphProperty->m_oPStyle.IsInit() && oox_paragraph->m_oParagraphProperty->m_oPStyle->m_sVal.IsInit())
		{
			std::wstring style_name = *oox_paragraph->m_oParagraphProperty->m_oPStyle->m_sVal;

			odf_writer::odf_style_state_ptr style_state;
			if (odt_context->styles_context()->find_odf_style_state(style_name, odf_types::style_family::Paragraph, style_state) && style_state)
			{
				list_present	= style_state->get_list_style_exist();
				list_level		= style_state->get_list_style_level();
				list_style_id	= style_state->get_list_style_id();
			}
		}
		if (oox_paragraph->m_oParagraphProperty->m_oNumPr.IsInit())
		{
			if (bStartNewParagraph == false)//demo.docx
			{
				odt_context->end_paragraph();
				odt_context->text_context()->set_KeepNextParagraph(false);
				bStartNewParagraph = true; 
			}
			list_present = true;
		
			if (oox_paragraph->m_oParagraphProperty->m_oNumPr->m_oIlvl.IsInit() && oox_paragraph->m_oParagraphProperty->m_oNumPr->m_oIlvl->m_oVal.IsInit())
				list_level = *oox_paragraph->m_oParagraphProperty->m_oNumPr->m_oIlvl->m_oVal;		
			
			if (oox_paragraph->m_oParagraphProperty->m_oNumPr->m_oNumID.IsInit() && oox_paragraph->m_oParagraphProperty->m_oNumPr->m_oNumID->m_oVal.IsInit())
			{
				list_style_id = *oox_paragraph->m_oParagraphProperty->m_oNumPr->m_oNumID->m_oVal;
			}
		}
	}
	if (list_present)
	{
		if (list_style_id < 0)
			list_style_id = odt_context->text_context()->list_state_.prevNumID;
		else
			odt_context->text_context()->list_state_.prevNumID = list_style_id;
	}
	if (oox_paragraph->m_oParagraphProperty || odt_context->is_empty_section() || current_section_properties)
	{
		bStyled			= true;
		bool bRunPara	= oox_paragraph->m_oParagraphProperty ? (oox_paragraph->m_oParagraphProperty->m_oRPr.IsInit() ? true : false) : false;
		
		odf_writer::style_paragraph_properties	*paragraph_properties	= NULL;
		odf_writer::style_text_properties		*text_properties		= NULL;

		if (odt_context->text_context()->get_KeepNextParagraph())
		{
			odf_writer::odf_style_state_ptr state = odt_context->styles_context()->last_state(odf_types::style_family::Paragraph);
			if (state)
			{
				paragraph_properties = state->get_paragraph_properties();

				if (bRunPara) 
					text_properties = state->get_text_properties();
				
				if (odt_context->in_drop_cap()) //после буквицы (Nadpis.docx) - нужно накатить свойства параграфа нормального
				{
					//очистить все кроме drop_cap
					paragraph_properties->content_.clear(false);
				}
				else
				{
					// ????
					if (oox_paragraph->m_oParagraphProperty && oox_paragraph->m_oParagraphProperty->m_oPStyle.IsInit() && oox_paragraph->m_oParagraphProperty->m_oPStyle->m_sVal.IsInit())
					{
						//перезатираем все свойства ... наложение не катит -- ваще то надо чистить после буквицы (Nadpis.docx) .. проверить надобность с остальными случами
						paragraph_properties->content_.clear(true);
						if (text_properties)
							text_properties->content_.clear();
					}
				}
			}
		}
		else
		{
			odt_context->styles_context()->create_style(L"", odf_types::style_family::Paragraph, true, false, -1);					
			
			paragraph_properties	= odt_context->styles_context()->last_state()->get_paragraph_properties();
			if (bRunPara)
				text_properties		= odt_context->styles_context()->last_state()->get_text_properties();
			
			if(list_present && list_style_id >= 0)
			{
				list_style_name = odt_context->styles_context()->lists_styles().get_style_name(list_style_id); 
				odt_context->styles_context()->last_state()->set_list_style_name(list_style_name);
			}
		}
		if (odt_context->in_drop_cap())
		{
			odt_context->end_drop_cap(); 
		}

		convert(oox_paragraph->m_oParagraphProperty, paragraph_properties); 
		
		if (text_properties && oox_paragraph->m_oParagraphProperty)
		{
			if (odt_context->in_drop_cap())
			{
				//вообще то свойства правильные параграфа идут в следующем после буквицы параграфе
				odf_writer::style_text_properties *text_properties_drop_cap = odt_context->get_drop_cap_properties();
				convert(oox_paragraph->m_oParagraphProperty->m_oRPr.GetPointer(), text_properties_drop_cap); 
				if (text_properties_drop_cap->content_.fo_font_size_)
				{
					double sz = text_properties_drop_cap->content_.fo_font_size_->get_length().get_value_unit(odf_types::length::pt);

					sz = sz / 1.75 / odt_context->get_drop_cap_lines();
					text_properties_drop_cap->content_.fo_font_size_= odf_types::length(sz, odf_types::length::pt);
				}
			}
			else
			{
				//Thesis.docx
				convert(oox_paragraph->m_oParagraphProperty->m_oRPr.GetPointer(), text_properties, true); 
			}
		}
	}
	else 
	{
		odt_context->text_context()->set_KeepNextParagraph(false); //НУЖНО ПРИВЯЗАТЬ к УРОВНЮ на котором хранить m_bKeepNextParagraph !!! todooo
	//пока - если следующий не параграф - скидываем !!!
	}

	if(list_present)
	{
		odt_context->start_list_item(list_level, list_style_name);
	}

	if ((list_present = odt_context->text_context()->get_list_item_state()) == false) odt_context->set_no_list();

	bool empty_para = !((oox_paragraph->m_oParagraphProperty) && (oox_paragraph->m_oParagraphProperty->m_oRPr.IsInit())) ;
	if (empty_para && oox_paragraph->m_arrItems.size() < 2 && odt_context->text_context()->get_KeepNextParagraph())
	{//rapcomnat12.docx - стр 185

        for (size_t i = 0; i < oox_paragraph->m_arrItems.size(); ++i)
		{		
			switch(oox_paragraph->m_arrItems[i]->getType())
			{
				case OOX::et_w_pPr: break;
				default:
					empty_para = false;
			}
		}
		if (empty_para) return;
	}

	if (bStartNewParagraph)
	{
		odt_context->start_paragraph(bStyled);

		if (odt_context->is_paragraph_in_current_section_)
		{
			odt_context->set_master_page_name(odt_context->page_layout_context()->last_master() ?
											  odt_context->page_layout_context()->last_master()->get_name() : L"");
		}		
	}

//---------------------------------------------------------------------------------------------------------------------
    for (size_t i = 0; i < oox_paragraph->m_arrItems.size(); ++i)
	{
		//те элементы которые тока для Paragraph - здесь - остальные в общей куче		
		switch(oox_paragraph->m_arrItems[i]->getType())
		{
			case OOX::et_w_pPr:
			{
				// пропускаем .. 
			}break;		
			default:
				convert(oox_paragraph->m_arrItems[i]);
				break;
		}
	}
//---------------------------------------------------------------------------------------------------------------------

	if (!odt_context->text_context()->get_KeepNextParagraph())  odt_context->end_paragraph();
	
	if(list_present && !odt_context->text_context()->get_KeepNextParagraph())
	{
		odt_context->end_list_item();
	}
//---------------------------------------------------------------------------------------------------------------------
	//std::sort(id_change_properties.begin(), id_change_properties.end());

	for (size_t i = 0; i < id_change_properties.size(); i++)
	{
		odt_context->end_change(id_change_properties[i].second, id_change_properties[i].first); 
	}
}
void DocxConverter::convert(OOX::Logic::CRun *oox_run)//wordprocessing 22.1.2.87 math 17.3.2.25
{
	if (oox_run == NULL) return;
	//хм разобраться а нужен ли он ... частенько бывает в неправильном месте!!! - A   GRUBU.docx
	//https://forums.asp.net/t/1951556.aspx?Facing+problem+finding+out+page+end+in+Ms+Word+Open+XML+SDK+Asp+net+c+We+can+t+get+a+lastRenderedPageBreak
	////test for break - 2 first element ЭТОТ элемент НУЖНО вытащить отдельно !!!
	//for(size_t i = 0; i < (std::min) ( (size_t)2, oox_run->m_arrItems.size()); ++i)
	//{
	//	if (*it->getType() == OOX::et_w_lastRenderedPageBreak)
	//	{
	//		odt_context->add_page_break();
	//	}
	//}	
	bool styled					= false;	
	int id_change_properties	= -1;

	if (oox_run->m_oRunProperty) 
	{
		styled = true;

		id_change_properties = convert(oox_run->m_oRunProperty->m_oRPrChange.GetPointer());
		
		odt_context->styles_context()->create_style(L"", odf_types::style_family::Text, true, false, -1);					
		odf_writer::style_text_properties *text_properties = odt_context->styles_context()->last_state()->get_text_properties();

		convert(oox_run->m_oRunProperty, text_properties);
	}

	odt_context->start_run(styled);
	
    for (size_t i = 0; i < oox_run->m_arrItems.size(); ++i)
	{
		//те элементы которые тока для Run - здесь - остальные в общей куче		
		switch(oox_run->m_arrItems[i]->getType())
		{
			case OOX::et_w_fldChar:	
			{
				OOX::Logic::CFldChar* pFldChar= dynamic_cast<OOX::Logic::CFldChar*>(oox_run->m_arrItems[i]);
				convert(pFldChar);
			}break;
			case OOX::et_w_instrText:
			{
				OOX::Logic::CInstrText* pInstrText= dynamic_cast<OOX::Logic::CInstrText*>(oox_run->m_arrItems[i]);
				convert(pInstrText);
			}break;
			case OOX::et_w_delText:
			{
				OOX::Logic::CDelText* pDelText= dynamic_cast<OOX::Logic::CDelText*>(oox_run->m_arrItems[i]);
				convert(pDelText);
			}break;
			case OOX::et_w_rPr:	// пропускаем .. 
			{
			}break;		
			case OOX::et_w_lastRenderedPageBreak: // не информативное .. может быть неверно записано
			{
			}break;
			case OOX::et_w_br:
			{
				OOX::Logic::CBr* pBr= dynamic_cast<OOX::Logic::CBr*>(oox_run->m_arrItems[i]);
				if (pBr)
				{
					int type = pBr->m_oType.GetValue();
					
					bool need_restart_para = odt_context->text_context()->set_type_break(type, pBr->m_oClear.GetValue());

					if (need_restart_para)
						odt_context->add_paragraph_break(type);
				}
			}break;
			case OOX::et_w_t:
			{
				OOX::Logic::CText* pText= dynamic_cast<OOX::Logic::CText*>(oox_run->m_arrItems[i]);
				convert(pText);
			}break;
			case OOX::et_w_sym:
			{
				OOX::Logic::CSym* pSym= dynamic_cast<OOX::Logic::CSym*>(oox_run->m_arrItems[i]);
				convert(pSym);
			}break;
			case OOX::et_w_tab:
			{
				OOX::Logic::CTab* pTab= dynamic_cast<OOX::Logic::CTab*>(oox_run->m_arrItems[i]);
				odt_context->text_context()->add_tab();
			}break;

			case OOX::et_w_separator:
			case OOX::et_w_continuationSeparator:
			{
			}break;
			//contentPart
			//cr
			//dayLong, dayShort, monthLong, monthShort, yearLong, yearShort
			//noBreakHyphen
			//pgNum
			//ruby
			//softHyphen
			//delInstrText
			default:
				convert(oox_run->m_arrItems[i]);
		}
	}
	odt_context->end_run();

	if (id_change_properties >= 0)
		odt_context->end_change(id_change_properties, 3); //todooo change int type to enum

}
void DocxConverter::convert(OOX::Logic::CBookmarkStart *oox_bookmark_start)
{
	if (!oox_bookmark_start) return;
	
	if (oox_bookmark_start->m_sName.IsInit() == false) return;
	if (oox_bookmark_start->m_oId.IsInit() == false) return;

	odt_context->start_bookmark(oox_bookmark_start->m_oId->GetValue(), *oox_bookmark_start->m_sName);
}
void DocxConverter::convert(OOX::Logic::CBookmarkEnd *oox_bookmark_end)
{
	if (!oox_bookmark_end) return;
	
	if (oox_bookmark_end->m_oId.IsInit() == false) return;

	odt_context->end_bookmark(oox_bookmark_end->m_oId->GetValue());
}

void DocxConverter::convert(OOX::Logic::CPTab *oox_ptab)
{
	if (oox_ptab == NULL) return;

	_CP_OPT(int) ref;

	odf_writer::style_paragraph_properties * paragraph_properties = odt_context->styles_context()->last_state(style_family::Paragraph)->get_paragraph_properties();;

	if (paragraph_properties)
	{
		_CP_OPT(int)				type;
		_CP_OPT(int)				leader;
		_CP_OPT(odf_types::length)	length;
		if (oox_ptab->m_oAlignment.IsInit())
		{
			switch(oox_ptab->m_oAlignment->GetValue())
			{
				case SimpleTypes::ptabalignmentCenter:
				{
					length = odf_types::length(odt_context->page_layout_context()->current_page_width_ / 2, length::pt);
					type = 1;	
				}break;					
				case SimpleTypes::ptabalignmentRight:
				{
					length = odf_types::length(odt_context->page_layout_context()->current_page_width_, length::pt);
					type = 4;	
				}break;
				case SimpleTypes::ptabalignmentLeft:	
				default:
					type = 6;	break;//??
			}
		}
		if (oox_ptab->m_oLeader.IsInit())
		{
			switch(oox_ptab->m_oLeader->GetValue())
			{
				case SimpleTypes::ptableaderHyphen:			leader = 2;	break;
				case SimpleTypes::ptableaderMiddleDot:		leader = 3;	break;
				case SimpleTypes::ptableaderNone:			leader = 4;	break;
				case SimpleTypes::ptableaderUnderscore:		leader = 5;	break;
				case SimpleTypes::ptableaderDot:
				default:
															leader = 0;	break;
			}
		}

		//todooo m_oRelativeTo
		paragraph_properties->add_child_element(odf_context()->start_tabs());
			odt_context->add_tab(type, length, leader);
		odf_context()->end_tabs();
		
		odt_context->add_tab(type, length, leader);

		ref = 0;
	}

	odt_context->text_context()->add_tab(ref);
}
void DocxConverter::convert(OOX::Logic::CSym *oox_sym)
{
	if (oox_sym == NULL) return;
	if (oox_sym->m_oChar.IsInit() == false) return;
	
	if (oox_sym->m_oFont.IsInit())
		odt_context->text_context()->set_symbol_font(*oox_sym->m_oFont);
		
	odt_context->text_context()->set_symbol_text(oox_sym->m_oChar->GetValue());
	//odt_context->text_context()->add_text_content(std::wstring(L"/") + oox_sym->m_oChar->ToString());
}
void DocxConverter::convert(OOX::Logic::CFldChar *oox_fld)
{
	if (oox_fld == NULL) return;

		//nullable<SimpleTypes::COnOff<>       > m_oDirty;
		//nullable<SimpleTypes::COnOff<>       > m_oFldLock;
		//nullable<OOX::Logic::CFFData         > m_oFFData;
	if (oox_fld->m_oFldCharType.IsInit())
	{
		if (oox_fld->m_oFldCharType->GetValue() == SimpleTypes::fldchartypeBegin)	odt_context->start_field(false);
		if (oox_fld->m_oFldCharType->GetValue() == SimpleTypes::fldchartypeEnd)		odt_context->end_field();
		if (oox_fld->m_oFldCharType->GetValue() == SimpleTypes::fldchartypeSeparate)odt_context->separate_field();
	}

}
void DocxConverter::convert(OOX::Logic::CFldSimple	*oox_fld)
{
	if (oox_fld == NULL) return;

	//SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oDirty;
	//SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oFldLock;

	odt_context->start_field(true);
	{	
		if (oox_fld->m_sInstr.IsInit())	
			odt_context->add_field_instr(oox_fld->m_sInstr.get2());
		
		odt_context->separate_field();

        for (size_t i = 0; i < oox_fld->m_arrItems.size(); ++i)
		{
			convert(oox_fld->m_arrItems[i]);
		}
	}
	odt_context->end_field();
}
void DocxConverter::convert(OOX::Logic::CInstrText *oox_instrText)
{
	if (oox_instrText == NULL) return;

	odt_context->add_field_instr(oox_instrText->m_sText);

}
void DocxConverter::convert(OOX::Logic::CDelText *oox_delText)
{
	if (oox_delText == NULL) return;

	if (oox_delText->m_oSpace.IsInit())
	{
		if (oox_delText->m_oSpace->GetValue() == SimpleTypes::xmlspacePreserve)
		{
		}
	}
	odt_context->add_text_content(oox_delText->m_sText);
}
void DocxConverter::convert(OOX::Logic::CIns *oox_ins)
{
	if (oox_ins == NULL) return;
	
	std::wstring	author	= oox_ins->m_sAuthor.IsInit()	? oox_ins->m_sAuthor.get2()		: L"";
	std::wstring	userId	= oox_ins->m_sUserId.IsInit()	? oox_ins->m_sUserId.get2()		: L"";
	int				id		= oox_ins->m_oId.IsInit()		? oox_ins->m_oId->GetValue()	: -1;
	std::wstring	date	= oox_ins->m_oDate.IsInit()		? oox_ins->m_oDate->GetValue()	: L"";
			
	bool start_change = odt_context->start_change(id, 1, author, userId, date);

    for (size_t i = 0; i < oox_ins->m_arrItems.size(); ++i)
	{
		convert(oox_ins->m_arrItems[i]);
	}
	
	if (start_change)
		odt_context->end_change(id, 1);
}
int DocxConverter::convert(ComplexTypes::Word::CTrackChange *oox_change, int type)
{
	if (!oox_change) return -1;

	std::wstring	author	= oox_change->m_sAuthor.IsInit()	? oox_change->m_sAuthor.get2()		: L"";
	std::wstring	userId	= oox_change->m_sUserId.IsInit()	? oox_change->m_sUserId.get2()		: L"";
	int				id		= oox_change->m_oId.IsInit()		? oox_change->m_oId->GetValue()		: -1;
	std::wstring	date	= oox_change->m_oDate.IsInit()		? oox_change->m_oDate->GetValue()	: L"";

	if (!odt_context->start_change(id, type, author, userId, date)) return -1;

	return id;
}
int DocxConverter::convert(OOX::Logic::CSectPrChange *oox_sect_prop_change)
{
	if (!oox_sect_prop_change) return -1;

	std::wstring	author	= oox_sect_prop_change->m_sAuthor.IsInit()	? oox_sect_prop_change->m_sAuthor.get2()		: L"";
	std::wstring	userId	= oox_sect_prop_change->m_sUserId.IsInit()	? oox_sect_prop_change->m_sUserId.get2()		: L"";
	int				id		= oox_sect_prop_change->m_oId.IsInit()		? oox_sect_prop_change->m_oId->GetValue()		: -1;
	std::wstring	date	= oox_sect_prop_change->m_oDate.IsInit()	? oox_sect_prop_change->m_oDate->GetValue()		: L"";

	if (odt_context->start_change(id, 3, author, userId, date))
	{
		convert(oox_sect_prop_change->m_pSecPr.GetPointer());

		return id;
	}
	return -1;
}
int DocxConverter::convert(OOX::Logic::CPPrChange *oox_para_prop_change)
{
	if (!oox_para_prop_change) return -1;

	std::wstring	author	= oox_para_prop_change->m_sAuthor.IsInit()	? oox_para_prop_change->m_sAuthor.get2()		: L"";
	std::wstring	userId	= oox_para_prop_change->m_sUserId.IsInit()	? oox_para_prop_change->m_sUserId.get2()		: L"";
	int				id		= oox_para_prop_change->m_oId.IsInit()		? oox_para_prop_change->m_oId->GetValue()		: -1;
	std::wstring	date	= oox_para_prop_change->m_oDate.IsInit()	? oox_para_prop_change->m_oDate->GetValue()		: L"";
	std::wstring	style_name;

	if (oox_para_prop_change->m_pParPr.IsInit())
	{
		bool		 list_present = oox_para_prop_change->m_pParPr->m_oNumPr.IsInit();
		std::wstring list_style_name;

		int			 list_level = -1;
		int			 list_style_id = -1;

		odf_writer::style_text_properties		* text_properties		= NULL;
		odf_writer::style_paragraph_properties	* paragraph_properties	= NULL;

		bool bRunPara = oox_para_prop_change->m_pParPr->m_oRPr.IsInit() ? true : false;
		if (list_present)
		{
			if (oox_para_prop_change->m_pParPr->m_oNumPr->m_oIlvl.IsInit() && oox_para_prop_change->m_pParPr->m_oNumPr->m_oIlvl->m_oVal.IsInit())
				list_level = *oox_para_prop_change->m_pParPr->m_oNumPr->m_oIlvl->m_oVal;		
			
			if (oox_para_prop_change->m_pParPr->m_oNumPr->m_oNumID.IsInit() && oox_para_prop_change->m_pParPr->m_oNumPr->m_oNumID->m_oVal.IsInit())
				list_style_id = *oox_para_prop_change->m_pParPr->m_oNumPr->m_oNumID->m_oVal;
		}		
		
		odt_context->styles_context()->create_style(L"", odf_types::style_family::Paragraph, true, false, -1);					
		
		paragraph_properties	= odt_context->styles_context()->last_state()->get_paragraph_properties();
		if (bRunPara)
			text_properties		= odt_context->styles_context()->last_state()->get_text_properties();
		
		if (list_present && list_style_id >= 0)
		{
			list_style_name = odt_context->styles_context()->lists_styles().get_style_name(list_style_id); 
			odt_context->styles_context()->last_state()->set_list_style_name(list_style_name);
		}
		convert(oox_para_prop_change->m_pParPr.GetPointer(), paragraph_properties);

		if (bRunPara)
			convert(oox_para_prop_change->m_pParPr->m_oRPr.GetPointer(), text_properties);
		
		odf_writer::odf_style_state_ptr style_state = odt_context->styles_context()->last_state(style_family::Paragraph);
		if (style_state)
			style_name	= style_state->get_name();
	}

	if (!odt_context->start_change(id, 3, author, userId, date, style_name)) return -1;
	return id;
}
int DocxConverter::convert(OOX::Logic::CRPrChange *oox_run_prop_change)
{
	if (!oox_run_prop_change) return -1;
		
	std::wstring	author	= oox_run_prop_change->m_sAuthor.IsInit()	? oox_run_prop_change->m_sAuthor.get2()		: L"";
	std::wstring	userId	= oox_run_prop_change->m_sUserId.IsInit()	? oox_run_prop_change->m_sUserId.get2()		: L"";
	int				id		= oox_run_prop_change->m_oId.IsInit()		? oox_run_prop_change->m_oId->GetValue()	: -1;
	std::wstring	date	= oox_run_prop_change->m_oDate.IsInit()		? oox_run_prop_change->m_oDate->GetValue()	: L"";
	std::wstring	style_name;

	if (oox_run_prop_change->m_pRunPr.IsInit())
	{
		odt_context->styles_context()->create_style(L"", odf_types::style_family::Text, true, false, -1);					
		odf_writer::style_text_properties	* text_properties = odt_context->styles_context()->last_state()->get_text_properties();

		convert(oox_run_prop_change->m_pRunPr.GetPointer(), text_properties);
	
		odf_writer::odf_style_state_ptr style_state = odt_context->styles_context()->last_state(style_family::Text);
		if (style_state)
			style_name	= style_state->get_name();
	}

	if (!odt_context->start_change(id, 3, author, userId, date, style_name)) return -1;
	return id;
}
int DocxConverter::convert(OOX::Logic::CTrPrChange *oox_tr_prop_change)
{
	if (!oox_tr_prop_change) return -1;
	
	std::wstring	author	= oox_tr_prop_change->m_sAuthor.IsInit()	? oox_tr_prop_change->m_sAuthor.get2()		: L"";
	std::wstring	userId	= oox_tr_prop_change->m_sUserId.IsInit()	? oox_tr_prop_change->m_sUserId.get2()		: L"";
	int				id		= oox_tr_prop_change->m_oId.IsInit()		? oox_tr_prop_change->m_oId->GetValue()		: -1;
	std::wstring	date	= oox_tr_prop_change->m_oDate.IsInit()		? oox_tr_prop_change->m_oDate->GetValue()	: L"";
	std::wstring	style_name;

	odf_writer::odf_style_state_ptr style_state = odt_context->styles_context()->last_state(style_family::Text);
	if (style_state)
		style_name	= style_state->get_name();

	if (odt_context->start_change(id, 3, author, userId, date))
	{
		convert(oox_tr_prop_change->m_pTrPr.GetPointer());
		//odt_context->end_change(id, 3); в конце row
		return id;
	}
	if (!odt_context->start_change(id, 3, author, userId, date, style_name)) return -1;
	return id;	
}
int DocxConverter::convert(OOX::Logic::CTcPrChange *oox_tc_prop_change)
{
	if (!oox_tc_prop_change) return -1;

	std::wstring	author	= oox_tc_prop_change->m_sAuthor.IsInit()	? oox_tc_prop_change->m_sAuthor.get2()		: L"";
	std::wstring	userId	= oox_tc_prop_change->m_sUserId.IsInit()	? oox_tc_prop_change->m_sUserId.get2()		: L"";
	int				id		= oox_tc_prop_change->m_oId.IsInit()		? oox_tc_prop_change->m_oId->GetValue()		: -1;
	std::wstring	date	= oox_tc_prop_change->m_oDate.IsInit()		? oox_tc_prop_change->m_oDate->GetValue()	: L"";
	std::wstring	style_name;

	if (oox_tc_prop_change->m_pTcPr.IsInit())
	{
		convert(oox_tc_prop_change->m_pTcPr.GetPointer());

		odf_writer::odf_style_state_ptr style_state = odt_context->styles_context()->last_state(style_family::TableCell);
		if (style_state)
			style_name	= style_state->get_name();
	}
	if (!odt_context->start_change(id, 3, author, userId, date, style_name)) return -1;
	return id;	

}
int DocxConverter::convert(OOX::Logic::CTblPrChange *oox_table_prop_change)
{
	if (!oox_table_prop_change) return -1;
	
	std::wstring	author	= oox_table_prop_change->m_sAuthor.IsInit()	? oox_table_prop_change->m_sAuthor.get2()		: L"";
	std::wstring	userId	= oox_table_prop_change->m_sUserId.IsInit()	? oox_table_prop_change->m_sUserId.get2()		: L"";
	int				id		= oox_table_prop_change->m_oId.IsInit()		? oox_table_prop_change->m_oId->GetValue()		: -1;
	std::wstring	date	= oox_table_prop_change->m_oDate.IsInit()	? oox_table_prop_change->m_oDate->GetValue()	: L"";
	std::wstring	style_name;

	if (oox_table_prop_change->m_pTblPr.IsInit())
	{
		bool table_styled = false;
		if (oox_table_prop_change->m_pTblPr->m_oTblStyle.IsInit() && oox_table_prop_change->m_pTblPr->m_oTblStyle->m_sVal.IsInit())
		{//настройка предустановленного стиля

			std::wstring base_style_name = *oox_table_prop_change->m_pTblPr->m_oTblStyle->m_sVal;

			table_styled = odt_context->styles_context()->table_styles().start_table(base_style_name);
		}
		convert(oox_table_prop_change->m_pTblPr.GetPointer(), table_styled); 

		odf_writer::odf_style_state_ptr style_state = odt_context->styles_context()->last_state(style_family::Table);
		if (style_state)
			style_name	= style_state->get_name();
	}

	if (!odt_context->start_change(id, 3, author, userId, date, style_name)) return -1;
	return id;	

}
void DocxConverter::convert(OOX::Logic::CDel *oox_del)
{
	if (oox_del == NULL) return;

	std::wstring	author	= oox_del->m_sAuthor.IsInit()	? oox_del->m_sAuthor.get2()		: L"";
	std::wstring	userId	= oox_del->m_sUserId.IsInit()	? oox_del->m_sUserId.get2()		: L"";
	int				id		= oox_del->m_oId.IsInit()		? oox_del->m_oId->GetValue()	: -1;
	std::wstring	date	= oox_del->m_oDate.IsInit()		? oox_del->m_oDate->GetValue()	: L"";

	bool res_change  = odt_context->start_change(id, 2, author, userId, date);

    for (size_t i = 0; i < oox_del->m_arrItems.size(); ++i)
	{
		convert(oox_del->m_arrItems[i]);
	}
	if (res_change)
		odt_context->end_change(id, 2);
}
void DocxConverter::convert(OOX::Logic::CSmartTag *oox_tag)
{
	if (oox_tag == NULL) return;

    for (size_t i = 0; i < oox_tag->m_arrItems.size(); ++i)
	{
		convert(oox_tag->m_arrItems[i]);
	}
}
void DocxConverter::convert(OOX::Logic::CParagraphProperty	*oox_paragraph_pr, cpdoccore::odf_writer::style_paragraph_properties *paragraph_properties)
{
	odt_context->text_context()->set_KeepNextParagraph(false);
	
	if (!paragraph_properties)	return;
	if (!oox_paragraph_pr)		
	{
		if (current_section_properties)
		{
			convert(current_section_properties->props, !current_section_properties->bContinue);
		}
		return;
	}

	int outline_level = 0;

	if (oox_paragraph_pr->m_oPStyle.IsInit() && oox_paragraph_pr->m_oPStyle->m_sVal.IsInit())
	{
		std::wstring style_name = *oox_paragraph_pr->m_oPStyle->m_sVal;

		/////////////////////////find parent properties 
		odf_writer::style_paragraph_properties  parent_paragraph_properties;
		odt_context->styles_context()->calc_paragraph_properties(style_name, odf_types::style_family::Paragraph, &parent_paragraph_properties.content_);
		
		odf_writer::style_text_properties  parent_text_properties;
		odt_context->styles_context()->calc_text_properties(style_name, odf_types::style_family::Paragraph, &parent_text_properties.content_);

		odf_writer::odf_style_state_ptr style_state;
		style_state = odt_context->styles_context()->last_state();

		odf_writer::odf_drawing_context* drawing_context = odt_context->drawing_context();
		if ((drawing_context) && (false == drawing_context->is_current_empty()))
		{
			paragraph_properties->apply_from(&parent_paragraph_properties);
			
			odf_writer::style_text_properties* text_props = style_state->get_text_properties();
			text_props->apply_from(&parent_text_properties);
		}
		else
		{
			style_state->set_parent_style_name(style_name);
		}
		
		if (parent_paragraph_properties.content_.outline_level_)
		{
			outline_level = *parent_paragraph_properties.content_.outline_level_;
		}
		//список тож явно ??? угу :( - выше + велосипед для хранения
		if (parent_text_properties.content_.fo_font_size_)
		{
			current_font_size.push_back(parent_text_properties.content_.fo_font_size_->get_length().get_value_unit(odf_types::length::pt));
		}
	}

	if (oox_paragraph_pr->m_oSpacing.IsInit())
	{
		SimpleTypes::ELineSpacingRule rule = SimpleTypes::linespacingruleAtLeast;
		if (oox_paragraph_pr->m_oSpacing->m_oLineRule.IsInit())rule = oox_paragraph_pr->m_oSpacing->m_oLineRule->GetValue();

		if (oox_paragraph_pr->m_oSpacing->m_oLine.IsInit())
		{
			if (rule == SimpleTypes::linespacingruleExact)
			{
				_CP_OPT(odf_types::length) length_;
				
				convert(dynamic_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oSpacing->m_oLine.GetPointer()), length_);
				paragraph_properties->content_.fo_line_height_ = odf_types::line_width(*length_);
			}
			else if (rule == SimpleTypes::linespacingruleAtLeast)
			{
				convert(dynamic_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oSpacing->m_oLine.GetPointer()), 
					paragraph_properties->content_.style_line_height_at_least_);
			}
			else
			{
				double val = oox_paragraph_pr->m_oSpacing->m_oLine->ToPoints() * 20;
				odf_types::percent percent(val * 100. / 240);
				paragraph_properties->content_.fo_line_height_ = percent;
			}
		}
		if (oox_paragraph_pr->m_oSpacing->m_oAfter.IsInit())
		{
 			_CP_OPT(odf_types::length_or_percent) length;
			convert(dynamic_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oSpacing->m_oAfter.GetPointer()), length);
			paragraph_properties->content_.fo_margin_bottom_ = length;
		}
		if (oox_paragraph_pr->m_oSpacing->m_oBefore.IsInit())
		{
 			_CP_OPT(odf_types::length_or_percent) length;
			convert(dynamic_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oSpacing->m_oBefore.GetPointer()), length);
			paragraph_properties->content_.fo_margin_top_ = length;
		}
			//nullable<SimpleTypes::COnOff<>            > m_oAfterAutospacing;
			//nullable<SimpleTypes::CDecimalNumber<>    > m_oAfterLines;
			//nullable<SimpleTypes::COnOff<>            > m_oBeforeAutospacing;
			//nullable<SimpleTypes::CDecimalNumber<>    > m_oBeforeLines;
	}
	if (oox_paragraph_pr->m_oContextualSpacing.IsInit())
		paragraph_properties->content_.style_contextual_spacing_ = true;

	if (oox_paragraph_pr->m_oInd.IsInit())
	{
		if (oox_paragraph_pr->m_oInd->m_oStart.IsInit())
		{
 			_CP_OPT(odf_types::length) length;
			convert(dynamic_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oInd->m_oStart.GetPointer()), length);
			paragraph_properties->content_.fo_margin_left_ = odf_types::length(length->get_value_unit(odf_types::length::cm), odf_types::length::cm);
			//в случае списка тута надо выдумать indent (взять из стиля списка)
		}
		if (oox_paragraph_pr->m_oInd->m_oEnd.IsInit())
		{
 			_CP_OPT(odf_types::length) length;
			convert(dynamic_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oInd->m_oEnd.GetPointer()), length);
			paragraph_properties->content_.fo_margin_right_ = odf_types::length(length->get_value_unit(odf_types::length::cm), odf_types::length::cm);
		}
		if (oox_paragraph_pr->m_oInd->m_oFirstLine.IsInit())
		{
 			_CP_OPT(odf_types::length) length;
			convert(dynamic_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oInd->m_oFirstLine.GetPointer()), length);
			paragraph_properties->content_.fo_text_indent_ = odf_types::length(length->get_value_unit(odf_types::length::cm), odf_types::length::cm);
		}
		if (oox_paragraph_pr->m_oInd->m_oHanging.IsInit())
		{//first line ignored
 			_CP_OPT(odf_types::length) length;
			convert(dynamic_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oInd->m_oHanging.GetPointer()), length);
			if (length) paragraph_properties->content_.fo_text_indent_ = odf_types::length(-length->get_value_unit(odf_types::length::cm), odf_types::length::cm);
		}
			//nullable<SimpleTypes::CDecimalNumber<>    > m_oEndChars;
			//nullable<SimpleTypes::CDecimalNumber<>    > m_oFirstLineChars;
			//nullable<SimpleTypes::CDecimalNumber<>    > m_oHangingChars;
			//nullable<SimpleTypes::CDecimalNumber<>    > m_oStartChars;
	}
	//if (oox_paragraph_pr->m_oRtl.IsInit())
	//{
	//}	

	convert(oox_paragraph_pr->m_oJc.GetPointer(), paragraph_properties->content_.fo_text_align_);

	if (oox_paragraph_pr->m_oTextAlignment.IsInit() && oox_paragraph_pr->m_oTextAlignment->m_oVal.IsInit())
	{
		//switch(oox_paragraph_pr->m_oTextAlignment->m_oVal->GetValue())
		//{
		////case SimpleTypes::textalignAuto     :
		////case SimpleTypes::textalignBaseLine :
		////case SimpleTypes::textalignBottom   :
		////case SimpleTypes::textalignCenter   :
		////case SimpleTypes::textalignTop      :
		//}
	}

	//if (oox_paragraph_pr->m_oWordWrap.IsInit())	odt_context->set_word_wrap(oox_paragraph_pr->m_oWordWrap->ToBool());

	convert(oox_paragraph_pr->m_oPBdr.GetPointer(), paragraph_properties);

	if (odt_context->empty())
	{//Thesis.docx
		if (oox_paragraph_pr->m_oRPr.IsInit())
		{
			odf_writer::style_text_properties *text_properties = odf_context()->text_context()->get_text_properties();
			if (text_properties) 
				convert(oox_paragraph_pr->m_oRPr.GetPointer(), text_properties);
		}
	}
	if (oox_paragraph_pr->m_oShd.IsInit())
	{
		_CP_OPT(odf_types::color) odf_color;
		convert(oox_paragraph_pr->m_oShd.GetPointer(), odf_color);
		if (odf_color)
		{
			paragraph_properties->content_.fo_background_color_ = *odf_color;
		}
	}
	if (oox_paragraph_pr->m_oTextDirection.IsInit() && oox_paragraph_pr->m_oTextDirection->m_oVal.IsInit())
	{
		switch(oox_paragraph_pr->m_oTextDirection->m_oVal->GetValue())
		{
		case SimpleTypes::textdirectionLr   :
			paragraph_properties->content_.style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::Lr);		break;
		case SimpleTypes::textdirectionLrV  :
			paragraph_properties->content_.style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::LrTb);	break;
		case SimpleTypes::textdirectionRl   :
			paragraph_properties->content_.style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::Rl);		break;
		case SimpleTypes::textdirectionRlV  :
			paragraph_properties->content_.style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::RlTb);	break;
		case SimpleTypes::textdirectionTb   :
			paragraph_properties->content_.style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::Tb);		break;
		case SimpleTypes::textdirectionTbV  :
			paragraph_properties->content_.style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::TbLr);	break;
		}
	}
	if (oox_paragraph_pr->m_oOutlineLvl.IsInit())
	{
		outline_level = 0;
		if (oox_paragraph_pr->m_oOutlineLvl->m_oVal.IsInit())
			outline_level = *oox_paragraph_pr->m_oOutlineLvl->m_oVal;
		if (outline_level > 0) paragraph_properties->content_.outline_level_ =  outline_level /*+1*/;
		odt_context->text_context()->set_outline_level (outline_level);
	}
	if (oox_paragraph_pr->m_oPageBreakBefore.IsInit() && oox_paragraph_pr->m_oPageBreakBefore->m_oVal.ToBool())
	{
		paragraph_properties->content_.fo_break_before_ = odf_types::fo_break(odf_types::fo_break::Page);
		odt_context->text_context()->set_type_break(1, 0); //page, clear_all
	}

	if (oox_paragraph_pr->m_oKeepNext.IsInit() && odt_context->table_context()->empty() && !current_section_properties)
	{
		odt_context->text_context()->set_KeepNextParagraph(true);
	}

	convert(oox_paragraph_pr->m_oFramePr.GetPointer(), paragraph_properties);		//буквица или фрейм

	if (current_section_properties) 
	{
		if ((current_section_properties->props) && (current_section_properties->props->m_oPgNumType.IsInit()) && (current_section_properties->props->m_oPgNumType->m_oStart.IsInit()))
		{
			paragraph_properties->content_.style_page_number_ = current_section_properties->props->m_oPgNumType->m_oStart->GetValue();
		}
		convert(current_section_properties->props, !current_section_properties->bContinue);
	
		//odf_writer::odf_style_state_ptr state =  odt_context->styles_context()->last_state(odf_types::style_family::Paragraph);
		//if (odt_context->is_paragraph_in_current_section_ && state)
		//{
		//	odf_writer::style *style_ = dynamic_cast<odf_writer::style*>(state->get_office_element().get());

		//	if (style_)
		//	{
		//		odt_context->is_paragraph_in_current_section_	= false;
		//		style_->style_master_page_name_		= odt_context->page_layout_context()->last_master() ?
		//						  odt_context->page_layout_context()->last_master()->get_name() : L"";
		//	}
		//}

	}

	if (odt_context->notes_context()->is_started()	&& !paragraph_properties->content_.fo_margin_left_ 
													&& !paragraph_properties->content_.fo_text_indent_)
	{
		paragraph_properties->content_.fo_margin_left_			= odf_types::length( 0.2, odf_types::length::cm);	
		paragraph_properties->content_.fo_text_indent_			= odf_types::length(-0.2, odf_types::length::cm);
		paragraph_properties->content_.fo_line_height_			= odf_types::percent(100.); 
		paragraph_properties->content_.style_auto_text_indent_	= false;
	}

	if (oox_paragraph_pr->m_oTabs.IsInit())
	{
		paragraph_properties->add_child_element(odf_context()->start_tabs());
		for (size_t i = 0; i < oox_paragraph_pr->m_oTabs->m_arrTabs.size(); i++)
		{
			if (oox_paragraph_pr->m_oTabs->m_arrTabs[i] == NULL) continue;
			
			_CP_OPT(int)				type;
			_CP_OPT(int)				leader;
			_CP_OPT(odf_types::length)	length;

			if (oox_paragraph_pr->m_oTabs->m_arrTabs[i]->m_oVal.IsInit())
				type = oox_paragraph_pr->m_oTabs->m_arrTabs[i]->m_oVal->GetValue();
			if (type == 2) continue; //clear tab

			if (oox_paragraph_pr->m_oTabs->m_arrTabs[i]->m_oLeader.IsInit())
				leader = oox_paragraph_pr->m_oTabs->m_arrTabs[i]->m_oLeader->GetValue();

			convert(oox_paragraph_pr->m_oTabs->m_arrTabs[i]->m_oPos.GetPointer(), length );

			odf_context()->add_tab(type, length, leader);
		}
		odf_context()->end_tabs();
	}
}

void DocxConverter::convert( ComplexTypes::Word::CShading* shading, _CP_OPT(odf_types::color)& odf_color)
{	
	if (!shading) return;	

	if ((shading->m_oVal.IsInit()) && (shading->m_oVal->GetValue() == SimpleTypes::shdNil)) return;
	
	bool bColor = ((shading->m_oFill.IsInit()) && (shading->m_oFill->GetValue() == SimpleTypes::hexcolorRGB) )
		|| ((shading->m_oVal.IsInit()) && shading->m_oColor.IsInit() &&
			(shading->m_oVal->GetValue() != SimpleTypes::shdClear));

	bool bThemeColor = ((shading->m_oThemeFill.IsInit()) && (shading->m_oThemeFill->GetValue() == SimpleTypes::hexcolorRGB))
		|| ((shading->m_oVal.IsInit()) && shading->m_oThemeColor.IsInit() &&
			(shading->m_oVal->GetValue() != SimpleTypes::shdClear));

	if (!bThemeColor && !bColor) return;

	double kf = 0;
	if (shading->m_oVal.IsInit())
	{
		switch (shading->m_oVal->GetValue())
		{
		case SimpleTypes::shdPct10: kf = 0.10; break;
		case SimpleTypes::shdPct12: kf = 0.12; break;
		case SimpleTypes::shdPct15: kf = 0.15; break;
		case SimpleTypes::shdPct20: kf = 0.20; break;
		case SimpleTypes::shdPct25: kf = 0.25; break;
		case SimpleTypes::shdPct30: kf = 0.30; break;
		case SimpleTypes::shdPct35: kf = 0.35; break;
		case SimpleTypes::shdPct37: kf = 0.37; break;
		case SimpleTypes::shdPct40: kf = 0.40; break;
		case SimpleTypes::shdPct45: kf = 0.45; break;
		case SimpleTypes::shdPct5: kf = 0.05; break;
		case SimpleTypes::shdPct50: kf = 0.50; break;
		case SimpleTypes::shdPct55: kf = 0.55; break;
		case SimpleTypes::shdPct60: kf = 0.60; break;
		case SimpleTypes::shdPct62: kf = 0.62; break;
		case SimpleTypes::shdPct65: kf = 0.65; break;
		case SimpleTypes::shdPct70: kf = 0.70; break;
		case SimpleTypes::shdPct75: kf = 0.75; break;
		case SimpleTypes::shdPct80: kf = 0.80; break;
		case SimpleTypes::shdPct85: kf = 0.85; break;
		case SimpleTypes::shdPct87: kf = 0.87; break;
		case SimpleTypes::shdPct90: kf = 0.90; break;
		case SimpleTypes::shdPct95: kf = 0.95; break;
		default:
			break;
		}
	}
	convert(NULL, shading->m_oThemeFill.GetPointer(),
		shading->m_oThemeFillTint.GetPointer(), shading->m_oThemeFillShade.GetPointer(), odf_color);
		
	if (odf_color) return;

	convert(NULL, shading->m_oThemeColor.GetPointer(), 
		shading->m_oThemeTint.GetPointer(), shading->m_oThemeShade.GetPointer(), odf_color);

	if (odf_color) return;

	if (bColor)
	{
		BYTE ucR = 0xff, ucB = 0xff, ucG = 0xff;  

		if (shading->m_oFill.IsInit())
		{
			ucR = shading->m_oFill->Get_R();
			ucB = shading->m_oFill->Get_B();
			ucG = shading->m_oFill->Get_G();
			
			ucR = (BYTE)(ucR * (1 - kf)); ucB = (BYTE)(ucB * (1 - kf)); ucG = (BYTE)(ucG * (1 - kf));
		}
		else if (shading->m_oColor->GetValue() == SimpleTypes::hexcolorRGB)
		{
			ucR = shading->m_oColor->Get_R(); 
			ucB = shading->m_oColor->Get_B(); 
			ucG = shading->m_oColor->Get_G(); 
			
			ucR = (BYTE)(ucR * (kf)); ucB = (BYTE)(ucB * (kf)); ucG = (BYTE)(ucG * (kf));
		}
		SimpleTypes::CHexColor *oRgbColor = new SimpleTypes::CHexColor(ucR, ucG, ucB);

		if (oRgbColor)
		{
			std::wstring strColor = L"#" + oRgbColor->ToString().substr(2);//.Right(6);
			odf_color = odf_types::color(strColor);
			delete oRgbColor;
		}
	}
}
void DocxConverter::apply_HF_from(OOX::Logic::CSectionProperty *props, OOX::Logic::CSectionProperty *other)
{
	if (props == NULL || other== NULL)return;

	if (other->m_arrFooterReference.size() > 0)
	{
		for (size_t i =0 ; i < props->m_arrFooterReference.size() ; i++)
		{
			if (props->m_arrFooterReference[i]) delete props->m_arrFooterReference[i];
			props->m_arrFooterReference[i] = NULL;
		}
		props->m_arrFooterReference.clear();

		for (size_t i =0 ; i < other->m_arrFooterReference.size() ; i++)
		{
			ComplexTypes::Word::CHdrFtrRef* ref = new ComplexTypes::Word::CHdrFtrRef();

			ref->m_oId= other->m_arrFooterReference[i]->m_oId;
			ref->m_oType= other->m_arrFooterReference[i]->m_oType;

			props->m_arrFooterReference.push_back(ref);
		}
		
	}
	if (other->m_arrHeaderReference.size() > 0)
	{
		for (size_t i =0 ; i < props->m_arrHeaderReference.size() ; i++)
		{
			if (props->m_arrHeaderReference[i]) delete props->m_arrHeaderReference[i];
			props->m_arrHeaderReference[i] = NULL;
		}
		props->m_arrHeaderReference.clear();
		
		for (size_t i =0 ; i < other->m_arrHeaderReference.size() ; i++)
		{
			ComplexTypes::Word::CHdrFtrRef* ref = new ComplexTypes::Word::CHdrFtrRef();
			
			ref->m_oId= other->m_arrHeaderReference[i]->m_oId;
			ref->m_oType= other->m_arrHeaderReference[i]->m_oType;

			props->m_arrHeaderReference.push_back(ref);
		}
	}
}
void DocxConverter::convert(OOX::Logic::CSectionProperty *oox_section_pr, bool bSection, const std::wstring & master_name, bool bAlways)
{
	if (oox_section_pr == NULL) return;
	current_section_properties = NULL;

	odt_context->text_context()->set_type_break(-1, 0);
 
	bool continuous = false;

	if (oox_section_pr->m_oType.IsInit() && oox_section_pr->m_oType->m_oVal.IsInit())
	{
		switch(oox_section_pr->m_oType->m_oVal->GetValue())
		{		
		case SimpleTypes::sectionmarkContinious :
			continuous = true; 
			break;
		case SimpleTypes::sectionmarkNextColumn :
		case SimpleTypes::sectionmarkEvenPage   :
		case SimpleTypes::sectionmarkNextPage   :
		case SimpleTypes::sectionmarkOddPage    :
			// возможен разрыв
			break;
		}
	}
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool bDefault = (master_name == L"Standard");
	
	if (!bDefault)
	{
		if (!last_section_properties && (!bSection || continuous == false || oox_section_pr->m_oTitlePg.IsInit()))
		{	
			last_section_properties = oox_section_pr;
		}
		else if (!bSection || continuous == false)
		{
			apply_HF_from(last_section_properties, oox_section_pr);
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!bSection || continuous == false)
	{
		odt_context->page_layout_context()->add_master_page(master_name);
	}

	if (oox_section_pr->m_oPgMar.IsInit())
	{
		_CP_OPT(odf_types::length) top, left, right, bottom, header, footer, gutter, header_min, footer_min;

		convert(oox_section_pr->m_oPgMar->m_oBottom.GetPointer(),	bottom);
		convert(oox_section_pr->m_oPgMar->m_oLeft.GetPointer(),		left);
		convert(oox_section_pr->m_oPgMar->m_oRight.GetPointer(),	right);
		convert(oox_section_pr->m_oPgMar->m_oTop.GetPointer(),		top);
		convert(oox_section_pr->m_oPgMar->m_oHeader.GetPointer(),	header);
		convert(oox_section_pr->m_oPgMar->m_oFooter.GetPointer(),	footer);
		convert(oox_section_pr->m_oPgMar->m_oGutter.GetPointer(),	gutter);
		
		footer_min = footer;
		header_min = header;

		if (bottom)
		{
			double bottom_cm = bottom->get_value_unit(length::cm);
			double footer_cm = footer ? footer->get_value_unit(length::cm) : 0;
			double length_cm = bottom_cm - footer_cm;
		
			if ( length_cm > 2.4 )
			{
				bottom = footer;
				footer = length(fabs(length_cm), length::cm);
			}
			else if ( length_cm < 0 )
			{
				footer_min = length(-length_cm, length::cm);
				footer.reset();
			}
		}
		else
		{
			bottom = footer;
		}
		if (top)
		{
			double length_cm = top->get_value_unit(length::cm) - (header ? header->get_value_unit(length::cm) : 0);
		
			if ( length_cm > 2.4 )
			{
				top = header;
				header = length(fabs(length_cm), length::cm);
			}
			else if ( length_cm < 0 )
			{
				header_min = length(-length_cm, length::cm);
				header.reset();
			}
		}
		else
		{
			top = header;
		}
		odt_context->page_layout_context()->set_page_margin(top, left, bottom, right);
		odt_context->page_layout_context()->set_page_gutter(gutter);
		odt_context->page_layout_context()->set_header_size(header, header_min);
		odt_context->page_layout_context()->set_footer_size(footer, footer_min);
	}
	if (oox_section_pr->m_oPgBorders.IsInit())
	{
		std::wstring top, left, right, bottom;

		convert(oox_section_pr->m_oPgBorders->m_oBottom.GetPointer(), bottom);
		convert(oox_section_pr->m_oPgBorders->m_oLeft.GetPointer()	, left);
		convert(oox_section_pr->m_oPgBorders->m_oRight.GetPointer()	, right);
		convert(oox_section_pr->m_oPgBorders->m_oTop.GetPointer()	, top);
		
		odt_context->page_layout_context()->set_page_border(top, left, bottom, right);

		
		if (oox_section_pr->m_oPgBorders->m_oOffsetFrom.IsInit() && 
			(oox_section_pr->m_oPgBorders->m_oOffsetFrom->GetValue() == SimpleTypes::pageborderoffsetPage))
		{
			odt_context->page_layout_context()->set_page_border_offset(2);

		}

		if (oox_section_pr->m_oPgBorders->m_oBottom.IsInit())
		{
			int type = (oox_section_pr->m_oPgBorders->m_oBottom->m_oVal.IsInit() ? oox_section_pr->m_oPgBorders->m_oBottom->m_oVal->GetValue() : SimpleTypes::bordervalueSingle);

			if (oox_section_pr->m_oPgBorders->m_oBottom->m_oSpace.IsInit())
				odt_context->page_layout_context()->set_page_border_padding(2, oox_section_pr->m_oPgBorders->m_oBottom->m_oSpace->ToPoints());
		}
		if (oox_section_pr->m_oPgBorders->m_oTop.IsInit())
		{
			int type = (oox_section_pr->m_oPgBorders->m_oBottom->m_oVal.IsInit() ? oox_section_pr->m_oPgBorders->m_oTop->m_oVal->GetValue() : SimpleTypes::bordervalueSingle);

			if (oox_section_pr->m_oPgBorders->m_oTop->m_oSpace.IsInit())
				odt_context->page_layout_context()->set_page_border_padding(1, oox_section_pr->m_oPgBorders->m_oTop->m_oSpace->ToPoints());
		}
		if (oox_section_pr->m_oPgBorders->m_oLeft.IsInit())
		{
			int type = (oox_section_pr->m_oPgBorders->m_oBottom->m_oVal.IsInit() ? oox_section_pr->m_oPgBorders->m_oLeft->m_oVal->GetValue() : SimpleTypes::bordervalueSingle);
			
			if (oox_section_pr->m_oPgBorders->m_oLeft->m_oSpace.IsInit())
				odt_context->page_layout_context()->set_page_border_padding(3, oox_section_pr->m_oPgBorders->m_oLeft->m_oSpace->ToPoints());
		}
		if (oox_section_pr->m_oPgBorders->m_oRight.IsInit())
		{
			int type = (oox_section_pr->m_oPgBorders->m_oBottom->m_oVal.IsInit() ? oox_section_pr->m_oPgBorders->m_oRight->m_oVal->GetValue() : SimpleTypes::bordervalueSingle);
		
			if (oox_section_pr->m_oPgBorders->m_oRight->m_oSpace.IsInit())
				odt_context->page_layout_context()->set_page_border_padding(4, oox_section_pr->m_oPgBorders->m_oRight->m_oSpace->ToPoints());
		}


		bool shadow = false;
		if (oox_section_pr->m_oPgBorders->m_oBottom.IsInit() && oox_section_pr->m_oPgBorders->m_oBottom->m_oShadow.IsInit() && oox_section_pr->m_oPgBorders->m_oBottom->m_oShadow->ToBool()) shadow = true;
		if (oox_section_pr->m_oPgBorders->m_oTop.IsInit() && oox_section_pr->m_oPgBorders->m_oTop->m_oShadow.IsInit() && oox_section_pr->m_oPgBorders->m_oTop->m_oShadow->ToBool()) shadow = true;
		if (oox_section_pr->m_oPgBorders->m_oLeft.IsInit() && oox_section_pr->m_oPgBorders->m_oLeft->m_oShadow.IsInit() && oox_section_pr->m_oPgBorders->m_oLeft->m_oShadow->ToBool()) shadow = true;
		if (oox_section_pr->m_oPgBorders->m_oRight.IsInit() && oox_section_pr->m_oPgBorders->m_oRight->m_oShadow.IsInit() && oox_section_pr->m_oPgBorders->m_oRight->m_oShadow->ToBool()) shadow = true;
		
		if (shadow) odt_context->page_layout_context()->set_page_border_shadow(true);
		
		if (oox_section_pr->m_oPgBorders->m_oDisplay.IsInit())
		{
			// todooo
			//pageborderdisplayAllPages     = 0,
			//pageborderdisplayFirstPage    = 1,
			//pageborderdisplayNotFirstPage = 2		
			//нужно разделить + сделать новые (без этих свойств) стили страниц - действительно для раздела 
		}
		//nullable<SimpleTypes::CPageBorderOffset<>  > m_oOffsetFrom;
		//nullable<SimpleTypes::CPageBorderZOrder<>  > m_oZOrder;

	}
	if (oox_section_pr->m_oPgSz.IsInit())
	{
		if (oox_section_pr->m_oPgSz->m_oOrient.IsInit())
			odt_context->page_layout_context()->set_page_orientation(oox_section_pr->m_oPgSz->m_oOrient->GetValue());

		_CP_OPT(odf_types::length) width, height;

		convert(oox_section_pr->m_oPgSz->m_oW.GetPointer(), width);
		convert(oox_section_pr->m_oPgSz->m_oH.GetPointer(), height);

		odt_context->page_layout_context()->set_page_size(width, height);

		//nullable<SimpleTypes::CDecimalNumber<>   > m_oCode;
	}
	if (docx_document)
	{
		convert(docx_document->m_oMain.document->m_oBackground.GetPointer(), 1);//подложка - вот в таком она месте :(, причём одна на все разделы, не как в оо
	}
	else if (docx_flat_document)
	{
		convert(docx_flat_document->m_pBgPict.GetPointer(), 1);
	}
			//nullable<ComplexTypes::Word::CTextDirection                  > m_oTextDirection;
            //nullable<ComplexTypes::Word::COnOff2 > m_oRtlGutter;
			//nullable<ComplexTypes::Word::CVerticalJc                     > m_oVAlign;

	if (oox_section_pr->m_oPgNumType.IsInit())
	{
		_CP_OPT(int) format;
		_CP_OPT(int) start;

		if (oox_section_pr->m_oPgNumType->m_oFmt.IsInit())		format = oox_section_pr->m_oPgNumType->m_oFmt->GetValue();
		if (oox_section_pr->m_oPgNumType->m_oStart.IsInit())	start  = oox_section_pr->m_oPgNumType->m_oStart->GetValue();

		odt_context->page_layout_context()->set_page_number_format(	format, start);
			//nullable<SimpleTypes::CChapterSep<>    > m_oChapSep;
			//nullable<SimpleTypes::CDecimalNumber<> > m_oChapStyle;	
	}
	
	if (continuous == false || oox_section_pr->m_oTitlePg.IsInit() || bAlways)
	{
		OOX::Logic::CSectionProperty*	s = last_section_properties ? last_section_properties : oox_section_pr; 
		
		bool present_title_page			= s->m_oTitlePg.IsInit() ? true : false;
		bool present_odd_even_pages		= odt_context->page_layout_context()->even_and_left_headers_;
		
		bool add_title_header			= false, add_title_footer			= false;
		bool add_odd_even_pages_header	= false, add_odd_even_pages_footer	= false;
		bool add_default_header			= false, add_default_footer			= false;

		std::vector<int> types;

		for (size_t i = 0; i < s->m_arrHeaderReference.size(); i++)
		{
			if (s->m_arrHeaderReference[i] == NULL) continue;

			int type = s->m_arrHeaderReference[i]->m_oType.IsInit() ? s->m_arrHeaderReference[i]->m_oType->GetValue() : 0 ;

			if (type == 2 && !present_title_page)		continue;
			if (type == 1 && !present_odd_even_pages)	continue;

			if (type == 2 && present_title_page)		add_title_header			= true;
			if (type == 1 && present_odd_even_pages)	add_odd_even_pages_header	= true; //swap even & odd ?
			if (type == 0)								add_default_header			= true;

			if (odt_context->start_header(type))
			{
				if (s->m_arrHeaderReference[i]->m_oId.IsInit())
				{
					convert_hdr_ftr(s->m_arrHeaderReference[i]->m_oId->GetValue());
					if (docx_document)
					{
						convert(docx_document->m_oMain.document->m_oBackground.GetPointer(), 2);
					}
				}

				odt_context->end_header_footer();
			}
		}
		if (!add_title_header			&& present_title_page)								odt_context->add_empty_header(2);
		if (!add_odd_even_pages_header	&& present_odd_even_pages)							odt_context->add_empty_header(1);
		if (!add_default_header			&& (present_odd_even_pages || present_title_page))	odt_context->add_empty_header(0);

		for (size_t i=0; i< s->m_arrFooterReference.size(); i++)
		{
			if (s->m_arrFooterReference[i] == NULL) continue;

			int type = s->m_arrFooterReference[i]->m_oType.IsInit() ? s->m_arrFooterReference[i]->m_oType->GetValue() :0 ;

			if (type == 2 && !present_title_page)		continue;
			if (type == 1 && !present_odd_even_pages)	continue;

			if (type == 2 && present_title_page)		add_title_footer			= true;
			if (type == 1 && present_odd_even_pages)	add_odd_even_pages_footer	= true;
			if (type == 0)								add_default_footer			= true;

			if (odt_context->start_footer(type))
			{
				if (s->m_arrFooterReference[i]->m_oId.IsInit())
				{
					convert_hdr_ftr(s->m_arrFooterReference[i]->m_oId->GetValue());
					if (docx_document)
					{
						convert(docx_document->m_oMain.document->m_oBackground.GetPointer(), 3);
					}
				}

				odt_context->end_header_footer();	
			}
		}
		if (!add_title_footer			&& present_title_page)								odt_context->add_empty_footer(2);
		if (!add_odd_even_pages_footer	&& present_odd_even_pages)							odt_context->add_empty_footer(1);
		if (!add_default_footer			&& (present_odd_even_pages || present_title_page))	odt_context->add_empty_footer(0);

		if (!bDefault)
			odt_context->is_paragraph_in_current_section_ = true;

		//odt_context->set_master_page_name(odt_context->page_layout_context()->last_master() ?
		//									  odt_context->page_layout_context()->last_master()->get_name() : L"");
	}
	if (oox_section_pr->m_oLnNumType.IsInit())
	{//linenumbering-configuration один для всех секций и всех разметок страниц ((( - хуевый OpenOffice (также не начала нумерации)
		//Ms Office тоже фуфло - нет нумерации алфавитами и римскими, нет разделителей
		odf_writer::office_element_ptr lnNum_elm;
		odf_writer::create_element(L"text", L"linenumbering-configuration", lnNum_elm, odf_context());

		odf_writer::text_linenumbering_configuration *linenumbering = dynamic_cast<odf_writer::text_linenumbering_configuration *>(lnNum_elm.get());
		if (!linenumbering) return;

		linenumbering->text_style_name_ = odt_context->styles_context()->find_free_name(style_family::LineNumbering);
		linenumbering->text_number_lines_ = true;

		if (oox_section_pr->m_oLnNumType->m_oCountBy.IsInit())
		{
			linenumbering->text_increment_ = oox_section_pr->m_oLnNumType->m_oCountBy->GetValue();
		}
		if (oox_section_pr->m_oLnNumType->m_oDistance.IsInit())
		{
			linenumbering->text_offset_ = odf_types::length(oox_section_pr->m_oLnNumType->m_oDistance->ToPoints(), odf_types::length::pt);
					}
		if (oox_section_pr->m_oLnNumType->m_oRestart.IsInit())
		{
			if (oox_section_pr->m_oLnNumType->m_oRestart->GetValue() == SimpleTypes::linenumberrestartNewPage)
			{
				linenumbering->text_restart_on_page_ = true;
			}
			else
			{
			}
		}
		if (oox_section_pr->m_oLnNumType->m_oStart.IsInit())
		{
		}
		odt_context->styles_context()->add_style(lnNum_elm, false, true, style_family::LineNumbering);

	}
//------------------------------------------------------------------------------------------------------------------------------------------		
	// то что относится собственно к секциям-разделам

            //nullable<ComplexTypes::Word::COnOff2 > m_oBidi;
			//nullable<ComplexTypes::Word::CDocGrid                        > m_oDocGrid;
			//nullable<OOX::Logic::CEdnProps                               > m_oEndnotePr;
			//nullable<OOX::Logic::CFtnProps                               > m_oFootnotePr;
            //nullable<ComplexTypes::Word::COnOff2 > m_oFormProt;

            //nullable<ComplexTypes::Word::COnOff2 > m_oNoEndnote;
			//nullable<ComplexTypes::Word::CPaperSource                    > m_oPaperSrc;

			//nullable<ComplexTypes::Word::CRel                            > m_oPrinterSettings;
			//nullable<OOX::Logic::CSectPrChange                           > m_oSectPrChange;
//--------------------------------------------------------------------------------------------------------------------------------------------		
	if (bSection)
	{
		odt_context->add_section(continuous);
	}
	
	if (bSection && oox_section_pr->m_oCols.IsInit())
	{
		int num_columns = oox_section_pr->m_oCols->m_oNum.IsInit() ? oox_section_pr->m_oCols->m_oNum->GetValue() : 1;
		
		if (num_columns > 1 && oox_section_pr->m_oCols->m_arrColumns.size() > 0)
			num_columns = /*(std::max)*/( /*num_columns,*/ (int)oox_section_pr->m_oCols->m_arrColumns.size()) ; 

		double default_space_pt = -1;
		if (oox_section_pr->m_oCols->m_oSpace.IsInit())	default_space_pt = oox_section_pr->m_oCols->m_oSpace->ToPoints();
		
		bool separator = oox_section_pr->m_oCols->m_oSep.IsInit() && oox_section_pr->m_oCols->m_oSep->ToBool();
		
		odt_context->add_section_columns(num_columns, 
			oox_section_pr->m_oCols->m_arrColumns.size() > 0 ? -1 : default_space_pt , separator );

		if (num_columns > 1) //
		{
			std::vector<std::pair<double,double>> width_space;
			
			for (size_t i = 0; i < oox_section_pr->m_oCols->m_arrColumns.size(); i++)
			{
				if (oox_section_pr->m_oCols->m_arrColumns[i] == NULL) continue;
				
				double space = default_space_pt;
				if (oox_section_pr->m_oCols->m_arrColumns[i]->m_oSpace.IsInit())
					space = oox_section_pr->m_oCols->m_arrColumns[i]->m_oSpace->ToPoints();
			
				double w = -1; 
				if (oox_section_pr->m_oCols->m_arrColumns[i]->m_oW.IsInit())
					w = oox_section_pr->m_oCols->m_arrColumns[i]->m_oW->ToPoints();
				
				width_space.push_back(std::pair<double,double>(w, space));
			}

			odt_context->add_section_column(width_space);
		}
	}
}
void DocxConverter::convert(OOX::Logic::CBgPict *oox_bg_pict, int type)
{
	if (oox_bg_pict == NULL) return;

	if (oox_bg_pict->m_oColor.IsInit())
	{
		_CP_OPT(odf_types::color) color;
		convert (oox_bg_pict->m_oColor.GetPointer(), NULL, NULL, NULL, color);

		odt_context->set_background(color, type);	
	}
	odt_context->start_drawing_context();
		odt_context->drawing_context()->start_drawing();
		odt_context->drawing_context()->set_background_state(true);

	if (oox_bg_pict->m_oBackground.IsInit())
	{
		convert(oox_bg_pict->m_oBackground.GetPointer());
	}
	odf_writer::style_page_layout_properties *current_layout_properties = odt_context->page_layout_context()->last_layout()->get_properties();

	odt_context->drawing_context()->end_drawing_background(current_layout_properties->attlist_.common_draw_fill_attlist_);
	odt_context->drawing_context()->set_background_state(false);
	odt_context->end_drawing_context();
}
void DocxConverter::convert(OOX::Logic::CBackground *oox_background, int type)
{
	if (oox_background == NULL) return;

	_CP_OPT(odf_types::color) color;
	convert (	oox_background->m_oColor.GetPointer(), 
				oox_background->m_oThemeColor.GetPointer(), 
				oox_background->m_oThemeTint.GetPointer(), 
				oox_background->m_oThemeShade.GetPointer(), color);

	odt_context->set_background(color, type);	
	
	odt_context->start_drawing_context();
		odt_context->drawing_context()->start_drawing();
		odt_context->drawing_context()->set_background_state(true);

	if (oox_background->m_oDrawing.IsInit())
	{
		convert(oox_background->m_oDrawing.GetPointer());
	}
	else if (oox_background->m_oBackground.IsInit())
	{
		convert(oox_background->m_oBackground.GetPointer());
	}
	odf_writer::style_page_layout_properties * current_layout_properties = odt_context->page_layout_context()->last_layout()->get_properties();

	odt_context->drawing_context()->end_drawing_background(current_layout_properties->attlist_.common_draw_fill_attlist_);
	odt_context->drawing_context()->set_background_state(false);
	odt_context->end_drawing_context();
}

void DocxConverter::convert(ComplexTypes::Word::CFramePr *oox_frame_pr, odf_writer::style_paragraph_properties * paragraph_properties)
{
	if (oox_frame_pr == NULL) return;
	if (paragraph_properties == NULL) return;

	if (oox_frame_pr->m_oDropCap.IsInit() || oox_frame_pr->m_oLines.IsInit())
	{
		//буквица
		odt_context->start_drop_cap(paragraph_properties);

		if (oox_frame_pr->m_oDropCap.IsInit() && oox_frame_pr->m_oDropCap->GetValue() == SimpleTypes::dropcapMargin)
			odt_context->set_drop_cap_margin(true);
		if (oox_frame_pr->m_oLines.IsInit())
			odt_context->set_drop_cap_lines(oox_frame_pr->m_oLines->GetValue());
		//??
	//nullable<SimpleTypes::CTwipsMeasure       > m_oHSpace;
	//nullable<SimpleTypes::CTwipsMeasure       > m_oVSpace;
	}
	else
	{
		//фрейм текста .. ваще то его никто не формирует .. странно todoo ??
		if (oox_frame_pr->m_oXAlign.IsInit())
		{
			switch(oox_frame_pr->m_oXAlign->GetValue())
			{
			case SimpleTypes::xalignCenter  :	paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Center); break;
			case SimpleTypes::xalignInside  :	paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Start ); break;
			case SimpleTypes::xalignLeft    :	paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Left ); break;
			case SimpleTypes::xalignOutside :	paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::End ); break;
			case SimpleTypes::xalignRight   :	paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Right); break;
			}
		}
		if (oox_frame_pr->m_oYAlign.IsInit())
		{
			switch(oox_frame_pr->m_oYAlign->GetValue())
			{
			case SimpleTypes::yalignBottom  :	paragraph_properties->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Bottom); break;
			case SimpleTypes::yalignCenter  :	paragraph_properties->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Middle ); break;
			case SimpleTypes::yalignInline  :	paragraph_properties->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Baseline ); break;
			case SimpleTypes::yalignInside	:	paragraph_properties->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Auto ); break;
			case SimpleTypes::yalignOutside :	paragraph_properties->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Auto); break;
			case SimpleTypes::yalignTop		:	paragraph_properties->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Top); break;
			}
		}
	}
	//nullable<SimpleTypes::CHeightRule<>       > m_oHRule;

//имеют отношение только в drawing
	//nullable<SimpleTypes::CTwipsMeasure       > m_oW;
	//nullable<SimpleTypes::CWrap<>             > m_oWrap;
	//nullable<SimpleTypes::CTwipsMeasure       > m_oH;
	//nullable<SimpleTypes::CHAnchor<>          > m_oHAnchor;
	//nullable<SimpleTypes::CVAnchor<>          > m_oVAnchor;
	//nullable<SimpleTypes::CSignedTwipsMeasure > m_oX;
	//nullable<SimpleTypes::CSignedTwipsMeasure > m_oY;
}
void DocxConverter::convert(OOX::Logic::CTblBorders	*oox_border, odf_writer::style_table_cell_properties	*table_cell_properties)
{
	if (oox_border == NULL) return;
	if (table_cell_properties == NULL) return;

	std::wstring left,right,top,bottom, other;

	convert(oox_border->m_oBottom.GetPointer()	, bottom);
	convert(oox_border->m_oTop.GetPointer()		, top);
	convert(oox_border->m_oStart.GetPointer()	, left);
	convert(oox_border->m_oEnd.GetPointer()		, right);
	
	if (bottom == top && top == left && left== right && bottom.length() > 0)
	{
		table_cell_properties->content_.common_border_attlist_.fo_border_ = left;
	}
	else
	{
		table_cell_properties->content_.common_border_attlist_.fo_border_bottom_	= 
		table_cell_properties->content_.common_border_attlist_.fo_border_top_		= 
		table_cell_properties->content_.common_border_attlist_.fo_border_left_		= 
		table_cell_properties->content_.common_border_attlist_.fo_border_right_	= table_cell_properties->content_.common_border_attlist_.fo_border_;

		table_cell_properties->content_.common_border_attlist_.fo_border_ = boost::none;

		if (bottom.length() >0 )table_cell_properties->content_.common_border_attlist_.fo_border_bottom_	= bottom;
		if (top.length() >0 )	table_cell_properties->content_.common_border_attlist_.fo_border_top_		= top;
		if (left.length() >0 )	table_cell_properties->content_.common_border_attlist_.fo_border_left_		= left;
		if (right.length() >0 ) table_cell_properties->content_.common_border_attlist_.fo_border_right_		= right;
	}
	//nullable<ComplexTypes::Word::CBorder > m_oInsideH;
	//nullable<ComplexTypes::Word::CBorder > m_oInsideV;
}
void DocxConverter::convert(OOX::Logic::CTcBorders 	*oox_border, odf_writer::style_table_cell_properties	*table_cell_properties)
{
	if (oox_border == NULL) return;
	if (table_cell_properties == NULL) return;

	std::wstring left,right,top,bottom, other;

	convert(oox_border->m_oBottom.GetPointer()	, bottom);
	convert(oox_border->m_oTop.GetPointer()		, top);
	convert(oox_border->m_oStart.GetPointer()	, left);
	convert(oox_border->m_oEnd.GetPointer()		, right);
	
	if (bottom == top && top == left && left== right && bottom.length() > 0)
	{
		table_cell_properties->content_.common_border_attlist_.fo_border_ = left;

		table_cell_properties->content_.common_border_attlist_.fo_border_bottom_ =
		table_cell_properties->content_.common_border_attlist_.fo_border_top_ = 
		table_cell_properties->content_.common_border_attlist_.fo_border_left_ = 
		table_cell_properties->content_.common_border_attlist_.fo_border_right_ = boost::none;
	}
	else
	{
		if (table_cell_properties->content_.common_border_attlist_.fo_border_)
		{
			table_cell_properties->content_.common_border_attlist_.fo_border_bottom_	= 
			table_cell_properties->content_.common_border_attlist_.fo_border_top_		= 
			table_cell_properties->content_.common_border_attlist_.fo_border_left_		= 
			table_cell_properties->content_.common_border_attlist_.fo_border_right_		= table_cell_properties->content_.common_border_attlist_.fo_border_;
		}
		table_cell_properties->content_.common_border_attlist_.fo_border_ = boost::none;

		if (bottom.length() >0 )table_cell_properties->content_.common_border_attlist_.fo_border_bottom_	= bottom;
		if (top.length() >0  )	table_cell_properties->content_.common_border_attlist_.fo_border_top_		= top;
		if (left.length() >0 )	table_cell_properties->content_.common_border_attlist_.fo_border_left_		= left;
		if (right.length() >0 ) table_cell_properties->content_.common_border_attlist_.fo_border_right_		= right;
	}
	convert(oox_border->m_oTL2BR.GetPointer()	, other);
	if (other.length() >0 ) table_cell_properties->content_.style_diagonal_tl_br_ = other;
	
	convert(oox_border->m_oTR2BL.GetPointer()	, other);
	if (other.length() >0 )
	{
		table_cell_properties->content_.style_diagonal_bl_tr_ = other;
	}
	
	//nullable<ComplexTypes::Word::CBorder > m_oInsideH;
	//nullable<ComplexTypes::Word::CBorder > m_oInsideV;
}

void DocxConverter::convert(OOX::Logic::CPBdr *oox_border, odf_writer::style_paragraph_properties *paragraph_properties)
{
	if (oox_border == NULL) return;
	if (paragraph_properties == NULL) return;

	std::wstring left,right,top,bottom, other;

	convert(oox_border->m_oBottom.GetPointer()	, bottom);
	convert(oox_border->m_oTop.GetPointer()		, top);
	convert(oox_border->m_oLeft.GetPointer()	, left);
	convert(oox_border->m_oRight.GetPointer()	, right);
	
	if (bottom == top && top == left && left== right && bottom.length() > 0)
	{
		paragraph_properties->content_.common_border_attlist_.fo_border_ = left;
	}
	else
	{
		paragraph_properties->content_.common_border_attlist_.fo_border_bottom_	= 
		paragraph_properties->content_.common_border_attlist_.fo_border_top_		= 
		paragraph_properties->content_.common_border_attlist_.fo_border_left_		= 
		paragraph_properties->content_.common_border_attlist_.fo_border_right_	= paragraph_properties->content_.common_border_attlist_.fo_border_;

		paragraph_properties->content_.common_border_attlist_.fo_border_ = boost::none;

		if (bottom.length() > 0 )	paragraph_properties->content_.common_border_attlist_.fo_border_bottom_	= bottom;
		if (top.length()	> 0 )	paragraph_properties->content_.common_border_attlist_.fo_border_top_		= top;
		if (left.length()	> 0 )	paragraph_properties->content_.common_border_attlist_.fo_border_left_		= left;
		if (right.length()	> 0 )	paragraph_properties->content_.common_border_attlist_.fo_border_right_		= right;
	}
	bool shadow = false;//в либре только одна тень (
	if (oox_border->m_oBottom.IsInit()	&& oox_border->m_oBottom->m_oShadow.IsInit()	&& oox_border->m_oBottom->m_oShadow->ToBool())		shadow = true;
	if (oox_border->m_oTop.IsInit()		&& oox_border->m_oTop->m_oShadow.IsInit()		&& oox_border->m_oTop->m_oShadow->ToBool())			shadow = true;
	if (oox_border->m_oLeft.IsInit()	&& oox_border->m_oLeft->m_oShadow.IsInit()		&& oox_border->m_oLeft->m_oShadow->ToBool())		shadow = true;
	if (oox_border->m_oRight.IsInit()	&& oox_border->m_oRight->m_oShadow.IsInit()			&& oox_border->m_oRight->m_oShadow->ToBool())	shadow = true;
	
	if (shadow)
	{
		paragraph_properties->content_.style_shadow_ = L"#000000 0.159cm 0.159cm";
	}
	//if (oox_border->m_oTL2BR.IsInit())
	//{
	//	convert(oox_border->m_oTL2BR.GetPointer(), odf_border);
	//	if (odf_border.length() >0 ) paragraph_properties->content_.style_diagonal_tl_br_ = odf_border;
	//}

}
void DocxConverter::convert(ComplexTypes::Word::CPageBorder *borderProp, std::wstring & odf_border_prop)
{
	odf_border_prop = L"";
	if (!borderProp)return;
			
	//nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse> > m_oFrame;
	//nullable<SimpleTypes::CPointMeasure<>                 > m_oSpace;

	std::wstringstream border_style;
	std::wstring border_color;

	odf_types::length length;
	length = odf_types::length(borderProp->m_oSz.IsInit() ? borderProp->m_oSz->ToPoints() : 1 ,  odf_types::length::pt);

	length = odf_types::length( length.get_value_unit(odf_types::length::cm), odf_types::length::cm);
	border_style << length;
	if (borderProp->m_oVal.IsInit())//266 styles  oO
	{
		switch(borderProp->m_oVal->GetValue())
		{
			case SimpleTypes::bordervalueDotDash:
				border_style << L" dashed";
			break;
			case SimpleTypes::bordervalueDashed:
				border_style << L" dashed";
			break;
			case SimpleTypes::bordervalueDotted:
				border_style << L" dotted";
			break;
			case SimpleTypes::bordervalueDouble:
				border_style << L" double";
			break;
			case SimpleTypes::bordervalueSingle:
				border_style << L" solid";
			break;
			case SimpleTypes::bordervalueNone:
			case SimpleTypes::bordervalueNil:
				odf_border_prop = L"none";
				return;
			break;
			default:
				border_style << L" solid";
		}
	}else border_style << L" solid";
///////////////////////////////////////////////////////////////////
	_CP_OPT(odf_types::color) color;

	if (borderProp->m_oColor.IsInit())
	{
		if (borderProp->m_oColor->GetValue() != SimpleTypes::hexcolorAuto)
			convert(borderProp->m_oColor.GetPointer(), borderProp->m_oThemeColor.GetPointer(), borderProp->m_oThemeTint.GetPointer(), borderProp->m_oThemeShade.GetPointer(), color);
	}
	if (color) border_color = color->get_hex_value();
	else border_color = L"000000";
	
	odf_border_prop = border_style.str() + L" #" + border_color;
}
void DocxConverter::convert(ComplexTypes::Word::CBorder *borderProp, std::wstring & odf_border_prop)
{
	odf_border_prop = L"";
	if (!borderProp)
		return;

	std::wstringstream border_style;
	std::wstring border_color;

	odf_types::length length;
	length = odf_types::length(borderProp->m_oSz.IsInit() ? borderProp->m_oSz->ToPoints() : 1 ,  odf_types::length::pt);

	length = odf_types::length( length.get_value_unit(odf_types::length::cm), odf_types::length::cm);
	border_style << length;
	if (borderProp->m_oVal.IsInit())//266 styles  oO
	{
		switch(borderProp->m_oVal->GetValue())
		{
			case SimpleTypes::bordervalueDotDash:
				border_style << L" dashed";
			break;
			case SimpleTypes::bordervalueDashed:
				border_style << L" dashed";
			break;
			case SimpleTypes::bordervalueDotted:
				border_style << L" dotted";
			break;
			case SimpleTypes::bordervalueDouble:
				border_style << L" double";
			break;
			case SimpleTypes::bordervalueSingle:
				border_style << L" solid";
			break;
			case SimpleTypes::bordervalueNone:
			case SimpleTypes::bordervalueNil:
				odf_border_prop = L"none";
				return;
			break;
			default:
				border_style << L" solid";
		}
	}else border_style << L" solid";
///////////////////////////////////////////////////////////////////
	_CP_OPT(odf_types::color) color;

	if (borderProp->m_oColor.IsInit() || borderProp->m_oThemeColor.IsInit())
	{
		convert(borderProp->m_oColor.GetPointer(), borderProp->m_oThemeColor.GetPointer(), borderProp->m_oThemeTint.GetPointer(),borderProp->m_oThemeShade.GetPointer(), color);
	}
	if (color) border_color = color->get_hex_value();
	else border_color = L"000000";
	
	odf_border_prop = border_style.str() + L" #" + border_color;
}
void DocxConverter::convert(ComplexTypes::Word::CJc * oox_jc,  _CP_OPT(odf_types::text_align) & align)
{
	if (oox_jc == NULL) return;
	if (oox_jc->m_oVal.IsInit() == false) return;

	switch(oox_jc->m_oVal->GetValue())
	{
		case SimpleTypes::jcBoth            : align = odf_types::text_align(odf_types::text_align::Justify);break;
		case SimpleTypes::jcCenter          : align = odf_types::text_align(odf_types::text_align::Center);	break;
		case SimpleTypes::jcThaiDistribute  :	
		case SimpleTypes::jcDistribute      : align = odf_types::text_align(odf_types::text_align::Justify);break;
		case SimpleTypes::jcEnd             : align = odf_types::text_align(odf_types::text_align::End);	break;
		case SimpleTypes::jcHighKashida     :	break;
		case SimpleTypes::jcLowKashida      :	break;
		case SimpleTypes::jcMediumKashida   :	break;
		case SimpleTypes::jcNumTab          :	break;
		case SimpleTypes::jcStart           : align = odf_types::text_align(odf_types::text_align::Start);	break;
		case SimpleTypes::jcLeft            : align = odf_types::text_align(odf_types::text_align::Left);	break;
		case SimpleTypes::jcRight           : align = odf_types::text_align(odf_types::text_align::Right);	break;
	}
}
void DocxConverter::convert(SimpleTypes::CUniversalMeasure *oox_size, _CP_OPT(odf_types::length) & odf_size)
{
	odf_size = boost::none;
	if (oox_size == NULL)return;

	odf_size = odf_types::length(oox_size->ToPoints(), odf_types::length::pt);
}
void DocxConverter::convert(SimpleTypes::CUniversalMeasure* oox_size, _CP_OPT(odf_types::length_or_percent) & odf_size)
{
	odf_size = boost::none;
	if (oox_size == NULL)return;

		//odf_size = odf_types::percent(oox_size->ToPoints(());
	odf_size = odf_types::length(oox_size->ToPoints(), odf_types::length::pt);

}
void DocxConverter::convert(ComplexTypes::Word::CTblWidth *oox_size, _CP_OPT(odf_types::length) & odf_size)
{
	odf_size = boost::none;
	if (oox_size == NULL)return;

	if (oox_size->m_oW.IsInit() == false) return;

	if (oox_size->m_oType.IsInit() && oox_size->m_oType->GetValue() == SimpleTypes::tblwidthDxa)
	{
		odf_size = odf_types::length(oox_size->m_oW->GetValue() / 20., odf_types::length::pt);
	}

	//tblwidthAuto = 0,
	//tblwidthDxa  = 1,
	//tblwidthNil  = 2,
	//tblwidthPct  = 3
}
void DocxConverter::convert(OOX::Logic::CRunProperty *oox_run_pr, odf_writer::style_text_properties * text_properties, bool is_para_props)
{
	if (oox_run_pr		== NULL) return;
	if (text_properties == NULL) return;
	
	if (oox_run_pr->m_oRStyle.IsInit() && oox_run_pr->m_oRStyle->m_sVal.IsInit())
	{
		std::wstring style_name = *oox_run_pr->m_oRStyle->m_sVal;

		odf_writer::style_text_properties  parent_text_properties;
		odt_context->styles_context()->calc_text_properties(style_name, odf_types::style_family::Text, &parent_text_properties.content_);
		
		odf_writer::odf_style_state_ptr style_state;
		style_state = odt_context->styles_context()->last_state();

		if (style_state)
		{
			odf_writer::odf_drawing_context* drawing_context = odt_context->drawing_context();
			if ((drawing_context) && (false == drawing_context->is_current_empty()))
			{
				text_properties->apply_from(&parent_text_properties);
			}
			else
			{
				style_state->set_parent_style_name(style_name);
			}
		}		

		if (parent_text_properties.content_.fo_font_size_)
		{
			current_font_size.push_back(parent_text_properties.content_.fo_font_size_->get_length().get_value_unit(odf_types::length::pt));
		}
	}
	if (oox_run_pr->m_oBold.IsInit())
	{
		if (oox_run_pr->m_oBold->m_oVal.ToBool()) 
			text_properties->content_.fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WBold);
		else
			text_properties->content_.fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WNormal);
	}

	odf_writer::odf_drawing_context	 *drawing_context = odf_context()->drawing_context();	
	
	bool set_word_art = drawing_context ? drawing_context->is_wordart() : false;

	NSCommon::smart_ptr<PPTX::Logic::GradFill> gradFill = oox_run_pr->m_oTextFill.Fill.smart_dynamic_cast<PPTX::Logic::GradFill>();
	NSCommon::smart_ptr<PPTX::Logic::SolidFill> solidFill = oox_run_pr->m_oTextFill.Fill.smart_dynamic_cast<PPTX::Logic::SolidFill>();

	bool bFillText =	(oox_run_pr->m_oTextFill.m_type != PPTX::Logic::UniFill::notInit) && 
						(oox_run_pr->m_oTextFill.m_type != PPTX::Logic::UniFill::noFill);

	bool bOutlineText = ((oox_run_pr->m_oTextOutline.is_init()) && 
						(oox_run_pr->m_oTextOutline->Fill.m_type != PPTX::Logic::UniFill::notInit) && 
						(oox_run_pr->m_oTextOutline->Fill.m_type != PPTX::Logic::UniFill::noFill));

	bool bOutline	= ((oox_run_pr->m_oOutline.IsInit()) && (oox_run_pr->m_oOutline->m_oVal.ToBool()));
	bool bColorText = !bOutline && (oox_run_pr->m_oColor.IsInit() && (oox_run_pr->m_oColor->m_oVal.IsInit() && oox_run_pr->m_oColor->m_oVal->GetValue() == SimpleTypes::hexcolorRGB));
	
	_CP_OPT(odf_types::color) color;

	if (oox_run_pr->m_oColor.IsInit())
	{
		if(oox_run_pr->m_oColor->m_oVal.IsInit() && oox_run_pr->m_oColor->m_oVal->GetValue() == SimpleTypes::hexcolorAuto)
			color = odf_types::color(L"#000000");
		else
		   convert(oox_run_pr->m_oColor.GetPointer(), color);
		
		text_properties->content_.fo_color_ = color;
	}
	if	(gradFill.is_init() || (bOutlineText && (bFillText || bColorText)))
	{
		set_word_art = true;	
	}

	if (drawing_context && set_word_art)
	{
		if (drawing_context->change_text_box_2_wordart())
		{			
			drawing_context->start_area_properties();				
			if(gradFill.IsInit())
			{		
				OoxConverter::convert(gradFill.operator->());
			}
			else if (solidFill.IsInit())
			{
				OoxConverter::convert(solidFill.operator->());
			}
			else if (color)
			{
				drawing_context->set_solid_fill(color->get_hex_value());
			}
			else
			{
				drawing_context->set_no_fill();
			}
			drawing_context->end_area_properties();

			if (oox_run_pr->m_oTextOutline.IsInit())
			{
				drawing_context->start_line_properties(true);
				OoxConverter::convert(oox_run_pr->m_oTextOutline.operator->());
				drawing_context->end_line_properties();
			}
		}
		else
		{
			set_word_art = false;
		}
	}

	std::wstring	hexString;
	_CP_OPT(double) opacity;
	if (bOutlineText)
	{
		text_properties->content_.style_text_outline_ = true;
		
		gradFill = oox_run_pr->m_oTextOutline->Fill.Fill.smart_dynamic_cast<PPTX::Logic::GradFill>();
		solidFill = oox_run_pr->m_oTextOutline->Fill.Fill.smart_dynamic_cast<PPTX::Logic::SolidFill>();
	}
	if (solidFill.is_init())
	{
		OoxConverter::convert(&solidFill->Color, hexString, opacity);
	}
	else if ((gradFill.is_init()) && (false == gradFill->GsLst.empty()))
	{
		OoxConverter::convert(&gradFill->GsLst[0].color, hexString, opacity);
	}
	if (!hexString.empty())
	{
		text_properties->content_.fo_color_ = hexString;	
	}

    //text_properties->content_.style_text_underline_type_= odf_types::line_type(odf_types::line_type::None); //нельзя..если будет выше наследуемого то подчеркивания не будет
	if (oox_run_pr->m_oU.IsInit())
	{
		text_properties->content_.style_text_underline_style_	= odf_types::line_style(odf_types::line_style::Solid);
		text_properties->content_.style_text_underline_type_	= odf_types::line_type(odf_types::line_type::Single);
		text_properties->content_.style_text_underline_width_	= odf_types::line_width(odf_types::line_width::Auto);

		_CP_OPT(odf_types::color) color;
		convert(oox_run_pr->m_oU->m_oColor.GetPointer(), oox_run_pr->m_oU->m_oThemeColor.GetPointer(),
			oox_run_pr->m_oU->m_oThemeTint.GetPointer(), oox_run_pr->m_oU->m_oThemeShade.GetPointer(), color);
		
		if (color) text_properties->content_.style_text_underline_color_ = color;
		else text_properties->content_.style_text_underline_color_ = odf_types::underline_color(odf_types::underline_color::FontColor);
		
		
		if (oox_run_pr->m_oU->m_oVal.IsInit())
		{
			SimpleTypes::EUnderline 	type = oox_run_pr->m_oU->m_oVal->GetValue();

			switch(type)
			{
			case SimpleTypes::underlineDouble		:
					text_properties->content_.style_text_underline_type_= odf_types::line_type(odf_types::line_type::Double);break;
			case SimpleTypes::underlineNone			:
				{
                    text_properties->content_.style_text_underline_type_	= odf_types::line_type(odf_types::line_type::None);
					text_properties->content_.style_text_underline_style_	= boost::none;
					text_properties->content_.style_text_underline_color_  = boost::none;
				}break;
			case SimpleTypes::underlineDash            :
			case SimpleTypes::underlineDashedHeavy     :
					text_properties->content_.style_text_underline_style_= odf_types::line_style(odf_types::line_style::Dash);break;
			case SimpleTypes::underlineDashLong        :
			case SimpleTypes::underlineDashLongHeavy   :
					text_properties->content_.style_text_underline_style_= odf_types::line_style(odf_types::line_style::LongDash);break;
			case SimpleTypes::underlineDotDash         :
			case SimpleTypes::underlineDashDotHeavy    :
					text_properties->content_.style_text_underline_style_= odf_types::line_style(odf_types::line_style::DotDash);break;
			case SimpleTypes::underlineDotted          :
			case SimpleTypes::underlineDottedHeavy     :
					text_properties->content_.style_text_underline_style_= odf_types::line_style(odf_types::line_style::Dotted);break;
			case SimpleTypes::underlineDotDotDash      :
			case SimpleTypes::underlineDashDotDotHeavy :
					text_properties->content_.style_text_underline_style_= odf_types::line_style(odf_types::line_style::DotDotDash);break;
			case SimpleTypes::underlineThick           :
			case SimpleTypes::underlineWave            :
			case SimpleTypes::underlineWavyDouble      :
			case SimpleTypes::underlineWavyHeavy       :
					text_properties->content_.style_text_underline_style_= odf_types::line_style(odf_types::line_style::Wave);break;
			}
		}
	}
	if (oox_run_pr->m_oItalic.IsInit())
	{
		if (oox_run_pr->m_oItalic->m_oVal.ToBool() ==true)
			text_properties->content_.fo_font_style_ = odf_types::font_style(odf_types::font_style::Italic);
		else
			text_properties->content_.fo_font_style_ = odf_types::font_style(odf_types::font_style::Normal);
	}
	if (oox_run_pr->m_oSz.IsInit() && oox_run_pr->m_oSz->m_oVal.IsInit())
	{
		double font_size_pt = oox_run_pr->m_oSz->m_oVal->ToPoints();
		current_font_size.push_back(font_size_pt);

		OoxConverter::convert(font_size_pt, text_properties->content_.fo_font_size_);
	}
	if (oox_run_pr->m_oKern.IsInit() && oox_run_pr->m_oKern->m_oVal.IsInit())
	{
		//OoxConverter::convert(oox_run_pr->m_oSz->m_oVal->ToPoints(), text_properties->content_.fo_font_size_);
		text_properties->content_.style_letter_kerning_ = true;
	}
	if (oox_run_pr->m_oCaps.IsInit())
	{
		if (oox_run_pr->m_oCaps->m_oVal.ToBool())
			text_properties->content_.fo_text_transform_ = odf_types::text_transform(odf_types::text_transform::Uppercase);
		else
			text_properties->content_.fo_text_transform_ = odf_types::text_transform(odf_types::text_transform::None);
	}
	if (oox_run_pr->m_oSmallCaps.IsInit())
	{
		if (oox_run_pr->m_oSmallCaps->m_oVal.ToBool())
			text_properties->content_.fo_font_variant_ = odf_types::font_variant(odf_types::font_variant::SmallCaps);
		else
			text_properties->content_.fo_font_variant_ = odf_types::font_variant(odf_types::font_variant::Normal);
	}

	if (oox_run_pr->m_oRFonts.IsInit())
	{
		if (oox_run_pr->m_oRFonts->m_sAscii.IsInit())
			text_properties->content_.fo_font_family_ = oox_run_pr->m_oRFonts->m_sAscii.get();
		else convert(oox_run_pr->m_oRFonts->m_oAsciiTheme.GetPointer(), text_properties->content_.fo_font_family_);

		if (oox_run_pr->m_oRFonts->m_sCs.IsInit())
			text_properties->content_.style_font_family_complex_ = oox_run_pr->m_oRFonts->m_sCs.get();
		else convert(oox_run_pr->m_oRFonts->m_oCsTheme.GetPointer(), text_properties->content_.style_font_family_complex_);

		if (oox_run_pr->m_oRFonts->m_sEastAsia.IsInit())
			text_properties->content_.style_font_family_asian_= oox_run_pr->m_oRFonts->m_sEastAsia.get();
		else convert(oox_run_pr->m_oRFonts->m_oEastAsiaTheme.GetPointer(), text_properties->content_.style_font_family_asian_);

            //nullable<std::wstring              > m_sHAnsi;
			//nullable<SimpleTypes::CTheme<>> m_oHAnsiTheme;
			//nullable<SimpleTypes::CHint<> > m_oHint;
	}
	if (!is_para_props && oox_run_pr->m_oVertAlign.IsInit() && oox_run_pr->m_oVertAlign->m_oVal.IsInit())
	{ 
		switch(oox_run_pr->m_oVertAlign->m_oVal->GetValue())
		{
		case SimpleTypes::verticalalignrunSuperscript:
			text_properties->content_.style_text_position_ = odf_types::text_position(odf_types::text_position::Super, 58); break;
		case SimpleTypes::verticalalignrunSubscript:
			text_properties->content_.style_text_position_ = odf_types::text_position(odf_types::text_position::Sub, 58); break;
		}
	}
	if (oox_run_pr->m_oW.IsInit() && oox_run_pr->m_oW->m_oVal.IsInit())
	{
		text_properties->content_.style_text_scale_ = odf_types::percent(oox_run_pr->m_oW->m_oVal->GetValue());
	}
	if (oox_run_pr->m_oStrike.IsInit() )
	{
		if (oox_run_pr->m_oStrike->m_oVal.ToBool())
			text_properties->content_.style_text_line_through_type_ = odf_types::line_type(odf_types::line_type::Single);
		else
			text_properties->content_.style_text_line_through_type_ = odf_types::line_type(odf_types::line_type::None);
	}
	if (oox_run_pr->m_oDStrike.IsInit())
	{
		if (oox_run_pr->m_oDStrike->m_oVal.ToBool())
			text_properties->content_.style_text_line_through_type_ = odf_types::line_type(odf_types::line_type::Double);
		else
			text_properties->content_.style_text_line_through_type_ = odf_types::line_type(odf_types::line_type::None);
	}
	if (oox_run_pr->m_oSpacing.IsInit() && oox_run_pr->m_oSpacing->m_oVal.IsInit())
	{
		double spacing = oox_run_pr->m_oSpacing->m_oVal->ToPoints();
		text_properties->content_.fo_letter_spacing_ = odf_types::letter_spacing(odf_types::length(spacing, odf_types::length::pt));
	}
	if (oox_run_pr->m_oPosition.IsInit() && oox_run_pr->m_oPosition->m_oVal.IsInit())
	{
		double position_pt = oox_run_pr->m_oPosition->m_oVal->ToPoints();
		double percent = current_font_size.empty() ? 0 : position_pt / current_font_size.back() * 100;

		text_properties->content_.style_text_position_ = odf_types::text_position(percent, 100.);
	}
	if (oox_run_pr->m_oBdr.IsInit())
	{
		std::wstring odf_border;
		convert(oox_run_pr->m_oBdr.GetPointer(), odf_border);
		if (odf_border.length() > 0)
			text_properties->content_.common_border_attlist_.fo_border_ = odf_border;
	}
	if (oox_run_pr->m_oShd.IsInit())
	{
		_CP_OPT(odf_types::color) odf_color;
		convert(oox_run_pr->m_oShd.GetPointer(), odf_color);
		if (odf_color)
		{
			text_properties->content_.fo_background_color_= *odf_color;
		}
	}
	if (oox_run_pr->m_oHighlight.IsInit() && oox_run_pr->m_oHighlight->m_oVal.IsInit())
	{
		if (oox_run_pr->m_oHighlight->m_oVal->GetValue() != SimpleTypes::highlightcolorNone)
		{
			BYTE ucR = oox_run_pr->m_oHighlight->m_oVal->Get_R();
			BYTE ucB = oox_run_pr->m_oHighlight->m_oVal->Get_B();
			BYTE ucG = oox_run_pr->m_oHighlight->m_oVal->Get_G();
			SimpleTypes::CHexColor *oRgbColor = new SimpleTypes::CHexColor(ucR, ucG, ucB);

			if (oRgbColor)
			{
				std::wstring strColor = L"#" + oRgbColor->ToString().substr(2);//.Right(6);
				text_properties->content_.fo_background_color_ = odf_types::color(strColor);
				delete oRgbColor;
			}
		}
	}
	if ((oox_run_pr->m_oOutline.IsInit()) && (oox_run_pr->m_oOutline->m_oVal.ToBool()))
		text_properties->content_.style_text_outline_ = true; //контур

	if (oox_run_pr->m_oVanish.IsInit())
		text_properties->content_.text_display_ = odf_types::text_display(odf_types::text_display::None);

	if (oox_run_pr->m_oLang.IsInit())
	{
		if (oox_run_pr->m_oLang->m_oVal.IsInit())
		{
			std::wstring lang = *oox_run_pr->m_oLang->m_oVal;
			size_t split = lang.find(L"-");
			if (split != std::wstring::npos)
			{
				text_properties->content_.fo_language_ = lang.substr(0, split);
				text_properties->content_.fo_country_ = lang.substr(split + 1);
			}
		}
	}
}

void DocxConverter::convert(SimpleTypes::CTheme* oox_font_theme, _CP_OPT(std::wstring) & odf_font_name)
{
	if (oox_font_theme == NULL) return;

	if (docx_document->m_pTheme == NULL) return;

	std::wstring font;

	switch(oox_font_theme->GetValue())
	{
	case SimpleTypes::themeMajorAscii:
	case SimpleTypes::themeMajorHAnsi :
		font = docx_document->m_pTheme->themeElements.fontScheme.majorFont.latin.typeface;
		break;
	case SimpleTypes::themeMajorBidi:
		font = docx_document->m_pTheme->themeElements.fontScheme.majorFont.cs.typeface;
		break;
	case SimpleTypes::themeMajorEastAsia:
		font = docx_document->m_pTheme->themeElements.fontScheme.majorFont.ea.typeface;
		break;
	case SimpleTypes::themeMinorAscii:
	case SimpleTypes::themeMinorHAnsi:
		font = docx_document->m_pTheme->themeElements.fontScheme.minorFont.latin.typeface;
		break;
	case SimpleTypes::themeMinorBidi:
		font = docx_document->m_pTheme->themeElements.fontScheme.minorFont.cs.typeface;
		break;
	case SimpleTypes::themeMinorEastAsia:
		font = docx_document->m_pTheme->themeElements.fontScheme.minorFont.ea.typeface;
		break;
	}
	if (!font.empty()) odf_font_name = font;
}

void DocxConverter::convert(OOX::Logic::CText *oox_text)
{
	if (oox_text == NULL) return;

	if (oox_text->m_oSpace.IsInit())
	{
		if (oox_text->m_oSpace->GetValue() == SimpleTypes::xmlspacePreserve)
		{
		}
			//odt_context->text_context()->add_text_space(26);
	}
	odt_context->add_text_content(oox_text->m_sText);
}
void DocxConverter::convert(OOX::Logic::CAlternateContent *oox_alt_content)
{
	if (oox_alt_content == NULL)return;

	for(size_t i = 0; i < oox_alt_content->m_arrChoiceItems.size(); ++i) // правильный выбор
	{
		convert(oox_alt_content->m_arrChoiceItems[i]);
	}
	
	if (oox_alt_content->m_arrChoiceItems.size() > 0) return;  //чтоб не было дубляжа
	for(size_t i = 0; i < oox_alt_content->m_arrFallbackItems.size(); ++i) // альтернативный 
	// todooo нужно сверять по инддексно что нормально сконвертилось ... или делать ВСЕ по choice ( это правильнее)
	{
		convert(oox_alt_content->m_arrFallbackItems[i]);
	}
}
///////////////////////////////////////
//old formating vml
void DocxConverter::convert(OOX::Logic::CPicture* oox_pic)
{
	if (oox_pic == NULL) return;

	odt_context->start_drawing_context();
			
	if (odt_context->table_context()->empty())
		odf_context()->drawing_context()->set_anchor(anchor_type::Char);//default
	else
	{
		odf_context()->drawing_context()->set_anchor(anchor_type::Paragraph);
		odf_context()->drawing_context()->set_object_background(true);
	}
		
	OoxConverter::convert(oox_pic->m_oShapeType.GetPointer());
	OoxConverter::convert(oox_pic->m_oShapeElement.GetPointer());

	odt_context->end_drawing_context();
}
void DocxConverter::convert(OOX::Logic::CObject* oox_obj)
{
	if (oox_obj == NULL) return;

	odt_context->start_drawing_context();
			
	if (odt_context->table_context()->empty())
		odf_context()->drawing_context()->set_anchor(odf_types::anchor_type::AsChar);//default
	else
	{
		odf_context()->drawing_context()->set_anchor(odf_types::anchor_type::Paragraph);
		odf_context()->drawing_context()->set_object_background(true);
	}

	if (oox_obj->m_oShape.IsInit())
		OoxConverter::convert(oox_obj->m_oShape->m_oStyle.GetPointer());
		
	odf_context()->drawing_context()->start_drawing();
	
	bool bSetObject = false;
	
	if (oox_obj->m_oOleObject.IsInit())
	{
		std::wstring pathOle;
		bool bExternal = false;

		if (oox_obj->m_oOleObject->m_oId.IsInit())
		{
			pathOle = find_link_by_id(oox_obj->m_oOleObject->m_oId->GetValue(), 4, bExternal);
		}
		std::wstring odf_ref_ole = odf_context()->add_oleobject(pathOle);

		if (!odf_ref_ole.empty())
		{
			odf_context()->drawing_context()->start_object_ole(odf_ref_ole);
			OoxConverter::convert(oox_obj->m_oShape.GetPointer());

			if (oox_obj->m_oOleObject->m_sProgId.IsInit())
			{
				odf_context()->drawing_context()->set_program(*oox_obj->m_oOleObject->m_sProgId);
			}
			std::wstring sIdImageFileCache = GetImageIdFromVmlShape(oox_obj->m_oShape.GetPointer());
			
			std::wstring pathImage = find_link_by_id(sIdImageFileCache, 1, bExternal);
			std::wstring odf_ref_image = odf_context()->add_imageobject(pathImage);
			
			odf_context()->drawing_context()->set_image_replacement(odf_ref_image);

			odf_context()->drawing_context()->end_object_ole();
			
			bSetObject = true;
		}
	}
	if (!bSetObject)
	{
		if (oox_obj->m_oShape.IsInit())
		{
			SimpleTypes::Vml::SptType sptType = SimpleTypes::Vml::SptType::sptNotPrimitive;

			if ((oox_obj->m_oShapeType.IsInit()) && (oox_obj->m_oShapeType->m_oSpt.IsInit()))
				sptType = static_cast<SimpleTypes::Vml::SptType>(oox_obj->m_oShapeType->m_oSpt->GetValue());

			if (sptType != SimpleTypes::Vml::SptType::sptNotPrimitive)
			{
				odf_context()->drawing_context()->set_name(std::wstring(L"Custom") + std::to_wstring(sptType));
				odf_context()->drawing_context()->start_shape(OOX::VmlShapeType2PrstShape(sptType));
			}
			else if ((oox_obj->m_oShape->m_oConnectorType.IsInit()) && (oox_obj->m_oShape->m_oConnectorType->GetValue() != SimpleTypes::connectortypeNone))
			{
				odf_context()->drawing_context()->set_name(L"Connector");
				odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeStraightConnector1);
				odf_context()->drawing_context()->set_line_width(1.);
			}
			else if (oox_obj->m_oShape->m_oPath.IsInit())
			{
				odf_context()->drawing_context()->set_name(L"Path");
				odf_context()->drawing_context()->start_shape(1001);
				odf_context()->drawing_context()->set_line_width(1.);
			}
			else
			{
				odf_context()->drawing_context()->set_name(L"Rect");
				odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeRect);
			}
		}
		else
		{
			odf_context()->drawing_context()->set_name(L"Rect");
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeRect);
		}
		OoxConverter::convert(oox_obj->m_oShape.GetPointer()); 	
		odf_context()->drawing_context()->set_type_fill(2); //temp ...  image 

		odf_context()->drawing_context()->end_shape(); 
	}

	odf_context()->drawing_context()->end_drawing();

	odt_context->end_drawing_context();
}
///////////////////////////////////////////////
void DocxConverter::convert(OOX::Logic::CDrawing *oox_drawing)
{
	if (oox_drawing == NULL) return;

	odt_context->start_drawing_context();
		convert(oox_drawing->m_oAnchor.GetPointer());
		convert(oox_drawing->m_oInline.GetPointer());
	odt_context->end_drawing_context();
}
void DocxConverter::convert(OOX::Drawing::CAnchor *oox_anchor) 
{
	if (oox_anchor == NULL)return;

	_CP_OPT(double) width, height;
	_CP_OPT(double) x, y;

	if (oox_anchor->m_oExtent.IsInit()) //size
	{
		width = oox_anchor->m_oExtent->m_oCx.ToPoints();
		height = oox_anchor->m_oExtent->m_oCy.ToPoints();
	}
	if (oox_anchor->m_oDistL.IsInit())odt_context->drawing_context()->set_margin_left	(oox_anchor->m_oDistL->ToPoints());
	if (oox_anchor->m_oDistT.IsInit())odt_context->drawing_context()->set_margin_top	(oox_anchor->m_oDistT->ToPoints());
	if (oox_anchor->m_oDistR.IsInit())odt_context->drawing_context()->set_margin_right	(oox_anchor->m_oDistR->ToPoints());
	if (oox_anchor->m_oDistB.IsInit())odt_context->drawing_context()->set_margin_bottom	(oox_anchor->m_oDistB->ToPoints());

	odt_context->drawing_context()->set_drawings_rect(x, y, width, height);

	_CP_OPT(int) anchor_type_x, anchor_type_y;

	bool bBackground = oox_anchor->m_oBehindDoc.IsInit() ? oox_anchor->m_oBehindDoc->ToBool(): false;

	bool bThrough = oox_anchor->m_oAllowOverlap.IsInit() ? oox_anchor->m_oAllowOverlap->ToBool(): false;

	if (oox_anchor->m_oPositionV.IsInit() && oox_anchor->m_oPositionV->m_oRelativeFrom.IsInit())
	{
		int vert_rel = oox_anchor->m_oPositionV->m_oRelativeFrom->GetValue();

		odt_context->drawing_context()->set_vertical_rel(vert_rel);

		if ( oox_anchor->m_oPositionV->m_oAlign.IsInit())
			odt_context->drawing_context()->set_vertical_pos(oox_anchor->m_oPositionV->m_oAlign->GetValue());

		else if(oox_anchor->m_oPositionV->m_oPosOffset.IsInit())
		{
			switch(vert_rel)
			{
				case 3:	
				case 6:	anchor_type_y = anchor_type::Paragraph;	break;  
				case 5:	anchor_type_y = anchor_type::Page;		break;       
			}
			odt_context->drawing_context()->set_vertical_pos(oox_anchor->m_oPositionV->m_oPosOffset->ToPoints());
		}
		else
			odt_context->drawing_context()->set_vertical_pos(SimpleTypes::alignvTop);
	}
	if (oox_anchor->m_oPositionH.IsInit() && oox_anchor->m_oPositionH->m_oRelativeFrom.IsInit())
	{
		int horiz_rel = oox_anchor->m_oPositionH->m_oRelativeFrom->GetValue();
		odt_context->drawing_context()->set_horizontal_rel(horiz_rel);
		
		if (oox_anchor->m_oPositionH->m_oAlign.IsInit())
			odt_context->drawing_context()->set_horizontal_pos(oox_anchor->m_oPositionH->m_oAlign->GetValue());
		
		else if(oox_anchor->m_oPositionH->m_oPosOffset.IsInit())
		{
			odt_context->drawing_context()->set_horizontal_pos(oox_anchor->m_oPositionH->m_oPosOffset->ToPoints());
			switch(horiz_rel)
			{
				case 1:
				case 2:	anchor_type_x = anchor_type::Paragraph;	break;  
				case 6:	anchor_type_x = anchor_type::Page;		break;       
			}
		}
		else
			odt_context->drawing_context()->set_horizontal_pos(SimpleTypes::alignhLeft);
	}

	if ( (anchor_type_x && anchor_type_y) && (*anchor_type_x == *anchor_type_y))
		odt_context->drawing_context()->set_anchor(*anchor_type_x);

	bool wrap_set = false;
	if (oox_anchor->m_oWrapSquare.IsInit())
	{
		if (oox_anchor->m_oWrapSquare->m_oWrapText.IsInit() && oox_anchor->m_oWrapSquare->m_oWrapText->GetValue() == SimpleTypes::wraptextLargest)
			odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Dynamic);
		else	
			odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Parallel);

		wrap_set = true;
	}	
	else if (oox_anchor->m_oWrapThrough.IsInit())//style:wrap="run-through" draw:wrap-influence-on-position style:wrap-contour
	{
		odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::RunThrough);
	}
	else if (oox_anchor->m_oWrapTight.IsInit())
	{
		odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Parallel);
		bool bPolygon = oox_anchor->m_oWrapTight->m_oWrapPolygon.IsInit();

		if (oox_anchor->m_oWrapTight->m_oWrapText.IsInit())
		{
			switch(oox_anchor->m_oWrapTight->m_oWrapText->GetValue())
			{
			case SimpleTypes::wraptextBothSides:
			{
				if (bPolygon)
				{
					odt_context->drawing_context()->set_wrap_contour();					
				}
				else
				{
					odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Dynamic);
				}
			}break;
			case SimpleTypes::wraptextLargest:	odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Biggest); break;
			case SimpleTypes::wraptextLeft:		odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Left); break;
			case SimpleTypes::wraptextRight:	odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Right); break;
			}
		}
		else
		{
			odt_context->drawing_context()->set_wrap_contour();
		}
		wrap_set = true;
	}
	else if (oox_anchor->m_oWrapTopAndBottom.IsInit())
	{
		odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Parallel);
		wrap_set = true;
	}
	else if (oox_anchor->m_oWrapNone.IsInit())
	{
		odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::None);
		wrap_set = true;

		if (bThrough)//Silhouette_Project 11-11.docx
		{
			odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::RunThrough);
		}
		if(bBackground)//эффект_штурмовика.docx
		{
			odt_context->drawing_context()->set_object_background(true);
		}
	}
	else if (oox_anchor->m_oAllowOverlap.IsInit())
	{
		odt_context->drawing_context()->set_overlap(oox_anchor->m_oAllowOverlap->ToBool());
	}
	if (oox_anchor->m_oBehindDoc.IsInit() && !wrap_set)
	{
		if (oox_anchor->m_oBehindDoc->ToBool())
		{
			odt_context->drawing_context()->set_object_background(true);
			//odt_context->drawing_context()->set_object_foreground(true);
		}
		else
		{
			//z - order foreground
			odt_context->drawing_context()->set_object_foreground(true);
		}
	}

	if ((oox_anchor->m_oLayoutInCell.IsInit()) && (oox_anchor->m_oLayoutInCell->ToBool()))
	{
		if (odt_context->table_context()->empty() == false)
		{//VillageofSchaumburg.docx
			odt_context->drawing_context()->set_anchor(3); 
			odt_context->drawing_context()->set_vertical_rel(1); //paragraph content
			odt_context->drawing_context()->set_horizontal_rel(1); //paragraph
		}
	}
	if (oox_anchor->m_oRelativeHeight.IsInit())
	{
		int id = oox_anchor->m_oRelativeHeight->GetValue();
		odf_context()->drawing_context()->set_z_order(id);
	}
	
	OoxConverter::convert(oox_anchor->m_oDocPr.GetPointer());
	
	odf_context()->drawing_context()->start_drawing();
		OoxConverter::convert(&oox_anchor->m_oGraphic);
	odf_context()->drawing_context()->end_drawing();

	odf_context()->drawing_context()->check_anchor();
}
void DocxConverter::convert(OOX::Drawing::CInline *oox_inline)
{
	if (oox_inline == NULL)return;

	_CP_OPT(double) width, height;
	_CP_OPT(double) x = 0., y = 0.;
	
	if (oox_inline->m_oExtent.IsInit()) //size
	{
		width = oox_inline->m_oExtent->m_oCx.ToPoints();
		height = oox_inline->m_oExtent->m_oCy.ToPoints();
	}

	odt_context->drawing_context()->set_drawings_rect(x, y, width, height);

	odt_context->drawing_context()->set_anchor(odf_types::anchor_type::AsChar); 
	
	if (oox_inline->m_oDistL.IsInit())	odt_context->drawing_context()->set_margin_left		(oox_inline->m_oDistL->ToPoints());
	if (oox_inline->m_oDistT.IsInit())	odt_context->drawing_context()->set_margin_top		(oox_inline->m_oDistT->ToPoints());
	if (oox_inline->m_oDistR.IsInit())	odt_context->drawing_context()->set_margin_right	(oox_inline->m_oDistR->ToPoints());
	if (oox_inline->m_oDistB.IsInit())	odt_context->drawing_context()->set_margin_bottom	(oox_inline->m_oDistB->ToPoints());

	//вертикальное выравнивание относительно строки поставим в середину (иначе по нижнему краю почемуто)

	odt_context->drawing_context()->set_vertical_rel(2);//line
	odt_context->drawing_context()->set_vertical_pos(1);//middle
	
	OoxConverter::convert(oox_inline->m_oDocPr.GetPointer());
	
	odf_context()->drawing_context()->start_drawing();
		OoxConverter::convert(&oox_inline->m_oGraphic);
	odt_context->drawing_context()->set_anchor(odf_types::anchor_type::AsChar); 
	odf_context()->drawing_context()->end_drawing();
}

void DocxConverter::convert(SimpleTypes::CHexColor			*color,
							SimpleTypes::CThemeColor		*theme_color,
							SimpleTypes::CUcharHexNumber	*theme_tint,
							SimpleTypes::CUcharHexNumber	*theme_shade, _CP_OPT(odf_types::color) & odf_color)
{
	odf_color = boost::none;

	bool result = false;	
	if (color)
	{
		if (color->GetValue() == SimpleTypes::hexcolorAuto)
		{
			return;
		}
		else if (color->GetValue() == SimpleTypes::hexcolorRGB)
		{
			unsigned char ucA = 0, ucR = 0, ucG = 0, ucB = 0;

			ucR = color->Get_R();
			ucB = color->Get_B();
			ucG = color->Get_G();
			ucA = color->Get_A();

			SimpleTypes::CHexColor *oRgbColor = new SimpleTypes::CHexColor(ucR, ucG, ucB);

			if ((oRgbColor) && (oRgbColor->GetValue() == SimpleTypes::hexcolorRGB))
			{
				std::wstring strColor = L"#" + oRgbColor->ToString().substr(2);//.Right(6);

				odf_color = odf_types::color(strColor);
				result = true;
			}
			if (oRgbColor)
				delete oRgbColor;
		}
	}
	if(theme_color && result == false)
	{
		std::map<std::wstring, PPTX::Logic::UniColor>::iterator pFind = docx_document->m_pTheme->themeElements.clrScheme.Scheme.find(theme_color->ToString());

		if (pFind == docx_document->m_pTheme->themeElements.clrScheme.Scheme.end())
		{
			switch (theme_color->GetValue())
			{
			case SimpleTypes::themecolorBackground1:
				pFind = docx_document->m_pTheme->themeElements.clrScheme.Scheme.find(L"lt1");
				break;
			case SimpleTypes::themecolorBackground2:
				pFind = docx_document->m_pTheme->themeElements.clrScheme.Scheme.find(L"lt2");
				break;
			case SimpleTypes::themecolorText1:
				pFind = docx_document->m_pTheme->themeElements.clrScheme.Scheme.find(L"dk1");
				break;
			case SimpleTypes::themecolorText2:
				pFind = docx_document->m_pTheme->themeElements.clrScheme.Scheme.find(L"dk2");
				break;
			}
		}
		if (pFind != docx_document->m_pTheme->themeElements.clrScheme.Scheme.end())
		{
			DWORD argb = pFind->second.GetARGB();

			if (theme_tint || theme_shade)
			{
				double dH, dS, dL;
				RGB2HSL(argb, dH, dS, dL);

				if (theme_shade)
				{
					dL = dL * theme_shade->GetValue() / 255. ;
				}
				if (theme_tint)
				{
					dL = dL * theme_tint->GetValue() / 255. + (1 - theme_tint->GetValue() / 255.);
				}
				argb = HSL2RGB(dH, dS, dL);

			}
			std::wstring strColor = XmlUtils::ToString((unsigned int)(argb & 0x00FFFFFF), L"#%06X");
			odf_color = odf_types::color(strColor);
		}
	}
}
void DocxConverter::convert(ComplexTypes::Word::CColor *color, _CP_OPT(odf_types::color) & odf_color)
{
	if (!color)return;
	convert(color->m_oVal.GetPointer(), color->m_oThemeColor.GetPointer(),color->m_oThemeTint.GetPointer(),color->m_oThemeShade.GetPointer(), odf_color);
}
PPTX::Logic::ClrMap* DocxConverter::oox_clrMap()
{
	//return current_clrMap; todoooo
	if (!docx_document) return NULL;
	if (!docx_document->m_oMain.settings) return NULL;
	
	return docx_document->m_oMain.settings->m_oClrSchemeMapping.GetPointer();
}
void DocxConverter::convert_settings()
{
	if (!odt_context) return;

	OOX::CSettings *settings = docx_document ? docx_document->m_oMain.settings : (docx_flat_document ? docx_flat_document->m_pSettings.GetPointer() : NULL);
	if (!settings) return;

	if (settings->m_oWriteProtection.IsInit())
	{
	}
	if (settings->m_oZoom.IsInit())
	{
	}
	if (settings->m_oMirrorMargins.IsInit())
	{
		odt_context->page_layout_context()->set_pages_mirrored(true);
	}

	odt_context->page_layout_context()->even_and_left_headers_ = settings->m_oEvenAndOddHeaders.IsInit();

	if (settings->m_oPrintTwoOnOne.IsInit())
	{
		if (settings->m_oGutterAtTop.IsInit()){} //portrait
		else {}//landscape
	}

	if (settings->m_oDefaultTabStop.IsInit())
	{
		_CP_OPT(odf_types::length) length;
		convert(settings->m_oDefaultTabStop->m_oVal.GetPointer(), length);
		
		odf_writer::odf_style_state_ptr state;
		if (odt_context->styles_context()->find_odf_default_style_state(odf_types::style_family::Paragraph, state) && state)
		{
			odf_writer::style_paragraph_properties *props =	state->get_paragraph_properties();
			if (props)
				props->content_.style_tab_stop_distance_ = length;
		}

	}
//nullable<ComplexTypes::Word::CLanguage>                       m_oThemeFontLang;
//nullable<OOX::Settings::CEdnDocProps>                         m_oEndnotePr;
//nullable<OOX::Settings::CCaptions>                            m_oCaptions;
//m_oFootnotePr;
}
void DocxConverter::convert_lists_styles()
{
	if (!odt_context) return;

	OOX::CNumbering * lists_styles = docx_document ? docx_document->m_oMain.numbering : docx_flat_document ? docx_flat_document->m_pNumbering.GetPointer() : NULL;
	
	if (!lists_styles)return;

	oox_current_child_document = dynamic_cast<OOX::IFileContainer*>(lists_styles);

	for (size_t i = 0; i < lists_styles->m_arrNum.size(); i++)
	{
		if (lists_styles->m_arrNum[i] == NULL) continue;

		if (lists_styles->m_arrNum[i]->m_oNumId.IsInit() == false) continue;

		int abstr_num = 0;

		if (lists_styles->m_arrNum[i]->m_oAbstractNumId.IsInit() &&  lists_styles->m_arrNum[i]->m_oAbstractNumId->m_oVal.IsInit())
			abstr_num = *lists_styles->m_arrNum[i]->m_oAbstractNumId->m_oVal;			

		std::map<int, size_t>::iterator pFindAbstractNum = lists_styles->m_mapAbstractNum.find(abstr_num);		
		if (pFindAbstractNum == lists_styles->m_mapAbstractNum.end()) continue;
		
		OOX::Numbering::CAbstractNum* num_style = lists_styles->m_arrAbstractNum[pFindAbstractNum->second];
		if (!num_style) continue;

		odt_context->styles_context()->lists_styles().start_style(true, *lists_styles->m_arrNum[i]->m_oNumId);
		for (int j = 0; j < 9; ++j)
		{
			OOX::Numbering::CLvl* pLvl = NULL;
			OOX::Numbering::CNumLvl* pOverrideLvl = NULL;

			std::map<int, size_t>::iterator pFindLvl = num_style->m_mapLvl.find(j);
			std::map<int, size_t>::iterator pFindOverrideLvl = lists_styles->m_arrNum[i]->m_mapLvlOverride.find(j);			

			if (pFindLvl != num_style->m_mapLvl.end())
				pLvl = num_style->m_arrLvl[pFindLvl->second];

			if (pFindOverrideLvl != lists_styles->m_arrNum[i]->m_mapLvlOverride.end())
				pOverrideLvl = lists_styles->m_arrNum[i]->m_arrLvlOverride[pFindOverrideLvl->second];

			convert(pLvl, pOverrideLvl, j);
		}

		odt_context->styles_context()->lists_styles().end_style();
	}
	oox_current_child_document = NULL;
}
void DocxConverter::convert_styles()
{
	if (!odt_context) return;
	
	OOX::CStyles *styles = docx_document ? docx_document->m_oMain.styles : (docx_flat_document ? docx_flat_document->m_pStyles.GetPointer() : NULL);
	if (!styles)return;

	//nullable<OOX::CLatentStyles > m_oLatentStyles;

	convert(styles->m_oDocDefaults.GetPointer(), styles);

	for (size_t i = 0; i < styles->m_arrStyle.size(); i++)
	{
		if (styles->m_arrStyle[i] == NULL) continue;
		
		if (!current_font_size.empty())
		{
			current_font_size.erase(current_font_size.begin() + 1, current_font_size.end());
		}

		convert(styles->m_arrStyle[i]);
	
		//if (i == 0 && styles->m_arrStyle[i]->m_oDefault.IsInit() && styles->m_arrStyle[i]->m_oDefault->ToBool())
		//{
		//	//NADIE_COMO_TU.docx тут дефолтовый стиль не прописан явно, берем тот что Normal

		//}
	}
}

void DocxConverter::convert(OOX::Logic::CHyperlink *oox_hyperlink)
{
	if (oox_hyperlink == NULL)return;

	std::wstring link, location;
	bool bExternal = false;

	if (oox_hyperlink->m_oId.IsInit()) //гиперлинк
	{
		link = find_link_by_id(oox_hyperlink->m_oId->GetValue(), 2, bExternal);
	}
	else if (oox_hyperlink->m_sDestinition.IsInit()) //гиперлинк
	{
		link = *oox_hyperlink->m_sDestinition;
	}
	if (oox_hyperlink->m_sAnchor.IsInit())
	{
		location =  *oox_hyperlink->m_sAnchor;
	}
	if (false == link.empty() || false == location.empty())
	{
		odt_context->start_hyperlink(link, location);
		
        for (size_t i = 0; i < oox_hyperlink->m_arrItems.size(); ++i)
		{
			convert(oox_hyperlink->m_arrItems[i]);
		}
		odt_context->end_hyperlink();
	}
	else
	{
        for (size_t i = 0; i < oox_hyperlink->m_arrItems.size(); ++i)
		{
			convert(oox_hyperlink->m_arrItems[i]);
		}
	}
    //nullable<std::wstring                                      > m_sDocLocation;
	//nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse> > m_oHistory;
    //nullable<std::wstring                                      > m_sTgtFrame;
    //nullable<std::wstring                                      > m_sTooltip;
}

void DocxConverter::convert(OOX::CDocDefaults *def_style, OOX::CStyles *styles)
{
	if (def_style == NULL)return;
	if (styles == NULL)return;

	std::map<SimpleTypes::EStyleType, size_t>::iterator pFindParaDefault = styles->m_mapStyleDefaults.find(SimpleTypes::styletypeParagraph);
	std::map<SimpleTypes::EStyleType, size_t>::iterator pFindRunDefault = styles->m_mapStyleDefaults.find(SimpleTypes::styletypeCharacter);

	if (def_style->m_oParPr.IsInit() || pFindParaDefault != styles->m_mapStyleDefaults.end())
	{
		odt_context->styles_context()->create_default_style(odf_types::style_family::Paragraph);					
		
		odf_writer::style_paragraph_properties	* paragraph_properties	= odt_context->styles_context()->last_state()->get_paragraph_properties();
		odf_writer::style_text_properties		* text_properties		= NULL;

		OOX::Logic::CParagraphProperty paraProps;
		
		if (def_style->m_oParPr.IsInit()) paraProps = paraProps.Merge(paraProps, def_style->m_oParPr.get());

		if (pFindParaDefault != styles->m_mapStyleDefaults.end())
		{
			OOX::CStyle *style = styles->m_arrStyle[pFindParaDefault->second];
			if ((style) && (style->m_oParPr.IsInit()))
			{
				paraProps = paraProps.Merge(paraProps, style->m_oParPr.get());
			}
		}

		convert(&paraProps, paragraph_properties); 
		
		if (def_style->m_oParPr.IsInit() && def_style->m_oParPr->m_oRPr.IsInit())
		{
			text_properties = odt_context->styles_context()->last_state()->get_text_properties();
			convert(def_style->m_oParPr->m_oRPr.GetPointer(), text_properties);
		}
		else if (def_style->m_oRunPr.IsInit())
		{
			text_properties = odt_context->styles_context()->last_state()->get_text_properties();
			convert(def_style->m_oRunPr.GetPointer(), text_properties);
		}
		if (text_properties && !text_properties->content_.fo_font_size_)
		{
			text_properties->content_.fo_font_size_ = odf_types::font_size(odf_types::length(10,odf_types::length::pt));
		}
	}
	
	if (def_style->m_oRunPr.IsInit() || pFindRunDefault != styles->m_mapStyleDefaults.end())
	{
		odt_context->styles_context()->create_default_style(odf_types::style_family::Text);					
		odf_writer::style_text_properties* text_properties = odt_context->styles_context()->last_state()->get_text_properties();

		OOX::Logic::CRunProperty runProps;
		
		if (def_style->m_oRunPr.IsInit()) runProps = runProps.Merge(runProps, def_style->m_oRunPr.get());

		if (pFindRunDefault != styles->m_mapStyleDefaults.end())
		{
			OOX::CStyle *style = styles->m_arrStyle[pFindRunDefault->second];
			if ((style) && (style->m_oRunPr.IsInit()))
			{
				runProps = runProps.Merge(runProps, style->m_oRunPr.get());
			}
		}
		convert(&runProps, text_properties);

	///////на дефолтовый параграф - дефолтовые настройки шрифта
		odf_writer::odf_style_state_ptr def_style_state;

		odf_writer::style_text_properties* para_text_properties = NULL;
		if (odt_context->styles_context()->find_odf_default_style_state(odf_types::style_family::Paragraph, def_style_state) && def_style_state)
		{
			para_text_properties = def_style_state->get_text_properties();
			para_text_properties->apply_from(text_properties);
		}
		else
		{
			odt_context->styles_context()->create_default_style(odf_types::style_family::Paragraph);					
			para_text_properties = odt_context->styles_context()->last_state()->get_text_properties();

			para_text_properties->apply_from(text_properties);
		}
		if (para_text_properties && !para_text_properties->content_.fo_font_size_)
		{
			para_text_properties->content_.fo_font_size_ = odf_types::font_size(odf_types::length(10,odf_types::length::pt));
		}
	}

///////////////////////////////////////////////////////////////////////////

	odt_context->styles_context()->create_default_style(odf_types::style_family::Table);					
	odf_writer::style_table_properties	* table_properties	= odt_context->styles_context()->last_state()->get_table_properties();
	//для красивой отрисовки в редакторах - разрешим объеденить стили пересекающихся обрамлений 
	table_properties->content_.table_border_model_ = odf_types::border_model(odf_types::border_model::Collapsing);

	odt_context->styles_context()->create_default_style(odf_types::style_family::TableRow);					
	odf_writer::style_table_row_properties	* row_properties	= odt_context->styles_context()->last_state()->get_table_row_properties();
	row_properties->style_table_row_properties_attlist_.fo_keep_together_ = odf_types::keep_together(odf_types::keep_together::Auto);

/////////////////////////////////////////////////////////////////////////////////////////////////
	//зачемто ?! для OpenOffice для врезок/фреймов нужен базовый стиль - без него другой тип геометрии oO !!!
	
	odt_context->styles_context()->create_style(L"Frame", odf_types::style_family::Graphic, false, true);		
	odf_writer::graphic_format_properties* graphic_properties = odt_context->styles_context()->last_state()->get_graphic_properties();

	//для image ?!
	odt_context->styles_context()->create_style(L"Graphics", odf_types::style_family::Graphic, false, true);		
	graphic_properties = odt_context->styles_context()->last_state()->get_graphic_properties();
	//if (graphic_properties)graphic_properties->content_.common_background_color_attlist_.fo_background_color_ = odf_types::background_color(odf_types::background_color::Transparent);

}


void DocxConverter::convert(OOX::Numbering::CLvl *oox_num_lvl, OOX::Numbering::CNumLvl *oox_num_lvl_over, int lvl)
{
	if (oox_num_lvl == NULL) return;
	if (oox_num_lvl->m_oIlvl.IsInit() == false) return; 

	OOX::Numbering::CLvl *lvl_over = oox_num_lvl_over ? oox_num_lvl_over->m_oLvl.GetPointer() : NULL;

	int oox_type_list = 48; //none
	if ((oox_num_lvl->m_oNumFmt.IsInit()) && (oox_num_lvl->m_oNumFmt->m_oVal.IsInit()))
	{
		oox_type_list = oox_num_lvl->m_oNumFmt->m_oVal->GetValue();
	}
	if (oox_num_lvl->m_oLvlPicBulletId.IsInit())
		oox_type_list = 1000;

	if (lvl_over)
	{
		if ((lvl_over->m_oNumFmt.IsInit()) && (lvl_over->m_oNumFmt->m_oVal.IsInit()))
		{
			oox_type_list = oox_num_lvl->m_oNumFmt->m_oVal->GetValue();
		}
		if (lvl_over->m_oLvlPicBulletId.IsInit())
			oox_type_list = 1000;
	}

	int type_list = odt_context->styles_context()->lists_styles().start_style_level(*oox_num_lvl->m_oIlvl, oox_type_list );
	if (type_list < 0) return;

	odf_writer::style_list_level_properties			*level_props	= odt_context->styles_context()->lists_styles().get_list_level_properties();
	odf_writer::style_list_level_label_alignment	*aligment_props	= odt_context->styles_context()->lists_styles().get_list_level_alignment_properties();
	
	if (level_props		== NULL)return;
	if (aligment_props	== NULL)return;

	//nullable<SimpleTypes::COnOff<>         > m_oTentative;
	//nullable<SimpleTypes::CLongHexNumber<> > m_oTplc;
    //nullable<ComplexTypes::Word::COnOff2> m_oIsLgl;
	//nullable<ComplexTypes::Word::CLvlLegacy> m_oLegacy;

	double  size_bullet_number_marker = 0;
	if (oox_num_lvl->m_oLvlJc.IsInit())
	{
	}
	if (oox_num_lvl->m_oPPr.IsInit())
	{
		if ((lvl_over) && (lvl_over->m_oPPr.IsInit()))
		{
			OOX::Logic::CParagraphProperty para_props = OOX::Logic::CParagraphProperty::Merge(*oox_num_lvl->m_oPPr, *lvl_over->m_oPPr);

			oox_num_lvl->m_oPPr.reset();
			oox_num_lvl->m_oPPr = new OOX::Logic::CParagraphProperty(para_props);
		}
		if (oox_num_lvl->m_oPPr->m_oTabs.IsInit())
		{
			if ((oox_num_lvl->m_oPPr->m_oTabs->m_arrTabs.size() >0) && (oox_num_lvl->m_oPPr->m_oTabs->m_arrTabs[0]))
			{
				_CP_OPT(odf_types::length) length;
				convert(oox_num_lvl->m_oPPr->m_oTabs->m_arrTabs[0]->m_oPos.GetPointer(), length);
				if (length) aligment_props->text_list_tab_stop_position_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);
			}
		}
		if (oox_num_lvl->m_oPPr->m_oInd.IsInit())
		{
			_CP_OPT(odf_types::length) length_indent;
			
			convert(oox_num_lvl->m_oPPr->m_oInd->m_oHanging.GetPointer(), length_indent);	
			
			if (length_indent) aligment_props->fo_text_indent_ = odf_types::length(-length_indent->get_value_unit(odf_types::length::cm),odf_types::length::cm);//text_min_label_width_

			_CP_OPT(odf_types::length) length_margin;
			if (oox_num_lvl->m_oPPr->m_oInd->m_oStart.IsInit())
			{
				if (oox_num_lvl->m_oPPr->m_oInd->m_oStart.IsInit()) convert(oox_num_lvl->m_oPPr->m_oInd->m_oStart.GetPointer(), length_margin);
				
				if (length_margin) aligment_props->fo_margin_left_ = odf_types::length(length_margin->get_value_unit(odf_types::length::cm),odf_types::length::cm);
			}
			if (oox_num_lvl->m_oPPr->m_oInd->m_oEnd.IsInit())
			{
				convert(oox_num_lvl->m_oPPr->m_oInd->m_oEnd.GetPointer(), length_margin);
			
				if (length_margin) aligment_props->fo_margin_right_  = odf_types::length(length_margin->get_value_unit(odf_types::length::cm),odf_types::length::cm);
			}	
		}
		else
		{
			aligment_props->fo_text_indent_ = odf_types::length(0, odf_types::length::cm);
			aligment_props->fo_margin_left_ = odf_types::length(0, odf_types::length::cm);

		}
	}
	if (oox_num_lvl->m_oRPr.IsInit())//для обозначений списка
	{
		if ((lvl_over) && (lvl_over->m_oRPr.IsInit()))
		{
			OOX::Logic::CRunProperty run_props = OOX::Logic::CRunProperty::Merge(*oox_num_lvl->m_oRPr, *lvl_over->m_oRPr);

			oox_num_lvl->m_oRPr.reset();
			oox_num_lvl->m_oRPr = new OOX::Logic::CRunProperty(run_props);
		}
		odf_writer::odf_style_context* styles_context = odf_context()->page_layout_context()->get_local_styles_context();
		
		odf_writer::style_text_properties *text_props = odt_context->styles_context()->lists_styles().get_text_properties();
		convert(oox_num_lvl->m_oRPr.GetPointer(), text_props, true);

		//create text style for symbols list НА ЛОКАЛЬНОМ контексте - иначе пересечение имен стилей (todoo вытащить генерацию имен в общую часть)
		styles_context->create_style(L"", odf_types::style_family::Text, false, true, -1);					
		odf_writer::odf_style_state_ptr style_state = styles_context->last_state(odf_types::style_family::Text);
		if (style_state)
		{
			odt_context->styles_context()->lists_styles().set_text_style_name( style_state->get_name());		
			
			odf_writer::style_text_properties	* text_props_2 = style_state->get_text_properties();			
			if (text_props_2)text_props_2->apply_from(text_props);
		}

		if ((text_props) && (text_props->content_.fo_font_size_))
		{
			size_bullet_number_marker = text_props->content_.fo_font_size_->get_length().get_value();
		}
	}
	if (size_bullet_number_marker < 0.01 && (type_list == 2 || type_list == 3))
	{
		//выдернем из дефолтного
		odf_writer::odf_style_state_ptr state;
		if ((odf_context()->styles_context()->find_odf_default_style_state(odf_types::style_family::Paragraph,  state)) && (state))
		{
			odf_writer::style_text_properties *text_props = state->get_text_properties();
			if ((text_props) && (text_props->content_.fo_font_size_))
			{
				size_bullet_number_marker = text_props->content_.fo_font_size_->get_length().get_value();
			}
		}
	}
//-------------------------------------------
	int start_value = 1;
	if ((oox_num_lvl->m_oStart.IsInit()) && (oox_num_lvl->m_oStart->m_oVal.IsInit()))
	{
		start_value = *oox_num_lvl->m_oStart->m_oVal;
	}
	if ((lvl_over) && (lvl_over->m_oStart.IsInit()) && (lvl_over->m_oStart->m_oVal.IsInit()))
	{
		start_value = *lvl_over->m_oStart->m_oVal;
	}
	if ((oox_num_lvl_over) && (oox_num_lvl_over->m_oStartOverride.IsInit()) && (oox_num_lvl_over->m_oStartOverride->m_oVal.IsInit()))
	{
		start_value = *oox_num_lvl_over->m_oStartOverride->m_oVal;
	}
	if (start_value > 1)
		odt_context->styles_context()->lists_styles().set_start_number(start_value);
//-------------------------------------------
	if (oox_num_lvl->m_oSuffix.IsInit())
	{
	}
//-------------------------------------------
	std::wstring lvl_text;
	if (oox_num_lvl->m_oLvlText.IsInit() && oox_num_lvl->m_oLvlText->m_sVal.IsInit())
	{
		lvl_text = *oox_num_lvl->m_oLvlText->m_sVal;
	}
	if ((lvl_over) && lvl_over->m_oLvlText.IsInit() && lvl_over->m_oLvlText->m_sVal.IsInit())
	{
		lvl_text = *lvl_over->m_oLvlText->m_sVal;
	}
	if (false == lvl_text.empty())
	{
		if (type_list == 2)			odt_context->styles_context()->lists_styles().set_bullet_char(lvl_text);
		else if (type_list == 1)	odt_context->styles_context()->lists_styles().set_numeric_format(lvl_text);
	}
//-------------------------------------------
	if (oox_num_lvl->m_oLvlPicBulletId.IsInit() && oox_num_lvl->m_oLvlPicBulletId->m_oVal.IsInit())
	{
		int id = *oox_num_lvl->m_oLvlPicBulletId->m_oVal;
		OOX::CNumbering * lists_styles = docx_document->m_oMain.numbering;

		for (size_t i = 0; (lists_styles) && (i < lists_styles->m_arrNumPicBullet.size()); i++)
		{
			if (!lists_styles->m_arrNumPicBullet[i]) continue;
			
			if (false == lists_styles->m_arrNumPicBullet[i]->m_oNumPicBulletId.IsInit()) continue;
			
			if ((lists_styles->m_arrNumPicBullet[i]) && (*lists_styles->m_arrNumPicBullet[i]->m_oNumPicBulletId == id))
			{
				if (lists_styles->m_arrNumPicBullet[i]->m_oDrawing.IsInit())
				{
					convert(lists_styles->m_arrNumPicBullet[i]->m_oDrawing.GetPointer());
				}
				else
				{
					convert(lists_styles->m_arrNumPicBullet[i]->m_oVmlDrawing.GetPointer());
				}
				if (odt_context->mediaitems()->items().size() > 0)
				{
					odt_context->styles_context()->lists_styles().set_bullet_image(odt_context->mediaitems()->items().back().odf_ref);
				}
			}
		}

	}
	if (type_list == 2 || type_list == 3)
	{
		odt_context->styles_context()->lists_styles().set_bullet_image_size(size_bullet_number_marker);
	}
//-------------------------------------------
	int restart_level = 0; //continue
	if (oox_num_lvl->m_oLvlRestart.IsInit() && oox_num_lvl->m_oLvlRestart->m_oVal.IsInit() && type_list == 1)
	{
		restart_level = *oox_num_lvl->m_oLvlRestart->m_oVal;
	}
	if ((lvl_over) && lvl_over->m_oLvlRestart.IsInit() && lvl_over->m_oLvlRestart->m_oVal.IsInit() && type_list == 1)
	{
		restart_level = *lvl_over->m_oLvlRestart->m_oVal;
	}
	if (restart_level > 0)
		odt_context->styles_context()->lists_styles().set_restart_number(restart_level);
//-------------------------------------------
	odt_context->styles_context()->lists_styles().end_style_level();

}
void DocxConverter::convert_table_style(OOX::CStyle *oox_style)
{
	if (oox_style == NULL)return;

	std::wstring oox_name_id = oox_style->m_sStyleId.get_value_or(L"");

	odt_context->styles_context()->table_styles().start_style(oox_name_id);
//общие

	if (oox_style->m_oTblPr.IsInit())
	{
		odf_writer::style_table_properties	*table_properties = odt_context->styles_context()->table_styles().get_table_properties();
		convert(oox_style->m_oTblPr.GetPointer(), table_properties);
		
		//нужно проверить стоит ли отнаследоваться от base_on 
		
		if (oox_style->m_oTblPr->m_oTblBorders.IsInit())
		{
			odf_writer::style_table_cell_properties * table_cell_properties = odt_context->styles_context()->table_styles().get_table_cell_properties();
			convert(oox_style->m_oTblPr->m_oTblBorders.GetPointer(), table_cell_properties);

			std::wstring inside_border;
			if (oox_style->m_oTblPr->m_oTblBorders->m_oInsideH.IsInit()) 
			{
				convert(oox_style->m_oTblPr->m_oTblBorders->m_oInsideH.GetPointer(), inside_border);
				odt_context->styles_context()->table_styles().set_table_insideH(inside_border);
				
			}
			if (oox_style->m_oTblPr->m_oTblBorders->m_oInsideV.IsInit()) 
			{
				convert(oox_style->m_oTblPr->m_oTblBorders->m_oInsideV.GetPointer(), inside_border);
				odt_context->styles_context()->table_styles().set_table_insideV(inside_border);
				
			}
		}
	}
	if (oox_style->m_oRunPr.IsInit())
	{
		odf_writer::style_text_properties	*text_properties = odt_context->styles_context()->table_styles().get_text_properties();
		convert(oox_style->m_oRunPr.GetPointer(), text_properties);
	}
	if (oox_style->m_oParPr.IsInit())
	{
		odf_writer::style_paragraph_properties	*paragraph_properties = odt_context->styles_context()->table_styles().get_paragraph_properties();
		convert(oox_style->m_oParPr.GetPointer(), paragraph_properties);
	}

	if (oox_style->m_oTcPr.IsInit())
	{
		odf_writer::style_table_cell_properties	* table_cell_properties = odt_context->styles_context()->table_styles().get_table_cell_properties();
		convert(oox_style->m_oTcPr.GetPointer(), table_cell_properties);
	}	
	//if (oox_style->m_oTrPr.IsInit())
	//{
	//	odf_writer::style_table_row_properties	* table_row_properties = odt_context->styles_context()->table_styles().get_table_row_properties();
	//	convert(oox_style->m_oTrPr.GetPointer(), table_row_properties);
	//}
//отдельные
	for (size_t i = 0; i < oox_style->m_arrTblStylePr.size() ; i++)
	{
		if (oox_style->m_arrTblStylePr[i] == NULL) continue;

		if (oox_style->m_arrTblStylePr[i]->m_oType.IsInit() == false) continue;
		switch (oox_style->m_arrTblStylePr[i]->m_oType->GetValue())
		{
		case SimpleTypes::tblstyleoverridetypeBand1Horz : odt_context->styles_context()->table_styles().add_band1Horz();	break;
		case SimpleTypes::tblstyleoverridetypeBand1Vert : odt_context->styles_context()->table_styles().add_band1Vert();	break;
		case SimpleTypes::tblstyleoverridetypeBand2Horz : odt_context->styles_context()->table_styles().add_band2Horz();	break;
		case SimpleTypes::tblstyleoverridetypeBand2Vert : odt_context->styles_context()->table_styles().add_band2Vert();	break;
		case SimpleTypes::tblstyleoverridetypeFirstCol	: odt_context->styles_context()->table_styles().add_firstCol();		break;
		case SimpleTypes::tblstyleoverridetypeFirstRow	: odt_context->styles_context()->table_styles().add_firstRow();		break;
		case SimpleTypes::tblstyleoverridetypeLastCol	: odt_context->styles_context()->table_styles().add_lastCol();		break;
		case SimpleTypes::tblstyleoverridetypeLastRow	: odt_context->styles_context()->table_styles().add_lastRow();		break;
		case SimpleTypes::tblstyleoverridetypeNeCell	: odt_context->styles_context()->table_styles().add_neCell();		break;
		case SimpleTypes::tblstyleoverridetypeNwCell	: odt_context->styles_context()->table_styles().add_nwCell();		break;
		case SimpleTypes::tblstyleoverridetypeSeCell	: odt_context->styles_context()->table_styles().add_seCell();		break;
		case SimpleTypes::tblstyleoverridetypeSwCell	: odt_context->styles_context()->table_styles().add_swCell();		break;
		case SimpleTypes::tblstyleoverridetypeWholeTable : odt_context->styles_context()->table_styles().add_wholeTable();	break;
		}
		//сначела отнаследоваться от общих настроек???
		convert(oox_style->m_arrTblStylePr[i]->m_oTcPr.GetPointer(), odt_context->styles_context()->table_styles().get_table_cell_properties());
		convert(oox_style->m_arrTblStylePr[i]->m_oRunPr.GetPointer(),odt_context->styles_context()->table_styles().get_text_properties());
		convert(oox_style->m_arrTblStylePr[i]->m_oParPr.GetPointer(),odt_context->styles_context()->table_styles().get_paragraph_properties());

			//nullable<OOX::Logic::CTableProperty      >      m_oTblPr;
			//nullable<OOX::Logic::CTableRowProperties >      m_oTrPr;
	}

	odt_context->styles_context()->table_styles().end_style();

}
void DocxConverter::convert(OOX::CStyle	*oox_style)
{
	if (oox_style == NULL)return;

	if (oox_style->m_oType.IsInit() == false)return;
	odf_types::style_family::type family = odf_types::style_family::None;

	if ( SimpleTypes::styletypeNumbering == oox_style->m_oType->GetValue())
	{
		//????
		return;
	}
	
	if ( SimpleTypes::styletypeTable == oox_style->m_oType->GetValue())
	{
		convert_table_style(oox_style);
		return;
	}
	
	std::wstring oox_name_id = oox_style->m_sStyleId.get_value_or(L"");
	bool bDefault = oox_style->m_oDefault.IsInit() && oox_style->m_oDefault->ToBool();

	switch(oox_style->m_oType->GetValue())
	{
		case SimpleTypes::styletypeCharacter : family = odf_types::style_family::Text;		break;
		case SimpleTypes::styletypeParagraph : family = odf_types::style_family::Paragraph;	break;
		default:  
			return;
	}
	if (bDefault && family == odf_types::style_family::Paragraph && oox_name_id != L"Standart")
	{
		//todooo ???
		//odt_context->sRenamedStyle = oox_name_id;
		//oox_name_id = L"Standart";
	}

	odt_context->styles_context()->create_style(oox_name_id, family, false, true, -1);

	std::wstring style_name;
	if (oox_style->m_oName.IsInit() && oox_style->m_oName->m_sVal.IsInit()) 
	{
		style_name = *oox_style->m_oName->m_sVal;
		odt_context->styles_context()->last_state()->set_display_name(style_name);
	}
	if (bDefault)
	{
		odt_context->styles_context()->last_state()->set_class(L"default");
	}
	odf_writer::style_text_properties* text_properties = NULL;
	if (oox_style->m_oRunPr.IsInit())
	{
		text_properties = odt_context->styles_context()->last_state()->get_text_properties();
	
		if (bDefault)
		{
			odf_writer::odf_style_state_ptr def_style_state;
			if (odt_context->styles_context()->find_odf_default_style_state(odf_types::style_family::Paragraph, def_style_state) && def_style_state)
			{//??
				odf_writer::style_text_properties * props = def_style_state->get_text_properties();
				text_properties->apply_from(props);
			}
		}

		convert(oox_style->m_oRunPr.GetPointer(), text_properties);
	}
	if (oox_style->m_oParPr.IsInit())
	{
		odf_writer::style_paragraph_properties	*paragraph_properties = odt_context->styles_context()->last_state()->get_paragraph_properties();
		if (oox_style->m_oDefault.IsInit() && oox_style->m_oDefault->ToBool())
		{
			odf_writer::odf_style_state_ptr def_style_state;
			if (odt_context->styles_context()->find_odf_default_style_state(odf_types::style_family::Paragraph, def_style_state) && def_style_state)
			{//??
				odf_writer::style_paragraph_properties *props = def_style_state->get_paragraph_properties();
				paragraph_properties->apply_from(props);
			}
		}

		convert(oox_style->m_oParPr.GetPointer(), paragraph_properties);

		if (oox_style->m_oParPr->m_oNumPr.IsInit())
		{
			int level = (oox_style->m_oParPr->m_oNumPr->m_oIlvl.IsInit() && oox_style->m_oParPr->m_oNumPr->m_oIlvl->m_oVal.IsInit()) ?
				*oox_style->m_oParPr->m_oNumPr->m_oIlvl->m_oVal : -1;
			
			int id = (oox_style->m_oParPr->m_oNumPr->m_oNumID.IsInit() && oox_style->m_oParPr->m_oNumPr->m_oNumID->m_oVal.IsInit()) ? 
				*oox_style->m_oParPr->m_oNumPr->m_oNumID->m_oVal : -1;

			if (level < 0 && id >= 0) 
				level = 0;
			if (level >= 0 || id >= 0)
			{
				odt_context->styles_context()->last_state()->set_list_style_level(level);
				odt_context->styles_context()->last_state()->set_list_style_id(id);
				odt_context->styles_context()->last_state()->set_list_style_exist(true);
		
				if (id >= 0)
				{
					std::wstring list_style_name = odt_context->styles_context()->lists_styles().get_style_name(id);
					odt_context->styles_context()->last_state()->set_list_style_name(list_style_name);
				}
			}
		}
	}
	if (oox_style->m_oBasedOn.IsInit() && oox_style->m_oBasedOn->m_sVal.IsInit())
		odt_context->styles_context()->last_state()->set_parent_style_name(*oox_style->m_oBasedOn->m_sVal);

    //nullable<ComplexTypes::Word::COnOff2> m_oQFormat;
	//nullable<ComplexTypes::Word::std::wstring_>					m_oAliases;
//-------------------------------------------------------------------------------------------------------------------------
	if (style_name == L"Hyperlink")
	{
		odt_context->styles_context()->create_style(L"Internet_20_link", family, false, true, -1); 
		//odt_context->styles_context()->last_state()->set_parent_style_name(oox_name_id);
		odt_context->styles_context()->last_state()->set_display_name(L"Internet link");
		
		odf_writer::style_text_properties* hyperlink_text_props = odt_context->styles_context()->last_state()->get_text_properties();
		hyperlink_text_props->apply_from(text_properties);
		
	}
}

void DocxConverter::convert(OOX::Logic::CCommentRangeStart* oox_comm_start)
{
	if(oox_comm_start == NULL)return;
	if (oox_comm_start->m_oId.IsInit() == false) return;

	int oox_comm_id = oox_comm_start->m_oId->GetValue();

	int state = odt_context->start_comment(oox_comm_id);

	if (state > 0)
	{
		convert_comment(oox_comm_id);

		//if (state == 2)
		//{
		//	odt_context->text_context()->end_element();
		//}
	}
}

void DocxConverter::convert(OOX::Logic::CCommentRangeEnd* oox_comm_end)
{
	if(oox_comm_end == NULL)return;
	if (oox_comm_end->m_oId.IsInit() == false) return;

	int oox_comm_id = oox_comm_end->m_oId->GetValue();

	 odt_context->end_comment(oox_comm_id);
}
void DocxConverter::convert(OOX::CComment* oox_comment)
{
	if (oox_comment == NULL) return;
	if (oox_comment->m_oId.IsInit() == false) return;
	
	int oox_comm_id = oox_comment->m_oId->GetValue();
	
	odt_context->start_comment_content();
	{
		if (oox_comment->m_oAuthor.IsInit())	odt_context->comment_context()->set_author	(*oox_comment->m_oAuthor);
		if (oox_comment->m_oDate.IsInit())		odt_context->comment_context()->set_date	(oox_comment->m_oDate->GetValue());
		if (oox_comment->m_oInitials.IsInit())	{}

		for (size_t i = 0; i < oox_comment->m_arrItems.size(); ++i)
		{
			convert(oox_comment->m_arrItems[i]);
		}
	}
	odt_context->end_comment_content();

}
void DocxConverter::convert(OOX::Logic::CCommentReference* oox_comm_ref)
{
	if (oox_comm_ref == NULL)					return;
	if (oox_comm_ref->m_oId.IsInit() == false)	return;

	int oox_comm_id = oox_comm_ref->m_oId->GetValue();

	int state = odt_context->start_comment(oox_comm_id);

	if (state > 0) // типо тока стартанул
	{
		//значит старт тута а не по RangeStart
		convert_comment(oox_comm_id);

		if (state == 2)
		{
			odt_context->text_context()->end_element();
		}
	}
}
void DocxConverter::convert(OOX::Logic::CFootnoteReference* oox_ref)
{
	if (oox_ref == NULL)					return;
	if (oox_ref->m_oId.IsInit() == false)	return;

	int oox_ref_id = oox_ref->m_oId->GetValue();

	convert_footnote(oox_ref_id);
}
void DocxConverter::convert(OOX::Logic::CEndnoteReference* oox_ref)
{
	if (oox_ref == NULL)					return;
	if (oox_ref->m_oId.IsInit() == false)	return;

	int oox_ref_id = oox_ref->m_oId->GetValue();

	convert_endnote(oox_ref_id);
}
void DocxConverter::convert_comment(int oox_comm_id)
{
	bool bContent = false;

	OOX::CComments *pComments = NULL;

	if (docx_document) 
	{
		pComments = docx_document->m_oMain.comments;
	}
	else if (docx_flat_document)
	{
		pComments = docx_flat_document->m_pComments.GetPointer();
	}

	if (!pComments) return;
	
	OOX::IFileContainer* back_document = oox_current_child_document;
	oox_current_child_document = dynamic_cast<OOX::IFileContainer*>(pComments);

	std::map<int, int>::iterator pFind = pComments->m_mapComments.find(oox_comm_id);

	if (pFind != pComments->m_mapComments.end())
	{
		if ( pFind->second < (int)pComments->m_arrComments.size() && pFind->second >= 0)
		{
			OOX::CComment* oox_comment = pComments->m_arrComments[pFind->second];
			
			if (oox_comment != NULL)
			{				
				odt_context->start_comment_content();
				{
					if (oox_comment->m_oAuthor.IsInit())	odt_context->comment_context()->set_author	(*oox_comment->m_oAuthor);
					if (oox_comment->m_oDate.IsInit())		odt_context->comment_context()->set_date	(oox_comment->m_oDate->GetValue());
					if (oox_comment->m_oInitials.IsInit())	{}

					for (size_t i = 0; i < oox_comment->m_arrItems.size(); ++i)
					{
						convert(oox_comment->m_arrItems[i]);
					}
				}
				odt_context->end_comment_content();
			}
		}
	}
	oox_current_child_document = back_document;
}
void DocxConverter::convert_footnote(int oox_ref_id)
{
	OOX::CFootnotes *oox_footnotes = NULL;
	
	if (docx_document)
	{
		oox_footnotes = docx_document->m_oMain.footnotes;
	}
	else if (docx_flat_document)
	{
		oox_footnotes = docx_flat_document->m_pFootnotes.GetPointer();
	}
	if (oox_footnotes == NULL ) return;

	OOX::IFileContainer* back_document = oox_current_child_document;
	oox_current_child_document = dynamic_cast<OOX::IFileContainer*>(oox_footnotes);

	odt_context->start_note(oox_ref_id, 1);

	std::map<int, OOX::CFtnEdn*>::iterator pFind = oox_footnotes->m_mapFootnote.find(oox_ref_id);

	if (pFind != oox_footnotes->m_mapFootnote.end())
	{
		OOX::CFtnEdn* oox_note = pFind->second;

		odt_context->start_note_content();
		{
            for (size_t i = 0; i < oox_note->m_arrItems.size(); ++i)
			{
				convert(oox_note->m_arrItems[i]);
			}
		}
		odt_context->end_note_content();
	}
	odt_context->end_note();
	
	oox_current_child_document = back_document;
}
void DocxConverter::convert_endnote(int oox_ref_id)
{
	OOX::CEndnotes *oox_endnotes = NULL;
	
	if (docx_document)
	{
		oox_endnotes = docx_document->m_oMain.endnotes;
	}
	else if (docx_flat_document)
	{
		oox_endnotes = docx_flat_document->m_pEndnotes.GetPointer();
	}
	if (oox_endnotes == NULL ) return;

	OOX::IFileContainer* back_document = oox_current_child_document;
	oox_current_child_document = dynamic_cast<OOX::IFileContainer*>(oox_endnotes);

	odt_context->start_note(oox_ref_id, 2);

	std::map<int, OOX::CFtnEdn*>::iterator pFind = oox_endnotes->m_mapEndnote.find(oox_ref_id);

	if (pFind != oox_endnotes->m_mapEndnote.end())
	{
		OOX::CFtnEdn* oox_note = pFind->second;

		odt_context->start_note_content();
		{
            for (size_t i = 0; i < oox_note->m_arrItems.size(); ++i)
			{
				convert(oox_note->m_arrItems[i]);
			}
		}
		odt_context->end_note_content();
	}
	odt_context->end_note();

	oox_current_child_document = back_document;
}
void DocxConverter::convert_hdr_ftr(std::wstring sId)
{
	OOX::CHdrFtr *oox_hdr_ftr = NULL;
	
	if (docx_document)
	{
		oox_hdr_ftr = docx_document->GetHeaderOrFooter(sId);
	}
	else if (docx_flat_document)
	{
		oox_hdr_ftr = docx_flat_document->GetHeaderOrFooter(sId);
	}
	if (oox_hdr_ftr == NULL ) return;

	OOX::IFileContainer* back_document = oox_current_child_document;
	oox_current_child_document = dynamic_cast<OOX::IFileContainer*>(oox_hdr_ftr);

    for (size_t i = 0; i < oox_hdr_ftr->m_arrItems.size(); ++i)
	{
		convert(oox_hdr_ftr->m_arrItems[i]);
	}
	oox_current_child_document  = back_document;
}

void DocxConverter::convert(OOX::Logic::CTbl *oox_table)
{
	if (oox_table == NULL) return;

	if (odt_context->text_context()->get_KeepNextParagraph())
	{
		odt_context->end_paragraph();
		odt_context->text_context()->set_KeepNextParagraph(false);
	}

	//if (in_list, but not in paragraph)
	odt_context->set_no_list();

	bool styled_table			= false;
	bool in_frame				= false;
	int  in_frame_anchor		= odf_types::anchor_type::Paragraph;
	int	 id_change_properties	= -1;
	
	if (oox_table->m_oTableProperties)
		id_change_properties = convert(oox_table->m_oTableProperties->m_oTblPrChange.GetPointer());


	if (oox_table->m_oTableProperties && (oox_table->m_oTableProperties->m_oTblStyle.IsInit() && oox_table->m_oTableProperties->m_oTblStyle->m_sVal.IsInit()))
	{//настройка предустановленного стиля
		std::wstring base_style_name = *oox_table->m_oTableProperties->m_oTblStyle->m_sVal;

		styled_table = odt_context->styles_context()->table_styles().start_table(base_style_name);
	}

	convert(oox_table->m_oTableProperties, styled_table );	
	
	if(oox_table->m_oTableProperties && oox_table->m_oTableProperties->m_oTblpPr.IsInit())
	{
		if (oox_table->m_oTableProperties->m_oTblpPr->m_oTblpX.IsInit()) 
		{
			if (oox_table->m_oTableProperties->m_oTblpPr->m_oHorzAnchor.IsInit() && 
				oox_table->m_oTableProperties->m_oTblpPr->m_oHorzAnchor->GetValue() == SimpleTypes::hanchorText)
				in_frame = false;
			else 
				in_frame = true;
		}
		if (oox_table->m_oTableProperties->m_oTblpPr->m_oTblpY.IsInit())
		{
			in_frame = true;
			if (oox_table->m_oTableProperties->m_oTblpPr->m_oVertAnchor.IsInit() && 
				oox_table->m_oTableProperties->m_oTblpPr->m_oVertAnchor->GetValue() == SimpleTypes::vanchorText)
			{
				//in_frame_anchor	= odf_types::anchor_type::Char;
			}
		}
	}

	if (in_frame)
	{
		if (current_section_properties)
			convert(current_section_properties->props, !current_section_properties->bContinue);
		
		odt_context->start_paragraph();

		if (odt_context->is_paragraph_in_current_section_)
		{
			odt_context->set_master_page_name(odt_context->page_layout_context()->last_master() ?
								  odt_context->page_layout_context()->last_master()->get_name() : L"");
		}
			odt_context->start_drawing_context();
				_CP_OPT(double) width, height, x, y ;
				
				if (oox_table->m_oTableProperties->m_oTblpPr->m_oTblpXSpec.IsInit())
				{
					odt_context->drawing_context()->set_horizontal_pos(oox_table->m_oTableProperties->m_oTblpPr->m_oTblpXSpec->GetValue());
				}
				bool bRightAlignX = ((oox_table->m_oTableProperties->m_oTblpPr->m_oTblpXSpec.IsInit()) && 
					(oox_table->m_oTableProperties->m_oTblpPr->m_oTblpXSpec->GetValue() == SimpleTypes::xalignRight));
				
				if (!bRightAlignX && oox_table->m_oTableProperties->m_oTblpPr->m_oTblpX.IsInit())
					x = oox_table->m_oTableProperties->m_oTblpPr->m_oTblpX->ToPoints();
				if (oox_table->m_oTableProperties->m_oTblpPr->m_oTblpY.IsInit())
					y = oox_table->m_oTableProperties->m_oTblpPr->m_oTblpY->ToPoints();

				//if (oox_table->m_oTableProperties->m_oTblpPr->m_oLeftFromText.IsInit())
				//{
				//	if (!x) x = oox_table->m_oTableProperties->m_oTblpPr->m_oLeftFromText->ToPoints();
				//	else x = -*x + oox_table->m_oTableProperties->m_oTblpPr->m_oLeftFromText->ToPoints();
				//}


				odt_context->drawing_context()->set_drawings_rect(x, y, width, height);	
				
				odt_context->drawing_context()->set_anchor(in_frame_anchor); 
				odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Dynamic);

				if (oox_table->m_oTableProperties->m_oTblpPr->m_oVertAnchor.IsInit())
				{
					switch(oox_table->m_oTableProperties->m_oTblpPr->m_oVertAnchor->GetValue())
					{
						case SimpleTypes::vanchorMargin: 
						{	odt_context->drawing_context()->set_anchor(anchor_type::Paragraph);
							odt_context->drawing_context()->set_vertical_rel(6); //1???
						}break;
						case SimpleTypes::vanchorPage:	
						{	odt_context->drawing_context()->set_anchor(anchor_type::Page);
							odt_context->drawing_context()->set_vertical_rel(5); 
						}break;
					}
				}
				if (oox_table->m_oTableProperties->m_oTblpPr->m_oHorzAnchor.IsInit())
				{
					switch(oox_table->m_oTableProperties->m_oTblpPr->m_oHorzAnchor->GetValue())
					{
						case SimpleTypes::hanchorMargin:	odt_context->drawing_context()->set_horizontal_rel(8); break;
						case SimpleTypes::hanchorPage:		odt_context->drawing_context()->set_horizontal_rel(6); break;
					}
				}		

				if (oox_table->m_oTableProperties->m_oTblpPr->m_oRightFromText.IsInit())
				{
					if (!bRightAlignX && !x) //x = *x + oox_table->m_oTableProperties->m_oTblpPr->m_oRightFromText->ToPoints();
					{
						x = oox_table->m_oTableProperties->m_oTblpPr->m_oRightFromText->ToPoints();
						odt_context->drawing_context()->set_horizontal_pos(*x);
					}
				}
				if (oox_table->m_oTableProperties->m_oTblpPr->m_oTopFromText.IsInit())
				{
					if (!y) //y = *y + oox_table->m_oTableProperties->m_oTblpPr->m_oTopFromText->ToPoints();
						y = oox_table->m_oTableProperties->m_oTblpPr->m_oTopFromText->ToPoints();
					
					odt_context->drawing_context()->set_vertical_pos(*y);
				}
				odt_context->drawing_context()->start_drawing();	

				odt_context->drawing_context()->start_text_box();
					odt_context->drawing_context()->set_text_box_tableframe(true);
					odt_context->drawing_context()->set_text_box_min_size(0, 1.);
					odt_context->drawing_context()->set_z_order(0x7fffffff - 1024/* + id_tables*/);
					odt_context->drawing_context()->set_parent_style(L"Frame");
					odt_context->drawing_context()->set_name(L"TableFrame");
				odt_context->start_text_context();
	}

	odt_context->start_table(true); 

	_CP_OPT(std::wstring) border_inside_h, border_inside_v;

	if (styled_table)
	{
		odt_context->table_context()->set_table_styled(styled_table);
		
		if (oox_table->m_oTableProperties->m_oTblLook.IsInit() && oox_table->m_oTableProperties->m_oTblLook->m_oVal.IsInit())
			odt_context->styles_context()->table_styles().set_flags(oox_table->m_oTableProperties->m_oTblLook->m_oVal->GetValue());

		
		border_inside_h = odt_context->styles_context()->table_styles().get_table_insideH();
		border_inside_v = odt_context->styles_context()->table_styles().get_table_insideV();
	}
	if (oox_table->m_oTableProperties && oox_table->m_oTableProperties->m_oTblBorders.IsInit())
	{
		std::wstring ins_b;
		if (oox_table->m_oTableProperties && oox_table->m_oTableProperties->m_oTblBorders->m_oInsideH.IsInit())
		{
			convert(oox_table->m_oTableProperties->m_oTblBorders->m_oInsideH.GetPointer(), ins_b);
			border_inside_h = ins_b;
		}
		if (oox_table->m_oTableProperties && oox_table->m_oTableProperties->m_oTblBorders->m_oInsideV.IsInit())
		{
			convert(oox_table->m_oTableProperties->m_oTblBorders->m_oInsideV.GetPointer(), ins_b);
			border_inside_v = ins_b;
		}
	}
	odt_context->table_context()->set_table_inside_v(border_inside_v);
	odt_context->table_context()->set_table_inside_h(border_inside_h);

	int count_rows		= oox_table->m_nCountRow;
	int count_columns	= oox_table->m_oTblGrid.IsInit() ? (int)oox_table->m_oTblGrid->m_arrGridCol.size() : 0;

	odt_context->styles_context()->table_styles().set_current_dimension(count_columns, count_rows);
	odt_context->table_context()->set_table_size(count_columns, count_rows);
//------ колонки
	convert(oox_table->m_oTblGrid.GetPointer());

//------ строки
    for (size_t i = 0; i < oox_table->m_arrItems.size(); ++i)
	{
		switch(oox_table->m_arrItems[i]->getType())
		{
			case OOX::et_w_tblPr:	
			{
				//skip 
			}break;
			default:				
				convert(oox_table->m_arrItems[i]);
		}
	}
	if (in_frame)
	{
		_CP_OPT(double) width = odt_context->table_context()->get_table_width();
		_CP_OPT(double) height = odt_context->table_context()->get_table_height();
		
		odt_context->drawing_context()->set_size(width, height);

		odt_context->drawing_context()->set_text_box_min_size(-1, height.get_value_or(0));
	}
	
	odt_context->end_table();

	if (odt_context->table_context()->is_styled())
		odt_context->styles_context()->table_styles().end_table();

	if (in_frame)
	{
		odt_context->drawing_context()->set_text( odt_context->text_context());
		odt_context->end_text_context();
		
		odt_context->drawing_context()->end_text_box();
		odt_context->drawing_context()->end_drawing();
		odt_context->end_drawing_context();
		
		odt_context->text_context()->set_KeepNextParagraph(true);
	}	

	if (id_change_properties >= 0)
		odt_context->end_change(id_change_properties, 3); 
}

void DocxConverter::convert(OOX::Logic::CTblGrid	*oox_table_grid)
{
	if (oox_table_grid == NULL) return;

	int id_change_properties = convert(oox_table_grid->m_oTblGridChange.GetPointer());

	odt_context->start_table_columns();

	for (size_t i =0 ; i < oox_table_grid->m_arrGridCol.size(); i++)
	{
		if (oox_table_grid->m_arrGridCol[i] == NULL) continue;
		double width = -1;

		if (oox_table_grid->m_arrGridCol[i]->m_oW.IsInit())
		{
			width = oox_table_grid->m_arrGridCol[i]->m_oW->ToPoints();
		}

		odt_context->add_table_column(width);

		if (width > 0 && width < 5) //2222010_53102Reader final.docx  - настройка через ячейки
		{
			//odt_context->table_context()->set_column_optimal(true);
			//2222010_53102Reader final.docx 
		}
			
	}	
	odt_context->end_table_columns();

	if (id_change_properties >= 0)
		odt_context->end_change(id_change_properties, 3); 
}

int DocxConverter::convert(OOX::Logic::CTblGridChange *oox_table_grid_prop_change)
{
	if (!oox_table_grid_prop_change) return -1;

	int				id		= oox_table_grid_prop_change->m_oId.IsInit()		? oox_table_grid_prop_change->m_oId->GetValue()		: -1;
	//std::wstring	author	= oox_table_grid_prop_change->m_sAuthor.IsInit()	? oox_table_grid_prop_change->m_sAuthor.get2()		: L"";
	//std::wstring	userId	= oox_table_grid_prop_change->m_sUserId.IsInit()	? oox_table_grid_prop_change->m_sUserId.get2()		: L"";
	//std::wstring	date	= oox_table_grid_prop_change->m_oDate.IsInit()		? oox_table_grid_prop_change->m_oDate->GetValue()	: L"";

	/// ( нету в либре подходящей схемы
	//if (!odt_context->start_change(id, 3, author, userId, date, style_name)) return -1;
	//return id;
	return -1;
}
	
void DocxConverter::convert(OOX::Logic::CTr	*oox_table_row)
{
	if (oox_table_row == NULL) return;

	bool styled = oox_table_row->m_pTableRowProperties ? true : false;
	bool is_header = false;

	int id_insert_row	= -1;
	int id_delete_row	= -1;

	if (oox_table_row->m_pTableRowProperties)
	{//+ Format ???
		//id_insert_row = convert(oox_table_row->m_pTableRowProperties->m_oIns.GetPointer(), 1);
		//id_delete_row = convert(oox_table_row->m_pTableRowProperties->m_oDel.GetPointer(), 2); 
		//??? как в электороных таблицах? или ваще нету?
	}
		
	if (styled && oox_table_row->m_pTableRowProperties->m_oTblHeader.IsInit()
		&& oox_table_row->m_pTableRowProperties->m_oTblHeader->m_oVal.ToBool() )is_header = true;

	odt_context->start_table_row(styled, is_header);
	
	convert(oox_table_row->m_pTableRowProperties);

    for (size_t i =0; i <  oox_table_row->m_arrItems.size(); ++i)
	{
		switch(oox_table_row->m_arrItems[i]->getType())
		{
			case OOX::et_w_trPr:	
			{
				//skip
			}break;
			default:
				convert(oox_table_row->m_arrItems[i]);
		}
	}	
	odt_context->end_table_row();

	if (id_insert_row >= 0)
		odt_context->end_change(id_insert_row, 1);

	if (id_delete_row >= 0)
		odt_context->end_change(id_delete_row, 2);
}
void DocxConverter::convert(OOX::Logic::CTc	*oox_table_cell)
{
	if (oox_table_cell == NULL) return;

	bool covered				= false;
	int id_change_properties	= -1;

	if (oox_table_cell->m_pTableCellProperties)
	{
		//id_change_properties = convert(oox_table_cell->m_pTableCellProperties->m_oCellIns.GetPointer()); ?? 
		//id_change_properties = convert(oox_table_cell->m_pTableCellProperties->m_oCellDel.GetPointer()); ??
		//id_change_properties = convert(oox_table_cell->m_pTableCellProperties->m_oTcPrChange.GetPointer());
		//??? как в электороных таблицах? или ваще нету?

		if (oox_table_cell->m_pTableCellProperties->m_oVMerge.IsInit())
		{
			if (!(oox_table_cell->m_pTableCellProperties->m_oVMerge->m_oVal.IsInit() && 
					oox_table_cell->m_pTableCellProperties->m_oVMerge->m_oVal->GetValue() == SimpleTypes::mergeRestart))
				covered = true; 
		}
		if (oox_table_cell->m_pTableCellProperties->m_oHMerge.IsInit())
		{
			if (!(oox_table_cell->m_pTableCellProperties->m_oHMerge->m_oVal.IsInit() && 
					oox_table_cell->m_pTableCellProperties->m_oHMerge->m_oVal->GetValue() == SimpleTypes::mergeRestart))
				covered = true; 
		}
	}

	odt_context->start_table_cell( oox_table_cell->m_nNumCol, covered, convert(oox_table_cell->m_pTableCellProperties, oox_table_cell->m_nNumCol + 1));
	
	if (oox_table_cell->m_pTableCellProperties)
	{
		if (oox_table_cell->m_pTableCellProperties->m_oVMerge.IsInit())
		{
			if (oox_table_cell->m_pTableCellProperties->m_oVMerge->m_oVal.IsInit() && 
					oox_table_cell->m_pTableCellProperties->m_oVMerge->m_oVal->GetValue() == SimpleTypes::mergeRestart)
				odt_context->table_context()->set_cell_row_span_restart();
			else 
				odt_context->table_context()->set_cell_row_span();
		}
		if (oox_table_cell->m_pTableCellProperties->m_oGridSpan.IsInit() && oox_table_cell->m_pTableCellProperties->m_oGridSpan->m_oVal.IsInit())
		{
			odt_context->table_context()->set_cell_column_span(*oox_table_cell->m_pTableCellProperties->m_oGridSpan->m_oVal);
		}
		else if ((oox_table_cell->m_pTableCellProperties->m_oHMerge.IsInit()) &&
				 (oox_table_cell->m_pTableCellProperties->m_oHMerge->m_oVal.IsInit()))
		{
			if (oox_table_cell->m_pTableCellProperties->m_oHMerge->m_oVal->GetValue() == SimpleTypes::mergeRestart)
				odt_context->table_context()->set_cell_column_span_restart();
			else
				odt_context->table_context()->set_cell_column_span();
		}

	}

    for (size_t i = 0; i < oox_table_cell->m_arrItems.size(); ++i)
	{
		switch(oox_table_cell->m_arrItems[i]->getType())
		{
			case OOX::et_w_tcPr:	
			{
				//skip
			}break;
			default:
				convert(oox_table_cell->m_arrItems[i]);
		}
	}		
	odt_context->end_table_cell();

	if (id_change_properties >= 0)
		odt_context->end_change(id_change_properties, 3);
}
bool DocxConverter::convert(OOX::Logic::CTableProperty *oox_table_pr, odf_writer::style_table_properties *table_properties )
{
	if (oox_table_pr == NULL) return false;
	if (table_properties == NULL) return false;

	if (oox_table_pr->m_oTblW.IsInit() && oox_table_pr->m_oTblW->m_oW.IsInit())
	{
		if (oox_table_pr->m_oTblW->m_oType.IsInit() && oox_table_pr->m_oTblW->m_oW.IsInit())
		{
			if ( oox_table_pr->m_oTblW->m_oType->GetValue() == SimpleTypes::tblwidthDxa &&
				oox_table_pr->m_oTblW->m_oW->GetValue() > 0 )
			{
				if ( oox_table_pr->m_oTblW->m_oW->IsPercent() == false)
					odt_context->table_context()->set_default_column_width(oox_table_pr->m_oTblW->m_oW->GetValue() / 20.);
			}
			else if ( oox_table_pr->m_oTblW->m_oType->GetValue() == SimpleTypes::tblwidthAuto && 
				oox_table_pr->m_oTblW->m_oW->GetValue()	== 0 )
			{
				//динамическое расширение - автоподбор по содержимому.
				odt_context->table_context()->set_optimal_column_width(true);
				table_properties->content_.style_use_optimal_column_width_ = true;
			}
		}
	}

	if (oox_table_pr->m_oTblStyleColBandSize.IsInit())
	{
	}
	if (oox_table_pr->m_oTblStyleRowBandSize.IsInit())
	{
	}
	
	if (oox_table_pr->m_oTblInd.IsInit())//отступ слева - обтекания нет
	{
		_CP_OPT(odf_types::length) length;

		convert(oox_table_pr->m_oTblInd.GetPointer(), length);
        if (length)
        {
            table_properties->content_.common_horizontal_margin_attlist_.fo_margin_left_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);
        }
		table_properties->content_.table_align_ = odf_types::table_align(odf_types::table_align::Left);
	}
	else if(oox_table_pr->m_oTblpPr.IsInit()) //отступы, обтекание есть 
	{
		table_properties->content_.table_align_ = odf_types::table_align(odf_types::table_align::Left);

		if (oox_table_pr->m_oTblpPr->m_oTblpX.IsInit() && oox_table_pr->m_oTblpPr->m_oTblpY.IsInit()){}//floating position 
		else
		{
            _CP_OPT(odf_types::length) length_, length_left, length_right;
			
			convert(dynamic_cast<SimpleTypes::CUniversalMeasure *>(oox_table_pr->m_oTblpPr->m_oLeftFromText.GetPointer()), length_left);
            if (length_left)
                table_properties->content_.common_horizontal_margin_attlist_.fo_margin_left_ = odf_types::length(length_left->get_value_unit(odf_types::length::cm),odf_types::length::cm);
				
			convert(dynamic_cast<SimpleTypes::CUniversalMeasure *>(oox_table_pr->m_oTblpPr->m_oRightFromText.GetPointer()), length_right);
            if (length_right)
                table_properties->content_.common_horizontal_margin_attlist_.fo_margin_right_= odf_types::length(length_right->get_value_unit(odf_types::length::cm),odf_types::length::cm);

            convert(dynamic_cast<SimpleTypes::CUniversalMeasure *>(oox_table_pr->m_oTblpPr->m_oTopFromText.GetPointer()), length_);
            if (length_)
                table_properties->content_.common_vertical_margin_attlist_.fo_margin_top_= odf_types::length(length_->get_value_unit(odf_types::length::cm),odf_types::length::cm);

            convert(dynamic_cast<SimpleTypes::CUniversalMeasure *>(oox_table_pr->m_oTblpPr->m_oBottomFromText.GetPointer()),	length_);
            if (length_)
                table_properties->content_.common_vertical_margin_attlist_.fo_margin_bottom_= odf_types::length(length_->get_value_unit(odf_types::length::cm),odf_types::length::cm);

			if ((length_left && length_right ) && (*length_left == *length_right))
				table_properties->content_.table_align_ = odf_types::table_align(odf_types::table_align::Center);
		}
	}
	else if (oox_table_pr->m_oTblLayout.IsInit() && oox_table_pr->m_oTblLayout->m_oType.IsInit())
	{
		table_properties->content_.common_horizontal_margin_attlist_.fo_margin_left_ = odf_types::length(0, odf_types::length::cm);
		
		table_properties->content_.table_align_ = odf_types::table_align(odf_types::table_align::Left);
	}
	if(oox_table_pr->m_oJc.IsInit() && oox_table_pr->m_oJc->m_oVal.IsInit())
	{
		switch(oox_table_pr->m_oJc->m_oVal->GetValue())
		{
		case 0: table_properties->content_.table_align_ = odf_types::table_align(odf_types::table_align::Center); break;
		case 2: 
		case 3: table_properties->content_.table_align_ = odf_types::table_align(odf_types::table_align::Left); break;
		case 1: 
		case 4: table_properties->content_.table_align_ = odf_types::table_align(odf_types::table_align::Right); break;
		default: break;
		}
	}
    //nullable<ComplexTypes::Word::COnOff2 > m_oBidiVisual;
	//nullable<ComplexTypes::Word::CShading                        > m_oShade;
    //nullable<ComplexTypes::Word::std::wstring_                        > m_oTblCaption;
	//nullable<OOX::Logic::CTblCellMar                             > m_oTblCellMar;
	//nullable<ComplexTypes::Word::CTblWidth                       > m_oTblCellSpacing;
    //nullable<ComplexTypes::Word::std::wstring_                        > m_oTblDescription;
	//nullable<ComplexTypes::Word::CTblLayoutType                  > m_oTblLayout;
	//nullable<ComplexTypes::Word::CTblOverlap                     > m_oTblOverlap;

	return true;
}

void DocxConverter::convert(OOX::Logic::CTableProperty *oox_table_pr, odf_writer::style_table_cell_properties * table_cell_properties)
{
	if (oox_table_pr == NULL || table_cell_properties == NULL) return;

	convert(oox_table_pr->m_oTblBorders.GetPointer(), table_cell_properties);

	if (oox_table_pr->m_oTblCellMar.IsInit())
	{
		_CP_OPT(odf_types::length) length;
		convert(oox_table_pr->m_oTblCellMar->m_oStart.GetPointer(), length);
		if (length)table_cell_properties->content_.common_padding_attlist_.fo_padding_left_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

		convert(oox_table_pr->m_oTblCellMar->m_oEnd.GetPointer(), length);
		if (length)table_cell_properties->content_.common_padding_attlist_.fo_padding_right_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

		convert(oox_table_pr->m_oTblCellMar->m_oTop.GetPointer(), length);
		if (length)table_cell_properties->content_.common_padding_attlist_.fo_padding_top_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);
		
		convert(oox_table_pr->m_oTblCellMar->m_oBottom.GetPointer(), length);
		if (length)table_cell_properties->content_.common_padding_attlist_.fo_padding_bottom_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

	}
}
bool DocxConverter::convert(OOX::Logic::CTableProperty *oox_table_pr, bool base_styled)
{
	if (oox_table_pr && oox_table_pr->m_oTblBorders.IsInit())
	{//напрямую задать cell_prop на саму таблицу низя - тока как default-cell-style-name на columns & row

		//общие свойства ячеек
		odt_context->styles_context()->create_style(L"", odf_types::style_family::TableCell, true, false, -1); //ради нормального задания дефолтовых свойств на cells
		odt_context->styles_context()->last_state()->set_dont_write(true);
		odf_writer::style_table_cell_properties	* table_cell_properties = odt_context->styles_context()->last_state()->get_table_cell_properties();
		
		if (base_styled)// накатим свойства из стиля
		{			
			odt_context->styles_context()->table_styles().get_table_cell_properties(table_cell_properties);
		}		
		convert(oox_table_pr, table_cell_properties);

		odt_context->table_context()->set_default_cell_properties(odt_context->styles_context()->last_state()->get_name());
	}
//стиль создаем всегда	
	odt_context->styles_context()->create_style(L"", odf_types::style_family::Table, true, false, -1); 
	
	odf_writer::style_table_properties *table_properties = odt_context->styles_context()->last_state()->get_table_properties();
	if (base_styled)// накатим свойства - они не наследуются :(
	{		
		odt_context->styles_context()->table_styles().get_table_properties(table_properties);

		if (odt_context->styles_context()->table_styles().is_paragraph_properties())
		{
			odf_writer::style_paragraph_properties *para_properties = odt_context->styles_context()->last_state()->get_paragraph_properties();
			odt_context->styles_context()->table_styles().get_paragraph_properties(para_properties);
		}
		if (odt_context->styles_context()->table_styles().is_text_properties())
		{
			odf_writer::style_text_properties *text_properties = odt_context->styles_context()->last_state()->get_text_properties();
			odt_context->styles_context()->table_styles().get_text_properties(text_properties);
		}
	}
	convert(oox_table_pr, table_properties);
	
	return true;
}
void DocxConverter::convert(OOX::Logic::CTableRowProperties *oox_table_row_pr, odf_writer::style_table_row_properties *table_row_properties)
{
	if (oox_table_row_pr == NULL) return;
	if (table_row_properties == NULL) return;

	if ((oox_table_row_pr->m_oTblHeight.IsInit()) && 
		(oox_table_row_pr->m_oTblHeight->m_oVal.IsInit()))
	{
		_CP_OPT(odf_types::length) length;
		convert(dynamic_cast<SimpleTypes::CUniversalMeasure *>(oox_table_row_pr->m_oTblHeight->m_oVal.GetPointer()), length);

		odt_context->table_context()->set_row_height(oox_table_row_pr->m_oTblHeight->m_oVal->ToPoints());

		if (oox_table_row_pr->m_oTblHeight->m_oHRule.IsInit())
		{
			switch(oox_table_row_pr->m_oTblHeight->m_oHRule->GetValue())
			{
				case SimpleTypes::heightruleAtLeast:
					table_row_properties->style_table_row_properties_attlist_.style_min_row_height_ = odf_types::length(length->get_value_unit(odf_types::length::cm), odf_types::length::cm); break;
				case SimpleTypes::heightruleExact:
					table_row_properties->style_table_row_properties_attlist_.style_row_height_ = odf_types::length(length->get_value_unit(odf_types::length::cm), odf_types::length::cm); break;
				case SimpleTypes::heightruleAuto:
					table_row_properties->style_table_row_properties_attlist_.style_use_optimal_row_height_ = true; break;
			}
		}
		else
			table_row_properties->style_table_row_properties_attlist_.style_min_row_height_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

	}
}
void DocxConverter::convert(OOX::Logic::CTableRowProperties *oox_table_row_pr)
{
	if (oox_table_row_pr == NULL) return;

	odf_writer::style_table_row_properties	*table_row_properties = odt_context->styles_context()->last_state()->get_table_row_properties();

	if (oox_table_row_pr->m_oCnfStyle.IsInit())
	{
	}

	convert(oox_table_row_pr, table_row_properties);

    //nullable<ComplexTypes::Word::COnOff2 > m_oCantSplit;
	//nullable<ComplexTypes::Word::CCnf                            > m_oCnfStyle;
	//nullable<ComplexTypes::Word::CTrackChange                    > m_oDel;
	//nullable<ComplexTypes::Word::CDecimalNumber                  > m_oDivId;
	//nullable<ComplexTypes::Word::CDecimalNumber                  > m_oGridAfter;
	//nullable<ComplexTypes::Word::CDecimalNumber                  > m_oGridBefore;
    //nullable<ComplexTypes::Word::COnOff2 > m_oHidden;
	//nullable<ComplexTypes::Word::CTrackChange                    > m_oIns;
	//nullable<ComplexTypes::Word::CJcTable                        > m_oJc;
	//nullable<ComplexTypes::Word::CTblWidth                       > m_oTblCellSpacing;
	//nullable<OOX::Logic::CTrPrChange                             > m_oTrPrChange;
	//nullable<ComplexTypes::Word::CTblWidth                       > m_oWAfter;
	//nullable<ComplexTypes::Word::CTblWidth                       > m_oWBefore;
}
bool DocxConverter::convert(OOX::Logic::CTableCellProperties *oox_table_cell_pr, odf_writer::style_table_cell_properties *table_cell_properties)
{
	if (oox_table_cell_pr == NULL) return false;
	if (table_cell_properties == NULL) return false;

	if (oox_table_cell_pr->m_oShd.IsInit())
	{
		_CP_OPT(odf_types::color) odf_color;
		convert(oox_table_cell_pr->m_oShd.GetPointer(), odf_color);

		if (odf_color)
			table_cell_properties->content_.common_background_color_attlist_.fo_background_color_ = *odf_color;
	}

	if (oox_table_cell_pr->m_oTextDirection.IsInit() && oox_table_cell_pr->m_oTextDirection->m_oVal.IsInit())
	{
		switch(oox_table_cell_pr->m_oTextDirection->m_oVal->GetValue())
		{
		case SimpleTypes::textdirectionTb  :
		{
			table_cell_properties->content_.style_direction_ = odf_types::direction(odf_types::direction::Ltr);
		}break;
		case SimpleTypes::textdirectionLr  ://повернутость буковок
		case SimpleTypes::textdirectionLrV :
		case SimpleTypes::textdirectionTbV :
		case SimpleTypes::textdirectionRlV :
		{
			table_cell_properties->content_.style_direction_ = odf_types::direction(odf_types::direction::Ttb);
			odf_writer::style_text_properties *text_cell_properties	= odt_context->styles_context()->last_state()->get_text_properties();
			if (text_cell_properties)
			{
				text_cell_properties->content_.style_text_rotation_angle_ = 90;
				text_cell_properties->content_.style_text_rotation_scale_ = odf_types::text_rotation_scale::LineHeight;
			}
		}break;
		case SimpleTypes::textdirectionRl  ://rtl
			break;
		}
	}
	convert(oox_table_cell_pr->m_oTcBorders.GetPointer() , table_cell_properties);

	if (oox_table_cell_pr->m_oTcMar.IsInit())
	{
		_CP_OPT(odf_types::length) length;
		convert(oox_table_cell_pr->m_oTcMar->m_oStart.GetPointer(), length);
		if (length)table_cell_properties->content_.common_padding_attlist_.fo_padding_left_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

		convert(oox_table_cell_pr->m_oTcMar->m_oEnd.GetPointer(), length);
		if (length)table_cell_properties->content_.common_padding_attlist_.fo_padding_right_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

		convert(oox_table_cell_pr->m_oTcMar->m_oTop.GetPointer(), length);
		if (length)table_cell_properties->content_.common_padding_attlist_.fo_padding_top_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);
		
		convert(oox_table_cell_pr->m_oTcMar->m_oBottom.GetPointer(), length);
		if (length)table_cell_properties->content_.common_padding_attlist_.fo_padding_bottom_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

	}
	if (oox_table_cell_pr->m_oVAlign.IsInit() && oox_table_cell_pr->m_oVAlign->m_oVal.IsInit())
	{
		switch(oox_table_cell_pr->m_oVAlign->m_oVal->GetValue())
		{
		case SimpleTypes::verticaljcBoth   : //??????
			table_cell_properties->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Justify); break;
		case SimpleTypes::verticaljcBottom :
			table_cell_properties->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Bottom); break;
		case SimpleTypes::verticaljcCenter :
			table_cell_properties->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Middle); break;
		case SimpleTypes::verticaljcTop    :
			table_cell_properties->content_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Top); break;
		}
	}

	if (oox_table_cell_pr->m_oTcFitText.IsInit() )
	{
		table_cell_properties->content_.style_shrink_to_fit_ = oox_table_cell_pr->m_oTcFitText->m_oVal.ToBool();
	}

	if (oox_table_cell_pr->m_oNoWrap.IsInit())
	{
		if (oox_table_cell_pr->m_oNoWrap->m_oVal.ToBool() == true)
			table_cell_properties->content_.fo_wrap_option_ = odf_types::wrap_option(odf_types::wrap_option::NoWrap);
		else
			table_cell_properties->content_.fo_wrap_option_ = odf_types::wrap_option(odf_types::wrap_option::Wrap);
	}
	if (oox_table_cell_pr->m_oTcW.IsInit() && oox_table_cell_pr->m_oTcW->m_oW.IsInit() &&
		oox_table_cell_pr->m_oTcW->m_oType.IsInit() && oox_table_cell_pr->m_oTcW->m_oType->GetValue() == SimpleTypes::tblwidthDxa)
	{
		//кастомная ширина ячейки :( //2222010_53102Reader final.docx - все равно кривоватенько
		odt_context->table_context()->change_current_column_width(oox_table_cell_pr->m_oTcW->m_oW->GetValue()/ 20.);//dxa type
	}
	
	
	//nullable<OOX::Logic::CHeaders                                > m_oHeaders;
    //nullable<ComplexTypes::Word::COnOff2 > m_oHideMark;
	return true;
}
bool DocxConverter::IsOdfTableCellPresent(OOX::Logic::CTableCellProperties *oox_table_cell_pr)
{
	if (!oox_table_cell_pr) return false;

	if (oox_table_cell_pr->m_oShd.IsInit()) return true;
	if (oox_table_cell_pr->m_oTcBorders.IsInit()) return true;
	if (oox_table_cell_pr->m_oTextDirection.IsInit()) return true;
	if (oox_table_cell_pr->m_oVAlign.IsInit()) return true;
	if (oox_table_cell_pr->m_oTcMar.IsInit()) return true;
	if (oox_table_cell_pr->m_oTcFitText.IsInit()) return true;
	if (oox_table_cell_pr->m_oNoWrap.IsInit()) return true;
	
	if (oox_table_cell_pr->m_oTcW.IsInit() && oox_table_cell_pr->m_oTcW->m_oW.IsInit() &&
		oox_table_cell_pr->m_oTcW->m_oType.IsInit() && oox_table_cell_pr->m_oTcW->m_oType->GetValue() == SimpleTypes::tblwidthDxa)
		return true;
	
	//if (oox_table_cell_pr->m_oCnfStyle.IsInit()) return true;

	return false;
}
bool DocxConverter::convert(OOX::Logic::CTableCellProperties *oox_table_cell_pr, int col)
{	
	if (col < 0)col = odt_context->table_context()->current_column() + 1;
	int			row = odt_context->table_context()->current_row();
		
	bool is_base_styled = odt_context->table_context()->is_styled(row , col);

	odf_writer::style_table_cell_properties * parent_cell_properties = NULL;

	std::wstring parent_name = odt_context->table_context()->get_default_cell_properties();
	if (false == parent_name.empty()) 
	{
		odf_writer::style * style_ = NULL;		
		if (odt_context->styles_context()->find_odf_style(parent_name, odf_types::style_family::TableCell, style_))
		{
			parent_cell_properties = style_->content_.get_style_table_cell_properties();
		}
	}
	bool bTableCellPropsPresent = IsOdfTableCellPresent(oox_table_cell_pr);

	if (!bTableCellPropsPresent && is_base_styled == false && parent_cell_properties == NULL) return false;
	
	odt_context->styles_context()->create_style(L"", odf_types::style_family::TableCell, true, false, -1); 	
	
	odf_writer::style_table_cell_properties *cell_properties = odt_context->styles_context()->last_state()->get_table_cell_properties();

	if (cell_properties == NULL) return false;

	if (is_base_styled)
	{
		odf_writer::style_text_properties		*text_properties		= odt_context->styles_context()->last_state()->get_text_properties();
		odf_writer::style_paragraph_properties	*paragraph_properties	= odt_context->styles_context()->last_state()->get_paragraph_properties();
		
		odt_context->styles_context()->table_styles().get_table_cell_properties (col, row, cell_properties);
		odt_context->styles_context()->table_styles().get_text_properties		(col, row, text_properties);
		odt_context->styles_context()->table_styles().get_paragraph_properties	(col, row, paragraph_properties);
	}
	cell_properties->apply_from(parent_cell_properties);

//check for inside cell or not

	_CP_OPT(std::wstring) border_inside_v = odt_context->table_context()->get_table_inside_v();
	_CP_OPT(std::wstring) border_inside_h = odt_context->table_context()->get_table_inside_h();
	
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
			int del_border = (int)border_inside_h->find(L"none");
			if (row != 1)
			{
				if (cell_properties->content_.common_border_attlist_.fo_border_top_ && del_border>=0)
					cell_properties->content_.common_border_attlist_.fo_border_top_ = boost::none;

				else if (border_inside_h && del_border<0)
					cell_properties->content_.common_border_attlist_.fo_border_top_ = *border_inside_h;
			}
			if (row != odt_context->table_context()->count_rows())
			{
				if (cell_properties->content_.common_border_attlist_.fo_border_bottom_ && del_border>=0)
					cell_properties->content_.common_border_attlist_.fo_border_bottom_ = boost::none;

				else if (border_inside_h && del_border<0)
					cell_properties->content_.common_border_attlist_.fo_border_bottom_ = *border_inside_h;
			}
		}
		if (border_inside_v)
		{
			int del_border = (int)border_inside_v->find(L"none");
			if (col != 1)
			{
				if (cell_properties->content_.common_border_attlist_.fo_border_left_ && del_border>=0)
					cell_properties->content_.common_border_attlist_.fo_border_left_ = boost::none;

				else if (border_inside_h && del_border<0)
					cell_properties->content_.common_border_attlist_.fo_border_left_ = *border_inside_h;
			}
			if (col != odt_context->table_context()->count_columns())
			{
				if (cell_properties->content_.common_border_attlist_.fo_border_right_ && del_border>=0)
					cell_properties->content_.common_border_attlist_.fo_border_right_ = boost::none;

				else if (border_inside_h && del_border<0)
					cell_properties->content_.common_border_attlist_.fo_border_right_ = *border_inside_h;
			}
		}
	}
	bool res = convert(oox_table_cell_pr, cell_properties);

	return true;
}
} 

