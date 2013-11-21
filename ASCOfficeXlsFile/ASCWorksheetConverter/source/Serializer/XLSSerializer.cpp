#include "stdafx.h"
#include "XLSSerializer.h"
#include <XLS_bin/CFStream.h>
#include <XLS_bin/CompoundFile.h>
#include <XLS_bin/CFStreamCacheReader.h>
#include <XLS_bin/CFStreamCacheWriter.h>
#include <XLS_logic/GlobalWorkbookInfo.h>
#include <XLS_logic/WorkbookStreamObject.h>
#include <XLS_logic/BinProcessor.h>
#include <Exception/CompoundFileFormatError.h>
#include "XLS_logic/SummaryInformationStream/SummaryInformation.h"
#include "Auxiliary/HelpersTagsGenerator.h"

namespace XLS
{;


const bool XLSSerializer::read(Document& doc, const _bstr_t& from)
{
	CompoundFile cfile(from, CompoundFile::cf_ReadMode);

/*
	CFStreamPtr summary(cfile.getSummaryInformationStream());
	size_t summary_size = static_cast<size_t>(summary->getStreamSize());
	char* summary_raw = new char[summary_size];
	summary->read(summary_raw, summary_size);

	std::ofstream file1("SummaryInformation.bin", std::ios_base::binary);
	file1.write(summary_raw, summary_size);
	file1.flush();
	return true;
*/

/*
	CFStreamPtr summary(cfile.getDocumentSummaryInformationStream());
	size_t summary_size = static_cast<size_t>(summary->getStreamSize());
	char* summary_raw = new char[summary_size];
	summary->read(summary_raw, summary_size);

	std::ofstream file1("DocumentSummaryInformation.bin", std::ios_base::binary);
	file1.write(summary_raw, summary_size);
	file1.flush();
	return true;
*/
	CFStreamPtr summary;
	CFStreamPtr doc_summary;

	try
	{
		summary = cfile.getSummaryInformationStream();
	}
	catch (EXCEPT::RT::CompoundFileFormatError&)
	{
	}

	try
	{
		doc_summary = cfile.getDocumentSummaryInformationStream();
	}
	catch (EXCEPT::RT::CompoundFileFormatError&)
	{
	}

	WORD workbook_code_page = WorkbookStreamObject::DefaultCodePage;
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
		workbook_code_page = WorkbookStreamObject::DefaultCodePage;
	}

	GlobalWorkbookInfoPtr global_info(new GlobalWorkbookInfo(workbook_code_page));

	CFStreamCacheReader stream_reader(cfile.getWorkbookStream(), global_info);
	BinReaderProcessor proc(stream_reader, doc.getRoot(), true);
	return	proc.mandatory(WorkbookStreamObject(workbook_code_page)) && 
			AUX::HelpersTagsGenerator(doc.getRoot()).GenHelpers();
}

const bool XLSSerializer::write(const Document& doc, const _bstr_t& to)
{
	CompoundFile cfile(to, CompoundFile::cf_WriteMode);

/*
	{
		char* summary_raw2 = new char[200];
		std::ifstream file2("SummaryInformation.bin", std::ios_base::binary);
		file2.read(summary_raw2, 200);

		CFStreamPtr summary2(cfile.createSummaryInformationStream());
		summary2->write(summary_raw2, 200);
		cfile.closeSummaryInformationStream();
	}
	{
		char* summary_raw = new char[116];
		std::ifstream file1("DocumentSummaryInformation.bin", std::ios_base::binary);
		file1.read(summary_raw, 116);

		CFStreamPtr summary(cfile.createDocumentSummaryInformationStream());
		summary->write(summary_raw, 116);
		cfile.closeDocumentSummaryInformationStream();
	}
*/
	GlobalWorkbookInfoPtr global_info(new GlobalWorkbookInfo(WorkbookStreamObject::DefaultCodePage));

	CFStreamCacheWriter stream_writer(cfile.createWorkbookStream(), global_info);
	BinWriterProcessor proc(stream_writer, doc.getRoot(), true);
	return proc.mandatory(WorkbookStreamObject());
}

} // namespace XLS
