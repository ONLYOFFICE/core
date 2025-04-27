#include "md2html.h"

#include "md4c/src/md4c-html.h"
#include "../../../DesktopEditor/common/File.h"

void ToHtml(const MD_CHAR* pValue, MD_SIZE uSize, void* pData)
{
	if (NULL != pData)
		(*(std::string*)pData).append(pValue, uSize);
}

std::string ConvertMdStringToHtml(const std::string& sMdString)
{
	std::string sData;
	md_html(sMdString.c_str(), sMdString.length(), ToHtml, &sData, 0, 0);
	return sData;
}

void ToHtmlFile(const MD_CHAR* pValue, MD_SIZE uSize, void* pData)
{
	if (NULL != pData)
		((NSFile::CFileBinary*)pData)->WriteFile(pValue, uSize);
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

	bool bResult = true;

	if (0 != md_html(sMdData.c_str(), sMdData.length(), ToHtmlFile, &oFile, MD_DIALECT_GITHUB, 0))
		bResult = false;

	oFile.WriteStringUTF8(L"</body></html>");

	oFile.CloseFile();

	if (!bResult)
		NSFile::CFileBinary::Remove(wsPathToHtmlFile);

	return bResult;
}

