#include "XlsConverter.h"

#include "../XlsFormat/Binary/CompoundFile.h"
#include "../XlsFormat/Binary/CFStreamCacheReader.h"

#include "../XlsFormat/Logic/GlobalWorkbookInfo.h"
#include "../XlsFormat/Logic/WorkbookStreamObject.h"
#include "../XlsFormat/Logic/WorksheetSubstream.h"
#include "../XlsFormat/Logic/GlobalsSubstream.h"
#include "../XlsFormat/Logic/ChartSheetSubstream.h"

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
#include "../XlsFormat/Logic/Biff_unions/IMDATAOBJECT.h"
#include "../XlsFormat/Logic/Biff_unions/TEXTOBJECT.h"
#include "../XlsFormat/Logic/Biff_unions/CHART.h"
#include "../XlsFormat/Logic/Biff_unions/BACKGROUND.h"

#include <Logic/Biff_records/BkHim.h>
#include <Logic/Biff_records/HLink.h>
#include <Logic/Biff_records/MsoDrawingGroup.h>
#include <Logic/Biff_records/MsoDrawing.h>
#include <Logic/Biff_records/Obj.h>
#include <Logic/Biff_records/TxO.h>
#include <Logic/Biff_records/IMDATA.h>
#include <Logic/Biff_records/Note.h>

#include <Logic/Biff_structures/URLMoniker.h>
#include <Logic/Biff_structures/FileMoniker.h>

#include <Logic/Biff_structures/ODRAW/OfficeArtBStoreContainer.h>
#include <Logic/Biff_structures/ODRAW/SimpleOfficeArtContainers.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtFOPT.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtFOPTE.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtFSP.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtBlip.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtFSPGR.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtClientAnchorSheet.h>

#include "xlsx_conversion_context.h"
#include "xlsx_package.h"

#include <simple_xml_writer.h>
#include <utils.h>

#include <boost/lexical_cast.hpp>
#include <boost/utility.hpp>

#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/raster/BgraFrame.h"

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
        DWORD   bcSize;                 /* used to get to color table */
        WORD    bcWidth;
        WORD    bcHeight;
        WORD    bcPlanes;
        WORD    bcBitCount;
} BITMAPCOREHEADER;
#endif

XlsConverter::XlsConverter(const std::wstring & xls_file, const std::wstring & _xlsx_path, const ProgressCallback* CallBack) 
{
	xlsx_path		= _xlsx_path;
	output_document = NULL;
	xlsx_context	= NULL;
	
	pCallBack			= CallBack;
	bUserStopConvert	= false;
	is_older_version	= false;

	try{
		XLS::CompoundFile cfile(xls_file, XLS::CompoundFile::cf_ReadMode);

		if (cfile.isError())return;

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

        _UINT16 workbook_code_page = XLS::WorkbookStreamObject::DefaultCodePage;
		if(summary)
		{
			OLEPS::SummaryInformation summary_info(summary);
			workbook_code_page = summary_info.GetCodePage(); //from software last open 
		}
		if(doc_summary)
		{
			OLEPS::SummaryInformation doc_summary_info(doc_summary);
			workbook_code_page = doc_summary_info.GetCodePage(); 
		}
		if(  0/*error*/ == workbook_code_page)//|| 65001 /*UTF-8*/ == workbook_code_page || 1200/* UTF-16 */ == workbook_code_page
		{
			workbook_code_page = XLS::WorkbookStreamObject::DefaultCodePage;
		}

		xls_global_info = boost::shared_ptr<XLS::GlobalWorkbookInfo>(new XLS::GlobalWorkbookInfo(workbook_code_page, this));


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

	if (xls_global_info->Version < 0x0600) 
	{
		std::wstring sVer = STR::int2hex_wstr(xls_global_info->Version);
		Log::error("Version xls is old !!! - " + std::string(sVer.begin(), sVer.end()));
		is_older_version = true;
		//return;
	}

	
	output_document		= new oox::package::xlsx_document();
    xlsx_context		= new oox::xlsx_conversion_context(output_document);
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
	case XLS::typeTxO:
		{
			XLS::TxO * txo = dynamic_cast<XLS::TxO *>(xls_unknown);
			convert(txo);
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

	int count_sheets = 0, count_chart_sheets = 0;
    for (int i=0 ; i < woorkbook->m_arWorksheetSubstream.size(); i++)
	{
		if (woorkbook->m_arWorksheetSubstream[i]->get_type() == XLS::typeWorksheetSubstream)
		{
			count_sheets++;
			xls_global_info->current_sheet = count_sheets;
		
			xlsx_context->start_table(xls_global_info->sheets_names.size() > i ? xls_global_info->sheets_names[i] : L"Sheet_" + boost::lexical_cast<std::wstring>(count_sheets));
			xlsx_context->set_state(xls_global_info->sheets_state.size() > i ? xls_global_info->sheets_state[i] : L"visible");

			convert(dynamic_cast<XLS::WorksheetSubstream*>(woorkbook->m_arWorksheetSubstream[i].get()));
		}
		else if (woorkbook->m_arWorksheetSubstream[i]->get_type() == XLS::typeChartSheetSubstream)
		{
			count_chart_sheets++;
			xls_global_info->current_sheet = -1; 
			xlsx_context->start_table(xls_global_info->sheets_names.size() > i ? xls_global_info->sheets_names[i] : L"ChartSheet_" + boost::lexical_cast<std::wstring>(count_chart_sheets));

			convert_chart_sheet(dynamic_cast<XLS::ChartSheetSubstream*>(woorkbook->m_arWorksheetSubstream[i].get()));
		}

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

	if (sheet->m_arWINDOW.size() > 0)
	{
		sheet->m_arWINDOW[0]->serialize(xlsx_context->current_sheet().sheetViews());
	}
	if (sheet->m_Dimensions)
	{
		sheet->m_Dimensions->serialize(xlsx_context->current_sheet().dimension());
	}
	if (sheet->m_SheetExt)
	{
		sheet->m_SheetExt->serialize(xlsx_context->current_sheet().sheetProperties());
		// todooo + GLOBALS->WsBool
	}
	if (sheet->m_GLOBALS)
	{
		XLS::GLOBALS * globals  = dynamic_cast<XLS::GLOBALS *>(sheet->m_GLOBALS.get());
		XLS::COLUMNS * columns = dynamic_cast<XLS::COLUMNS *>(sheet->m_COLUMNS.get());

		if (columns)
		{
			globals->m_DefColWidth	= columns->m_DefColWidth; 
		}
		globals->m_DxGCol = sheet->m_DxGCol;
		
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
	if (sheet->m_arMergeCells.size() > 0)
	{
		CP_XML_WRITER(xlsx_context->current_sheet().mergeCells())    
		{
			CP_XML_NODE(L"mergeCells")
			{  		
                for (int i = 0 ; i < sheet->m_arMergeCells.size(); i++)
				{
					sheet->m_arMergeCells[i]->serialize(CP_XML_STREAM());
				}
			}
		}
	}
    for (int i = 0 ; i < sheet->m_arHLINK.size(); i++)
	{
		convert((XLS::HLINK*)sheet->m_arHLINK[i].get());
	}

	if (sheet->m_SORTANDFILTER)
	{
		sheet->m_SORTANDFILTER->serialize(xlsx_context->current_sheet().sheetSortAndFilters());
	}

	if (sheet->m_CONDFMTS)
	{
		sheet->m_CONDFMTS->serialize(xlsx_context->current_sheet().conditionalFormatting());
	}

	convert((XLS::OBJECTS*)sheet->m_OBJECTS.get(), sheet);

	if (sheet->m_arNote.size() > 0 && xls_global_info->Version < 0x0600)
	{
		for (int i = 0 ; i < sheet->m_arNote.size(); i++)
		{
			convert((XLS::Note*)sheet->m_arNote[i].get());
		}
	}

	if (sheet->m_PAGESETUP)
	{
		sheet->m_PAGESETUP->serialize(xlsx_context->current_sheet().pageProperties());
	}
 	
	if (sheet->m_arCUSTOMVIEW.size() > 0)
	{
		CP_XML_WRITER(xlsx_context->current_sheet().customViews())    
		{
			CP_XML_NODE(L"customSheetViews")
            {
				for (int i = 0 ; i < sheet->m_arCUSTOMVIEW.size(); i++)
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
}

void XlsConverter::convert(XLS::GlobalsSubstream* global)
{
	if (global == NULL) return;
	
	convert((XLS::FORMATTING*)global->m_Formating.get());
	
	convert((XLS::THEME*)global->m_THEME.get());

	convert((XLS::SHAREDSTRINGS*)global->m_SHAREDSTRINGS.get());

    for (int i = 0 ; i < global->m_arLBL.size(); i++)
	{
		convert((XLS::LBL*)global->m_arLBL[i].get());
	}

    for (int i = 0 ; i < global->m_arMSODRAWINGGROUP.size(); i++)
	{
		convert((XLS::MSODRAWINGGROUP*)global->m_arMSODRAWINGGROUP[i].get());
	}
    for (int i = 0 ; i < global->m_arWindow1.size(); i++)
	{
		global->m_arWindow1[i]->serialize(xlsx_context->workbook_views());
	}
    for (int i = 0 ; i < global->m_arUserBView.size(); i++)
	{
		global->m_arUserBView[i]->serialize(xlsx_context->custom_views());
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
	
	if (id < 0)		id = xlsx_context->get_mediaitems().count_image + 1000; // встроенные в поток 

	xlsx_context->get_mediaitems().create_media_path(xlsx_path);

	bool res = false;
	std::wstring file_name = L"image" + boost::lexical_cast<std::wstring>(id);
	

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
			
			bPNG = true;
			if (stride < header->biWidth)
			{
				bPNG = false;
			}
		}
		else if (header->biBitCount >=24)
		{
			offset = 40; //sizeof(BITMAPINFOHEADER)

			frame.put_Height	(header->biHeight );
			frame.put_Width		(header->biWidth );
			
			int sz_bitmap = header->biHeight * header->biWidth * header->biBitCount/ 8;
			
			//if (header->biWidth % 2 != 0 && sz_bitmap < size -offset)
			//	header->biWidth++;
			
			int stride = (size - offset) / header->biHeight;

			bPNG = true;
			if (stride < header->biWidth)
			{
				bPNG = false;
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
				DWORD dwLen		= biSizeImage;	file.WriteFile((BYTE*)&dwLen,	4);
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

void XlsConverter::convert(ODRAW::OfficeArtBStoreContainer* art_bstore)
{
	if (art_bstore == NULL) return;
	if (art_bstore->rgfb.size() < 1) return;

    for (int i =0 ; i < art_bstore->rgfb.size(); i++)
	{
		int bin_id = i + 1;

		WriteMediaFile(art_bstore->rgfb[i]->pict_data, art_bstore->rgfb[i]->pict_size, art_bstore->rgfb[i]->pict_type, bin_id);
	}
}

void XlsConverter::convert(XLS::HLINK * HLINK_)
{
	XLS::HLink * hLink = dynamic_cast<XLS::HLink*>(HLINK_->m_HLink.get());
	
	std::wstring target;
	
	bool bExternal = false;
	
	if (hLink->hyperlink.hlstmfHasMoniker)
	{
		target = GetTargetMoniker(hLink->hyperlink.oleMoniker.data.get());
		bExternal = true;
	}
	else if (hLink->hyperlink.hlstmfHasLocationStr)
	{
		target = hLink->hyperlink.location.value();
	}

	std::wstring display = hLink->hyperlink.displayName;

	if (display.empty())	display = target;

	xlsx_context->get_table_context().add_hyperlink( hLink->ref8.toString(), target, display, bExternal);
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
	if (imdata->cf == 0x09 && imdata->env == 0x02)	type_image = L".pict";
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

	CP_XML_WRITER(xlsx_context->current_sheet().picture())    
	{
		CP_XML_NODE(L"picture")
		{
			CP_XML_ATTR(L"r:id", rId);
		}
	}

}

void XlsConverter::convert(XLS::THEME* theme)
{
	if (theme == NULL) return;
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
		//{
		//	_group_object gr;		
		//	if (group_objects.back().ind < group_objects.back().spgr->child_records.size())
		//	{
		//		gr.spgr		= dynamic_cast<ODRAW::OfficeArtSpgrContainer*>(group_objects.back().spgr->child_records[group_objects.back().ind++].get());
		//		gr.count	= gr.spgr->child_records.size();
		//		group_objects.push_back(gr);
		//	}
		//	else //сюда попадать не должно !!!!
		//		continue;
		//}

		if (xlsx_context->get_drawing_context().start_drawing(type_object))
		{
			convert(obj->old_version.anchor.get());

			if (obj->old_version.bFill)
				xlsx_context->get_drawing_context().set_fill_old_version(obj->old_version.fill);
			else 
				xlsx_context->get_drawing_context().set_fill_type(0);	//no fill
			
			xlsx_context->get_drawing_context().set_name(obj->old_version.name);
			xlsx_context->get_drawing_context().set_line_old_version(obj->old_version.line);
			xlsx_context->get_drawing_context().set_flag_old_version(obj->old_version.flag, obj->old_version.flag2);

			for (int i = 0 ; i < obj->old_version.additional.size(); i++)
			{
				convert(obj->old_version.additional[i].get());
			}

			convert(image_obj);
			convert(chart);

			xlsx_context->get_drawing_context().end_drawing();
		}
		if (IMDATAOBJECT || CHART)
		{
			elem++;
			count++;
		}	
		//while ((group_objects.size() >0) && (group_objects.back().ind >= group_objects.back().count))
		//{
		//	group_objects.back().spgr = NULL;
		//	group_objects.pop_back();
		//	
		//	xlsx_context->get_drawing_context().end_group();
		//}
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
			if (type_object <0) type_object = 0x0006;			
		}

//-----------------------------------------------------------------------------
		if (type_object < 0)continue;
		
		ODRAW::OfficeArtSpContainer *sp			= NULL;

		if (type_object == 0)
		{
			_group_object gr;		
			if (group_objects.back().ind < group_objects.back().spgr->child_records.size())
			{
				gr.spgr		= dynamic_cast<ODRAW::OfficeArtSpgrContainer*>(group_objects.back().spgr->child_records[group_objects.back().ind++].get());
				gr.count	= gr.spgr->child_records.size();
				group_objects.push_back(gr);
			}
			else //сюда попадать не должно !!!!
				continue;
		}
		if ((group_objects.back().spgr ) && ( group_objects.back().ind < group_objects.back().count))
		{
			sp	= dynamic_cast<ODRAW::OfficeArtSpContainer*>(group_objects.back().spgr->child_records[group_objects.back().ind++].get());
		}	
		
		if (xlsx_context->get_drawing_context().start_drawing(type_object))
		{
			convert(sp);

			if ((!sp) || (!sp->m_OfficeArtAnchor))
			{
				if ((group_objects.back().spgr ) && ( group_objects.back().ind < group_objects.back().count))
				{
					sp	= dynamic_cast<ODRAW::OfficeArtSpContainer*>(group_objects.back().spgr->child_records[group_objects.back().ind++].get());
				}
				convert(sp, true);
			}			
			if (type_object == 0x19)
			{	
				text_obj->preserve_enabled = true;

				for (int i = 0 ; i < sheet->m_arNote.size(); i++)
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

void XlsConverter::convert(ODRAW::OfficeArtSpContainer *sp, bool anchor_only)
{
	if (sp == NULL) return;

	if (!anchor_only)
	{
		convert(sp->m_OfficeArtFSP.get());

		for (int i = 0; i < sp->child_records.size(); i++)
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
	case XLS::typeOfficeArtFSP:
		{
			convert(dynamic_cast<ODRAW::OfficeArtFSP *>(art));
		}break;
	case XLS::typeOfficeArtFSPGR:
		{
			ODRAW::OfficeArtFSPGR * ch = dynamic_cast<ODRAW::OfficeArtFSPGR *>(art);

			xlsx_context->get_drawing_context().set_group_anchor(ch->_x, ch->_y, ch->_cx, ch->_cy);
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
			//xlsx_context->get_drawing_context().set_child_anchor(ch->_x, ch->_y, ch->_cx, ch->_cy);
			xlsx_context->get_drawing_context().set_sheet_anchor(ch->colL, ch->_dxL, ch->rwT, ch->_dyT, ch->colR, ch->_dxR, ch->rwB, ch->_dyB);
		}break;
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
	for (int i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
			case NSOfficeDrawing::fillType:
			{
				switch(props[i]->op)
				{
				case 1://fillPattern:
					{
						xlsx_context->get_drawing_context().set_fill_type(2);
						//texture + change black to color2, white to color1
					}break;
					case 2://fillTexture :
					{
						xlsx_context->get_drawing_context().set_fill_type(3);
						xlsx_context->get_drawing_context().set_fill_texture_mode(0);
					}break;
					case 3://fillPicture :
					{
						xlsx_context->get_drawing_context().set_fill_type(3);
						xlsx_context->get_drawing_context().set_fill_texture_mode(1);
					}break;
					case 4://fillShadeCenter://1 color
					case 5://fillShadeShape:
					{
						xlsx_context->get_drawing_context().set_fill_type(5);
					}break;//
					case 6://fillShadeTitle://2 colors and more
					case 7://fillShade : 
					case 8://fillShadeScale: 
					{
						xlsx_context->get_drawing_context().set_fill_type(4);
					}break;
					case 9://fillBackground:
					{
						xlsx_context->get_drawing_context().set_fill_type(0);
					}break;				
				}
			}break;
			case NSOfficeDrawing::fillColor:
			{
				ODRAW::fillColor * fill = (ODRAW::fillColor *)(props[i].get());
				ODRAW::OfficeArtCOLORREF color(fill->op);
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
			case NSOfficeDrawing::fillOpacity:
			{
				ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
				xlsx_context->get_drawing_context().set_fill_opacity(fixed_point->dVal);
			}break;
			case NSOfficeDrawing::fillBackColor:
			{
				ODRAW::fillColor * fill = (ODRAW::fillColor *)(props[i].get());
				ODRAW::OfficeArtCOLORREF color(fill->op);
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
			case NSOfficeDrawing::fillBackOpacity:
			{
				ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
				xlsx_context->get_drawing_context().set_fill_opacity(fixed_point->dVal, true);
			}break;
			case NSOfficeDrawing::fillBlip:
			{
				std::wstring target;
				ODRAW::fillBlip *fillBlip = (ODRAW::fillBlip *)(props[i].get());
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
			case NSOfficeDrawing::fillAngle:
			{
				ODRAW::fillAngle * angle = (ODRAW::fillAngle *)(props[i].get());
				xlsx_context->get_drawing_context().set_fill_angle(angle->dVal);
			}break;
			case NSOfficeDrawing::fillFocus:
			{
				xlsx_context->get_drawing_context().set_fill_focus(props[i]->op);
			}break;
			case NSOfficeDrawing::fillShadePreset:
			{
			}break;
			case NSOfficeDrawing::fillShadeColors:
			{
				ODRAW::fillShadeColors *shadeColors = (ODRAW::fillShadeColors *)(props[i].get());

				for (int i = 0 ; (shadeColors) && (i < shadeColors->fillShadeColors_complex.data.size()); i++)
				{
					ODRAW::OfficeArtCOLORREF & color = shadeColors->fillShadeColors_complex.data[i].color;

					std::wstring strColor;
					if (!color.sColorRGB.empty())	strColor = color.sColorRGB;
					else if (color. fPaletteIndex)
					{
						std::map<int,  std::wstring>::iterator it = xls_global_info->colors_palette.find(color.index);
						if (it != xls_global_info->colors_palette.end())	strColor = it->second;
					}
					if (!strColor.empty())
						xlsx_context->get_drawing_context().add_fill_colors( shadeColors->fillShadeColors_complex.data[i].dPosition, strColor);
					else
					{
						xlsx_context->get_drawing_context().add_fill_colors( shadeColors->fillShadeColors_complex.data[i].dPosition, 
																									color.index, color.fSchemeIndex ? 1 : 3 );
					}
				}
			}break;
			case NSOfficeDrawing::fillShadeType:
			{
			}break;
			case NSOfficeDrawing::fillBoolean:
			{
				ODRAW::FillStyleBooleanProperties * bools = (ODRAW::FillStyleBooleanProperties *)(props[i].get());
				if (bools)
				{
					if (bools->fUsefFilled && bools->fFilled == false) 
						xlsx_context->get_drawing_context().set_fill_type(0);
				}
			}break;
		
		}
	}
}
void XlsConverter::convert_line_style(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.size() < 1) return;

	for (int i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
			case NSOfficeDrawing::lineColor:
			{
				ODRAW::OfficeArtCOLORREF color(props[i]->op);
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
			case NSOfficeDrawing::lineType:
			{
				xlsx_context->get_drawing_context().set_line_type(props[i]->op);
			}break;
			case NSOfficeDrawing::lineFillBlip: //blip 
			{
			}break;
			case NSOfficeDrawing::lineWidth: 
				xlsx_context->get_drawing_context().set_line_width(props[i]->op);
			{
			}break;
			case NSOfficeDrawing::lineStyle:
			{
				xlsx_context->get_drawing_context().set_line_style(props[i]->op);
			}break;
			case NSOfficeDrawing::lineDashing:
			{
				xlsx_context->get_drawing_context().set_line_dash(props[i]->op);
			}break;
			case NSOfficeDrawing::lineBoolean:
			{
				ODRAW::LineStyleBooleanProperties * bools = (ODRAW::LineStyleBooleanProperties *)(props[i].get());
				if (bools)
				{
					if (bools->fUsefLine && bools->fLine == false) 
						xlsx_context->get_drawing_context().set_line_type(5); //no line
				}
			}break;
		}
	}
}
void XlsConverter::convert_blip(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.size() < 1) return;

	for (int i = 0 ; i < props.size() ; i++)
	{
		ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
		switch(props[i]->opid)
		{
		case 0x100:
			{
				if (fixed_point)
					xlsx_context->get_drawing_context().set_crop_top(fixed_point->dVal);
			}break;
		case 0x101:
			{
				if (fixed_point)
					xlsx_context->get_drawing_context().set_crop_bottom(fixed_point->dVal);
			}break;
		case 0x102:
			{
				if (fixed_point)
					xlsx_context->get_drawing_context().set_crop_left(fixed_point->dVal);
			}break;
		case 0x103:
			{
				if (fixed_point)
					xlsx_context->get_drawing_context().set_crop_right(fixed_point->dVal);
			}break;
		case 0x104:
			{
				bool isIternal = false;
				std::wstring target;
				std::wstring rId = xlsx_context->get_mediaitems().find_image(props[i]->op , target, isIternal);
				xlsx_context->get_drawing_context().set_fill_texture(target);
			}break;
		}
	}
}
void XlsConverter::convert_geometry(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.size() < 1) return;

	oox::_rect					rect;
	std::vector<_CP_OPT(int)>	adjustValues(8);
	
	for (int i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
		case 0x0140:	rect.x	= props[i]->op; break;
		case 0x0141:	rect.y	= props[i]->op; break;
		case 0x0142:	rect.cx	= props[i]->op; break;
		case 0x0143:	rect.cy = props[i]->op; break;
		case 0x0144:
			xlsx_context->get_drawing_context().set_custom_path(props[i]->op); break;
		case 0x0145:
			{
				ODRAW::PVertices * v = (ODRAW::PVertices *)(props[i].get());
				xlsx_context->get_drawing_context().set_custom_verticles(v->complex.data);
			}break;
		case 0x0146:
			{
				ODRAW::PSegmentInfo * s = (ODRAW::PSegmentInfo *)(props[i].get());
				xlsx_context->get_drawing_context().set_custom_segments(s->complex.data);
			}break;
		case 0x0147: //adjustValue .... //adjust8Value
		case 0x0148:
		case 0x0149:
		case 0x014A:
		case 0x014B:
		case 0x014C:
		case 0x014D:
		case 0x014E:
			{
				adjustValues[props[i]->opid - 0x0147] = props[i]->op ;
			}break;
		case 0x0151:
			{
				ODRAW::pAdjustHandles * a = (ODRAW::pAdjustHandles *)(props[i].get());
				xlsx_context->get_drawing_context().set_custom_adjustHandles(a->complex.data);
			}break;
		case 0x0156:
			{
				ODRAW::pGuides* s = (ODRAW::pGuides *)(props[i].get());
				xlsx_context->get_drawing_context().set_custom_guides(s->complex.data);
			}break;
		}
	}
	rect.cy -= rect.y;
	rect.cx -= rect.x;
	xlsx_context->get_drawing_context().set_custom_rect(rect);
	
	xlsx_context->get_drawing_context().set_custom_adjustValues(adjustValues);
}
void XlsConverter::convert_geometry_text(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.size() < 1) return;

	for (int i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
		case NSOfficeDrawing::gtextUNICODE://word art text
			{
				ODRAW::anyString *str = dynamic_cast<ODRAW::anyString*>(props[i].get());
				if (str) xlsx_context->get_drawing_context().set_wordart_text(str->string_);
			}break;
		case NSOfficeDrawing::gtextFont:
			{
				ODRAW::anyString *str = dynamic_cast<ODRAW::anyString*>(props[i].get());
				if (str) xlsx_context->get_drawing_context().set_wordart_font(str->string_);
			}break;
		case NSOfficeDrawing::gtextSize:
			{
				xlsx_context->get_drawing_context().set_wordart_size((INT)((props[i]->op >> 16) & 0x0000FFFF));
			}break;
		case NSOfficeDrawing::gtextSpacing:
			{
				ODRAW::FixedPoint *val = dynamic_cast<ODRAW::FixedPoint*>(props[i].get());
				if (val) xlsx_context->get_drawing_context().set_wordart_spacing(val->dVal);
			}break;
		case NSOfficeDrawing::gtextAlign:
			{
				switch (props[i]->op)
				{
				case NSOfficeDrawing::alignTextLeft:
					xlsx_context->get_drawing_context().set_text_align(1);	break;
				case NSOfficeDrawing::alignTextCenter:
					xlsx_context->get_drawing_context().set_text_align(2);	break;
				case NSOfficeDrawing::alignTextRight:
					xlsx_context->get_drawing_context().set_text_align(3);	break;
				case NSOfficeDrawing::alignTextWordJust:
					xlsx_context->get_drawing_context().set_text_align(4);	break;
				case NSOfficeDrawing::alignTextLetterJust:
					xlsx_context->get_drawing_context().set_text_align(5);	break;
				}				
			}break;
		case 0xff:
			{				
				ODRAW::GeometryTextBooleanProperties *bools = dynamic_cast<ODRAW::GeometryTextBooleanProperties*>(props[i].get());
				if (bools) 
				{
					if (bools->fUsegFBold		&& bools->fBold)		xlsx_context->get_drawing_context().set_wordart_bold	(true); 
					if (bools->fUsegFItalic		&& bools->fItalic)		xlsx_context->get_drawing_context().set_wordart_italic	(true); 
					if (bools->fUsegFVertical	&& bools->fVertical)	xlsx_context->get_drawing_context().set_wordart_vertical(true); 
					if (bools->fUsegFUnderline	&& bools->fUnderline)	xlsx_context->get_drawing_context().set_wordart_underline(true); 
					if (bools->fStrikethrough	&& bools->fStrikethrough)xlsx_context->get_drawing_context().set_wordart_strike(true);

					//if (bools->fUsegFShrinkFit && bools->fShrinkFit)
				}
			}break;
		}
	}
}
void XlsConverter::convert_text(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	if (props.size() < 1) return;

	RECT text_margin = {0x00016530, 0x0000b298, 0x00016530, 0x0000b298};
	for (int i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
		case NSOfficeDrawing::dxTextLeft:		text_margin.left	= props[i]->op;	break;
		case NSOfficeDrawing::dxTextRight:		text_margin.right	= props[i]->op;	break;			
		case NSOfficeDrawing::dyTextTop:		text_margin.top		= props[i]->op;	break;
		case NSOfficeDrawing::dyTextBottom:		text_margin.bottom	= props[i]->op;	break;
		case NSOfficeDrawing::WrapText:
			{
				xlsx_context->get_drawing_context().set_text_wrap(props[i]->op);				
			}break;
		case NSOfficeDrawing::txflTextFlow:
			{
				xlsx_context->get_drawing_context().set_text_vertical(props[i]->op);
			}break;
		case NSOfficeDrawing::textBoolean:
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
		case NSOfficeDrawing::anchorText:
			{
				switch (props[i]->op)
				{
				case NSOfficeDrawing::anchorTop:
				case NSOfficeDrawing::anchorTopBaseline:
					{	
						xlsx_context->get_drawing_context().set_text_align		(1);
						xlsx_context->get_drawing_context().set_text_vert_align	(1);
					}break;
				case NSOfficeDrawing::anchorMiddle:
					{
						xlsx_context->get_drawing_context().set_text_align		(1);
						xlsx_context->get_drawing_context().set_text_vert_align	(2);
					}break;						
				case NSOfficeDrawing::anchorBottom:
				case NSOfficeDrawing::anchorBottomBaseline:
					{
						xlsx_context->get_drawing_context().set_text_align		(1);
						xlsx_context->get_drawing_context().set_text_vert_align	(3);
					}break;
				case NSOfficeDrawing::anchorTopCentered:
				case NSOfficeDrawing::anchorTopCenteredBaseline:
					{
						xlsx_context->get_drawing_context().set_text_align		(2);
						xlsx_context->get_drawing_context().set_text_vert_align	(1);
					}break;
				case NSOfficeDrawing::anchorMiddleCentered:
					{
						xlsx_context->get_drawing_context().set_text_align		(2);
						xlsx_context->get_drawing_context().set_text_vert_align	(2);
					}break;
				case NSOfficeDrawing::anchorBottomCentered:
				case NSOfficeDrawing::anchorBottomCenteredBaseline:
					{
						xlsx_context->get_drawing_context().set_text_align		(2);
						xlsx_context->get_drawing_context().set_text_vert_align	(3);
					}break;
				}
			}break;
		}
	}
	xlsx_context->get_drawing_context().set_text_margin(text_margin);
}
void XlsConverter::convert_shadow(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	for (int i = 0 ; i < props.size() ; i++)
	{
	}
}
void XlsConverter::convert_shape(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	for (int i = 0 ; i < props.size() ; i++)
	{

	}
}
void XlsConverter::convert_group_shape(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	for (int i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
		case 0x380:
			{
				ODRAW::anyString *str = dynamic_cast<ODRAW::anyString*>(props[i].get());
		xlsx_context->get_drawing_context().set_name(str->string_);
			}break;
		case 0x381:
			{
				ODRAW::anyString *str = dynamic_cast<ODRAW::anyString*>(props[i].get());
				xlsx_context->get_drawing_context().set_description(str->string_);
			}break;
		case 0x0382:
			{
				ODRAW::pihlShape *pihlShape = dynamic_cast<ODRAW::pihlShape*>(props[i].get());
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
						sTarget = pihlShape->complex.hyperlink.location.value();
					}

					std::wstring sDisplay = pihlShape->complex.hyperlink.displayName;
					if (sDisplay.empty())	sDisplay = sTarget;

					xlsx_context->get_drawing_context().set_hyperlink( sTarget, sDisplay, bExternal);				
				}
			}break;
		}
	}
}

void XlsConverter::convert(XLS::Note* note)
{
	if (note == NULL) return;

	note->note_sh.calculate();

	if (xls_global_info->Version < 0x0600)
	{
		xlsx_context->get_comments_context().start_comment();
	}

	xlsx_context->get_comments_context().set_ref	(note->note_sh.ref_, 
													 note->note_sh.col, 
													 note->note_sh.row);
	xlsx_context->get_comments_context().set_author	(note->note_sh.stAuthor);
	xlsx_context->get_comments_context().set_visibly(note->note_sh.fShow);

	if (xls_global_info->Version < 0x0600)
	{
		//todooo размеры произвольные .. можно сделать оценку по размеру строки
		xlsx_context->get_comments_context().set_size(120, 64, note->note_sh.x_/ 12700. , note->note_sh.y_/ 12700.);
		xlsx_context->get_comments_context().set_content(std::wstring(L"<t>") + note->note_sh.stText.value() + std::wstring(L"</t>"));
		xlsx_context->get_comments_context().end_comment();
	}
}

void XlsConverter::convert_transform(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	for (int i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
			case 0x0004:
			{
				double d = props[i]->op / 65536.;
				d *= 60000; //60 000 per 1 gr - 19.5.5 oox 

				xlsx_context->get_drawing_context().set_rotation((int)d);
			}break;
		}
	}
}
void XlsConverter::convert(ODRAW::OfficeArtFOPT * fort)
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
				for (std::list<XLS::BaseObjectPtr>::iterator it = sharedstrings->elements_.begin(); it != sharedstrings->elements_.end(); it++)
				{
					(*it)->serialize(CP_XML_STREAM());
				}
			}

			for (int i = 0 ; i < xls_global_info->arAddedSharedStrings.size(); i++)
			{
				CP_XML_NODE(L"si")
				{	
					CP_XML_NODE(L"t")
					{		
						CP_XML_ATTR(L"xml:space", "preserve");
						CP_XML_STREAM() << STR::escape_ST_Xstring(xml::utils::replace_text_to_xml(xls_global_info->arAddedSharedStrings[i]));
					}
				}
			}
		}
	}
}

void XlsConverter::convert(XLS::TxO * text_obj)
{
	if (text_obj == NULL) return;
	
	std::wstringstream strm;
	text_obj->serialize(strm);

	int rot = text_obj->rot;

	if (rot > 0)
	{
		xlsx_context->get_drawing_context().set_text_align		(text_obj->vAlignment);
		if (rot==3)
		{
			if		(text_obj->hAlignment == (unsigned char)1) text_obj->hAlignment = 3;
			else if (text_obj->hAlignment == (unsigned char)3) text_obj->hAlignment = 1;
		}
		xlsx_context->get_drawing_context().set_text_vert_align	(text_obj->hAlignment);
	}
	else
	{
		xlsx_context->get_drawing_context().set_text_align		(text_obj->hAlignment);
		xlsx_context->get_drawing_context().set_text_vert_align	(text_obj->vAlignment);
	}
	xlsx_context->get_drawing_context().set_text(strm.str());
}

void XlsConverter::convert_chart_sheet(XLS::ChartSheetSubstream * chart)
{
	if (chart == NULL) return;
		
	if (xlsx_context->get_drawing_context().start_drawing(0x0005))		
	{
		xlsx_context->get_drawing_context().set_id(1);
		convert(chart);

		xlsx_context->get_drawing_context().end_drawing();
	}
}
void XlsConverter::convert(XLS::ChartSheetSubstream * chart)
{
	if (chart == NULL) return;

	chart->serialize(xlsx_context->current_chart().chartData());	
	//convert(chart->m_OBJECTSCHART.get());непонятные какие то текстбоксы - пустые и бз привязок
}
