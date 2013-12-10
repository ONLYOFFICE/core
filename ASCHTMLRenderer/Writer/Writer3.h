#pragma once
#include "Document.h"
#include "CalculatorCRC32.h"
#include "FontManager.h"
#include "..\Graphics\Matrix.h"
#include "../../Common/OfficeFileFormats.h"

namespace NSHtmlRenderer
{	
	class CRenderers
	{
	public:
		Graphics::IASCGraphicsRenderer*		m_pRendererSimple;
		Graphics::IASCGraphicsRenderer*		m_pRenderer;
		Graphics::IASCGraphicsRenderer*		m_pRendererDst;

		MediaCore::IAVSUncompressedVideoFrame*	m_pFrameSimple;
		MediaCore::IAVSUncompressedVideoFrame*	m_pFrame;
		MediaCore::IAVSUncompressedVideoFrame*	m_pFrameDst;

		BYTE*							m_pGraphicsCache;
		
		double							m_dWidth;
		double							m_dHeight;

		LONG							m_lWidthPix;
		LONG							m_lHeightPix;

		bool m_bIsNoSimpleAttack;
		bool m_bIsRaster;
		
		bool m_bIsSimple;
		bool m_bIsClip;
		
		bool m_bIsClipping;
		bool m_bIsDraw;

		bool m_bIsCheckGraphics;

	public:
		CRenderers()
		{
			m_pRenderer			= NULL;
			m_pRendererSimple	= NULL;
			m_pRendererDst		= NULL;

			m_pFrameSimple		= NULL;
			m_pFrame			= NULL;
			m_pFrameDst			= NULL;

			m_pGraphicsCache	= NULL;

			m_dWidth			= -1;
			m_dHeight			= -1;

			m_lWidthPix			= -1;
			m_lHeightPix		= -1;

			m_bIsNoSimpleAttack = false;
			m_bIsSimple			= true;
			m_bIsClip			= false;
			m_bIsClipping		= false;
			m_bIsDraw			= false;

			m_bIsCheckGraphics	= false;
		}
		~CRenderers()
		{
			RELEASEINTERFACE(m_pRendererSimple);
			RELEASEINTERFACE(m_pRenderer);
			RELEASEINTERFACE(m_pRendererDst);
			RELEASEINTERFACE(m_pFrameSimple);
			RELEASEINTERFACE(m_pFrame);
			RELEASEINTERFACE(m_pFrameDst);
			RELEASEARRAYOBJECTS(m_pGraphicsCache);
		}

		void NewPage(double dWidth, double dHeight)
		{
			m_bIsNoSimpleAttack = false;
			m_bIsRaster			= false;
			m_bIsSimple			= true;
			m_bIsClip			= false;
			m_bIsClipping		= false;
			m_bIsDraw			= false;

			m_bIsCheckGraphics	= false;

			if (dWidth != m_dWidth || dHeight != m_dHeight)
			{
				RELEASEINTERFACE(m_pFrameSimple);
				RELEASEINTERFACE(m_pFrame);
				RELEASEINTERFACE(m_pFrameDst);

				m_dWidth	= dWidth;
				m_dHeight	= dHeight;

				RELEASEARRAYOBJECTS(m_pGraphicsCache);
			}

			RELEASEINTERFACE(m_pRenderer);
			RELEASEINTERFACE(m_pRendererSimple);
			RELEASEINTERFACE(m_pRendererDst);

			m_lWidthPix		= (LONG)(96 * dWidth / 25.4);
			m_lHeightPix	= (LONG)(96 * dHeight / 25.4);

			if (NULL == m_pFrame)
			{
				CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pFrameSimple);
				CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pFrame);
				CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pFrameDst);

				m_pFrameSimple->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
				m_pFrameSimple->put_Width( m_lWidthPix );
				m_pFrameSimple->put_Height( m_lHeightPix );
				m_pFrameSimple->put_AspectRatioX( m_lWidthPix );
				m_pFrameSimple->put_AspectRatioY( m_lHeightPix );
				m_pFrameSimple->put_Interlaced( VARIANT_FALSE );
				m_pFrameSimple->put_Stride( 0, 4 * m_lWidthPix );
				m_pFrameSimple->AllocateBuffer( -1 );

				m_pFrame->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
				m_pFrame->put_Width( m_lWidthPix );
				m_pFrame->put_Height( m_lHeightPix );
				m_pFrame->put_AspectRatioX( m_lWidthPix );
				m_pFrame->put_AspectRatioY( m_lHeightPix );
				m_pFrame->put_Interlaced( VARIANT_FALSE );
				m_pFrame->put_Stride( 0, 4 * m_lWidthPix );
				m_pFrame->AllocateBuffer( -1 );

				m_pFrameDst->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
				m_pFrameDst->put_Width( m_lWidthPix );
				m_pFrameDst->put_Height( m_lHeightPix );
				m_pFrameDst->put_AspectRatioX( m_lWidthPix );
				m_pFrameDst->put_AspectRatioY( m_lHeightPix );
				m_pFrameDst->put_Interlaced( VARIANT_FALSE );
				m_pFrameDst->put_Stride( 0, 4 * m_lWidthPix );
				m_pFrameDst->AllocateBuffer( -1 );
			}

			BYTE* pBuffer = NULL;
			m_pFrame->get_Buffer(&pBuffer);

			memset(pBuffer, 0xFF, 4 * m_lWidthPix * m_lHeightPix);

			if (NULL == m_pGraphicsCache)
				m_pGraphicsCache = new BYTE[4 * m_lWidthPix * m_lHeightPix];

			CoCreateInstance(Graphics::CLSID_CASCGraphicsRenderer, NULL, CLSCTX_ALL, Graphics::IID_IASCGraphicsRenderer, (void**)&m_pRendererSimple);
			CoCreateInstance(Graphics::CLSID_CASCGraphicsRenderer, NULL, CLSCTX_ALL, Graphics::IID_IASCGraphicsRenderer, (void**)&m_pRenderer);
			CoCreateInstance(Graphics::CLSID_CASCGraphicsRenderer, NULL, CLSCTX_ALL, Graphics::IID_IASCGraphicsRenderer, (void**)&m_pRendererDst);
			//ставим FontManager
			VARIANT vtVariant;
			vtVariant.vt = VT_UNKNOWN;
			vtVariant.punkVal = NULL;
			m_pRendererSimple->SetAdditionalParam( L"FontManager", vtVariant );
			m_pRenderer->SetAdditionalParam( L"FontManager", vtVariant );
			m_pRendererDst->SetAdditionalParam( L"FontManager", vtVariant );

			m_pRendererSimple->put_Width(m_dWidth);
			m_pRendererSimple->put_Height(m_dHeight);
			m_pRendererSimple->CreateFromMediaData((IUnknown*)m_pFrameSimple, 0, 0, m_lWidthPix, m_lHeightPix );

			m_pRenderer->put_Width(m_dWidth);
			m_pRenderer->put_Height(m_dHeight);
			m_pRenderer->CreateFromMediaData((IUnknown*)m_pFrame, 0, 0, m_lWidthPix, m_lHeightPix );

			m_pRendererDst->put_Width(m_dWidth);
			m_pRendererDst->put_Height(m_dHeight);
			m_pRendererDst->CreateFromMediaData((IUnknown*)m_pFrameDst, 0, 0, m_lWidthPix, m_lHeightPix );

			m_bIsCheckGraphics = false;
		}

		void CheckNeedInitGraphics()
		{
			if (!m_bIsCheckGraphics)
			{
				BYTE* pBuffer1 = NULL;
				m_pFrame->get_Buffer(&pBuffer1);

				LONG lSize = m_lWidthPix * m_lHeightPix;

				// сохраняем картинку в кэш
				memcpy(m_pGraphicsCache, pBuffer1, lSize << 2);

				// теперь нужно подготовить фрейм dst
				BYTE* pBufferDst = NULL;
				m_pFrameDst->get_Buffer(&pBufferDst);
				memcpy(pBufferDst, m_pGraphicsCache, lSize << 2);

				for (LONG i = 0; i < lSize; i++)
				{
					pBufferDst[3] = 0;
					pBufferDst += 4;
				}

				// теперь копируем память во врейм для simple graphics
				BYTE* pBufferSimple = NULL;
				m_pFrameSimple->get_Buffer(&pBufferSimple);
				memcpy(pBufferSimple, pBuffer1, lSize << 2);
			}
			m_bIsCheckGraphics = true;
		}

		bool IsEqualImages()
		{
			BYTE* pBufferG = NULL;
			m_pFrameSimple->get_Buffer(&pBufferG);
			BYTE* pBuffer = NULL;
			m_pFrame->get_Buffer(&pBuffer);

			for (LONG i = 0; i < (4 * m_lWidthPix * m_lHeightPix); i++)
			{
				if (abs(pBuffer[i] - pBufferG[i]) > 10)
					return false;
			}
			return true;

			#if 0
			bool bIsEqual = (0 == memcmp(pBuffer, pBufferG, (size_t)(4 * m_lWidthPix * m_lHeightPix))) ? true : false;
			return bIsEqual;
			#endif
		}
		bool IsNoChangedGraphics()
		{
			BYTE* pBuffer = NULL;
			m_pFrame->get_Buffer(&pBuffer);

			bool bIsEqual = (0 == memcmp(pBuffer, m_pGraphicsCache, (size_t)(4 * m_lWidthPix * m_lHeightPix))) ? true : false;
			return bIsEqual;
		}

		template<typename T>
		bool ConvertVectorGraphics(bool bIsLongGraphics, bool& bIsEmpty, T pPage)
		{
			// не пользуемся goto по нашим соглашениям. Имитируем этот оператор сами
			bool bIsGoToSimple = false;
			bool bIsGoToPicture = false;

			bIsEmpty = false;
			bool bReturn = false;
			if (!m_bIsSimple && !bIsLongGraphics && !m_bIsNoSimpleAttack)
			{
				// а вот тут самое сложное. Тут нужно сначала понять, повлияли ли сложные настройки
				// clip/even-odd на вид графики. Если да - то опять вырезаем картинку. (пункт 3)
				// если нет - то переходим в пункт 2, т.е. просто скидываем вектор на клиент				
				bool bIsEqual = IsEqualImages();
				if (bIsEqual)
					bIsGoToSimple = true;
				else
					bIsGoToPicture = true;
			}

			if (bIsGoToSimple || (m_bIsSimple && !bIsLongGraphics && !m_bIsNoSimpleAttack))
			{
				// графика вся простая, и никаких клипов нет, и никаких сложных
				// линий и заливок нет. Значит - эту графику можно просто скинуть для последующей
				// отрисовки на канве. Потом наверное мы научимся рисовать и все сложное сразу на клиенте,
				// но пока - нет.
				bReturn = false;

				// теперь нужно понять, были ли изменения вообще
				bIsEmpty = IsNoChangedGraphics();

				// теперь скопируем изменения в кэш
				/*
				if (!bIsEmpty)
				{
					BYTE* pBuffer = NULL;
					m_pFrame->get_Buffer(&pBuffer);
					memcpy(m_pGraphicsCache, pBuffer, 4 * m_lWidthPix * m_lHeightPix);
				}
				*/
			}
			else if (bIsGoToPicture || bIsLongGraphics || m_bIsNoSimpleAttack)
			{
				// опять ничего анализировать не нужно. потому что динамически мы уже определили
				// что нас спасет только картинка. никакой тут векторной графики.
				// осталось только ее вырезать. Не забыть определить, если ли она вообще.
				// если нет - то просто не создавать медиадату, а вернуть null

				// TODO:
				bReturn = true;

				// и создаем картинку. ищем отличия от сохраненной в кэше
				BYTE* pBuffer = NULL;
				m_pFrameDst->get_Buffer(&pBuffer);

				RECT rect = GetImageBounds2(m_pFrame, m_pGraphicsCache);

				// пожертвуем всякими мелкими штуками
				if (((rect.right - rect.left) < 5) && ((rect.bottom - rect.top) < 5))
				{
					m_bIsSimple = (m_bIsClip == true) ? false : true;
					m_bIsNoSimpleAttack = false;
					m_bIsDraw = false;
					return true;
				}

				BYTE* pBufferSrcMem = pBuffer + 4 * rect.top * m_lWidthPix + 4 * rect.left;
				BYTE* pBufferCacheMem = m_pGraphicsCache + 4 * rect.top * m_lWidthPix + 4 * rect.left;
				LONG lWidthShape	= rect.right - rect.left + 1;
				LONG lHeightShape	= rect.bottom - rect.top + 1;

				MediaCore::IAVSUncompressedVideoFrame* pShapePicture = NULL;
				CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, 
										MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pShapePicture);

				pShapePicture->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
				pShapePicture->put_Width( lWidthShape );
				pShapePicture->put_Height( lHeightShape );
				pShapePicture->put_AspectRatioX( lWidthShape );
				pShapePicture->put_AspectRatioY( lHeightShape );
				pShapePicture->put_Interlaced( VARIANT_FALSE );
				pShapePicture->put_Stride( 0, 4 * lWidthShape );
				pShapePicture->AllocateBuffer( -1 );

				BYTE* pBufferDst = NULL;
				pShapePicture->get_Buffer(&pBufferDst);
				
				for (LONG lLine = 0; lLine < lHeightShape; ++lLine)
				{
					memcpy(pBufferDst, pBufferSrcMem, 4 * lWidthShape);
					
					pBufferDst		+= 4 * lWidthShape;
					pBufferSrcMem	+= 4 * m_lWidthPix;
				}

				double dL = 25.4 * rect.left / 96.0;
				double dT = 25.4 * rect.top / 96.0;
				double dW = 25.4 * lWidthShape / 96.0;
				double dH = 25.4 * lHeightShape / 96.0;

				double dHeightMM	= 25.4 * m_lHeightPix / 96.0;
				dT = (dHeightMM - dT - dH);
				
				pPage->WriteImage((IUnknown*)pShapePicture, dL, dT, dW, dH, 0);

				RELEASEINTERFACE(pShapePicture);
			}

			m_bIsSimple = (m_bIsClip == true) ? false : true;
			m_bIsNoSimpleAttack = false;
			m_bIsRaster = false;
			m_bIsDraw = false;
			m_bIsCheckGraphics = false;
			return bReturn;
		}

	public:
		// pen --------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetPen(BSTR bsXML)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetPen(bsXML);
				m_pRendererSimple->SetPen(bsXML);
				m_pRendererDst->SetPen(bsXML);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenColor(LONG lColor)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenColor(lColor);
				m_pRendererSimple->put_PenColor(lColor);
				m_pRendererDst->put_PenColor(lColor);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenAlpha(LONG lAlpha)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenAlpha(lAlpha);
				m_pRendererSimple->put_PenAlpha(lAlpha);
				m_pRendererDst->put_PenAlpha(lAlpha);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenSize(double dSize)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenSize(dSize);
				m_pRendererSimple->put_PenSize(dSize);
				m_pRendererDst->put_PenSize(dSize);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenDashStyle(BYTE val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenDashStyle(val);
				m_pRendererSimple->put_PenDashStyle(val);
				m_pRendererDst->put_PenDashStyle(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenLineStartCap(BYTE val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenLineStartCap(val);
				m_pRendererSimple->put_PenLineStartCap(val);
				m_pRendererDst->put_PenLineStartCap(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenLineEndCap(BYTE val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenLineEndCap(val);
				m_pRendererSimple->put_PenLineEndCap(val);
				m_pRendererDst ->put_PenLineEndCap(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenLineJoin(BYTE val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenLineJoin(val);
				m_pRendererSimple->put_PenLineJoin(val);
				m_pRendererDst->put_PenLineJoin(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenDashOffset(double val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenDashOffset(val);
				m_pRendererSimple->put_PenDashOffset(val);
				m_pRendererDst->put_PenDashOffset(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenAlign(LONG val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenAlign(val);
				m_pRendererSimple->put_PenAlign(val);
				m_pRendererDst->put_PenAlign(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenMiterLimit(double val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenMiterLimit(val);
				m_pRendererSimple->put_PenMiterLimit(val);
				m_pRendererDst->put_PenMiterLimit(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PenDashPattern(SAFEARRAY* pPattern)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PenDashPattern(pPattern);
				m_pRendererSimple->PenDashPattern(pPattern);
				m_pRendererDst->PenDashPattern(pPattern);
			}
			return S_OK;
		}
		// brush ------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetBrush(BSTR bsXML)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetBrush(bsXML);
				m_pRendererSimple->SetBrush(bsXML);
				m_pRendererDst->SetBrush(bsXML);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushType(LONG lType)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushType(lType);
				m_pRendererSimple->put_BrushType(lType);
				m_pRendererDst->put_BrushType(lType);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushColor1(LONG lColor)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushColor1(lColor);
				m_pRendererSimple->put_BrushColor1(lColor);
				m_pRendererDst->put_BrushColor1(lColor);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushAlpha1(LONG lAlpha)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushAlpha1(lAlpha);
				m_pRendererSimple->put_BrushAlpha1(lAlpha);
				m_pRendererDst->put_BrushAlpha1(lAlpha);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushColor2(LONG lColor)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushColor2(lColor);
				m_pRendererSimple->put_BrushColor2(lColor);
				m_pRendererDst->put_BrushColor2(lColor);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushAlpha2(LONG lAlpha)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushAlpha2(lAlpha);
				m_pRendererSimple->put_BrushAlpha2(lAlpha);
				m_pRendererDst->put_BrushAlpha2(lAlpha);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushTexturePath(BSTR bsPath)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushTexturePath(bsPath);
				m_pRendererSimple->put_BrushTexturePath(bsPath);
				m_pRendererDst->put_BrushTexturePath(bsPath);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushTextureMode(LONG lMode)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushTextureMode(lMode);
				m_pRendererSimple->put_BrushTextureMode(lMode);
				m_pRendererDst->put_BrushTextureMode(lMode);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushTextureAlpha(LONG lTxAlpha)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushTextureAlpha(lTxAlpha);
				m_pRendererSimple->put_BrushTextureAlpha(lTxAlpha);
				m_pRendererDst->put_BrushTextureAlpha(lTxAlpha);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushLinearAngle(double dAngle)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushLinearAngle(dAngle);
				m_pRendererSimple->put_BrushLinearAngle(dAngle);
				m_pRendererDst->put_BrushLinearAngle(dAngle);
			}
			return S_OK;
		}
		AVSINLINE HRESULT BrushRect(BOOL val, double left, double top, double width, double height)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->BrushRect(val, left, top, width, height);
				m_pRendererSimple->BrushRect(val, left, top, width, height);
				m_pRendererDst->BrushRect(val, left, top, width, height);
			}
			return S_OK;
		}
		// font -------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetFont(BSTR bsXML)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetFont(bsXML);
				m_pRendererSimple->SetFont(bsXML);
				m_pRendererDst->SetFont(bsXML);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_FontName(BSTR bsName)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_FontName(bsName);
				m_pRendererSimple->put_FontName(bsName);
				m_pRendererDst->put_FontName(bsName);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_FontPath(BSTR bsName)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_FontPath(bsName);
				m_pRendererSimple->put_FontPath(bsName);
				m_pRendererDst->put_FontPath(bsName);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_FontSize(double dSize)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_FontSize(dSize);
				m_pRendererSimple->put_FontSize(dSize);
				m_pRendererDst->put_FontSize(dSize);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_FontStyle(LONG lStyle)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_FontStyle(lStyle);
				m_pRendererSimple->put_FontStyle(lStyle);
				m_pRendererDst->put_FontStyle(lStyle);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_FontStringGID(BOOL bGID)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_FontStringGID(bGID);
				m_pRendererSimple->put_FontStringGID(bGID);
				m_pRendererDst->put_FontStringGID(bGID);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_FontCharSpace(double dSpace)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_FontCharSpace(dSpace);
				m_pRendererSimple->put_FontCharSpace(dSpace);
				m_pRendererDst->put_FontCharSpace(dSpace);
			}
			return S_OK;
		}
		// shadow -----------------------------------------------------------------------------------
		AVSINLINE HRESULT SetShadow(BSTR bsXML)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetShadow(bsXML);
				m_pRendererSimple->SetShadow(bsXML);
				m_pRendererDst->SetShadow(bsXML);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowDistanceX(double val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ShadowDistanceX(val);
				m_pRendererSimple->put_ShadowDistanceX(val);
				m_pRendererDst->put_ShadowDistanceX(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowDistanceY(double val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ShadowDistanceY(val);
				m_pRendererSimple->put_ShadowDistanceY(val);
				m_pRendererDst->put_ShadowDistanceY(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowBlurSize(double val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ShadowBlurSize(val);
				m_pRendererSimple->put_ShadowBlurSize(val);
				m_pRendererDst->put_ShadowBlurSize(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowColor(LONG val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ShadowColor(val);
				m_pRendererSimple->put_ShadowColor(val);
				m_pRendererDst->put_ShadowColor(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowAlpha(LONG val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ShadowAlpha(val);
				m_pRendererSimple->put_ShadowAlpha(val);
				m_pRendererDst->put_ShadowAlpha(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowVisible(BOOL val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ShadowVisible(val);
				m_pRendererSimple->put_ShadowVisible(val);
				m_pRendererDst->put_ShadowVisible(val);
			}
			return S_OK;
		}
		// edge -------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetEdgeText(BSTR bsXML)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetEdgeText(bsXML);
				m_pRendererSimple->SetEdgeText(bsXML);
				m_pRendererDst->SetEdgeText(bsXML);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeVisible(LONG val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_EdgeVisible(val);
				m_pRendererSimple->put_EdgeVisible(val);
				m_pRendererDst->put_EdgeVisible(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeColor(LONG val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_EdgeColor(val);
				m_pRendererSimple->put_EdgeColor(val);
				m_pRendererDst->put_EdgeColor(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeAlpha(LONG val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_EdgeAlpha(val);
				m_pRendererSimple->put_EdgeAlpha(val);
				m_pRendererDst->put_EdgeAlpha(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeDist(double val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_EdgeDist(val);
				m_pRendererSimple->put_EdgeDist(val);
				m_pRendererDst->put_EdgeDist(val);
			}
			return S_OK;
		}

		//-------- Функции для вывода текста --------------------------------------------------------
		AVSINLINE HRESULT CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
			}
			return S_OK;
		}
		AVSINLINE HRESULT CommandDrawTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->CommandDrawTextEx(bsUnicodeText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
			}
			return S_OK;
		}
		//-------- Маркеры для команд ---------------------------------------------------------------
		AVSINLINE HRESULT BeginCommand(DWORD lType)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->BeginCommand(lType);

				if (c_nClipType == lType)
					m_bIsClipping = true;
				
				if (!m_bIsClipping)
					m_pRendererSimple->BeginCommand(lType);
			}
			return S_OK;
		}
		AVSINLINE HRESULT EndCommand(DWORD lType)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->EndCommand(lType);
				m_pRendererDst->EndCommand(lType);

				if (c_nClipType == lType)
					m_bIsClipping = false;

				if (c_nClipType != lType)
					m_pRendererSimple->EndCommand(lType);
			}
			return S_OK;
		}
		//-------- Функции для работы с Graphics Path -----------------------------------------------
		AVSINLINE HRESULT PathCommandMoveTo(double fX, double fY)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandMoveTo(fX, fY);
				m_pRendererDst->PathCommandMoveTo(fX, fY);
								
				if (!m_bIsClipping)
					m_pRendererSimple->PathCommandMoveTo(fX, fY);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandLineTo(double fX, double fY)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandLineTo(fX, fY);
				m_pRendererDst->PathCommandLineTo(fX, fY);

				if (!m_bIsClipping)
					m_pRendererSimple->PathCommandLineTo(fX, fY);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandLinesTo(SAFEARRAY* pPoints)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandLinesTo(pPoints);
				m_pRendererDst->PathCommandLinesTo(pPoints);

				if (!m_bIsClipping)
					m_pRendererSimple->PathCommandLinesTo(pPoints);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
				m_pRendererDst->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);

				if (!m_bIsClipping)
					m_pRendererSimple->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandCurvesTo(SAFEARRAY* pPoints)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandCurvesTo(pPoints);
				m_pRendererDst->PathCommandCurvesTo(pPoints);

				if (!m_bIsClipping)
					m_pRendererSimple->PathCommandCurvesTo(pPoints);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
				m_pRendererDst->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);

				if (!m_bIsClipping)
					m_pRendererSimple->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandClose()
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandClose();
				m_pRendererDst->PathCommandClose();

				if (!m_bIsClipping)
					m_pRendererSimple->PathCommandClose();
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandEnd()
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandEnd();
				m_pRendererDst->PathCommandEnd();

				if (!m_bIsClipping)
					m_pRendererSimple->PathCommandEnd();
			}
			return S_OK;
		}
		AVSINLINE HRESULT DrawPath(long nType)
		{
			m_bIsDraw = true;
			CheckNeedInitGraphics();

			if (NULL != m_pRenderer)
			{
				m_pRenderer->DrawPath(nType);
				m_pRendererDst->DrawPath(nType);

				BYTE lDash = 0;
				m_pRenderer->get_PenDashStyle(&lDash);
				if ((nType & 0xFF) != 0 && 0 != lDash)
					m_bIsNoSimpleAttack = true;

				LONG lBrStyle = 0;
				m_pRenderer->get_BrushType(&lBrStyle);
				if (nType > 255 && lBrStyle != c_BrushTypeSolid)
					m_bIsNoSimpleAttack = true;

				m_pRendererSimple->DrawPath(nType);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandStart()
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandStart();
				m_pRendererDst->PathCommandStart();

				if (!m_bIsClipping)
					m_pRendererSimple->PathCommandStart();
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
				m_pRendererDst->PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);

				if (!m_bIsClipping)
					m_pRendererSimple->PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandTextEx(bsUnicodeText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
				m_pRendererDst->PathCommandTextEx(bsUnicodeText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);

				if (!m_bIsClipping)
					m_pRendererSimple->PathCommandTextEx(bsUnicodeText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
			}
			return S_OK;
		}
		AVSINLINE HRESULT SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
				m_pRendererDst->SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
				m_pRendererSimple->SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
			}
			return S_OK;
		}
		//-------- Функции для вывода изображений --------------------------------------------------
		AVSINLINE HRESULT DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
		{
			if (NULL != m_pRenderer)
			{
				CheckNeedInitGraphics();

				m_pRenderer->DrawImage(pInterface, fX, fY, fWidth, fHeight);
				m_pRendererDst->DrawImage(pInterface, fX, fY, fWidth, fHeight);
				m_bIsNoSimpleAttack = true;
				m_bIsRaster = true;
			}
			return S_OK;
		}
		AVSINLINE HRESULT DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
		{
			if (NULL != m_pRenderer)
			{
				CheckNeedInitGraphics();

				m_pRenderer->DrawImageFromFile(bstrVal, fX, fY, fWidth, fHeight);
				m_pRendererDst->DrawImageFromFile(bstrVal, fX, fY, fWidth, fHeight);
				m_bIsNoSimpleAttack = true;
				m_bIsRaster = true;
			}
			return S_OK;
		}
		//------------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetAdditionalParam(ParamName, ParamValue);
				m_pRendererDst->SetAdditionalParam(ParamName, ParamValue);
				m_pRendererSimple->SetAdditionalParam(ParamName, ParamValue);
			}
			return S_OK;
		}
		AVSINLINE HRESULT SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetTransform(dA, dB, dC, dD, dE, dF);
				m_pRendererDst->SetTransform(dA, dB, dC, dD, dE, dF);
				m_pRendererSimple->SetTransform(dA, dB, dC, dD, dE, dF);
			}
			return S_OK;
		}
		AVSINLINE HRESULT ResetTransform(void)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->ResetTransform();
				m_pRendererDst->ResetTransform();
				m_pRendererSimple->ResetTransform();
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ClipMode(LONG lMode)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ClipMode(lMode);
				m_pRendererDst->put_ClipMode(lMode);
				//m_pRendererSimple->put_ClipMode(lMode); // здесь только winding
			}
			return S_OK;
		}
	};

	class CFigure
	{
	public:
		CAtlArray<RECT>	m_arComplete;
		CAtlArray<RECT>	m_arNeedCalcs;

	public:
		BYTE*					m_pBuffer;
		LONG					m_lWidth;
		LONG					m_lHeight;

		LONG					m_lEpsilon;

	public:

		void Parse()
		{
			m_arComplete.RemoveAll();
			m_arNeedCalcs.RemoveAll();

			RECT oRect;
			oRect.left		= 0;
			oRect.top		= 0;
			oRect.right		= m_lWidth - 1;
			oRect.bottom	= m_lHeight - 1;

			m_lEpsilon		= 10;

			m_arNeedCalcs.Add(oRect);

			while (true)
			{
				if (0 == m_arNeedCalcs.GetCount())
					break;

				Graduation();
			}
		}
		void Graduation()
		{
			size_t nCountOld = m_arNeedCalcs.GetCount();
			for (size_t i = 0; i < nCountOld; ++i)
				Graduation(i);
			m_arNeedCalcs.RemoveAt(0, nCountOld);
		}
		void Graduation(size_t nIndex)
		{
			RECT oRect = m_arNeedCalcs[nIndex];

			CAtlArray<LONG> arrHors;
			CAtlArray<LONG> arrVers;

			// hors -----------------------------------------------------------------------------------
			LONG lCurrent = oRect.left;
			bool bIsEmptyOld = true;
			while (lCurrent <= oRect.right)
			{
				DWORD* pBufferMem = (DWORD*)(m_pBuffer + 4 * oRect.top * m_lWidth + 4 * lCurrent);

				bool bIsEmptyCur = true;

				long lCritical = oRect.bottom + 1;
				for (long i = oRect.top; i != lCritical; ++i, pBufferMem += m_lWidth)
				{
					if (*pBufferMem != 0x00)
					{
						bIsEmptyCur = false;
						break;
					}
				}

				if (bIsEmptyCur != bIsEmptyOld)
				{
					bIsEmptyOld = bIsEmptyCur;
					arrHors.Add(lCurrent);
				}

				++lCurrent;
			}
			arrHors.Add(oRect.right);

			if (0 != m_lEpsilon)
			{
				int nNecet = 1;
				int nCount = (int)arrHors.GetCount();
				for (; nNecet < (nCount - 1);)
				{
					if (((arrHors[nNecet] + m_lEpsilon) > arrHors[nNecet + 1]) && (nNecet + 1 != (nCount - 1)))
					{
						arrHors.RemoveAt(nNecet, 2);
						nCount -= 2;
					}
					else
					{
						nNecet += 2;
					}
				}
			}
			// ---------------------------------------------------------------------------------------
			// vers -----------------------------------------------------------------------------------
			lCurrent = oRect.top;
			bIsEmptyOld = true;
			while (lCurrent <= oRect.bottom)
			{
				DWORD* pBufferMem = (DWORD*)(m_pBuffer + 4 * lCurrent * m_lWidth + 4 * oRect.left);

				bool bIsEmptyCur = true;

				long lCritical = oRect.right + 1;
				for (long i = oRect.left; i != lCritical; ++i, ++pBufferMem)
				{
					if (*pBufferMem != 0x00)
					{
						bIsEmptyCur = false;
						break;
					}
				}

				if (bIsEmptyCur != bIsEmptyOld)
				{
					bIsEmptyOld = bIsEmptyCur;
					arrVers.Add(lCurrent);
				}

				++lCurrent;
			}
			arrVers.Add(oRect.bottom);

			if (0 != m_lEpsilon)
			{
				int nNecet = 1;
				int nCount = (int)arrVers.GetCount();
				for (; nNecet < (nCount - 1);)
				{
					if (((arrVers[nNecet] + m_lEpsilon) > arrVers[nNecet + 1]) && (nNecet + 1 != (nCount - 1)))
					{
						arrVers.RemoveAt(nNecet, 2);
						nCount -= 2;
					}
					else
					{
						nNecet += 2;
					}
				}
			}
			// ---------------------------------------------------------------------------------------

			LONG lCountH = (LONG)arrHors.GetCount() / 2;
			LONG lCountV = (LONG)arrVers.GetCount() / 2;

			if ((1 == lCountH) && (1 == lCountV))
			{
				RECT oCompleteRect;
				oCompleteRect.left		= arrHors[0];
				oCompleteRect.right		= arrHors[1];
				oCompleteRect.top		= arrVers[0];
				oCompleteRect.bottom	= arrVers[1];

				m_arComplete.Add(oCompleteRect);
				return;
			}

			for (LONG lJ = 0; lJ < lCountV; ++lJ)
			{
				for (LONG lI = 0; lI < lCountH; ++lI)
				{
					RECT oNeedRect;
					oNeedRect.left		= arrHors[2 * lI];
					oNeedRect.right		= arrHors[2 * lI + 1];
					oNeedRect.top		= arrVers[2 * lJ];
					oNeedRect.bottom	= arrVers[2 * lJ + 1];

					m_arNeedCalcs.Add(oNeedRect);
				}
			}

			return;
		}

		template<typename T>
		void Convert(T pPage, CRenderers* pRenderers, bool bIsScan)
		{
			if ((NULL == pRenderers->m_pFrameSimple) || (NULL == pRenderers->m_pFrame) || (NULL == pPage))
				return;

			pRenderers->m_pFrame->get_Buffer(&m_pBuffer);
			pRenderers->m_pFrame->get_Width(&m_lWidth);
			pRenderers->m_pFrame->get_Height(&m_lHeight);

			Parse();

			BYTE* pBufferSrc = NULL;
			pRenderers->m_pFrame->get_Buffer(&pBufferSrc);

			/*
			if (0 == m_arComplete.GetCount())
			{
				RECT rect;
				rect.left		= 0;
				rect.top		= 0;
				rect.right		= m_lWidth - 1;
				rect.bottom		= m_lHeight - 1;
				m_arComplete.Add(rect);
			}
			*/
			size_t nCount = m_arComplete.GetCount();

			if (bIsScan && (nCount > 2))
			{
				m_arComplete.RemoveAll();
				RECT oRect;
				oRect.left		= 0;
				oRect.top		= 0;
				oRect.right		= m_lWidth - 1;
				oRect.bottom	= m_lHeight - 1;
				m_arComplete.Add(oRect);
				nCount = 1;
			}

			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				BYTE* pBufferSrcMem = pBufferSrc + 4 * m_arComplete[nIndex].top * m_lWidth + 4 * m_arComplete[nIndex].left;
				LONG lWidthShape	= m_arComplete[nIndex].right - m_arComplete[nIndex].left + 1;
				LONG lHeightShape	= m_arComplete[nIndex].bottom - m_arComplete[nIndex].top + 1;

				MediaCore::IAVSUncompressedVideoFrame* pShapePicture = NULL;
				CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, 
										MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pShapePicture);

				pShapePicture->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
				pShapePicture->put_Width( lWidthShape );
				pShapePicture->put_Height( lHeightShape );
				pShapePicture->put_AspectRatioX( lWidthShape );
				pShapePicture->put_AspectRatioY( lHeightShape );
				pShapePicture->put_Interlaced( VARIANT_FALSE );
				pShapePicture->put_Stride( 0, 4 * lWidthShape );
				pShapePicture->AllocateBuffer( -1 );

				BYTE* pBufferDst = NULL;
				pShapePicture->get_Buffer(&pBufferDst);
				
				for (LONG lLine = 0; lLine < lHeightShape; ++lLine)
				{
					memcpy(pBufferDst, pBufferSrcMem, 4 * lWidthShape);
					pBufferDst		+= 4 * lWidthShape;
					pBufferSrcMem	+= 4 * m_lWidth;
				}

				double dL = 25.4 * m_arComplete[nIndex].left / 96.0;
				double dT = 25.4 * m_arComplete[nIndex].top / 96.0;
				double dW = 25.4 * lWidthShape / 96.0;
				double dH = 25.4 * lHeightShape / 96.0;

				double dHeightMM	= 25.4 * m_lHeight / 96.0;
				dT = (dHeightMM - dT - dH);
				
				pPage->WriteImage((IUnknown*)pShapePicture, dL, dT, dW, dH, 0);

				RELEASEINTERFACE(pShapePicture);
			}
		}

		template<typename T>
		void ConvertSimple(T pPage, CRenderers* pRenderers)
		{
			if ((NULL == pRenderers->m_pFrameSimple) || (NULL == pRenderers->m_pFrame) || (NULL == pPage))
				return;

			pRenderers->m_pFrame->get_Buffer(&m_pBuffer);
			pRenderers->m_pFrame->get_Width(&m_lWidth);
			pRenderers->m_pFrame->get_Height(&m_lHeight);

			RECT rect = GetImageBounds(pRenderers->m_pFrame);

			if (((rect.right - rect.left) < 5) && ((rect.bottom - rect.top) < 5))
				return;

			BYTE* pBufferSrcMem = m_pBuffer + 4 * rect.top * m_lWidth + 4 * rect.left;
			LONG lWidthShape	= rect.right - rect.left + 1;
			LONG lHeightShape	= rect.bottom - rect.top + 1;

			MediaCore::IAVSUncompressedVideoFrame* pShapePicture = NULL;
			CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, 
									MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pShapePicture);

			pShapePicture->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
			pShapePicture->put_Width( lWidthShape );
			pShapePicture->put_Height( lHeightShape );
			pShapePicture->put_AspectRatioX( lWidthShape );
			pShapePicture->put_AspectRatioY( lHeightShape );
			pShapePicture->put_Interlaced( VARIANT_FALSE );
			pShapePicture->put_Stride( 0, 4 * lWidthShape );
			pShapePicture->AllocateBuffer( -1 );

			BYTE* pBufferDst = NULL;
			pShapePicture->get_Buffer(&pBufferDst);
			
			for (LONG lLine = 0; lLine < lHeightShape; ++lLine)
			{
				memcpy(pBufferDst, pBufferSrcMem, 4 * lWidthShape);
				pBufferDst		+= 4 * lWidthShape;
				pBufferSrcMem	+= 4 * m_lWidth;
			}

			double dL = 25.4 * rect.left / 96.0;
			double dT = 25.4 * rect.top / 96.0;
			double dW = 25.4 * lWidthShape / 96.0;
			double dH = 25.4 * lHeightShape / 96.0;

			double dHeightMM	= 25.4 * m_lHeight / 96.0;
			dT = (dHeightMM - dT - dH);
			
			pPage->WriteImage((IUnknown*)pShapePicture, dL, dT, dW, dH, 0);

			RELEASEINTERFACE(pShapePicture);
		}
	};

	static double	__g_matrix_eps	= 0.0001;

	class CFontEmbedded
	{
	public:
		CAtlMap<WCHAR, BYTE>	m_mapChars;

		CString					m_strFontName;
		LONG					m_lFontStyle;
		int						m_lFontPathLen;

		CString					m_strFontPath;
		DWORD					m_dwCRC32;

	public:
		CFontEmbedded() : m_mapChars()
		{
			m_strFontName		= _T("");
			m_strFontPath		= _T("");

			m_lFontPathLen		= 0;
			m_lFontStyle		= 0;
			m_dwCRC32			= 0;
		}
		CFontEmbedded(const CFontEmbedded& oSrc)
		{
			*this = oSrc;
		}
		CFontEmbedded& operator=(const CFontEmbedded& oSrc)
		{
			m_strFontPath		= oSrc.m_strFontPath;
			m_strFontName		= oSrc.m_strFontName;
			m_lFontStyle		= oSrc.m_lFontStyle;
			m_dwCRC32			= oSrc.m_dwCRC32;

			m_lFontPathLen		= oSrc.m_lFontPathLen;

			m_mapChars.RemoveAll();
			POSITION pos = oSrc.m_mapChars.GetStartPosition();
			while (NULL != pos)
			{
				const CAtlMap<WCHAR, BYTE>::CPair* pPair = oSrc.m_mapChars.GetNext(pos);
				m_mapChars.SetAt(pPair->m_key, pPair->m_value);
			}

			return *this;
		}

	public:
		void AddBSTR(BSTR bsText)
		{
			if (NULL == bsText)
				return;

			WCHAR* s = (WCHAR*)bsText;
			for (; *s != 0; ++s)
				m_mapChars.SetAt(*s, 1);
		}
		SAFEARRAY* GenerateSafearray()
		{
			ULONG nCount = (ULONG)m_mapChars.GetCount();
			
			SAFEARRAYBOUND rgsab;
			rgsab.lLbound	= 0;
			rgsab.cElements	= nCount;

			SAFEARRAY* pArray = SafeArrayCreate(VT_UI2, 1, &rgsab);
			USHORT* pBuffer = (USHORT*)(pArray->pvData);

			POSITION pos = m_mapChars.GetStartPosition();
			while (NULL != pos)
			{
				CAtlMap<WCHAR, BYTE>::CPair* pPair = m_mapChars.GetNext(pos);
				*pBuffer = pPair->m_key;
				++pBuffer;
			}
			return pArray;
		}
	};
	class CFontDstGenerator
	{
	public:
		CFontEmbedded*	m_pFonts;
		LONG			m_lCurrentIndex;
		LONG			m_lCountFonts;
		LONG			m_lSize;

		CAtlMap<CString, DWORD> m_mapFontPathCRC;
		CAtlMap<DWORD, LONG> m_mapFontCRCIndex;
		CCalculatorCRC32 m_oCalc;

		CString m_strCurrentFontPath;
		LONG	m_lCurrentFontStyle;
		LONG	m_lCurrentFontPathLen;
		CAtlMap<CString, LONG> m_mapFontPathToIndex;

	public:
		CFontDstGenerator()
		{
			m_lSize			= 50;
			m_pFonts		= NULL;
			m_lCurrentIndex	= -1;
			m_lCountFonts	= 0;

			m_strCurrentFontPath = _T("");
			m_lCurrentFontPathLen = 0;
			m_lCurrentFontStyle = -1;

			Grow();
		}
		~CFontDstGenerator()
		{
			RELEASEARRAYOBJECTS(m_pFonts);
		}
	public:
		LONG AddFont2(NSStructures::CFont* pFont, CFontManagerBase* pBase, BSTR bsTextUnicode)
		{
			bool bIsDumpFontEmptyPath = false;

			int nLen = pFont->Path.GetLength();
			if (nLen == 0)
			{
				pFont->Path = pBase->GetFontPath(pFont);
				nLen = pFont->Path.GetLength();
				bIsDumpFontEmptyPath = true;
			}

			LONG lNewStyle = pFont->GetStyle2();

			bool bIsFontChanged = true;
			if (-1 != m_lCurrentIndex)
			{
				if (nLen == m_pFonts[m_lCurrentIndex].m_lFontPathLen && (m_pFonts[m_lCurrentIndex].m_lFontStyle == lNewStyle))
				{
					if (m_pFonts[m_lCurrentIndex].m_strFontPath == pFont->Path)
						bIsFontChanged = false;
				}
			}
			
			if (-1 != m_lCurrentIndex && !bIsFontChanged)
			{
				m_pFonts[m_lCurrentIndex].AddBSTR(bsTextUnicode);

				if (bIsDumpFontEmptyPath)
					pFont->Path = _T("");

				return m_lCurrentIndex;
			}

			LONG lIndex = m_lCountFonts - 1;
			while (lIndex >= 0)
			{
				if (nLen == m_pFonts[lIndex].m_lFontPathLen && (m_pFonts[lIndex].m_lFontStyle == lNewStyle))
				{
					if (m_pFonts[lIndex].m_strFontPath == pFont->Path)
						break;
				}
				--lIndex;
			}
			if (lIndex >= 0)
			{
				m_lCurrentIndex = lIndex;
				m_pFonts[m_lCurrentIndex].AddBSTR(bsTextUnicode);

				if (bIsDumpFontEmptyPath)
					pFont->Path = _T("");

				return m_lCurrentIndex;
			}

			// теперь нужно проверить чексумму
			CAtlMap<CString, DWORD>::CPair* pPair1 = m_mapFontPathCRC.Lookup(pFont->Path);
			if (NULL == pPair1)
			{
				// новый шрифт.
				CFile oFile;
				oFile.OpenFile(pFont->Path);
				DWORD lFontFileSize = (DWORD)oFile.GetFileSize();
				BYTE* pFontPathData = new BYTE[lFontFileSize];
				oFile.ReadFile(pFontPathData, lFontFileSize);
				DWORD dwCRC32 = m_oCalc.Calc(pFontPathData, (int)lFontFileSize);

				oFile.CloseFile();
				RELEASEARRAYOBJECTS(pFontPathData);

				m_mapFontPathCRC.SetAt(pFont->Path, dwCRC32);

				CAtlMap<DWORD, LONG>::CPair* pPair2 = m_mapFontCRCIndex.Lookup(dwCRC32);
				if (NULL == pPair2)
				{
					m_mapFontCRCIndex.SetAt(dwCRC32, m_lCountFonts);
				}
				else
				{
					m_lCurrentIndex = pPair2->m_value;

					m_pFonts[m_lCurrentIndex].AddBSTR(bsTextUnicode);

					if (bIsDumpFontEmptyPath)
						pFont->Path = _T("");

					return m_lCurrentIndex;
				}
			}
			else
			{
				// такой шрифт уже был. просто чексумма может быть записана не не него
				CAtlMap<DWORD, LONG>::CPair* pPair2 = m_mapFontCRCIndex.Lookup(pPair1->m_value);
				// тут NULL быть не может
				m_lCurrentIndex = pPair2->m_value;

				m_pFonts[m_lCurrentIndex].AddBSTR(bsTextUnicode);

				if (bIsDumpFontEmptyPath)
					pFont->Path = _T("");

				return m_lCurrentIndex;
			}

			if (m_lCountFonts == m_lSize)
			{
				// нужно перевыделить память
				Grow();
			}

			m_lCurrentIndex = m_lCountFonts;
			++m_lCountFonts;

			NSHtmlRenderer::CFontEmbedded* pCurEmb = &m_pFonts[m_lCurrentIndex];
			pCurEmb->m_strFontName = pFont->Name;
			pCurEmb->m_strFontPath = pFont->Path;
			pCurEmb->m_lFontStyle	= lNewStyle;
			pCurEmb->m_lFontPathLen = nLen;
			pCurEmb->AddBSTR(bsTextUnicode);

			// теперь нужно найти к нему путь...
			if (0 == nLen)
			{
				pCurEmb->m_strFontPath = pBase->GetFontPath(pFont);
				pCurEmb->m_lFontPathLen = pCurEmb->m_strFontPath.GetLength();
			}

			if (bIsDumpFontEmptyPath)
				pFont->Path = _T("");

			return m_lCurrentIndex;
		}

		LONG AddFont(NSStructures::CFont* pFont, CFontManagerBase* pBase, BSTR bsTextUnicode)
		{
			bool bIsDumpFontEmptyPath = false;

			int nLen = pFont->Path.GetLength();
			if (nLen == 0)
			{
				pFont->Path = pBase->GetFontPath(pFont);
				nLen = pFont->Path.GetLength();
				bIsDumpFontEmptyPath = true;
			}

			LONG lNewStyle = pFont->GetStyle2();

			bool bIsFontChanged = true;
			if (nLen == m_lCurrentFontPathLen && (m_lCurrentFontStyle == lNewStyle))
			{
				if (m_strCurrentFontPath == pFont->Path)
					bIsFontChanged = false;
			}
			
			if (!bIsFontChanged)
			{
				m_pFonts[m_lCurrentIndex].AddBSTR(bsTextUnicode);

				if (bIsDumpFontEmptyPath)
					pFont->Path = _T("");

				return m_lCurrentIndex;
			}

			CAtlMap<CString, LONG>::CPair* pPairIndex = m_mapFontPathToIndex.Lookup(pFont->Path);
			if (NULL != pPairIndex)
			{
				m_lCurrentIndex = pPairIndex->m_value;

				NSHtmlRenderer::CFontEmbedded* pEmb = &m_pFonts[m_lCurrentIndex];

				m_strCurrentFontPath = pFont->Path;
				m_lCurrentFontPathLen = nLen;
				m_lCurrentFontStyle = lNewStyle;

				pEmb->AddBSTR(bsTextUnicode);

				if (bIsDumpFontEmptyPath)
					pFont->Path = _T("");

				return m_lCurrentIndex;
			}

			// такой путь еще не приходил.
			// первым делом проверим, не совпадает ли он с каким-то (по чексумме)
			CFile oFile;
			oFile.OpenFile(pFont->Path);
			DWORD lFontFileSize = (DWORD)oFile.GetFileSize();
			BYTE* pFontPathData = new BYTE[lFontFileSize];
			oFile.ReadFile(pFontPathData, lFontFileSize);
			DWORD dwCRC32 = m_oCalc.Calc(pFontPathData, (int)lFontFileSize);

			oFile.CloseFile();
			RELEASEARRAYOBJECTS(pFontPathData);

			CAtlMap<DWORD, LONG>::CPair* pPair2 = m_mapFontCRCIndex.Lookup(dwCRC32);
			if (NULL == pPair2)
			{
				// шрифт реально новый
				m_mapFontCRCIndex.SetAt(dwCRC32, m_lCountFonts);
				m_mapFontPathToIndex.SetAt(pFont->Path, m_lCountFonts);
			}
			else
			{
				m_lCurrentIndex = pPair2->m_value;

				m_mapFontPathToIndex.SetAt(pFont->Path, m_lCurrentIndex);

				m_strCurrentFontPath = pFont->Path;
				m_lCurrentFontPathLen = nLen;
				m_lCurrentFontStyle = lNewStyle;

				m_pFonts[m_lCurrentIndex].AddBSTR(bsTextUnicode);

				if (bIsDumpFontEmptyPath)
					pFont->Path = _T("");

				return m_lCurrentIndex;
			}

			if (m_lCountFonts == m_lSize)
			{
				// нужно перевыделить память
				Grow();
			}

			m_lCurrentIndex = m_lCountFonts;
			++m_lCountFonts;

			NSHtmlRenderer::CFontEmbedded* pCurEmb = &m_pFonts[m_lCurrentIndex];
			pCurEmb->m_strFontName = pFont->Name;
			pCurEmb->m_strFontPath = pFont->Path;
			pCurEmb->m_lFontStyle	= lNewStyle;
			pCurEmb->m_lFontPathLen = nLen;
			pCurEmb->AddBSTR(bsTextUnicode);

			// теперь нужно найти к нему путь...
			if (0 == nLen)
			{
				pCurEmb->m_strFontPath = pBase->GetFontPath(pFont);
				pCurEmb->m_lFontPathLen = pCurEmb->m_strFontPath.GetLength();
			}

			if (bIsDumpFontEmptyPath)
				pFont->Path = _T("");

			m_strCurrentFontPath = pCurEmb->m_strFontPath;
			m_lCurrentFontPathLen = pCurEmb->m_lFontPathLen;
			m_lCurrentFontStyle = pCurEmb->m_lFontStyle;

			return m_lCurrentIndex;
		}


	public:
		void WriteFonts(CString strFolderDst, bool bIsGid = false)
		{
			CString sDstF = strFolderDst;
			CDirectory::CreateDirectory(sDstF);

			CString strAllTypes = _T("");

			CString strTempFont = sDstF + _T("\\font.tmp");
			FontConverter::IFontConverter* pFontConverter = NULL;
			CoCreateInstance(__uuidof(FontConverter::CFontConverter), NULL, CLSCTX_ALL, __uuidof(FontConverter::IFontConverter), (void**)&pFontConverter);
			
			LONG lFontConverterFlag = 16;
			if (bIsGid)
				lFontConverterFlag |= 0x0080;

			Graphics::IASCFontManagerPtr pMan;
			pMan.CreateInstance(Graphics::CLSID_CASCFontManager);
			pMan->Initialize(L"");
			for (LONG lIndex = 0; lIndex < m_lCountFonts; ++lIndex)
			{
				CString sPath = m_pFonts[lIndex].m_strFontPath;

				BSTR bsF = sPath.AllocSysString();
				pMan->LoadFontFromFile(bsF, 12, 72, 72, 0);
				SysFreeString(bsF);

				BSTR bsName = NULL;
				BSTR bsType = NULL;
				pMan->GetFamilyName(&bsName);
				pMan->GetFontType(&bsType);

				CString sN = (CString)bsName;
				CString sT = (CString)bsType;

				for (int i = 0; i < sN.GetLength(); ++i)
				{
					if ((bsName[i] >= 'a' && bsName[i] <= 'z') ||
						(bsName[i] >= 'A' && bsName[i] <= 'Z') ||
						(bsName[i] >= '0' && bsName[i] <= '9'))
						continue;

					bsName[i] = WCHAR('_');
				}

				CString str_p1 = _T("");
				str_p1.Format(_T("emdedded%d: "), lIndex);
				strAllTypes += str_p1;
				strAllTypes += sT;
				strAllTypes += _T(", ");
				strAllTypes += CString(bsName);
				strAllTypes += _T("\n");

				CString strFileD = _T("");
				strFileD.Format(_T("embedded%d"), lIndex);
				//strFileD += CString(bsName);

				if (NULL != pFontConverter)
				{
					BSTR bsFontIn = sPath.AllocSysString();
					BSTR bsFontDst = strTempFont.AllocSysString();

					SAFEARRAY* pArrayUnicodes = m_pFonts[lIndex].GenerateSafearray();
					pFontConverter->ToOTF(bsFontIn, bsFontDst, pArrayUnicodes, bsName, lFontConverterFlag); // TrueType only
					RELEASEARRAY(pArrayUnicodes);

					SysFreeString(bsFontIn);
					SysFreeString(bsFontDst);

					sPath = strTempFont;
				}

				SysFreeString(bsType);
				SysFreeString(bsName);

				// dump font
				CFile oFileFontFile;
				oFileFontFile.OpenFile(sPath);
				
				int nInputLen = (int)oFileFontFile.GetFileSize();
				BYTE* pData = new BYTE[nInputLen];
				oFileFontFile.ReadFile(pData, (DWORD)nInputLen);

				oFileFontFile.CloseFile();

				int nOutputLen = Base64EncodeGetRequiredLength(nInputLen, ATL_BASE64_FLAG_NOCRLF);
				BYTE* pOutput = new BYTE[nOutputLen];
				Base64Encode(pData, nInputLen, (LPSTR)pOutput, &nOutputLen, ATL_BASE64_FLAG_NOCRLF);

				CString sFilePathDst = sDstF + _T("\\") + strFileD + _T(".js");
				CFile oFileFontFileJS;
				oFileFontFileJS.CreateFile(sFilePathDst);

				CString strWriteJS = _T("window[\"") + strFileD + _T("\"] = \"");
				CStringA t1 = (CStringA)strWriteJS;
				oFileFontFileJS.WriteFile((void*)t1.GetBuffer(), (DWORD)t1.GetLength());
				t1 = "";
				t1.Format("%d;", nInputLen);
				oFileFontFileJS.WriteFile((void*)t1.GetBuffer(), (DWORD)t1.GetLength());
				oFileFontFileJS.WriteFile((void*)pOutput, (DWORD)(nOutputLen));
				t1 = "\";";
				oFileFontFileJS.WriteFile((void*)t1.GetBuffer(), (DWORD)t1.GetLength());				
			
				oFileFontFileJS.CloseFile();

				RELEASEARRAYOBJECTS(pOutput);
				RELEASEARRAYOBJECTS(pData);

				if (NULL != pFontConverter)
					::DeleteFile(sPath);
			}

			CFile oFileFontFileJS_type;
			oFileFontFileJS_type.CreateFile(sDstF + _T("\\types.txt"));

			CStringA strA = (CStringA)strAllTypes;
			oFileFontFileJS_type.WriteFile((void*)strA.GetBuffer(), (DWORD)strA.GetLength());
			oFileFontFileJS_type.CloseFile();

			RELEASEINTERFACE(pFontConverter);
		}

	protected:
		void Grow()
		{
			if (NULL == m_pFonts)
			{
				m_pFonts = new CFontEmbedded[m_lSize];
				return;
			}

			m_lSize *= 2;
			CFontEmbedded* pNewBuffer = new CFontEmbedded[m_lSize];
			for (LONG i = 0; i < m_lCountFonts; ++i)
			{
				pNewBuffer[i] = m_pFonts[i];
			}
			RELEASEARRAYOBJECTS(m_pFonts);
			m_pFonts = pNewBuffer;
		}
	};

	static LONG __strlen(const char* str)
	{
		const char* s = str;
		for (; *s != 0; ++s);
		return (LONG)(s - str);
	}
	static LONG __wstrlen(const wchar_t* str)
	{
		const wchar_t* s = str;
		for (; *s != 0; ++s);
		return (LONG)(s - str);
	}

	class CMetafile
	{
	public:
		enum CommandType
		{
			// pen
			ctPenXML						= 0,
			ctPenColor						= 1,
			ctPenAlpha						= 2,
			ctPenSize						= 3,
			ctPenDashStyle					= 4,
			ctPenLineStartCap				= 5,
			ctPenLineEndCap					= 6,
			ctPenLineJoin					= 7,
			ctPenDashPatern					= 8,
			ctPenDashPatternCount			= 9,
			ctPenDashOffset					= 10,
			ctPenAlign						= 11,
			ctPenMiterLimit					= 12,

			// brush
			ctBrushXML						= 20,
			ctBrushType						= 21,
			ctBrushColor1					= 22,
			ctBrushColor2					= 23,
			ctBrushAlpha1					= 24,
			ctBrushAlpha2					= 25,
			ctBrushTexturePath				= 26,
			ctBrushTextureAlpha				= 27,
			ctBrushTextureMode				= 28,
			ctBrushRectable					= 29,
			ctBrushAngle                    = 30,
			ctBrushSubColors                = 31,

			// font
			ctFontXML						= 40,
			ctFontName						= 41,
			ctFontSize						= 42,
			ctFontStyle						= 43,
			ctFontPath						= 44,
			ctFontGID						= 45,
			ctFontCharSpace					= 46,

			// shadow
			ctShadowXML						= 50,
			ctShadowVisible					= 51,
			ctShadowDistanceX				= 52,
			ctShadowDistanceY				= 53,
			ctShadowBlurSize				= 54,
			ctShadowColor					= 55,
			ctShadowAlpha					= 56,
			
			// edge
			ctEdgeXML						= 70,
			ctEdgeVisible					= 71,
			ctEdgeDistance					= 72,
			ctEdgeColor						= 73,
			ctEdgeAlpha						= 74,

			// text
			ctDrawText						= 80,
			ctDrawTextEx					= 81,

			// pathcommands		
			ctPathCommandMoveTo				= 91,
			ctPathCommandLineTo				= 92,
			ctPathCommandLinesTo			= 93,
			ctPathCommandCurveTo			= 94,
			ctPathCommandCurvesTo			= 95,
			ctPathCommandArcTo				= 96,
			ctPathCommandClose				= 97,
			ctPathCommandEnd				= 98,
			ctDrawPath						= 99,
			ctPathCommandStart				= 100,
			ctPathCommandGetCurrentPoint	= 101,
			ctPathCommandText				= 102,
			ctPathCommandTextEx				= 103,

			// image
			ctDrawImage						= 110,
			ctDrawImageFromFile				= 111,

			ctSetParams						= 120,

			ctBeginCommand					= 121,
			ctEndCommand					= 122,

			ctSetTransform					= 130,
			ctResetTransform				= 131,
			
			ctClipMode						= 140,

			ctCommandLong1					= 150,
			ctCommandDouble1				= 151,
			ctCommandString1				= 152,
			ctCommandLong2					= 153,
			ctCommandDouble2				= 154,
			ctCommandString2				= 155,

			ctCommandTextLine				= 160,
			ctCommandTextTransform			= 161,
			ctCommandTextLineEnd			= 162,

			ctCommandTextClipRectReset		= 163,
			ctCommandTextClipRect			= 164,

			ctError							= 255
		};

	private:

		// сам метафайл
		BYTE* m_pBuffer;
		BYTE* m_pBufferMem;
		
		size_t m_lPosition;
		size_t m_lSize;

		LONG m_lSizeofDouble;
		LONG m_lSizeofFloat;
		LONG m_lSizeofLONG;
		LONG m_lSizeofBYTE;

	public:
		double m_fWidth;
		double m_fHeight;

	public:
		CMetafile()
		{
			Clear();

			m_lSizeofDouble	= sizeof(double);
			m_lSizeofFloat 	= sizeof(float);
			m_lSizeofLONG	= sizeof(long);
			m_lSizeofBYTE   = sizeof(BYTE);
		}
		~CMetafile()
		{
			RELEASEARRAYOBJECTS(m_pBuffer);
		}

	public:

		inline LONG GetPosition()
		{
			return (LONG)m_lPosition;
		}
		inline BYTE* GetData()
		{
			return m_pBuffer;
		}
		inline void ClearNoAttack()
		{
			m_lPosition = 0;
			m_pBufferMem = m_pBuffer;
		}

		inline void Clear()
		{
			m_lSize		= 0;
			m_lPosition = 0;

			m_pBuffer = NULL;
			m_pBufferMem = NULL;
		}
		inline void Seek(LONG lPos)
		{
			m_lPosition = (size_t)lPos;
			m_pBufferMem = m_pBuffer + m_lPosition;
		}

		inline void CheckBufferSize(size_t lPlus)
		{
			if (NULL != m_pBuffer)
			{
				size_t nNewSize = m_lPosition + lPlus;

				if (nNewSize >= m_lSize)
				{
					while (nNewSize >= m_lSize)
					{
						m_lSize *= 2;
					}
					
					BYTE* pNew = new BYTE[m_lSize];
					memcpy(pNew, m_pBuffer, m_lPosition);

					RELEASEARRAYOBJECTS(m_pBuffer);
					m_pBuffer = pNew;
				}
			}
			else
			{
				m_lSize		= 1000;
				m_pBuffer	= new BYTE[m_lSize];

				CheckBufferSize(lPlus);
			}
		}

		inline void WriteCommandType(const CommandType& eType)
		{
			CheckBufferSize(m_lSizeofBYTE);

			*(m_pBuffer + m_lPosition)	= (BYTE)eType; 
			m_lPosition += m_lSizeofBYTE;
		}

		//
		inline void WriteBYTE_nocheck(const BYTE& lValue)
		{
			*(m_pBuffer + m_lPosition)	= lValue; 
			m_lPosition += m_lSizeofBYTE;
		}
		inline void WriteLONG_nocheck(const LONG& lValue)
		{
			*((LONG*)(m_pBuffer + m_lPosition))	= lValue; 
			m_lPosition += m_lSizeofLONG;
		}
		inline void WriteUSHORT_nocheck(const USHORT& lValue)
		{
			*((USHORT*)(m_pBuffer + m_lPosition)) = lValue; 
			m_lPosition += sizeof(USHORT);
		}
		inline void WriteWCHAR_nocheck(const WCHAR& lValue)
		{
			*((WCHAR*)(m_pBuffer + m_lPosition))	= lValue; 
			m_lPosition += sizeof(WCHAR);
		}
		inline void WriteDouble_nocheck(const double& dValue)
		{
			// здесь никаких даблов. Сплошные округления
			LONG lValue = (LONG)(dValue * 10000);
			WriteLONG_nocheck(lValue);
			return;

			CheckBufferSize(m_lSizeofDouble);

			*((double*)(m_pBuffer + m_lPosition)) = dValue; 
			m_lPosition += m_lSizeofDouble;
		}
		inline void WriteDouble2_nocheck(const double& dValue)
		{
			// здесь никаких даблов. Сплошные округления
			SHORT lValue = (SHORT)(dValue * 100);
			*((SHORT*)(m_pBuffer + m_lPosition)) = lValue; 
			m_lPosition += sizeof(SHORT);
		}
		//
		inline void WriteBYTE(const BYTE& lValue)
		{
			CheckBufferSize(m_lSizeofBYTE);

			*(m_pBuffer + m_lPosition)	= lValue; 
			m_lPosition += m_lSizeofBYTE;
		}
		inline void WriteLONG(const LONG& lValue)
		{
			CheckBufferSize(m_lSizeofLONG);

			*((LONG*)(m_pBuffer + m_lPosition))	= lValue; 
			m_lPosition += m_lSizeofLONG;
		}
		inline void WriteUSHORT(const USHORT& lValue)
		{
			CheckBufferSize(sizeof(USHORT));

			*((USHORT*)(m_pBuffer + m_lPosition)) = lValue; 
			m_lPosition += sizeof(USHORT);
		}
		inline void WriteWCHAR(const WCHAR& lValue)
		{
			CheckBufferSize(sizeof(WCHAR));

			*((WCHAR*)(m_pBuffer + m_lPosition))	= lValue; 
			m_lPosition += sizeof(WCHAR);
		}
		inline void WriteDouble(const double& dValue)
		{
			// здесь никаких даблов. Сплошные округления
			LONG lValue = (LONG)(dValue * 10000);
			WriteLONG(lValue);
			return;

			CheckBufferSize(m_lSizeofDouble);

			*((double*)(m_pBuffer + m_lPosition)) = dValue; 
			m_lPosition += m_lSizeofDouble;
		}
		inline void WriteDouble2(const double& dValue)
		{
			// здесь никаких даблов. Сплошные округления
			SHORT lValue = (SHORT)(dValue * 100);
			CheckBufferSize(sizeof(SHORT));

			*((SHORT*)(m_pBuffer + m_lPosition)) = lValue; 
			m_lPosition += sizeof(SHORT);
		}
		inline void WriteFloat(const float& fValue)
		{
			CheckBufferSize(m_lSizeofFloat);

			*((float*)(m_pBuffer + m_lPosition))	= fValue; 
			m_lPosition += m_lSizeofFloat;
		}
		inline void WriteBSTR(BSTR bstrValue)
		{
			LONG lSize = __wstrlen(bstrValue) + 1;

			LONG lSizeMem = lSize * sizeof(wchar_t);

			CheckBufferSize(m_lSizeofLONG + lSizeMem);

			*((LONG*)(m_pBuffer + m_lPosition))	= lSizeMem;
			m_lPosition += m_lSizeofLONG;

			memcpy(m_pBuffer + m_lPosition, bstrValue, lSizeMem);
			m_lPosition += lSizeMem;
		}

		inline void WriteBYTE(const CommandType& eType, const BYTE& lValue)
		{
			CheckBufferSize(2 * m_lSizeofBYTE);

			*(m_pBuffer + m_lPosition)	= (BYTE)eType; 
			m_lPosition += m_lSizeofBYTE;

			*(m_pBuffer + m_lPosition)	= lValue; 
			m_lPosition += m_lSizeofBYTE;
		}
		inline void WriteLONG(const CommandType& eType, const LONG& lValue)
		{
			CheckBufferSize(m_lSizeofBYTE + m_lSizeofLONG);

			*(m_pBuffer + m_lPosition)	= (BYTE)eType; 
			m_lPosition += m_lSizeofBYTE;

			*((LONG*)(m_pBuffer + m_lPosition))	= lValue; 
			m_lPosition += m_lSizeofLONG;
		}
		inline void WriteDouble(const CommandType& eType, const double& dValue)
		{
			CheckBufferSize(m_lSizeofBYTE + m_lSizeofDouble);

			*(m_pBuffer + m_lPosition)	= (BYTE)eType; 
			m_lPosition += m_lSizeofBYTE;

			*((double*)(m_pBuffer + m_lPosition)) = dValue; 
			m_lPosition += m_lSizeofDouble;
		}
		inline void WriteFloat(const CommandType& eType, const float& fValue)
		{
			CheckBufferSize(m_lSizeofBYTE + m_lSizeofFloat);

			*(m_pBuffer + m_lPosition)	= (BYTE)eType; 
			m_lPosition += m_lSizeofBYTE;

			*((float*)(m_pBuffer + m_lPosition))	= fValue; 
			m_lPosition += m_lSizeofFloat;
		}
		inline void WriteBSTR(const CommandType& eType, BSTR bstrValue)
		{
			LONG lSize = __wstrlen(bstrValue) + 1;

			LONG lSizeMem = lSize * sizeof(wchar_t);

			CheckBufferSize(m_lSizeofBYTE + m_lSizeofLONG + lSizeMem);

			*(m_pBuffer + m_lPosition)	= (BYTE)eType; 
			m_lPosition += m_lSizeofBYTE;

			*((LONG*)(m_pBuffer + m_lPosition))	= lSizeMem;
			m_lPosition += m_lSizeofLONG;

			memcpy(m_pBuffer + m_lPosition, bstrValue, lSizeMem);
			m_lPosition += lSizeMem;
		}

		inline void Write(const BYTE* pData, const LONG& lLen)
		{
			CheckBufferSize((size_t)lLen);
			memcpy(m_pBuffer + m_lPosition, pData, lLen);
			m_lPosition += lLen;
		}

		inline void Write(const CommandType& eCommand, const double& f1, const double& f2)
		{
			size_t lMem = m_lSizeofBYTE + 2 * m_lSizeofDouble;

			CheckBufferSize(lMem);

			*(m_pBuffer + m_lPosition)			= (BYTE)eCommand;	m_lPosition += m_lSizeofBYTE;
			*((double*)(m_pBuffer + m_lPosition))= f1;				m_lPosition += m_lSizeofDouble;
			*((double*)(m_pBuffer + m_lPosition))= f2;				m_lPosition += m_lSizeofDouble;
		}

		inline void Write(const CommandType& eCommand, const double& f1, const double& f2, const double& f3, const double& f4, const double& f5, const double& f6)
		{
			size_t lMem = m_lSizeofBYTE + 6 * m_lSizeofDouble;

			CheckBufferSize(lMem);

			*(m_pBuffer + m_lPosition)	= (BYTE)eCommand;	m_lPosition += m_lSizeofBYTE;
			*((double*)(m_pBuffer + m_lPosition))= f1;		m_lPosition += m_lSizeofDouble;
			*((double*)(m_pBuffer + m_lPosition))= f2;		m_lPosition += m_lSizeofDouble;
			*((double*)(m_pBuffer + m_lPosition))= f3;		m_lPosition += m_lSizeofDouble;
			*((double*)(m_pBuffer + m_lPosition))= f4;		m_lPosition += m_lSizeofDouble;
			*((double*)(m_pBuffer + m_lPosition))= f5;		m_lPosition += m_lSizeofDouble;
			*((double*)(m_pBuffer + m_lPosition))= f6;		m_lPosition += m_lSizeofDouble;
		}

		inline void Write(const CommandType& eCommand, const LONG& lCount, float* pData)
		{
			size_t lFloats = lCount * m_lSizeofFloat;
			size_t lMem = m_lSizeofBYTE + m_lSizeofLONG + lFloats;

			CheckBufferSize(lMem);

			*(m_pBuffer + m_lPosition)	= (BYTE)eCommand;	m_lPosition += m_lSizeofBYTE;
			*((LONG*)(m_pBuffer + m_lPosition))	= lCount;	m_lPosition += m_lSizeofLONG;

			memcpy(m_pBuffer + m_lPosition, pData, lFloats);
			m_lPosition += lFloats;
		}
		inline void Write(const CommandType& eCommand, const LONG& lCount, double* pData)
		{
			size_t lFloats = lCount * m_lSizeofDouble;
			size_t lMem = m_lSizeofBYTE + m_lSizeofLONG + lFloats;

			CheckBufferSize(lMem);

			*(m_pBuffer + m_lPosition)	= (BYTE)eCommand;	m_lPosition += m_lSizeofBYTE;
			*((LONG*)(m_pBuffer + m_lPosition))	= lCount;	m_lPosition += m_lSizeofLONG;

			memcpy(m_pBuffer + m_lPosition, pData, lFloats);
			m_lPosition += lFloats;
		}

		inline void Write(CMetafile& oMeta)
		{
			LONG lPos = oMeta.GetPosition();
			CheckBufferSize(lPos);

			memcpy(m_pBuffer + m_lPosition, oMeta.GetData(), lPos);
			m_lPosition += lPos;
		}
	};

	class CPageMeta
	{
	public:
		double m_dWidth;
		double m_dHeight;
		LONG m_lStart;
		LONG m_lEnd;

	public:
		CPageMeta()
		{
			m_dWidth = 0;
			m_dHeight = 0;
			m_lStart = 0;
			m_lEnd = 0;
		}
		CPageMeta(const CPageMeta& oSrc)
		{
			*this = oSrc;
		}

		CPageMeta& operator=(const CPageMeta& oSrc)
		{
			m_dWidth = oSrc.m_dWidth;
			m_dHeight = oSrc.m_dHeight;
			m_lStart = oSrc.m_lStart;
			m_lEnd = oSrc.m_lEnd;

			return *this;
		}
	};

	class CHFontInfo
	{
	public:
		USHORT m_lAscent;
		USHORT m_lDescent;
		USHORT m_lLineHeight;
		USHORT m_lUnitsPerEm;

	public:
		CHFontInfo()
		{
			m_lAscent		= 0;
			m_lDescent		= 0;
			m_lLineHeight	= 0;
			m_lUnitsPerEm	= 0;
		}

		CHFontInfo(const CHFontInfo& oSrc)
		{
			m_lAscent		= oSrc.m_lAscent;
			m_lDescent		= oSrc.m_lDescent;
			m_lLineHeight	= oSrc.m_lLineHeight;
			m_lUnitsPerEm	= oSrc.m_lUnitsPerEm;
		}

		CHFontInfo& operator=(const CHFontInfo& oSrc)
		{
			m_lAscent		= oSrc.m_lAscent;
			m_lDescent		= oSrc.m_lDescent;
			m_lLineHeight	= oSrc.m_lLineHeight;
			m_lUnitsPerEm	= oSrc.m_lUnitsPerEm;

			return *this;
		}
	};

	class CFontMapInfo
	{
	public:
		CString Path;
		LONG FaceIndex;

	public:
		CFontMapInfo()
		{
			Path = _T("");
			FaceIndex = 0;
		}
		CFontMapInfo(const CFontMapInfo& oSrc)
		{
			Path = oSrc.Path;
			FaceIndex = oSrc.FaceIndex;
		}
		CFontMapInfo& operator=(const CFontMapInfo& oSrc)
		{
			Path = oSrc.Path;
			FaceIndex = oSrc.FaceIndex;

			return *this;
		}
	};

	class CFontManagerWrapper
	{
	private:
		Graphics::IASCFontManager*	m_pManager;		
	public:
		CHFontInfo m_oCurrentInfo;
		NSStructures::CFont*			m_pFont;
		
		CAtlMap<CString, CHFontInfo> m_mapInfos;
		CAtlMap<CString, CFontMapInfo> m_mapFontFiles;

	public:
		CFontManagerWrapper() : m_mapInfos()
		{
			m_pManager = NULL;
			CoCreateInstance(Graphics::CLSID_CASCFontManager, NULL, CLSCTX_ALL, Graphics::IID_IASCFontManager, (void**)&m_pManager);
		}
		virtual ~CFontManagerWrapper()
		{
			RELEASEINTERFACE(m_pManager);
		}

		void SetStringGID(BOOL bGid)
		{
			if (NULL != m_pManager)
				m_pManager->SetStringGID(bGid);
		}

		void Init(CString strFolderPath)
		{
			if (_T("") == strFolderPath)
			{
				m_pManager->Initialize(L"");
			}
			else
			{
				VARIANT var;
				var.vt = VT_BSTR;
				var.bstrVal = strFolderPath.AllocSysString();
				m_pManager->SetAdditionalParam(L"InitializeFromFolder", var);
				RELEASESYSSTRING(var.bstrVal);
			}

			m_pManager->SetDefaultFont(L"Arial");

			// теперь нужно заполнить мап name->path
			LONG lCount = 0;
			m_pManager->GetWinFontsCount(&lCount);
			for (LONG i = 0; i < lCount; ++i)
			{
				BSTR bsName = NULL;
				BSTR bsPath = NULL;
				BSTR bsStyle = NULL;
				LONG lFaceIndex = 0;
				LONG lBold = 0;
				LONG lItalic = 0;
				
				m_pManager->GetWinFontByIndex(i, &bsName, &bsStyle, &bsPath, &lFaceIndex, &lBold, &lItalic);

				CFontMapInfo oInfo;
				oInfo.Path = (CString)bsPath;
				oInfo.FaceIndex = lFaceIndex;

				m_mapFontFiles.SetAt((CString)bsName, oInfo);

				SysFreeString(bsName);
				SysFreeString(bsPath);
				SysFreeString(bsStyle);
			}
		}

	public:		
		AVSINLINE void LoadCurrentFont(bool bIsAttack, long lFaceIndex = 0)
		{
			if (_T("") == m_pFont->Path)
			{
				CString sStyle = _T("");
				sStyle.Format(_T("__AVS_FONT__%d"), m_pFont->GetStyle());

				CAtlMap<CString, CHFontInfo>::CPair* pPair = m_mapInfos.Lookup(m_pFont->Name + sStyle);
				if (NULL != pPair)
				{
					if (bIsAttack)
					{
						LoadFontByName(m_pFont->Name, m_pFont->Size, m_pFont->GetStyle());
					}
					else
					{
						m_oCurrentInfo = pPair->m_value;
					}
				}
				else
				{
					LoadFontByName(m_pFont->Name, m_pFont->Size, m_pFont->GetStyle());
					m_mapInfos.SetAt(m_pFont->Name + sStyle, m_oCurrentInfo);
				}
			}
			else
			{
				CAtlMap<CString, CHFontInfo>::CPair* pPair = m_mapInfos.Lookup(m_pFont->Path);
				if (NULL != pPair)
				{
					if (bIsAttack)
					{
						LoadFontByFile(m_pFont->Path, m_pFont->Size, lFaceIndex);
					}
					else
					{
						m_oCurrentInfo = pPair->m_value;
					}
				}
				else
				{
					LoadFontByFile(m_pFont->Path, m_pFont->Size, lFaceIndex);
					m_mapInfos.SetAt(m_pFont->Path, m_oCurrentInfo);
				}
			}
		}

		void LoadFontByName(CString& strName, const double& dSize, const LONG& lStyle)
		{
			BSTR bsName = strName.AllocSysString();
			m_pManager->LoadFontByName(bsName, (float)dSize, lStyle, c_dDpi, c_dDpi);
			SysFreeString(bsName);

			LoadFontMetrics();			
		}

		void LoadFontByFile(CString& strPath, const double& dSize, const LONG& lFaceIndex)
		{
			BSTR bsPath = strPath.AllocSysString();
			m_pManager->LoadFontFromFile(bsPath, (float)dSize, c_dDpi, c_dDpi, lFaceIndex);
			SysFreeString(bsPath);
			
			LoadFontMetrics();
		}

	public:
	
		AVSINLINE void MeasureString(const CString& strText, double x, double y, double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight)
		{
			BSTR bsText = strText.AllocSysString();
			MeasureString(bsText, x, y, dBoxX, dBoxY, dBoxWidth, dBoxHeight);
			SysFreeString(bsText);
		}
		AVSINLINE double MeasureString(BSTR bsText, double x, double y, double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight)
		{
			dBoxX		= 0;
			dBoxY		= 0;
			dBoxWidth	= 0;
			dBoxHeight	= 0;

			if (NULL == m_pManager)
				return 0;

			m_pManager->LoadString(bsText, (float)x, (float)y);

			float fx		= 0;
			float fy		= 0;
			float fwidth	= 0;
			float fheight	= 0;

			m_pManager->MeasureString2(&fx, &fy, &fwidth, &fheight);

			dBoxX		= (double)fx;
			dBoxY		= (double)fy;
			dBoxWidth	= (double)fwidth;
			dBoxHeight	= (double)fheight;

			// переводим в миллиметры
			dBoxX		*= c_dPixToMM;
			dBoxY		*= c_dPixToMM;
			dBoxWidth	*= c_dPixToMM;
			dBoxHeight	*= c_dPixToMM;

			return dBoxWidth;
		}

	public:
		void LoadFontMetrics()
		{
			m_pManager->GetCellAscent(&m_oCurrentInfo.m_lAscent);
			m_pManager->GetCellDescent(&m_oCurrentInfo.m_lDescent);
			m_pManager->GetLineSpacing(&m_oCurrentInfo.m_lLineHeight);
			m_pManager->GetEmHeight(&m_oCurrentInfo.m_lUnitsPerEm);
		}
	};

	class CHChar
	{
	public:
		WCHAR	unicode;	// юникодное значение
		USHORT	gid;		// индекс глифа в файле
		double	x;			// сдвиг по baseline
		double	width;		// ширина символа (сдвиг до след буквы)
		double* matrix;		// матрица преобразования (!!! без сдвига)

	public:
		CHChar()
		{
			unicode = (WCHAR)'\0';
			gid = 0;
			width = 0;
			matrix = NULL;
		}
		CHChar(const CHChar& oSrc)
		{
			*this = oSrc;
		}
		CHChar& operator=(const CHChar& oSrc)
		{
			unicode	= oSrc.unicode;
			gid		= oSrc.gid;
			width	= oSrc.width;
			matrix	= NULL;
			if (NULL != oSrc.matrix)
			{
				matrix = new double[4];
				memcpy(matrix, oSrc.matrix, 4 * sizeof(double));
			}
			return *this;
		}
		~CHChar()
		{
			RELEASEARRAYOBJECTS(matrix);
		}

		AVSINLINE void Clear()
		{
			unicode = (WCHAR)'\0';
			gid = 0;
			width = 0;
			
			RELEASEARRAYOBJECTS(matrix);
		}
	};

	class CHLine
	{
	public:
		double m_dAscent;
		double m_dDescent;
		double m_dX;
		double m_dY;

		double m_dEndX;
		double m_dEndY;

		// baseline ruler: y = k*x + b
		double m_dK;
		double m_dB;
		double m_ex;
		double m_ey;
		bool m_bIsConstX;

		// symbols 
		// не менять на списки. постоянное создание объектов и их удаление -
		// плохо влияет на скорость
		CHChar*	m_pChars;
		LONG m_lSizeChars;
		LONG m_lCharsTail;

		bool m_bIsSetUpTransform;
		double m_sx;
		double m_sy;
		double m_shx;
		double m_shy;

	public:
		CHLine()
		{
			m_dAscent	= 0;
			m_dDescent	= 0;
			m_dX		= 0;
			m_dY		= 0;

			m_dK		= 0;
			m_dB		= 0;
			m_bIsConstX	= false;

			m_ex		= 0;
			m_ey		= 0;

			m_lSizeChars = 1000;
			m_lCharsTail = 0;
			m_pChars = new CHChar[m_lSizeChars];

			m_bIsSetUpTransform = false;
			m_sx = 1;
			m_sy = 1;
			m_shx = 0;
			m_shy = 0;
		}
		CHLine& operator=(const CHLine& oLine)
		{
			m_dAscent	= oLine.m_dAscent;
			m_dDescent	= oLine.m_dDescent;
			m_dX		= oLine.m_dX;
			m_dY		= oLine.m_dY;

			m_dK		= oLine.m_dK;
			m_dB		= oLine.m_dB;

			m_lSizeChars = oLine.m_lSizeChars;
			m_lCharsTail = oLine.m_lCharsTail;

			RELEASEARRAYOBJECTS(m_pChars);
			m_pChars = new CHChar[m_lSizeChars];
			
			for (LONG i = 0; i < m_lSizeChars; ++i)
				m_pChars[i] = oLine.m_pChars[i];

			m_bIsSetUpTransform = oLine.m_bIsSetUpTransform;
			m_sx = oLine.m_sx;
			m_sy = oLine.m_sy;
			m_shx = oLine.m_shx;
			m_shy = oLine.m_shy;

			return *this;
		}
		~CHLine()
		{
			RELEASEARRAYOBJECTS(m_pChars);
		}

		AVSINLINE void Clear()
		{
			m_dAscent	= 0;
			m_dDescent	= 0;
			m_dX		= 0;
			m_dY		= 0;

			m_dK		= 0;
			m_dB		= 0;
			m_bIsConstX	= false;

			m_ex		= 0;
			m_ey		= 0;

			m_lCharsTail = 0;

			m_bIsSetUpTransform = false;
			m_sx = 1;
			m_sy = 1;
			m_shx = 0;
			m_shy = 0;
		}

		AVSINLINE CHChar* AddTail()
		{
			if (m_lCharsTail >= m_lSizeChars)
			{
				CHChar* pNews = new CHChar[2 * m_lSizeChars];
				for (LONG i = 0; i < m_lSizeChars; ++i)
				{
					pNews[i] = m_pChars[i];
				}

				RELEASEARRAYOBJECTS(m_pChars);
				m_pChars = pNews;
				m_lSizeChars *= 2;
			}

			CHChar* pChar = &m_pChars[m_lCharsTail];
			++m_lCharsTail;
			pChar->Clear();

			return pChar;
		}

		AVSINLINE CHChar* GetTail()
		{
			if (0 == m_lCharsTail)
				return NULL;

			return &m_pChars[m_lCharsTail - 1];
		}

		AVSINLINE LONG GetCountChars()
		{
			return m_lCharsTail;
		}
	};

	const BYTE g_lfHorizontal	= 0x01;
	const BYTE g_lfGids			= 0x02;
	const BYTE g_lfWidth		= 0x04;

	class CHText
	{
	public:
		CFontManagerWrapper	m_oFontManager;

		CHLine m_oLine;
		CMetafile* m_pMeta;

		NSStructures::CBrush*	m_pBrush;
		NSStructures::CFont*	m_pFont;

		NSStructures::CBrush*	m_pLastBrush;
		NSStructures::CFont*	m_pLastFont;

		CMatrix*				m_pTransform;
		CMatrix*				m_pLastTransform;

		CMetafile				m_oMeta;
		CMetafile*				m_pPageMeta;

		double					m_dTextSpaceEps;

		LONG m_lCountParagraphs;
		LONG m_lCountWords;
		LONG m_lCountSymbols;
		LONG m_lCountSpaces;

		BSTR m_bsOneSymbol;
	
	public:
		CHText() : m_oFontManager(), m_oLine()
 		{
			m_dTextSpaceEps = 0.1;

			m_lCountParagraphs = 0;
			m_lCountWords = 0;
			m_lCountSymbols = 0;
			m_lCountSpaces = 0;

			m_bsOneSymbol = NULL;
			CString strT = _T(" ");
			m_bsOneSymbol = strT.AllocSysString();
		}

		template<typename T>
		void SetParams(T writer)
		{
			m_oFontManager.m_pFont = writer->m_pFont;
			
			m_pBrush	= writer->m_pBrush;
			m_pFont		= writer->m_pFont;

			m_pLastBrush	= &writer->m_oLastBrush;
			m_pLastFont		= &writer->m_oLastFont;

			m_pTransform		= writer->m_pTransform;
			m_pLastTransform	= &writer->m_oLastTransform;

			m_pPageMeta			= &writer->m_oPage;

			switch (writer->m_lSrcFileType)
			{
			case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:
			case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS:
				break;
			default:
				m_dTextSpaceEps = 0.1;
				break;
			}
		}

		~CHText()
		{
			SysFreeString(m_bsOneSymbol);
		}

		void NewPage()
		{
			m_oMeta.ClearNoAttack();
		}
		void ClosePage()
		{
			LONG nCount = m_oLine.GetCountChars();
			if (0 != nCount)
				DumpLine();
		}
	
	public:
		template<typename T>
		void CommandText(BSTR& bsText, BSTR& bsGid, double& x, double& y, double& width, double& height, bool bIsDumpFont, T writer)
		{
			// 1) сначала определяем точку отсчета и направление baseline
			double _x1 = x;
			double _y1 = y;
			double _x2 = x + 1;
			double _y2 = y;
			m_pTransform->TransformPoint(_x1, _y1);
			m_pTransform->TransformPoint(_x2, _y2);			

			double _k = 0;
			double _b = 0;
			bool _isConstX = false;
			if (fabs(_x1 - _x2) < 0.001)
			{
				_isConstX = true;
				_b = _x1;
			}
			else
			{
				_k = (_y1 - _y2) / (_x1 - _x2);
				_b = _y1 - _k * _x1;
			}

			double dAbsVec = sqrt((_x1 - _x2) * (_x1 - _x2) + (_y1 - _y2) * (_y1 - _y2));
			if (dAbsVec == 0)
				dAbsVec = 1;

			LONG nCountChars = m_oLine.GetCountChars();

			bool bIsNewLine = true;

			if (0 != nCountChars)
			{
				if (_isConstX && m_oLine.m_bIsConstX && fabs(_b - m_oLine.m_dB) < 0.001)
					bIsNewLine = false;
				else if (!_isConstX && !m_oLine.m_bIsConstX && fabs(_k - m_oLine.m_dK) < 0.001 && fabs(_b - m_oLine.m_dB) < 0.001)
					bIsNewLine = false;
			}
			
			if (bIsNewLine && (0 != nCountChars))
			{
				// не совпала baseline. поэтому просто скидываем линию в поток
				DumpLine();
			}

			// теперь нужно определить сдвиг по baseline относительно destination точки
			nCountChars = m_oLine.GetCountChars();
			double dOffsetX = 0;
			if (0 == nCountChars)
			{
				m_oLine.m_bIsConstX = _isConstX;
				m_oLine.m_dK = _k;
				m_oLine.m_dB = _b;

				m_oLine.m_dX = _x1;
				m_oLine.m_dY = _y1;

				m_oLine.m_ex = (_x2 - _x1) / dAbsVec;
				m_oLine.m_ey = (_y2 - _y1) / dAbsVec;

				m_oLine.m_dEndX = _x1;
				m_oLine.m_dEndY = _y1;
			}
			else
			{
				double sx = _x1 - m_oLine.m_dEndX;
				double sy = _y1 - m_oLine.m_dEndY;
				double len = sqrt(sx*sx + sy*sy);				

				if (sx*m_oLine.m_ex >= 0 && sy*m_oLine.m_ey >= 0)
				{
					// продолжаем линию
					dOffsetX = len;

					// теперь посмотрим, может быть нужно вставить пробел??
					CHChar* pLastChar = m_oLine.GetTail();
					if (dOffsetX > (pLastChar->width + 0.5))
					{
						// вставляем пробел. Пробел у нас будет не совсем пробел. А специфический
						CHChar* pSpaceChar = m_oLine.AddTail();
						pSpaceChar->x = pLastChar->width;
						pSpaceChar->width = dOffsetX - pLastChar->width;
						pSpaceChar->unicode = 0xFFFF;
						pSpaceChar->gid = 0xFFFF;
						dOffsetX -= pLastChar->width;

						m_oMeta.WriteBYTE(0);
					}
				}
				else
				{
					// буква сдвинута влево относительно предыдущей буквы
					// на такую ситуацию реагируем просто - просто начинаем новую линию, 
					// предварительно сбросив старую
					DumpLine();

					m_oLine.m_bIsConstX = _isConstX;

					m_oLine.m_dX = _x1;
					m_oLine.m_dY = _y1;

					m_oLine.m_dK = _k;
					m_oLine.m_dB = _b;

					m_oLine.m_ex = (_x2 - _x1) / dAbsVec;
					m_oLine.m_ey = (_y2 - _y1) / dAbsVec;
				}

				m_oLine.m_dEndX = _x1;
				m_oLine.m_dEndY = _y1;
			}

			// смотрим, совпадает ли главная часть матрицы.
			bool bIsTransform = !m_pLastTransform->IsEqualMain(m_pTransform);
			if (bIsTransform)
				bIsDumpFont = true;

			bool bIsColor = ((m_pBrush->Color1 != m_pLastBrush->Color1) || (m_pBrush->Alpha1 != m_pLastBrush->Alpha1));
			
			BYTE nLenMetaCommands = 0;
			if (bIsColor)
				nLenMetaCommands += 5;
			if (bIsTransform)
				nLenMetaCommands += 17;
			if (bIsDumpFont)
				nLenMetaCommands += 13;

			m_oMeta.WriteBYTE(nLenMetaCommands);

			agg::trans_affine* _src = &m_pTransform->m_agg_mtx;
			
			double _dumpSize = writer->m_dCurrentFontSize;
			double _dumpMtx[4];
			_dumpMtx[0] = _src->sx;
			_dumpMtx[1] = _src->shy;
			_dumpMtx[2] = _src->shx;
			_dumpMtx[3] = _src->sy;		

			double dTextScale = min( sqrt( _dumpMtx[2] * _dumpMtx[2] + _dumpMtx[3] * _dumpMtx[3] ), sqrt( _dumpMtx[0] * _dumpMtx[0] + _dumpMtx[1] * _dumpMtx[1] ) );

			if ((_dumpSize < 0.1 && dTextScale > 10) || (_dumpSize > 10 && dTextScale < 0.1))
			{
				_dumpSize *= dTextScale;

				_dumpMtx[0] /= dTextScale;
				_dumpMtx[1] /= dTextScale;
				_dumpMtx[2] /= dTextScale;
				_dumpMtx[3] /= dTextScale;
			}

			if (bIsDumpFont)
			{
				m_oMeta.WriteCommandType(CMetafile::ctFontName);
				m_oMeta.WriteLONG(writer->m_lCurrentFont);
				m_oMeta.WriteLONG(writer->m_pFont->GetStyle());
				m_oMeta.WriteDouble(_dumpSize/*writer->m_dCurrentFontSize*/);
			}
			if (bIsTransform)
			{
				agg::trans_affine* _dst = &m_pLastTransform->m_agg_mtx;

				_dst->sx = _src->sx;
				_dst->shx = _src->shx;
				_dst->shy = _src->shy;
				_dst->sy = _src->sy;

				m_oLine.m_bIsSetUpTransform = true;
				m_oLine.m_sx = _src->sx;
				m_oLine.m_shx = _src->shx;
				m_oLine.m_shy = _src->shy;
				m_oLine.m_sy = _src->sy;

				m_oMeta.WriteBYTE(CMetafile::ctCommandTextTransform);
				//m_oMeta.WriteDouble(_dst->sx);
				//m_oMeta.WriteDouble(_dst->shy);
				//m_oMeta.WriteDouble(_dst->shx);
				//m_oMeta.WriteDouble(_dst->sy);

				m_oMeta.WriteDouble(_dumpMtx[0]);
				m_oMeta.WriteDouble(_dumpMtx[1]);
				m_oMeta.WriteDouble(_dumpMtx[2]);
				m_oMeta.WriteDouble(_dumpMtx[3]);
			}
			if (bIsColor)
			{
				m_pLastBrush->Color1 = m_pBrush->Color1;
				m_pLastBrush->Alpha1 = m_pBrush->Alpha1;

				m_oMeta.WriteBYTE(CMetafile::ctBrushColor1);
				
				LONG lBGR = m_pBrush->Color1;
				m_oMeta.WriteBYTE((BYTE)(lBGR & 0xFF));
				m_oMeta.WriteBYTE((BYTE)((lBGR >> 8) & 0xFF));
				m_oMeta.WriteBYTE((BYTE)((lBGR >> 16) & 0xFF));
				m_oMeta.WriteBYTE((BYTE)m_pBrush->Alpha1);
			}			
			
			// все, baseline установлен. теперь просто продолжаем линию
			LONG lTextLen = __wstrlen(bsText);
			bool bIsLoadFontAttack = true;
			
			// говенные значения приходят из пдф
			/*
			if (1 == lTextLen && 0 <= width)
				bIsLoadFontAttack = false;
			*/

			if (bIsDumpFont)
				m_oFontManager.LoadCurrentFont(bIsLoadFontAttack);

			double dKoef = m_oFontManager.m_pFont->Size * 25.4 / (72 * m_oFontManager.m_oCurrentInfo.m_lUnitsPerEm);
			double dKoefMetr = dAbsVec;
			double dAscender = m_oFontManager.m_oCurrentInfo.m_lAscent * dKoef * dKoefMetr;
			double dDescender = m_oFontManager.m_oCurrentInfo.m_lDescent * dKoef * dKoefMetr;

			if (m_oLine.m_dAscent < dAscender)
				m_oLine.m_dAscent = dAscender;
			if (m_oLine.m_dDescent < dDescender)
				m_oLine.m_dDescent = dDescender;

			if (!bIsLoadFontAttack)
			{
				CHChar* pChar = m_oLine.AddTail();

				pChar->unicode = bsText[0];
				pChar->gid = (NULL == bsGid) ? 0xFFFF : bsGid[0];
				pChar->width = width;
				pChar->x = dOffsetX;
			}
			else
			{
				double dPlusOffset = 0;

				BSTR input = NULL;
				if (NULL != bsGid)
				{
					input = bsGid;
					m_oFontManager.SetStringGID(TRUE);
				}
				else
				{
					input = bsText;
					m_oFontManager.SetStringGID(FALSE);
				}

				double dBoxX = 0;
				double dBoxY = 0;
				double dBoxW = 0;
				double dBoxH = 0;

				double dPrevW = dOffsetX;
				for (LONG lIndex = 0; lIndex < lTextLen; ++lIndex)
				{
					m_bsOneSymbol[0] = input[lIndex];
					double dW = m_oFontManager.MeasureString(m_bsOneSymbol, 0, 0, dBoxX, dBoxY, dBoxW, dBoxH);

					CHChar* pChar = m_oLine.AddTail();
					pChar->unicode = bsText[lIndex];
					if (NULL != bsGid)
					{
						pChar->gid = bsGid[0];
					}
					else
						pChar->gid = 0xFFFF;

					pChar->x = dPrevW;
					if (lIndex != 0)
						dPlusOffset += dPrevW;
					dPrevW = dW;

					pChar->width = dW * dAbsVec;

					if (0 != lIndex)
						m_oMeta.WriteBYTE(0);

					if (lIndex == (lTextLen - 1))
					{
						m_oLine.m_dEndX += dPlusOffset * m_oLine.m_ex;
						m_oLine.m_dEndY += dPlusOffset * m_oLine.m_ey;
					}
				}
			}			
		}

		void DumpLine()
		{
			if (m_oLine.m_bIsSetUpTransform)
			{
				// выставится трансформ!!!
				// cравнивать нужно с ним!!!
				agg::trans_affine* _dst = &m_pLastTransform->m_agg_mtx;

				_dst->sx = m_oLine.m_sx;
				_dst->shx = m_oLine.m_shx;
				_dst->shy = m_oLine.m_shy;
				_dst->sy = m_oLine.m_sy;
				_dst->tx = 0;
				_dst->ty = 0;
			}

			// скидываем линию в поток pMeta
			BYTE mask = 0;
			if (fabs(m_oLine.m_ex - 1.0) < 0.001 && fabs(m_oLine.m_ey) < 0.001)
				mask |= g_lfHorizontal;

			LONG lCountSpaces = 0;
			LONG lCountSymbols = 0;
			LONG lCountWords = 0;
			bool bIsLastSymbol = false;

			bool bIsGidExist = false;

			LONG nCount = m_oLine.GetCountChars();
			for (LONG i = 0; i < nCount; ++i)
			{
				CHChar* pChar = &m_oLine.m_pChars[i];
				if (pChar->gid != 0xFFFF)
				{
					mask |= g_lfGids;
					bIsGidExist = true;
				}

				if (0xFFFF == pChar->unicode || ((WCHAR)' ') == pChar->unicode || ((WCHAR)'\t') == pChar->unicode)
				{
					lCountSpaces++;
					if (bIsLastSymbol)
					{
						bIsLastSymbol = false;
						lCountWords++;
					}
				}
				else
				{
					lCountSymbols++;
					bIsLastSymbol = true;
				}
			}

			if (bIsLastSymbol)
				lCountWords++;

			if (0 == nCount)
			{
				m_oLine.Clear();
				m_oMeta.ClearNoAttack();
				return;
			}

			m_lCountParagraphs += 1;
			m_lCountWords += lCountWords;
			m_lCountSymbols += lCountSymbols;
			m_lCountSpaces += lCountSpaces;

			if (nCount > 1)
				mask |= g_lfWidth;

			m_pPageMeta->CheckBufferSize(60);

			m_pPageMeta->WriteBYTE_nocheck(CMetafile::ctCommandTextLine);
			m_pPageMeta->WriteBYTE_nocheck(mask);

			m_pPageMeta->WriteDouble_nocheck(m_oLine.m_dX);
			m_pPageMeta->WriteDouble_nocheck(m_oLine.m_dY);
			
			if ((mask & g_lfHorizontal) == 0)
			{
				m_pPageMeta->WriteDouble_nocheck(m_oLine.m_ex);
				m_pPageMeta->WriteDouble_nocheck(m_oLine.m_ey);
			}
			
			m_pPageMeta->WriteDouble_nocheck(m_oLine.m_dAscent);
			m_pPageMeta->WriteDouble_nocheck(m_oLine.m_dDescent);

			LONG _position = 0;
			if (nCount > 1)
			{
				_position = m_pPageMeta->GetPosition();
				m_pPageMeta->WriteLONG_nocheck(0);
			}

			BYTE* pBufferMeta = m_oMeta.GetData();
			double dWidthLine = 0;

			double dCurrentGlyphLineOffset = 0;
			for (LONG lIndexChar = 0; lIndexChar < nCount; ++lIndexChar)
			{
				CHChar* pChar = &m_oLine.m_pChars[lIndexChar];

				// все настроки буквы (m_oMeta)
				BYTE lLen = *pBufferMeta;
				++pBufferMeta;
				if (lLen > 0)
				{
					m_pPageMeta->Write(pBufferMeta, lLen);
				}
				pBufferMeta += lLen;
				// смещение относительно предыдущей буквы (у всех, кроме первой)
				// юникодное значение
				// гид (если bIsGidExist == true)
				// ширина буквы

				m_pPageMeta->CheckBufferSize(20);

				m_pPageMeta->WriteBYTE_nocheck(CMetafile::ctDrawText);
				if (0 != lIndexChar)
				{
					m_pPageMeta->WriteDouble2_nocheck(pChar->x);
				}

				m_pPageMeta->WriteWCHAR_nocheck(pChar->unicode);
				if (bIsGidExist)
					m_pPageMeta->WriteUSHORT_nocheck(pChar->gid);
				m_pPageMeta->WriteDouble2_nocheck(pChar->width);

				if (lIndexChar != 0)
					dCurrentGlyphLineOffset += pChar->x;
				
				if (lIndexChar == (nCount - 1))
					dWidthLine = dCurrentGlyphLineOffset + pChar->width;
			}

			if (nCount > 1)
			{
				LONG* pWidthBuf = (LONG*)(m_pPageMeta->GetData() + _position);
				*pWidthBuf = (LONG)(dWidthLine * 10000);
			}
			
			m_oLine.Clear();
			m_oMeta.ClearNoAttack();

			m_pPageMeta->WriteBYTE(CMetafile::ctCommandTextLineEnd);
		}
	};

	using namespace NSStrings;
	class CWriter3
	{
	public:
		CMetafile m_oPage;
		CMetafile m_oVectors;

		CAtlArray<CPageMeta> m_arrPages;		
		bool m_bIsBigPicture;

	public:
		NSStructures::CPen*		m_pPen;
		NSStructures::CBrush*	m_pBrush;
		NSStructures::CFont*	m_pFont;

		NSStructures::CPen		m_oLastPen;
		NSStructures::CBrush	m_oLastBrush;
		NSStructures::CFont		m_oLastFont;

		LONG					m_lCurrentFont;
		double					m_dCurrentFontSize;

		CFontDstGenerator		m_oDstFontGenerator;
		CFontManager			m_oFontManager;

		CHText					m_oSmartText;

		double m_dDpiX;
		double m_dDpiY;

		BOOL					m_bPathClosed;

		CAtlMap<CString, CImageInfo>	m_mapImagesFile;
		CAtlMap<DWORD, CImageInfo>		m_mapImageData;

		CCalculatorCRC32		m_oCRC;

		double m_dWidthDocMM;
		double m_dHeightDocMM;
		double m_dHeightPageMM;

		Graphics::IASCGraphicSimpleComverter* m_pSimpleConverter;
		CFile m_oFileWriter;

		bool m_bIsGids;

	public:

		double					m_dWidth;
		double					m_dHeight;
		CRenderers				m_oRenderers;

		bool m_bIsImageFromVectors;

	private:

		LONG					m_lNextIDShape;
		LONG					m_lNextIDImage;

		LONG					m_lCurrentPage;
		LONG					m_lCurrentFunctionPage;
		LONG					m_lPagesCount;

		LONG					m_lMaxSizeImage;

	public:
		CString					m_strDstDirectory;
		CString					m_strDstMedia;
		CString					m_strDstDirectoryFiles;
		CString					m_strFileName;

		CMatrix*				m_pTransform;
		CMatrix					m_oLastTransform;

		LONG					m_lCurrentDumpSize;

		LONG					m_lSrcFileType;
	public:
		CWriter3()
		{
			m_dDpiX = 96.0;
			m_dDpiY = 96.0;

			m_dWidth	= 0;
			m_dHeight	= 0;

			m_lNextIDImage	= 0;
			m_lNextIDShape	= 0;

			m_lCurrentPage	= -1;

			m_bPathClosed	= TRUE;

			m_pSimpleConverter = NULL;

			m_lMaxSizeImage = 800;

			m_dWidthDocMM	= 0;
			m_dHeightDocMM	= 0;
			m_dHeightPageMM	= 0;

			m_lPagesCount		= 0;

			m_bIsImageFromVectors = false;

			m_lSrcFileType = 0;
		}


		void CreateFile(CString& strDir, CString& strFileName)
		{
			m_lPagesCount		= 0;
			m_lCurrentPage		= -1;

			m_strDstDirectory		= strDir;
			m_strDstDirectoryFiles	= m_strDstDirectory + _T("\\") + strFileName;// + _T("_files");
			m_strFileName			= strFileName;

			CDirectory::CreateDirectory(m_strDstDirectoryFiles);
			m_strDstMedia = m_strDstDirectoryFiles + _T("\\media");
			CDirectory::CreateDirectory(m_strDstMedia);

			CString strFileFonts = m_strDstDirectoryFiles + _T("\\fonts");
			CDirectory::CreateDirectory(strFileFonts);

			CString strDocRendererS = m_strDstDirectoryFiles + _T("\\document_temp.bin");
			m_oFileWriter.CreateFile(strDocRendererS);

			m_oPage.Clear();
			m_arrPages.RemoveAll();
			m_bIsGids = false;
			m_lCurrentDumpSize = 0;

			m_oSmartText.SetParams(this);

			m_oSmartText.m_lCountParagraphs = 0;
			m_oSmartText.m_lCountWords = 0;
			m_oSmartText.m_lCountSymbols = 0;
			m_oSmartText.m_lCountSpaces = 0;
		}

		void SetSimpleConverter(Graphics::IASCGraphicSimpleComverter* pSimpleConverter, CMatrix* pMatrix)
		{
			m_pSimpleConverter = pSimpleConverter;
		}
				
		AVSINLINE void WriteText(BSTR bsText, BSTR bsGid, double x, double y, double width, double height, double baselineoffset)
		{
			if (m_lSrcFileType == AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)
			{
				// не нужно ничего писать из djvu
				return;
			}

			CheckVectorGraphics();

			bool bIsT = true;
			if (NULL == bsText || bsText[0] == 0)
				bIsT = false;
			bool bIsG = true;
			if (NULL == bsGid || bsGid[0] == 0)
				bIsG = false;
			
			if (!bIsT && !bIsG)
				return;

			bool bIsDumpFont = false;
		
			BSTR bsFontsSymbols = (NULL == bsGid) ? bsText : bsGid;
			LONG lCurrentFontIndex = m_oDstFontGenerator.AddFont(m_pFont, &m_oFontManager, bsFontsSymbols);
			if ((lCurrentFontIndex != m_lCurrentFont) || (m_pFont->Size != m_dCurrentFontSize))
			{
				m_lCurrentFont		= lCurrentFontIndex;
				m_dCurrentFontSize	= m_pFont->Size;

				bIsDumpFont = true;				
			}

			if (NULL != bsGid)
				m_bIsGids = true;
			m_oSmartText.CommandText(bsText, bsGid, x, y, width, height, bIsDumpFont, this);
			return;

			#if 0
			if (!m_oLastBrush.IsEqual(m_pBrush))
			{
				m_oLastBrush = *m_pBrush;
				SetFillColor(false);
			}

			LONG lCurrentFontIndex = m_oDstFontGenerator.AddFont(m_pFont, &m_oFontManager, bsText);
			if ((lCurrentFontIndex != m_lCurrentFont) || (m_pFont->Size != m_dCurrentFontSize))
			{
				m_lCurrentFont		= lCurrentFontIndex;
				m_dCurrentFontSize	= m_pFont->Size;
				SetFontStyle();
			}

			double _x = x;
			double _y = y;
			if ((fabs(1 - m_pTransform->m_agg_mtx.sx) < __g_matrix_eps) &&
				(fabs(m_pTransform->m_agg_mtx.shx) < __g_matrix_eps) &&
				(fabs(m_pTransform->m_agg_mtx.shy) < __g_matrix_eps) &&
				(fabs(1 - m_pTransform->m_agg_mtx.sy) < __g_matrix_eps))
			{
				m_pTransform->TransformPoint(_x, _y);
			}

			SetTransformToDocument(false);
			
			if (NULL == bsGid)
			{
				m_oPage.WriteCommandType(CMetafile::ctDrawText);
				m_oPage.WriteWCHAR((WCHAR)-1);
				m_oPage.WriteWCHAR(bsText[0]);
				m_oPage.WriteDouble(_x);
				m_oPage.WriteDouble(_y + baselineoffset);
			}
			else
			{
				m_oPage.WriteCommandType(CMetafile::ctDrawText);
				m_oPage.WriteWCHAR(bsGid[0]);
				m_oPage.WriteWCHAR(bsText[0]);
				m_oPage.WriteDouble(_x);
				m_oPage.WriteDouble(_y + baselineoffset);
			}
			#endif
		}
		AVSINLINE void WriteImage(IUnknown* punkImage, double x, double y, double width, double height, double dAngle)
		{			
			if (m_bIsImageFromVectors || m_lSrcFileType == AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)
			{
				SetTransformToDocument(true);

				if (height < 0)
				{
					FlipY(punkImage);
					height = -height;
					y -= height;
				}
				
				CImageInfo oID = GenerateImageID(punkImage);
				if (oID.m_eType == itJPG)
				{
					m_oPage.WriteCommandType(CMetafile::ctDrawImage);
					m_oPage.WriteBYTE(0);
					m_oPage.WriteLONG(oID.m_lID);
					m_oPage.WriteDouble(x);
					m_oPage.WriteDouble(y);
					m_oPage.WriteDouble(width);
					m_oPage.WriteDouble(height);
				}
				else
				{
					m_oPage.WriteCommandType(CMetafile::ctDrawImage);
					m_oPage.WriteBYTE(1);
					m_oPage.WriteLONG(oID.m_lID);
					m_oPage.WriteDouble(x);
					m_oPage.WriteDouble(y);
					m_oPage.WriteDouble(width);
					m_oPage.WriteDouble(height);
				}
			}
			else
			{
				if (!m_oRenderers.m_bIsNoSimpleAttack)
				{
					CheckVectorGraphics();
					m_oRenderers.CheckNeedInitGraphics();
				}

				m_oRenderers.m_bIsNoSimpleAttack = true;
			}			
		}
		AVSINLINE void WriteImage(CString& strFile, double x, double y, double width, double height, double dAngle)
		{
			if (m_bIsImageFromVectors || m_lSrcFileType == AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)
			{
				SetTransformToDocument(true);

				CImageInfo oID = GenerateImageID(strFile);

				if (oID.m_eType == itJPG)
				{
					m_oPage.WriteCommandType(CMetafile::ctDrawImage);
					m_oPage.WriteBYTE(0);
					m_oPage.WriteLONG(oID.m_lID);
					m_oPage.WriteDouble(x);
					m_oPage.WriteDouble(y);
					m_oPage.WriteDouble(width);
					m_oPage.WriteDouble(height);
				}
				else
				{
					m_oPage.WriteCommandType(CMetafile::ctDrawImage);
					m_oPage.WriteBYTE(1);
					m_oPage.WriteLONG(oID.m_lID);
					m_oPage.WriteDouble(x);
					m_oPage.WriteDouble(y);
					m_oPage.WriteDouble(width);
					m_oPage.WriteDouble(height);
				}
			}
			else
			{
				if (!m_oRenderers.m_bIsNoSimpleAttack)
				{
					CheckVectorGraphics();
					m_oRenderers.CheckNeedInitGraphics();
				}

				m_oRenderers.m_bIsNoSimpleAttack = true;
			}
		}
		
		AVSINLINE void WriteBeginPath()
		{
			if (m_oRenderers.m_bIsClipping)
				return;

			m_oVectors.WriteCommandType(CMetafile::ctPathCommandStart);
		}

		AVSINLINE void WriteEndPath()
		{	
			if (m_oRenderers.m_bIsClipping)
				return;

			m_oVectors.WriteCommandType(CMetafile::ctPathCommandStart);
		}		
		
		AVSINLINE void WritePathStart()
		{			
		}
		AVSINLINE void WritePathClose()
		{
			if (m_oRenderers.m_bIsClipping)
				return;

			m_oVectors.WriteCommandType(CMetafile::ctPathCommandClose);
		}

		AVSINLINE void WritePathMoveTo(double& x, double& y)
		{
			if (m_oRenderers.m_bIsClipping)
				return;

			m_oVectors.WriteCommandType(CMetafile::ctPathCommandMoveTo);
			m_oVectors.WriteDouble(x);
			m_oVectors.WriteDouble(y);
		}
		AVSINLINE void WritePathLineTo(double& x, double& y)
		{
			if (m_oRenderers.m_bIsClipping)
				return;

			m_oVectors.WriteCommandType(CMetafile::ctPathCommandLineTo);
			m_oVectors.WriteDouble(x);
			m_oVectors.WriteDouble(y);
		}
		AVSINLINE void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			if (m_oRenderers.m_bIsClipping)
				return;

			m_oVectors.WriteCommandType(CMetafile::ctPathCommandCurveTo);
			m_oVectors.WriteDouble(x1);
			m_oVectors.WriteDouble(y1);
			m_oVectors.WriteDouble(x2);
			m_oVectors.WriteDouble(y2);
			m_oVectors.WriteDouble(x3);
			m_oVectors.WriteDouble(y3);
		}
		AVSINLINE void WriteDrawPath(LONG lType, const double& dAngle)
		{
			/*
			if (m_oRenderers.m_bIsRaster)
			{
				CheckVectorGraphics();
				m_oRenderers.CheckNeedInitGraphics();
			}
			*/

			m_oRenderers.DrawPath(lType);

			if (lType > 0xFF)
			{
				if (!m_oLastBrush.IsEqual(m_pBrush))
				{
					m_oLastBrush = *m_pBrush;
					SetFillColor(true);
				}
			}
			if (lType & 0x01)
			{
				if (!m_oLastPen.IsEqual(m_pPen))
				{
					m_oLastPen = *m_pPen;
					SetStrokeColor(true);
					SetLineWidth(true);
				}
			}

			m_oVectors.WriteCommandType(CMetafile::ctDrawPath);
			m_oVectors.WriteLONG(lType);
		}

		AVSINLINE void WritePathClip()
		{
		}
		AVSINLINE void WritePathClipEnd()
		{
			m_oRenderers.m_bIsClip = true;
			m_oRenderers.m_bIsSimple = false;
		}		
		AVSINLINE void WritePathResetClip()
		{
			m_oRenderers.m_bIsClip = false;
			m_oRenderers.m_bIsSimple = false;
		}

		AVSINLINE void SetStrokeColor(bool bIsGraphics)
		{
			CMetafile* pMeta = bIsGraphics ? &m_oVectors : &m_oPage;
			
			pMeta->WriteCommandType(CMetafile::ctPenColor);

			LONG lBGR	= m_pPen->Color;

			pMeta->WriteBYTE((BYTE)(lBGR & 0xFF));
			pMeta->WriteBYTE((BYTE)((lBGR >> 8) & 0xFF));
			pMeta->WriteBYTE((BYTE)((lBGR >> 16) & 0xFF));
			pMeta->WriteBYTE((BYTE)m_pPen->Alpha);
		}
		AVSINLINE void SetLineWidth(bool bIsGraphics)
		{
			CMetafile* pMeta = bIsGraphics ? &m_oVectors : &m_oPage;

			pMeta->WriteCommandType(CMetafile::ctPenSize);

			double _x1 = 0;
			double _y1 = 0;
			double _x2 = 1;
			double _y2 = 1;
			m_pTransform->TransformPoint(_x1, _y1);
			m_pTransform->TransformPoint(_x2, _y2);

			double dKoef = sqrt(((_x2 - _x1) * (_x2 - _x1) + (_y2 - _y1) * (_y2 - _y1)) / 2);

			pMeta->WriteDouble(m_pPen->Size * dKoef);
		}
		AVSINLINE void SetFillColor(bool bIsGraphics)
		{
			CMetafile* pMeta = bIsGraphics ? &m_oVectors : &m_oPage;

			pMeta->WriteCommandType(CMetafile::ctBrushColor1);

			LONG lBGR	= m_pBrush->Color1;

			pMeta->WriteBYTE((BYTE)(lBGR & 0xFF));
			pMeta->WriteBYTE((BYTE)((lBGR >> 8) & 0xFF));
			pMeta->WriteBYTE((BYTE)((lBGR >> 16) & 0xFF));
			pMeta->WriteBYTE((BYTE)m_pBrush->Alpha1);
		}
		AVSINLINE void SetFontStyle()
		{
			m_oPage.WriteCommandType(CMetafile::ctFontName);
			m_oPage.WriteLONG(m_lCurrentFont);
			m_oPage.WriteLONG(m_pFont->GetStyle());
			m_oPage.WriteDouble(m_dCurrentFontSize);
		}
		AVSINLINE void SetTransformToDocument(bool bIsFromGraphics)
		{
			if (NULL == m_pTransform)
				return;

			if (bIsFromGraphics)
			{
				if ((fabs(1 - m_oLastTransform.m_agg_mtx.sx) < __g_matrix_eps) &&
					(fabs(0 - m_oLastTransform.m_agg_mtx.shx) < __g_matrix_eps) &&
					(fabs(0 - m_oLastTransform.m_agg_mtx.shy) < __g_matrix_eps) &&
					(fabs(1 - m_oLastTransform.m_agg_mtx.sy) < __g_matrix_eps))
				{
					// ничего делать не нужно
				}
				else
				{
					m_oPage.WriteCommandType(CMetafile::ctResetTransform);
					m_oLastTransform.Reset();
				}
			}
			else
			{
				if (true)
				{
					m_oLastTransform = *m_pTransform;

					agg::trans_affine* m = &m_pTransform->m_agg_mtx;
					//m_oPage.Write(CMetafile::ctSetTransform, m->sx, m->shy, m->shx, m->sy, m->tx, m->ty);
					m_oPage.WriteCommandType(CMetafile::ctSetTransform);
					m_oPage.WriteDouble(m->sx);
					m_oPage.WriteDouble(m->shy);
					m_oPage.WriteDouble(m->shx);
					m_oPage.WriteDouble(m->sy);
					m_oPage.WriteDouble(m->tx);
					m_oPage.WriteDouble(m->ty);
					return;
				}

				if ((fabs(m_pTransform->m_agg_mtx.sx - m_oLastTransform.m_agg_mtx.sx) < __g_matrix_eps) &&
					(fabs(m_pTransform->m_agg_mtx.shx - m_oLastTransform.m_agg_mtx.shx) < __g_matrix_eps) &&
					(fabs(m_pTransform->m_agg_mtx.shy - m_oLastTransform.m_agg_mtx.shy) < __g_matrix_eps) &&
					(fabs(m_pTransform->m_agg_mtx.sy - m_oLastTransform.m_agg_mtx.sy) < __g_matrix_eps) &&
					(fabs(m_pTransform->m_agg_mtx.tx - m_oLastTransform.m_agg_mtx.tx) < __g_matrix_eps) &&
					(fabs(m_pTransform->m_agg_mtx.ty - m_oLastTransform.m_agg_mtx.ty) < __g_matrix_eps))
				{
					// ничего делать не нужно
					return;
				}

				if ((fabs(1 - m_pTransform->m_agg_mtx.sx) < __g_matrix_eps) &&
					(fabs(0 - m_pTransform->m_agg_mtx.shx) < __g_matrix_eps) &&
					(fabs(0 - m_pTransform->m_agg_mtx.shy) < __g_matrix_eps) &&
					(fabs(1 - m_pTransform->m_agg_mtx.sy) < __g_matrix_eps) &&
					(fabs(1 - m_oLastTransform.m_agg_mtx.sx) < __g_matrix_eps) &&
					(fabs(0 - m_oLastTransform.m_agg_mtx.shx) < __g_matrix_eps) &&
					(fabs(0 - m_oLastTransform.m_agg_mtx.shy) < __g_matrix_eps) &&
					(fabs(1 - m_oLastTransform.m_agg_mtx.sy) < __g_matrix_eps))
				{
					// ничего делать не нужно
					m_oLastTransform.Reset();
				}
				else
				{
					if ((fabs(1 - m_pTransform->m_agg_mtx.sx) < __g_matrix_eps) &&
						(fabs(0 - m_pTransform->m_agg_mtx.shx) < __g_matrix_eps) &&
						(fabs(0 - m_pTransform->m_agg_mtx.shy) < __g_matrix_eps) &&
						(fabs(1 - m_pTransform->m_agg_mtx.sy) < __g_matrix_eps))
					{
						// ничего делать не нужно
						m_oPage.WriteCommandType(CMetafile::ctResetTransform);
						m_oLastTransform.Reset();
						return;
					}

					m_oLastTransform = *m_pTransform;

					agg::trans_affine* m = &m_pTransform->m_agg_mtx;
					//m_oPage.Write(CMetafile::ctSetTransform, m->sx, m->shy, m->shx, m->sy, m->tx, m->ty);
					m_oPage.WriteCommandType(CMetafile::ctSetTransform);
					m_oPage.WriteDouble(m->sx);
					m_oPage.WriteDouble(m->shy);
					m_oPage.WriteDouble(m->shx);
					m_oPage.WriteDouble(m->sy);
					m_oPage.WriteDouble(m->tx);
					m_oPage.WriteDouble(m->ty);
				}
			}
		}

		void NewPage(const double& dWidthMM, const double& dHeightMM)
		{	
			++m_lPagesCount;

			CPageMeta oInfo;
			oInfo.m_dWidth = dWidthMM;
			oInfo.m_dHeight = dHeightMM;
			oInfo.m_lStart = m_lCurrentDumpSize;
			oInfo.m_lEnd = 0;

			m_dWidth = dWidthMM;
			m_dHeight = dHeightMM;

			m_arrPages.Add(oInfo);			

			m_oLastBrush.Color1 = -1;
			m_oLastPen.Color	= -1;
			m_oLastFont.Name	= _T("");

			m_lCurrentFont		= -1;
			m_dCurrentFontSize	= 0.0;

			m_oLastTransform.Reset();
			m_pTransform->Reset();

			m_bIsBigPicture = false;
			m_oRenderers.NewPage(dWidthMM, dHeightMM);
		}
		void EndPage()
		{
			m_oSmartText.ClosePage();
			CheckVectorGraphics();

			m_arrPages[m_arrPages.GetCount() - 1].m_lEnd = m_lCurrentDumpSize + m_oPage.GetPosition();
			m_lCurrentDumpSize += m_oPage.GetPosition();
		
			m_oFileWriter.WriteFile((void*)m_oPage.GetData(), m_oPage.GetPosition());
			m_oPage.Clear();
		}

		void WriteStartDocument()
		{
			m_bIsGids = false;
		}
		
		void WriteEndDocument(CDocument& oDocument)
		{
			CMetafile oDocInfo;
			oDocInfo.WriteLONG(m_lPagesCount);
			oDocInfo.WriteLONG(m_oSmartText.m_lCountParagraphs);
			oDocInfo.WriteLONG(m_oSmartText.m_lCountWords);
			oDocInfo.WriteLONG(m_oSmartText.m_lCountSymbols);
			oDocInfo.WriteLONG(m_oSmartText.m_lCountSpaces);

			oDocInfo.WriteLONG(m_oDstFontGenerator.m_lCountFonts);

			LONG lOffset = (6 + 4 * m_lPagesCount) * sizeof(LONG);
			for (LONG i = 0; i < m_lPagesCount; i++)
			{
				CPageMeta& oMeta = m_arrPages[i];
				oDocInfo.WriteDouble(oMeta.m_dWidth);
				oDocInfo.WriteDouble(oMeta.m_dHeight);
				oDocInfo.WriteLONG(oMeta.m_lStart + lOffset);
				oDocInfo.WriteLONG(oMeta.m_lEnd + lOffset);
			}

			m_oFileWriter.CloseFile();

			CFile oFilePages;
			oFilePages.OpenFile(m_strDstDirectoryFiles + _T("\\document_temp.bin"));

			LONG lMetaSize = (LONG)oFilePages.GetFileSize();
			LONG lSizeAll = oDocInfo.GetPosition() + lMetaSize;

			BYTE* pData = new BYTE[lSizeAll];
			memcpy(pData, oDocInfo.GetData(), oDocInfo.GetPosition());
			
			oFilePages.ReadFile(pData + oDocInfo.GetPosition(), lMetaSize);

			oFilePages.CloseFile();
			::DeleteFile(m_strDstDirectoryFiles + _T("\\document_temp.bin"));

			int nOutputLen = Base64EncodeGetRequiredLength(lSizeAll, ATL_BASE64_FLAG_NOCRLF);
			BYTE* pOutput = new BYTE[nOutputLen];
			Base64Encode(pData, lSizeAll, (LPSTR)pOutput, &nOutputLen, ATL_BASE64_FLAG_NOCRLF);
			
			CFile _file;
			_file.CreateFile(m_strDstDirectoryFiles + _T("\\document.js"));

			CStringA sDstLen = "";
			#ifdef _DEBUG
			sDstLen.Format("window[\"document_base64\"] = \"%d;", lSizeAll);
			#else
			sDstLen.Format("%d;", lSizeAll);
			#endif
			_file.WriteFile((void*)sDstLen.GetBuffer(), sDstLen.GetLength());
			
			_file.WriteFile((void*)pOutput, nOutputLen);

			#ifdef _DEBUG
			sDstLen = "\";";
			_file.WriteFile((void*)sDstLen.GetBuffer(), sDstLen.GetLength());
			#endif

			_file.CloseFile();
			
			m_oDstFontGenerator.WriteFonts(m_strDstDirectoryFiles + _T("\\fonts"), m_bIsGids);
		}

		AVSINLINE void CheckVectorGraphics()
		{
			LONG lLenVector = m_oVectors.GetPosition();
			if (!m_oRenderers.m_bIsNoSimpleAttack && (0 == lLenVector || false == m_oRenderers.m_bIsDraw))
				return;

			m_bIsImageFromVectors = true;

			m_oSmartText.DumpLine();

			bool bIsEmpty = false;
			bool bIsNoSimple = m_oRenderers.ConvertVectorGraphics((lLenVector > 10000) ? true : false, bIsEmpty, this);
			
			m_bIsImageFromVectors = false;

			if (!bIsNoSimple && !bIsEmpty)
			{
				SetTransformToDocument(true);
				m_oPage.Write(m_oVectors);
			}
			else
			{
				m_oLastPen = *m_pPen;
				m_oLastBrush = *m_pBrush;
				SetFillColor(false);
				SetStrokeColor(false);
				SetLineWidth(false);
			}

			m_oVectors.ClearNoAttack();			
		}

protected:
		AVSINLINE void CopyFile(CString& strFileSrc, CString& strFileDst)
		{
			CDirectory::CopyFile(strFileSrc, strFileDst, NULL, NULL);
		}
		void SaveImage(CString& strFileSrc, CImageInfo& oInfo)
		{
			CString strLoadXml = _T("<transforms><ImageFile-LoadImage sourcepath=\"") + strFileSrc + _T("\"/></transforms>");

			ImageStudio::IImageTransforms* pTransform = NULL;
			CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC_SERVER, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

			VARIANT_BOOL vbRes = VARIANT_FALSE;
			BSTR bsLoad = strLoadXml.AllocSysString();
			pTransform->SetXml(bsLoad, &vbRes);
			SysFreeString(bsLoad);

			pTransform->Transform(&vbRes);

			VARIANT var;
			var.punkVal = NULL;
			pTransform->GetResult(0, &var);

			if (NULL == var.punkVal)
			{
				RELEASEINTERFACE(pTransform);
				return;
			}

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			var.punkVal->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			RELEASEINTERFACE((var.punkVal));

			if (NULL == pFrame)
			{
				RELEASEINTERFACE(pTransform);
				return;
			}

			LONG lWidth		= 0;
			LONG lHeight	= 0;
			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);

			oInfo.m_eType = GetImageType(pFrame);

			RELEASEINTERFACE(pFrame);

			CString strSaveItem = _T("");
			strSaveItem.Format(_T("\\image%d."), oInfo.m_lID);
			if (itJPG == oInfo.m_eType)
			{
				strSaveItem = _T("<ImageFile-SaveAsJpeg destinationpath=\"") + m_strDstMedia + strSaveItem + _T("jpg\" format=\"888\"/>");
			}
			else
			{
				strSaveItem = _T("<ImageFile-SaveAsPng destinationpath=\"") + m_strDstMedia + strSaveItem + _T("png\" format=\"888\"/>");
			}

			CString strXml = _T("");

			// не ресайзим картинки. Они рисуются теперь у нас с dpi 96
			if (true || (lWidth <= m_lMaxSizeImage) && (lHeight <= m_lMaxSizeImage))
			{
				strXml = _T("<transforms>") + strSaveItem + _T("</transforms>");
			}
			else
			{
				LONG lW = 0;
				LONG lH = 0;
				double dAspect = (double)lWidth / lHeight;

				if (lWidth >= lHeight)
				{
					lW = m_lMaxSizeImage;
					lH = (LONG)((double)lW / dAspect);
				}
				else
				{
					lH = m_lMaxSizeImage;
					lW = (LONG)(dAspect * lH);
				}

				CString strResize = _T("");
				strResize.Format(_T("<ImageTransform-TransformResize width=\"%d\" height=\"%d\"/>"), lW, lH);

				strXml = _T("<transforms>") + strResize + strSaveItem + _T("</transforms>");
			}
			
			VARIANT_BOOL vbSuccess = VARIANT_FALSE;
			BSTR bsXml = strXml.AllocSysString();
			pTransform->SetXml(bsXml, &vbSuccess);
			SysFreeString(bsXml);

			pTransform->Transform(&vbSuccess);

			RELEASEINTERFACE(pTransform);
		}
		void SaveImage(IUnknown* punkImage, CImageInfo& oInfo)
		{
			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			punkImage->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			if (NULL == pFrame)
				return;

			LONG lWidth		= 0;
			LONG lHeight	= 0;
			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);

			oInfo.m_eType = GetImageType(pFrame);

			RELEASEINTERFACE(pFrame);
			
			ImageStudio::IImageTransforms* pTransform = NULL;
			CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL ,CLSCTX_INPROC_SERVER, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

			VARIANT var;
			var.vt = VT_UNKNOWN;
			var.punkVal = punkImage;
			pTransform->SetSource(0, var);

			CString strSaveItem = _T("");
			strSaveItem.Format(_T("\\image%d."), oInfo.m_lID);
			if (itJPG == oInfo.m_eType)
			{
				strSaveItem = _T("<ImageFile-SaveAsJpeg destinationpath=\"") + m_strDstMedia + strSaveItem + _T("jpg\" format=\"888\"/>");
			}
			else
			{
				strSaveItem = _T("<ImageFile-SaveAsPng destinationpath=\"") + m_strDstMedia + strSaveItem + _T("png\" format=\"888\"/>");
			}

			CString strXml = _T("");
			// не ресайзим картинки. Они рисуются теперь у нас с dpi 96
			if (true || (lWidth <= m_lMaxSizeImage) && (lHeight <= m_lMaxSizeImage))
			{
				strXml = _T("<transforms>") + strSaveItem + _T("</transforms>");
			}
			else
			{
				LONG lW = 0;
				LONG lH = 0;
				double dAspect = (double)lWidth / lHeight;

				if (lWidth >= lHeight)
				{
					lW = m_lMaxSizeImage;
					lH = (LONG)((double)lW / dAspect);
				}
				else
				{
					lH = m_lMaxSizeImage;
					lW = (LONG)(dAspect * lH);
				}

				CString strResize = _T("");
				strResize.Format(_T("<ImageTransform-TransformResize width=\"%d\" height=\"%d\"/>"), lW, lH);

				strXml = _T("<transforms>") + strResize + strSaveItem + _T("</transforms>");
			}
			
			VARIANT_BOOL vbSuccess = VARIANT_FALSE;
			BSTR bsXml = strXml.AllocSysString();
			pTransform->SetXml(bsXml, &vbSuccess);
			SysFreeString(bsXml);

			pTransform->Transform(&vbSuccess);

			RELEASEINTERFACE(pTransform);
		}

		void SaveImage2(IUnknown* punkImage, CString strDst)
		{
			ImageStudio::IImageTransforms* pTransform = NULL;
			CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL ,CLSCTX_INPROC_SERVER, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

			VARIANT var;
			var.vt = VT_UNKNOWN;
			var.punkVal = punkImage;
			pTransform->SetSource(0, var);

			CString strSaveItem = _T("<ImageFile-SaveAsPng destinationpath=\"") + strDst + _T(".png\" format=\"888\"/>");
			
			CString strXml = _T("<transforms>") + strSaveItem + _T("</transforms>");
						
			VARIANT_BOOL vbSuccess = VARIANT_FALSE;
			BSTR bsXml = strXml.AllocSysString();
			pTransform->SetXml(bsXml, &vbSuccess);
			SysFreeString(bsXml);

			pTransform->Transform(&vbSuccess);
			RELEASEINTERFACE(pTransform);
		}

		CImageInfo GenerateImageID(IUnknown* punkData)
		{
			CImageInfo oInfo;

			if (NULL == punkData)
				return oInfo;

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			punkData->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			BYTE* pBuffer = NULL;
			LONG lLen = 0;

			pFrame->get_Buffer(&pBuffer);
			pFrame->get_BufferSize(&lLen);

			DWORD dwSum = m_oCRC.Calc(pBuffer, lLen);

			CAtlMap<DWORD, CImageInfo>::CPair* pPair = m_mapImageData.Lookup(dwSum);
			if (NULL == pPair)
			{
				// нужно добавить
				++m_lNextIDImage;
				
				oInfo.m_lID = m_lNextIDImage;
				SaveImage(punkData, oInfo);
				m_mapImageData.SetAt(dwSum, oInfo);
			}
			else
			{
				oInfo = pPair->m_value;
			}

			RELEASEINTERFACE(pFrame);

			return oInfo;
		}

		CImageInfo GenerateImageID(CString& strFileName)
		{
			CImageInfo oInfo;
			CAtlMap<CString, CImageInfo>::CPair* pPair = m_mapImagesFile.Lookup(strFileName);

			if (NULL == pPair)
			{
				// нужно добавить
				++m_lNextIDImage;
				
				oInfo.m_lID = m_lNextIDImage;
				SaveImage(strFileName, oInfo);
				m_mapImagesFile.SetAt(strFileName, oInfo);
			}
			else
			{
				oInfo = pPair->m_value;
			}

			return oInfo;
		}

		ImageType GetImageType(MediaCore::IAVSUncompressedVideoFrame* pFrame)
		{
			LONG lWidth		= 0;
			LONG lHeight	= 0;
			BYTE* pBuffer	= NULL;

			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);
			pFrame->get_Buffer(&pBuffer);

			BYTE* pBufferMem = pBuffer + 3;
			LONG lCountPix = lWidth * lHeight;

			for (LONG i = 0; i < lCountPix; ++i, pBufferMem += 4)
			{
				if (255 != *pBufferMem)
					return itPNG;
			}
			return itJPG;
		}

		void FlipY(IUnknown* punkImage)
		{
			if (NULL == punkImage)
				return;

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			punkImage->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			if (NULL == pFrame)
				return;

			BYTE* pBuffer	= NULL;
			LONG lWidth		= 0;
			LONG lHeight	= 0;
			LONG lStride	= 0;

			pFrame->get_Buffer(&pBuffer);
			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);
			pFrame->get_Stride(0, &lStride);

			if (lStride < 0)
				lStride = -lStride;
			
			if ((lWidth * 4) != lStride)
			{
				RELEASEINTERFACE(pFrame);
				return;
			}

			BYTE* pBufferMem = new BYTE[lStride];

			BYTE* pBufferEnd = pBuffer + lStride * (lHeight - 1);

			LONG lCountV = lHeight / 2;

			for (LONG lIndexV = 0; lIndexV < lCountV; ++lIndexV)
			{
				memcpy(pBufferMem, pBuffer, lStride);
				memcpy(pBuffer, pBufferEnd, lStride);
				memcpy(pBufferEnd, pBufferMem, lStride);
				
				pBuffer		+= lStride;
				pBufferEnd	-= lStride;
			}

			RELEASEARRAYOBJECTS(pBufferMem);
			RELEASEINTERFACE(pFrame);
		}

		void FlipX(IUnknown* punkImage)
		{
			if (NULL == punkImage)
				return;

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			punkImage->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			if (NULL == pFrame)
				return;

			BYTE* pBuffer	= NULL;
			LONG lWidth		= 0;
			LONG lHeight	= 0;
			LONG lStride	= 0;

			pFrame->get_Buffer(&pBuffer);
			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);
			pFrame->get_Stride(0, &lStride);

			if (lStride < 0)
				lStride = -lStride;
			
			if ((lWidth * 4) != lStride)
			{
				RELEASEINTERFACE(pFrame);
				return;
			}

			DWORD* pBufferDWORD	= (DWORD*)pBuffer;

			LONG lW2 = lWidth / 2;
			for (LONG lIndexV = 0; lIndexV < lHeight; ++lIndexV)
			{
				DWORD* pMem1 = pBufferDWORD;
				DWORD* pMem2 = pBufferDWORD + lWidth - 1;
				
				LONG lI = 0;
				while (lI < lW2)
				{
					DWORD dwMem = *pMem1;
					*pMem1++ = *pMem2;
					*pMem2-- = dwMem;
				}
			}

			RELEASEINTERFACE(pFrame);
		}

	};
}