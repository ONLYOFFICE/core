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
#ifndef _BUILD_BYTE_BUILDER_CROSSPLATFORM_H_
#define _BUILD_BYTE_BUILDER_CROSSPLATFORM_H_

#include <string>
#include <string.h>
#include <algorithm>
#include "Types.h"

#include "../../Common/kernel_config.h"

namespace NSMemoryUtils
{
    class KERNEL_DECL CByteBuilder
	{
	private:
        BYTE*		m_pData;
		size_t		m_lSize;

        BYTE*		m_pDataCur;
		size_t		m_lSizeCur;

	public:
        CByteBuilder();
        ~CByteBuilder();
        void AddSize(size_t nSize);

	public:
        void WriteString(const std::string& sText);
        void WriteString(const std::wstring& sText);
        void WriteStringUTF8(const std::wstring& sText);

        void WriteInt(const int& value);

        size_t GetCurSize();
        void SetCurSize(size_t lCurSize);
        size_t GetSize();

        void Clear();
        void ClearNoAttack();

        BYTE* GetData();
	};

    class KERNEL_DECL CByteReader
    {
    private:
        BYTE*		m_pData;
        BYTE*		m_pDataCur;

    public:
        CByteReader(BYTE* pData);
        ~CByteReader();

    public:
        std::string GetString();
        std::wstring GetStringW();
        std::wstring GetStringUTF8();

        int GetInt();
    };
}

#endif // _BUILD_BYTE_BUILDER_CROSSPLATFORM_H_
