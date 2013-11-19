#pragma once
#include "stdafx.h"
#include "atlsimpcoll.h"
#include "PaintStruct.h"
#include "mediaformatdefine.h"
#include "OfficeCommandsRenderer.h"
#include "GeometryHdr.h"


#include "gdiplus.h"
using namespace Gdiplus;

[object, uuid("72FECD99-6900-4f3f-85EF-544387A1F340"), dual, pointer_default(unique)]
__interface IAVSHDCRenderer : IAVSOfficeRendererTemplate
{
	[id(1000 + 1)] HRESULT SaveEMFPage([in] BSTR bstrFileName, [in] IUnknown* punkCommandRenderer, [in] LONG lPageNumber);
	[id(1000 + 2)] HRESULT PrintPage([in] LONG* pHDC, [in] IUnknown* punkCommandRenderer, [in] LONG lPageNumber,
		[in] double dLeftPix, [in] double dTopPix, [in] double dWidthPix, [in] double dHeightPix, [in] double dAngle);

	[id(2000), propget] HRESULT TempDirectory([out, retval] BSTR* bstrTempDir);
	[id(2000), propput] HRESULT TempDirectory([in] BSTR pbstrTempDir);
};

[coclass, uuid("3537C509-60FF-4cfb-8036-FA7926FDB4E1"), threading(apartment), vi_progid("HDCRenderer"), progid("HDCRenderer.1"), version(1.0)]
class ATL_NO_VTABLE CAVSHDCRenderer
	:	public IAVSHDCRenderer, public IAVSOfficeRendererTemplate2
{
protected:
	CGdiplusRenderer	m_oRenderer;

	Metafile*			m_pMetaFile;
	HDC					m_hDC;

	CString				m_strTempDirectory;
	CString				m_strFilePath;

public:

	CAVSHDCRenderer() : m_oRenderer()
	{
		m_hDC = NULL;
		m_pMetaFile = NULL;

		m_strTempDirectory = _T("");
		m_strFilePath = _T("");
	}

	~CAVSHDCRenderer()
	{
		m_oRenderer.Clear();
	}

public:
	// ---------------------------------------------
	// функции для работы со страницей (здесь - только лишь для вычисления dpi)
	STDMETHOD(NewPage)()
	{
		// ну не влезло так не влезло
		return S_OK;
	}
	STDMETHOD(SetHeight)(float fHeight)
	{
		return m_oRenderer.SetHeight(fHeight);
	}
	STDMETHOD(SetWidth)(float fWidth)
	{
		return m_oRenderer.SetWidth(fWidth);
	}
	// ---------------------------------------------
	// функции состояния откисовщика ---------------
	STDMETHOD(SetPen)(BSTR bsXML)
	{
		return m_oRenderer.SetPen(bsXML);
	}
	STDMETHOD(SetBrush)(BSTR bsXML)
	{
		return m_oRenderer.SetBrush(bsXML);
	}
	STDMETHOD(SetFont)(BSTR bsXML)
	{
		return m_oRenderer.SetFont(bsXML);
	}
	STDMETHOD(SetShadow)(BSTR bsXML)
	{
		return m_oRenderer.SetShadow(bsXML);
	}
	STDMETHOD(SetEdgeText)(BSTR bsXML)
	{
		return m_oRenderer.SetEdgeText(bsXML);
	}
	// ---------------------------------------------
	// отрисовка текста
	STDMETHOD(CommandDrawText)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset)
	{
		return m_oRenderer.CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	// ---------------------------------------------
	// GraphicPath Methods
	STDMETHOD(PathCommandMoveTo)(float fX, float fY)
	{
		return m_oRenderer.PathCommandMoveTo(fX, fY);
	}
	STDMETHOD(PathCommandLineTo)(float fX, float fY)
	{
		return m_oRenderer.PathCommandLineTo(fX, fY);
	}
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY** ppPoints)
	{
		return m_oRenderer.PathCommandLinesTo(ppPoints);
	}

	STDMETHOD(PathCommandCurveTo)(float fX1, float fY1, float fX2, float fY2, float fX3, float fY3)
	{
		return m_oRenderer.PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY** ppPoints)
	{
		return m_oRenderer.PathCommandCurvesTo(ppPoints);
	}
	STDMETHOD(PathCommandArcTo)(float fX, float fY, float fWidth, float fHeight, float fStartAngle, float fSweepAngle)
	{
		return m_oRenderer.PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
	}
	STDMETHOD(PathCommandClose)()
	{
		return m_oRenderer.PathCommandClose();
	}
	STDMETHOD(PathCommandEnd)()
	{
		return m_oRenderer.PathCommandEnd();
	}

	STDMETHOD(PathCommandStart)()
	{
		return m_oRenderer.PathCommandStart();
	}
	STDMETHOD(PathCommandGetCurrentPoint)(float* fX, float* fY)
	{
		return m_oRenderer.PathCommandGetCurrentPoint(fX, fY);
	}
	
	STDMETHOD(DrawPath)(long nType)
	{
		return m_oRenderer.DrawPath(nType);
	}
	// ---------------------------------------------
	STDMETHOD(DrawImage)(IUnknown **pInterface, float fX, float fY, float fWidth, float fHeight)
	{
		return m_oRenderer.DrawImage(pInterface, fX, fY, fWidth, fHeight);
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
		return m_oRenderer.BeginCommand(lType);
	}
	STDMETHOD(EndCommand)(DWORD lType)
	{
		return m_oRenderer.EndCommand(lType);
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
		return m_oRenderer.GetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
	}
	STDMETHOD(SetCommandParams)(double dAngle,  double dLeft,  double dTop,  double dWidth,  double dHeight,  DWORD lFlags)
	{
		return m_oRenderer.SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
	}

	STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		return m_oRenderer.SetTransform(dA, dB, dC, dD, dE, dF);
	}
	STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
	{
		return m_oRenderer.GetTransform(pdA, pdB, pdC, pdD, pdE, pdF);
	}
	STDMETHOD(ResetTransform)(void)
	{
		return m_oRenderer.ResetTransform();
	}

	STDMETHOD(get_ClipMode)(LONG* plMode)
	{
		return m_oRenderer.get_ClipMode(plMode);
	}
	STDMETHOD(put_ClipMode)(LONG lMode)
	{
		return m_oRenderer.put_ClipMode(lMode);
	}

	STDMETHOD(PathCommandText)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset)
	{
		return m_oRenderer.PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	
	// IAVSHDCRenderer -------------------------------------------------------------
	STDMETHOD(SaveEMFPage)(BSTR bstrFileName, IUnknown* punkCommandRenderer, LONG lPageNumber)
	{
		if (NULL == punkCommandRenderer)
			return S_FALSE;

		IAVSCommandsRenderer* pCommandRenderer = NULL;
		punkCommandRenderer->QueryInterface(__uuidof(IAVSCommandsRenderer), (void**)&pCommandRenderer);

		if (NULL == pCommandRenderer)
			return S_FALSE;

		LONG lPageCount = 0;
		pCommandRenderer->get_PageCount(&lPageCount);

		if (0 > lPageNumber || lPageNumber >= lPageCount)
		{
			RELEASEINTERFACE(pCommandRenderer);			
			return S_FALSE;
		}

		CString strFile = (CString)bstrFileName;
		float fWidth	= 0;
		float fHeight	= 0;
		pCommandRenderer->GetPageSize(lPageNumber, &fWidth, &fHeight);

		SetWidth(fWidth);
		SetHeight(fHeight);
		Create(strFile);

		IUnknown* pHDC = NULL;
		this->QueryInterface(IID_IUnknown, (void**)&pHDC);

		pCommandRenderer->DrawPage(lPageNumber, pHDC);

		RELEASEINTERFACE(pHDC);

		ReleaseMeta();

		RELEASEINTERFACE(pCommandRenderer);

		return S_OK;
	}

	STDMETHOD(get_TempDirectory)(BSTR* bstrFileName)
	{
		if (NULL == bstrFileName)
			return S_FALSE;

		*bstrFileName = m_strTempDirectory.AllocSysString();
		return S_OK;
	}

	STDMETHOD(put_TempDirectory)(BSTR bstrFileName)
	{
		m_strTempDirectory = (CString)bstrFileName;
		return S_OK;
	}

	STDMETHOD(PrintPage)(LONG* pHDC, IUnknown* punkCommandRenderer, LONG lPageNumber,
		double dLeftPix, double dTopPix, double dWidthPix, double dHeightPix, double dAngle)
	{
		IAVSCommandsRenderer* pCommandRenderer = NULL;
		punkCommandRenderer->QueryInterface(__uuidof(IAVSCommandsRenderer), (void**)&pCommandRenderer);

		if (NULL == pCommandRenderer)
			return S_FALSE;
		
		m_hDC = (*(HDC*)(pHDC));
		m_oRenderer.CreateFromHDC((HDC*)pHDC);

		LONG lMM_Width		= GetDeviceCaps(m_hDC, HORZSIZE);
		LONG lMM_Height		= GetDeviceCaps(m_hDC, VERTSIZE);

		LONG lPix_Width		= GetDeviceCaps(m_hDC, HORZRES);
		LONG lPix_Height	= GetDeviceCaps(m_hDC, VERTRES);

		LONG lDpiX			= GetDeviceCaps(m_hDC, LOGPIXELSX);
		LONG lDpiY			= GetDeviceCaps(m_hDC, LOGPIXELSY);

		double l1			= (double)(25.4 / lDpiX);
		double l2			= (double)(25.4 / lDpiY);

		float fWidthPage = 0;
		float fHeightPage = 0;

		pCommandRenderer->GetPageSize(lPageNumber, &fWidthPage, &fHeightPage);

		double dScale1		= l1 * dWidthPix / fWidthPage;
		double dScale2		= l2 * dHeightPix / fHeightPage;

		Gdiplus::Matrix oBaseTransformMatrix(1, 0, 0, 1, 0, 0);

		double dCX = (dLeftPix + dWidthPix / 2.0);
		double dCY = (dTopPix + dHeightPix / 2.0);

		double dCos = cos(dAngle);
		double dSin = sin(dAngle);
		double X = (dLeftPix - dCX) * dCos - (dTopPix - dCY) * dSin + dCX;
		double Y = (dLeftPix - dCX) * dSin + (dTopPix - dCY) * dCos + dCY;

		X *= l1;
		Y *= l2;

		oBaseTransformMatrix.Scale((REAL)(dScale1), (REAL)(dScale2), MatrixOrderAppend);
		oBaseTransformMatrix.Rotate((REAL)(180 * dAngle / M_PI), MatrixOrderAppend);
		oBaseTransformMatrix.Translate((REAL)X, (REAL)Y, MatrixOrderAppend);

		m_oRenderer.SetBaseMatrix(oBaseTransformMatrix);
		m_oRenderer.ApplyBaseMatrix();
		
		IUnknown* punkThis = NULL;
		this->QueryInterface(IID_IUnknown, (void**)&punkThis);
		pCommandRenderer->DrawPage(lPageNumber, punkThis);
		RELEASEINTERFACE(punkThis);

		m_oRenderer.ResetBaseMatrix();

		RELEASEINTERFACE(pCommandRenderer);

		m_hDC = NULL;

		return S_OK;
		

		// нужно проскалировать систему координат. Но пока - через метафайл
		
		//NSGeometry::CDoubleRect oRect;
		//oRect.left		= dLeftPix;
		//oRect.top		= dTopPix;
		//oRect.right		= dLeftPix + dWidthPix;
		//oRect.bottom	= dTopPix + dHeightPix;

		//NSGeometry::CDoubleRectRotate oRectRotate(oRect, dAngle);
		//PointF* points = oRectRotate.GetPoints();

		//HDC* pHdc = (HDC*)pHDC;

		////CString strMeta = m_strTempDirectory + _T("\\temp.emf");
		//CString strMeta = _T("E:\\Image6.wmf");
		//
		//BSTR bstrTemp = strMeta.AllocSysString();
		////SaveEMFPage(bstrTemp, punkCommandRenderer, lPageNumber);

		////Metafile oMeta(bstrTemp);

		//RECT oRectPlay;
		//oRectPlay.left		= (long)(dLeftPix);
		//oRectPlay.top		= (long)(dTopPix);
		//oRectPlay.right		= (long)(dLeftPix + dWidthPix);
		//oRectPlay.bottom	= (long)(dTopPix + dHeightPix);

		//REAL fCentreX = (REAL)(oRect.GetCentreX());
		//REAL fCentreY = (REAL)(oRect.GetCentreY());

		//Matrix oMatrixRotate(1, 0, 0, 1, 0, 0);

		//oMatrixRotate.Translate(-fCentreX, -fCentreY		, MatrixOrderAppend);
		//
		//oMatrixRotate.Rotate((REAL)(180.0 * dAngle / M_PI)	, MatrixOrderAppend);
		//
		//oMatrixRotate.Translate(fCentreX, fCentreY			, MatrixOrderAppend);

		//REAL pElements[6];
		//oMatrixRotate.GetElements(pElements);

		//XFORM oMatrix;
		//oMatrix.eM11	= pElements[0];
		//oMatrix.eM12	= pElements[1];
		//oMatrix.eM21	= pElements[2];
		//oMatrix.eM22	= pElements[3];
		//oMatrix.eDx		= pElements[4];
		//oMatrix.eDy		= pElements[5];

		//if (0 != dAngle)
		//{
		//	SetGraphicsMode(*pHdc, GM_ADVANCED);

		//	LONG lRes = 0;
		//	lRes = SetWorldTransform(*pHdc, &oMatrix);
		//}

		//CFile oFile;
		//oFile.OpenFile(strMeta);

		//CFile oFile2;
		//oFile2.CreateFile(_T("E:\\meta.wmf"));

		//BYTE* pHeader = ToWMFHeader(oRectPlay);
		//oFile2.WriteFile((void*)pHeader, 22);

		//DWORD dwData = oFile.GetFileSize();

		//BYTE* pMeta = new BYTE[dwData];
		//oFile.ReadFile(pMeta, dwData);

		//oFile2.WriteFile((void*)pMeta, dwData);

		//oFile.CloseFile();
		//oFile2.CloseFile();

		//RELEASEARRAYOBJECTS(pHeader);
		//RELEASEARRAYOBJECTS(pMeta);


		//HENHMETAFILE hEnhMeta = GetEnhMetaFile(bstrTemp);
		//PlayEnhMetaFile(*pHdc, hEnhMeta, &oRectPlay);

		////HMETAFILE hEnhMeta = GetMetaFile(bstrTemp);
		////BOOL b = PlayMetaFile(*pHdc, hEnhMeta);
		////
		////if (0 != dAngle)
		////{
		////	XFORM oMatrix;
		////	oMatrix.eM11 = 1;
		////	oMatrix.eM12 = 0;
		////	oMatrix.eM21 = 0;
		////	oMatrix.eM22 = 1;
		////	oMatrix.eDx	 = 0;
		////	oMatrix.eDy	 = 0;	
		////	SetWorldTransform(*pHdc, &oMatrix);
		////}

		//DeleteEnhMetaFile(hEnhMeta);
		////DeleteMetaFile(hEnhMeta);
		//
		////DeleteFile(bstrTemp);

		//SysFreeString(bstrTemp);
		//RELEASEARRAYOBJECTS(points);
		//
		//return S_OK;
	}

	// -----------------------------------------------------------------------------

protected:
	void Create(CString strFile)
	{
		m_strFilePath = strFile;
		//m_hDC = GetDC(NULL);
		RectF oBounds(0, 0, (REAL)(m_oRenderer.m_dWidth * 100), (REAL)(m_oRenderer.m_dHeight * 100));

		RECT oRect;
		oRect.left = 0;
		oRect.top = 0;
		oRect.right = (LONG)(100 * m_oRenderer.m_dWidth);
		oRect.bottom = (LONG)(100 * m_oRenderer.m_dHeight);

		BSTR bsFile = m_strFilePath.AllocSysString();

		m_hDC = CreateEnhMetaFile(NULL, bsFile, &oRect, NULL);

		//m_pMetaFile = new Gdiplus::Metafile(bsFile, m_hDC, oBounds);

		//m_pRenderer = new Gdiplus::Graphics(m_pMetaFile);
		m_oRenderer.m_pRenderer = new Gdiplus::Graphics(m_hDC);
		m_oRenderer.m_pRenderer->SetSmoothingMode(SmoothingModeAntiAlias);
		m_oRenderer.m_pRenderer->SetTextRenderingHint(TextRenderingHintAntiAlias);

		m_oRenderer.m_pRenderer->SetPageUnit(UnitMillimeter);

		m_oRenderer.m_pPath = new Gdiplus::GraphicsPath(FillModeWinding);

		LONG lPixWidth  = GetDeviceCaps(m_hDC, HORZRES);
		LONG lPixHeight = GetDeviceCaps(m_hDC, VERTRES);

		LONG lMMWidth	= GetDeviceCaps(m_hDC, HORZSIZE);
		LONG lMMHeight	= GetDeviceCaps(m_hDC, VERTSIZE);

		m_oRenderer.m_dPixelsWidth  = (double)(lPixWidth  * m_oRenderer.m_dWidth)  / lMMWidth;
		m_oRenderer.m_dPixelsHeight = (double)(lPixHeight * m_oRenderer.m_dHeight) / lMMHeight;

		double dNewDpi = lPixHeight / (lMMHeight / 25.4);

		m_oRenderer.m_dPixelsHeight *=  (96 / dNewDpi);
		m_oRenderer.m_dPixelsWidth	= (m_oRenderer.m_dWidth / m_oRenderer.m_dHeight) * m_oRenderer.m_dPixelsHeight;
	}

	void ReleaseMeta()
	{
		//RELEASEOBJECT(m_pMetaFile);
		//ReleaseDC(NULL, m_hDC);
		HENHMETAFILE hMeta = CloseEnhMetaFile(m_hDC);
		DeleteEnhMetaFile(hMeta);
		m_hDC = NULL;

		if (_T("") == m_strFilePath)
			return;

		// теперь заменяем размеры
		CFile oFile;
		if (S_OK != oFile.OpenFileRW(m_strFilePath))
			return;

		/*DWORD lReadSize = min((DWORD)oFile.GetFileSize(), 1000);
		BYTE* pBuffer = new BYTE[lReadSize];

		if (NULL != pBuffer)
		{
			oFile.ReadFile(pBuffer, lReadSize);

			LONG lIndex = 0;
			for (; lIndex < lReadSize - 3; ++lIndex)
			{
				if ((0x20 == pBuffer[lIndex]) &&
					(0x45 == pBuffer[lIndex + 1]) &&
					(0x4D == pBuffer[lIndex + 2]) &&
					(0x46 == pBuffer[lIndex + 3]))
				{
					break;
				}
			}

			
		}*/

		DWORD dwNULL = 0;
		
		DWORD dwSizePixW	= (DWORD)(m_oRenderer.m_dPixelsWidth);
		DWORD dwSizePixH	= (DWORD)(m_oRenderer.m_dPixelsHeight);
		DWORD dwSizeMMW		= (DWORD)(m_oRenderer.m_dWidth);
		DWORD dwSizeMMH		= (DWORD)(m_oRenderer.m_dHeight);

		oFile.SetPosition(4 * 2);

		oFile.WriteFile((void*)&dwNULL, 4);
		oFile.WriteFile((void*)&dwNULL, 4);
		oFile.WriteFile((void*)&dwSizePixW, 4);
		oFile.WriteFile((void*)&dwSizePixH, 4);

		oFile.SetPosition(4 * 18);

		oFile.WriteFile((void*)&dwSizePixW, 4);
		oFile.WriteFile((void*)&dwSizePixH, 4);
		oFile.WriteFile((void*)&dwSizeMMW, 4);
		oFile.WriteFile((void*)&dwSizeMMH, 4);

		oFile.SetPosition(100);

		DWORD dwSizeMicroMW = dwSizeMMW * 1000;
		DWORD dwSizeMicroMH = dwSizeMMH * 1000;

		oFile.WriteFile((void*)&dwSizeMicroMW, 4);
		oFile.WriteFile((void*)&dwSizeMicroMH, 4);

		oFile.CloseFile();
	}
};