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

#ifndef CGRAPHICSLAYER_BLEND_H
#define CGRAPHICSLAYER_BLEND_H

#include "GraphicsLayer.h"

namespace Aggplus
{
	template <class SrcPixelFormatRenderer>
	void BlendTo(CGraphicsLayer* pLayer, SrcPixelFormatRenderer& oSrc)
	{
		if (NULL == pLayer->GetBuffer() || 0 == oSrc.width() || 0 == oSrc.height())
			return;

		typedef typename SrcPixelFormatRenderer::order_type order_type;
		typedef typename SrcPixelFormatRenderer::value_type value_type;

		int nStep = 4;
		BYTE* pSrcBuffer = pLayer->GetBuffer();
		value_type* pDstBuffer = NULL;
		BYTE uchAlpha;

		unsigned int unSrcW = oSrc.width();
		unsigned int unSrcH = oSrc.height();

		BYTE nOpacity = pLayer->GetSettings().m_uchOpacity;
		bool bFlip = oSrc.stride() < 0;

		for (unsigned int unY = 0; unY < unSrcH; ++unY)
		{
			pDstBuffer = oSrc.row_ptr(bFlip ? unSrcH - 1 - unY : unY);
			for (unsigned int unX = 0; unX < unSrcW; ++unX)
			{
				uchAlpha = (SrcPixelFormatRenderer::base_mask + nOpacity * pSrcBuffer[order_type::A]) >> 8;
				if (uchAlpha)
				{
					if(uchAlpha == SrcPixelFormatRenderer::base_mask)
					{
						pDstBuffer[order_type::R] = pSrcBuffer[order_type::R];
						pDstBuffer[order_type::G] = pSrcBuffer[order_type::G];
						pDstBuffer[order_type::B] = pSrcBuffer[order_type::B];
						pDstBuffer[order_type::A] = SrcPixelFormatRenderer::base_mask;
					}
					else
					{
						SrcPixelFormatRenderer::blender_type::blend_pix(pDstBuffer, pSrcBuffer[order_type::R], pSrcBuffer[order_type::G], pSrcBuffer[order_type::B], uchAlpha);
					}
				}

				pSrcBuffer += nStep;
				pDstBuffer += nStep;
			}
		}
	}

	template <class SrcPixelFormatRenderer>
	void BlendTo(CGraphicsLayer* pLayer, SrcPixelFormatRenderer& oSrc, int nBlendMode)
	{
		if (NULL == pLayer->GetBuffer() || 0 == oSrc.width() || 0 == oSrc.height())
			return;

		typedef typename SrcPixelFormatRenderer::order_type order_type;
		typedef typename SrcPixelFormatRenderer::value_type value_type;

		int nStep = 4;
		BYTE* pSrcBuffer = pLayer->GetBuffer();
		value_type* pDstBuffer = NULL;
		BYTE uchAlpha;

		unsigned int unSrcW = oSrc.width();
		unsigned int unSrcH = oSrc.height();

		BYTE nOpacity = pLayer->GetSettings().m_uchOpacity;
		bool bFlip = oSrc.stride() < 0;

		for (unsigned int unY = 0; unY < unSrcH; ++unY)
		{
			pDstBuffer = oSrc.row_ptr(bFlip ? unSrcH - 1 - unY : unY);
			for (unsigned int unX = 0; unX < unSrcW; ++unX)
			{
				uchAlpha = (SrcPixelFormatRenderer::base_mask + nOpacity * pSrcBuffer[order_type::A]) >> 8;
				if (uchAlpha)
				{
					SrcPixelFormatRenderer::blender_type::blend_pix(nBlendMode, pDstBuffer, pSrcBuffer[order_type::R], pSrcBuffer[order_type::G], pSrcBuffer[order_type::B], uchAlpha, 255);
				}

				pSrcBuffer += nStep;
				pDstBuffer += nStep;
			}
		}
	}

	template <class AlphaMaskFunction, class SrcPixelFormatRenderer>
	void BlendTo(CGraphicsLayer* pLayer, SrcPixelFormatRenderer& oSrc, BYTE* pAlphaMaskBuffer, UINT unAlphaMaskStep)
	{
		if (NULL == pLayer->GetBuffer() || 0 == oSrc.width() || 0 == oSrc.height())
			return;

		typedef typename SrcPixelFormatRenderer::order_type order_type;
		typedef typename SrcPixelFormatRenderer::value_type value_type;

		int nStep = 4;
		BYTE* pSrcBuffer = pLayer->GetBuffer();
		value_type* pDstBuffer = NULL;
		BYTE* pSrcAlphaMaskBuffer = pAlphaMaskBuffer;
		BYTE uchAlpha;

		unsigned int unSrcW = oSrc.width();
		unsigned int unSrcH = oSrc.height();

		BYTE nOpacity = pLayer->GetSettings().m_uchOpacity;
		bool bFlip = oSrc.stride() < 0;

		for (unsigned int unY = 0; unY < unSrcH; ++unY)
		{
			pDstBuffer = oSrc.row_ptr(bFlip ? unSrcH - 1 - unY : unY);
			for (unsigned int unX = 0; unX < unSrcW; ++unX)
			{
				uchAlpha = ((SrcPixelFormatRenderer::base_mask + nOpacity * pSrcBuffer[order_type::A] * AlphaMaskFunction::calculate(pSrcAlphaMaskBuffer)) >> 16);

				if(uchAlpha == SrcPixelFormatRenderer::base_mask)
				{
					pDstBuffer[order_type::R] = pSrcBuffer[order_type::R];
					pDstBuffer[order_type::G] = pSrcBuffer[order_type::G];
					pDstBuffer[order_type::B] = pSrcBuffer[order_type::B];
					pDstBuffer[order_type::A] = SrcPixelFormatRenderer::base_mask;
				}
				else
				{
					SrcPixelFormatRenderer::blender_type::blend_pix(pDstBuffer, pSrcBuffer[order_type::R], pSrcBuffer[order_type::G], pSrcBuffer[order_type::B], uchAlpha);
				}

				pSrcBuffer          += nStep;
				pDstBuffer          += nStep;
				pSrcAlphaMaskBuffer += unAlphaMaskStep;
			}
		}
	}
}

#endif // CGRAPHICSLAYER_BLEND_H
