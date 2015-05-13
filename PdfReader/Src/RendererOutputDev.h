#ifndef _PDF_READER_RENDERER_OUTPUTDEV_H
#define _PDF_READER_RENDERER_OUTPUTDEV_H

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/fontengine/FontManager.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"

#include "OutputDevice.h"
#include "XmlUtils.h"

namespace PdfReader
{
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
		std::wstring    wsFilePath;       // Путь к шрифту на диске
		std::wstring    wsFontName;       // Имя шрифта, которое записано в PDF(ветка для случаев, когда имя шрифта в самом шрифте не указано)
		unsigned short *pCodeToGID;       // Таблица код - номер глифа в шрифте
		unsigned short *pCodeToUnicode;   // Таблица код - юникодное значение
		unsigned int    unLenGID;         // Количество элементов в таблицах
		unsigned int    unLenUnicode;     //
		bool            bAvailable;       // Доступен ли шрифт. Сделано для многопотоковости
		
	};

	class CFontList
	{
	public:

		CFontList();
		~CFontList();
		void LoadFromFile(std::wstring wsDirPath);
		void SaveToFile(std::wstring wsDirPath);
		bool Find(Ref oRef, TFontEntry *pEntry);
		bool Find2(Ref oRef, TFontEntry **ppEntry);
		TFontEntry *Add(Ref oRef, std::wstring wsFileName, unsigned short *pCodeToGID, unsigned short *pCodeToUnicode, unsigned int nLenGID, unsigned int nLenUnicode);
		void Clear();
		bool GetFont(Ref *pRef, TFontEntry *pEntry);
	private:

		TFontEntry* Lookup(Ref& oRef)
		{
			CRefFontMap::const_iterator oPos = m_oFontMap.find(oRef);
			if (m_oFontMap.end() != oPos)
				return oPos->second;

			return NULL;
		}
		void        Add(Ref& oRef, TFontEntry* pFontEntry)
		{
			// До вызова данной функции надо проверять есть ли элемент с данным ключом
			m_oFontMap.insert(std::pair<Ref, TFontEntry*>(oRef, pFontEntry));
		}

	private:

		typedef std::map<Ref, TFontEntry*>  CRefFontMap;
		CRefFontMap                         m_oFontMap;
		NSCriticalSection::CRITICAL_SECTION m_oCS;       // Критическая секция
	};
	//-------------------------------------------------------------------------------------------------------------------------------
	template <typename T>
	inline static double PDFCoordsToMM(T tX)
	{
		return  ((double)tX / 72.0) * 25.4;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	static void FileWrite(void *pStream, char *sData, int nLen)
	{
		::fwrite(sData, 1, nLen, (FILE *)pStream);
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	// RendererOutputDev
	//-------------------------------------------------------------------------------------------------------------------------------

	class RendererOutputDev : public OutputDev
	{
	public:

		RendererOutputDev(GlobalParams *pGlobalParams, IRenderer *pRenderer, CFontManager* pFontManager, CFontList *pFontList = NULL);
		virtual ~RendererOutputDev();
		virtual bool UpSideDown()
		{
			return false;
		}
		virtual bool UseDrawChar()
		{
			return true;
		}
		virtual bool UseTilingPatternFill()
		{
			return false;
		}
		virtual bool UseFunctionalShadedFills()
		{
			return false;
		}
		virtual bool UseAxialShadedFills()
		{
			return m_bUseAxialShaded;
		}
		virtual bool UseRadialShadedFills()
		{
			return m_bUseRadialShaded;
		}
		virtual bool UseClipTo()
		{
			return false;//true;
		}
		virtual bool InterpretType3Chars()
		{
			return true;
		}
		virtual bool UseFillAndStroke()
		{
			return true;
		}
		virtual bool UseSimpleTransparentGroup()
		{
			return true;
		}
		virtual bool UseSimpleTilingPatternFill()
		{
			if (NULL == m_pRenderer)
				return false;

			// TODO: m_pRenderer->GetAdditionalParam(L"TilingHtmlPattern");

			return false;
		}
		virtual bool IsStopped()
		{
			if (NULL != m_pbBreak)
				return *m_pbBreak;
			else
				return false;
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
		virtual void StartTilingFill(GrState *pGState);
		virtual void EndTilingFill();
		virtual bool FunctionShadedFill(GrState *pGState, GrFunctionShading *pShading);
		virtual bool AxialShadedFill(GrState *pGState, GrAxialShading    *pShading);
		virtual bool RadialShadedFill(GrState *pGState, GrRadialShading   *pShading);
		virtual void StartShadedFill(GrState *pGState);
		virtual void EndShadedFill();
		virtual void StartTilingFillIteration();
		virtual void EndTilingFillIteration();
		virtual void StartSimpleTilingFill(GrState *pGState, int  nX0, int nY0, int nX1, int nY1, double dStepX, double dStepY, double dXMin, double dYMin, double dXMax, double dYMax, double* pMatrix);
		virtual void EndSimpleTilingFill();
		//----- Path clipping
		virtual void Clip(GrState *pGState);
		virtual void ClipAttack(GrState *pGState)
		{
			UpdateClipAttack(pGState);
		}
		virtual void EoClip(GrState *pGState);
		virtual void ClipToStrokePath(GrState *pGState);
		virtual void ClipToPath(GrState *pGState, GrPath *pPath, double *pMatrix, bool bEO);
		//----- Вывод текста
		virtual void BeginStringOperator(GrState *pGState);
		virtual void EndStringOperator(GrState *pGState);
		virtual void DrawString(GrState *pGState, StringExt *seString);
		virtual void DrawChar(GrState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen);
		bool BeginType3Char(GrState *pGState, double dX, double dY, double dDx, double dDy, CharCode nCode, Unicode *pUnicode, int nUnicodeLen);
		void EndType3Char(GrState *pGState);
		void Type3D0(GrState *pGState, double dWx, double dWy);
		void Type3D1(GrState *pGState, double dWx, double dWy, double dBLx, double dBLy, double dTRx, double dTRy);
		//----- Вывод картинок
		virtual void DrawImageMask(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, bool bInvert, bool bInlineImage);
		virtual void DrawImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, int *pMaskColors, bool bInlineImg);
		virtual void DrawMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, bool bMaskInvert);
		virtual void DrawSoftMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GrImageColorMap *pMaskColorMap);
		//----- Transparency groups и SMasks
		virtual void BeginTransparencyGroup(GrState *pGState, double *pBBox, GrColorSpace *pBlendingColorSpace, bool bIsolated, bool bKnockout, bool bForSoftMask);
		virtual void EndTransparencyGroup(GrState *pGState);
		virtual void PaintTransparencyGroup(GrState *pGState, double *pBBox);
		virtual void SetSoftMask(GrState *pGState, double *pBBox, bool bAlpha, Function *pTransferFunc, GrColor *pBackdropColor);
		virtual void ClearSoftMask(GrState *pGState);
		//----- Дополнительные функции для данного устройства
		void NewPDF(XRef *pXref);
		void SetBreak(bool* pbBreak)
		{
			m_pbBreak = pbBreak;
		}
	private:

		void Transform(double *pMatrix, double dUserX, double dUserY, double *pdDeviceX, double *pdDeviceY);
		void DoPath(GrState *pGState, GrPath *pPath, double dPageHeight, double *pCTM);
		void ClipToText(std::wstring& wsFontName, std::wstring& wsFontPath, double dFontSize, int nFontStyle, double* pMatrix, std::wstring& wsText, double dX, double dY, double dWidth = 0, double dHeight = 0, double dBaseLineOffset = 0);
		void UpdateClip(GrState *pGState);
		void UpdateClipAttack(GrState *pGState);
		void DoTransform(double *pMatrix, double *pdShiftX, double *pdShiftY, bool bText = false);
	private:

		IRenderer*                    m_pRenderer;
		long                          m_lRendererType;
		double                        m_arrMatrix[6];
		CFontManager*                 m_pFontManager;

		GrTextClip                   *m_pBufferTextClip;

		XRef                         *m_pXref;           // Таблица Xref для данного PDF-документа
		CFontList                    *m_pFontList;

		bool                         *m_pbBreak;         // Внешняя остановка рендерера

		bool                          m_bUseAxialShaded;
		bool                          m_bUseRadialShaded;

		GrClip                       *m_pClip;
		bool                          m_bTiling;
		bool                          m_bTransparentGroup;

		bool                          m_bTransparentGroupSoftMask;
		unsigned char*                m_pTransparentGroupSoftMask;
	};
}

#endif // _PDF_READER_RENDERER_OUTPUTDEV_H
