#pragma once
#include "stdafx.h"
#include "mediaformatdefine.h"
#include "OfficeRendererTemplate.h"
#include "..\agg\AVSWrapper\Graphics.h"

// IAVS2DRenderer
[
	object,
	uuid("B85F5A4D-825C-4adc-8D26-6D866B302FBB"),
	dual,	helpstring("IAVS2DRenderer Interface"),
	pointer_default(unique)
]
__interface IAVS2DRenderer : IAVSOfficeRendererTemplate
{
	[id(20000 + 1)]  HRESULT CreateFromMediaData(IUnknown* punkMediaData);
};

[dispinterface, uuid("0CE6CE34-26F3-4ee5-98DD-8271F1ED7D8C")]
__interface _IAVS2DRendererEvents
{
};

[coclass, uuid("284AE5C1-B6B5-4331-BC81-E9462AC7C705"), event_source(com), threading(apartment), vi_progid("AVS2DRenderer"), progid("AVS2DRenderer.1"), version(1.0)]
class ATL_NO_VTABLE CAVS2DRenderer
	:	public IAVS2DRenderer, public IAVSOfficeRendererTemplate2
{
protected:

	IAVSFontManager*						m_pFontManager;

	MediaCore::IAVSUncompressedVideoFrame*	m_pImage;
	Aggplus::CBitmap*						m_pBitmap;

	Aggplus::CGraphics*						m_pRenderer;
	Aggplus::CGraphicsPath*					m_pPath;

	NSStructures::CPen						m_oPen;
	NSStructures::CBrush					m_oBrush;
	NSStructures::CFont						m_oFont;
	NSStructures::CShadow					m_oShadow;
	NSStructures::CEdgeText					m_oEdge;

	double m_dWidth;
	double m_dHeight;

	double m_dPixelsWidth;
	double m_dPixelsHeight;

	// настройки текущего объекта отрисовки
	double m_dCurrentAngle;
	
	double m_dCurrentLeft;
	double m_dCurrentTop;
	double m_dCurrentWidth;
	double m_dCurrentHeight;

	DWORD m_lCurrentFlags;

	DWORD m_lCurrentCommandType;

	LONG m_lCurrentClipMode;
	BOOL			m_bIsSetupClip;

public:

	__event __interface _IAVS2DRendererEvents;

	CAVS2DRenderer()
	{
		m_pRenderer = NULL;
		m_pPath = NULL;

		m_pImage = NULL;
		m_pBitmap = NULL;

		m_dWidth = 210;
		m_dHeight = 190;

		m_dPixelsWidth = 720;
		m_dPixelsHeight = 576;

		m_dCurrentAngle		= 0;
	
		m_dCurrentLeft		= 0;
		m_dCurrentTop		= 0;
		m_dCurrentWidth		= 0;
		m_dCurrentHeight	= 0;

		m_lCurrentFlags		= 0;

		m_lCurrentCommandType	= 0;

		m_lCurrentClipMode	= 0;
		m_bIsSetupClip		= FALSE;

		m_pFontManager = NULL;
		CoCreateInstance(__uuidof(CAVSFontManager), NULL, CLSCTX_INPROC, __uuidof(IAVSFontManager), (void**)&m_pFontManager);

		m_pFontManager->Initialize(L"");
	}

	~CAVS2DRenderer()
	{
		Clear();

		RELEASEINTERFACE(m_pFontManager);
	}

protected:
	BOOL CheckValidate(BOOL bOnlyGraphics = FALSE)
	{
		if (NULL == m_pRenderer)
			return FALSE;

		if (!bOnlyGraphics)
		{
			return (NULL != m_pPath);
		}

		return TRUE;
	}

	void Clear()
	{
		RELEASEOBJECT(m_pPath);
		RELEASEOBJECT(m_pRenderer);
		
		RELEASEINTERFACE(m_pImage);
		RELEASEOBJECT(m_pBitmap);
	}

	inline BOOL COM_GetBGRAImageFromInterface(IUnknown* pSource, BYTE*& pBGRA, int& nWidth, int& nHeight, int* pStride = NULL, long* pAspectX = NULL, long* pAspectY = NULL)
	{
		/* @desc:
		int COM_GetBGRAImageFromInterface(IUnknown* pSource, BYTE*& pBGRA, int& nWidth, int& nHeight)
		{
			из указанного интерфейса (pSource) вытаскиваетс€ картинка (pBGRA, nWidth, nHeight)
			в формате BGRA; если формат интерфейса не CSP_BGRA - конвертаци€ формата не производитс€
		}
		*/

		if (NULL == pSource)
			return FALSE;

		// вытаскиваем из интерфейса uncompressed frame
		MediaCore::IAVSUncompressedVideoFrame* pUncompressedFrame = NULL;
		pSource->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pUncompressedFrame));
		if (NULL == pUncompressedFrame)
			return FALSE;

		// запрашиваем настройки картинки
		long lWidth = 0; pUncompressedFrame->get_Width(&lWidth);
		long lHeight = 0; pUncompressedFrame->get_Height(&lHeight);
		long lColorSpace = 0; pUncompressedFrame->get_ColorSpace(&lColorSpace);
		long lStride = 0; pUncompressedFrame->get_Stride(0, &lStride);
		long lBufferSize = 0; pUncompressedFrame->get_BufferSize(&lBufferSize);
		BYTE* pSourceBuffer = NULL; pUncompressedFrame->get_Buffer(&pSourceBuffer);

		// провер€ем совместимость кадра и формата
		if (CSP_BGRA != (CSP_COLOR_MASK & lColorSpace) || 4*lWidth != lStride || lWidth < 1 || lHeight < 1 || lBufferSize != 4*lWidth*lHeight || !pSourceBuffer)
		{
			pUncompressedFrame->Release();

			return FALSE;
		}

		if( pStride )
			*pStride = (lColorSpace & CSP_VFLIP) ? -lStride : lStride;

		if( pAspectX ) pUncompressedFrame->get_AspectRatioX( pAspectX );
		if( pAspectY ) pUncompressedFrame->get_AspectRatioY( pAspectY );

		// все хорошо, озвращаем наверх нужные параметры
		pBGRA = pSourceBuffer;
		nWidth = lWidth;
		nHeight = lHeight;

		pUncompressedFrame->Release();

		return TRUE;
	}

	inline void UpdateSize()
	{
		if (NULL == m_pBitmap || NULL == m_pRenderer)
			return;

		m_pRenderer->SetPageWidth(m_dWidth, Aggplus::UnitMillimeter);
		m_pRenderer->SetPageHeight(m_dHeight, Aggplus::UnitMillimeter);

		m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);
	}

public:
	// создание рендерера из картинки
	STDMETHOD(CreateFromMediaData)(IUnknown* punkMediaData)
	{
		Clear();

		if (NULL == punkMediaData)
			return S_FALSE;

		punkMediaData->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pImage);
		if (NULL == m_pImage)
			return S_FALSE;

		BYTE* pBGRA = NULL;
		int nWidth = 0;
		int nHeight = 0;

		int nStride = 0;

		// вытаскиваем из интерфейса указатель на пам€ть в формате BGRA
		BOOL bRes = COM_GetBGRAImageFromInterface(punkMediaData, pBGRA, nWidth, nHeight, &nStride);
		if (!bRes)
		{
			RELEASEINTERFACE(m_pImage);
			return S_FALSE;
		}

		m_dPixelsWidth = max(1.0, (double)nWidth);
		m_dPixelsHeight = max(1.0, (double)nHeight);

		m_pBitmap = new Aggplus::CBitmap( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );

		if ( Aggplus::Ok != m_pBitmap->m_Status )
		{
			RELEASEINTERFACE(m_pImage);
			RELEASEOBJECT(m_pBitmap);
			return S_FALSE;
		}

		m_pRenderer = new Aggplus::CGraphics(m_pBitmap);

		m_pRenderer->SetPageWidth(m_dWidth, Aggplus::UnitMillimeter);
		m_pRenderer->SetPageHeight(m_dHeight, Aggplus::UnitMillimeter);
		m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);

		m_pPath = new Aggplus::CGraphicsPath();
		return S_OK;
	}
	// ---------------------------------------------
	// функции дл€ работы со страницей (здесь - только лишь дл€ вычислени€ dpi)
	STDMETHOD(NewPage)()
	{
		// ну не влезло так не влезло
		return S_OK;
	}
	STDMETHOD(SetHeight)(float fHeight)
	{
		m_dHeight = (double)fHeight;
		if (NULL != m_pRenderer)
		{
			m_pRenderer->SetPageHeight(m_dHeight, Aggplus::UnitMillimeter);
			m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);
		}
		return S_OK;
	}
	STDMETHOD(SetWidth)(float fWidth)
	{
		m_dWidth = (double)fWidth;
		if (NULL != m_pRenderer)
		{
			m_pRenderer->SetPageWidth(m_dWidth, Aggplus::UnitMillimeter);
			m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);
		}
		return S_OK;
	}
	// ---------------------------------------------
	// функции состо€ни€ откисовщика ---------------
	STDMETHOD(SetPen)(BSTR bsXML)
	{
		//return S_OK;
		
		CString str = (CString)bsXML;
		m_oPen.FromXmlString(str);
		return S_OK;
	}
	STDMETHOD(SetBrush)(BSTR bsXML)
	{
		//return S_OK;

		CString str = (CString)bsXML;
		m_oBrush.FromXmlString(str);
		return S_OK;
	}
	STDMETHOD(SetFont)(BSTR bsXML)
	{
		//return S_OK;
		
		CString str = (CString)bsXML;
		m_oFont.FromXmlString(str);
		return S_OK;
	}
	STDMETHOD(SetShadow)(BSTR bsXML)
	{
		return S_OK;
		
		CString str = (CString)bsXML;
		m_oShadow.FromXmlString(str);
		return S_OK;
	}
	STDMETHOD(SetEdgeText)(BSTR bsXML)
	{
		return S_OK;

		CString str = (CString)bsXML;
		m_oEdge.FromXmlString(str);
		return S_OK;
	}
	// ---------------------------------------------
	// отрисовка текста
	STDMETHOD(CommandDrawText)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset)
	{
		_SetFont();

		Aggplus::CBrush* pBrush = Aggplus::CreateBrush(&m_oBrush);
		m_pRenderer->DrawString(bsText, m_pFontManager, pBrush, (double)fX, (double)fY);

		return S_OK;
	}
	// ---------------------------------------------
	// GraphicPath Methods
	STDMETHOD(PathCommandMoveTo)(float fX, float fY)
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->MoveTo((double)fX, (double)fY);
		return S_OK;
	}
	STDMETHOD(PathCommandLineTo)(float fX, float fY)
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->LineTo((double)fX, (double)fY);
		return S_OK;
	}
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY** ppPoints)
	{
		if (!CheckValidate() || (NULL == ppPoints) || (NULL == *ppPoints))
			return S_FALSE;

		LONG lCount = (*ppPoints)->rgsabound[0].cElements;
		//lCount /= 2;

		float* pPoints = (float*)((*ppPoints)->pvData); 

		m_pPath->AddLines(pPoints, lCount);
		return S_OK;
	}

	STDMETHOD(PathCommandCurveTo)(float fX1, float fY1, float fX2, float fY2, float fX3, float fY3)
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->CurveTo((double)fX1, (double)fY1, (double)fX2, (double)fY2, (double)fX3, (double)fY3);
		return S_OK;
	}
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY** ppPoints)
	{
		if (!CheckValidate())
			return S_FALSE;

		LONG lCount = (*ppPoints)->rgsabound[0].cElements;
		//lCount /= 2;

		float* pPoints = (float*)((*ppPoints)->pvData); 

		m_pPath->AddBeziers(pPoints, lCount);

		return S_OK;
	}
	STDMETHOD(PathCommandArcTo)(float fX, float fY, float fWidth, float fHeight, float fStartAngle, float fSweepAngle)
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->AddArc2((double)fX, (double)fY, (double)fWidth, (double)fHeight, (double)-fStartAngle, (double)-fSweepAngle);
		return S_OK;
	}
	STDMETHOD(PathCommandClose)()
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->CloseFigure();
		return S_OK;
	}
	STDMETHOD(PathCommandEnd)()
	{
		m_pPath->Reset();
		return S_OK;
	}

	STDMETHOD(PathCommandStart)()
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->StartFigure();
		return S_OK;
	}
	STDMETHOD(PathCommandGetCurrentPoint)(float* fX, float* fY)
	{
		double x = 0, y = 0;
		m_pPath->GetLastPoint(x, y);

		*fX = (float)x;
		*fY = (float)y;
		return S_OK;
	}
	
	STDMETHOD(DrawPath)(long nType)
	{
		//return S_OK;
		
		if (!CheckValidate(TRUE))
			return S_FALSE;

		LONG lFillType = (nType & 0xFF00);
		BOOL bIsStroke = (0x01 == (nType & 0x01));

		switch (lFillType)
		{
		case c_nWindingFillMode:
			{
				m_pPath->SetRuler(false);
				Aggplus::CBrush* pBrush = Aggplus::CreateBrush(&m_oBrush);
				m_pRenderer->FillPath(pBrush, m_pPath);
				RELEASEOBJECT(pBrush);
				break;
			}
		case c_nEvenOddFillMode:
			{
				m_pPath->SetRuler(true);
				Aggplus::CBrush* pBrush = Aggplus::CreateBrush(&m_oBrush);
				m_pRenderer->FillPath(pBrush, m_pPath);
				RELEASEOBJECT(pBrush);

				break;
			}
		default:
			break;
		};

		if (bIsStroke)
		{
			m_pRenderer->DrawPath(&m_oPen, m_pPath);
		}
		
		return S_OK;
	}
	// ---------------------------------------------
	STDMETHOD(DrawImage)(IUnknown **pInterface, float fX, float fY, float fWidth, float fHeight)
	{
		if (!CheckValidate(TRUE) || NULL == pInterface)
			return S_FALSE;

		BYTE* pBGRA = NULL;
		int nWidth = 0;
		int nHeight = 0;

		int lStride = 0;
		BOOL bRes = COM_GetBGRAImageFromInterface(*pInterface, pBGRA, nWidth, nHeight, &lStride);
		if (!bRes)
			return S_FALSE;

		// здесь мы внутри картинки перевернем
		//if (0 > lStride)
		//{
		//	// переворачиваем изображение дл€ GDI+
		//	pBGRA += 4 * nWidth * (nHeight - 1);
		//}
		
		Aggplus::CBitmap oImage( nWidth, nHeight, lStride, PixelFormat32bppARGB, pBGRA );
		
		BOOL bFlipX = (0 != (c_nParamFlipX & m_lCurrentFlags));
		BOOL bFlipY = (0 != (c_nParamFlipY & m_lCurrentFlags));

		m_pRenderer->DrawImage(&oImage, fX, fY, fWidth, fHeight);
		
		return S_OK;
	}
	//----------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

	// new
	STDMETHOD(BeginCommand)(DWORD lType)
	{
		m_lCurrentCommandType = lType;
		
		switch (lType)
		{
		case c_nPageType:
			{
				return NewPage();
			}
		default:
			break;
		};
		
		return S_OK;
	}
	STDMETHOD(EndCommand)(DWORD lType)
	{
		switch (lType)
		{
		case c_nClipType:
			{
				BOOL bIsIn = (c_nClipRegionTypeWinding == (0x0001 & m_lCurrentClipMode));

				m_pPath->SetRuler(bIsIn ? false : true);
				m_pRenderer->SetClip(m_pPath);
				break;
			}
		case c_nResetClipType:
			{
				m_pRenderer->ResetClip();
				m_bIsSetupClip = FALSE;
				break;
			}
		default:
			break;
		};

		m_lCurrentCommandType = 0;
		return S_OK;
	}
	STDMETHOD(CommandDrawTextEx)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset, DWORD lFlags, BSTR sParams)
	{
		return CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	STDMETHOD(DrawImageFromFile)(BSTR bstrVal, float fX, float fY, float fWidth, float fHeight)
	{
		return S_OK;
	}

	STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
	{
		if (NULL != dAngle) 
			*dAngle = m_dCurrentAngle;
		
		if (NULL != dLeft)
			*dLeft = m_dCurrentLeft;
		if (NULL != dTop)
			*dTop = m_dCurrentTop;
		if (NULL != dWidth)
			*dWidth = m_dCurrentWidth;
		if (NULL != dHeight)
			*dHeight = m_dCurrentHeight;

		if (NULL != lFlags)
			*lFlags = m_lCurrentFlags;
		
		return S_OK;
	}
	STDMETHOD(SetCommandParams)(double dAngle,  double dLeft,  double dTop,  double dWidth,  double dHeight,  DWORD lFlags)
	{
		m_dCurrentAngle		= dAngle;

		m_dCurrentLeft		= dLeft;
		m_dCurrentTop		= dTop;
		m_dCurrentWidth		= dWidth;
		m_dCurrentHeight	= dHeight;

		m_lCurrentFlags = lFlags;

		ApplyTransform2(m_dCurrentAngle, m_dCurrentLeft, m_dCurrentTop, m_dCurrentWidth, m_dCurrentHeight, m_lCurrentFlags);

		return S_OK;
	}

	STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		if (!CheckValidate())
			return S_FALSE;

		ApplyTransform(dA, dB, dC, dD, dE, dF);
		
		return S_OK;
	}
	STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
	{
		if (!CheckValidate())
			return S_FALSE;

		double buffer[6];
		Aggplus::CMatrix oMatrix;
		m_pRenderer->GetTransform()->GetElements(buffer);

		*pdA = (double)buffer[0];
		*pdB = (double)buffer[1];
		*pdC = (double)buffer[2];
		*pdD = (double)buffer[3];
		*pdE = (double)buffer[4];
		*pdF = (double)buffer[5];

		return S_OK;
	}
	STDMETHOD(ResetTransform)(void)
	{
		_ResetTransform();
		return S_OK;
	}

	STDMETHOD(get_ClipMode)(LONG* plMode)
	{
		if ((!CheckValidate()) || (NULL == plMode))
			return S_FALSE;

		*plMode = m_lCurrentClipMode;
		return S_OK;
	}
	STDMETHOD(put_ClipMode)(LONG lMode)
	{
		if (!CheckValidate())
			return S_FALSE;

		m_lCurrentClipMode = lMode;
		return S_OK;
	}

	STDMETHOD(PathCommandText)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset)
	{
		if (!CheckValidate())
			return S_FALSE;

		_SetFont();

		// так как пат рисуетс€ в миллиметрах - надо перевести пункты в миллиметры
		float fSize = (float)(0.3528 * m_oFont.Size);
		m_pPath->AddString(bsText, m_pFontManager, (double)fX, (double)fY);

		return S_OK;
	}

protected:
	void _SetFont()
	{
		BSTR bsName = m_oFont.Name.AllocSysString();
		m_pFontManager->LoadFontByName(bsName, m_oFont.Size, m_oFont.GetStyle(), (UINT)m_pRenderer->GetDpiX(), (UINT)m_pRenderer->GetDpiY());
	}

	void ApplyTransform(double d1, double d2, double d3, double d4, double d5, double d6)
	{
		Aggplus::CMatrix oMatrix(d1, d2, d3, d4, d5, d6);
		ApplyTransform(&oMatrix);
	}

	void ApplyTransform2(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
	{
		if ((dWidth <= 1) || (dHeight <= 1))
			lFlags = 0;

		BOOL bFlipX = (0 != (c_nParamFlipX & lFlags));
		BOOL bFlipY = (0 != (c_nParamFlipY & lFlags));
		
		double m11 = bFlipX ? -1.0 : 1.0;
		double m22 = bFlipY ? -1.0 : 1.0;
		
		Aggplus::CMatrix oMatrix(1, 0, 0, 1, 0, 0);

		if ((0 != m_dCurrentAngle) || (0 != lFlags))
		{
			double dCentreX = (dLeft + dWidth / 2.0);
			double dCentreY = (dTop + dHeight / 2.0);
			
			oMatrix.Translate(-dCentreX, -dCentreY	, Aggplus::MatrixOrderAppend);
			
			oMatrix.Rotate(m_dCurrentAngle			, Aggplus::MatrixOrderAppend);
			oMatrix.Scale(m11, m22					, Aggplus::MatrixOrderAppend);
			
			oMatrix.Translate(dCentreX, dCentreY	, Aggplus::MatrixOrderAppend);
		}

		ApplyTransform(&oMatrix);
	}

	inline void ApplyTransform(Aggplus::CMatrix* pMatrix)
	{
		if (!CheckValidate())
			return;

		m_pRenderer->SetTransform(pMatrix);
		m_pRenderer->CalculateFullTransform();
	}

	inline void _ResetTransform()
	{
		if (!CheckValidate())
			return;
		
		ApplyTransform(1, 0, 0, 1, 0, 0);
	}
};