#include "md2html.h"

#include "md4c/src/md4c-html.h"
#include "../../../DesktopEditor/common/File.h"

#include <map>
#include <sstream>

namespace Md
{
#define MD_PARSER_FLAGS MD_DIALECT_GITHUB | MD_FLAG_NOINDENTEDCODEBLOCKS | MD_HTML_FLAG_SKIP_UTF8_BOM | MD_FLAG_HARD_SOFT_BREAKS | MD_HTML_FLAG_XHTML | MD_FLAG_LATEXMATHSPANS
#define MD_RENDERER_FLAGS MD_HTML_FLAG_XHTML

void ToHtml(const MD_CHAR* pValue, MD_SIZE uSize, void* pData)
{
	if (NULL != pData)
		(*(std::string*)pData).append(pValue, uSize);
}

std::string ConvertMdStringToHtml(const std::string& sMdString)
{
	std::string sData;
	md_html(sMdString.c_str(), sMdString.length(), ToHtml, &sData, MD_PARSER_FLAGS, MD_RENDERER_FLAGS);
	return sData;
}

void ToHtmlFile(const MD_CHAR* pValue, MD_SIZE uSize, void* pData)
{
	if (NULL != pData)
		((NSFile::CFileBinary*)pData)->WriteFile(pValue, uSize);
}

void WriteBaseHtmlStyles(NSFile::CFileBinary& oFile)
{
	oFile.WriteStringUTF8(L"<style>");

	// Main styles
	oFile.WriteStringUTF8(L"* { font-family: Arial; color:black; white-space:pre; }");
	oFile.WriteStringUTF8(L"p { margin: 0 0 10px; display: block; }");
	oFile.WriteStringUTF8(L"a { color: #0553c1; text-decoration: underline; } a:visited { color: #954f72; text-decoration: underline; }");
	oFile.WriteStringUTF8(L"ul { margin-top: 0; margin-bottom: 10px; }");
	oFile.WriteStringUTF8(L"img { vertical-align: middle; }");

	// Styles for tables
	oFile.WriteStringUTF8(L"table { margin-bottom: 20px; width: 100%; max-width: 100%; border-spacing:0; border-collapse: collapse; border-color: gray; vertical-align:middle;}");
	oFile.WriteStringUTF8(L"thead { display: table-header-group;}");
	oFile.WriteStringUTF8(L"tr { display: table-row; }");
	oFile.WriteStringUTF8(L"th { text-align: center; display: table-cell; font-weight: bold; }");

	oFile.WriteStringUTF8(L"table thead tr th, table thead tr td { border-bottom: 2px solid #ddd; border-top: none; }");
	oFile.WriteStringUTF8(L"table tbody tr th, table tbody tr td { padding 8px; line-height: 1.4; border-top: 1px solid #ddd; }");

	// Styles for blockquote
	oFile.WriteStringUTF8(L"blockquote { border-left: 3px solid #e9e9e9;  margin: 1.5em 0; padding: 0.5em 10px 0.5em 24px; font-size: 1.25rem; display: block; margin-top: 8pt; font-style: italic; color: #404040; }");

	// Styles for code
	oFile.WriteStringUTF8(L"code { padding: 2px 4px; font-size: 90%; color: #c7254e; background-color: #f9f2f4; }");
	oFile.WriteStringUTF8(L"pre code { padding: 0px; white-space: pre-wrap; border-radius: 0; background-color: #f8f8f8; color:black; }");
	oFile.WriteStringUTF8(L"pre { display: block; padding: 9.5px; margin: 0 0 10px; line-height: 1.4; word-break: break-all; word-wrap: break-word; background-color: #f8f8f8; border: none; font-size: 1em; }");
	oFile.WriteStringUTF8(L"code, pre { font-family: Menlo, Monaco, Consolas, \"Courier New\", monospace; }");

	// Styles for headings
	oFile.WriteStringUTF8(L"h1 { font-size: 20pt; color: #0f4761; margin-top: 18pt; margin-bottom: 4pt; }");
	oFile.WriteStringUTF8(L"h2 { font-size: 16pt; color: #0f4761; margin-top: 8pt; margin-bottom: 4pt; }");
	oFile.WriteStringUTF8(L"h3 { font-size: 14pt; color: #0f4761; margin-top: 8pt; margin-bottom: 4pt; }");
	oFile.WriteStringUTF8(L"h4 { font-style: italic; color: #0f4761; margin-top: 4pt; margin-bottom: 2pt; }");
	oFile.WriteStringUTF8(L"h5 { color: #0f4761; margin-top: 4pt; margin-bottom: 2pt; }");
	oFile.WriteStringUTF8(L"h6 { font-style: italic; color: #595959; margin-top: 2pt; margin-bottom: 0; }");

	oFile.WriteStringUTF8(L"</style>");
}

static std::string Trim(const std::string& sValue)
{
	const size_t unStart{sValue.find_first_not_of(" \t")};

	if (std::string::npos == unStart)
		return std::string();

	return sValue.substr(unStart, sValue.find_last_not_of(" \t") - unStart + 1);
}

static bool IsFrontMatterDelimiter(const std::string& line)
{
	return "---" == Trim(line);
}

std::map<std::string, std::string> ExtractMetadata(std::string& sMdData)
{
	const size_t unFirstLineEnd{sMdData.find('\n')};

	std::string sFirstLine;

	if (std::string::npos == unFirstLineEnd)
		sFirstLine = sMdData;
	else
	{
		size_t unLen{unFirstLineEnd};

		if (unLen > 0 && sMdData[unLen - 1] == '\r')
			--unLen;

		sFirstLine = sMdData.substr(0, unLen);
	}

	if (!IsFrontMatterDelimiter(sFirstLine))
		return {};

	size_t unMetaStart{(std::string::npos == unFirstLineEnd) ? sMdData.size() : unFirstLineEnd + 1};
    size_t unSearchPos{unMetaStart};
	size_t unCloseDelimStart{std::string::npos};
	size_t unCloseDelimLineEnd{std::string::npos};

	while (unSearchPos < sMdData.size())
	{
		size_t unLineEnd{sMdData.find('\n', unSearchPos)};
		std::string sLine;

		if (unLineEnd == std::string::npos)
		{
			sLine = sMdData.substr(unSearchPos);

			if (IsFrontMatterDelimiter(sLine))
			{
				unCloseDelimStart = unSearchPos;
				unCloseDelimLineEnd = sMdData.size();
			}

			break;
		}
		else
		{
			size_t unLen{unLineEnd - unSearchPos};

			if (unLen > 0 && sMdData[unLineEnd - 1] == '\r')
				--unLen;

			sLine = sMdData.substr(unSearchPos, unLen);

			if (IsFrontMatterDelimiter(sLine))
			{
				unCloseDelimStart = unSearchPos;
				unCloseDelimLineEnd = unLineEnd + 1;
				break;
			}

			unSearchPos = unLineEnd + 1;
		}
	}

	if (unCloseDelimStart == std::string::npos)
		return {};

	const size_t unMetaLength = unCloseDelimStart - unMetaStart;
	const std::string sMetaBlock{sMdData.substr(unMetaStart, unMetaLength)};

	std::istringstream oStream(sMetaBlock);
	std::string sMetaLine;

	std::map<std::string, std::string> mMetadata;

	while (std::getline(oStream, sMetaLine, '\n'))
	{
		if (!sMetaLine.empty() && sMetaLine.back() == '\r')
			sMetaLine.pop_back();

		const std::string sTrimmed{Trim(sMetaLine)};

		if (sTrimmed.empty())
			continue;

		const size_t unColon{sTrimmed.find(':')};

		if (std::string::npos == unColon)
			continue;

		mMetadata[Trim(sTrimmed.substr(0, unColon))] = Trim(sTrimmed.substr(unColon + 1));
	}

	sMdData.erase(0, unCloseDelimLineEnd);

	return mMetadata;
}

bool ConvertMdFileToHtml(const std::wstring& wsPathToMdFile, const std::wstring& wsPathToHtmlFile)
{
	std::string sMdData;

	if (!NSFile::CFileBinary::ReadAllTextUtf8A(wsPathToMdFile, sMdData))
		return false;

	NSFile::CFileBinary oFile;

	if (!oFile.CreateFile(wsPathToHtmlFile))
		return false;

	oFile.WriteStringUTF8(L"<html><body>");

	oFile.WriteStringUTF8(L"<head>");

	for (const std::pair<std::string, std::string>& oMeta : ExtractMetadata(sMdData))
	{
		if (oMeta.first.empty())
			continue;

		oFile.WriteStringUTF8(L"<meta name=\"" + UTF8_TO_U(oMeta.first) + L"\" content=\"" + UTF8_TO_U(oMeta.second) + L"\"/>");
	}

	oFile.WriteStringUTF8(L"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>");
	WriteBaseHtmlStyles(oFile);
	oFile.WriteStringUTF8(L"</head>");

	bool bResult = true;

	if (0 != md_html(sMdData.c_str(), sMdData.length(), ToHtmlFile, &oFile, MD_PARSER_FLAGS, MD_RENDERER_FLAGS))
		bResult = false;

	oFile.WriteStringUTF8(L"</body></html>");

	oFile.CloseFile();

	if (!bResult)
		NSFile::CFileBinary::Remove(wsPathToHtmlFile);

	return bResult;
}
}

