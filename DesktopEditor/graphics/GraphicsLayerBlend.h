/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

		for (unsigned int unY = 0; unY < unSrcH; ++unY)
		{
			pDstBuffer = oSrc.row_ptr(unY);
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

		for (unsigned int unY = 0; unY < unSrcH; ++unY)
		{
			pDstBuffer = oSrc.row_ptr(unY);
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
