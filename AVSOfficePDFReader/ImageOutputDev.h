#ifndef _SIMAGE_OUTPUTDEV_H
#define _SIMAGE_OUTPUTDEV_H

#include "STypes.h"
#include "OutputDevice.h"
#include "GState.h"

class Gr8BitFont;
class SBitmap;
class SImage;
class SPath;
class SPattern;
class SFontEngine;
class SFont;
class T3FontCache;
struct T3FontCacheTag;
struct T3GlyphStack;
struct TransparencyGroup;

//-------------------------------------------------------------------------------------------------------------------------------

// максимальный размер кэша для хранения Type3-шрифтов
#define T3FontCacheSize 8

//-------------------------------------------------------------------------------------------------------------------------------
// SImageOutputDev
//-------------------------------------------------------------------------------------------------------------------------------

class SImageOutputDev: public OutputDev 
{
public:

	SImageOutputDev(GlobalParams *pGlobalaParams, SColorMode eColorMode, int nBitmapRowPad, BOOL bReverseVideo, SColorPointer pPaperColor, BOOL nBitmapTopDown = TRUE, BOOL bAllowAA = TRUE);

	virtual ~SImageOutputDev();

	virtual BOOL UpSideDown() 
	{ 
		return TRUE; 
	}

	virtual BOOL UseDrawChar() 
	{ 
		return TRUE; 
	}

	virtual BOOL InterpretType3Chars() 
	{ 
		return TRUE; 
	}

	//---------------------------------------------------------------------------------------------------------------------------

	virtual void StartPage(int nPageIndex, GrState *pGState);

	virtual void EndPage();

	//----- Save/Restore GState
	virtual void SaveGState(GrState *pGState);
	virtual void RestoreGState(GrState *pGState);

	//----- Изменение параметров в GState
	virtual void UpdateAll(GrState *pGState);
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

	//----- Изменение текстовых параметров
	virtual void UpdateFont(GrState *pGState);

	//----- Рисование Path
	virtual void Stroke(GrState *pGState);
	virtual void Fill(GrState *pGState);
	virtual void EoFill(GrState *pGState);

	//----- Path clipping
	virtual void Clip(GrState *pGState);
	virtual void EoClip(GrState *pGState);
	virtual void ClipToStrokePath(GrState *pGState);

	//----- Вывод текста
	virtual void DrawChar(GrState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen);
	virtual BOOL BeginType3Char(GrState *pGState, double dX, double dY, double dDx, double dDy, CharCode nCode, Unicode *pUnicode, int nUnicodeLen);
	virtual void EndType3Char(GrState *pGState);
	virtual void EndTextObject(GrState *pGState);

	//----- Операторы Type 3 - шрифтов
	virtual void Type3D0(GrState *pGState, double dWx, double dWy);
	virtual void Type3D1(GrState *pGState, double dWx, double dWy, double dBLx, double dBLy, double dTRx, double dTRy);

	//----- Вывод картинок
	virtual void DrawImageMask(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, BOOL bInvert, BOOL bInlineImage);
	virtual void DrawImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, int *pMaskColors, BOOL inlineImg);
	virtual void DrawMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, BOOL bMaskInvert);
	virtual void DrawSoftMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GrImageColorMap *pMaskColorMap);

	//----- Transparency groups и SMasks
	virtual void BeginTransparencyGroup(GrState *pGState, double *pBBox, GrColorSpace *pBlendingColorSpace, BOOL bIsolated, BOOL bKnockout, BOOL bForSoftMask);
	virtual void EndTransparencyGroup(GrState *pGState);
	virtual void PaintTransparencyGroup(GrState *pGState, double *pBBox);
	virtual void SetSoftMask(GrState *pGState, double *pBBox, BOOL bAlpha, Function *pTransferFunc, GrColor *pBackdropColor);
	virtual void ClearSoftMask(GrState *pGState);

	//----- Дополнительные функции для данного устройства

	// Эту функцию нужно вызвать, чтобы обозначить, что был открыт новый PDF-документ.
	void     NewPDF(XRef *pXref);
 
	void     SetPaperColor(SColorPointer pPaperColor);
	BOOL     IsReverseVideo() 
	{ 
		return m_bReverseVideo; 
	}
	void     SetReverseVideo(BOOL bReverseVideo) 
	{ 
		m_bReverseVideo = bReverseVideo; 
	}

	SBitmap *GetBitmap() 
	{ 
		return m_pBitmap; 
	}
	int      GetBitmapWidth();
	int      GetBitmapHeight();
	SBitmap *TakeBitmap();
	SImage  *GetSImage() 
	{ 
		return m_pSImage; 
	}

	void     GetModRegion(int *pnMinX, int *pnMinY, int *pnMaxX, int *pnMaxY);
	void     ClearModRegion();
	void     SetFillColor(int nR, int nG, int nB);
	SFont   *GetFont(StringExt *seName, double *pTextMatrix);
	SFont   *GetCurrentFont() 
	{ 
		return m_pFont; 
	}
	virtual BOOL GetVectorAntialias();
	virtual void SetVectorAntialias(BOOL bVectorAA);

private:

	void SetupScreenParams(double dHorDPI, double dVerDPI);
	SPattern *GetColor(GrGray oGray, GrRGB *pRGB);
	SPath *ConvertPath(GrState *pGState, GrPath *pPath);
	void DoUpdateFont(GrState *pGState);
	void DrawType3Glyph(T3FontCache *pT3Font, T3FontCacheTag *pT3Tag, unsigned char *pData);
	static BOOL ImageMaskSrc  (void *pData, SColorPointer pColorLine);
	static BOOL ImageSrc      (void *pData, SColorPointer pColorLine, unsigned char *pAlphaLine);
	static BOOL AlphaImageSrc (void *pData, SColorPointer pColorLine, unsigned char *pAlphaLine);
	static BOOL MaskedImageSrc(void *pData, SColorPointer pColorLine, unsigned char *pAlphaLine);

private:
	
	SColorMode    m_eColorMode;      // Основная цветовая модель

	int           m_nBitmapRowPad;   //
	BOOL          m_bBitmapTopDown;  // Картинка идет сверху вниз? 

	BOOL          m_bAllowAA;        //
	BOOL          m_bVectorAA;       //
	BOOL          m_bReverseVideo;   // Reverse video mode
	SColor        m_pPaperColor;     // Цвет бумаги(подложки/заднего фона)
	SScreenParams m_pScreenParams;   // Параметры экрана (размер пикселя, порог белого/черного цветов и т.д.)

	XRef         *m_pXref;           // Таблица Xref для данного PDF-документа

	SBitmap      *m_pBitmap;         // Основной Битмап, на котором мы все рисуется
	SImage       *m_pSImage;         // Основной объект для рисования
	SFontEngine  *m_pFontEngine;     // Объект для разбора фонтов

	T3FontCache  *m_pT3FontCache[T3FontCacheSize]; // Кэш со шрифтами типа Type 3
	int           m_nT3FontsCount;   // Количество доступных шрифтов из m_pT3FontCache
	T3GlyphStack *m_pT3GlyphStack;   // Стек с символами для Type3-шрифта

	SFont        *m_pFont;           // Текущий шрифт
	BOOL          m_bNeedFontUpdate; // Если True, тогда текущий шрифт нужно обновить

	SPath        *m_pTextClipPath;   // Clipping path ( построенный по тексту )

	TransparencyGroup *m_pTranspGroupStack; // Стек Transparency group

};

#endif /* _SIMAGE_OUTPUTDEV_H */
