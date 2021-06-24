/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _PDF_READER_RENDERER_OUTPUTDEV_H
#define _PDF_READER_RENDERER_OUTPUTDEV_H

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"
#include "../../DesktopEditor/graphics/structures.h"
#include "../PdfReader.h"
#include "../lib/xpdf/Gfx.h"

#include "../lib/xpdf/OutputDev.h"
#include "../lib/xpdf/Object.h"
#include "../lib/xpdf/GlobalParams.h"
#include "XmlUtils.h"
#include "Adaptors.h"

namespace PdfReader
{


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
        RendererOutputDev(GlobalParams *pGlobalParams, IRenderer *pRenderer, NSFonts::IFontManager* pFontManager, CFontList *pFontList = NULL);
		virtual ~RendererOutputDev();

        virtual GBool interpretType3Chars()
        {
        }
		virtual GBool upsideDown()
		{
			return false;
		}
		virtual GBool useDrawChar()
		{
			return true;
		}
		virtual bool UseTilingPatternFill()
        {
			if (m_bDrawOnlyText)
				return true;

			return false;
		}
		virtual bool UseFunctionalShadedFills()
		{
			return true;
		}
		virtual bool UseAxialShadedFills()
		{
			return true;
		}
		virtual bool UseRadialShadedFills()
		{
			return true;
		}
		virtual bool UseGouraundTriangleFills()
		{
			return true;
		}
		virtual bool UsePatchMeshFills()
		{
			return true;
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
		virtual void StartPage(int nPageIndex, GfxState *pGState);
		virtual void EndPage();
		//----- Save/Restore GState
		virtual void SaveGState(GfxState *pGState);
		virtual void RestoreGState(GfxState *pGState);
		//----- Изменение параметров в GState
		virtual void UpdateCTM(GfxState *pGState, double dMatrix11, double dMatrix12, double dMatrix21, double dMatrix22, double dMatrix31, double dMatrix32);
		virtual void UpdateLineDash(GfxState *pGState);
		virtual void UpdateFlatness(GfxState *pGState);
		virtual void UpdateLineJoin(GfxState *pGState);
		virtual void UpdateLineCap(GfxState *pGState);
		virtual void UpdateMiterLimit(GfxState *pGState);
		virtual void UpdateLineWidth(GfxState *pGState);
		virtual void UpdateStrokeAdjust(GfxState *pGState);
		virtual void UpdateFillColor(GfxState *pGState);
		virtual void UpdateStrokeColor(GfxState *pGState);
		virtual void UpdateBlendMode(GfxState *pGState);
		virtual void UpdateFillOpacity(GfxState *pGState);
		virtual void UpdateStrokeOpacity(GfxState *pGState);
		virtual void UpdateAll(GfxState *pGState);
		virtual void UpdateRender(GfxState *pGState);
		//----- Изменение текстовых параметров
		virtual void UpdateFont(GfxState *pGState);
		//----- Рисование Path
		virtual void Stroke(GfxState *pGState);
		virtual void Fill(GfxState *pGState);
		virtual void EoFill(GfxState *pGState);
		virtual void FillStroke(GfxState *pGState);
		virtual void EoFillStroke(GfxState *pGState);
		virtual void TilingPatternFill(GfxState *pGState, Object *pStream, int nPaintType, Dict *pResourcesDict, double *pMatrix, double *pBBox, int nX0, int nY0, int nX1, int nY1, double dXStep, double dYStep);
		virtual void StartTilingFill(GfxState *pGState);
		virtual void EndTilingFill();
		//todo overide
		virtual bool FunctionShadedFill(GfxState *pGState, GfxFunctionShading *pShading);
		virtual bool AxialShadedFill(GfxState *pGState, GfxAxialShading    *pShading);
		virtual bool RadialShadedFill(GfxState *pGState, GfxRadialShading   *pShading);
		virtual bool GouraundTriangleFill(GfxState *pGState, const std::vector<GfxColor*> &colors, const std::vector<NSStructures::Point> &points);
		virtual bool PatchMeshFill(GfxState *pGState, GfxPatch* pPatch);
		virtual void StartShadedFill(GfxState *pGState);
		virtual void EndShadedFill();
		virtual void StartTilingFillIteration();
		virtual void EndTilingFillIteration();
		virtual void StartSimpleTilingFill(GfxState *pGState, int  nX0, int nY0, int nX1, int nY1, double dStepX, double dStepY, double dXMin, double dYMin, double dXMax, double dYMax, double* pMatrix);
		virtual void EndSimpleTilingFill();
		//----- Path clipping
		virtual void Clip(GfxState *pGState);
		virtual void ClipAttack(GfxState *pGState)
		{
			UpdateClipAttack(pGState);
		}
		virtual void EoClip(GfxState *pGState);
		virtual void ClipToStrokePath(GfxState *pGState);
		virtual void ClipToPath(GfxState *pGState, GfxPath *pPath, double *pMatrix, bool bEO);
		//----- Вывод текста
        virtual void EndTextObject(GfxState *pGState);
		virtual void BeginStringOperator(GfxState *pGState);
		virtual void EndStringOperator(GfxState *pGState);
		virtual void DrawString(GfxState *pGState, GString *seString);
		virtual void DrawChar(GfxState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen);
		bool BeginType3Char(GfxState *pGState, double dX, double dY, double dDx, double dDy, CharCode nCode, Unicode *pUnicode, int nUnicodeLen);
		void EndType3Char(GfxState *pGState);
		void Type3D0(GfxState *pGState, double dWx, double dWy);
		void Type3D1(GfxState *pGState, double dWx, double dWy, double dBLx, double dBLy, double dTRx, double dTRy);
		//----- Вывод картинок
		virtual void DrawImageMask(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, bool bInvert, bool bInlineImage);
		virtual void DrawImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap, int *pMaskColors, bool bInlineImg);
		virtual void DrawMaskedImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, bool bMaskInvert);
		virtual void DrawSoftMaskedImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GfxImageColorMap *pMaskColorMap, unsigned char *pMatte);
		//----- Transparency groups и SMasks
		virtual void BeginTransparencyGroup(GfxState *pGState, double *pBBox, GfxColorSpace *pBlendingColorSpace, bool bIsolated, bool bKnockout, bool bForSoftMask);
		virtual void EndTransparencyGroup(GfxState *pGState);
		virtual void PaintTransparencyGroup(GfxState *pGState, double *pBBox);
		virtual void SetSoftMask(GfxState *pGState, double *pBBox, bool bAlpha, Function *pTransferFunc, GfxColor *pBackdropColor);
		virtual void ClearSoftMask(GfxState *pGState);
		//----- Дополнительные функции для данного устройства
		void NewPDF(XRef *pXref);
		void SetBreak(bool* pbBreak)
		{
			m_pbBreak = pbBreak;
		}


	private:

		void Transform(double *pMatrix, double dUserX, double dUserY, double *pdDeviceX, double *pdDeviceY);
		void DoPath(GfxState *pGState, GfxPath *pPath, double dPageHeight, double *pCTM);
		void ClipToText(const std::wstring& wsFontName, const std::wstring& wsFontPath, double dFontSize, int nFontStyle, double* pMatrix, const std::wstring& wsText, double dX, double dY, double dWidth = 0, double dHeight = 0, double dBaseLineOffset = 0);
		void UpdateClip(GfxState *pGState);
		void UpdateClipAttack(GfxState *pGState);
		void DoTransform(double *pMatrix, double *pdShiftX, double *pdShiftY, bool bText = false);
	private:

		IRenderer*                    m_pRenderer;
		long                          m_lRendererType;
		double                        m_arrMatrix[6];
        NSFonts::IFontManager*        m_pFontManager;

		GfxTextClip                   *m_pBufferTextClip;

		XRef                         *m_pXref;           // Таблица Xref для данного PDF-документа
		CFontList                    *m_pFontList;

		bool                         *m_pbBreak;         // Внешняя остановка рендерера

		//GrClip                       *m_pClip;
		bool                          m_bTiling;
		bool                          m_bTransparentGroup;

		bool                          m_bTransparentGroupSoftMask;
		unsigned char*                m_pTransparentGroupSoftMask;

        bool                          m_bDrawOnlyText; // Special option for html-renderer
	};
}

#endif // _PDF_READER_RENDERER_OUTPUTDEV_H
