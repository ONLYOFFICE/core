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
#ifndef _PDF_READER_OUTPUT_DEVICE_H
#define _PDF_READER_OUTPUT_DEVICE_H

#include "CharTypes.h"
#include "GlobalParams.h"

namespace PdfReader
{
	class StringExt;
	class GrState;
	class GrPath;
	struct GrColor;
	class GrColorSpace;
	class GrImageColorMap;
	class GrFunctionShading;
	class GrAxialShading;
	class GrRadialShading;
	class Stream;
	class Links;
	class Link;
	class Catalog;
	class Page;
	class Function;

	//-------------------------------------------------------------------------------------------------------------------------------
	// OutputDev
	//-------------------------------------------------------------------------------------------------------------------------------

	class OutputDev
	{
	public:

		OutputDev() {}

		virtual ~OutputDev() {}

		//------ Информация о выходном устройстве

		// True, если точка с координатой (0,0) находится в левом верхнем углу.
		virtual bool UpSideDown() = 0;

		// Вывод текста с помощью функции DrawChar() или DrawString()?
		virtual bool UseDrawChar() = 0;

		// Поддерживает ли данное устройство TilingPatternFill?  Если нет, тогда
		// TilingPatternFill будет рисоваться с помощью комбинации более простых
		// графических примитивов.
		virtual bool UseTilingPatternFill()
		{
			return false;
		}

		// Поддерживает ли данное устройство SimpleTilingPatternFill?  Если нет, тогда
		// TilingPatternFill будет рисоваться с помощью комбинации более простых
		// графических примитивов.
		virtual bool UseSimpleTilingPatternFill()
		{
			return false;
		}

		// Поддерживает ли данное устройство FunctionShadedFill? Если нет, тогда данные
		// Shaded fills будут заменены комбинацией более простых графических примитивов.
		virtual bool UseFunctionalShadedFills()
		{
			return false;
		}
		// Поддерживает ли данное устройство AxialShadedFill? Если нет, тогда данные
		// Shaded fills будут заменены комбинацией более простых графических примитивов.
		virtual bool UseAxialShadedFills()
		{
			return false;
		}
		// Поддерживает ли данное устройство RadialShadedFill? Если нет, тогда данные
		// Shaded fills будут заменены комбинацией более простых графических примитивов.
		virtual bool UseRadialShadedFills()
		{
			return false;
		}

		// Поддерживается ли DrawForm()?
		virtual bool UseDrawForm()
		{
			return false;
		}

		// Используем команду ClipToPath для клипа или команды Clip, EoClip, ClipToPathStroke
		virtual bool UseClipTo()
		{
			return false;
		}


		// Использует ли данное устройство методы BeginType3Char/EndType3Char?
		// В противном случае тект в шрифте Type3 будет выводится с помощью
		// функций DrawChar/DrawString.
		virtual bool InterpretType3Chars() = 0;

		// Поддерживается ли данным устройством что-нибудь кроме текста?
		virtual bool NeedNonText()
		{
			return true;
		}

		// Поддерживается ли данным устройствой одновременные обводка и заливка Path
		virtual bool UseFillAndStroke()
		{
			return false;
		}

		// В каком виде мы используем группу прозрачных объектов
		virtual bool UseSimpleTransparentGroup()
		{
			return false;
		}

		//------ Информация о выходном устройстве

		// Устанавливаем стандартную матрицу преобразований.
		virtual void SetDefaultCTM(double *pCTM);

		// Проверяем, должна ли мы показывать данную часть страницы. Если возращаемое
		// значение False, показ страницы прерывается. Как правило, OutputDev будет 
		// использовать дополнительные средства, чтобы показать страницу, прежде чем
		// вернет значение False.
		virtual bool CheckPageSlice(Page *pPage, double dHorDPI, double dVerDPI, int nRotate, bool bUseMediaBox, bool bCrop, int nSliceX, int nSliceY, int nSliceW, int nSliceH, bool bPrinting, Catalog *pCatalog, bool(*pAbortCheckCallBack)(void *pData) = NULL, void *pAbortCheckData = NULL)
		{
			return true;
		}

		// Новая страница.
		virtual void StartPage(int nPageNum, GrState *pGState) {}
		// Конец страницы.
		virtual void EndPage() {}

		// Вывод содержимого страницы.
		virtual void Dump() {}

		// Остановка вывода
		virtual bool IsStopped() { return false; }

		//----- Системы координат (пользовательские и устройства)

		// Конвертации
		virtual void ConvertDeviceToUser(double dDevX, double dDevY, double *pdUserX, double *pdUserY);
		virtual void ConvertUserToDevice(double dUserX, double dUserY, int *pnDevX, int *pnDevY);

		double *GetDefaultCTM()
		{
			return m_arrDefaultCTM;
		}
		double *GetDefaultInvCTM()
		{
			return m_arrDefaultInvCTM;
		}

		//----- Save/Restore GState
		virtual void SaveGState(GrState *pGState) {}
		virtual void RestoreGState(GrState *pGState) {}

		//----- Изменение параметров в GState
		virtual void UpdateAll(GrState *pGState);
		virtual void UpdateCTM(GrState *pGState, double dM11, double dM12, double dM21, double dM22, double dM31, double dM32) {}
		virtual void UpdateLineDash(GrState *pGState) {}
		virtual void UpdateFlatness(GrState *pGState) {}
		virtual void UpdateLineJoin(GrState *pGState) {}
		virtual void UpdateLineCap(GrState *pGtate) {}
		virtual void UpdateMiterLimit(GrState *pGState) {}
		virtual void UpdateLineWidth(GrState *pGState) {}
		virtual void UpdateStrokeAdjust(GrState *pGState) {}
		virtual void UpdateFillColorSpace(GrState *pGState) {}
		virtual void UpdateStrokeColorSpace(GrState *pGState) {}
		virtual void UpdateFillColor(GrState *pGState) {}
		virtual void UpdateStrokeColor(GrState *pGState) {}
		virtual void UpdateBlendMode(GrState *pGState) {}
		virtual void UpdateFillOpacity(GrState *pGState) {}
		virtual void UpdateStrokeOpacity(GrState *pGState) {}
		virtual void UpdateFillOverprint(GrState *pGState) {}
		virtual void UpdateStrokeOverprint(GrState *pGState) {}
		virtual void UpdateTransfer(GrState *pGState) {}

		//----- Изменение текстовых параметров
		virtual void UpdateFont(GrState *pGState) {}
		virtual void UpdateTextMatrix(GrState *pGState) {}
		virtual void UpdateCharSpace(GrState *pGState) {}
		virtual void UpdateRender(GrState *pGState) {}
		virtual void UpdateRise(GrState *pGState) {}
		virtual void UpdateWordSpace(GrState *pGState) {}
		virtual void UpdateHorizScaling(GrState *pGState) {}
		virtual void UpdateTextPos(GrState *pGState) {}
		virtual void UpdateTextShift(GrState *pGState, double dShift) {}

		//----- Рисование Path
		virtual void Stroke(GrState *pGState) {}
		virtual void Fill(GrState *pGState) {}
		virtual void EoFill(GrState *pGState) {}
		virtual void FillStroke(GrState *pGState) {}
		virtual void EoFillStroke(GrState *pGState) {}
		virtual void TilingPatternFill(GrState *pGState, Object *pStream, int nPaintType, Dict *pResourcesDict, double *pMatrix, double *pBBox, int nX0, int nY0, int nX1, int nY1, double dXStep, double dYStep) {}
		virtual void StartTilingFill(GrState *pGState) {}
		virtual void EndTilingFill()                 {}
		virtual bool FunctionShadedFill(GrState *pGState, GrFunctionShading *pShading)
		{
			return false;
		}
		virtual bool AxialShadedFill(GrState *pGState, GrAxialShading    *pShading)
		{
			return false;
		}
		virtual bool RadialShadedFill(GrState *pGState, GrRadialShading   *pShading)
		{
			return false;
		}

		virtual void StartTilingFillIteration() {}
		virtual void EndTilingFillIteration() {}
		virtual void StartSimpleTilingFill(GrState *pGState, int  nX0, int nY0, int nX1, int nY1, double dStepX, double dStepY, double dXMin, double dYMin, double dXMax, double dYMax, double* pMatrix) {}
		virtual void EndSimpleTilingFill() {}

		virtual void StartShadedFill(GrState *pGState) {}
		virtual void EndShadedFill()                 {}

		//----- Path clipping
		virtual void Clip(GrState *pGState) {}
		virtual void ClipAttack(GrState *pGState) {}
		virtual void EoClip(GrState *pGState) {}
		virtual void ClipToStrokePath(GrState *pGState) {}
		virtual void ClipToPath(GrState *pGState, GrPath *pPath, double *pMatrix, bool bEO) {}

		//----- Вывод текста
		virtual void BeginStringOperator(GrState *pGState) {}
		virtual void EndStringOperator(GrState *pGState) {}
		virtual void BeginString(GrState *pGState, StringExt *seString) {}
		virtual void EndString(GrState *pGState) {}
		virtual void DrawChar(GrState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pnUnicode, int nUnLen) {}
		virtual void DrawString(GrState *pGState, StringExt *seString) {}
		virtual bool BeginType3Char(GrState *pGState, double dX, double dY, double dDx, double dDy, CharCode nCode, Unicode *pnUnicode, int nUnLen);
		virtual void EndType3Char(GrState *pGState) {}
		virtual void BegintTextObject(GrState *pGState){}
		virtual void EndTextObject(GrState *pGState) {}

		//----- Вывод картинок
		virtual void DrawImageMask(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, bool bInvert, bool bInlineImage);
		virtual void DrawImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, int *pnMaskColors, bool bInlineImage);
		virtual void DrawMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, bool bMaskInvert);
		virtual void DrawSoftMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GrImageColorMap *pMaskColorMap);

#if OPI_SUPPORT
		//----- OPI
		virtual void OpiBegin(GrState *pGState, Dict *pOpiDict);
		virtual void OpiEnd  (GrState *pGState, Dict *pOpiDict);
#endif

		//----- Операторы Type 3 - шрифтов
		virtual void Type3D0(GrState *pGState, double dWx, double dWy) {}
		virtual void Type3D1(GrState *pGState, double dWx, double dWy, double dLLx, double dLLy, double dURx, double dURy) {}

		//----- Form XObjects
		virtual void DrawForm(Ref oID) {}

		//----- PostScript XObjects
		virtual void PSXObject(Stream *pPSStream, Stream *pLevel1Stream) {}

		//----- Transparency groups и SMasks
		virtual void BeginTransparencyGroup(GrState *pGState, double *pBBox, GrColorSpace *pBlendingColorSpace, bool bIsolated, bool bKnockout, bool bForSoftMask) {}
		virtual void EndTransparencyGroup(GrState *pGState) {}
		virtual void PaintTransparencyGroup(GrState *pGState, double *pBBox) {}
		virtual void SetSoftMask(GrState *pGState, double *pBBox, bool bAlpha, Function *pTransferFunction, GrColor *pBackdropColor) {}
		virtual void ClearSoftMask(GrState *pGState) {}

		//----- Links
		virtual void ProcessLink(Link *pLink, Catalog *pCatalog) {}

		virtual bool GetVectorAntialias()
		{
			return false;
		}
		virtual void SetVectorAntialias(bool bVectorAntiAlias) {}

	private:

		double        m_arrDefaultCTM[6];    // Матрица преобразования по умолчанию
		double        m_arrDefaultInvCTM[6]; // Обратная матрица

	protected:

		GlobalParams *m_pGlobalParams;
	};
}

#endif // _PDF_READER_OUTPUT_DEVICE_H
