#ifndef X2T_TESTER_H
#define X2T_TESTER_H

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>

#include "../../../Common/OfficeFileFormats.h"
#include "../../../Common/OfficeFileFormatChecker.h"

#include "../../../DesktopEditor/graphics/Timer.h"
#include "../../../DesktopEditor/graphics/TemporaryCS.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../DesktopEditor/common/StringBuilder.h"
#include "../../../DesktopEditor/fontengine/ApplicationFontsWorker.h"

#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../UnicodeConverter/UnicodeConverter_Encodings.h"

class CFormatsList
{
public:
	CFormatsList();

	std::vector<int> GetDocuments() const;
	std::vector<int> GetPresentations() const;
	std::vector<int> GetSpreadsheets() const;
	std::vector<int> GetCrossplatform() const;
	std::vector<int> GetImages() const;
	int GetPdf() const;

	bool IsDocument(int format) const;
	bool IsPresentation(int format) const;
	bool IsSpreadsheet(int format) const;
	bool IsCrossplatform(int format) const;
	bool IsImage(int format) const;
	bool IsPdf(int format) const;

	// all formats
	void SetDefault();

	// all writable formats
	void SetOutput();

	std::vector<int> AllFormats() const;

private:
	std::vector<int> m_documents;
	std::vector<int> m_presentations;
	std::vector<int> m_spreadsheets;
	std::vector<int> m_crossplatform;
	std::vector<int> m_images;
	int m_pdf;
};


// setups folders, parse xml config, takes formats, create CConverters
class Cx2tTester
{
public:
	struct Report
	{
		std::wstring inputFile;
		std::wstring outputFile;
		std::wstring direction;
		DWORD time;
		int inputSize;
		int outputSize;
		int exitCode;
		std::wstring log;
	};

	Cx2tTester(const std::wstring& configPath);
	~Cx2tTester();

	void SetConfig(const std::wstring& configPath);
	void Start();

	void WriteReportHeader();
	void WriteReport(const Report& report);
	void WriteReports(const std::vector<Report>& reports);
	void WriteTime();

	bool IsAllBusy();
	bool IsAllFree();

	NSCriticalSection::CRITICAL_SECTION m_coresCS;
	NSCriticalSection::CRITICAL_SECTION m_reportCS;
	NSCriticalSection::CRITICAL_SECTION m_outputCS;

	int m_currentProc;
	int m_maxProc;

private:
	// parse string like "docx txt" into vector of formats
	std::vector<int> ParseExtensionsString(std::wstring extensions, const CFormatsList& fl);

	// takes from config
	std::wstring m_reportFile;
	std::wstring m_inputDirectory;
	std::wstring m_outputDirectory;
	std::wstring m_x2tPath;

	std::wstring m_errorsXmlDirectory;

	// fonts
	bool m_bIsUseSystemFonts;
	std::vector<std::wstring> m_arAdditionalFontsDirs;

	NSFile::CFileBinary m_reportStream;

	// takes from config or sets all
	std::vector<int> m_inputFormats;
	std::vector<int> m_outputFormats;

	std::vector<std::wstring> m_inputFiles;

	// list of formats
	CFormatsList m_inputFormatsList;
	CFormatsList m_outputFormatsList;

	bool m_bIsErrorsOnly;
	bool m_bIsTimestamp;
	bool m_bIsDeleteOk;
	bool m_bIsfilenameCsvTxtParams;

	std::wstring m_defaultCsvTxtEndcoding;
	std::wstring m_defaultCsvDelimiter;

	DWORD m_timeStart;
};

// generates temp xml, convert, calls m_internal->writeReport
class CConverter : public NSThreads::CBaseThread
{
public:
	CConverter(Cx2tTester* internal);
	virtual ~CConverter();

	void SetInputFile(const std::wstring& inputFile);
	void SetInputFormat(int inputFormat);
	void SetOutputFilesDirectory(const std::wstring& outputFilesDirectory);
	void SetOutputFormats(const std::vector<int> outputFormats);
	void SetFontsDirectory(const std::wstring& fontsDirectory);
	void SetX2tPath(const std::wstring& x2tPath);
	void SetErrorsOnly(bool bIsErrorsOnly);
	void SetDeleteOk(bool bIsDeleteOk);
	void SetXmlErrorsDirectory(const std::wstring& errorsXmlDirectory);
	void SetCsvTxtEncoding(int csvTxtEncoding);
	void SetCsvDelimiter(std::wstring csvDelimiter);
	void SetFilesCount(int totalFiles, int currFile);

	virtual DWORD ThreadProc();

private:
	Cx2tTester* m_internal;

	std::wstring m_inputFile;
	int m_inputFormat;

	std::wstring m_outputFilesDirectory;
	std::vector<int> m_outputFormats;

	std::wstring m_fontsDirectory;
	COfficeFileFormatChecker checker;

	std::wstring m_x2tPath;
	std::wstring m_errorsXmlDirectory;

	int m_csvTxtEncoding;
	std::wstring m_csvDelimiter;

	bool m_bIsErrorsOnly;
	bool m_bIsDeleteOk;

	int m_totalFiles;
	int m_currFile;
};

#endif // X2T_TESTER_H
