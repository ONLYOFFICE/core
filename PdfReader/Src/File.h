/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
		// Ищем '.' начиная с конца пути, и разделяем путь на расширение и остальную часть
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
