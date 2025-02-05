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
	CFormatsList(const CFormatsList& list);
	CFormatsList& operator=(const CFormatsList& list);

	std::vector<std::wstring> GetDocuments() const;
	std::vector<std::wstring> GetPresentations() const;
	std::vector<std::wstring> GetSpreadsheets() const;
	std::vector<std::wstring> GetCrossplatform() const;
	std::vector<std::wstring> GetDraw() const;
	std::vector<std::wstring> GetImages() const;
	std::wstring GetPdf() const;

	bool IsDocument(const std::wstring& ext) const;
	bool IsPresentation(const std::wstring& ext) const;
	bool IsSpreadsheet(const std::wstring& ext) const;
	bool IsCrossplatform(const std::wstring& ext) const;
	bool IsDraw(const std::wstring& ext) const;
	bool IsImage(const std::wstring& ext) const;
	bool IsPdf(const std::wstring& ext) const;
	bool IsAny(const std::wstring& ext) const;

	void AddDocument(const std::wstring& ext);
	void AddPresentation(const std::wstring& ext);
	void AddSpreadsheet(const std::wstring& ext);
	void AddCrossplatform(const std::wstring& ext);
	void AddDraw(const std::wstring& ext);
	void AddImage(const std::wstring& ext);

	std::vector<std::wstring> GetAllExts() const;

	// all supported exts
	static CFormatsList GetDefaultExts();

	// all writable exts
	static CFormatsList GetOutputExts();

	// default exts to extract
	static CFormatsList GetExtractExts();

private:
	std::vector<std::wstring> m_documents;
	std::vector<std::wstring> m_presentations;
	std::vector<std::wstring> m_spreadsheets;
	std::vector<std::wstring> m_crossplatform;
	std::vector<std::wstring> m_images;
	std::vector<std::wstring> m_draw;
	std::wstring m_pdf;
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
		unsigned long time;
		int inputSize;
		int outputSize;
		std::wstring exitCode;
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

	void AddDeleteLaterFile(const std::wstring& file);
	void AddDeleteLaterDirectory(const std::wstring& directory);

	bool IsAllBusy();
	bool IsAllFree();

	NSCriticalSection::CRITICAL_SECTION m_coresCS;
	NSCriticalSection::CRITICAL_SECTION m_reportCS;
	NSCriticalSection::CRITICAL_SECTION m_outputCS;

	int m_currentProc;
	int m_maxProc;

private:
	// parse string like "docx txt" into vector
	std::vector<std::wstring> ParseExtensionsString(std::wstring extensions, const CFormatsList& fl);
	void Convert(const std::vector<std::wstring>& files, bool bNoDirectory = false, bool bTrough = false);
	void Extract(const std::vector<std::wstring>& files);

	// takes from config
	std::wstring m_reportFile;
	std::wstring m_inputDirectory;
	std::wstring m_outputDirectory;
	std::wstring m_x2tPath;

	std::wstring m_errorsXmlDirectory;
	std::wstring m_troughConversionDirectory;
	std::wstring m_fontsDirectory;
	std::wstring m_tempDirectory;

	// fonts
	bool m_bIsUseSystemFonts;
	std::vector<std::wstring> m_arAdditionalFontsDirs;

	NSFile::CFileBinary m_reportStream;

	// takes from config or sets all
	std::vector<std::wstring> m_inputExts;
	std::vector<std::wstring> m_outputExts;

	std::vector<std::wstring> m_inputFiles;

	// lists
	CFormatsList m_inputFormatsList;
	CFormatsList m_outputFormatsList;
	CFormatsList m_extractFormatsList;

	bool m_bIsErrorsOnly;
	bool m_bIsTimestamp;
	bool m_bIsDeleteOk;
	bool m_bIsFilenameCsvTxtParams;
	bool m_bIsFilenamePassword;

	std::wstring m_defaultCsvTxtEndcoding;
	std::wstring m_defaultCsvDelimiter;


	unsigned long m_timeout;
	unsigned long m_timeStart;

	// format -> *t format -> all formats
	bool m_bTroughConversion;
	bool m_bSaveEnvironment;

	std::vector<std::wstring> m_deleteLaterFiles;
	std::vector<std::wstring> m_deleteLaterDirectories;

	// extract files with output_ext from input_files
	bool m_bExtract;

	// convert to docx before extract
	bool m_bConvertBeforeExtract;
};

// generates temp xml, convert, calls m_internal->writeReport
class CConverter : public NSThreads::CBaseThread
{
public:
	CConverter(Cx2tTester* internal);
	virtual ~CConverter();

	void SetInputFile(const std::wstring& inputFile);
	void SetInputExt(const std::wstring& inputExt);
	void SetOutputFilesDirectory(const std::wstring& outputFilesDirectory);
	void SetOutputExts(const std::vector<std::wstring>& outputExts);
	void SetFontsDirectory(const std::wstring& fontsDirectory);
	void SetX2tPath(const std::wstring& x2tPath);
	void SetErrorsOnly(bool bIsErrorsOnly);
	void SetDeleteOk(bool bIsDeleteOk);
	void SetTrough(bool bIsTrough);
	void SetXmlErrorsDirectory(const std::wstring& errorsXmlDirectory);
	void SetCsvTxtEncoding(int csvTxtEncoding);
	void SetCsvDelimiter(const std::wstring& csvDelimiter);
	void SetPassword(const std::wstring& password);
	void SetTimeout(unsigned long timeout);
	void SetFilesCount(int totalFiles, int currFile);
	void SetSaveEnvironment(bool bSaveEnvironment);

	virtual DWORD ThreadProc();

private:
	Cx2tTester* m_internal;
	std::wstring m_inputFile;

	std::wstring m_outputFilesDirectory;
	std::vector<std::wstring> m_outputExts;
	std::wstring m_inputExt;

	std::wstring m_fontsDirectory;
	COfficeFileFormatChecker m_checker;

	std::wstring m_x2tPath;
	std::wstring m_errorsXmlDirectory;

	int m_csvTxtEncoding;
	std::wstring m_csvDelimiter;
	std::wstring m_password;

	bool m_bIsErrorsOnly;
	bool m_bIsDeleteOk;
	bool m_bIsTrough;
	bool m_bSaveEnvironment;

	int m_totalFiles;
	int m_currFile;

	unsigned long m_timeout;
};

// extracts files from office files
class CExtractor : public NSThreads::CBaseThread
{
public:
	CExtractor(Cx2tTester* internal);
	virtual ~CExtractor();

	void SetInputFile(const std::wstring& inputFile);
	void SetOutputFilesDirectory(const std::wstring& outputFilesDirectory);
	void SetExtractExts(const std::vector<std::wstring>& extractExts);
	void SetFilesCount(int totalFiles, int currFile);

	virtual DWORD ThreadProc();

private:
	Cx2tTester* m_internal;
	std::wstring m_inputFile;
	std::wstring m_outputFilesDirectory;
	std::vector<std::wstring> m_extractExts;
	COfficeUtils m_utils;

	int m_totalFiles;
	int m_currFile;

};

#endif // X2T_TESTER_H
