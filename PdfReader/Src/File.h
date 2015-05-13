#ifndef _PDF_READER_FILE_H
#define _PDF_READER_FILE_H

#include <stdio.h>
#include <string>

#include "StringExt.h"
#include "../../DesktopEditor/common/File.h"

using namespace std;

namespace PdfReader
{
	static void SpitPathExt(std::wstring& wsFullPath, std::wstring* pwsFilePath, std::wstring* pwsExt)
	{
		// »щем '.' начина€ с конца пути, и раздел€ем путь на расширение и остальную часть
		unsigned int nPos = wsFullPath.find_last_of(L".");
		*pwsFilePath = wsFullPath.substr(0, nPos);
		*pwsExt      = wsFullPath.substr(nPos + 1);
	}
	static bool OpenTempFile(wstring* pwsName, FILE **ppFile, wchar_t *wsMode, wchar_t *wsExt, wchar_t *wsFolder, wchar_t *wsName = NULL)
	{
		return NSFile::CFileBinary::OpenTempFile(pwsName, ppFile, wsMode, wsExt, wsFolder, wsName);		
	}
	static char*GetLine(char *sBuffer, int nSize, FILE *pFile)
	{
		int nChar, nCurIndex = 0;

		while (nCurIndex < nSize - 1)
		{
			if ((nChar = fgetc(pFile)) == EOF)
				break;

			sBuffer[nCurIndex++] = (char)nChar;
			if ('\x0a' == nChar)
			{
				break;
			}
			if ('\x0d' == nChar)
			{
				nChar = fgetc(pFile);
				if ('\x0a' == nChar && nCurIndex < nSize - 1)
				{
					sBuffer[nCurIndex++] = (char)nChar;
				}
				else if (EOF != nChar)
				{
					ungetc(nChar, pFile);
				}
				break;
			}
		}
		sBuffer[nCurIndex] = '\0';
		if (0 == nCurIndex)
			return NULL;
		return sBuffer;
	}
}

#endif // _PDF_READER_FILE_H
