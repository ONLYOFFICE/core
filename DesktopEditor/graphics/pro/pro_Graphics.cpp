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

#include "../GraphicsRenderer.h"

namespace NSGraphics
{
    IGraphicsRenderer* Create()
    {
        return new CGraphicsRenderer();
    }

	std::string GetHatchBase64(const std::wstring& name,
							   unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1,
							   unsigned char r2, unsigned char g2, unsigned char b2, unsigned char a2)
	{
		agg::rgba8 c1 = agg::rgba8(r1, g1, b1, a1);
		agg::rgba8 c2 = agg::rgba8(r2, g2, b2, a2);

		BYTE* pPattern = new BYTE[HATCH_TX_SIZE * HATCH_TX_SIZE * 4];
		agg::GetHatchPattern(name, (agg::rgba8*)pPattern, c1, c2);

		CBgraFrame oFrame;
		oFrame.put_Data(pPattern);
		oFrame.put_Width(HATCH_TX_SIZE);
		oFrame.put_Height(HATCH_TX_SIZE);
		oFrame.put_Stride(4 * HATCH_TX_SIZE);

		BYTE* pPngBuffer = NULL;
		int nPngSize = 0;
		oFrame.Encode(pPngBuffer, nPngSize, 4);

		char* cData64 = NULL;
		int nData64Dst = 0;
		NSFile::CBase64Converter::Encode(pPngBuffer, nPngSize, cData64, nData64Dst, NSBase64::B64_BASE64_FLAG_NOCRLF);

		std::string sRet(cData64, (size_t)nData64Dst);
		RELEASEARRAYOBJECTS(cData64);

		return sRet;
	}
}
