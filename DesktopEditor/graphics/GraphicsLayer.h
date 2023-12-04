#ifndef CGRAPHICSLAYER_H
#define CGRAPHICSLAYER_H

#include "Defines.h"
#include "./config.h"
#include "../common/IGrObject.h"

namespace Aggplus
{
	struct TGraphicsLayerSettings
	{
		double m_dOpacity;
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
					uchAlpha = (pSrcBuffer[order_type::A] * ((value_type)(m_oSettings.m_dOpacity * 255.)) + 1) >> 8;
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
					uchAlpha = ((SrcPixelFormatRenderer::base_mask + (value_type)m_oSettings.m_dOpacity * pSrcBuffer[order_type::A] * AlphaMaskFunction::calculate(pSrcAlphaMaskBuffer)) >> 8);

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
