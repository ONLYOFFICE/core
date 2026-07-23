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
#pragma once
#include "../../Common/3dParty/pole/pole.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/xml/include/xmlwriter.h"

namespace StreamUtils
{
	std::string md5(const BYTE* pData, const ULONG dataLen);

	BYTE ReadBYTE(POLE::Stream* pStream);
	WORD ReadWORD(POLE::Stream* pStream);
	DWORD ReadDWORD(POLE::Stream* pStream);
	SHORT ReadSHORT(POLE::Stream* pStream);
	LONG ReadLONG(POLE::Stream* pStream);
	FLOAT ReadFLOAT(POLE::Stream* pStream);

	std::string ReadStringA(POLE::Stream* pStream, LONG lLen);
	std::wstring ReadStringW(POLE::Stream* pStream, LONG lLen);

	void StreamSeek(long lOffset, POLE::Stream* pStream);
	void StreamPosition(long& lPosition, POLE::Stream* pStream);
	void StreamSkip(long lCount, POLE::Stream* pStream);
	void StreamSkipBack(long lCount, POLE::Stream* pStream);
}

namespace CDirectory
{
	void SaveToFile(const std::wstring& strFileName, const std::wstring& strXml);
	bool DeleteFile(const std::wstring& strFileName);
	bool CopyFile(const std::wstring& strExists, const std::wstring& strNew);

	void WriteValueToNode(std::wstring strName, DWORD value, XmlUtils::CXmlWriter* pWriter);
	void WriteValueToNode(std::wstring strName, LONG value, XmlUtils::CXmlWriter* pWriter);
	void WriteValueToNode(std::wstring strName, std::wstring value, XmlUtils::CXmlWriter* pWriter);
	void WriteValueToNode(std::wstring strName, WCHAR value, XmlUtils::CXmlWriter* pWriter);
	void WriteValueToNode(std::wstring strName, bool value, XmlUtils::CXmlWriter* pWriter);

	double FixedPointToDouble(DWORD point);
	LONG NormFixedPoint(DWORD point, LONG base);

	std::wstring BYTEArrayToString(BYTE* arr, size_t nCount);
	std::wstring BYTEArrayToStringW(BYTE* arr, size_t nCount);
}
