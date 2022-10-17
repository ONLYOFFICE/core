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

	bool isDocument(int format) const;
	bool isPresentation(int format) const;
	bool isSpreadsheet(int format) const;
	bool isCrossplatform(int format) const;
	bool isImage(int format) const;
	bool isPdf(int format) const;

	// all formats
	void SetDefault();

	// all writable formats
	void SetOutput();

	std::vector<int> allFormats() const;

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
		std::wstring inputExt;
		std::wstring outputExt;
		DWORD time;
		int exitCode;
	};

	Cx2tTester(const std::wstring& configPath);
	~Cx2tTester();

	void setConfig(const std::wstring& configPath);
	void Start();

	void writeReportHeader();
	void writeReport(const Report& report);
	void writeReports(const std::vector<Report>& reports);
	void writeTime();

	bool isAllBusy();
	bool isAllFree();

	NSCriticalSection::CRITICAL_SECTION m_coresCS;
	NSCriticalSection::CRITICAL_SECTION m_reportCS;

	int m_currentProc;
	int m_maxProc;

private:
	// parse string like "docx txt" into vector of formats
	std::vector<int> parseExtensionsString(std::wstring extensions, const CFormatsList& fl);

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

	// list of formats
	CFormatsList m_inputFormatsList;
	CFormatsList m_outputFormatsList;

	bool m_bIsErrorsOnly;
	bool m_bIsTimestamp;

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

	void SetOnlyErrors(bool bIsErrorsOnly);
	void SetXmlErrorsDirectory(const std::wstring& errorsXmlDirectory);

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

	bool m_bIsErrorsOnly;
};

#endif // X2T_TESTER_H
