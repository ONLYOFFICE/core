#include "XlsConverter.h"

#include "../XlsFormat/Document/Document.h" 
#include "../XlsFormat/Binary/CompoundFile.h"
#include "../XlsFormat/Binary/BinSmartPointers.h"
#include "../XlsFormat/Binary/CFStreamCacheReader.h"
//#include "../XlsFormat/Binary/CFStreamCacheWriter.h"
#include "../XlsFormat/Logic/GlobalWorkbookInfo.h"
#include "../XlsFormat/Logic/WorkbookStreamObject.h"
#include "../XlsFormat/Logic/WorksheetSubstream.h"
#include "../XlsFormat/Logic/GlobalsSubstream.h"
#include "../XlsFormat/Logic/BinProcessor.h"
#include "../XlsFormat/Logic/SummaryInformationStream/SummaryInformation.h"
//#include "../XlsFormat\Auxiliary\HelpersTagsGenerator.h"
#include "../XlsFormat/Logic/BaseObject.h"

#include "xlsx_conversion_context.h"
#include "xlsx_package.h"

XlsConverter::XlsConverter(const std::wstring & path, const ProgressCallback* CallBack) 
{
	output_document = NULL;
	xlsx_context	= NULL;
	
	pCallBack			= CallBack;
	bUserStopConvert	= false;

	try{
		XLS::CompoundFile cfile(path, XLS::CompoundFile::cf_ReadMode);

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
void XlsConverter::write(const std::wstring & path)
{
	if (!output_document)return;
	output_document->write(path);

	delete output_document; output_document = NULL;

	if (UpdateProgress(1000000))return;
}

void XlsConverter::convertDocument()
{
    if (!xls_document)     return;
    if (!output_document)   return;
    if (!xlsx_context)       return;

	xlsx_context->start_document();

	convert(xls_document.get());

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
	case XLS::typeWorkbookStreamObject:
		{
			convert(dynamic_cast<XLS::WorkbookStreamObject*>(xls_unknown));
		}break;
	case XLS::typeWorksheetSubstream:
		{
			convert(dynamic_cast<XLS::WorksheetSubstream*>(xls_unknown));
		}break;
	case XLS::typeGlobalsSubstream:
		{
			convert(dynamic_cast<XLS::GlobalsSubstream*>(xls_unknown));
		}break;
	default:
		{
			for (std::list<XLS::BaseObjectPtr>::iterator it = xls_unknown->elements_.begin(); it != xls_unknown->elements_.end(); it++)
			{
				it->get()->serialize(xlsx_context->current_stream());
			}
		}
	}

}

void XlsConverter::convert(XLS::WorkbookStreamObject* woorkbook)
{
	if (woorkbook == NULL) return;

	for (std::list<XLS::BaseObjectPtr>::iterator it = woorkbook->elements_.begin(); it != woorkbook->elements_.end(); it++)
	{
		convert(it->get());
	}
}

void XlsConverter::convert(XLS::WorksheetSubstream* sheet)
{
	if (sheet == NULL) return;

	xlsx_context->start_table(L"", L"");

	xlsx_context->end_table();

}

void XlsConverter::convert(XLS::GlobalsSubstream* elem)
{
	for (std::list<XLS::BaseObjectPtr>::iterator it = woorkbook->elements_.begin(); it != woorkbook->elements_.end(); it++)
	{
		convert(it->get());
	}

}

void XlsConverter::convert(XLS::FORMATTING* formating)
{
}