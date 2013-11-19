#ifndef _RENDERER_OUTPUTDEV_H
#define _RENDERER_OUTPUTDEV_H

#include "..\..\AVSImageStudio3\AVSGraphics\Interfaces\AVSRenderer.h"
#include "..\..\AVSImageStudio3\AVSGraphics\Objects\Structures.h"

#define _ONLY_PDF_WRITER_INTERFACE
#include "..\AVSOfficePdfWriter\PDFWriter.h"

#include "OutputDevice.h"
#include "TemporaryCS.h"

class Gr8BitFont;
class OutputDev;
class GrPath;
class GrTextClip;
class GrState;
class GrClip;
struct Ref;

//-------------------------------------------------------------------------------------------------------------------------------
struct TFontEntry
{
	Ref             oRef;             // Ссылка на объект-шрифт
	CStringW        wsFilePath;       // Путь к шрифту на диске
	CStringW        wsFontName;       // Имя шрифта, которое записано в PDF(ветка для случаев, когда имя шрифта в самом шрифте не указано)
	unsigned short *pCodeToGID;       // Таблица код - номер глифа в шрифте
	unsigned short *pCodeToUnicode;   // Таблица код - юникодное значение
	unsigned int    unLenGID;         // Количество элементов в таблицах
	unsigned int    unLenUnicode;     //
	BOOL            bAvailable;       // Доступен ли шрифт. Сделано для многопотоковости
};

class CFontList
{
public:

	CFontList();
	~CFontList();
	void LoadFromFile(CStringW wsDirPath);
	void SaveToFile(CStringW wsDirPath);
	BOOL Find(Ref oRef, TFontEntry *pEntry);
	BOOL Find2(Ref oRef, TFontEntry **ppEntry);
	TFontEntry *Add(Ref oRef, CStringW wsFileName, unsigned short *pCodeToGID, unsigned short *pCodeToUnicode, unsigned int nLenGID, unsigned int nLenUnicode);
	void Clear();
	BOOL GetFont(Ref *pRef, TFontEntry *pEntry);
private:

	CSimpleMap<Ref, TFontEntry*> m_oFontMap;
	CRITICAL_SECTION             m_oCS;       // Критическая секция
};
//-------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline static double PDFCoordsToMM(T tX)
{
	return  ( (double)tX / 72.0 ) * 25.4;
}

//-------------------------------------------------------------------------------------------------------------------------------
static void FileWrite(void *pStream, char *sData, int nLen) 
{
	::fwrite( sData, 1, nLen, (FILE *)pStream );
}
//-------------------------------------------------------------------------------------------------------------------------------
// RendererOutputDev
//-------------------------------------------------------------------------------------------------------------------------------

class RendererOutputDev: public OutputDev 
{
public:

	RendererOutputDev(GlobalParams *pGlobalParams, IAVSRenderer *pRenderer, CFontList *pFontList = NULL);
	virtual ~RendererOutputDev();
	virtual BOOL UpSideDown() 
	{ 
		return FALSE; 
	}

	virtual BOOL UseDrawChar() 
	{ 
		//return FALSE;
		return TRUE; 
	}

	virtual BOOL UseTilingPatternFill() 
	{ 
		return FALSE; 
	}
	virtual BOOL UseFunctionalShadedFills()
	{
		return FALSE;
	}

	virtual BOOL UseAxialShadedFills()
	{
		return m_bUseAxialShaded;
	}

	virtual BOOL UseRadialShadedFills()
	{
		return m_bUseRadialShaded;
	}
	virtual BOOL UseClipTo()
	{
		return FALSE;//TRUE;
	}

	virtual BOOL InterpretType3Chars() 
	{ 
		return TRUE; 
	}

	virtual BOOL UseFillAndStroke()
	{
		return TRUE;
	}
	virtual BOOL UseSimpleTransparentGroup()
	{
		return TRUE;
	}
	virtual BOOL IsStopped()
	{
		if ( NULL != m_pbBreak )
			return *m_pbBreak;
		else
			return FALSE;
	}
	//---------------------------------------------------------------------------------------------------------------------------
	virtual void StartPage(int nPageIndex, GrState *pGState);
	virtual void EndPage();
	//----- Save/Restore GState
	virtual void SaveGState(GrState *pGState);
	virtual void RestoreGState(GrState *pGState);
	//----- Изменение параметров в GState
	virtual void UpdateCTM(GrState *pGState, double dMatrix11, double dMatrix12, double dMatrix21, double dMatrix22, double dMatrix31, double dMatrix32);
	virtual void UpdateLineDash(GrState *pGState);
	virtual void UpdateFlatness(GrState *pGState);
	virtual void UpdateLineJoin(GrState *pGState);
	virtual void UpdateLineCap(GrState *pGState);
	virtual void UpdateMiterLimit(GrState *pGState);
	virtual void UpdateLineWidth(GrState *pGState);
	virtual void UpdateStrokeAdjust(GrState *pGState);
	virtual void UpdateFillColor(GrState *pGState);
	virtual void UpdateStrokeColor(GrState *pGState);
	virtual void UpdateBlendMode(GrState *pGState);
	virtual void UpdateFillOpacity(GrState *pGState);
	virtual void UpdateStrokeOpacity(GrState *pGState);
	virtual void UpdateAll(GrState *pGState);
	virtual void UpdateRender(GrState *pGState);
	//----- Изменение текстовых параметров
	virtual void UpdateFont(GrState *pGState);
	//----- Рисование Path
	virtual void Stroke(GrState *pGState);
	virtual void Fill(GrState *pGState);
	virtual void EoFill(GrState *pGState);
	virtual void FillStroke(GrState *pGState);
	virtual void EoFillStroke(GrState *pGState);
	virtual void TilingPatternFill(GrState *pGState, Object *pStream, int nPaintType, Dict *pResourcesDict, double *pMatrix, double *pBBox, int nX0, int nY0, int nX1, int nY1, double dXStep, double dYStep);
	virtual void StartTilingFill  (GrState *pGState);
	virtual void EndTilingFill    ();
	virtual BOOL FunctionShadedFill(GrState *pGState, GrFunctionShading *pShading);
	virtual BOOL AxialShadedFill   (GrState *pGState, GrAxialShading    *pShading);
	virtual BOOL RadialShadedFill  (GrState *pGState, GrRadialShading   *pShading);
	virtual void StartShadedFill   (GrState *pGState);
	virtual void EndShadedFill     ();
	//----- Path clipping
	virtual void Clip(GrState *pGState);
	virtual void ClipAttack(GrState *pGState) 
	{
		UpdateClipAttack(pGState);
	}
	virtual void EoClip(GrState *pGState);
	virtual void ClipToStrokePath(GrState *pGState);
	virtual void ClipToPath(GrState *pGState, GrPath *pPath, double *pMatrix, BOOL bEO);
	//----- Вывод текста
	virtual void BeginStringOperator(GrState *pGState);
	virtual void EndStringOperator(GrState *pGState);
	virtual void DrawString(GrState *pGState, StringExt *seString);
	virtual void DrawChar(GrState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen);
	BOOL BeginType3Char(GrState *pGState, double dX, double dY, double dDx, double dDy, CharCode nCode, Unicode *pUnicode, int nUnicodeLen);
	void EndType3Char(GrState *pGState);
	void Type3D0(GrState *pGState, double dWx, double dWy);
	void Type3D1(GrState *pGState, double dWx, double dWy, double dBLx, double dBLy, double dTRx, double dTRy);
	// begin/end commands
	virtual void BeginCommand(const ULONG& lCommand)
	{
		if (NULL != m_pRenderer)
			m_pRenderer->BeginCommand(lCommand);
	}
	virtual void EndCommand(const ULONG& lCommand) 
	{
		if (NULL != m_pRenderer)
			m_pRenderer->EndCommand(lCommand);
	}
	// detect support futures
	virtual bool IsSupportFuture(const CString& sName)
	{
		if (NULL == m_pRenderer)
			return false;

		VARIANT var;
		var.vt = VT_BOOL;
		var.boolVal = VARIANT_FALSE;

		BSTR bsVal = sName.AllocSysString();
		m_pRenderer->GetAdditionalParam(bsVal, &var);
		SysFreeString(bsVal);

		if (VARIANT_TRUE == var.boolVal)
			return true;
		return false;
	}
	virtual void SetAdditionalParam(const CString& sName, VARIANT& var)
	{
		if (NULL == m_pRenderer)
			return;

		BSTR bsVal = sName.AllocSysString();
		m_pRenderer->SetAdditionalParam(bsVal, var);
		SysFreeString(bsVal);
	}

	virtual void nativeSetTilingFill(BOOL bVal)
	{
		m_bTiling = bVal;
	}

	//----- Вывод картинок
	virtual void DrawImageMask(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, BOOL bInvert, BOOL bInlineImage);
	virtual void DrawImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, int *pMaskColors, BOOL bInlineImg);
	virtual void DrawMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, BOOL bMaskInvert);
	virtual void DrawSoftMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GrImageColorMap *pMaskColorMap);
	//----- Transparency groups и SMasks
	virtual void BeginTransparencyGroup(GrState *pGState, double *pBBox, GrColorSpace *pBlendingColorSpace, BOOL bIsolated, BOOL bKnockout, BOOL bForSoftMask);
	virtual void EndTransparencyGroup(GrState *pGState);
	virtual void PaintTransparencyGroup(GrState *pGState, double *pBBox);
	virtual void SetSoftMask(GrState *pGState, double *pBBox, BOOL bAlpha, Function *pTransferFunc, GrColor *pBackdropColor);
	virtual void ClearSoftMask(GrState *pGState);
	//----- Дополнительные функции для данного устройства
	void NewPDF(XRef *pXref);
	void SetAVSRenderer(IAVSRenderer *pRenderer)
	{
		m_bUseAxialShaded  = FALSE;
		m_bUseRadialShaded = FALSE;

		m_pRenderer = pRenderer;

		if ( NULL != m_pRenderer )
		{
			m_pRenderer->put_PenColor( 0 );
			m_pRenderer->put_PenAlpha( 255 );
			m_pRenderer->put_PenSize( 1 );

			m_pRenderer->put_ShadowVisible( 0 );
			m_pRenderer->put_ShadowColor( 0 );

			m_pRenderer->put_FontName( _T("Arial") );
			m_pRenderer->put_FontStyle( 0 );
			m_pRenderer->put_FontSize( 10.0 );

			IPDFWriter *pInterface = NULL;
			
			m_pRenderer->QueryInterface( __uuidof( IPDFWriter ), (void**)&pInterface );
			if ( NULL != pInterface )
				m_nRendererType = 1;
			RELEASEINTERFACE( pInterface );

			LONG lType = c_nUnknownRenderer;
			m_pRenderer->get_Type( &lType );
			if ( c_nSWFRenderer == lType )
			{
				m_bUseAxialShaded  = TRUE;
				m_bUseRadialShaded = TRUE;
			}
		}
	}
	void SetBreak(BOOL *pbBreak)
	{
		m_pbBreak = pbBreak;
	}
private:

	void Transform(double *pMatrix, double dUserX, double dUserY, double *pdDeviceX, double *pdDeviceY);
	void DoPath(GrState *pGState, GrPath *pPath, double dPageHeight, double *pCTM);
	void ClipToText(BSTR bsFontName, BSTR bsFontPath, double dFontSize, LONG lFontStyle, double *pMatrix, CString sText, double dX, double dY, double dWidth = 0, double dHeight = 0, double dBaseLineOffset = 0);
	void UpdateClip(GrState *pGState);
	void UpdateClipAttack(GrState *pGState);
	void DoTransform(double *pMatrix, double *pdShiftX, double *pdShiftY, BOOL bText = FALSE);
private:
	
	IAVSRenderer                 *m_pRenderer;
	int                           m_nRendererType;

	double                        m_arrMatrix[6];

	NSStructures::CFont           m_oFont;

	GrTextClip                   *m_pBufferTextClip;

	XRef                         *m_pXref;           // Таблица Xref для данного PDF-документа
	CFontList                    *m_pFontList;
	AVSGraphics::IAVSFontManager *m_pFontManager;

	BOOL                         *m_pbBreak;         // Внешняя остановка рендерера

	BOOL                          m_bUseAxialShaded;
	BOOL                          m_bUseRadialShaded;

	GrClip                       *m_pClip;
	BOOL                          m_bTiling;
	BOOL                          m_bTransparentGroup;

	BOOL                          m_bTransparentGroupSoftMask;
	BYTE*                         m_pTransparentGroupSoftMask;
};

#endif /* _RENDERER_OUTPUTDEV_H */
