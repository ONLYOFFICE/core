/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "CalculatorCRC32.h"

CCalculatorCRC32::CCalculatorCRC32()
{
	m_dwMagicWord = 0xEDB88320;
	m_dwInitCrc = 0xFFFFFFFF;
	m_bInitTable = false;
}
DWORD CCalculatorCRC32::Calc(BYTE const*pStream, int nSize)
{
	InitCRCTable();
	DWORD dwRes = m_dwInitCrc;
	for (int i=0;i<nSize;i++)
	{
		dwRes = m_arCRCTable[(dwRes ^ pStream[i])& 0xFF] ^ (dwRes >> 8);
	}

	dwRes = dwRes ^ 0xFFFFFFFF;
	return dwRes;
}
DWORD CCalculatorCRC32::Calc(const std::wstring &sStream)
{
	InitCRCTable();
	DWORD dwRes = m_dwInitCrc;

	for (size_t i=0; i < sStream.length(); i++)
	{
		dwRes = m_arCRCTable[(dwRes ^ (BYTE)sStream[i]) & 0xFF] ^ (dwRes >> 8);
	}

	dwRes = dwRes ^ 0xFFFFFFFF;
	return dwRes;
}
DWORD CCalculatorCRC32::CalcPartFile(const std::wstring &sFilepath)
{
	DWORD dwRet = 0xFFFFFFFF;
	//LPBYTE pBuffer = new BYTE[g_clFilePartSize];
	//if (NULL==pBuffer)
	//	return dwRet;
	//FILE *pFile = fopen(sFilepath, "rb");
	//if (NULL==pFile)
	//{
	//	delete [] pBuffer;
	//	return dwRet;
	//}

	//size_t nReaded = fread(pBuffer, 1, 1024, pFile);
	//fclose(pFile);

	//dwRet = CCalculatorCRC32::Calc(pBuffer, nReaded);
	//
	//delete [] pBuffer;
	return dwRet;
}
void CCalculatorCRC32::InitCRCTable()
{
	if (m_bInitTable)
		return;

	DWORD dwTemp;
	for (int i = 0; i < 256; i++)
	{
		dwTemp = i;
		for (int j = 0;j < 8;j++)
		{
			if (0x1==(dwTemp & 0x1))
				dwTemp = (dwTemp >> 1) ^ m_dwMagicWord;
			else
				dwTemp = dwTemp >> 1;
		}
		m_arCRCTable[i] = dwTemp;
	}
}
