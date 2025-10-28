/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#ifndef _PDF_WRITER_SRC_REDACT_OUTPUTDEV_H
#define _PDF_WRITER_SRC_REDACT_OUTPUTDEV_H

#include "../PdfWriter.h"
#include "../PdfEditor.h"

#include "../SrcReader/GfxClip.h"

#include "../lib/xpdf/Gfx.h"
#include "../lib/xpdf/OutputDev.h"

#include <stack>

namespace PdfWriter
{
	class RedactOutputDev : public OutputDev
	{
	public:
		RedactOutputDev(CPdfWriter* pRenderer, CObjectsManager* pObjMng);
		virtual ~RedactOutputDev();

		void SetRedact(const std::vector<double>& arrQuadPoints);
		void NewPDF(XRef* pXref);

		//----- get info about output device
		virtual GBool upsideDown() override
		{
			return gFalse;
		}
		virtual GBool useDrawChar() override
		{
			return gTrue;
		}
		virtual GBool useTilingPatternFill() override
		{
			return gTrue;
		}
		virtual GBool useDrawForm() override
		{
			return gTrue;
		}
		virtual GBool interpretType3Chars() override
		{
			return gTrue;
		}
		virtual GBool needNonText() override
		{
			return gTrue;
		}
		virtual GBool needCharCount() override
		{
			return gFalse;
		}
		//----- initialization and control
		virtual void setDefaultCTM(double *ctm) override;
		virtual void startPage(int nPageIndex, GfxState *pGState) override;
		virtual void endPage() override;
		//----- save/restore graphics state
		virtual void saveState(GfxState *pGState) override;
		virtual void restoreState(GfxState *pGState) override;
		//----- update graphics state
		virtual void updateAll(GfxState *pGState) override;
		// updateCTM -> UpdateTransform
		virtual void updateLineDash(GfxState *pGState) override;
		virtual void updateFlatness(GfxState *pGState) override;
		virtual void updateLineJoin(GfxState *pGState) override;
		virtual void updateLineCap(GfxState *pGState) override;
		virtual void updateMiterLimit(GfxState *pGState) override;
		virtual void updateLineWidth(GfxState *pGState) override;
		// updateStrokeAdjust -> setExtGState
		// updateFillColorSpace
		// updateStrokeColorSpace
		virtual void updateFillColor(GfxState *pGState) override;
		virtual void updateStrokeColor(GfxState *pGState) override;
		// updateBlendMode -> setExtGState
		// updateFillOpacity -> setExtGState
		// updateStrokeOpacity -> setExtGState
		// updateFillOverprint -> setExtGState
		// updateStrokeOverprint -> setExtGState
		// updateOverprintMode -> setExtGState
		virtual void updateRenderingIntent(GfxState *pGState) override;
		// updateTransfer -> setExtGState
		//----- update text state
		virtual void updateFont(GfxState *pGState) override;
		// updateTextMat -> drawChar
		virtual void updateCharSpace(GfxState *pGState) override;
		virtual void updateRender(GfxState *pGState) override;
		virtual void updateRise(GfxState *pGState) override;
		virtual void updateWordSpace(GfxState *pGState) override;
		virtual void updateHorizScaling(GfxState *pGState) override;
		// updateTextPos -> drawChar
		// updateTextShift -> drawChar
		// saveTextPos
		// restoreTextPos
		//----- path painting
		virtual void stroke(GfxState *pGState) override;
		virtual void fill(GfxState *pGState) override;
		virtual void eoFill(GfxState *pGState) override;
		virtual void tilingPatternFill(GfxState *pGState, Gfx *gfx, Object *pStream, int nPaintType, int nTilingType, Dict *pResourcesDict, double *pMatrix, double *pBBox, int nX0, int nY0, int nX1, int nY1, double dXStep, double dYStep) override;
		//----- path clipping
		virtual void clip(GfxState *pGState) override;
		virtual void eoClip(GfxState *pGState) override;
		virtual void clipToStrokePath(GfxState *pGState) override;
		//----- text drawing
		virtual void beginStringOp(GfxState *pGState) override;
		// endStringOp
		// beginString
		// endString
		virtual void drawChar(GfxState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen) override;
		// drawString
		// beginType3Char
		// endType3Char
		// endTextObject
		// beginActualText
		// endActualText
		//----- additional
		// beginMarkedContent
		// beginMCOShapes
		// endMarkedContent
		virtual GBool useNameOp() override;
		virtual void setExtGState(const char* name) override;
		virtual void setFillColorSpace(const char* name) override;
		virtual void setFillColor(Object* args, int numArgs) override;
		virtual void setFillColorN(Object* args, int numArgs) override;
		virtual void setStrokeColorSpace(const char* name) override;
		virtual void setStrokeColor(Object* args, int numArgs) override;
		virtual void setStrokeColorN(Object* args, int numArgs) override;
		virtual void setShading(GfxState *state, const char* name) override;
		//----- image drawing
		virtual void drawImageMask(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate) override;
		virtual void setSoftMaskFromImageMask(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate) override;
		virtual void drawImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap, int *pMaskColors, GBool bInlineImg, GBool interpolate) override;
		virtual void drawMaskedImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap,
									 Object* pMaskRef, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GBool bMaskInvert, GBool interpolate) override;
		virtual void drawSoftMaskedImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap,
										 Object *maskRef, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GfxImageColorMap *pMaskColorMap, double *pMatte, GBool interpolate) override;
		//----- Type 3 font operators
		virtual void type3D0(GfxState *pGState, double wx, double wy) override;
		virtual void type3D1(GfxState *pGState, double wx, double wy, double llx, double lly, double urx, double ury) override;
		//----- form XObjects
		virtual void drawForm(GfxState *pGState, Gfx *gfx, Ref id, const char *name = NULL) override;
		virtual void drawImage(GfxState *pGState, Ref id, const char* name = NULL) override;
		//----- transparency groups and soft masks
		virtual void beginTransparencyGroup(GfxState *pGState, double *pBBox, GfxColorSpace *pBlendingColorSpace, GBool bIsolated, GBool bKnockout, GBool bForSoftMask) override;
		virtual void endTransparencyGroup(GfxState *pGState) override;
		virtual void paintTransparencyGroup(GfxState *pGState, double *pBBox) override;
		virtual void setSoftMask(GfxState *pGState, double *pBBox, GBool bAlpha, Function *pTransferFunc, GfxColor *pBackdropColor) override;
		virtual void clearSoftMask(GfxState *pGState) override;

	private:
		struct GfxRedactState
		{
			GfxClip* m_pClip;
			std::vector< std::pair<std::string, std::string> > m_arrOp;

			GfxRedactState() : m_pClip(NULL) {}
			~GfxRedactState()
			{
				RELEASEOBJECT(m_pClip);
			}
		};

		void DoPathRedact(GfxState* pGState, GfxPath* pPath, double* pCTM, bool bStroke = false, bool bEoFill = false);
		void DrawPathRedact(Aggplus::CGraphicsPath* oPath, bool bStroke, const std::vector<CSegment>& arrForStroke = {});
		void DoPath(GfxState* pGState, GfxPath* pPath, double* pCTM);
		void DoTransform(double* pMatrix, double* pdShiftX, double* pdShiftY, bool bActual = false);
		void DrawPath(const LONG& lType);
		void UpdateTransform();
		void AddClip(GfxState* pGState, GfxRedactState* pState, int nIndex);
		void DoStateOp();

		XRef* m_pXref;
		std::vector<double> m_arrQuadPoints;
		Aggplus::CGraphicsPath m_oPathRedact;

		CPdfWriter* m_pRenderer;
		CObjectsManager* m_mObjManager;
		CDocument*  m_pDoc;
		CPage*      m_pPage;
		double      m_arrMatrix[6];

		bool m_bUpdateAll;
		std::deque<GfxRedactState> m_sStates;
	};
}

#endif // _PDF_WRITER_SRC_REDACT_OUTPUTDEV_H
