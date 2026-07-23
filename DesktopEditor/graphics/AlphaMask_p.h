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

#ifndef _BUILD_ALPHAMASK_P_H_
#define _BUILD_ALPHAMASK_P_H_

#include "AlphaMask.h"
#include <string>
#include <string.h>

#include "../agg-2.4/include/agg_rendering_buffer.h"
#include "../agg-2.4/include/agg_scanline_u.h"
#include "../agg-2.4/include/agg_alpha_mask_u8.h"

namespace Aggplus
{
	class CSoftMask_private
	{
	public:
		virtual ~CSoftMask_private()
		{
			BYTE* pBuffer = m_oRenderingBuffer.buf();
			if (NULL != pBuffer)
			{
				if (!m_bExternalBuffer)
					RELEASEARRAYOBJECTS(pBuffer);

				m_oRenderingBuffer.attach(NULL, 0, 0, 0);
			}
		}

		unsigned int GetStep() const { return 4; }
		unsigned int GetWidth() const { return m_unWidth; }
		unsigned int GetHeight() const { return m_unHeight; }
		BYTE* GetBuffer() { return m_oRenderingBuffer.buf(); }

		virtual ESoftMaskType GetDataType() const = 0;
		virtual bool GetSwapRGB() const { return true; };

	protected:
		CSoftMask_private(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip)
		{
			m_bExternalBuffer = bExternalBuffer;
			m_unWidth         = unWidth;
			m_unHeight        = unHeight;
			m_oRenderingBuffer.attach(pBuffer, unWidth, unHeight, (bFlip ? -1 : 1) * GetStep() * unWidth);
		}

		agg::rendering_buffer m_oRenderingBuffer;
		bool                  m_bExternalBuffer;
		unsigned int          m_unWidth;
		unsigned int          m_unHeight;
	};

	class CSoftMaskBGRAgray : public CSoftMask_private
	{
	public:
		CSoftMaskBGRAgray(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip)
			: CSoftMask_private(pBuffer, unWidth, unHeight, bExternalBuffer, bFlip), m_oAlphaMask(m_oRenderingBuffer), m_oScanLine(m_oAlphaMask) {}

		agg::scanline_u8_am<agg::alpha_mask_bgra32gray>& GetScanline() { return m_oScanLine; }
		virtual ESoftMaskType GetDataType() const override { return ESoftMaskType::BGRGrayBuffer; }

	private:
		agg::alpha_mask_bgra32gray                      m_oAlphaMask;
		agg::scanline_u8_am<agg::alpha_mask_bgra32gray> m_oScanLine;
	};

	class CSoftMaskRGBAgray : public CSoftMask_private
	{
	public:
		CSoftMaskRGBAgray(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip)
			: CSoftMask_private(pBuffer, unWidth, unHeight, bExternalBuffer, bFlip), m_oAlphaMask(m_oRenderingBuffer), m_oScanLine(m_oAlphaMask) {}

		agg::scanline_u8_am<agg::alpha_mask_rgba32gray>& GetScanline() { return m_oScanLine; }
		virtual bool GetSwapRGB() const override { return false; };
		virtual ESoftMaskType GetDataType() const override { return ESoftMaskType::RGBGrayBuffer; }

	private:
		agg::alpha_mask_rgba32gray                      m_oAlphaMask;
		agg::scanline_u8_am<agg::alpha_mask_rgba32gray> m_oScanLine;
	};

	class CSoftMaskAlpha : public CSoftMask_private
	{
	public:
		CSoftMaskAlpha(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip)
			: CSoftMask_private(pBuffer, unWidth, unHeight, bExternalBuffer, bFlip), m_oAlphaMask(m_oRenderingBuffer), m_oScanLine(m_oAlphaMask) {}

		agg::scanline_u8_am<agg::alpha_mask_rgba32a>& GetScanline() { return m_oScanLine; }
		virtual ESoftMaskType GetDataType() const override { return ESoftMaskType::Alpha4Buffer; }

	private:
		agg::alpha_mask_rgba32a                      m_oAlphaMask;
		agg::scanline_u8_am<agg::alpha_mask_rgba32a> m_oScanLine;
	};
}

#endif // _BUILD_ALPHAMASK_P_H_
