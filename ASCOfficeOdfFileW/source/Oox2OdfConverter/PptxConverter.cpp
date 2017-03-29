/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#include "PptxConverter.h"
#include "../utils.h"

#include "../../../ASCOfficePPTXFile/PPTXFormat/Folder.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Presentation.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Slide.h"

#include <boost/lexical_cast.hpp>

#include "../OdfFormat/odp_conversion_context.h"

#include "../OdfFormat/odf_text_context.h"
#include "../OdfFormat/odf_drawing_context.h"

#include "../OdfFormat/styles.h"

#include "../OdfFormat/style_table_properties.h"
#include "../OdfFormat/style_text_properties.h"
#include "../OdfFormat/style_paragraph_properties.h"
#include "../OdfFormat/style_graphic_properties.h"
#include "../OdfFormat/styles_list.h"


using namespace cpdoccore;

namespace Oox2Odf
{
PptxConverter::PptxConverter(const std::wstring & path, const ProgressCallback* CallBack)
{
    const OOX::CPath oox_path(std::wstring(path.c_str()));

	pptx_document = new PPTX::Folder();
	if (!pptx_document->isValid(oox_path.GetPath())) // true ???
	{
		delete pptx_document;
		return;
	}

	pptx_document->read(oox_path.GetPath() + FILE_SEPARATOR_STR, NULL);

	smart_ptr<PPTX::Presentation> presentation_ptr = pptx_document->Get(OOX::Presentation::FileTypes::Presentation).smart_dynamic_cast<PPTX::Presentation>();
	if (!presentation_ptr.is_init())
	{
		delete pptx_document;
		return;
	}
	presentation = presentation_ptr.operator->();
	
	output_document = new odf_writer::package::odf_document(L"presentation");
    odp_context     = new odf_writer::odp_conversion_context(output_document);
	
	current_clrMap	= NULL;
	current_slide	= NULL;
	current_theme	= NULL;

	pCallBack		= CallBack;

	if (UpdateProgress(290000))return;
}
PptxConverter::~PptxConverter()
{
	current_clrMap	= NULL;
	current_slide	= NULL;
	current_theme	= NULL;
	presentation	= NULL;

	if (odp_context)		delete odp_context;		odp_context		= NULL;
	if (pptx_document)		delete pptx_document;	pptx_document	= NULL;
	if (output_document)	delete output_document;	output_document = NULL;
}
void PptxConverter::write(const std::wstring & path)
{
	if (!output_document)return;

	output_document->write(path);
		
	if (UpdateProgress(1000000))return;
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

NSCommon::smart_ptr<OOX::File> PptxConverter::find_file_by_id(std::wstring sId)
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

std::wstring PptxConverter::find_link_by_id (std::wstring sId, int type)
{
    std::wstring ref;

    if (ref.empty() && oox_current_child_document)
	{
		smart_ptr<OOX::File> oFile = oox_current_child_document->Find(sId);
		if (oFile.IsInit())
		{		
			if (type==1 && OOX::FileTypes::Image == oFile->type())
			{
				OOX::Image* pImage = (OOX::Image*)oFile.operator->();

				ref = pImage->filename().GetPath();
			}
			if (type==2 && oFile.IsInit() && OOX::FileTypes::HyperLink == oFile->type())
			{
				OOX::HyperLink* pHyperlink = (OOX::HyperLink*)oFile.operator->();

				ref = pHyperlink->Uri().GetPath();
			}
		}
	}
	smart_ptr<OOX::File> oFile = current_slide ? current_slide->Find(sId) : pptx_document->Find(sId);
    if (ref.empty() && oFile.IsInit())
	{
		if (type==1 && OOX::FileTypes::Image == oFile->type())
		{
			OOX::Image* pImage = (OOX::Image*)oFile.operator->();

			ref = pImage->filename().GetPath();
		}
		if (type == 2 && OOX::FileTypes::HyperLink == oFile->type())
		{
			OOX::HyperLink* pHyperlink = (OOX::HyperLink*)oFile.operator->();

			ref = pHyperlink->Uri().GetPath();
		}
	}

	return ref;
}

void PptxConverter::convertDocument()
{
    if (!pptx_document) return;
    if (!odp_context)   return;
		
	odp_context->start_document();

	if (UpdateProgress(300000))return;

	//convert_settings(); 
	
	convert_slides();

	if (UpdateProgress(800000))return;
	//удалим уже ненужный документ pptx 
	delete pptx_document; pptx_document = NULL;

	odp_context->end_document();
 	
	if (UpdateProgress(850000))return;
}

void PptxConverter::convert_slides()
{
	for (size_t i = 0; i < presentation->sldIdLst.size(); ++i)
	{
        std::wstring rId = presentation->sldIdLst[i].rid.get();
        smart_ptr<PPTX::Slide> slide = ((*presentation)[rId]).smart_dynamic_cast<PPTX::Slide>();
		
		if (slide.IsInit() == false)
        {
            continue;// странное ... слайд 38 в FY10_September_Partner_Call.pptx
        }
		current_theme = slide->theme.operator->();

		std::wstring master_style_name;
		std::wstring layout_style_name;

		if (slide->Master.IsInit())
		{
			current_clrMap	= &slide->Master->clrMap;

			std::map<std::wstring, std::wstring>::iterator pFind = m_mapMasters.find(slide->Master->m_sOutputFilename);
			if (pFind == m_mapMasters.end())
			{
				current_slide = slide->Master.operator->();
				master_style_name = odp_context->start_master_slide();
					convert(&slide->Master->cSld);		
				odp_context->end_master_slide();
				
				m_mapMasters.insert(std::make_pair(slide->Master->m_sOutputFilename, master_style_name));
			}
			else
				master_style_name = pFind->second;
		}
		
		if (slide->clrMapOvr.IsInit())
			current_clrMap	= slide->clrMapOvr->overrideClrMapping.GetPointer();
		
		current_slide = slide.operator->();
		odp_context->start_slide();
		
		odp_context->current_slide().set_master_page(master_style_name);
		//odp_context->current_slide().set_layout_style(layout_style_name);
		//nullable_bool		show;
		//nullable_bool		showMasterPhAnim;
		//nullable_bool		showMasterSp;
		
		convert(slide->cSld.GetPointer());


		//nullable<Logic::Transition>	transition;
		//nullable<Logic::Timing>		timing;


		//smart_ptr<NotesSlide>			Note;
		//smart_ptr<Theme>				theme;
		//smart_ptr<OOX::CVmlDrawing>	Vml;
		//smart_ptr<TableStyles>		tableStyles_;

		//smart_ptr<PPTX::Comments>		comments;

		odp_context->end_slide();
	}
}
void PptxConverter::convert(OOX::WritingElement  *oox_unknown)
{
	if (oox_unknown == NULL)return;

	switch(oox_unknown->getType())
	{
		default:
		{
			OoxConverter::convert(oox_unknown);
		}break;
	}
}

void PptxConverter::convert(PPTX::Logic::CSld *slide)
{
	if (slide == NULL) return;

	if (slide->attrName.IsInit())
		odp_context->current_slide().set_page_name(slide->attrName.get());

	for (size_t i = 0 ; i < slide->spTree.SpTreeElems.size(); i++)
	{
		convert(slide->spTree.SpTreeElems[i].GetElem().operator->());
	}
	//nullable<Bg>			bg;
	//nullable<Controls>	controls;

}



} 

