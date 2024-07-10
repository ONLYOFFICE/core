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
		bool bStride = oSrc.stride() < 0;

		for (unsigned int unY = 0; unY < unSrcH; ++unY)
		{
			pDstBuffer = oSrc.row_ptr(bStride ? unSrcH - unY - 1: unY);
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
		bool bStride = oSrc.stride() < 0;

		for (unsigned int unY = 0; unY < unSrcH; ++unY)
		{
			pDstBuffer = oSrc.row_ptr(bStride ? unSrcH - unY - 1: unY);
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
		bool bStride = oSrc.stride() < 0;

		for (unsigned int unY = 0; unY < unSrcH; ++unY)
		{
			pDstBuffer = oSrc.row_ptr(bStride ? unSrcH - unY - 1: unY);
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
