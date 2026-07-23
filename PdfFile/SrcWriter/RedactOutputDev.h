/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#ifndef _PDF_WRITER_SRC_REDACT_OUTPUTDEV_H
#define _PDF_WRITER_SRC_REDACT_OUTPUTDEV_H

#include "../PdfWriter.h"
#include "../PdfEditor.h"

#include "../SrcReader/GfxClip.h"

#include "../lib/xpdf/Gfx.h"
#include "../lib/xpdf/OutputDev.h"

#include <stack>
#include <set>

namespace PdfWriter
{
	class RedactOutputDev : public OutputDev
	{
	public:
		RedactOutputDev(CPdfWriter* pRenderer, CObjectsManager* pObjMng, int nStartRefID, int nRedactInfo);
		virtual ~RedactOutputDev();

		void SetRedact(const std::vector<double>& arrQuadPoints);
		void SetOverlapInfo(const std::set<int>& arrHiddenTextIndices,
							const std::vector<int>* pFormTextCounts,
							const std::vector<int>* pFormGraphicsCounts,
							int nStartTextCounter = 0,
							int nStartFormCounter = 0,
							int nStartGraphicsCounter = 0);
		int  GetTextCounter()    const { return m_nTextCounter; }
		int  GetFormCounter()    const { return m_nFormCounter; }
		int GetGraphicsCounter() const { return m_nGraphicsCounter; }
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
		// updateStrokeAdjust    -> setExtGState
		// updateFillColorSpace
		// updateStrokeColorSpace
		virtual void updateFillColor(GfxState *pGState) override;
		virtual void updateStrokeColor(GfxState *pGState) override;
		// updateBlendMode       -> setExtGState
		// updateFillOpacity     -> setExtGState
		// updateStrokeOpacity   -> setExtGState
		// updateFillOverprint   -> setExtGState
		// updateStrokeOverprint -> setExtGState
		// updateOverprintMode   -> setExtGState
		virtual void updateRenderingIntent(GfxState *pGState) override;
		// updateTransfer        -> setExtGState
		//----- update text state
		virtual void updateFont(GfxState *pGState) override;
		// updateTextMat -> drawChar
		virtual void updateCharSpace(GfxState *pGState) override;
		virtual void updateRender(GfxState *pGState) override;
		virtual void updateRise(GfxState *pGState) override;
		virtual void updateWordSpace(GfxState *pGState) override;
		virtual void updateHorizScaling(GfxState *pGState) override;
		// updateTextPos   -> drawChar
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
		virtual GBool beginMarkedContent(GfxState *state, Object args[], int numArgs) override;
		// beginMCOShapes
		virtual void endMarkedContent(GfxState *state) override;
		virtual GBool useNameOp() override;
		virtual void setExtGState(GfxState *pGState, Object* pExtGState, const char* name) override;
		virtual void setFillColorSpace(const char* name) override;
		virtual void setFillColor(Object* args, int numArgs) override;
		virtual void setFillColorN(Object* args, int numArgs) override;
		virtual void setStrokeColorSpace(const char* name) override;
		virtual void setStrokeColor(Object* args, int numArgs) override;
		virtual void setStrokeColorN(Object* args, int numArgs) override;
		virtual void setShading(GfxState *state, const char* name) override;
		//----- image drawing
		virtual void drawImageMask(GfxState *pGState, Gfx *gfx, Object *pRef, Stream *pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate) override;
		// setSoftMaskFromImageMask -> drawImageMask
		virtual void drawImage(GfxState *pGState, Gfx *gfx, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap, int *pMaskColors, GBool bInlineImg, GBool interpolate) override;
		virtual void drawMaskedImage(GfxState *pGState, Gfx *gfx, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap,
									 Object* pMaskRef, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GBool bMaskInvert, GBool interpolate) override;
		virtual void drawSoftMaskedImage(GfxState *pGState, Gfx *gfx, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap,
										 Object *maskRef, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GfxImageColorMap *pMaskColorMap, double *pMatte, GBool interpolate) override;
		//----- Type 3 font operators
		virtual void type3D0(GfxState *pGState, double wx, double wy) override;
		virtual void type3D1(GfxState *pGState, double wx, double wy, double llx, double lly, double urx, double ury) override;
		//----- form XObjects
		virtual void drawForm(GfxState *pGState, Gfx *gfx, Ref id, const char *name = NULL) override;
		virtual void drawImage(GfxState *pGState, Gfx *gfx, Ref id, const char* name = NULL) override;
		//----- transparency groups and soft masks
		// beginTransparencyGroup -> drawForm
		// endTransparencyGroup   -> drawForm
		// paintTransparencyGroup -> drawForm
		// setSoftMask            -> drawForm
		// clearSoftMask          -> drawForm

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
		void DrawPathRedact(Aggplus::CGraphicsPath* oPath, bool bStroke, double& dXEnd, double& dYEnd, const std::vector<CSegment>& arrForStroke = {});
		void DoPath(GfxState* pGState, GfxPath* pPath, double* pCTM);
		void DoTransform(double* pMatrix, double* pdShiftX, double* pdShiftY, bool bActual = false);
		void DrawPath(const LONG& lType);
		void UpdateTransform();
		void AddClip(GfxState* pGState, GfxRedactState* pState, int nIndex);
		void DoStateOp();
		void DrawXObject(const char* name, const double* pCropMatrix = NULL);
		CObjectBase* CreateImage(Gfx *gfx, int nWidth, int nHeight, unsigned int nFilter, int nBPC, const char* sCS);
		CResourcesDict* GetResources(Gfx *gfx, CDictObject* pNewForm = NULL);

		XRef* m_pXref;
		std::vector<double> m_arrQuadPoints;
		Aggplus::CGraphicsPath m_oPathRedact;
		CResourcesDict* m_pResources;

		CPdfWriter* m_pRenderer;
		CObjectsManager* m_mObjManager;
		int m_nStartRefID;
		int m_nRedactInfo;
		CDocument*  m_pDoc;
		CPage*      m_pPage;
		double      m_arrMatrix[6];
		std::string m_sImageName;

		std::set<int> m_arrHiddenTextIndices;
		const std::vector<int>* m_pFormTextCounts;
		const std::vector<int>* m_pFormGraphicsCounts;
		int m_nTextCounter;
		int m_nFormCounter;
		int m_nGraphicsCounter;

		bool m_bUpdateAll;
		std::deque<GfxRedactState> m_sStates;
	};
}

#endif // _PDF_WRITER_SRC_REDACT_OUTPUTDEV_H
