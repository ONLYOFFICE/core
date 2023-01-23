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
