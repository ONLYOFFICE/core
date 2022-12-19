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
#include "XlsConverter.h"

#include "../Format/Binary/CompoundFile.h"
#include "../Format/Binary/CFStreamCacheReader.h"

#include "../Format/Logic/GlobalWorkbookInfo.h"
#include "../Format/Logic/WorkbookStreamObject.h"
#include "../Format/Logic/WorksheetSubstream.h"
#include "../Format/Logic/GlobalsSubstream.h"
#include "../Format/Logic/ChartSheetSubstream.h"
#include "../Format/Logic/MacroSheetSubstream.h"
#include "../Format/Logic/EncryptionStream.h"

#include "../Format/Logic/BinProcessor.h"
#include "../../Common/SummaryInformation/PropertySetStream.h"

#include "../Format/Logic/Biff_unions/FORMATTING.h"
#include "../Format/Logic/Biff_unions/THEME.h"
#include "../Format/Logic/Biff_unions/GLOBALS.h"
#include "../Format/Logic/Biff_unions/COLUMNS.h"
#include "../Format/Logic/Biff_unions/SHAREDSTRINGS.h"
#include "../Format/Logic/Biff_unions/HLINK.h"
#include "../Format/Logic/Biff_unions/LBL.h"
#include "../Format/Logic/Biff_unions/OBJECTS.h"
#include "../Format/Logic/Biff_unions/MSODRAWINGGROUP.h"
#include "../Format/Logic/Biff_unions/OBJ.h"
#include "../Format/Logic/Biff_unions/IMDATAOBJECT.h"
#include "../Format/Logic/Biff_unions/TEXTOBJECT.h"
#include "../Format/Logic/Biff_unions/CHART.h"
#include "../Format/Logic/Biff_unions/BACKGROUND.h"
#include "../Format/Logic/Biff_unions/PIVOTVIEW.h"
#include "../Format/Logic/Biff_unions/PIVOTCACHE.h"
#include "../Format/Logic/Biff_unions/PIVOTCACHEDEFINITION.h"
#include "../Format/Logic/Biff_unions/SUPBOOK.h"
#include "../Format/Logic/Biff_unions/QUERYTABLE.h"
#include "../Format/Logic/Biff_unions/FEAT.h"
#include "../Format/Logic/Biff_unions/FEAT11.h"

#include "../Format/Logic/Biff_records/BkHim.h"
#include "../Format/Logic/Biff_records/HLink.h"
#include "../Format/Logic/Biff_records/MsoDrawingGroup.h"
#include "../Format/Logic/Biff_records/MsoDrawing.h"
#include "../Format/Logic/Biff_records/Obj.h"
#include "../Format/Logic/Biff_records/TxO.h"
#include "../Format/Logic/Biff_records/IMDATA.h"
#include "../Format/Logic/Biff_records/Note.h"
#include "../Format/Logic/Biff_records/WsBool.h"
#include "../Format/Logic/Biff_records/Theme.h"
#include "../Format/Logic/Biff_records/Format.h"
#include "../Format/Logic/Biff_records/CalcMode.h"

#include "../Format/Logic/Biff_structures/URLMoniker.h"
#include "../Format/Logic/Biff_structures/FileMoniker.h"

#include "../Format/Logic/Biff_structures/ODRAW/OfficeArtBStoreContainer.h"
#include "../Format/Logic/Biff_structures/ODRAW/SimpleOfficeArtContainers.h"
#include "../Format/Logic/Biff_structures/ODRAW/OfficeArtFOPT.h"
#include "../Format/Logic/Biff_structures/ODRAW/OfficeArtFOPTE.h"
#include "../Format/Logic/Biff_structures/ODRAW/OfficeArtTertiaryFOPT.h"
#include "../Format/Logic/Biff_structures/ODRAW/OfficeArtFSP.h"
#include "../Format/Logic/Biff_structures/ODRAW/OfficeArtBlip.h"
#include "../Format/Logic/Biff_structures/ODRAW/OfficeArtFSPGR.h"
#include "../Format/Logic/Biff_structures/ODRAW/OfficeArtClientAnchorSheet.h"
#include "../Format/Logic/Biff_structures/ODRAW/OfficeArtClientAnchorHF.h"

#include "xlsx_conversion_context.h"
#include "xlsx_package.h"

#include "../../Common/Utils/simple_xml_writer.h"
#include "../../../OOXML/Base/Unit.h"

#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/raster/BgraFrame.h"

#include <boost/make_shared.hpp>

#if !defined(_WIN32) && !defined(_WIN64)

    typedef struct tagBITMAPINFOHEADER
    {
            _UINT32      biSize;
            _INT32       biWidth;
            _INT32       biHeight;
            _UINT16       biPlanes;
            _UINT16       biBitCount;
            _UINT32      biCompression;
            _UINT32      biSizeImage;
            _INT32       biXPelsPerMeter;
            _INT32       biYPelsPerMeter;
            _UINT32      biClrUsed;
            _UINT32      biClrImportant;
    } BITMAPINFOHEADER;

typedef struct tagBITMAPCOREHEADER {
        _UINT32   bcSize;                 /* used to get to color table */
        _UINT16    bcWidth;
        _UINT16    bcHeight;
        _UINT16    bcPlanes;
        _UINT16    bcBitCount;
} BITMAPCOREHEADER;
#endif
XlsConverter::XlsConverter() 
{
	output_document		= NULL;
	xlsx_context		= NULL;
	
	is_older_version	= false;
	is_encrypted		= false;
}
XlsConverter::XlsConverter(const std::wstring & xlsFileName, const std::wstring & xlsxFilePath, const std::wstring & password, const std::wstring & fontsPath, const std::wstring & tempPath, const int lcid_user, bool & bMacros) 
{
	xlsx_path			= xlsxFilePath;
	output_document		= NULL;
	xlsx_context		= NULL;
	
	is_older_version	= false;
	is_encrypted		= false;
	output_document		= new oox::package::xlsx_document();

	_UINT16 workbook_code_page = XLS::WorkbookStreamObject::DefaultCodePage;
	
	try
	{
		xls_file = boost::shared_ptr<XLS::CompoundFile>(new XLS::CompoundFile(xlsFileName, XLS::CompoundFile::cf_ReadMode));

		if (xls_file->isError())
		{
			xls_global_info = boost::shared_ptr<XLS::GlobalWorkbookInfo>(new XLS::GlobalWorkbookInfo(workbook_code_page, this));
			
			xls_global_info->lcid_user = lcid_user;
			xls_global_info->fontsDirectory = fontsPath;
			xls_global_info->password = password;
			xls_global_info->tempDirectory = tempPath;
			xls_global_info->CodePage = 0;
			xls_global_info->Version = 0;

			XLS::GlobalWorkbookInfo::_sheet_info sheet_info;
			xls_global_info->sheets_info.push_back(sheet_info);

			XLS::StreamCacheReaderPtr file_reader(new XLS::FileStreamCacheReader(xlsFileName, xls_global_info));
			xls_document = boost::shared_ptr<XLS::WorkbookStreamObject>(new XLS::WorkbookStreamObject(workbook_code_page));
		
			XLS::BinReaderProcessor proc(file_reader, xls_document.get(), true);
			
			XLS::BaseObjectPtr stream = XLS::BaseObjectPtr(new XLS::WorksheetSubstream(0));
            if (proc.mandatory(*stream.get()))
			{
				XLS::WorksheetSubstream *worksheet = dynamic_cast<XLS::WorksheetSubstream*>(stream.get());
				XLS::WorkbookStreamObject *workbook = dynamic_cast<XLS::WorkbookStreamObject*>(xls_document.get());
				if (workbook)
				{
					workbook->m_arWorksheetSubstream.push_back(stream);

					workbook->m_GlobalsSubstream = XLS::BaseObjectPtr(new XLS::GlobalsSubstream(0));

					XLS::GlobalsSubstream* globals = dynamic_cast<XLS::GlobalsSubstream*>(workbook->m_GlobalsSubstream.get());
					if (globals)
					{
						globals->m_Formating = worksheet->m_Formating;
						globals->UpdateXFC();
					}
				}
			}
			else
			{		
				return;
			}
		}
		else
		{
			OLEPS::PropertySetStream summary_stream;
			
			summary_stream.read(xls_file->getNamedStream(L"SummaryInformation"));
			summary_stream.read(xls_file->getNamedStream(L"DocumentSummaryInformation"), true);

			workbook_code_page = summary_stream.GetCodePage();
				
			output_document->get_docProps_files().set_app_content(summary_stream.GetApp());
			output_document->get_docProps_files().set_core_content(summary_stream.GetCore());
//--------------------------------------------------------------------------------------------------------------------
			if(  0/*error*/ == workbook_code_page)//|| 65001 /*UTF-8*/ == workbook_code_page || 1200/* UTF-16 */ == workbook_code_page
			{
				workbook_code_page = XLS::WorkbookStreamObject::DefaultCodePage;
			}

			xls_global_info = boost::shared_ptr<XLS::GlobalWorkbookInfo>(new XLS::GlobalWorkbookInfo(workbook_code_page, this));
			
			xls_global_info->lcid_user		= lcid_user;
			xls_global_info->fontsDirectory = fontsPath;
			xls_global_info->password		= password;
			xls_global_info->tempDirectory	= tempPath;
//--------------------------------------------------------------------------------------------------------------------
			XLS::StreamCacheReaderPtr workbook_stream(new XLS::CFStreamCacheReader(xls_file->getWorkbookStream(), xls_global_info));
			xls_document = boost::shared_ptr<XLS::WorkbookStreamObject>(new XLS::WorkbookStreamObject(workbook_code_page));		
			XLS::BinReaderProcessor proc(workbook_stream, xls_document.get() , true);
			proc.mandatory(*xls_document.get());

			if (xls_global_info->decryptor)
			{
				is_encrypted = true;
				if (xls_global_info->decryptor->IsVerify() == false) return;
			}
//--------------------------------------------------------------------------------------------------------------------
			XLS::EncryptionSummaryStream encryption_summary(xls_file->getNamedStream(L"encryption"), xls_global_info->decryptor);
//--------------------------------------------------------------------------------------------------------------------
			if (xls_file->storage_->isDirectory(L"_SX_DB_CUR"))
			{
				std::list<std::wstring> listStream = xls_file->storage_->entries(L"_SX_DB_CUR");

				int last_index = 0;
				for (std::list<std::wstring>::iterator it = listStream.begin(); it != listStream.end(); ++it)
				{
					XLS::CFStreamPtr pivot_cache_stream = xls_file->getNamedStream(L"_SX_DB_CUR/" + *it);
					
					if (!pivot_cache_stream) continue;
					//if (pivot_cache_stream->getStreamSize() < 1) continue;

					XLS::StreamCacheReaderPtr pivot_cache_reader(new XLS::CFStreamCacheReader(pivot_cache_stream, xls_global_info));

					XLS::BaseObjectPtr pivot_cache = boost::shared_ptr<XLS::PIVOTCACHE>(new XLS::PIVOTCACHE());
					
					XLS::BinReaderProcessor proc(pivot_cache_reader , pivot_cache.get() , true);
					proc.mandatory(*pivot_cache.get());

					int index = XmlUtils::GetHex(*it); //hexadecimal digits uniquely identifying

					xls_global_info->mapPivotCacheStream.insert(std::make_pair(index, pivot_cache));

					last_index = index;
				}
			}
			if (bMacros)
			{
				if (xls_file->storage_->isDirectory(L"_VBA_PROJECT_CUR"))
				{
					// if false == global_info_->bVbaProjectExist ??

					std::wstring xl_path = xlsx_path + FILE_SEPARATOR_STR + L"xl";
					NSDirectory::CreateDirectory(xl_path.c_str());

					std::wstring sVbaProjectFile = xl_path + FILE_SEPARATOR_STR + L"vbaProject.bin";

					POLE::Storage *storageVbaProject = new POLE::Storage(sVbaProjectFile.c_str());

					if ((storageVbaProject) && (storageVbaProject->open(true, true)))
					{
						xls_file->copy(0, L"_VBA_PROJECT_CUR/", storageVbaProject, false);

						storageVbaProject->close();
						delete storageVbaProject;

						output_document->get_xl_files().add_vba_project();
					}
				}
				else if (xls_global_info->bMacrosExist)
					output_document->get_xl_files().set_macros_enabled();
				else 
					bMacros = false;
			}

			XLS::CFStreamPtr controls = xls_file->getNamedStream(L"Ctls");
			if(controls)
			{
				unsigned long size = controls->getStreamSize();
				boost::shared_array<BYTE> buffer(new BYTE[size]);
				
				controls->read(buffer.get(), size);

				xls_global_info->controls_data = std::make_pair(buffer, size);			
			}
			else
			{
				xls_global_info->controls_data = encryption_summary.GetStream(L"Ctls");
			}
			XLS::CFStreamPtr listdata = xls_file->getNamedStream(L"List Data");
			if(listdata)
			{
				unsigned long size = listdata->getStreamSize();
				boost::shared_array<BYTE> buffer(new BYTE[size]);
				
				listdata->read(buffer.get(), size);

				xls_global_info->listdata_data = std::make_pair(buffer, size);			
			}
			else
			{
				xls_global_info->listdata_data = encryption_summary.GetStream(L"List Data");
			}
			if (xls_file->storage_->isDirectory(L"MsoDataStore"))
			{
				std::list<std::wstring> msoStores = xls_file->storage_->entries(L"MsoDataStore");

				int index = 0;
				for (std::list<std::wstring>::iterator it = msoStores.begin(); it != msoStores.end(); ++it)
				{
					XLS::CFStreamPtr item_stream  = xls_file->getNamedStream(L"MsoDataStore/" + *it + L"/Item");
					XLS::CFStreamPtr props_stream = xls_file->getNamedStream(L"MsoDataStore/" + *it + L"/Properties");

					if (!item_stream || !props_stream) continue;
					
					unsigned long item_size = item_stream->getStreamSize();
					unsigned long props_size = props_stream->getStreamSize();

					oox::package::customXml_content_ptr content = oox::package::customXml_content::create();
		
					char *item_buffer = new char[item_size];
					if (item_buffer)
					{
						item_stream->read(item_buffer, item_size);				
						content->set_item(item_buffer, item_size);
						delete []item_buffer;
					}
					char *props_buffer = new char[props_size];

					if (props_buffer)
					{
						props_stream->read(props_buffer, props_size);
						content->set_props(props_buffer, props_size);
						delete []props_buffer;
					}
					output_document->add_customXml(content);	
				}
			}
			XLS::CFStreamPtr toolbar_data = xls_file->getNamedStream(L"XCB");
			if(toolbar_data)
			{
				std::wstring xl_path = xlsx_path + FILE_SEPARATOR_STR + L"xl";	
				NSDirectory::CreateDirectory(xl_path.c_str());

				std::wstring sToolbarsFile = xl_path + FILE_SEPARATOR_STR + L"attachedToolbars.bin";
	
				NSFile::CFileBinary file;
				if (file.CreateFileW(sToolbarsFile))
				{
					unsigned long size = toolbar_data->getStreamSize();
					boost::shared_array<BYTE> buffer(new BYTE[size]);
					
					toolbar_data->read(buffer.get(), size);
					file.WriteFile(buffer.get(), size);
					file.CloseFile();

					output_document->get_xl_files().add_attachedToolbars();
				}
			}
		}
	}
	catch(...)
	{
		return;
	}

	if (xls_global_info->Version < 0x0600) 
	{
		std::wstring sVer = STR::int2hex_wstr(xls_global_info->Version);
		Log::error("Version xls is old !!! - " + std::string(sVer.begin(), sVer.end()));
		is_older_version = true;
	}	
    xlsx_context = new oox::xlsx_conversion_context(output_document);
}

XlsConverter::~XlsConverter() 
{
	if (xlsx_context)		delete xlsx_context;
	if (output_document)	delete output_document;
}

bool XlsConverter::isError()
{
	if (!xlsx_context)		return true;
	if (!output_document)	return true;

	return false;
}

void XlsConverter::write()
{
	if (!output_document)return;
	
	output_document->write(xlsx_path);

	delete output_document; output_document = NULL;
}

void XlsConverter::convertDocument()
{
    if (!xls_document)     return;
    if (!output_document)   return;
    if (!xlsx_context)       return;

	xlsx_context->start_document();

	convert((XLS::WorkbookStreamObject*)xls_document.get());

	xlsx_context->end_document();
}

void XlsConverter::convert(XLS::BaseObject	*xls_unknown)
{
	if (xls_unknown == NULL) return;

	XLS::ElementType type = xls_unknown->get_type();

	switch (type)
	{
	case XLS::typeHLINK:
		{
			XLS::HLINK * hlink = dynamic_cast<XLS::HLINK*>(xls_unknown);
			convert(hlink);
		}break;
	case XLS::typeLBL:	
		{
			XLS::LBL * lbl = dynamic_cast<XLS::LBL*>(xls_unknown);
			convert(lbl);
		}break;
	case XLS::typeOBJECTS:
		{
			XLS::OBJECTS * obj = dynamic_cast<XLS::OBJECTS*>(xls_unknown);
			convert(obj, NULL);
		}break;
	case XLS::typeTxO:
		{
			XLS::TxO * txo = dynamic_cast<XLS::TxO*>(xls_unknown);
			convert(txo);
		}break;
	case XLS::typeObj:
		{
			XLS::Obj * obj = dynamic_cast<XLS::Obj*>(xls_unknown);
			convert(obj);
		}break;
	case XLS::typeAnyObject:	
	default:
		{
			for (std::list<XLS::BaseObjectPtr>::iterator it = xls_unknown->elements_.begin(); it != xls_unknown->elements_.end(); ++it)
			{
				(*it)->serialize(xlsx_context->current_sheet().sheetData());
			}
		}
	}

}

void XlsConverter::convert(XLS::WorkbookStreamObject* woorkbook)
{
	if (woorkbook == NULL) return;

	convert(dynamic_cast<XLS::GlobalsSubstream*>(woorkbook->m_GlobalsSubstream.get()));

    for (size_t i = 0 ; i < woorkbook->m_arWorksheetSubstream.size(); i++)
	{
		xlsx_context->start_sheet();
			convert(dynamic_cast<XLS::WorksheetSubstream*>(woorkbook->m_arWorksheetSubstream[i].get()));
		xlsx_context->end_sheet();
	}
	for (size_t i = 0 ; i < woorkbook->m_arChartSheetSubstream.size(); i++)
	{
		xlsx_context->start_sheet();
			convert_chart_sheet(dynamic_cast<XLS::ChartSheetSubstream*>(woorkbook->m_arChartSheetSubstream[i].get()));
		xlsx_context->end_sheet();
	}
	for (size_t i = 0 ; i < woorkbook->m_arMacroSheetSubstream.size(); i++)
	{
		xlsx_context->start_sheet();
			convert(dynamic_cast<XLS::MacroSheetSubstream*>(woorkbook->m_arMacroSheetSubstream[i].get()));
		xlsx_context->end_sheet();
	}

	for (std::list<XLS::BaseObjectPtr>::iterator it = woorkbook->elements_.begin(); it != woorkbook->elements_.end(); ++it)
	{
		convert(it->get());
	}

	xlsx_context->add_connections(xls_global_info->connections_stream.str());
}

void XlsConverter::convert_common (XLS::CommonSubstream* sheet)
{
	if (sheet == NULL) return;
	
	xls_global_info->current_sheet = sheet->ws_index_ + 1; 
		
	XLS::GLOBALS *globals = dynamic_cast<XLS::GLOBALS *>(sheet->m_GLOBALS.get());
	
	if (globals)
	{
		globals->serialize_formatPr(xlsx_context->current_sheet().sheetFormat());
		
		if (xlsx_context->workbook_calcpr().rdbuf()->in_avail() == 0)
			globals->serialize_calcPr(xlsx_context->workbook_calcpr());
	}
	
	if (!sheet->m_arWINDOW.empty())
	{
		sheet->m_arWINDOW[0]->serialize(xlsx_context->current_sheet().sheetViews());
	}	
	
	sheet->serialize_format(xlsx_context->current_sheet().sheetProperties());

	if (sheet->m_PROTECTION)
	{
		sheet->m_PROTECTION->serialize(xlsx_context->current_sheet().protection());
	}
	if (sheet->m_COLUMNS)
	{
		sheet->m_COLUMNS->serialize(xlsx_context->current_sheet().cols());
	}
	if (sheet->m_CELLTABLE)
	{
		sheet->m_CELLTABLE->serialize(xlsx_context->current_sheet().sheetData());
	}
	if (sheet->m_SORTANDFILTER)
	{
		sheet->m_SORTANDFILTER->serialize(xlsx_context->current_sheet().sheetSortAndFilters());
	}	
	if (sheet->m_PAGESETUP)
	{
		sheet->m_PAGESETUP->serialize(xlsx_context->current_sheet().pageProperties());
	}
	if (globals)
	{
		if (globals->m_HorizontalPageBreaks)
		{
			globals->m_HorizontalPageBreaks->serialize(xlsx_context->current_sheet().pageProperties());
		}
		if (globals->m_VerticalPageBreaks)
		{
			globals->m_VerticalPageBreaks->serialize(xlsx_context->current_sheet().pageProperties());
		}
		//if (globals->m_CalcMode)
		//{
		//	CP_XML_WRITER(xlsx_context->current_sheet().sheetCalcPr())
		//	{
		//		CP_XML_NODE(L"sheetCalcPr")
		//		{
		//			XLS::CalcMode *calcMode = dynamic_cast<XLS::CalcMode *>(globals->m_CalcMode.get());
		//			CP_XML_ATTR(L"fullCalcOnLoad", calcMode->nAutoRecalc > 0 ? 1 : 0);					
		//		}
		//	}
		//}
	}

	if (sheet->m_arCUSTOMVIEW.size() > 0)
	{
		CP_XML_WRITER(xlsx_context->current_sheet().customViews())    
		{
			CP_XML_NODE(L"customSheetViews")
            {
				for (size_t i = 0 ; i < sheet->m_arCUSTOMVIEW.size(); i++)
				{
					sheet->m_arCUSTOMVIEW[i]->serialize(CP_XML_STREAM());
				}
			}
		}
	}

	if (sheet->m_BACKGROUND)
	{
		convert(dynamic_cast<XLS::BACKGROUND*>(sheet->m_BACKGROUND.get()));
	}

	for (size_t i = 0 ; i < sheet->m_arHFPictureDrawing.size(); i++)
	{
		convert((ODRAW::OfficeArtDgContainer*)sheet->m_arHFPictureDrawing[i].get());
	}

	//for (size_t i = 0 ; i < sheet->m_arFEAT.size(); i++)
	//{
	//	convert(dynamic_cast<XLS::FEAT*>(sheet->m_arFEAT.get()));
	//}

	for (size_t i = 0 ; i < sheet->m_arFEAT11.size(); i++)
	{
		convert(dynamic_cast<XLS::FEAT11*>(sheet->m_arFEAT11[i].get()));
	}
}

void XlsConverter::convert (XLS::WorksheetSubstream* sheet)
{
	if (sheet == NULL) return;
	
	std::wstring name = xls_global_info->sheets_info[sheet->ws_index_].name;
	if (name.empty()) 
		name = L"Sheet_" + std::to_wstring(sheet->ws_index_ + 1);

	xlsx_context->set_sheet_type(1);
	xlsx_context->set_sheet_name(name) ;
	xlsx_context->set_sheet_id(sheet->ws_index_ + 1);
	xlsx_context->set_sheet_state(xls_global_info->sheets_info[sheet->ws_index_].state);

	if (sheet->m_GLOBALS)
	{
		XLS::GLOBALS * globals  = dynamic_cast<XLS::GLOBALS *>(sheet->m_GLOBALS.get());
		XLS::COLUMNS * columns = dynamic_cast<XLS::COLUMNS *>(sheet->m_COLUMNS.get());

		if (columns)
		{
			globals->m_DefColWidth	= columns->m_DefColWidth; 
		}
		globals->m_DxGCol = sheet->m_DxGCol;		

		if (globals->is_dialog)
			xlsx_context->set_sheet_type(2);
	}

	convert_common(dynamic_cast<XLS::CommonSubstream*>(sheet));

	convert((XLS::OBJECTS*)sheet->m_OBJECTS.get(), sheet);

	if (sheet->m_Dimensions)
	{
		sheet->m_Dimensions->serialize(xlsx_context->current_sheet().dimension());
	}	
	if (sheet->m_arMergeCells.size() > 0)
	{
		CP_XML_WRITER(xlsx_context->current_sheet().mergeCells())    
		{
			CP_XML_NODE(L"mergeCells")
			{  		
                for (size_t i = 0 ; i < sheet->m_arMergeCells.size(); i++)
				{
					sheet->m_arMergeCells[i]->serialize(CP_XML_STREAM());
				}
			}
		}
	}
    for (size_t i = 0 ; i < sheet->m_arHLINK.size(); i++)
	{
		convert((XLS::HLINK*)sheet->m_arHLINK[i].get());
	}
	if (sheet->m_CONDFMTS)
	{
		sheet->m_CONDFMTS->serialize(xlsx_context->current_sheet().conditionalFormatting());
	}
	if (sheet->m_DVAL)
	{
		sheet->m_DVAL->serialize(xlsx_context->current_sheet().dataValidations());
	}
	if (!sheet->m_arNote.empty() && xls_global_info->Version < 0x0600)
	{
		xlsx_context->get_drawing_context().start_drawing(0);
		for (size_t i = 0 ; i < sheet->m_arNote.size(); i++)
		{
			xlsx_context->get_drawing_context().start_drawing(0x0019);
				convert(dynamic_cast<XLS::Note*>(sheet->m_arNote[i].get()));
			xlsx_context->get_drawing_context().end_drawing();
		}
		xlsx_context->get_drawing_context().end_group();
	}

	for (size_t i = 0; i < sheet->m_arQUERYTABLE.size(); i++)
	{
		convert(dynamic_cast<XLS::QUERYTABLE*>(sheet->m_arQUERYTABLE[i].get()));
	}
	for (size_t i = 0; i < sheet->m_arPIVOTVIEW.size(); i++)
	{
		convert((XLS::PIVOTVIEW*)sheet->m_arPIVOTVIEW[i].get());
	}
}

void XlsConverter::convert (XLS::MacroSheetSubstream* sheet)
{
	if (sheet == NULL) return;

	xls_global_info->current_sheet = sheet->ws_index_ + 1; 
	
	std::wstring name = xls_global_info->sheets_info[sheet->ws_index_].name;
	if (name.empty()) 
		name = L"MacroSheet_" + std::to_wstring(sheet->ws_index_ + 1);

	xlsx_context->set_sheet_type(4);
	xlsx_context->set_sheet_name(name) ;
	xlsx_context->set_sheet_id(sheet->ws_index_ + 1);
	xlsx_context->set_sheet_state(xls_global_info->sheets_info[sheet->ws_index_].state);

	if (sheet->m_GLOBALS)
	{
		XLS::GLOBALS * globals  = dynamic_cast<XLS::GLOBALS *>(sheet->m_GLOBALS.get());
		XLS::COLUMNS * columns = dynamic_cast<XLS::COLUMNS *>(sheet->m_COLUMNS.get());

		if (columns)
		{
			globals->m_DefColWidth	= columns->m_DefColWidth; 
		}
		globals->m_DxGCol = sheet->m_DxGCol;		

		if (globals->is_dialog)
			xlsx_context->set_sheet_type(2);
	}
	convert_common(dynamic_cast<XLS::CommonSubstream*>(sheet));

	convert((XLS::OBJECTS*)sheet->m_OBJECTS.get(), NULL);

	if (sheet->m_Dimensions)
	{
		sheet->m_Dimensions->serialize(xlsx_context->current_sheet().dimension());
	}
	
	if (!sheet->m_arNote.empty() && xls_global_info->Version < 0x0600)
	{
		xlsx_context->get_drawing_context().start_drawing(0);
		for (size_t i = 0 ; i < sheet->m_arNote.size(); i++)
		{
			xlsx_context->get_drawing_context().start_drawing(0x0019);
				convert(dynamic_cast<XLS::Note*>(sheet->m_arNote[i].get()));
			xlsx_context->get_drawing_context().end_drawing();
		}
		xlsx_context->get_drawing_context().end_group();
	}
}

void XlsConverter::convert(XLS::GlobalsSubstream* globals)
{
	if (globals == NULL) return;
	
	convert_theme();
	
	convert((XLS::FORMATTING*)globals->m_Formating.get());	

	convert((XLS::SHAREDSTRINGS*)globals->m_SHAREDSTRINGS.get());

    for (size_t i = 0 ; i < globals->m_arLBL.size(); i++)
	{
		convert((XLS::LBL*)globals->m_arLBL[i].get());
	}

    for (size_t i = 0 ; i < globals->m_arMSODRAWINGGROUP.size(); i++)
	{
		convert((XLS::MSODRAWINGGROUP*)globals->m_arMSODRAWINGGROUP[i].get());
	}

	for (size_t i = 0 ; i < globals->m_arHFPictureDrawing.size(); i++)
	{
		convert((ODRAW::OfficeArtDgContainer*)globals->m_arHFPictureDrawing[i].get());
	}
	globals->serialize_protection(xlsx_context->workbook_protection());
	
	globals->serialize_format(xlsx_context->workbook_format());

    for (size_t i = 0 ; i < globals->m_arWindow1.size(); i++)
	{
		globals->m_arWindow1[i]->serialize(xlsx_context->workbook_views());
	}
    for (size_t i = 0 ; i < globals->m_arUserBView.size(); i++)
	{
		globals->m_arUserBView[i]->serialize(xlsx_context->custom_views());
	}

	for (size_t i = 0 ; i < globals->m_arSUPBOOK.size(); i++)
	{
		convert((XLS::SUPBOOK*)globals->m_arSUPBOOK[i].get());
	}

	for (size_t i = 0 ; i < xls_global_info->arPIVOTCACHEDEFINITION.size(); i++)
	{
		convert((XLS::PIVOTCACHEDEFINITION*)xls_global_info->arPIVOTCACHEDEFINITION[i].get());
	}
}

void XlsConverter::convert_chart_sheet(XLS::ChartSheetSubstream* chartsheet)
{
	if (chartsheet == NULL) return;
		
	xls_global_info->current_sheet = chartsheet->ws_index_ + 1; 
	
	std::wstring name = xls_global_info->sheets_info[chartsheet->ws_index_].name;
	if (name.empty()) 
		name = L"ChartSheet_" + std::to_wstring(chartsheet->ws_index_ + 1);

	xlsx_context->set_sheet_type(3);
	xlsx_context->set_sheet_name(name) ;
	xlsx_context->set_sheet_id(chartsheet->ws_index_ + 1);
	xlsx_context->set_sheet_state(xls_global_info->sheets_info[chartsheet->ws_index_].state);

	convert_common(dynamic_cast<XLS::CommonSubstream*>(chartsheet));

	if (xlsx_context->get_drawing_context().start_drawing(0x0005))		
	{
		xlsx_context->get_drawing_context().set_id(1);
		convert(chartsheet);

		xlsx_context->get_drawing_context().end_drawing();
	}
}
typedef boost::unordered_map<XLS::FillInfo, int>	mapFillInfo;
typedef boost::unordered_map<XLS::BorderInfo, int>	mapBorderInfo;
typedef boost::unordered_map<XLS::FontInfo, int>	mapFontInfo;

void XlsConverter::convert(XLS::FORMATTING* formating)
{
	if (formating == NULL) return;

	std::wstringstream strm;
    CP_XML_WRITER(strm)    
    {
        CP_XML_NODE(L"styleSheet")
        {   
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
			CP_XML_ATTR(L"xmlns:mc", L"http://schemas.openxmlformats.org/markup-compatibility/2006");
			CP_XML_ATTR(L"mc:Ignorable", L"x14ac x16r2");
			CP_XML_ATTR(L"xmlns:x14ac", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac");
			CP_XML_ATTR(L"xmlns:x16r2", L"http://schemas.microsoft.com/office/spreadsheetml/2015/02/main");
			
			CP_XML_NODE(L"numFmts")
			{
				CP_XML_ATTR(L"count", xls_global_info->m_arNumFormats.size());
				for (size_t i = 0; i < xls_global_info->m_arNumFormats.size(); i++)
				{
					XLS::Format* fmt = dynamic_cast<XLS::Format*>(xls_global_info->m_arNumFormats[i].get());

					if (fmt->ifmt < 5 || (fmt->ifmt > 8 && fmt->ifmt < 23) || (fmt->ifmt > 36 && fmt->ifmt < 41) || (fmt->ifmt > 44 && fmt->ifmt < 50))
						continue;

					std::map<_UINT16, bool>::iterator pFind = xls_global_info->mapUsedFormatCode.find(fmt->ifmt);

					if (pFind != xls_global_info->mapUsedFormatCode.end())
					{
						CP_XML_STREAM() << L"<numFmt";
						{
							CP_XML_STREAM() << L" numFmtId=\"" << fmt->ifmt << L"\"";
							CP_XML_STREAM() << L" formatCode=\"" << fmt->stFormat << L"\"";
						}
						CP_XML_STREAM() << L"/>";
					}
				}
			}
			CP_XML_NODE(L"fonts")
			{
				std::vector<XLS::FontInfo> fonts_out;
				fonts_out.resize(xls_global_info->font_x_ids.size());

				for (mapFontInfo::iterator it = xls_global_info->font_x_ids.begin(); it != xls_global_info->font_x_ids.end(); ++it)
				{
					fonts_out[it->second] = it->first;
				}

				CP_XML_ATTR(L"count", fonts_out.size());
				for (size_t i = 0 ;i < fonts_out.size(); i++)
				{
					fonts_out[i].serialize(CP_XML_STREAM());
				}
			}
			CP_XML_NODE(L"fills")
			{
				std::vector<XLS::FillInfo> fills_out;
				fills_out.resize(xls_global_info->fill_x_ids.size());

				for (mapFillInfo::iterator it = xls_global_info->fill_x_ids.begin(); it != xls_global_info->fill_x_ids.end(); ++it)
				{
					fills_out[it->second] = it->first;
				}

				CP_XML_ATTR(L"count", fills_out.size());
				for (size_t i = 0 ;i < fills_out.size(); i++)
				{
					fills_out[i].serialize(CP_XML_STREAM());
				}
			}
			CP_XML_NODE(L"borders")
			{				
				std::vector<XLS::BorderInfo> borders_out;
				borders_out.resize(xls_global_info->border_x_ids.size());

				for (mapBorderInfo::iterator it = xls_global_info->border_x_ids.begin(); it != xls_global_info->border_x_ids.end(); ++it)
				{
					borders_out[it->second] = it->first;
				}
				
				CP_XML_ATTR(L"count", borders_out.size());
				for (size_t i = 0 ;i < borders_out.size(); i++)
				{
					borders_out[i].serialize(CP_XML_STREAM());
				}
			}

			formating->serialize(CP_XML_STREAM());
		}
	}
    
	output_document->get_xl_files().set_styles( oox::package::simple_element::create(L"styles.xml", strm.str()) );
}

std::wstring XlsConverter::GetTargetMoniker(XLS::BiffStructure *moniker)
{
	if (moniker == NULL) return L"";

    if (moniker->getClassName() == "URLMoniker")
	{
		OSHARED::URLMoniker* urlMoniker = dynamic_cast<OSHARED::URLMoniker* >(moniker);
		if (urlMoniker)return urlMoniker->url;
	}
    else if (moniker->getClassName() == "FileMoniker")
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

	//files 
	convert (dynamic_cast<ODRAW::OfficeArtBStoreContainer*>(mso_group->rgChildRec.m_OfficeArtBStoreContainer.get()));

		
}

std::wstring XlsConverter::WriteMediaFile(char *data, int size, std::wstring type_ext, int id)
{
	if (size < 1 || !data) return L"";
	
	if (id < 0)		id = xlsx_context->get_mediaitems().count_image + 1000; // 1000 - встроенные в поток , 3000 - footer/header

	xlsx_context->get_mediaitems().create_media_path(xlsx_path);

	bool res = false;
	std::wstring file_name = L"image" + std::to_wstring(id);
	
	if (type_ext == L"dib_data")
	{
		bool bPNG = false;
		int offset = 0, biSizeImage = 0;
		CBgraFrame frame;

		BITMAPINFOHEADER * header = (BITMAPINFOHEADER *)data;

		if (header->biWidth > 100000 || header->biHeight > 100000)
		{
			//biff8
			//Formulas Matriciais - A Outra Dimensão do Excel.xls	775x20 		todoooo найти еще файлы 
			//Planilha Bastter Blue 7.0 Free.xls 10x3836
			//biff 5
			//test-picture.xls
			offset = 12; //sizeof(BITMAPCOREHEADER)
			
			BITMAPCOREHEADER * header_core = (BITMAPCOREHEADER *)data;
		
			frame.put_Height	(header_core->bcHeight );
			frame.put_Width		(header_core->bcWidth );
			
			int sz_bitmap = header_core->bcHeight * header_core->bcWidth * header_core->bcBitCount/ 8;
			
			//if (header_core->bcWidth % 2 != 0 && sz_bitmap < size - offset)
			//	header_core->bcWidth++;
			///???? todooo непонятно .. в biff5 нужно флипать картинку, в biff8 не ясно ( - 
			
			int stride = - (size - offset) / header_core->bcHeight;
			frame.put_Stride	(stride/*header_core->bcBitCount * header_core->bcWidth /8 */);

			biSizeImage = size - offset;
			
			if (-stride >= header_core->bcWidth && header_core->bcBitCount >=24 )
			{
				bPNG = true;
			}
		}
		else
		{
			offset = 40; //sizeof(BITMAPINFOHEADER)

			frame.put_Height	(header->biHeight );
			frame.put_Width		(header->biWidth );
			
			int sz_bitmap = header->biHeight * header->biWidth * header->biBitCount/ 8;
			
			//if (header->biWidth % 2 != 0 && sz_bitmap < size -offset)
			//	header->biWidth++;
			
			int stride =  -(size - offset) / header->biHeight;

			if (-stride >= header->biWidth && header->biBitCount >= 24)
			{
				bPNG = true;
			}
			frame.put_Stride	(stride/*header->biBitCount * header->biWidth /8*/);
			
			biSizeImage = header->biSizeImage;
		}
		if (bPNG)
		{
			frame.put_Data((unsigned char*)data + offset);
			
			file_name += std::wstring(L".png");
			
			res = frame.SaveFile(xlsx_context->get_mediaitems().media_path() + file_name, 4/*CXIMAGE_FORMAT_PNG*/);
			frame.put_Data(NULL);

			if (res = false)
			{
				//
			}
		}
		else if (biSizeImage > 0)
		{
			//тут паттерные картинки
			file_name += std::wstring(L".bmp");
			NSFile::CFileBinary file;
			if (file.CreateFileW(xlsx_context->get_mediaitems().media_path() + file_name))
			{
				WORD vtType		= 0x4D42;				file.WriteFile((BYTE*)&vtType,	2);
				DWORD dwLen		= biSizeImage;			file.WriteFile((BYTE*)&dwLen,	4);
				DWORD dwRes		= 0;					file.WriteFile((BYTE*)&dwRes,	4);
				DWORD dwOffset	= 2;					file.WriteFile((BYTE*)&dwOffset, 4);
			
				file.WriteFile((BYTE*)data, size);
				file.CloseFile();
				res = true;
			}
		}
	}
	else
	{
		file_name += type_ext;

		NSFile::CFileBinary file;
		if (file.CreateFileW(xlsx_context->get_mediaitems().media_path() + file_name))
		{		
			file.WriteFile((BYTE*)data, size);
			file.CloseFile();
		}
	}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
	res = true;
	if (res)
	{
		xlsx_context->get_mediaitems().add_image(L"media/" + file_name, id);
		return L"media/" + file_name;
	}
	return L"";
}

void XlsConverter::convert(ODRAW::OfficeArtBStoreContainer* art_bstore, int start_id)
{
	if (art_bstore == NULL) return;
	if (art_bstore->rgfb.size() < 1) return;

    for (size_t i = 0 ; i < art_bstore->rgfb.size(); i++)
	{
		int bin_id = i + start_id + 1;

		WriteMediaFile(art_bstore->rgfb[i]->pict_data, art_bstore->rgfb[i]->pict_size, art_bstore->rgfb[i]->pict_type, bin_id);
	}
}
void XlsConverter::convert(XLS::FEAT11 * shared_feature)
{
	if (!shared_feature) return;

	for (size_t i = 0; i < shared_feature->m_arFEAT.size(); i++)
	{
		xlsx_context->start_table();

		std::wstringstream strm;
		shared_feature->serialize(strm, i);

		xlsx_context->get_tables_context().add_table(strm.str());

		xlsx_context->end_table();	
	}
}

void XlsConverter::convert(XLS::HLINK * HLINK_)
{
	if (!HLINK_) return;
	XLS::HLink * hLink = dynamic_cast<XLS::HLink*>(HLINK_->m_HLink.get());
	if (!hLink) return;

	std::wstring target;
	
	bool bExternal = false;
	
	if (hLink->hyperlink.hlstmfHasMoniker)
	{
		target = GetTargetMoniker(hLink->hyperlink.oleMoniker.data.get());
		bExternal = true;
	}
	else if (hLink->hyperlink.hlstmfHasLocationStr)
	{
		target = hLink->hyperlink.location;
	}

	std::wstring display = hLink->hyperlink.displayName;

	if (display.empty())	display = target;

	xlsx_context->get_sheet_context().add_hyperlink( hLink->ref8.toString(), target, display, bExternal);
}

void XlsConverter::convert(XLS::LBL * def_name)
{
	if (def_name == NULL) return;

	def_name->serialize(xlsx_context->defined_names());
}
void XlsConverter::convert(XLS::IMDATA * imdata)
{
	if (imdata == NULL) return;

	if (imdata->lcb < 1 || imdata->pData == NULL) return;
	
	bool bInternal = false;

	std::wstring type_image;

	if (imdata->cf == 0x09 && imdata->env == 0x01)	type_image = L".wmf";
	if ((imdata->cf == 0x09 || imdata->cf == 0x02)
							&& imdata->env == 0x02)	type_image = L".pict";
	if (imdata->cf == 0x09)							type_image = L"dib_data";
	if (imdata->cf == 0x0e)							type_image = L"";			//native aka unknown

	std::wstring target = WriteMediaFile(imdata->pData.get(), imdata->lcb, type_image);
	xlsx_context->get_drawing_context().set_fill_texture(target);

}

void XlsConverter::convert(XLS::BACKGROUND * back)
{
	if (back == NULL) return;

	XLS::BkHim * bkHim = dynamic_cast<XLS::BkHim*>(back->m_BkHim.get());
	
	if (bkHim->lcb < 1 || bkHim->pData == NULL) return;
	
	bool bInternal = false;

	std::wstring target = WriteMediaFile(bkHim->pData.get(), bkHim->lcb, L"dib_data");

	std::wstring rId = xlsx_context->get_mediaitems().find_image(target, bInternal);
	
	xlsx_context->current_sheet().sheet_rels().add(oox::relationship(rId, oox::external_items::typeImage, std::wstring(L"../") + target, !bInternal));

	if (rId.empty()) return;

	CP_XML_WRITER(xlsx_context->current_sheet().picture_background())    
	{
		CP_XML_NODE(L"picture")
		{
			CP_XML_ATTR(L"r:id", rId);
		}
	}

}

void XlsConverter::convert_theme()
{
	if (xls_global_info->sTheme.empty()) return;
	
	oox::package::theme_content_ptr content = 
		oox::package::theme_content::create((char*)xls_global_info->sTheme.c_str(), xls_global_info->sTheme.length());
	output_document->get_xl_files().add_theme(content);
}

struct _group_object
{
	_group_object() {spgr = NULL; ind = count = 0;}
	ODRAW::OfficeArtSpgrContainer	*spgr;
	int ind;
	int count;
};
void XlsConverter::convert_old(XLS::OBJECTS* objects, XLS::WorksheetSubstream * sheet)
{
	int count = 0;
	
	for ( std::list<XLS::BaseObjectPtr>::iterator elem = objects->elements_.begin(); elem != objects->elements_.end(); elem++)
	{
		count++;
		short type_object = -1;

		XLS::OBJ			* OBJ			= dynamic_cast<XLS::OBJ*>		(elem->get());
		XLS::IMDATAOBJECT	* IMDATAOBJECT	= NULL;
		XLS::CHART			* CHART			= NULL;
		
		std::list<XLS::BaseObjectPtr>::iterator elem_next = boost::next(elem);
		if ( elem_next !=objects->elements_.end() )
		{
			IMDATAOBJECT	= dynamic_cast<XLS::IMDATAOBJECT*>	(elem_next->get());
			CHART			= dynamic_cast<XLS::CHART*>			(elem_next->get());
		}

		XLS::Obj					* obj		= OBJ			? dynamic_cast<XLS::Obj *>(OBJ->m_Obj.get()) : NULL;
		XLS::IMDATA					* image_obj	= IMDATAOBJECT	? dynamic_cast<XLS::IMDATA *>(IMDATAOBJECT->m_IMDATA.get())	: NULL;
		XLS::ChartSheetSubstream	* chart		= CHART			? dynamic_cast<XLS::ChartSheetSubstream *>(CHART->m_ChartSheetSubstream.get()) : NULL;		
		
		if (!obj)continue;
		
		type_object = obj->cmo.ot;
//-----------------------------------------------------------------------------
		if (type_object < 0)continue;
		
		if (type_object == 0) 
			continue;

		if (obj->cmo.fUIObj)
		{
			continue; // automatically inserted by the application
		}
		if (xlsx_context->get_drawing_context().start_drawing(type_object))
		{
			convert(obj->old_version.anchor.get());

			if (obj->old_version.bFill)
				xlsx_context->get_drawing_context().set_fill_old_version(obj->old_version.fill);
			else 
				xlsx_context->get_drawing_context().set_fill_type(oox::fillNone);	//no fill
			
			xlsx_context->get_drawing_context().set_name(obj->old_version.name);
			xlsx_context->get_drawing_context().set_line_old_version(obj->old_version.line);
			xlsx_context->get_drawing_context().set_flag_old_version(obj->old_version.flag, obj->old_version.flag2);

			for (size_t i = 0 ; i < obj->old_version.additional.size(); i++)
			{
				convert(obj->old_version.additional[i].get());
			}
			convert(image_obj);
			convert(chart);
			convert(obj);

			xlsx_context->get_drawing_context().end_drawing();
		}
		if (IMDATAOBJECT || CHART)
		{
			elem++;
			count++;
		}	
	}
}
void XlsConverter::convert(XLS::OBJECTS* objects, XLS::WorksheetSubstream * sheet)
{
	if (objects == NULL) return;

	if (xls_global_info->Version < 0x0600)
		return convert_old(objects, sheet);

	std::vector<_group_object> group_objects;

	_group_object gr;		
	gr.spgr		= dynamic_cast<ODRAW::OfficeArtSpgrContainer*>(objects->m_MsoDrawing.get()->rgChildRec.m_OfficeArtSpgrContainer.get());
	
	if (gr.spgr	== NULL) 
		return;

	gr.count	= gr.spgr->child_records.size();
	group_objects.push_back(gr);

	if ((group_objects.back().spgr) && (group_objects.back().ind < group_objects.back().spgr->child_records.size()))
	{
		ODRAW::OfficeArtSpContainer *s	= dynamic_cast<ODRAW::OfficeArtSpContainer*>(group_objects.back().spgr->child_records[group_objects.back().ind].get());
		if (s)
		{
			ODRAW::OfficeArtFSP *fsp = dynamic_cast<ODRAW::OfficeArtFSP*>(s->m_OfficeArtFSP.get());
			if ((fsp) && (fsp->fPatriarch)) group_objects.back().ind++;
		}
	}
	int count = 0;
	
	for ( std::list<XLS::BaseObjectPtr>::iterator elem = objects->elements_.begin(); elem != objects->elements_.end(); elem++)
	{
		count++;
		short type_object = -1;

		XLS::OBJ		* OBJ			= dynamic_cast<XLS::OBJ*>		(elem->get());

		XLS::TEXTOBJECT	* TEXTOBJECT	= NULL;
		XLS::CHART		* CHART			= NULL;
		
		std::list<XLS::BaseObjectPtr>::iterator elem_next = boost::next(elem);
		if ( elem_next !=objects->elements_.end() )
		{
			TEXTOBJECT	= dynamic_cast<XLS::TEXTOBJECT*>(elem_next->get());
			CHART		= dynamic_cast<XLS::CHART*>		(elem_next->get());
		}

		XLS::Obj					* obj		= NULL;
		XLS::TxO					* text_obj	= NULL; 
		XLS::ChartSheetSubstream	* chart		= NULL;

		if (OBJ)		obj			= dynamic_cast<XLS::Obj *>(OBJ->m_Obj.get());		
		if (TEXTOBJECT) text_obj	= dynamic_cast<XLS::TxO *>(TEXTOBJECT->m_TxO.get());
		if (CHART)		chart		= dynamic_cast<XLS::ChartSheetSubstream *>(CHART->m_ChartSheetSubstream.get());
		
		if (obj)	type_object = obj->cmo.ot;
		if (chart)	type_object = 0x0005;

		if (text_obj)
		{ 
			if (type_object < 0) type_object = 0x0006;			
		}

//-----------------------------------------------------------------------------
		if (type_object < 0) continue;
		if (group_objects.empty())
			break; /// что то с объектами не то ! 2006 02.xls

		if (type_object == 0)
		{
			_group_object gr;		
			if (group_objects.back().ind < group_objects.back().spgr->child_records.size())
			{
				gr.spgr		= dynamic_cast<ODRAW::OfficeArtSpgrContainer*>(group_objects.back().spgr->child_records[group_objects.back().ind++].get());
				gr.count	= gr.spgr ? gr.spgr->child_records.size() : 0;
				group_objects.push_back(gr);
			}
			else //сюда попадать не должно !!!!
				continue;
		}
		if (obj->cmo.fUIObj)
		{
			group_objects.back().ind++;
			continue; // automatically inserted by the application
		}
		
		ODRAW::OfficeArtSpContainer *sp = NULL;

		if ((group_objects.size() > 0) && (group_objects.back().spgr ) && ( group_objects.back().ind < group_objects.back().count))
		{
			sp	= dynamic_cast<ODRAW::OfficeArtSpContainer*>(group_objects.back().spgr->child_records[group_objects.back().ind++].get());
		}	
		
		if (xlsx_context->get_drawing_context().start_drawing(type_object))
		{
			convert(sp);

			if ((!sp) || (!sp->m_OfficeArtAnchor))
			{
				if ((group_objects.size() > 0) && (group_objects.back().spgr ) && ( group_objects.back().ind < group_objects.back().count))
				{
					sp	= dynamic_cast<ODRAW::OfficeArtSpContainer*>(group_objects.back().spgr->child_records[group_objects.back().ind++].get());
				}
				convert(sp, true);
			}			
			if (type_object == 0x19)
			{	
				text_obj->preserve_enabled = true;

				for (size_t i = 0 ; sheet && i < sheet->m_arNote.size(); i++)
				{
					XLS::Note* note = dynamic_cast<XLS::Note*>(sheet->m_arNote[i].get());
					if ((note) && (note->note_sh.idObj == obj->cmo.id))
					{
						convert(note);
						break;
					}
				}
			}
			convert(text_obj);
			convert(chart);
			convert(obj);

			xlsx_context->get_drawing_context().end_drawing();
		}
		if (TEXTOBJECT || CHART)
		{
			elem++;
			count++;
		}		
		
		while ((group_objects.size() >0) && (group_objects.back().ind >= group_objects.back().count))
		{
			group_objects.back().spgr = NULL;
			group_objects.pop_back();
			
			xlsx_context->get_drawing_context().end_group();
		}
	}
}
void XlsConverter::convert(ODRAW::OfficeArtSpgrContainer * spgr)
{
	if (spgr == NULL) return;
	if (spgr->anchor_type_ != ODRAW::OfficeArtRecord::CA_HF) return;

	for (size_t i = 0; i < spgr->child_records.size(); i++)
	{
		int type_object = 2;//rect

		if (xlsx_context->get_drawing_context().start_drawing(type_object))
		{
			xlsx_context->get_drawing_context().set_mode_HF(true);
			convert(spgr->child_records[i].get());

			xlsx_context->get_drawing_context().end_drawing();
		}
	}
}
void XlsConverter::convert(ODRAW::OfficeArtSpContainer *sp, bool anchor_only)
{
	if (sp == NULL) return;

	if (!anchor_only)
	{
		convert(sp->m_OfficeArtFSP.get());

		for (size_t i = 0; i < sp->child_records.size(); i++)
		{
			convert(sp->child_records[i].get());
		}
	}
	convert(sp->m_OfficeArtAnchor.get());
}

void XlsConverter::convert(ODRAW::OfficeArtRecord * art)
{
	if (art == NULL) return;

	std::wstringstream strm;

	switch(art->get_type())
	{
	case XLS::typeOfficeArtFOPT://properties
		{
			convert(dynamic_cast<ODRAW::OfficeArtFOPT *>(art));
		}break;
	case XLS::typeOfficeArtTertiaryFOPT://properties
		{
			convert(dynamic_cast<ODRAW::OfficeArtTertiaryFOPT *>(art));
		}break;
	case XLS::typeOfficeArtFSP:
		{
			convert(dynamic_cast<ODRAW::OfficeArtFSP *>(art));
		}break;
	case XLS::typeOfficeArtFSPGR:
		{
			ODRAW::OfficeArtFSPGR * ch = dynamic_cast<ODRAW::OfficeArtFSPGR *>(art);

			xlsx_context->get_drawing_context().set_group_anchor(ch->_x, ch->_y, ch->_cx, ch->_cy);
		}break;
	case XLS::typeOfficeArtClientAnchorHF:
		{
			ODRAW::OfficeArtClientAnchorHF * hf = dynamic_cast<ODRAW::OfficeArtClientAnchorHF *>(art);
			xlsx_context->get_drawing_context().set_child_anchor(0, 0, hf->_cx * 3 / 5, hf->_cy * 3 / 5);
		}break;
	case XLS::typeOfficeArtChildAnchor:
		{
			ODRAW::OfficeArtChildAnchor * ch = dynamic_cast<ODRAW::OfficeArtChildAnchor *>(art);
			xlsx_context->get_drawing_context().set_child_anchor(ch->_x, ch->_y, ch->_cx, ch->_cy);
		}break;
	case XLS::typeOfficeArtClientAnchorSheet:
		{
			ODRAW::OfficeArtClientAnchorSheet * ch = dynamic_cast<ODRAW::OfficeArtClientAnchorSheet *>(art);
        
			ch->calculate();
			ch->calculate_1();
			
			if (xlsx_context->get_drawing_context().getType()	== oox::external_items::typeGroup &&
				xlsx_context->get_drawing_context().getLevel()	== 1)
			{
				xlsx_context->get_drawing_context().set_child_anchor(ch->_x, ch->_y, ch->_cx, ch->_cy);
			}
			xlsx_context->get_drawing_context().set_sheet_anchor(ch->colL, ch->_dxL, ch->rwT, ch->_dyT, ch->colR, ch->_dxR, ch->rwB, ch->_dyB,
				ch->_x, ch->_y, ch->_cx, ch->_cy);
		}break;
	case XLS::typeOfficeArtBStoreContainer:
		{
			convert (dynamic_cast<ODRAW::OfficeArtBStoreContainer *>(art), 3000);
		}break;
	case XLS::typeOfficeArtSpContainer:
		{
			convert (dynamic_cast<ODRAW::OfficeArtSpContainer *>(art));
		}break;
	case XLS::typeOfficeArtSpgrContainer:
		{
			convert (dynamic_cast<ODRAW::OfficeArtSpgrContainer *>(art));
		}break;
	case XLS::typeOfficeArtDgContainer:
		{
			ODRAW::OfficeArtDgContainer * dg = dynamic_cast<ODRAW::OfficeArtDgContainer *>(art);
        
			for (size_t i = 0 ; i < dg->child_records.size(); i++)	//a-la msodrawing for headers/footers
			{
				convert(dg->child_records[i].get());
			}

			for (size_t i = 0; i < dg->m_OfficeArtSpContainer.size(); i++)
			{
				convert(dg->m_OfficeArtSpContainer[i].get());
			}

			convert(dg->m_OfficeArtSpgrContainer.get());
		}break;
	default:
		break;
	}
}

void XlsConverter::convert(ODRAW::OfficeArtFSP * fsp)
{
	if (fsp == NULL) return;

	if (fsp->fFlipH) xlsx_context->get_drawing_context().set_FlipH();
	if (fsp->fFlipV) xlsx_context->get_drawing_context().set_FlipV();

	xlsx_context->get_drawing_context().set_id(fsp->spid);
	xlsx_context->get_drawing_context().set_shape_id(fsp->shape_id);
}
void XlsConverter::convert_fill_style(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.empty()) return;
	
	for (size_t i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
			case ODRAW::fillType:
			{
				switch(props[i]->op)
				{
				case 1://fillPattern:
					{
						xlsx_context->get_drawing_context().set_fill_type(oox::fillPattern);
						//texture + change black to color2, white to color1
					}break;
					case 2://fillTexture :
					{
						xlsx_context->get_drawing_context().set_fill_type(oox::fillTexture);
						xlsx_context->get_drawing_context().set_fill_texture_mode(0);
					}break;
					case 3://fillPicture :
					{
						xlsx_context->get_drawing_context().set_fill_type(oox::fillTexture);
						xlsx_context->get_drawing_context().set_fill_texture_mode(1);
					}break;
					case 4://fillShadeCenter://1 color
					case 5://fillShadeShape:
					{
						xlsx_context->get_drawing_context().set_fill_type(oox::fillGradientOne);
					}break;//
					case 6://fillShadeTitle://2 colors and more
					case 7://fillShade : 
					case 8://fillShadeScale: 
					{
						xlsx_context->get_drawing_context().set_fill_type(oox::fillGradient);
					}break;
					case 9://fillBackground:
					{
						xlsx_context->get_drawing_context().set_fill_type(oox::fillNone);
					}break;	
					case 0:
					default:
					{ //undefined
						xlsx_context->get_drawing_context().set_fill_type(oox::fillUndefined);
					}break;
				}
			}break;
			case ODRAW::fillColor:
			case ODRAW::fillColorExt:
			{
				ODRAW::FillColor * fill = (ODRAW::FillColor *)(props[i].get());
				ODRAW::OfficeArtCOLORREF color((_UINT32)fill->op);
				if (!color.sColorRGB.empty())
					xlsx_context->get_drawing_context().set_fill_color(color.nColorRGB, color.sColorRGB);
				else if (color. fPaletteIndex)
				{
					std::map<int,  std::wstring>::iterator it = xls_global_info->colors_palette.find(color.index);
					if (it != xls_global_info->colors_palette.end())
					{					
						//убрать 0!!! todooo
						xlsx_context->get_drawing_context().set_fill_color(0, it->second);
					}
				}
				else
					xlsx_context->get_drawing_context().set_fill_color(color.index, color.fSchemeIndex ? 1 : 3 );
			}break;
			case ODRAW::fillOpacity:
			{
				ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
				xlsx_context->get_drawing_context().set_fill_opacity(fixed_point->dVal);
			}break;
			case ODRAW::fillBackColor:
			case ODRAW::fillBackColorExt:
			{
				ODRAW::FillColor * fill = (ODRAW::FillColor *)(props[i].get());
				ODRAW::OfficeArtCOLORREF color((_UINT32)fill->op);
				if (!color.sColorRGB.empty())
					xlsx_context->get_drawing_context().set_fill_color(color.nColorRGB,color.sColorRGB, true );
				else if (color. fPaletteIndex)
				{
					std::map<int,  std::wstring>::iterator it = xls_global_info->colors_palette.find(color.index);
					if (it != xls_global_info->colors_palette.end())
					{					
						//todooo убрать 0 !!!
						xlsx_context->get_drawing_context().set_fill_color(0, it->second, true );
					}
				}
				else
					xlsx_context->get_drawing_context().set_fill_color(color.index, color.fSchemeIndex ? 1 : 3, true );
			}break;
			case ODRAW::fillBackOpacity:
			{
				ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
				xlsx_context->get_drawing_context().set_fill_opacity(fixed_point->dVal, true);
			}break;
			case ODRAW::fillBlip:
			{
				std::wstring target;
				ODRAW::FillBlip *fillBlip = (ODRAW::FillBlip *)(props[i].get());
				if ((fillBlip) && (fillBlip->blip))
				{
					target = WriteMediaFile(fillBlip->blip->pict_data,fillBlip->blip->pict_size, fillBlip->blip->pict_type);
				}
				else
				{
					bool isIternal = false;
					std::wstring rId = xlsx_context->get_mediaitems().find_image(props[i]->op , target, isIternal);
				}
				xlsx_context->get_drawing_context().set_fill_texture(target);
			}break;
			case ODRAW::fillAngle:
			{
				ODRAW::FillAngle * angle = (ODRAW::FillAngle *)(props[i].get());
				xlsx_context->get_drawing_context().set_fill_angle(angle->dVal);
			}break;
			case ODRAW::fillFocus:
			{
				xlsx_context->get_drawing_context().set_fill_focus(props[i]->op);
			}break;
			case ODRAW::fillShadeType:
			{
				ODRAW::FillShadeType *shadeType = dynamic_cast<ODRAW::FillShadeType*>(props[i].get());
			}break;
			case ODRAW::fillShadePreset:
			{
			}break;
			case ODRAW::fillShadeColors:
			{
				ODRAW::FillShadeColors *shadeColors = dynamic_cast<ODRAW::FillShadeColors*>(props[i].get());

				for (size_t i = 0 ; (shadeColors) && (i < shadeColors->complex.data.size()); i++)
				{
					ODRAW::OfficeArtCOLORREF & color = shadeColors->complex.data[i].color;

					std::wstring strColor;
					if (!color.sColorRGB.empty())	strColor = color.sColorRGB;
					else if (color. fPaletteIndex)
					{
						std::map<int,  std::wstring>::iterator it = xls_global_info->colors_palette.find(color.index);
						if (it != xls_global_info->colors_palette.end())	strColor = it->second;
					}
					if (!strColor.empty())
						xlsx_context->get_drawing_context().add_fill_colors( shadeColors->complex.data[i].dPosition, strColor);
					else
					{
						xlsx_context->get_drawing_context().add_fill_colors( shadeColors->complex.data[i].dPosition, 
																									color.index, color.fSchemeIndex ? 1 : 3 );
					}
				}
			}break;
			case ODRAW::fillStyleBooleanProperties:
			{
				ODRAW::FillStyleBooleanProperties * bools = (ODRAW::FillStyleBooleanProperties *)(props[i].get());
				if (bools)
				{
					if (bools->fUsefFilled && bools->fFilled == false) 
						xlsx_context->get_drawing_context().set_fill_type(oox::fillNone);
				}
			}break;
			default:
			{
				i =i;
			}break;
		}
	}
}
void XlsConverter::convert_line_style(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.empty()) return;

	for (size_t i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
			case ODRAW::lineColor:
			{
				ODRAW::OfficeArtCOLORREF color((_UINT32)props[i]->op);
				if (!color.sColorRGB.empty())
					xlsx_context->get_drawing_context().set_line_color(color.nColorRGB, color.sColorRGB);
				else if (color. fPaletteIndex)
				{
					std::map<int,  std::wstring>::iterator it = xls_global_info->colors_palette.find(color.index);
					if (it != xls_global_info->colors_palette.end())
					{					
						//todooo убрать 0 !!
						xlsx_context->get_drawing_context().set_line_color(0, it->second);
					}
				}
				else
					xlsx_context->get_drawing_context().set_line_color(color.index, color.fSchemeIndex ? 1 : 3 );
			}break;
			case ODRAW::lineType:
			{
				xlsx_context->get_drawing_context().set_line_type(props[i]->op);
			}break;
			case ODRAW::lineFillBlip: //blip 
			{
			}break;
			case ODRAW::lineWidth: 
				xlsx_context->get_drawing_context().set_line_width(props[i]->op);
			{
			}break;
			case ODRAW::lineStyle:
			{
				xlsx_context->get_drawing_context().set_line_style(props[i]->op);
			}break;
			case ODRAW::lineDashing:
			{
				xlsx_context->get_drawing_context().set_line_dash(props[i]->op);
			}break;
        	case ODRAW::lineStartArrowhead:
			{
				xlsx_context->get_drawing_context().set_arrow_start(props[i]->op);
			}break;
        	case ODRAW::lineEndArrowhead:
			{
				xlsx_context->get_drawing_context().set_arrow_end(props[i]->op);
			}break;
        	case ODRAW::lineStartArrowWidth:
			{
				xlsx_context->get_drawing_context().set_arrow_start_width(props[i]->op);
			}break;
        	case ODRAW::lineStartArrowLength:
			{
				xlsx_context->get_drawing_context().set_arrow_start_length(props[i]->op);
			}break;
        	case ODRAW::lineEndArrowWidth:
			{
				xlsx_context->get_drawing_context().set_arrow_end_width(props[i]->op);
			}break;
        	case ODRAW::lineEndArrowLength:
			{
				xlsx_context->get_drawing_context().set_arrow_end_length(props[i]->op);
			}break;
			case ODRAW::lineStyleBooleanProperties:
			{
				ODRAW::LineStyleBooleanProperties * bools = (ODRAW::LineStyleBooleanProperties *)(props[i].get());
				if (bools)
				{
					if (bools->fUsefLine && bools->fLine == false) 
						xlsx_context->get_drawing_context().set_line_type(5); //no line

					if (bools->fUsefArrowheadsOK && bools->fArrowheadsOK)
					{
						xlsx_context->get_drawing_context().set_line_arrow(true);
					}
				}
			}break;
			case ODRAW::lineMiterLimit:
			{
				xlsx_context->get_drawing_context().set_line_miter(props[i]->op);
			}break;
			case ODRAW::lineJoinStyle:
			{
				xlsx_context->get_drawing_context().set_line_join(props[i]->op);
			}break;
			case ODRAW::lineEndCapStyle:
			{
				xlsx_context->get_drawing_context().set_line_endcap(props[i]->op);
			}break;
		}
	}
}
void XlsConverter::convert_blip(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.empty()) return;

	for (size_t i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
			case ODRAW::cropFromTop:
				{
					ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
					xlsx_context->get_drawing_context().set_picture_crop_top(fixed_point->dVal * 10);
				}break;
			case ODRAW::cropFromBottom:
				{
					ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
					xlsx_context->get_drawing_context().set_picture_crop_bottom(fixed_point->dVal * 10);
				}break;
			case ODRAW::cropFromLeft:
				{
					ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
					xlsx_context->get_drawing_context().set_picture_crop_left(fixed_point->dVal * 10);
				}break;
			case ODRAW::cropFromRight:
				{
					ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
					xlsx_context->get_drawing_context().set_picture_crop_right(fixed_point->dVal * 10);
				}break;
			case ODRAW::pib:
				{
					bool isIternal = false;
					std::wstring target;

					int id = props[i]->op;
					if (xlsx_context->get_drawing_context().get_mode_HF()) 
						id += 3000;
					
					std::wstring rId = xlsx_context->get_mediaitems().find_image(id , target, isIternal);
					xlsx_context->get_drawing_context().set_picture(target);
				}break;
			case ODRAW::pibName:
			{
				ODRAW::AnyString *str = dynamic_cast<ODRAW::AnyString*>(props[i].get());
				xlsx_context->get_drawing_context().set_picture_name(str->string_);
			}break;
			case ODRAW::pibFlags:
			{
			}break;
			case ODRAW::pictureTransparent:
			{
				ODRAW::OfficeArtCOLORREF color((_UINT32)props[i]->op);
				
				if (!color.sColorRGB.empty())
					xlsx_context->get_drawing_context().set_picture_transparent(color.nColorRGB, color.sColorRGB);
				//...
			}break;
			case ODRAW::pictureContrast:
			{
				int val = 0;
				if (props[i]->op == 0x7FFFFFFF)
				{
					val = 100;
				}
				else if (props[i]->op == 0x00010000)
				{
					val = 0;
				}
				else if (props[i]->op > 0x00010000)
				{
					val = 51. / (props[i]->op / 65536.);
					val = (50 - val) * 2;
				}
				else
				{
					val = (100. * props[i]->op / 65536. + 0.5) - 100;
				}
				xlsx_context->get_drawing_context().set_picture_contrast(val);
			}break;
			case ODRAW::pictureBrightness:
			{
				int val = ((_INT32)props[i]->op * 100. + 0.5) / 0x00008000;
				xlsx_context->get_drawing_context().set_picture_brightness(val);
			}break;
			case ODRAW::pictureGamma:
			{
			}break;
			case ODRAW::pictureId:
			{
			}break;
			case ODRAW::pictureDblCrMod:
			{
			}break;
			case ODRAW::pictureFillCrMod:
			{
			}break;
			case ODRAW::pictureLineCrMod:
			{
			}break;
			case ODRAW::pibPrint:
			{
			}break;
			case ODRAW::pibPrintName:
			{
			}break;
			case ODRAW::pibPrintFlags:
			{
			}break;
			case ODRAW::blipBooleanProperties:
			{				
				ODRAW::BlipBooleanProperties * bools = (ODRAW::BlipBooleanProperties *)(props[i].get());
				if (bools)
				{
					if (bools->fUsefPictureGray && bools->fPictureGray) 
						xlsx_context->get_drawing_context().set_picture_grayscale(true);
					if (bools->fUsefPictureBiLevel && bools->fPictureBiLevel) 
						xlsx_context->get_drawing_context().set_picture_biLevel(50);
				}

			}break;
		}
	}
}
void XlsConverter::convert_geometry(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.empty()) return;

	_CP_OPT(oox::_rect)			rect;
	std::vector<_CP_OPT(int)>	adjustValues(9);
	
	for (size_t i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
		case ODRAW::geoLeft:		if (!rect) rect = oox::_rect(); rect->x = props[i]->op; break;
			case ODRAW::geoTop:		if (!rect) rect = oox::_rect(); rect->y	= props[i]->op; break;
			case ODRAW::geoRight:	if (!rect) rect = oox::_rect(); rect->cx = props[i]->op; break;
			case ODRAW::geoBottom:	if (!rect) rect = oox::_rect(); rect->cy = props[i]->op; break;
			case ODRAW::shapePath:
				xlsx_context->get_drawing_context().set_custom_path(props[i]->op); break;
			case ODRAW::pVertices:
				{
					ODRAW::PVertices * v = (ODRAW::PVertices *)(props[i].get());
					xlsx_context->get_drawing_context().set_custom_verticles(v->complex.data);
				}break;
			case ODRAW::pSegmentInfo:
				{
					ODRAW::PSegmentInfo * s = (ODRAW::PSegmentInfo *)(props[i].get());
					xlsx_context->get_drawing_context().set_custom_segments(s->complex.data);
				}break;
			case ODRAW::adjustValue: //0x0147
			case ODRAW::adjust2Value:
			case ODRAW::adjust3Value:
			case ODRAW::adjust4Value:
			case ODRAW::adjust5Value:
			case ODRAW::adjust6Value:
			case ODRAW::adjust7Value:
			case ODRAW::adjust8Value:
			case ODRAW::adjust9Value:
			case ODRAW::adjust10Value:
			{
				adjustValues[props[i]->opid - 0x0147] = props[i]->op ;
			}break;
			case ODRAW::pConnectionSites:
			{
				ODRAW::PConnectionSites * a = (ODRAW::PConnectionSites *)(props[i].get());
				xlsx_context->get_drawing_context().set_custom_connection(a->complex.data);
			}break;
			case ODRAW::pConnectionSitesDir:
			{
				ODRAW::PConnectionSitesDir * a = (ODRAW::PConnectionSitesDir *)(props[i].get());
				xlsx_context->get_drawing_context().set_custom_connectionDir(a->complex.data);
			}break;
			case ODRAW::xLimo:
			{
				xlsx_context->get_drawing_context().set_custom_x_limo(props[i]->op);
			}break;
			case ODRAW::yLimo:
			{
				xlsx_context->get_drawing_context().set_custom_y_limo(props[i]->op);
			}break;
			case ODRAW::pAdjustHandles:
			{
				ODRAW::PAdjustHandles * a = (ODRAW::PAdjustHandles *)(props[i].get());
				xlsx_context->get_drawing_context().set_custom_adjustHandles(a->complex.data);
			}break;
			case ODRAW::pGuides:
			{
				ODRAW::PGuides* s = (ODRAW::PGuides *)(props[i].get());
				xlsx_context->get_drawing_context().set_custom_guides(s->complex.data);
			}break;
			case ODRAW::pInscribe:
			{
				ODRAW::PInscribe * a = (ODRAW::PInscribe *)(props[i].get());
				xlsx_context->get_drawing_context().set_custom_inscribe(a->complex.data);
			}break;
			//case ODRAW::cxk:
			//	{
			//		ODRAW::cxk * a = (ODRAW::cxk *)(props[i].get());
			//		xlsx_context->get_drawing_context().set_custom_cxk(a->complex.data);
			//	}break;
		}
	}
	if (rect)
	{
		rect->cy -= rect->y;
		rect->cx -= rect->x;

		xlsx_context->get_drawing_context().set_custom_rect(*rect);
	}
	
	xlsx_context->get_drawing_context().set_custom_adjustValues(adjustValues);
}
void XlsConverter::convert_geometry_text(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.empty()) return;

	for (size_t i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
		case ODRAW::gtextUNICODE://word art text
			{
				ODRAW::AnyString *str = dynamic_cast<ODRAW::AnyString*>(props[i].get());
				if (str) xlsx_context->get_drawing_context().set_wordart_text(str->string_);
			}break;
		case ODRAW::gtextFont:
			{
				ODRAW::AnyString *str = dynamic_cast<ODRAW::AnyString*>(props[i].get());
				if (str)
				{
					xlsx_context->get_drawing_context().set_wordart_font(str->string_);
				}
			}break;
		case ODRAW::gtextSize:
			{
				xlsx_context->get_drawing_context().set_wordart_size((INT)((props[i]->op >> 16) & 0x0000FFFF));
			}break;
		case ODRAW::gtextSpacing:
			{
				ODRAW::FixedPoint *val = dynamic_cast<ODRAW::FixedPoint*>(props[i].get());
				if (val) xlsx_context->get_drawing_context().set_wordart_spacing(val->dVal);
			}break;
		case ODRAW::gtextAlign:
			{
				switch (props[i]->op)
				{
				case ODRAW::alignTextLeft:
					xlsx_context->get_drawing_context().set_text_align(1);	break;
				case ODRAW::alignTextCenter:
					xlsx_context->get_drawing_context().set_text_align(2);	break;
				case ODRAW::alignTextRight:
					xlsx_context->get_drawing_context().set_text_align(3);	break;
				case ODRAW::alignTextWordJust:
					xlsx_context->get_drawing_context().set_text_align(4);	break;
				case ODRAW::alignTextLetterJust:
					xlsx_context->get_drawing_context().set_text_align(5);	break;
				}				
			}break;
		case 0xff:
			{				
				ODRAW::GeometryTextBooleanProperties *bools = dynamic_cast<ODRAW::GeometryTextBooleanProperties*>(props[i].get());
				if (bools) 
				{
					if (bools->fUsegFBold			&& bools->fBold)		xlsx_context->get_drawing_context().set_wordart_bold	(true); 
					if (bools->fUsegFItalic			&& bools->fItalic)		xlsx_context->get_drawing_context().set_wordart_italic	(true); 
					if (bools->fUsegFVertical		&& bools->fVertical)	xlsx_context->get_drawing_context().set_wordart_vertical(true); 
					if (bools->fUsegFUnderline		&& bools->fUnderline)	xlsx_context->get_drawing_context().set_wordart_underline(true); 
					if (bools->fUsegFStrikethrough	&& bools->fStrikethrough)xlsx_context->get_drawing_context().set_wordart_strike(true);

					//if (bools->fUsegFShrinkFit && bools->fShrinkFit)
				}
			}break;
		}
	}
}
void XlsConverter::convert_text(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.empty()) return;

	RECT text_margin = {0x00016530, 0x0000b298, 0x00016530, 0x0000b298};
	for (size_t i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
		case ODRAW::lTxid: break;
		case ODRAW::txdir: break;
		case ODRAW::dxTextLeft:		text_margin.left	= props[i]->op;	break;
		case ODRAW::dxTextRight:	text_margin.right	= props[i]->op;	break;			
		case ODRAW::dyTextTop:		text_margin.top		= props[i]->op;	break;
		case ODRAW::dyTextBottom:	text_margin.bottom	= props[i]->op;	break;
		case ODRAW::WrapText:
			{
				xlsx_context->get_drawing_context().set_text_wrap(props[i]->op);				
			}break;
		case ODRAW::txflTextFlow:
			{
				xlsx_context->get_drawing_context().set_text_vertical(props[i]->op);
			}break;
		case ODRAW::textBooleanProperties:
			{
				ODRAW::TextBooleanProperties *bools = dynamic_cast<ODRAW::TextBooleanProperties*>(props[i].get());
				if (bools)
				{
					if (bools->fUsefFitShapeToText)
					{
						xlsx_context->get_drawing_context().set_text_fit_shape(bools->fFitShapeToText);
					}
					if (bools->fUsefAutoTextMargin)
					{
					}
				}
			}break;
		case ODRAW::anchorText:
			{
				switch (props[i]->op)
				{
				case ODRAW::anchorTop:
				case ODRAW::anchorTopBaseline:
					{	
						xlsx_context->get_drawing_context().set_text_align		(1);
						xlsx_context->get_drawing_context().set_text_vert_align	(1);
					}break;
				case ODRAW::anchorMiddle:
					{
						xlsx_context->get_drawing_context().set_text_align		(1);
						xlsx_context->get_drawing_context().set_text_vert_align	(2);
					}break;						
				case ODRAW::anchorBottom:
				case ODRAW::anchorBottomBaseline:
					{
						xlsx_context->get_drawing_context().set_text_align		(1);
						xlsx_context->get_drawing_context().set_text_vert_align	(3);
					}break;
				case ODRAW::anchorTopCentered:
				case ODRAW::anchorTopCenteredBaseline:
					{
						xlsx_context->get_drawing_context().set_text_align		(2);
						xlsx_context->get_drawing_context().set_text_vert_align	(1);
					}break;
				case ODRAW::anchorMiddleCentered:
					{
						xlsx_context->get_drawing_context().set_text_align		(2);
						xlsx_context->get_drawing_context().set_text_vert_align	(2);
					}break;
				case ODRAW::anchorBottomCentered:
				case ODRAW::anchorBottomCenteredBaseline:
					{
						xlsx_context->get_drawing_context().set_text_align		(2);
						xlsx_context->get_drawing_context().set_text_vert_align	(3);
					}break;
				}
			}break;
		default:
			int j = 0;
			break;		
		}
	}
	xlsx_context->get_drawing_context().set_text_margin(text_margin);
}
void XlsConverter::convert_shadow(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.empty()) return;

	xlsx_context->get_drawing_context().set_shadow_enabled(true);

	for (size_t i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
			case ODRAW::shadowType:
			{
				xlsx_context->get_drawing_context().set_shadow_type(props[i]->op);
			}break;
			case ODRAW::shadowColor:
			{
				ODRAW::OfficeArtCOLORREF color((_UINT32)props[i]->op);
				
				if (!color.sColorRGB.empty())
					xlsx_context->get_drawing_context().set_shadow_color(color.nColorRGB, color.sColorRGB);
			}break;
			case ODRAW::shadowHighlight:
			{
				ODRAW::OfficeArtCOLORREF color((_UINT32)props[i]->op);
				
				if (!color.sColorRGB.empty())
					xlsx_context->get_drawing_context().set_shadow_highlight(color.nColorRGB, color.sColorRGB);
			}break;
			case ODRAW::shadowCrMod:
			{
			}break;
			case ODRAW::shadowOpacity:
			{
				ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
				xlsx_context->get_drawing_context().set_shadow_opacity(fixed_point->dVal);
			}break;
			case ODRAW::shadowOffsetX:
			{
				xlsx_context->get_drawing_context().set_shadow_offsetX((_INT32)props[i]->op);
			}break;
			case ODRAW::shadowOffsetY:
			{
				xlsx_context->get_drawing_context().set_shadow_offsetY((_INT32)props[i]->op);
			}break;
			//case ODRAW::shadowSecondOffsetX:
			//{
			//}break;
			//case ODRAW::shadowSecondOffsetY:
			//{
			//}break;
			case ODRAW::shadowScaleXToX:
			{
				ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
				xlsx_context->get_drawing_context().set_shadow_scaleX2X(fixed_point->dVal);
			}break;
			case ODRAW::shadowScaleYToX:
			{
			}break;
			case ODRAW::shadowScaleXToY:
			{
			}break;
			case ODRAW::shadowScaleYToY:
			{
				ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
				xlsx_context->get_drawing_context().set_shadow_scaleY2Y(fixed_point->dVal);
			}break;
			//case ODRAW::shadowPerspectiveX:
			//{
			//}break;
			//case ODRAW::shadowPerspectiveY:
			//{
			//}break;
			//case ODRAW::shadowWeight:
			//{
			//}break;
			case ODRAW::shadowOriginX:
			{
				ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
				xlsx_context->get_drawing_context().set_shadow_originX(fixed_point->dVal);
			}break;
			case ODRAW::shadowOriginY:
			{
				ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
				xlsx_context->get_drawing_context().set_shadow_originY(fixed_point->dVal);
			}break;
		}
	}
}
void XlsConverter::convert_shape(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.empty()) return;

	for (size_t i = 0 ; i < props.size() ; i++)
	{

	}
}
void XlsConverter::convert_group_shape(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.empty()) return;

	for (size_t i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
		case 0x380:
			{
				ODRAW::AnyString *str = dynamic_cast<ODRAW::AnyString*>(props[i].get());
				xlsx_context->get_drawing_context().set_name(str->string_);
			}break;
		case 0x381:
			{
				ODRAW::AnyString *str = dynamic_cast<ODRAW::AnyString*>(props[i].get());
				xlsx_context->get_drawing_context().set_description(str->string_);
			}break;
		case 0x0382:
			{
				ODRAW::PihlShape *pihlShape = dynamic_cast<ODRAW::PihlShape*>(props[i].get());
				if (pihlShape)
				{
					std::wstring	sTarget;					
					bool			bExternal = false;					
					
					if (pihlShape->complex.hyperlink.hlstmfHasMoniker)
					{
						sTarget = GetTargetMoniker(pihlShape->complex.hyperlink.oleMoniker.data.get());
						bExternal = true;
					}
					else if (pihlShape->complex.hyperlink.hlstmfHasLocationStr)
					{
						sTarget = pihlShape->complex.hyperlink.location;
					}

					std::wstring sDisplay = pihlShape->complex.hyperlink.displayName;
					if (sDisplay.empty())	sDisplay = sTarget;

					xlsx_context->get_drawing_context().set_hyperlink( sTarget, sDisplay, bExternal);				
				}
			}break;
		case 0x03A9:
			{
				ODRAW::MetroBlob *alternative_data = dynamic_cast<ODRAW::MetroBlob*>(props[i].get());
				if (alternative_data)
				{
					xlsx_context->get_drawing_context().set_alternative_drawing( alternative_data->xmlString);				
				}
			}break;
		case 0x03BF:
			{
				ODRAW::GroupShapeBooleanProperties *bools = dynamic_cast<ODRAW::GroupShapeBooleanProperties*>(props[i].get());
				if (bools)
				{
					if (bools->fHidden && bools->fUsefHidden)	
						xlsx_context->get_drawing_context().set_hidden(true);
				}
			}break;
		}
	}
}

void XlsConverter::convert(XLS::Note* note)
{
	if (note == NULL) return;

	note->note_sh.calculate();

	xlsx_context->get_comments_context().set_ref	(note->note_sh.ref_, note->note_sh.col, note->note_sh.row);
	xlsx_context->get_comments_context().set_author	(note->note_sh.stAuthor);
	xlsx_context->get_comments_context().set_visibly(note->note_sh.fShow);
	
	xlsx_context->get_drawing_context().set_object_id		(xlsx_context->get_comments_context().get_id());
	xlsx_context->get_drawing_context().set_object_visible	(note->note_sh.fShow);
	xlsx_context->get_drawing_context().set_object_anchor	(note->note_sh.col, note->note_sh.row);

	if (xls_global_info->Version < 0x0600)
	{
		//todooo размеры произвольные .. можно сделать оценку по размеру строки
		xlsx_context->get_drawing_context().set_sheet_anchor(0, 0, 0, 0, 0, 0, 0, 0, note->note_sh.x_, note->note_sh.y_, 120 * 12700., 64 * 12700.);
		xlsx_context->get_drawing_context().set_text(std::wstring(L"<t>") + note->note_sh.stText.value() + std::wstring(L"</t>"));
	}
}

void XlsConverter::convert_transform(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	for (size_t i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
			case 0x0004:
			{
				ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
				xlsx_context->get_drawing_context().set_rotation(fixed_point->dVal);
			}break;
		}
	}
}
void XlsConverter::convert(ODRAW::OfficeArtTertiaryFOPT	* fort)
{
	if (fort == NULL) return;

	convert_shape			(fort->fopt.Shape_props);
	convert_group_shape		(fort->fopt.GroupShape_props);
	convert_transform		(fort->fopt.Transform_props);
	convert_blip			(fort->fopt.Blip_props);
	convert_geometry		(fort->fopt.Geometry_props);
	convert_fill_style		(fort->fopt.FillStyle_props);
	convert_line_style		(fort->fopt.LineStyle_props);
	convert_shadow			(fort->fopt.Shadow_props);
	convert_text			(fort->fopt.Text_props);
	convert_geometry_text	(fort->fopt.GeometryText_props);
}
void XlsConverter::convert(ODRAW::OfficeArtFOPT * fort)
{
	if (fort == NULL) return;

	convert_shape			(fort->fopt.Shape_props);
	convert_group_shape		(fort->fopt.GroupShape_props);
	convert_transform		(fort->fopt.Transform_props);
	convert_geometry		(fort->fopt.Geometry_props);
	convert_fill_style		(fort->fopt.FillStyle_props);
	convert_blip			(fort->fopt.Blip_props);
	convert_line_style		(fort->fopt.LineStyle_props);
	convert_shadow			(fort->fopt.Shadow_props);
	convert_text			(fort->fopt.Text_props);
	convert_geometry_text	(fort->fopt.GeometryText_props);
}

void XlsConverter::convert(XLS::SHAREDSTRINGS* sharedstrings)
{
	int count = xls_global_info->startAddedSharedStrings + xls_global_info->arAddedSharedStrings.size();

	CP_XML_WRITER(xlsx_context->shared_strings())    
	{
		CP_XML_NODE(L"sst")
		{
			CP_XML_ATTR(L"uniqueCount", count);
			CP_XML_ATTR(L"xmlns", "http://schemas.openxmlformats.org/spreadsheetml/2006/main");
			if (sharedstrings)
			{	
				for (std::list<XLS::BaseObjectPtr>::iterator it = sharedstrings->elements_.begin(); it != sharedstrings->elements_.end(); ++it)
				{
					(*it)->serialize(CP_XML_STREAM());
				}
			}

			for (size_t i = 0 ; i < xls_global_info->arAddedSharedStrings.size(); i++)
			{
				CP_XML_NODE(L"si")
				{	
					CP_XML_NODE(L"t")
					{		
						CP_XML_ATTR(L"xml:space", "preserve");
						CP_XML_STREAM() << STR::escape_ST_Xstring(XmlUtils::EncodeXmlString(xls_global_info->arAddedSharedStrings[i]));
					}
				}
			}
		}
	}
}
void XlsConverter::convert(XLS::TxO * text_obj)
{
	if (text_obj == NULL) return;
	
	int rot = text_obj->rot;

	if (rot > 0)
	{
		switch(rot)
		{
		case 1: //text appears top to bottom; letters are upright
		{
			xlsx_context->get_drawing_context().set_text_vertical(5);		
		}break;
		case 2: //text is rotated 90 degrees counterclockwise
		{
			if		(text_obj->hAlignment == (unsigned char)1) text_obj->hAlignment = 3;
			else if (text_obj->hAlignment == (unsigned char)3) text_obj->hAlignment = 1;

			xlsx_context->get_drawing_context().set_text_vertical(2);	
		}break;
		case 3: //text is rotated 90 degrees clockwise
		{
			if		(text_obj->vAlignment == (unsigned char)1) text_obj->vAlignment = 3;
			else if (text_obj->vAlignment == (unsigned char)3) text_obj->vAlignment = 1;
		
			xlsx_context->get_drawing_context().set_text_vertical(3);
		}break;
		}

	}

	xlsx_context->get_drawing_context().set_text_align		(text_obj->hAlignment);
	xlsx_context->get_drawing_context().set_text_vert_align	(text_obj->vAlignment);
	
	std::wstringstream strm, strm_vml;

	text_obj->serialize(strm);	
	xlsx_context->get_drawing_context().set_text(strm.str());

	text_obj->serialize_vml(strm_vml);	
	xlsx_context->get_drawing_context().set_text_vml(strm_vml.str());

}

void XlsConverter::convert(XLS::Obj * obj)
{
	if ( obj == NULL ) return;

//controls & objects
	if ( obj->cmo.ot == 8 && obj->pictFmla.fExist && obj->pictFlags.fExist)
	{
		std::wstring info;
		if (obj->pictFmla.fmla.bFmlaExist)
		{
			if (obj->pictFmla.fmla.bInfoExist)
				info = obj->pictFmla.fmla.embedInfo.strClass.value();
		}
		if (obj->pictFlags.fCtl && obj->pictFlags.fPrstm && xls_global_info->controls_data.first)//Controls Storage
		{
		//binary data
			xlsx_context->get_mediaitems().create_activeX_path(xlsx_path);
			
			std::wstring target_bin;
			std::wstring objectId_bin = xlsx_context->get_mediaitems().add_control_activeX(target_bin);

			NSFile::CFileBinary file;
			if ( file.CreateFileW(xlsx_context->get_mediaitems().activeX_path() + target_bin) )
			{		
				file.WriteFile(xls_global_info->controls_data.first.get() + obj->pictFmla.lPosInCtlStm, obj->pictFmla.cbBufInCtlStm);
				file.CloseFile();
			}
			std::wstring objectId_xml = xlsx_context->start_activeX();	
			xlsx_context->get_drawing_context().set_control_activeX(objectId_xml);
	
			xlsx_context->current_activeX().setDataBinRid(objectId_bin, target_bin);
			xlsx_context->current_activeX().setProgId(info);
			xlsx_context->current_activeX().setLicense(obj->pictFmla.key.keyBuf);

		}
		else if (!obj->pictFlags.fPrstm) 
		{
			std::wstring object_stream;
			if (obj->pictFlags.fDde)	object_stream = L"LNK";
			else						object_stream = L"MBD";
		
			object_stream += XmlUtils::ToString(obj->pictFmla.lPosInCtlStm, L"%08X") + L"/";
			if (xls_file->storage_->isDirectory(object_stream))
			{
				xlsx_context->get_mediaitems().create_embeddings_path(xlsx_path);
				
				std::wstring target;

				if (xls_file->storage_->exists(object_stream + L"Workbook")) target = L".xls";
				if (xls_file->storage_->exists(object_stream + L"WordDocument")) target = L".doc";

				std::wstring objectId = xlsx_context->get_mediaitems().add_embedding(target, info);

				POLE::Storage *storageOle = new POLE::Storage((xlsx_context->get_mediaitems().embeddings_path() + target).c_str());

				if ((storageOle) && (storageOle->open(true, true)))
				{			
					xls_file->copy(0, object_stream, storageOle, false, true);

					storageOle->close();
					delete storageOle;
				}
				xlsx_context->current_sheet().sheet_rels().add(oox::relationship(
					objectId, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject", L"../embeddings/" + target));

				xlsx_context->get_drawing_context().set_ole_object(objectId, info);
			}
		}
		if (obj->pictFmla.key.fmlaLinkedCell.bFmlaExist)
		{
			std::wstring link = obj->pictFmla.key.fmlaLinkedCell.fmla.getAssembledFormula(true);
			xlsx_context->get_drawing_context().set_object_link(link);
		}
		if (obj->pictFmla.key.fmlaListFillRange.bFmlaExist)
		{
			std::wstring link = obj->pictFmla.key.fmlaListFillRange.fmla.getAssembledFormula(true);
			xlsx_context->get_drawing_context().set_object_fmlaRange(link);
		}
	}
	if (obj->sbs.fExist)
	{
		xlsx_context->get_drawing_context().set_object_x_val(obj->sbs.iVal);
		xlsx_context->get_drawing_context().set_object_x_min(obj->sbs.iMin);
		xlsx_context->get_drawing_context().set_object_x_max(obj->sbs.iMax);
		xlsx_context->get_drawing_context().set_object_x_inc(obj->sbs.dInc);
		xlsx_context->get_drawing_context().set_object_x_page(obj->sbs.dPage);
	}
	if (obj->list.fExist)
	{
		xlsx_context->get_drawing_context().set_object_x_sel(obj->list.iSel);
		xlsx_context->get_drawing_context().set_object_x_sel_type(obj->list.wListSelType);
		xlsx_context->get_drawing_context().set_object_lct(obj->list.lct);

		if (obj->list.fmla.bFmlaExist)
		{
			std::wstring link = obj->list.fmla.fmla.getAssembledFormula(true);
			xlsx_context->get_drawing_context().set_object_fmlaRange(link);
		}
		if (obj->list.dropData)
		{
			xlsx_context->get_drawing_context().set_object_drop_style(obj->list.dropData->wStyle);
			xlsx_context->get_drawing_context().set_object_drop_lines(obj->list.dropData->cLine);
		}
	}
	if (obj->checkBox.fExist)
	{
	//unsigned short fChecked;
	//unsigned short accel;
	//bool fNo3d;
	}
	if (obj->radioButton.fExist)
	{
	//unsigned short idRadNext;
	//Boolean<unsigned short> fFirstBtn;
	}
	if (obj->edit.fExist)
	{
	//unsigned short ivtEdit;
	//Boolean<unsigned short> fMultiLine;
	//unsigned short fVScroll;
	//unsigned short id;
	}
	bool full_ref = false;
	if (obj->cmo.ot > 0x06) full_ref = true;
	
	if (obj->linkFmla.fExist && obj->linkFmla.fmla.bFmlaExist)
	{		
		std::wstring link = obj->linkFmla.fmla.fmla.getAssembledFormula(full_ref);
		xlsx_context->get_drawing_context().set_object_link(link);
	}
	if (obj->macro.fExist && obj->macro.fmla.bFmlaExist)
	{
		std::wstring macro = obj->macro.fmla.fmla.getAssembledFormula(full_ref);
		xlsx_context->get_drawing_context().set_macro(macro);
	}
}

void XlsConverter::convert(XLS::ChartSheetSubstream* chart)
{
	if (chart == NULL) return;

	chart->serialize(xlsx_context->current_chart().chartData());	
	//convert(chart->m_OBJECTSCHART.get());непонятные какие то текстбоксы - пустые и бз привязок
}

void XlsConverter::convert(XLS::PIVOTVIEW* pivot_view)
{
	if (pivot_view == NULL) return;

	std::wstringstream strm;

	pivot_view->serialize(strm);

	int index_view = xlsx_context->get_pivots_context().add_view(strm.str(), pivot_view->indexCache);
	
	if (index_view > 0)
	{
		xlsx_context->current_sheet().sheet_rels().add(oox::relationship(L"pvId" + std::to_wstring(index_view),
			L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotTable",
			L"../pivotTables/pivotTable" + std::to_wstring(index_view) + L".xml"));
	}
}

void XlsConverter::convert(XLS::PIVOTCACHEDEFINITION* pivot_cached)
{
	if (pivot_cached == NULL) return;

	std::wstringstream strmD, strmR;

	pivot_cached->serialize_definitions(strmD);
	pivot_cached->serialize_records(strmR);

	xlsx_context->get_pivots_context().add_cache(strmD.str(), strmR.str());
	
	if (!xls_global_info->mapPivotCacheExternal.empty())
	{
		xlsx_context->get_pivots_context().add_cache_external(xls_global_info->mapPivotCacheExternal);
	}
}

void XlsConverter::convert(XLS::SUPBOOK* external)
{
	if (external == NULL) return;
	if (external->IsExternal() == false) return;

	xlsx_context->start_external();

	external->serialize(xlsx_context->current_external().externalData());

	if (!external->sExternPathLink.empty()) //??? 
	{
		xlsx_context->current_external().add_rels(false, L"rId1", external->sExternPathLink, oox::external_items::typeExternalLink);
	}

	xlsx_context->end_external();
}
void XlsConverter::convert(XLS::QUERYTABLE* query_table)
{
	if (query_table == NULL) return;

	std::wstringstream strm;		
	query_table->serialize(strm);

	xlsx_context->add_query_table(strm.str());
}
