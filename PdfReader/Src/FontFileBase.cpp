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
#include <stdio.h>
#include "MemoryUtils.h"
#include "FontFileBase.h"
#include "../../DesktopEditor/common/File.h"

namespace PdfReader
{
	//------------------------------------------------------------------------
	// CFontFileBase
	//------------------------------------------------------------------------

	CFontFileBase::CFontFileBase(char *sFile, int nLen, bool bFreeFileData)
	{
		m_sFileData = m_sFile = (unsigned char *)sFile;
		m_nLen = nLen;
		m_bFreeFileData = bFreeFileData;
	}

	CFontFileBase::~CFontFileBase()
	{
		if (m_bFreeFileData)
			MemUtilsFree(m_sFileData);
	}

	char *CFontFileBase::ReadFile(wchar_t *wsFileName, int *pnFileLen)
	{
		FILE* pFile = NSFile::CFileBinary::OpenFileNative(wsFileName, L"rb");
		if (!pFile)
			return NULL;

		fseek(pFile, 0, SEEK_END);
		int nLen = (int)ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		char *sBuffer = (char *)MemUtilsMalloc(nLen);
		if ((int)fread(sBuffer, 1, nLen, pFile) != nLen)
		{
			MemUtilsFree(sBuffer);
			fclose(pFile);
			return NULL;
		}

		fclose(pFile);
		*pnFileLen = nLen;
		return sBuffer;
	}

	int          CFontFileBase::GetS8(int nPos, bool *pbSuccess)
	{
		*pbSuccess = true;

		if (nPos < 0 || nPos >= m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		int nRes = m_sFile[nPos];
		if (nRes & 0x80)
			nRes |= ~0xff;
		return nRes;
	}

	int          CFontFileBase::GetU8(int nPos, bool *pbSuccess)
	{
		*pbSuccess = true;
		if (nPos < 0 || nPos >= m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		return m_sFile[nPos];
	}

	int          CFontFileBase::GetS16BE(int nPos, bool *pbSuccess)
	{
		*pbSuccess = true;

		if (nPos < 0 || nPos + 1 >= m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		int nRes = m_sFile[nPos];
		nRes = (nRes << 8) + m_sFile[nPos + 1];
		if (nRes & 0x8000)
			nRes |= ~0xffff;
		return nRes;
	}

	int          CFontFileBase::GetU16BE(int nPos, bool *pbSuccess)
	{
		*pbSuccess = true;

		if (nPos < 0 || nPos + 1 >= m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		int nRes = m_sFile[nPos];
		nRes = (nRes << 8) + m_sFile[nPos + 1];
		return nRes;
	}

	int          CFontFileBase::GetS32BE(int nPos, bool *pbSuccess)
	{
		*pbSuccess = true;

		if (nPos < 0 || nPos + 3 >= m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		int nRes = m_sFile[nPos];
		nRes = (nRes << 8) + m_sFile[nPos + 1];
		nRes = (nRes << 8) + m_sFile[nPos + 2];
		nRes = (nRes << 8) + m_sFile[nPos + 3];
		if (nRes & 0x80000000)
			nRes |= ~0xffffffff;

		return nRes;
	}

	unsigned int CFontFileBase::GetU32BE(int nPos, bool *pbSuccess)
	{
		*pbSuccess = true;

		if (nPos < 0 || nPos + 3 >= m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		unsigned int nRes = m_sFile[nPos];
		nRes = (nRes << 8) + m_sFile[nPos + 1];
		nRes = (nRes << 8) + m_sFile[nPos + 2];
		nRes = (nRes << 8) + m_sFile[nPos + 3];
		return nRes;
	}

	unsigned int CFontFileBase::GetUVarBE(int nPos, int nSize, bool *pbSuccess)
	{
		*pbSuccess = true;

		if (nPos < 0 || nPos + nSize > m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		unsigned int nRes = 0;
		for (int nIndex = 0; nIndex < nSize; ++nIndex)
			nRes = (nRes << 8) + m_sFile[nPos + nIndex];

		return nRes;
	}

	bool CFontFileBase::CheckRegion(int nPos, int nSize)
	{
		return (nPos >= 0 && nPos + nSize >= nPos && nPos + nSize <= m_nLen);
	}
}
