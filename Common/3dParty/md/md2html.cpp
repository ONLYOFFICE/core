#include "md2html.h"

#include "md4c/src/md4c-html.h"
#include "../../../DesktopEditor/common/File.h"

namespace Md
{
#define MD_PARSER_FLAGS MD_DIALECT_GITHUB | MD_FLAG_NOINDENTEDCODEBLOCKS | MD_HTML_FLAG_SKIP_UTF8_BOM | MD_FLAG_HARD_SOFT_BREAKS | MD_HTML_FLAG_XHTML
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
	//oFile.WriteStringUTF8(L"<meta charset=\"UTF-8\">");
	oFile.WriteStringUTF8(L"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
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

