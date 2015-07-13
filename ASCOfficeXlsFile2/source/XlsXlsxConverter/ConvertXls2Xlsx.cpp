
#include "ConvertXls2Xlsx.h"

#include "..\..\Common\OfficeFileErrorDescription.h"

#include "XlsFormat\Document\Document.h" 
#include "XlsFormat\Binary\CompoundFile.h"
#include "XlsFormat\Binary\BinSmartPointers.h"
#include "XlsFormat\Binary\CFStreamCacheReader.h"
//#include "XlsFormat\Binary\CFStreamCacheWriter.h"
#include "XlsFormat\Logic\GlobalWorkbookInfo.h"
#include "XlsFormat\Logic\WorkbookStreamObject.h"
#include "XlsFormat\Logic\BinProcessor.h"
#include "XlsFormat\Logic\SummaryInformationStream\SummaryInformation.h"
//#include "XlsFormat\Auxiliary\HelpersTagsGenerator.h"
#include "XlsFormat\Logic\BaseObject.h"

HRESULT ConvertXls2Xlsx(const std::wstring & srcFile, const std::wstring & dstPath, const ProgressCallback* CallBack)
{
	HRESULT hr = S_OK;

	try 
    {
		XLS::CompoundFile cfile(srcFile, XLS::CompoundFile::cf_ReadMode);

		Document		doc(dstPath);

		XLS::CFStreamPtr summary;
		XLS::CFStreamPtr doc_summary;

		try
		{
			summary = cfile.getSummaryInformationStream();
		}
		catch (...)
		{
			return AVS_ERROR_UNEXPECTED;
		}

		try
		{
			doc_summary = cfile.getDocumentSummaryInformationStream();
		}
		catch (...)
		{
			return AVS_ERROR_UNEXPECTED;
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

		XLS::GlobalWorkbookInfoPtr global_info(new XLS::GlobalWorkbookInfo(workbook_code_page));

		XLS::CFStreamCacheReader stream_reader(cfile.getWorkbookStream(), global_info);

		XLS::WorkbookStreamObject workbook;
		workbook.set_code_page(workbook_code_page);

		//XLS::BaseObjectPtr workbook_ptr = boost::shared_ptr<XLS::BaseObject>(&workbook);
		//XLS::BaseObjectPtr workbook_ptr = boost::shared_ptr<XLS::BaseObject>(new XLS::WorkbookStreamObject(workbook_code_page));
		XLS::BinReaderProcessor proc(stream_reader , &workbook , true);
		proc.mandatory(workbook);

		return	S_OK;

	}
	catch(...)
	{
		return AVS_ERROR_UNEXPECTED;
	}
	return hr;

}