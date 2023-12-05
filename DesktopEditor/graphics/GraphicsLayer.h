#ifndef CGRAPHICSLAYER_H
#define CGRAPHICSLAYER_H

#include "Defines.h"
#include "./config.h"
#include "../common/IGrObject.h"

#include <iostream>

namespace Aggplus
{
	struct TGraphicsLayerSettings
	{
		BYTE m_uchOpacity;
	};

	class GRAPHICS_DECL CGraphicsLayer : public IGrObject
	{
	public:
		CGraphicsLayer(BYTE* pBuffer, bool bExternalBuffer = true);
		~CGraphicsLayer();

		bool Empty() const;
		BYTE* GetBuffer();

		void SetDefaultSettings();
		void ClearBuffer(bool bDeleteData = true);

		void SetSettings(const TGraphicsLayerSettings& oSettings);
		const TGraphicsLayerSettings& GetSettings() const;

		void SetOpacity(double dOpacity);
		void SetOpacity(BYTE uchOpacity);

		template <class SrcPixelFormatRenderer>
		void BlendTo(SrcPixelFormatRenderer& oSrc)
		{
			if (NULL == m_pBuffer || 0 == oSrc.width() || 0 == oSrc.height())
				return;

			typedef typename SrcPixelFormatRenderer::order_type order_type;
			typedef typename SrcPixelFormatRenderer::value_type value_type;

			int nStep = 4;
			BYTE* pSrcBuffer = m_pBuffer;
			value_type* pDstBuffer = NULL;
			BYTE uchAlpha;

			unsigned int unSrcW = oSrc.width();
			unsigned int unSrcH = oSrc.height();

			for (unsigned int unY = 0; unY < unSrcH; ++unY)
			{
				pDstBuffer = oSrc.row_ptr(unY);
				for (unsigned int unX = 0; unX < unSrcW; ++unX)
				{
					uchAlpha = (SrcPixelFormatRenderer::base_mask + m_oSettings.m_uchOpacity * pSrcBuffer[order_type::A]) >> 8;
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
		void BlendTo(SrcPixelFormatRenderer& oSrc, BYTE* pAlphaMaskBuffer, UINT unAlphaMaskStep)
		{
			if (NULL == m_pBuffer || 0 == oSrc.width() || 0 == oSrc.height())
				return;

			typedef typename SrcPixelFormatRenderer::order_type order_type;
			typedef typename SrcPixelFormatRenderer::value_type value_type;

			int nStep = 4;
			BYTE* pSrcBuffer = m_pBuffer;
			value_type* pDstBuffer = NULL;
			BYTE* pSrcAlphaMaskBuffer = pAlphaMaskBuffer;
			BYTE uchAlpha;

			unsigned int unSrcW = oSrc.width();
			unsigned int unSrcH = oSrc.height();

			for (unsigned int unY = 0; unY < unSrcH; ++unY)
			{
				pDstBuffer = oSrc.row_ptr(unY);
				for (unsigned int unX = 0; unX < unSrcW; ++unX)
				{
					uchAlpha = ((SrcPixelFormatRenderer::base_mask + m_oSettings.m_uchOpacity * pSrcBuffer[order_type::A] * AlphaMaskFunction::calculate(pSrcAlphaMaskBuffer)) >> 16);

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
	private:
		BYTE*                  m_pBuffer;
		bool                   m_bExternalBuffer;

		TGraphicsLayerSettings m_oSettings;
	};

}

#endif // CGRAPHICSLAYER_H
