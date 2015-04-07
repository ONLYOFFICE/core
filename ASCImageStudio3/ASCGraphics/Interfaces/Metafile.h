#pragma once
#include "..\stdafx.h"

#include "..\Objects\Structures.h"
#include "..\Interfaces\ASCRenderer.h"
#include "..\Interfaces\IASCGraphicsRenderer.h"
#include "TemporaryCS.h"
#include "..\Objects\ASCImage.h"
#include "..\agg\ASCWrapper\DIB.h"
#include "..\agg\ASCWrapper\ArrowHead.h"

#include "../../../Common/MediaFormatDefine.h"

// IASCMetafile
[object, uuid("8E19E616-F99C-46c9-B56D-BFF98241EB39"), dual, pointer_default(unique)]
__interface IASCMetafile : IASCRenderer
{
	[id(20000)] HRESULT Draw([in] IUnknown* punkRenderer, [in] BOOL* pBreak);
	[id(20001)] HRESULT Draw2([in] IUnknown* punkRenderer, [in] double dDPIX, [in] double dDPIY, [in] BOOL* pBreak);

	[id(20002)] HRESULT Save([in] BSTR bsFile, [in] DWORD dwType, [in] BSTR bsParams);

	[id(20003)] HRESULT SetPageInstance([in] IUnknown* punkInstance, [in] LONG lPage);
	[id(20005)] HRESULT SetAdditionalCommands([in] BSTR bsXml);
};

class CImageExt
{
public:
	LONG GetImageType(CString strFile)
	{		
		CFile oFile;
		if (S_OK != oFile.OpenFile(strFile))
		{
			oFile.CloseFile();
			return 0;
		}

		DWORD dwSize = (DWORD)oFile.GetFileSize();

		if (44 > dwSize)
		{
			return 0;
		}

		DWORD dwKey = 0;
		oFile.ReadFile((BYTE*)(&dwKey), 4);

		if (0x9AC6CDD7 == dwKey)
		{
			// placeable meta
			oFile.CloseFile();
			return 1;
		}

		if (0x00000001 == dwKey)
		{
			oFile.SetPosition(40);
			oFile.ReadFile((BYTE*)(&dwKey), 4);
			oFile.CloseFile();

			if (0x464D4520 == dwKey)
			{
				// EMF/EMF+
				return 2;
			}
			else
			{
				// 
				return 0;
			}
		}

		oFile.CloseFile();

		if (0x00090001 == dwKey)
		{
			return 1;
		}

		return 0;
	}

	void DrawOnRenderer(IASCRenderer* pRenderer, BSTR strFile, double dX, double dY, double dW, double dH, BOOL bIsFromFileUse = TRUE)
	{
		if (NULL == pRenderer)
			return;

		LONG lRendType = 0;
		pRenderer->get_Type(&lRendType);
		if (c_nPDFWriter == lRendType)
		{
			VARIANT var;
			var.vt = VT_BOOL;
			var.boolVal = VARIANT_TRUE;
			pRenderer->SetAdditionalParam(L"WhiteBackImage", var);
		}

		LONG lFileType = 0;
		
		if (c_nEbookWriter != lRendType)
			lFileType = GetImageType((CString)strFile);

		BOOL bIsGraphics = FALSE;
		
		IASCGraphicsRenderer* pGR = NULL;
		pRenderer->QueryInterface(__uuidof(IASCGraphicsRenderer), (void**)&pGR);

		if (NULL != pGR)
		{
			bIsGraphics = TRUE;
		}

		RELEASEINTERFACE(pGR);

		if (0 == lFileType)
		{
			// картинка
			if (bIsGraphics)
			{
				pRenderer->DrawImageFromFile(strFile, dX, dY, dW, dH);
			}
			else
			{
				IUnknown* pImage = ImageStudio::ISLoadImage(strFile);
				pRenderer->DrawImage(pImage, dX, dY, dW, dH);
				RELEASEINTERFACE(pImage);
			}
		}
		else if (1 == lFileType || 2 == lFileType)
		{
			// Запоминаем все настройки Brush, Font, Pen, Shadow и т.д.
			NSStructures::CBrush oBrush;
			BSTR bsBrushTexturePath = NULL;
			pRenderer->get_BrushAlpha1( &oBrush.Alpha1 );
			pRenderer->get_BrushAlpha2( &oBrush.Alpha2 );
			pRenderer->get_BrushColor1( &oBrush.Color1 );
			pRenderer->get_BrushColor2( &oBrush.Color2 );
			pRenderer->get_BrushLinearAngle( &oBrush.LinearAngle );
			pRenderer->get_BrushTextureMode( &oBrush.TextureMode );
			pRenderer->get_BrushTextureAlpha( &oBrush.TextureAlpha );
			pRenderer->get_BrushTexturePath( &bsBrushTexturePath );
			pRenderer->get_BrushType( &oBrush.Type );

			NSStructures::CFont oFont;
			BSTR bsFontPath = NULL, bsFontName = NULL;
			pRenderer->get_FontName( &bsFontName );
			pRenderer->get_FontPath( &bsFontPath );
			pRenderer->get_FontSize( &oFont.Size );
			LONG lStyle;
			pRenderer->get_FontStyle( &lStyle );
			oFont.Bold      = lStyle & FontConstants::FontStyleBold;
			oFont.Italic    = lStyle & FontConstants::FontStyleItalic;
			oFont.Strikeout = lStyle & FontConstants::FontStyleStrikeout;
			oFont.Underline = lStyle & FontConstants::FontStyleUnderline;
			pRenderer->get_FontStringGID( &oFont.StringGID );

			NSStructures::CPen oPen;
			pRenderer->get_PenAlign( &oPen.Align );
			pRenderer->get_PenAlpha( &oPen.Alpha );
			pRenderer->get_PenColor( &oPen.Color );


			IAVSImage *pImage = NULL;
			CoCreateInstance( __uuidof(CAVSImage), NULL, CLSCTX_ALL, __uuidof(IAVSImage), (void**)(&pImage) );

			pImage->put_FontManager(NULL);

			pImage->LoadFromFile( strFile );
			pImage->DrawOnRenderer( pRenderer, dX, dY, dW, dH );

			RELEASEINTERFACE( pImage );

			// Восстанавливаем параметры

			pRenderer->put_BrushAlpha1( oBrush.Alpha1 );
			pRenderer->put_BrushAlpha2( oBrush.Alpha2 );
			pRenderer->put_BrushColor1( oBrush.Color1 );
			pRenderer->put_BrushColor2( oBrush.Color2 );
			pRenderer->put_BrushLinearAngle( oBrush.LinearAngle );
			pRenderer->put_BrushTextureMode( oBrush.TextureMode );
			pRenderer->put_BrushTextureAlpha( oBrush.TextureAlpha );
			pRenderer->put_BrushTexturePath( bsBrushTexturePath ); if ( bsBrushTexturePath ) ::SysFreeString( bsBrushTexturePath );
			pRenderer->put_BrushType( oBrush.Type );

			pRenderer->put_FontName( bsFontName ); if ( bsFontName ) ::SysFreeString( bsFontName );
			pRenderer->put_FontPath( bsFontPath ); if ( bsFontPath ) ::SysFreeString( bsFontPath );
			pRenderer->put_FontSize( oFont.Size );
			pRenderer->put_FontStyle( oFont.GetStyle() );
			pRenderer->put_FontStringGID( oFont.StringGID );

			pRenderer->put_PenAlign( oPen.Align );
			pRenderer->put_PenAlpha( oPen.Alpha );
			pRenderer->put_PenColor( oPen.Color );

			// на всякий случай скидываем path
			pRenderer->PathCommandEnd();
		}
		else if (2 == lFileType)
		{		
			if (bIsGraphics)
			{
				pRenderer->DrawImageFromFile(strFile, dX, dY, dW, dH);
				return;
			}

			NSDocumentViewer::CDIB oSection;
		
			double dDpiX = 0;
			double dDpiY = 0;
			pRenderer->get_DpiX(&dDpiX);
			pRenderer->get_DpiY(&dDpiY);

			LONG lPixW = (LONG)dW * dDpiX / NSDocumentViewer::c_dInch_to_MM;
			LONG lPixH = (LONG)dH * dDpiX / NSDocumentViewer::c_dInch_to_MM;

			oSection.Create(lPixW, lPixH, dDpiX, dDpiY);

			LONG lCount = lPixW * lPixH;
			memset(oSection.m_pBits, 0xFF, 4 * lCount);

			HDC hDC = GetDC(NULL);

			HDC memDC = CreateCompatibleDC(hDC);
			ReleaseDC(0, hDC);

			HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, oSection.m_hBitmap);

			RECT oRectPlay;
			oRectPlay.left		= 0;
			oRectPlay.top		= 0;
			oRectPlay.right		= lPixW;
			oRectPlay.bottom	= lPixH;

			HENHMETAFILE hMeta = GetEnhMetaFile(strFile);

			PlayEnhMetaFile(memDC, hMeta, &oRectPlay);
			DeleteEnhMetaFile(hMeta);

			MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;
			CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_INPROC, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pMediaData);

			pMediaData->put_ColorSpace(CSP_VFLIP | CSP_BGRA);
			pMediaData->put_Width(lPixW);
			pMediaData->put_Height(lPixH);
			pMediaData->SetDefaultStrides();
			pMediaData->AllocateBuffer(-1);

			BYTE* pPixels = NULL;
			pMediaData->get_Buffer(&pPixels);
			memcpy(pPixels, oSection.m_pBits, 4 * lPixW * lPixH);

			DeleteObject(hOldBitmap);
			DeleteDC(memDC);

			IUnknown* punkImage = NULL;
			pMediaData->QueryInterface(IID_IUnknown, (void**)&punkImage);

			RELEASEINTERFACE(pMediaData);

			pRenderer->DrawImage(punkImage, dX, dY, dW, dH);
			RELEASEINTERFACE(punkImage);
		}
	}
};

class CMetafile
{
private:
	inline LONG __strlen(const char* str)
	{
		const char* s = str;
		for (; *s != 0; ++s);
		return (LONG)(s - str);
	}
	inline LONG __wstrlen(const wchar_t* str)
	{
		const wchar_t* s = str;
		for (; *s != 0; ++s);
		return (LONG)(s - str);
	}


private:
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

		ctError							= 255
	};


public:
	// состояние
	NSStructures::CPen		m_oPen;
	NSStructures::CBrush	m_oBrush;
	NSStructures::CFont		m_oFont;
	NSStructures::CShadow	m_oShadow;
	NSStructures::CEdgeText	m_oEdge;

private:

	// сам метафайл
	BYTE* m_pBuffer;
	BYTE* m_pBufferMem;
	
	size_t m_lPosition;
	size_t m_lSize;

	CAtlArray<CString> m_arStrings;
	CAtlArray<IUnknown*> m_arImages;

	LONG m_lSizeofDouble;
	LONG m_lSizeofFloat;
	LONG m_lSizeofLONG;
	LONG m_lSizeofBYTE;

	// окончание пена
	Aggplus::CArrowHead m_oArrowHead;

	CRITICAL_SECTION m_oCS;
	IASCMetafile* m_pAdditionalCommands;

public:
	double m_fWidth;
	double m_fHeight;

public:
	CMetafile()
	{
		InitializeCriticalSection(&m_oCS);

		m_pAdditionalCommands = NULL;

		Clear();

		m_lSizeofDouble	= sizeof(double);
		m_lSizeofFloat 	= sizeof(float);
		m_lSizeofLONG	= sizeof(long);
		m_lSizeofBYTE   = sizeof(BYTE);
	}
	~CMetafile()
	{
		RELEASEARRAYOBJECTS(m_pBuffer);

		size_t nCount = m_arImages.GetCount();
		for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
		{
			IUnknown* punkImage = m_arImages[nIndex];
			RELEASEINTERFACE(punkImage);
		}

		m_arImages.RemoveAll();

		RELEASEINTERFACE(m_pAdditionalCommands);

		DeleteCriticalSection(&m_oCS);
	}

protected:

	inline void Clear()
	{
		m_lSize		= 0;
		m_lPosition = 0;

		m_pBuffer = NULL;

		m_arImages.RemoveAll();
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
	inline void WriteDouble(const double& dValue)
	{
		CheckBufferSize(m_lSizeofDouble);

		*((double*)(m_pBuffer + m_lPosition)) = dValue; 
		m_lPosition += m_lSizeofDouble;
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


public:
	
	// pen ---------------------------------------------------------------------------------	
	inline void PenXML(BSTR bsXml)
	{
		NSStructures::CPen oNewPen;
		oNewPen.FromXmlString(CString(bsXml));

		PenColor(oNewPen.Color);
		PenAlpha(oNewPen.Alpha);
		PenSize(oNewPen.Size);
		PenDashStyle(oNewPen.DashStyle);
		PenLineStartCap(oNewPen.LineStartCap);
		PenLineEndCap(oNewPen.LineEndCap);
		PenLineJoin(oNewPen.LineJoin);
		PenDashPatern(oNewPen.DashPattern, oNewPen.Count);
		PenDashOffset(oNewPen.DashOffset);
		PenAlign(oNewPen.Align);
	}
	inline void Pen(NSStructures::CPen* pNewPen)
	{
		PenColor(pNewPen->Color);
		PenAlpha(pNewPen->Alpha);
		PenSize(pNewPen->Size);
		PenDashStyle(pNewPen->DashStyle);
		PenLineStartCap(pNewPen->LineStartCap);
		PenLineEndCap(pNewPen->LineEndCap);
		PenLineJoin(pNewPen->LineJoin);
		PenDashPatern(pNewPen->DashPattern, pNewPen->Count);
		PenDashOffset(pNewPen->DashOffset);
		PenAlign(pNewPen->Align);
	}
	inline void PenXMLAttack(BSTR bsXml)
	{
		WriteBSTR(ctPenXML, bsXml);
	}
	inline void PenColor(const LONG& lColor)
	{
		if (m_oPen.Color != lColor)
		{
			m_oPen.Color = lColor;
			WriteLONG(ctPenColor, lColor);
		}
	}
	inline void PenAlpha(const LONG& lAlpha)
	{
		if (m_oPen.Alpha != lAlpha)
		{
			m_oPen.Alpha = lAlpha;
			WriteBYTE(ctPenAlpha, (BYTE)lAlpha);
		}
	}
	inline void PenSize(const double& dSize)
	{
		if (m_oPen.Size != dSize)
		{
			m_oPen.Size = dSize;
			WriteDouble(ctPenSize, dSize);
		}
	}
	inline void PenDashStyle(const BYTE& val)
	{
		if (m_oPen.DashStyle != val)
		{
			m_oPen.DashStyle = val;
			WriteBYTE(ctPenDashStyle, val);
		}
	}
	inline void PenLineStartCap(const BYTE& val)
	{
		if (m_oPen.LineStartCap != val)
		{
			m_oPen.LineStartCap = val;
			WriteBYTE(ctPenLineStartCap, val);
		}
	}
	inline void PenLineEndCap(const BYTE& val)
	{
		if (m_oPen.LineEndCap != val)
		{
			m_oPen.LineEndCap = val;
			m_oArrowHead.SetType(val);
			WriteBYTE(ctPenLineEndCap, val);
		}
	}
	inline void PenLineJoin(const BYTE& val)
	{
		if (m_oPen.LineJoin != val)
		{
			m_oPen.LineJoin = val;
			WriteBYTE(ctPenLineJoin, val);
		}
	}
	inline void PenDashPatern(double* pPattern, LONG lCount)
	{
		if ((NULL != pPattern) && (0 < lCount))
		{
			m_oPen.SetDashPattern(pPattern, lCount);

			LONG lSizeMem = lCount * m_lSizeofDouble;
			LONG lMem = m_lSizeofBYTE + m_lSizeofLONG + lSizeMem;

			CheckBufferSize(lMem);

			*(m_pBuffer + m_lPosition)	= (BYTE)ctPenDashPatern; 
			m_lPosition += m_lSizeofBYTE;

			*((LONG*)(m_pBuffer + m_lPosition))	= lCount; 
			m_lPosition += m_lSizeofLONG;

			memcpy(m_pBuffer + m_lPosition, pPattern, lSizeMem);
			m_lPosition += lSizeMem;
			
		}
	}
	inline void PenDashOffset(const double& fOffset)
	{
		if (m_oPen.DashOffset != fOffset)
		{
			m_oPen.DashOffset = fOffset;
			WriteDouble(ctPenDashOffset, fOffset);
		}
	}
	inline void PenAlign(const LONG& val)
	{
		if (m_oPen.Align != val)
		{
			m_oPen.Align = val;
			WriteLONG(ctPenAlign, val);
		}
	}
	inline void PenMiterLimit(const double& val)
	{
		if (m_oPen.MiterLimit != val)
		{
			m_oPen.MiterLimit = val;
			WriteDouble(ctPenMiterLimit, val);
		}
	}
	// -------------------------------------------------------------------------------------	

	// brush -------------------------------------------------------------------------------	
		
	inline void BrushXML(BSTR bsXml)
	{
		NSStructures::CBrush oNewBrush;
		oNewBrush.FromXmlString(CString(bsXml));

		BrushType(oNewBrush.Type);

		if (m_oBrush.IsOneColor())
		{
			BrushColor1(oNewBrush.Color1);
			BrushAlpha1(oNewBrush.Alpha1);
		}
		else if (m_oBrush.IsTexture())
		{
			BrushTexturePath(oNewBrush.TexturePath);
			BrushTextureMode(oNewBrush.TextureMode);
			BrushTextureAlpha(oNewBrush.TextureAlpha);
			BrushRect(oNewBrush.Rectable, oNewBrush.Rect);

			if( oNewBrush.Type == c_BrushTypePattern )
			{
				BrushColor1(oNewBrush.Color1);
				BrushAlpha1(oNewBrush.Alpha1);
				BrushColor2(oNewBrush.Color2);
				BrushAlpha2(oNewBrush.Alpha2);
			}
		}
		else if (m_oBrush.IsTwoColor())
		{
			BrushColor1(oNewBrush.Color1);
			BrushAlpha1(oNewBrush.Alpha1);
			BrushColor2(oNewBrush.Color2);
			BrushAlpha2(oNewBrush.Alpha2);

			BrushLinearAngle( oNewBrush.LinearAngle );
			BrushSubColors( oNewBrush.m_arrSubColors );
		}
	}
	inline void Brush(NSStructures::CBrush* pBrush)
	{
		BrushType(pBrush->Type);

		if (m_oBrush.IsOneColor())
		{
			BrushColor1(pBrush->Color1);
			BrushAlpha1(pBrush->Alpha1);
		}
		else if (m_oBrush.IsTexture())
		{
			BrushTexturePath(pBrush->TexturePath);
			BrushTextureMode(pBrush->TextureMode);
			BrushTextureAlpha(pBrush->TextureAlpha);
			BrushRect(pBrush->Rectable, pBrush->Rect);
		}
		else if (m_oBrush.IsTwoColor())
		{
			BrushColor1(pBrush->Color1);
			BrushAlpha1(pBrush->Alpha1);
			BrushColor2(pBrush->Color2);
			BrushAlpha2(pBrush->Alpha2);

			BrushLinearAngle( pBrush->LinearAngle );
		}
	}

	inline void BrushXMLAttack(BSTR bsXml)
	{
		WriteBSTR(ctBrushXML, bsXml);
	}
	inline void BrushType(const LONG& val)
	{
		if (m_oBrush.Type != val)
		{
			m_oBrush.Type = val;
			WriteLONG(ctBrushType, val);
		}
	}
	inline void BrushColor1(const LONG& val)
	{
		if (m_oBrush.Color1 != val)
		{
			m_oBrush.Color1 = val;
			WriteLONG(ctBrushColor1, val);
		}
	}
	inline void BrushColor2(const LONG& val)
	{
		if (m_oBrush.Color2 != val)
		{
			m_oBrush.Color2 = val;
			WriteLONG(ctBrushColor2, val);
		}
	}
	inline void BrushAlpha1(const LONG& val)
	{
		if (m_oBrush.Alpha1 != val)
		{
			m_oBrush.Alpha1 = val;
			WriteBYTE(ctBrushAlpha1, (BYTE)val);
		}
	}
	inline void BrushAlpha2(const LONG& val)
	{
		if (m_oBrush.Alpha2 != val)
		{
			m_oBrush.Alpha2 = val;
			WriteBYTE(ctBrushAlpha2, (BYTE)val);
		}
	}

	inline void BrushTextureAlpha(const LONG& val)
	{
		if (m_oBrush.TextureAlpha != val)
		{
			m_oBrush.TextureAlpha = val;
			WriteBYTE(ctBrushTextureAlpha, (BYTE)val);
		}
	}
	inline void BrushTextureMode(const LONG& val)
	{
		if (m_oBrush.TextureMode != val)
		{
			m_oBrush.TextureMode = val;
			WriteLONG(ctBrushTextureMode, val);
		}
	}
	inline void BrushRect(const BOOL& bRectable, const Gdiplus::RectF& oRect)
	{
		WriteBYTE(ctBrushRectable, (BYTE)bRectable);

		if (bRectable)
		{
			WriteFloat(oRect.X);
			WriteFloat(oRect.Y);
			WriteFloat(oRect.Width);
			WriteFloat(oRect.Height);
		}
	}
	inline void BrushTexturePath(const CString& strPath)
	{
		if (m_oBrush.TexturePath != strPath)
		{
			m_oBrush.TexturePath = strPath;
			
			BSTR bsPath = strPath.AllocSysString();
			WriteBSTR(ctBrushTexturePath, bsPath);
			SysFreeString(bsPath);
		}
	}
	inline void BrushLinearAngle(const double& val)
	{
		if (m_oBrush.LinearAngle != val)
		{
			m_oBrush.LinearAngle = val;
			WriteDouble(ctBrushAngle, val);
		}
	}
	inline void BrushSubColors( const ATL::CSimpleArray<NSStructures::CBrush::TSubColor>& arrSubColors )
	{
		m_oBrush.m_arrSubColors = arrSubColors;
		
		int count = arrSubColors.GetSize();
		if( count )
		{
			int datasize = (count * 2 + 1) * m_lSizeofLONG + m_lSizeofBYTE;

			CheckBufferSize(datasize);

			BYTE* data = m_pBuffer + m_lPosition;
			m_lPosition += datasize;

			data[0] = ctBrushSubColors;
			long* buffer = (long*)(data + 1);

			buffer[0] = count;

			for( int i = 0; i < count; i++ )
			{
				buffer[i * 2 + 1] = arrSubColors[i].color;
				buffer[i * 2 + 2] = arrSubColors[i].position;
			}
		}
	}
	// -------------------------------------------------------------------------------------	

	// font --------------------------------------------------------------------------------
	inline void FontXML(BSTR bsXml)
	{
		NSStructures::CFont oNewFont;
		oNewFont.FromXmlString(CString(bsXml));

		FontName(oNewFont.Name);
		FontPath(oNewFont.Path);
		FontSize(oNewFont.Size);
		FontStyle(oNewFont.GetStyle());
		FontStringGID(oNewFont.StringGID);
		FontCharSpace(oNewFont.CharSpace);
	}

	inline void Font(NSStructures::CFont* pNewFont)
	{
		FontName(pNewFont->Name);
		FontPath(pNewFont->Path);
		FontSize(pNewFont->Size);
		FontStyle(pNewFont->GetStyle());
		FontStringGID(pNewFont->StringGID);
	}

	inline void FontXMLAttack(BSTR bsXml)
	{
		WriteBSTR(ctFontXML, bsXml);
	}
	inline void FontName(CString strName)
	{
		if (m_oFont.Name != strName)
		{
			m_oFont.Name = strName;
			WriteBSTR(ctFontName, (BSTR)(strName.GetBuffer()));
		}
	}
	inline void FontPath(CString strName)
	{
		if (m_oFont.Path != strName)
		{
			m_oFont.Path = strName;
			WriteBSTR(ctFontPath, (BSTR)(strName.GetBuffer()));
		}
	}
	inline void FontSize(const double& dSize)
	{
		if (m_oFont.Size != dSize)
		{
			m_oFont.Size = dSize;
			WriteDouble(ctFontSize, dSize);
		}
	}
	inline void FontStyle(const LONG& lStyle)
	{
		if (m_oFont.GetStyle() != lStyle)
		{
			m_oFont.SetStyle(lStyle);
			WriteLONG(ctFontStyle, lStyle);
		}
	}
	inline void FontStringGID(const BOOL& bGID)
	{
		if (m_oFont.StringGID != bGID)
		{
			m_oFont.StringGID = bGID;
			WriteBYTE(ctFontGID, (BYTE)bGID);
		}
	}
	inline void FontCharSpace(const double& dSpace)
	{
		if (m_oFont.CharSpace != dSpace)
		{
			m_oFont.CharSpace = dSpace;
			WriteDouble(ctFontCharSpace, dSpace);
		}
	}
	// -------------------------------------------------------------------------------------

	// shadow ------------------------------------------------------------------------------

	inline void ShadowXML(BSTR bsXml)
	{
		NSStructures::CShadow oNewShadow;
		oNewShadow.FromXmlString(CString(bsXml));
		
		ShadowVisible(oNewShadow.Visible);

		if (m_oShadow.Visible)
		{
			ShadowDistanceX(oNewShadow.DistanceX);
			ShadowDistanceY(oNewShadow.DistanceY);
			ShadowBlurSize(oNewShadow.BlurSize);
			ShadowColor(oNewShadow.Color);
			ShadowAlpha(oNewShadow.Alpha);
		}
	}
	inline void Shadow(NSStructures::CShadow* pNewShadow)
	{
		ShadowVisible(pNewShadow->Visible);

		if (m_oShadow.Visible)
		{
			ShadowDistanceX(pNewShadow->DistanceX);
			ShadowDistanceY(pNewShadow->DistanceY);
			ShadowBlurSize(pNewShadow->BlurSize);
			ShadowColor(pNewShadow->Color);
			ShadowAlpha(pNewShadow->Alpha);
		}
	}

	inline void ShadowXMLAttack(BSTR bsXml)
	{
		WriteBSTR(ctShadowXML, bsXml);
	}
	inline void ShadowVisible(const BOOL& bVis)
	{
		if (m_oShadow.Visible != bVis)
		{
			m_oShadow.Visible = bVis;
			WriteBYTE(ctShadowVisible, (BYTE)bVis);
		}
	}
	inline void ShadowDistanceX(const double& val)
	{
		if (m_oShadow.DistanceX != val)
		{
			m_oShadow.DistanceX = val;
			WriteDouble(ctShadowDistanceX, val);
		}
	}
	inline void ShadowDistanceY(const double& val)
	{
		if (m_oShadow.DistanceY != val)
		{
			m_oShadow.DistanceY = val;
			WriteDouble(ctShadowDistanceY, val);
		}
	}
	inline void ShadowBlurSize(const double& val)
	{
		if (m_oShadow.BlurSize != val)
		{
			m_oShadow.BlurSize = val;
			WriteDouble(ctShadowBlurSize, val);
		}
	}
	inline void ShadowColor(const LONG& val)
	{
		if (m_oShadow.Color != val)
		{
			m_oShadow.Color = val;
			WriteLONG(ctShadowColor, val);
		}
	}
	inline void ShadowAlpha(const LONG& val)
	{
		if (m_oShadow.Alpha != val)
		{
			m_oShadow.Alpha = val;
			WriteBYTE(ctShadowAlpha, (BYTE)val);
		}
	}
	// -------------------------------------------------------------------------------------

	// edge --------------------------------------------------------------------------------
	
	inline void EdgeXML(BSTR bsXml)
	{
		NSStructures::CEdgeText oNewEdge;
		oNewEdge.FromXmlString(CString(bsXml));

		EdgeVisible(oNewEdge.Visible);

		if (0 != m_oEdge.Visible)
		{
			EdgeDist(oNewEdge.Dist);
			EdgeColor(oNewEdge.Color);
			EdgeAlpha(oNewEdge.Alpha);
		}
	}
	inline void Edge(NSStructures::CEdgeText* pNewEdge)
	{
		EdgeVisible(pNewEdge->Visible);

		if (0 != m_oEdge.Visible)
		{
			EdgeDist(pNewEdge->Dist);
			EdgeColor(pNewEdge->Color);
			EdgeAlpha(pNewEdge->Alpha);
		}
	}
	inline void EdgeXMLAttack(BSTR bsXml)
	{
		WriteBSTR(ctEdgeXML, bsXml);
	}
	inline void EdgeVisible(const LONG& val)
	{
		if (m_oEdge.Visible != val)
		{
			m_oEdge.Visible = val;
			WriteBYTE(ctEdgeVisible, (BYTE)val);
		}
	}
	inline void EdgeDist(const double& val)
	{
		if (m_oEdge.Dist != val)
		{
			m_oEdge.Dist = val;
			WriteDouble(ctEdgeDistance, val);
		}
	}
	inline void EdgeColor(const LONG& val)
	{
		if (m_oEdge.Color != val)
		{
			m_oEdge.Color = val;
			WriteLONG(ctEdgeColor, val);
		}
	}
	inline void EdgeAlpha(const LONG& val)
	{
		if (m_oEdge.Alpha != val)
		{
			m_oEdge.Alpha = val;
			WriteLONG(ctEdgeAlpha, val);
		}
	}
	// -------------------------------------------------------------------------------------
	inline void PathCommandMoveTo(double fX, double fY)
	{
		Write(ctPathCommandMoveTo, fX, fY);
	}
	inline void PathCommandLineTo(double fX, double fY)
	{
		Write(ctPathCommandLineTo, fX, fY);
		m_oArrowHead.PathCommandLineTo(fX, fY);
	}
	inline void PathCommandLinesTo(SAFEARRAY* pPoints)
	{
		LONG lCount = pPoints->rgsabound[0].cElements;
		double* pData = (double*)pPoints->pvData;

		Write(ctPathCommandLinesTo, lCount, pData);

		m_oArrowHead.PathCommandLinesTo(pPoints);
	}

	inline void PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
	{
		Write(ctPathCommandCurveTo, fX1, fY1, fX2, fY2, fX3, fY3);
		m_oArrowHead.PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}
	inline void PathCommandCurvesTo(SAFEARRAY* pPoints)
	{
		LONG lCount = pPoints->rgsabound[0].cElements;
		double* pData = (double*)pPoints->pvData;

		Write(ctPathCommandCurvesTo, lCount, pData);

		m_oArrowHead.PathCommandCurvesTo(pPoints);
	}
	inline void PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
	{
		Write(ctPathCommandArcTo, fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
		m_oArrowHead.PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
	}
	inline void PathCommandClose()
	{
		WriteCommandType(ctPathCommandClose);
		m_oArrowHead.PathCommandClose();
	}
	inline void PathCommandEnd()
	{
		WriteCommandType(ctPathCommandEnd);
		m_oArrowHead.PathCommandEnd();
	}

	inline void PathCommandStart()
	{
		WriteCommandType(ctPathCommandStart);
		m_oArrowHead.PathCommandStart();
	}

	inline void PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseOffset)
	{
		LONG lLen = __wstrlen(bsText) + 1;
		if (1 == lLen)
			return;

		lLen *= sizeof(wchar_t);

		size_t lMem = m_lSizeofBYTE + m_lSizeofLONG + 5 * m_lSizeofDouble + lLen;

		CheckBufferSize(lMem);

		*(m_pBuffer + m_lPosition)	= (BYTE)ctPathCommandText;					m_lPosition += m_lSizeofBYTE;

		*((LONG*)(m_pBuffer + m_lPosition))	= lLen;								m_lPosition += m_lSizeofLONG;
		memcpy(m_pBuffer + m_lPosition, bsText, lLen);
		m_lPosition += lLen;

		*((double*)(m_pBuffer + m_lPosition))= fX;								m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fY;								m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fWidth;							m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fHeight;							m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fBaseOffset;						m_lPosition += m_lSizeofDouble;
	}
	inline void DrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseOffset)
	{
		LONG lLen = __wstrlen(bsText) + 1;
		if (1 == lLen)
			return;

		lLen *= sizeof(wchar_t);

		size_t lMem = m_lSizeofBYTE + m_lSizeofLONG + 5 * m_lSizeofDouble + lLen;

		CheckBufferSize(lMem);

		*(m_pBuffer + m_lPosition)	= (BYTE)ctDrawText;							m_lPosition += m_lSizeofBYTE;

		*((LONG*)(m_pBuffer + m_lPosition))	= lLen;								m_lPosition += m_lSizeofLONG;
		memcpy(m_pBuffer + m_lPosition, bsText, lLen);
		m_lPosition += lLen;

		*((double*)(m_pBuffer + m_lPosition))= fX;								m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fY;								m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fWidth;							m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fHeight;							m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fBaseOffset;						m_lPosition += m_lSizeofDouble;
	}
	inline void DrawTextEx(BSTR bsText, BSTR bsGid, BSTR bsSource, 
		double fX, double fY, double fWidth, double fHeight, double fBaseOffset, DWORD lFlags, BOOL bIsPath = FALSE)
	{
		LONG lLen1 = __wstrlen(bsText);
		LONG lLen2 = __wstrlen(bsGid);
		LONG lLen3 = __wstrlen(bsSource);

		if (0 == (lLen1 + lLen2 + lLen3))
			return;
				
		LONG lSizeofWCHAR = sizeof(WCHAR);

		lLen1 = (lLen1 + 1) * lSizeofWCHAR;
		lLen2 = (lLen2 + 1) * lSizeofWCHAR;
		lLen3 = (lLen3 + 1) * lSizeofWCHAR;

		size_t lMem = m_lSizeofBYTE + 3 * m_lSizeofLONG + lLen1 + lLen2 + lLen3 + 5 * m_lSizeofDouble + m_lSizeofLONG;

		CheckBufferSize(lMem);

		if (!bIsPath)
		{
			*(m_pBuffer + m_lPosition)	= (BYTE)ctDrawTextEx;					m_lPosition += m_lSizeofBYTE;
		}
		else
		{
			*(m_pBuffer + m_lPosition)	= (BYTE)ctPathCommandTextEx;			m_lPosition += m_lSizeofBYTE;
		}

		
		*((LONG*)(m_pBuffer + m_lPosition))	= lLen1;			m_lPosition += m_lSizeofLONG;
		if (1 != lLen1)
		{
			memcpy(m_pBuffer + m_lPosition, bsText, lLen1);		m_lPosition += lLen1;
		}
		*((LONG*)(m_pBuffer + m_lPosition))	= lLen2;			m_lPosition += m_lSizeofLONG;
		if (1 != lLen2)
		{
			memcpy(m_pBuffer + m_lPosition, bsGid, lLen2);   	m_lPosition += lLen2;
		}
		*((LONG*)(m_pBuffer + m_lPosition))	= lLen3;			m_lPosition += m_lSizeofLONG;
		if (1 != lLen3)
		{
			memcpy(m_pBuffer + m_lPosition, bsSource, lLen3);	m_lPosition += lLen3;
		}

		*((double*)(m_pBuffer + m_lPosition))= fX;								m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fY;								m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fWidth;							m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fHeight;							m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fBaseOffset;						m_lPosition += m_lSizeofDouble;

		//*((DWORD*)(m_pBuffer + m_lPosition)) = lFlags;						m_lPosition += m_lSizeofLONG;
	}
	inline void DrawPath(LONG lType)
	{
		WriteLONG(ctDrawPath, lType);

		if (m_oArrowHead.DrawPath(lType))
		{
			LONG lColor1 = m_oBrush.Color1;
			LONG lAlpha1 = m_oBrush.Alpha1;
			LONG lType   = m_oBrush.Type;

			BrushColor1(m_oPen.Color);
			BrushAlpha1(m_oPen.Alpha);
			BrushType(Aggplus::BrushTypeSolidColor);
			
			double lpredx = m_oArrowHead.m_dX1 - m_oArrowHead.m_dX2;
			double lpredy = m_oArrowHead.m_dY1 - m_oArrowHead.m_dY2;
			double length = sqrt(lpredx * lpredx + lpredy * lpredy);
			lpredx = lpredx / length;
			lpredy = lpredy / length;

			double sin30 = 0.5;
			double cos30 = sqrt(3.0) / 2.0;

			double lpredxp = cos30 * lpredx - sin30 * lpredy;
			double lpredyp = sin30 * lpredx + cos30 * lpredy;
			double lpredxn = cos30 * lpredx + sin30 * lpredy;
			double lpredyn = - sin30 * lpredx + cos30 * lpredy;

			//double lengthp = sqrt(lpredxp * lpredxp + lpredyp * lpredyp);
			//double lengthn = sqrt(lpredxn * lpredxn + lpredyn * lpredyn);
			lpredxp *= 2.0;
			lpredyp *= 2.0;
			lpredxn *= 2.0;
			lpredyn *= 2.0;

			WriteCommandType(ctPathCommandEnd);
			WriteCommandType(ctPathCommandStart);

			Write(ctPathCommandMoveTo, m_oArrowHead.m_dX2, m_oArrowHead.m_dY2);
			Write(ctPathCommandLineTo, m_oArrowHead.m_dX2 + lpredxp, m_oArrowHead.m_dY2 + lpredyp);
			Write(ctPathCommandLineTo, m_oArrowHead.m_dX2 + lpredxn, m_oArrowHead.m_dY2 + lpredyn);
			WriteCommandType(ctPathCommandClose);

			WriteLONG(ctDrawPath, 0x100);
			
			BrushColor1(lColor1);
			BrushAlpha1(lAlpha1);
			BrushType(lType);
		}
	}
	// -------------------------------------------------------------------------------------

	inline void DrawImage(IUnknown* punkImage, double fX, double fY, double fWidth, double fHeight)
	{
		size_t lMem = m_lSizeofBYTE + m_lSizeofLONG + 4 * m_lSizeofDouble;

		CheckBufferSize(lMem);

		*(m_pBuffer + m_lPosition)	= (BYTE)ctDrawImage;					m_lPosition += m_lSizeofBYTE;

		*((LONG*)(m_pBuffer + m_lPosition))	= (LONG)m_arImages.GetCount();	m_lPosition += m_lSizeofLONG;

		*((double*)(m_pBuffer + m_lPosition))= fX;							m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fY;							m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fWidth;						m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fHeight;						m_lPosition += m_lSizeofDouble;

		ADDREFINTERFACE(punkImage);
		m_arImages.Add(punkImage);
	}
	// ---------------------------------------------
	inline void DrawImageFromFile(BSTR bstrPath, double fX, double fY, double fWidth, double fHeight)
	{
		LONG lLen = __wstrlen(bstrPath) + 1;
		if (1 == lLen)
			return;

		lLen *= sizeof(wchar_t);

		size_t lMem = m_lSizeofBYTE + m_lSizeofLONG + 4 * m_lSizeofDouble + lLen;

		CheckBufferSize(lMem);

		*(m_pBuffer + m_lPosition)	= (BYTE)ctDrawImageFromFile;				m_lPosition += m_lSizeofBYTE;

		*((LONG*)(m_pBuffer + m_lPosition))	= lLen;								m_lPosition += m_lSizeofLONG;
		memcpy(m_pBuffer + m_lPosition, bstrPath, lLen);
		m_lPosition += lLen;

		*((double*)(m_pBuffer + m_lPosition))= fX;								m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fY;								m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fWidth;							m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= fHeight;							m_lPosition += m_lSizeofDouble;
	}

	inline void SetCommandParams(double dAngle,  double dLeft,  double dTop,  double dWidth,  double dHeight,  DWORD lFlags)
	{
		size_t lMem = m_lSizeofBYTE + m_lSizeofLONG + 5 * m_lSizeofDouble;

		CheckBufferSize(lMem);

		*(m_pBuffer + m_lPosition)	= (BYTE)ctSetParams;	m_lPosition += m_lSizeofBYTE;

		*((double*)(m_pBuffer + m_lPosition))= dAngle;		m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= dLeft;		m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= dTop;		m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= dWidth;		m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= dHeight;		m_lPosition += m_lSizeofDouble;

		*((LONG*)(m_pBuffer + m_lPosition))	= lFlags;		m_lPosition += m_lSizeofLONG;
	}

	inline void SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		size_t lMem = m_lSizeofBYTE + 6 * m_lSizeofDouble;

		CheckBufferSize(lMem);

		*(m_pBuffer + m_lPosition)	= (BYTE)ctSetTransform;		m_lPosition += m_lSizeofBYTE;

		*((double*)(m_pBuffer + m_lPosition))= dA;				m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= dB;				m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= dC;				m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= dD;				m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= dE;				m_lPosition += m_lSizeofDouble;
		*((double*)(m_pBuffer + m_lPosition))= dF;				m_lPosition += m_lSizeofDouble;		
	}

	inline void ResetTransform()
	{
		WriteCommandType(ctResetTransform);
	}

	inline void WriteClipMode(LONG lMode)
	{
		WriteLONG(ctClipMode, lMode);
	}

	inline void BeginCommand(DWORD lType)
	{
		WriteLONG(ctBeginCommand, (LONG)lType);
	}
	inline void EndCommand(DWORD lType)
	{
		WriteLONG(ctEndCommand, lType);
	}

	inline void InitAdvancedCommands()
	{
	}
	inline void DestroyAdvancedCommands()
	{
	}

	inline void CommandLong(LONG lCommand)
	{
		WriteCommandType(ctCommandLong1);
		WriteLONG(lCommand);
	}
	inline void CommandDouble(double dCommand)
	{
		WriteCommandType(ctCommandDouble1);
		WriteDouble(dCommand);
	}
	inline void CommandString(BSTR bsCommand)
	{
		WriteCommandType(ctCommandString1);
		WriteBSTR(bsCommand);
	}
	inline void CommandLong2(LONG lType, LONG lCommand)
	{
		WriteCommandType(ctCommandLong2);
		WriteLONG(lType);
		WriteLONG(lCommand);
	}
	inline void CommandDouble2(LONG lType, double dCommand)
	{
		WriteCommandType(ctCommandDouble2);
		WriteLONG(lType);
		WriteDouble(dCommand);
	}
	inline void CommandString2(LONG lType, BSTR bsCommand)
	{
		WriteCommandType(ctCommandString2);
		WriteLONG(lType);
		WriteBSTR(bsCommand);
	}
public:
	inline LONG ReadLONG()
	{
		LONG l = *((LONG*)m_pBufferMem);
		m_pBufferMem += m_lSizeofLONG;
		return l;
	}
	inline float ReadFloat()
	{
		float l = *((float*)m_pBufferMem);
		m_pBufferMem += m_lSizeofFloat;
		return l;
	}
	inline BYTE ReadByte()
	{
		BYTE l = *m_pBufferMem;
		++m_pBufferMem;
		return l;
	}
	inline double ReadDouble()
	{
		double l = *((double*)m_pBufferMem);
		m_pBufferMem += m_lSizeofDouble;
		return l;
	}
	inline BSTR ReadBSTR()
	{
		LONG l = ReadLONG();
		BSTR bs = (BSTR)m_pBufferMem;
		m_pBufferMem += l;
		return bs;
	}


	inline LONG ReadLONG(BYTE*& pBufferMem)
	{
		LONG l = *((LONG*)pBufferMem);
		pBufferMem += m_lSizeofLONG;
		return l;
	}
	inline float ReadFloat(BYTE*& pBufferMem)
	{
		float l = *((float*)pBufferMem);
		pBufferMem += m_lSizeofFloat;
		return l;
	}
	inline BYTE ReadByte(BYTE*& pBufferMem)
	{
		BYTE l = *pBufferMem;
		++pBufferMem;
		return l;
	}
	inline double ReadDouble(BYTE*& pBufferMem)
	{
		double l = *((double*)pBufferMem);
		pBufferMem += m_lSizeofDouble;
		return l;
	}
	inline BSTR ReadBSTR(BYTE*& pBufferMem)
	{
		LONG l = ReadLONG(pBufferMem);
		BSTR bs = (BSTR)pBufferMem;
		pBufferMem += l;
		return bs;
	}

	inline BSTR ReadBSTR2(BYTE*& pBufferMem)
	{
		LONG l = ReadLONG(pBufferMem);
		
		if (1 == l)
		{
			pBufferMem += l;
			return NULL;
		}
		
		BSTR bs = (BSTR)pBufferMem;
		pBufferMem += l;
		return bs;
	}

public:
	BOOL Draw(IASCRenderer* pRenderer, BOOL* pBreak)
	{
		if (NULL == pRenderer)
			return FALSE;

		// выставляем размеры страницы
		pRenderer->put_Width(m_fWidth);
		pRenderer->put_Height(m_fHeight);

		// отрисовываем все объекты...
		m_pBufferMem = m_pBuffer;

		// здесь старая команда нужна для того, чтобы не посылать подряд хмлки с пеном, брашем...
		// (например выставился цвет и альфа - а надо послать только один (!!!) раз)
		CommandType eOldCommand = ctError;
		CommandType eCurCommand = ctError;

		volatile BOOL* pCancel = pBreak;

		HRESULT hRes = S_OK;
		while (m_lPosition > ((size_t)(m_pBufferMem - m_pBuffer)))
		{
			eCurCommand = (CommandType)ReadByte();
			
			switch (eCurCommand)
			{
			case ctPenXML:
				{
					hRes = pRenderer->SetPen(ReadBSTR());
					break;
				}
			case ctBrushXML:
				{
					hRes = pRenderer->SetBrush(ReadBSTR());
					break;
				}
			case ctFontXML:
				{
					hRes = pRenderer->SetFont(ReadBSTR());
					break;
				}
			case ctShadowXML:
				{
					hRes = pRenderer->SetShadow(ReadBSTR());					
					break;
				}
			case ctEdgeXML:
				{
					hRes = pRenderer->SetEdgeText(ReadBSTR());	
					break;
				}

			case ctDrawText:
			case ctPathCommandText:
				{
					BSTR bsText			= ReadBSTR();

					double fX			= ReadDouble();
					double fY			= ReadDouble();
					double fWidth		= ReadDouble();
					double fHeight		= ReadDouble();
					double fBaseOffset	= ReadDouble();

					if (ctDrawText == eCurCommand)
					{
						hRes = pRenderer->CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseOffset);
					}
					else
					{
						hRes = pRenderer->PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseOffset);
					}

					break;
				}

			case ctPathCommandMoveTo:
				{
					double fX = ReadDouble();
					double fY = ReadDouble();

					hRes = pRenderer->PathCommandMoveTo(fX, fY);
					
					break;
				}
			case ctPathCommandLineTo:
				{
					double fX = ReadDouble();
					double fY = ReadDouble();

					hRes = pRenderer->PathCommandLineTo(fX, fY);
					
					break;
				}
			case ctPathCommandLinesTo:
				{
					LONG lCount = ReadLONG();
					ULONG lFloats = lCount * m_lSizeofDouble;

					SAFEARRAYBOUND rgsab;
					rgsab.lLbound	= 0;
					rgsab.cElements	= lCount;

					SAFEARRAY* pArray = SafeArrayCreate(VT_R8, 1, &rgsab);
					memcpy(pArray->pvData, m_pBufferMem, lFloats);

					m_pBufferMem += lFloats;

					hRes = pRenderer->PathCommandLinesTo(pArray);

					RELEASEARRAY(pArray);
					
					break;
				}
			case ctPathCommandCurveTo:
				{
					double fX1 = ReadDouble();
					double fY1 = ReadDouble();
					double fX2 = ReadDouble();
					double fY2 = ReadDouble();
					double fX3 = ReadDouble();
					double fY3 = ReadDouble();

					hRes = pRenderer->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
					
					break;
				}
			case ctPathCommandCurvesTo:
				{
					LONG lCount = ReadLONG();
					ULONG lFloats = lCount * m_lSizeofDouble;

					SAFEARRAYBOUND rgsab;
					rgsab.lLbound	= 0;
					rgsab.cElements	= lCount;

					SAFEARRAY* pArray = SafeArrayCreate(VT_R8, 1, &rgsab);
					memcpy(pArray->pvData, m_pBufferMem, lFloats);

					m_pBufferMem += lFloats;

					hRes = pRenderer->PathCommandCurvesTo(pArray);

					RELEASEARRAY(pArray);
					
					break;
				}
			case ctPathCommandArcTo:
				{
					double fX1 = ReadDouble();
					double fY1 = ReadDouble();
					double fX2 = ReadDouble();
					double fY2 = ReadDouble();
					double fX3 = ReadDouble();
					double fY3 = ReadDouble();

					hRes = pRenderer->PathCommandArcTo(fX1, fY1, fX2, fY2, fX3, fY3);
					
					break;
				}
			case ctPathCommandClose:
				{
					hRes = pRenderer->PathCommandClose();
					
					break;
				}
			case ctPathCommandEnd:
				{
					hRes = pRenderer->PathCommandEnd();

					break;
				}
			case ctDrawPath:
				{
					LONG lType = ReadLONG();

					hRes = pRenderer->DrawPath(lType);

					break;
				}
			case ctPathCommandStart:
				{
					hRes = pRenderer->PathCommandStart();

					break;
				}
			case ctDrawImage:
				{
					LONG lIndex			= ReadLONG();
					
					double fX			= ReadDouble();
					double fY			= ReadDouble();
					double fWidth		= ReadDouble();
					double fHeight		= ReadDouble();

					hRes = pRenderer->DrawImage(m_arImages[lIndex], fX, fY, fWidth, fHeight);
					
					break;
				}
			case ctDrawImageFromFile:
				{
					BSTR bsPath			= ReadBSTR();
				
					double fX			= ReadDouble();
					double fY			= ReadDouble();
					double fWidth		= ReadDouble();
					double fHeight		= ReadDouble();

					pRenderer->DrawImageFromFile(bsPath, fX, fY, fWidth, fHeight);
					break;
				}
			case ctSetParams:
				{
					double angle		= ReadDouble();
					double left			= ReadDouble();
					double top			= ReadDouble();
					double width		= ReadDouble();
					double height		= ReadDouble();
					LONG   flags		= ReadLONG();

					hRes = pRenderer->SetCommandParams(angle, left, top, width, height, flags);

					break;
				}
			case ctBeginCommand:
				{
					LONG lType			= ReadLONG();

					hRes = pRenderer->BeginCommand(lType);
					break;
				}
			case ctEndCommand:
				{
					LONG lType			= ReadLONG();

					hRes = pRenderer->EndCommand(lType);

					break;
				}
			case ctSetTransform:
				{
					double d1		= ReadDouble();
					double d2		= ReadDouble();
					double d3		= ReadDouble();
					double d4		= ReadDouble();
					double d5		= ReadDouble();
					double d6		= ReadDouble();

					pRenderer->SetTransform(d1, d2, d3, d4, d5, d6);
					break;
				}
			case ctResetTransform:
				{
					pRenderer->ResetTransform();
					break;
				}
			case ctClipMode:
				{
					LONG lMode		= ReadLONG();
					pRenderer->put_ClipMode(lMode);
					break;
				}
			default:
				{
					// мало ли что тут произошло, уж лучше выйдем
					return FALSE;
				}
			};

			if (S_OK != hRes)
				break;

			if ((NULL != pCancel) && (TRUE == *pCancel))
			{
				// отрисовщик остановлен
				break;
			}
		}
		return (S_OK == hRes);
	}
	BOOL Draw2(IASCRenderer* pRenderer, BOOL* pBreak)
	{
		if (NULL == pRenderer)
			return FALSE;

		// выставляем размеры страницы
		pRenderer->put_Width(m_fWidth);
		pRenderer->put_Height(m_fHeight);

		IASCRenderer2* pRenderer2 = NULL;
		pRenderer->QueryInterface(__uuidof(IASCRenderer2), (void**)&pRenderer2);

		// отрисовываем все объекты...
		m_pBufferMem = m_pBuffer;

		BYTE* pBufferMem = m_pBuffer;

		CommandType eOldCommand = ctError;
		CommandType eCurCommand = ctError;

		volatile BOOL* pCancel = pBreak;

		HRESULT hRes = S_OK;
		while (m_lPosition > ((size_t)(pBufferMem - m_pBuffer)))
		{
			eCurCommand = (CommandType)ReadByte(pBufferMem);
			
			switch (eCurCommand)
			{
			case ctPenXML:
				{
					pRenderer->SetPen(ReadBSTR(pBufferMem));
					break;
				}
			case ctPenColor:
				{
					pRenderer->put_PenColor(ReadLONG(pBufferMem));
					break;
				}
			case ctPenAlpha:
				{
					pRenderer->put_PenAlpha((LONG)ReadByte(pBufferMem));
					break;
				}
			case ctPenSize:
				{
					pRenderer->put_PenSize(ReadDouble(pBufferMem));
					break;
				}
			case ctPenDashStyle:
				{
					pRenderer->put_PenDashStyle(ReadByte(pBufferMem));
					break;
				}
			case ctPenLineStartCap:
				{
					pRenderer->put_PenLineStartCap(ReadByte(pBufferMem));
					break;
				}
			case ctPenLineEndCap:
				{
					pRenderer->put_PenLineEndCap(ReadByte(pBufferMem));
					break;
				}
			case ctPenLineJoin:
				{
					pRenderer->put_PenLineJoin(ReadByte(pBufferMem));
					break;
				}
			case ctPenDashPatern:
				{
					LONG lCount = ReadLONG(pBufferMem);
					ULONG lFloats = lCount * m_lSizeofDouble;

					SAFEARRAYBOUND rgsab;
					rgsab.lLbound	= 0;
					rgsab.cElements	= lCount;

					SAFEARRAY* pArray = SafeArrayCreate(VT_R8, 1, &rgsab);
					memcpy(pArray->pvData, pBufferMem, lFloats);

					pBufferMem += lFloats;

					hRes = pRenderer->PenDashPattern(pArray);

					RELEASEARRAY(pArray);
					
					break;
				}
			case ctPenDashOffset:
				{
					pRenderer->put_PenDashOffset(ReadDouble(pBufferMem));
					break;
				}
			case ctPenAlign:
				{
					pRenderer->put_PenAlign(ReadLONG(pBufferMem));
					break;
				}
			case ctPenMiterLimit:
				{
					pRenderer->put_PenMiterLimit(ReadDouble(pBufferMem));
					break;
				}
			case ctBrushXML:
				{
					pRenderer->SetBrush(ReadBSTR(pBufferMem));
					break;
				}
			case ctBrushType:
				{
					pRenderer->put_BrushType(ReadLONG(pBufferMem));
					break;
				}
			case ctBrushColor1:
				{
					pRenderer->put_BrushColor1(ReadLONG(pBufferMem));
					break;
				}
			case ctBrushColor2:
				{
					pRenderer->put_BrushColor2(ReadLONG(pBufferMem));
					break;
				}
			case ctBrushAlpha1:
				{
					pRenderer->put_BrushAlpha1((LONG)ReadByte(pBufferMem));
					break;
				}
			case ctBrushAlpha2:
				{
					pRenderer->put_BrushAlpha2((LONG)ReadByte(pBufferMem));
					break;
				}
			case ctBrushTexturePath:
				{
					pRenderer->put_BrushTexturePath(ReadBSTR(pBufferMem));
					break;
				}
			case ctBrushTextureAlpha:
				{
					pRenderer->put_BrushTextureAlpha((LONG)ReadByte());
					break;
				}
			case ctBrushTextureMode:
				{
					pRenderer->put_BrushTextureMode(ReadLONG(pBufferMem));
					break;
				}
			case ctBrushRectable:
				{
					BOOL bRect = (BOOL)ReadByte(pBufferMem);
								
					if (bRect)
					{
						pRenderer->BrushRect(bRect, (double)ReadFloat(pBufferMem), (double)ReadFloat(pBufferMem),
							(double)ReadFloat(pBufferMem), (double)ReadFloat(pBufferMem));
					}
					else
					{
						pRenderer->BrushRect(bRect, 0, 0, 0, 0);
					}

					break;
				}
			case ctBrushAngle:
				{
					pRenderer->put_BrushLinearAngle( ReadDouble(pBufferMem));
					break;
				}
			case ctBrushSubColors:
				{
					long* data = (long*)pBufferMem;
					pBufferMem += (data[0] * 2 + 1) * m_lSizeofLONG;
					
					VARIANT var;
					var.plVal = data;
					var.vt = VT_BYREF|VT_I4;
					pRenderer->SetAdditionalParam( _bstr_t(_T("subcolors")), var );
					//pRenderer->SetSubColors( data + 1, data[0] );
					break;
				}
			case ctFontXML:
				{
					hRes = pRenderer->SetFont(ReadBSTR(pBufferMem));
					break;
				}
			case ctFontName:
				{
					pRenderer->put_FontName(ReadBSTR(pBufferMem));
					break;
				}
			case ctFontPath:
				{
					pRenderer->put_FontPath(ReadBSTR(pBufferMem));
					break;
				}
			case ctFontSize:
				{
					pRenderer->put_FontSize(ReadDouble(pBufferMem));
					break;
				}
			case ctFontStyle:
				{
					pRenderer->put_FontStyle(ReadLONG(pBufferMem));
					break;
				}
			case ctFontGID:
				{
					pRenderer->put_FontStringGID((BOOL)ReadByte(pBufferMem));
					break;
				}
			case ctFontCharSpace:
				{
					pRenderer->put_FontCharSpace(ReadDouble(pBufferMem));
					break;
				}
			case ctShadowXML:
				{
					hRes = pRenderer->SetShadow(ReadBSTR(pBufferMem));
					break;
				}
			case ctShadowVisible:
				{
					pRenderer->put_ShadowVisible((BOOL)ReadByte(pBufferMem));
					break;
				}
			case ctShadowDistanceX:
				{
					pRenderer->put_ShadowDistanceX(ReadDouble(pBufferMem));
					break;
				}
			case ctShadowDistanceY:
				{
					pRenderer->put_ShadowDistanceY(ReadDouble(pBufferMem));
					break;
				}
			case ctShadowBlurSize:
				{
					pRenderer->put_ShadowBlurSize(ReadDouble(pBufferMem));
					break;
				}
			case ctShadowColor:
				{
					pRenderer->put_ShadowColor(ReadLONG(pBufferMem));
					break;
				}
			case ctShadowAlpha:
				{
					pRenderer->put_ShadowAlpha((LONG)ReadByte(pBufferMem));
					break;
				}
			case ctEdgeXML:
				{
					hRes = pRenderer->SetEdgeText(ReadBSTR(pBufferMem));	
					break;
				}
			case ctEdgeVisible:
				{
					pRenderer->put_EdgeVisible(ReadLONG(pBufferMem));
					break;
				}
			case ctEdgeDistance:
				{
					pRenderer->put_EdgeDist(ReadDouble(pBufferMem));
					break;
				}
			case ctEdgeColor:
				{
					pRenderer->put_EdgeColor(ReadLONG(pBufferMem));
					break;
				}
			case ctEdgeAlpha:
				{
					pRenderer->put_EdgeAlpha((LONG)ReadByte(pBufferMem));
					break;
				}
			case ctDrawText:
			case ctPathCommandText:
				{
					BSTR bsText			= ReadBSTR(pBufferMem);

					double fX			= ReadDouble(pBufferMem);
					double fY			= ReadDouble(pBufferMem);
					double fWidth		= ReadDouble(pBufferMem);
					double fHeight		= ReadDouble(pBufferMem);
					double fBaseOffset	= ReadDouble(pBufferMem);

					if (ctDrawText == eCurCommand)
					{
						hRes = pRenderer->CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseOffset);
					}
					else
					{
						hRes = pRenderer->PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseOffset);
					}

					break;
				}
			case ctDrawTextEx:
			case ctPathCommandTextEx:
				{
					BSTR bsText			= ReadBSTR2(pBufferMem);
					BSTR bsGid			= ReadBSTR2(pBufferMem);
					BSTR bsSource		= ReadBSTR2(pBufferMem);

					double fX			= ReadDouble(pBufferMem);
					double fY			= ReadDouble(pBufferMem);
					double fWidth		= ReadDouble(pBufferMem);
					double fHeight		= ReadDouble(pBufferMem);
					double fBaseOffset	= ReadDouble(pBufferMem);

					DWORD dwFlags = 0;
					//DWORD dwFlags		= ReadDWORD(pBufferMem);

					if (ctDrawTextEx == eCurCommand)
					{
						hRes = pRenderer->CommandDrawTextEx(bsText, bsGid, bsSource, fX, fY, fWidth, fHeight, fBaseOffset, dwFlags);
					}
					else
					{
						hRes = pRenderer->PathCommandTextEx(bsText, bsGid, bsSource, fX, fY, fWidth, fHeight, fBaseOffset, dwFlags);
					}

					break;
				}

			case ctPathCommandMoveTo:
				{
					double fX = ReadDouble(pBufferMem);
					double fY = ReadDouble(pBufferMem);

					hRes = pRenderer->PathCommandMoveTo(fX, fY);
					
					break;
				}
			case ctPathCommandLineTo:
				{
					double fX = ReadDouble(pBufferMem);
					double fY = ReadDouble(pBufferMem);

					hRes = pRenderer->PathCommandLineTo(fX, fY);
					
					break;
				}
			case ctPathCommandLinesTo:
				{
					LONG lCount = ReadLONG(pBufferMem);
					ULONG lFloats = lCount * m_lSizeofDouble;

					SAFEARRAYBOUND rgsab;
					rgsab.lLbound	= 0;
					rgsab.cElements	= lCount;

					SAFEARRAY* pArray = SafeArrayCreate(VT_R8, 1, &rgsab);
					memcpy(pArray->pvData, pBufferMem, lFloats);

					pBufferMem += lFloats;

					hRes = pRenderer->PathCommandLinesTo(pArray);

					RELEASEARRAY(pArray);
					
					break;
				}
			case ctPathCommandCurveTo:
				{
					double fX1 = ReadDouble(pBufferMem);
					double fY1 = ReadDouble(pBufferMem);
					double fX2 = ReadDouble(pBufferMem);
					double fY2 = ReadDouble(pBufferMem);
					double fX3 = ReadDouble(pBufferMem);
					double fY3 = ReadDouble(pBufferMem);

					hRes = pRenderer->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
					
					break;
				}
			case ctPathCommandCurvesTo:
				{
					LONG lCount = ReadLONG(pBufferMem);
					ULONG lFloats = lCount * m_lSizeofDouble;

					SAFEARRAYBOUND rgsab;
					rgsab.lLbound	= 0;
					rgsab.cElements	= lCount;

					SAFEARRAY* pArray = SafeArrayCreate(VT_R8, 1, &rgsab);
					memcpy(pArray->pvData, pBufferMem, lFloats);

					pBufferMem += lFloats;

					hRes = pRenderer->PathCommandCurvesTo(pArray);

					RELEASEARRAY(pArray);
					
					break;
				}
			case ctPathCommandArcTo:
				{
					double fX1 = ReadDouble(pBufferMem);
					double fY1 = ReadDouble(pBufferMem);
					double fX2 = ReadDouble(pBufferMem);
					double fY2 = ReadDouble(pBufferMem);
					double fX3 = ReadDouble(pBufferMem);
					double fY3 = ReadDouble(pBufferMem);

					hRes = pRenderer->PathCommandArcTo(fX1, fY1, fX2, fY2, fX3, fY3);
					
					break;
				}
			case ctPathCommandClose:
				{
					hRes = pRenderer->PathCommandClose();
					
					break;
				}
			case ctPathCommandEnd:
				{
					hRes = pRenderer->PathCommandEnd();

					break;
				}
			case ctDrawPath:
				{
					LONG lType = ReadLONG(pBufferMem);

					hRes = pRenderer->DrawPath(lType);

					break;
				}
			case ctPathCommandStart:
				{
					hRes = pRenderer->PathCommandStart();

					break;
				}
			case ctDrawImage:
				{
					LONG lIndex			= ReadLONG(pBufferMem);
					
					double fX			= ReadDouble(pBufferMem);
					double fY			= ReadDouble(pBufferMem);
					double fWidth		= ReadDouble(pBufferMem);
					double fHeight		= ReadDouble(pBufferMem);

					hRes = pRenderer->DrawImage(m_arImages[lIndex], fX, fY, fWidth, fHeight);
					
					break;
				}
			case ctDrawImageFromFile:
				{
					BSTR bsPath			= ReadBSTR(pBufferMem);
				
					double fX			= ReadDouble(pBufferMem);
					double fY			= ReadDouble(pBufferMem);
					double fWidth		= ReadDouble(pBufferMem);
					double fHeight		= ReadDouble(pBufferMem);

					CImageExt oImage;
					oImage.DrawOnRenderer(pRenderer, bsPath, fX, fY, fWidth, fHeight, TRUE);

					//pRenderer->DrawImageFromFile(bsPath, fX, fY, fWidth, fHeight);
					break;
				}
			case ctSetParams:
				{
					double angle		= ReadDouble(pBufferMem);
					double left			= ReadDouble(pBufferMem);
					double top			= ReadDouble(pBufferMem);
					double width		= ReadDouble(pBufferMem);
					double height		= ReadDouble(pBufferMem);
					LONG   flags		= ReadLONG(pBufferMem);

					hRes = pRenderer->SetCommandParams(angle, left, top, width, height, flags);

					break;
				}
			case ctBeginCommand:
				{
					LONG lType			= ReadLONG(pBufferMem);

					hRes = pRenderer->BeginCommand(lType);
					break;
				}
			case ctEndCommand:
				{
					LONG lType			= ReadLONG(pBufferMem);

					hRes = pRenderer->EndCommand(lType);

					break;
				}
			case ctSetTransform:
				{
					double d1		= ReadDouble(pBufferMem);
					double d2		= ReadDouble(pBufferMem);
					double d3		= ReadDouble(pBufferMem);
					double d4		= ReadDouble(pBufferMem);
					double d5		= ReadDouble(pBufferMem);
					double d6		= ReadDouble(pBufferMem);

					pRenderer->SetTransform(d1, d2, d3, d4, d5, d6);
					break;
				}
			case ctResetTransform:
				{
					pRenderer->ResetTransform();
					break;
				}
			case ctClipMode:
				{
					LONG lMode		= ReadLONG(pBufferMem);
					pRenderer->put_ClipMode(lMode);
					break;
				}
			case ctCommandLong1:
				{
					LONG lValue		= ReadLONG(pBufferMem);
					if (NULL != pRenderer2)
						pRenderer2->CommandLong(lValue);
					break;
				}
			case ctCommandDouble1:
				{
					double dValue	= ReadDouble(pBufferMem);
					if (NULL != pRenderer2)
						pRenderer2->CommandDouble(dValue);
					break;
				}
			case ctCommandString1:
				{
					BSTR bsVal		= ReadBSTR(pBufferMem);
					if (NULL != pRenderer2)
						pRenderer2->CommandString(bsVal);
					break;
				}
			case ctCommandLong2:
				{
					LONG _Type		= ReadLONG(pBufferMem);
					LONG lValue		= ReadLONG(pBufferMem);
					if (NULL != pRenderer2)
						pRenderer2->CommandLong2(_Type, lValue);
					break;
				}
			case ctCommandDouble2:
				{
					LONG _Type		= ReadLONG(pBufferMem);
					double dValue	= ReadDouble(pBufferMem);
					if (NULL != pRenderer2)
						pRenderer2->CommandDouble2(_Type, dValue);
					break;
				}
			case ctCommandString2:
				{
					LONG _Type		= ReadLONG(pBufferMem);
					BSTR bsVal		= ReadBSTR(pBufferMem);
					if (NULL != pRenderer2)
						pRenderer2->CommandString2(_Type, bsVal);
					break;
				}				
			default:
				{
					// мало ли что тут произошло, уж лучше выйдем
					return FALSE;
				}
			};

			if (S_OK != hRes)
				break;

			if ((NULL != pCancel) && (TRUE == *pCancel))
			{
				// отрисовщик остановлен
				break;
			}
		}

		//if (NULL != m_pAdditionalCommands)
		//{
		//	m_pAdditionalCommands->Draw2(pRenderer, -1, -1, pBreak);
		//}

		return (S_OK == hRes);
	}

};