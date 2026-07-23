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

#ifndef HWPSTREAM_H
#define HWPSTREAM_H

#include <stack>
#include "Common/Common.h"

namespace HWP
{
enum class EStringCharacter
{
	ASCII,
	UTF16,
	UTF32
};

class CHWPStream
{
	HWP_BYTE* m_pBegin;
	HWP_BYTE* m_pCur;
	HWP_BYTE* m_pEnd;
	std::stack<HWP_BYTE*> m_arSavedPositions;

	bool m_bExternalBuffer;
public:
	CHWPStream();
	CHWPStream(unsigned long ulSize);
	CHWPStream(HWP_BYTE* pBuffer, unsigned long ulSize, bool bExternalBuffer = true);
	~CHWPStream();

	void Clear();
	void Copy(CHWPStream& oStream, unsigned long ulSize);

	void SetStream(HWP_BYTE* pBuffer, unsigned long ulSize, bool bExternalBuffer = true);

	HWP_BYTE* GetCurPtr();
	const HWP_BYTE* GetCurPtr() const;
	unsigned long Tell() const;
	unsigned long SizeToEnd() const;

	void Expand(unsigned long ulSize);

	bool ReadChar(HWP_CHAR& chValue);
	bool ReadFloat(float& fValue);
	bool ReadDouble(double& dValue);
	bool ReadLong(long long& lValue);
	bool ReadInt(int& nValue);
	bool ReadColor(int& nValue);
	bool ReadShort(short& shValue);
	short ReadShort();
	bool ReadByte(HWP_BYTE& chValue);
	HWP_BYTE ReadByte();
	bool ReadString(HWP_STRING& sValue, EStringCharacter eCharacter);
	bool ReadString(HWP_STRING& sValue, int nLength, EStringCharacter eCharacter);
	unsigned long ReadBytes(HWP_BYTE* pBytes, unsigned long unSize);

	void Skip(int nStep);
	void MoveToStart();
	void MoveTo(unsigned int unPosition);

	bool CanRead(int nSize = 1) const;
	bool IsValid() const;
	bool IsEof() const;
	unsigned int GetSize() const;

	void SavePosition();
	void RemoveLastSavedPos();
	int GetDistanceToLastPos(bool bRemoveLastPos = false);

	HWP_BYTE operator[](unsigned int unPosition) const;

	bool WriteBytes(const HWP_BYTE* pBuffer, unsigned long ulSize);
};

#define CHECK_FLAG(value, flag) ((value) & flag) == flag
}

#endif // HWPSTREAM_H
