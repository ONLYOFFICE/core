#ifndef X2T_TESTER_H
#define X2T_TESTER_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>

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


class Cx2tTester
{
public:
	Cx2tTester(std::wstring configPath);
	~Cx2tTester();

	void Start();

private:
	struct Report
	{
		std::wstring inputFile;
		std::wstring outputFile;
		std::wstring inputExt;
		std::wstring outputExt;
		int exitCode;
	};

	void setConfig(const std::wstring& configPath);
	void setReportHeader();

	void writeReport(const Report& report);
	int convert();

	std::wstring m_reportPath;
	std::wstring m_inputFolder;
	std::wstring m_outputFolder;
	std::wstring m_x2tPath;

	// fonts
	bool m_bIsUseSystemFonts;
	std::vector<std::wstring> m_arAdditionalFontsDirs;

	std::wstring m_xmlParams;

	std::wofstream m_reportStream;

	// from 1 to N formats
	std::map<int, std::vector<int>> m_formats;

};

#endif // X2T_TESTER_H
