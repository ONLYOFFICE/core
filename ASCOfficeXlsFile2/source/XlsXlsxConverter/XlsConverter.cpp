#include "XlsConverter.h"

#include "../XlsFormat/Binary/CompoundFile.h"
#include "../XlsFormat/Binary/CFStreamCacheReader.h"

#include "../XlsFormat/Logic/GlobalWorkbookInfo.h"
#include "../XlsFormat/Logic/WorkbookStreamObject.h"
#include "../XlsFormat/Logic/WorksheetSubstream.h"
#include "../XlsFormat/Logic/GlobalsSubstream.h"
#include "../XlsFormat/Logic/BinProcessor.h"
#include "../XlsFormat/Logic/SummaryInformationStream/SummaryInformation.h"

#include "../XlsFormat/Logic/Biff_unions/FORMATTING.h"
#include "../XlsFormat/Logic/Biff_unions/THEME.h"
#include "../XlsFormat/Logic/Biff_unions/GLOBALS.h"
#include "../XlsFormat/Logic/Biff_unions/COLUMNS.h"
#include "../XlsFormat/Logic/Biff_unions/SHAREDSTRINGS.h"
#include "../XlsFormat/Logic/Biff_unions/HLINK.h"
#include "../XlsFormat/Logic/Biff_unions/LBL.h"
#include "../XlsFormat/Logic/Biff_unions/OBJECTS.h"
#include "../XlsFormat/Logic/Biff_unions/MSODRAWINGGROUP.h"
#include "../XlsFormat/Logic/Biff_unions/OBJ.h"

#include <Logic/Biff_records/HLink.h>
#include <Logic/Biff_records/MsoDrawingGroup.h>
#include <Logic/Biff_records/MsoDrawing.h>
#include <Logic/Biff_records/Obj.h>

#include <Logic/Biff_structures/URLMoniker.h>
#include <Logic/Biff_structures/FileMoniker.h>

#include <Logic/Biff_structures/ODRAW/OfficeArtBStoreContainer.h>

#include "xlsx_conversion_context.h"
#include "xlsx_package.h"

#include <simple_xml_writer.h>

#include <boost/lexical_cast.hpp>
#include "../../../DesktopEditor/common/File.h"

XlsConverter::XlsConverter(const std::wstring & xls_file, const std::wstring & _xlsx_path, const ProgressCallback* CallBack) 
{
	xlsx_path		= _xlsx_path;
	output_document = NULL;
	xlsx_context	= NULL;
	
	pCallBack			= CallBack;
	bUserStopConvert	= false;

	try{
		XLS::CompoundFile cfile(xls_file, XLS::CompoundFile::cf_ReadMode);

		XLS::CFStreamPtr summary;
		XLS::CFStreamPtr doc_summary;

		try
		{
			summary = cfile.getSummaryInformationStream();
		}
		catch (...)
		{
		}

		try
		{
			doc_summary = cfile.getDocumentSummaryInformationStream();
		}
		catch (...)
		{
			return;
		}

		WORD workbook_code_page = XLS::WorkbookStreamObject::DefaultCodePage;
		if(summary)
		{
			OLEPS::SummaryInformation summary_info(summary);
			workbook_code_page = summary_info.GetCodePage();
		}
		else if(doc_summary)
		{
			OLEPS::SummaryInformation doc_summary_info(doc_summary);
			workbook_code_page = doc_summary_info.GetCodePage();
		}

		if(1200/* UTF-16 */ == workbook_code_page || 0/*error*/ == workbook_code_page)
		{
			workbook_code_page = XLS::WorkbookStreamObject::DefaultCodePage;
		}

		xls_global_info = boost::shared_ptr<XLS::GlobalWorkbookInfo>(new XLS::GlobalWorkbookInfo(workbook_code_page));

		XLS::CFStreamCacheReader stream_reader(cfile.getWorkbookStream(), xls_global_info);

		xls_document = boost::shared_ptr<XLS::WorkbookStreamObject>(new XLS::WorkbookStreamObject(workbook_code_page));

		XLS::BinReaderProcessor proc(stream_reader , xls_document.get() , true);
		proc.mandatory(*xls_document.get());

	}
	catch(...)
	{
		return;
	}
	if (UpdateProgress(400000))return;

	output_document		= new oox::package::xlsx_document();
    xlsx_context		= new oox::xlsx_conversion_context(output_document);
}

XlsConverter::~XlsConverter() 
{
	if (xlsx_context)		delete xlsx_context;
	if (output_document)	delete output_document;
}

#define PROGRESSEVENT_ID	0

bool XlsConverter::UpdateProgress(long nComplete)
{
	if (pCallBack)
	{
		pCallBack->OnProgress (pCallBack->caller, PROGRESSEVENT_ID, nComplete);

		bUserStopConvert = 0;
		pCallBack->OnProgressEx (pCallBack->caller, PROGRESSEVENT_ID, nComplete, &bUserStopConvert);

		if (bUserStopConvert !=0 ) return TRUE;
	}

	return FALSE;
}
void XlsConverter::write()
{
	if (!output_document)return;
	output_document->write(xlsx_path);

	delete output_document; output_document = NULL;

	if (UpdateProgress(1000000))return;
}

void XlsConverter::convertDocument()
{
    if (!xls_document)     return;
    if (!output_document)   return;
    if (!xlsx_context)       return;

	xlsx_context->start_document();

	convert((XLS::WorkbookStreamObject*)xls_document.get());

	if (UpdateProgress(800000))return;

	xlsx_context->end_document();

	if (UpdateProgress(850000))return;
}

void XlsConverter::convert(XLS::BaseObject	*xls_unknown)
{
	if (xls_unknown == NULL) return;

	XLS::ElementType type = xls_unknown->get_type();

	switch (type)
	{
	case XLS::typeHLINK:
		{
			XLS::HLINK * hlink = dynamic_cast<XLS::HLINK *>(xls_unknown);
			convert(hlink);
		}break;
	case XLS::typeLBL:	
		{
			XLS::LBL * lbl = dynamic_cast<XLS::LBL *>(xls_unknown);
			convert(lbl);
		}break;
	case XLS::typeOBJECTS:
		{
			XLS::OBJECTS * obj = dynamic_cast<XLS::OBJECTS *>(xls_unknown);
			convert(obj);
		}break;
	case XLS::typeAnyObject:	
	default:
		{
			for (std::list<XLS::BaseObjectPtr>::iterator it = xls_unknown->elements_.begin(); it != xls_unknown->elements_.end(); it++)
			{
				(*it)->serialize(xlsx_context->current_sheet().sheetData());
			}
		}
	}

}

void XlsConverter::convert(XLS::WorkbookStreamObject* woorkbook)
{
	if (woorkbook == NULL) return;

	convert((XLS::GlobalsSubstream*)woorkbook->m_GlobalsSubstream.get());

	for (long i=0 ; i < woorkbook->m_WorksheetSubstream.size(); i++)
	{
		xlsx_context->start_table(xls_global_info->sheets_names[i]);
		convert((XLS::WorksheetSubstream*)woorkbook->m_WorksheetSubstream[i].get());
		xlsx_context->end_table();
	}

	for (std::list<XLS::BaseObjectPtr>::iterator it = woorkbook->elements_.begin(); it != woorkbook->elements_.end(); it++)
	{
		convert(it->get());
	}
}

void XlsConverter::convert(XLS::WorksheetSubstream* sheet)
{
	if (sheet == NULL) return;

	if (sheet->m_Dimensions)
	{
		sheet->m_Dimensions->serialize(xlsx_context->current_sheet().dimension());
	}
	if (sheet->m_GLOBALS)
	{
		XLS::GLOBALS * globals  = dynamic_cast<XLS::GLOBALS *>(sheet->m_GLOBALS.get());
		XLS::COLUMNS * columns = dynamic_cast<XLS::COLUMNS *>(sheet->m_COLUMNS.get());

		if (globals && columns)
		{
			globals->m_DefColWidth = columns->m_DefColWidth; 
		}
		sheet->m_GLOBALS->serialize(xlsx_context->current_sheet().sheetFormat());
	}
	if (sheet->m_COLUMNS)
	{
		sheet->m_COLUMNS->serialize(xlsx_context->current_sheet().cols());
	}
	if (sheet->m_CELLTABLE)
	{
		sheet->m_CELLTABLE->serialize(xlsx_context->current_sheet().sheetData());
	}
	if (sheet->m_MergeCells.size() > 0)
	{
		CP_XML_WRITER(xlsx_context->current_sheet().mergeCells())    
		{
			CP_XML_NODE(L"mergeCells")
			{  		
				for (long i = 0 ; i < sheet->m_MergeCells.size(); i++)
				{
					sheet->m_MergeCells[i]->serialize(CP_XML_STREAM());
				}
			}
		}
	}
	for (long i = 0 ; i < sheet->m_HLINK.size(); i++)
	{
		convert((XLS::HLINK*)sheet->m_HLINK[i].get());
	}
	convert((XLS::OBJECTS*)sheet->m_OBJECTS.get());

	if (sheet->m_PAGESETUP)
	{
		sheet->m_PAGESETUP->serialize(xlsx_context->current_sheet().pageProperties());
	}
}

void XlsConverter::convert(XLS::GlobalsSubstream* global)
{
	if (global == NULL) return;
	
	convert((XLS::FORMATTING*)global->m_Formating.get());
	
	convert((XLS::THEME*)global->m_Theme.get());

	convert((XLS::SHAREDSTRINGS*)global->m_SHAREDSTRINGS.get());

	for (long i = 0 ; i < global->m_LBL.size(); i++)
	{
		convert((XLS::LBL*)global->m_LBL[i].get());
	}

	for (long i = 0 ; i < global->m_MSODRAWINGGROUP.size(); i++)
	{
		convert((XLS::MSODRAWINGGROUP*)global->m_MSODRAWINGGROUP[i].get());
	}
}

typedef boost::unordered_map<XLS::FillInfo, int>	mapFillInfo;
typedef boost::unordered_map<XLS::BorderInfo, int>	mapBorderInfo;

void XlsConverter::convert(XLS::FORMATTING* formating)
{
	if (formating == NULL) return;

	std::wstringstream strm;
    CP_XML_WRITER(strm)    
    {
        CP_XML_NODE(L"styleSheet")
        {   
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");

			formating->serialize1(CP_XML_STREAM()); //важен порядок в styles

			CP_XML_NODE(L"fills")
			{
				std::vector<XLS::FillInfo> fills_out;
				fills_out.resize(xls_global_info->fill_x_ids.size());

				for (mapFillInfo::iterator it = xls_global_info->fill_x_ids.begin(); it != xls_global_info->fill_x_ids.end(); it++)
				{
					fills_out[it->second] = it->first;
				}

				CP_XML_ATTR(L"count", fills_out.size());
				for (int i = 0 ;i < fills_out.size(); i++)
				{
					fills_out[i].serialize(CP_XML_STREAM());
				}
			}
			CP_XML_NODE(L"borders")
			{				
				std::vector<XLS::BorderInfo> borders_out;
				borders_out.resize(xls_global_info->border_x_ids.size());

				for (mapBorderInfo::iterator it = xls_global_info->border_x_ids.begin(); it != xls_global_info->border_x_ids.end(); it++)
				{
					borders_out[it->second] = it->first;
				}
				
				CP_XML_ATTR(L"count", borders_out.size());
				for (int i = 0 ;i < borders_out.size(); i++)
				{
					borders_out[i].serialize(CP_XML_STREAM());
				}
			}

			formating->serialize2(CP_XML_STREAM());

		}
	}
    
	output_document->get_xl_files().set_styles( oox::package::simple_element::create(L"styles.xml", strm.str()) );
}

std::wstring XlsConverter::GetTargetMoniker(XLS::BiffStructure *moniker)
{
	if (moniker->getClassName() == L"URLMoniker")
	{
		OSHARED::URLMoniker* urlMoniker = dynamic_cast<OSHARED::URLMoniker* >(moniker);
		if (urlMoniker)return urlMoniker->url;
	}
	else if (moniker->getClassName() == L"FileMoniker")
	{
		OSHARED::FileMoniker* fileMoniker = dynamic_cast<OSHARED::FileMoniker* >(moniker);
		if (fileMoniker)
		{
			if (!fileMoniker->unicodePath.empty()) return fileMoniker->unicodePath;
			else return std::wstring(fileMoniker->ansiPath.begin(), fileMoniker->ansiPath.end());//codePage ??? todooo
		}
	}

	return L"";
}

void XlsConverter::convert(XLS::MSODRAWINGGROUP * mso_drawing)
{
	if ( mso_drawing == NULL) return;
	XLS::MsoDrawingGroup * mso_group = dynamic_cast<XLS::MsoDrawingGroup*>(mso_drawing->m_MsoDrawingGroup.get());
	if (mso_group == NULL) return;


	for (long i = 0 ; i < mso_group->rgChildRec.child_records.size(); i++)
	{
		ODRAW::OfficeArtRecord * art_record = dynamic_cast<ODRAW::OfficeArtRecord*>(mso_group->rgChildRec.child_records[i].get());

		if (art_record == NULL) return;

		switch(art_record->rh_own.recType)
		{
		case ODRAW::OfficeArtRecord::BStoreContainer:
			{
				convert((ODRAW::OfficeArtBStoreContainer*)art_record);
			}break;
		case ODRAW::OfficeArtRecord::BlipPICT:
			{
			}break;
		}
	}
		
}

void XlsConverter::convert(ODRAW::OfficeArtBStoreContainer* art_bstore)
{
	if (art_bstore == NULL) return;

	for (long i =0 ; i < art_bstore->rgfb.size(); i++)
	{
		int bin_id = i + 1;
		if (art_bstore->rgfb[i]->data_size > 0)
		{
			FileSystem::Directory::CreateDirectory((xlsx_path + FILE_SEPARATOR_STR + L"media").c_str());
			
			std::wstring file_name = L"image" + boost::lexical_cast<std::wstring>(bin_id) + art_bstore->rgfb[i]->pict_type;

			NSFile::CFileBinary file;
			if (file.CreateFileW(xlsx_path + FILE_SEPARATOR_STR + L"media" + FILE_SEPARATOR_STR + file_name))
			{
				file.WriteFile((BYTE*)art_bstore->rgfb[i]->pict_data, art_bstore->rgfb[i]->data_size);
				file.CloseFile();
			}

			xlsx_context->get_mediaitems().add_image(L"media/" + file_name, bin_id);
		}
		else
		{
			//???
		}

	}
}

void XlsConverter::convert(XLS::HLINK * HLINK_)
{
	XLS::HLink * hLink = dynamic_cast<XLS::HLink*>(HLINK_->m_HLink.get());
	
	std::wstring target = GetTargetMoniker(hLink->hyperlink.oleMoniker.data.get());

	std::wstring display = hLink->hyperlink.displayName;

	if (display.empty())	display = target;

	xlsx_context->get_table_context().add_hyperlink( hLink->ref8.toString(), target, display);
}

void XlsConverter::convert(XLS::LBL * def_name)
{
	if (def_name == NULL) return;

	def_name->serialize(xlsx_context->defined_names());
}

void XlsConverter::convert(XLS::THEME* theme)
{
	if (theme == NULL) return;
}

void XlsConverter::convert(XLS::OBJECTS* objects)
{
	if (objects == NULL) return;

	for (long i = 0 ; i < objects->m_OBJs.size(); i++)
	{
		XLS::OBJ* OBJ = dynamic_cast<XLS::OBJ*>(objects->m_OBJs[i].get());
		XLS::Obj *obj = dynamic_cast<XLS::Obj*>(OBJ->m_Obj.get());

		if (obj->cmo.ot == 0x08)//image
		{
			xlsx_context->get_drawing_context().start_drawing(L"", obj->cmo.id);


			xlsx_context->get_drawing_context().end_drawing();
		}
	}

	for (long i = 0 ; i < objects->m_CHARTs.size(); i++)
	{
		//xlsx_context->get_chart_context().start_drawing();


		//xlsx_context->get_chart_context().end_drawing();
	}
}

void XlsConverter::convert(XLS::SHAREDSTRINGS* sharedstrings)
{
	if (sharedstrings == NULL) return;
	
	for (std::list<XLS::BaseObjectPtr>::iterator it = sharedstrings->elements_.begin(); it != sharedstrings->elements_.end(); it++)
	{
		(*it)->serialize(xlsx_context->shared_strings());
	}
}