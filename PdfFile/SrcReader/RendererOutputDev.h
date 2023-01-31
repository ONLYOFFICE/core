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
//#include "../PdfReader.h"
#include "../lib/xpdf/Gfx.h"

#include "../lib/xpdf/OutputDev.h"
#include "../lib/xpdf/Object.h"
#include "../lib/xpdf/GlobalParams.h"
#include "XmlUtils.h"
#include "Adaptors.h"
#include "MemoryUtils.h"
#include "GfxClip.h"
#include <stack>
#ifdef BUILDING_WASM_MODULE
#include "../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"
#endif

namespace PdfReader
{


	//-------------------------------------------------------------------------------------------------------------------------------
	struct TFontEntry
	{
		Ref             oRef;             // Ссылка на объект-шрифт
		std::wstring    wsFilePath;       // Путь к шрифту на диске
		std::wstring    wsFontName;       // Имя шрифта, которое записано в PDF(ветка для случаев, когда имя шрифта в самом шрифте не указано)
		int *pCodeToGID;       // Таблица код - номер глифа в шрифте
		int *pCodeToUnicode;   // Таблица код - юникодное значение
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
		void Remove(Ref oRef);
		TFontEntry *Add(Ref oRef, std::wstring wsFileName, int *pCodeToGID, int *pCodeToUnicode, unsigned int nLenGID, unsigned int nLenUnicode);
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
        RendererOutputDev(IRenderer *pRenderer, NSFonts::IFontManager* pFontManager, CFontList *pFontList = NULL);
		virtual ~RendererOutputDev();

		virtual GBool upsideDown()
		{
			return false;
		}
		virtual GBool useDrawChar()
		{
			return true;
		}
		virtual GBool useTilingPatternFill()
        {
			if (m_bDrawOnlyText)
				return true;

			return false;
		}
		virtual GBool useFunctionalShadedFills()
		{
			return true;
		}
		virtual GBool useAxialShadedFills()
		{
			return true;
		}
		virtual GBool useRadialShadedFills()
		{
			return true;
		}
		virtual GBool useGouraundTriangleFills()
		{
			return true;
		}
		virtual  GBool usePatchMeshFills()
		{
			return true;
		}
		virtual GBool useClipTo()
		{
			return false;//true;
		}
		virtual GBool interpretType3Chars()
		{
			return true;
		}
		virtual GBool useFillAndStroke()
		{
			return true;
		}
		virtual GBool  useSimpleTransparentGroup()
		{
			return true;
		}
		virtual GBool useSimpleTilingPatternFill()
		{
			if (NULL == m_pRenderer)
				return false;

			// TODO: m_pRenderer->GetAdditionalParam(L"TilingHtmlPattern");

			return false;
		}
		virtual GBool isStopped()
		{
			if (NULL != m_pbBreak)
				return *m_pbBreak;
			else
				return false;
		}
		//---------------------------------------------------------------------------------------------------------------------------
		virtual void startPage(int nPageIndex, GfxState *pGState);
		virtual void endPage();
		//----- Save/Restore GState
		virtual void saveState(GfxState *pGState);
		virtual void restoreState(GfxState *pGState);
		//----- Изменение параметров в GState
		virtual void updateCTM(GfxState *pGState, double dMatrix11, double dMatrix12, double dMatrix21, double dMatrix22, double dMatrix31, double dMatrix32);
		virtual void updateLineDash(GfxState *pGState);
		virtual void updateFlatness(GfxState *pGState);
		virtual void updateLineJoin(GfxState *pGState);
		virtual void updateLineCap(GfxState *pGState);
		virtual void updateMiterLimit(GfxState *pGState);
		virtual void updateLineWidth(GfxState *pGState);
		virtual void updateStrokeAdjust(GfxState *pGState);
		virtual void updateFillColor(GfxState *pGState);
		virtual void updateStrokeColor(GfxState *pGState);
		virtual void updateBlendMode(GfxState *pGState);
		virtual void updateFillOpacity(GfxState *pGState);
		virtual void updateStrokeOpacity(GfxState *pGState);
		virtual void updateAll(GfxState *pGState);
		virtual void updateRender(GfxState *pGState);
		//----- Изменение текстовых параметров
		virtual void updateFont(GfxState *pGState);
		//----- Рисование Path
		virtual void stroke(GfxState *pGState);
		virtual void fill(GfxState *pGState);
		virtual void eoFill(GfxState *pGState);
		virtual void FillStroke(GfxState *pGState);
		virtual void EoFillStroke(GfxState *pGState);
		virtual void tilingPatternFill(GfxState *pGState, Object *pStream, int nPaintType, Dict *pResourcesDict, double *pMatrix, double *pBBox, int nX0, int nY0, int nX1, int nY1, double dXStep, double dYStep);
		virtual void StartTilingFill(GfxState *pGState);
		virtual void EndTilingFill();
		//todo overide
		virtual GBool shadedFill(GfxState *state, GfxShading *shading);
		virtual bool FunctionShadedFill(GfxState *pGState, GfxFunctionShading *pShading);
		virtual bool AxialShadedFill(GfxState *pGState, GfxAxialShading    *pShading);
		virtual bool RadialShadedFill(GfxState *pGState, GfxRadialShading   *pShading);
		virtual bool GouraundTriangleFill(GfxState *pGState, const std::vector<GfxColor*> &colors, const std::vector<NSStructures::Point> &points);
		virtual bool PatchMeshFill(GfxState *pGState, GfxPatch* pPatch, GfxPatchMeshShading *pShading);
		virtual void StartShadedFill(GfxState *pGState);
		virtual void EndShadedFill();
		virtual void StartTilingFillIteration();
		virtual void EndTilingFillIteration();
		virtual void StartSimpleTilingFill(GfxState *pGState, int  nX0, int nY0, int nX1, int nY1, double dStepX, double dStepY, double dXMin, double dYMin, double dXMax, double dYMax, double* pMatrix);
		virtual void EndSimpleTilingFill();
		//----- Path clipping
		virtual void clip(GfxState *pGState);
		virtual void clipAttack(GfxState *pGState)
		{
			updateClipAttack(pGState);
		}
		virtual void eoClip(GfxState *pGState);
		virtual void clipToStrokePath(GfxState *pGState);
		virtual void clipToPath(GfxState *pGState, GfxPath *pPath, double *pMatrix, bool bEO);
		//----- Вывод текста
        virtual void endTextObject(GfxState *pGState);
		virtual void beginStringOp(GfxState *pGState);
		virtual void endStringOp(GfxState *pGState);
		virtual void drawString(GfxState *pGState, GString *seString);
		virtual void drawChar(GfxState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen);
        GBool beginType3Char(GfxState *state, double x, double y,
                             double dx, double dy,
                             CharCode code, Unicode *u, int uLen) {
            return false;
        }
		void endType3Char(GfxState *pGState);
		void Type3D0(GfxState *pGState, double dWx, double dWy);
		void Type3D1(GfxState *pGState, double dWx, double dWy, double dBLx, double dBLy, double dTRx, double dTRy);
		//----- Вывод картинок
		virtual void drawImageMask(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate) override;
		virtual void setSoftMaskFromImageMask(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate) override;
		virtual void drawImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap, int *pMaskColors, GBool bInlineImg, GBool interpolate) override;
		virtual void drawMaskedImage(GfxState *pGState,
									 Object *pRef,
									 Stream *pStream,
									 int nWidth, int nHeight,
									 GfxImageColorMap *pColorMap,
									 Object* pMaskRef,
									 Stream *pMaskStream,
									 int nMaskWidth, int nMaskHeight,
									 GBool bMaskInvert,
									 GBool interpolate) override;
		virtual void drawSoftMaskedImage(GfxState *pGState, Object *pRef, Stream *pStream,
										 int nWidth, int nHeight,
										 GfxImageColorMap *pColorMap,
										 Object *maskRef, Stream *pMaskStream,
										 int nMaskWidth, int nMaskHeight,
										 GfxImageColorMap *pMaskColorMap,
										 double *pMatte, GBool interpolate) override;
		//----- Transparency groups и SMasks
		virtual void beginTransparencyGroup(GfxState *pGState, double *pBBox, GfxColorSpace *pBlendingColorSpace, GBool bIsolated, GBool bKnockout, GBool bForSoftMask);
		virtual void endTransparencyGroup(GfxState *pGState);
		virtual void paintTransparencyGroup(GfxState *pGState, double *pBBox);
		virtual void setSoftMask(GfxState *pGState, double *pBBox, GBool bAlpha, Function *pTransferFunc, GfxColor *pBackdropColor);
		virtual void clearSoftMask(GfxState *pGState);
		//----- Дополнительные функции для данного устройства
		void NewPDF(XRef *pXref);
		void SetBreak(bool* pbBreak)
		{
			m_pbBreak = pbBreak;
		}

	private:

		void Transform(double *pMatrix, double dUserX, double dUserY, double *pdDeviceX, double *pdDeviceY);
		void DoPath(GfxState *pGState, GfxPath *pPath, double dPageHeight, double *pCTM, GfxClipMatrix* pCTM2 = NULL);
		void ClipToText(const std::wstring& wsFontName, const std::wstring& wsFontPath, double dFontSize, int nFontStyle, double* pMatrix, const std::wstring& wsText, double dX, double dY, double dWidth = 0, double dHeight = 0, double dBaseLineOffset = 0);
		void updateClip(GfxState *pGState);
		void updateClipAttack(GfxState *pGState);
		void DoTransform(double *pMatrix, double *pdShiftX, double *pdShiftY, bool bText = false);
	private:

		IRenderer*                    m_pRenderer;
		long                          m_lRendererType;
		double                        m_arrMatrix[6];
        NSFonts::IFontManager*        m_pFontManager;

		//GfxTextClip                   *m_pBufferTextClip;

		XRef                         *m_pXref;           // Таблица Xref для данного PDF-документа
		CFontList                    *m_pFontList;

		bool                         *m_pbBreak;         // Внешняя остановка рендерера

		std::deque<GfxClip>           m_sClip;
		bool                          m_bClipChanged;

		bool                          m_bTiling;
		bool                          m_bTransparentGroup;
		bool                          m_bIsolatedTransparentGroup;
		bool                          m_bTransparentGroupSoftMask;
		bool                          m_bTransparentGroupSoftMaskEnd;
		std::vector<bool>             m_arrTransparentGroupSoftMask;

        /*
		unsigned char*                m_pSoftMask;
		int                           m_nSoftMaskWidth;
		int                           m_nSoftMaskHeight;
        */

        bool                          m_bDrawOnlyText; // Special option for html-renderer

	};
}

#endif // _PDF_READER_RENDERER_OUTPUTDEV_H
