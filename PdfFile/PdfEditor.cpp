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
#include "PdfEditor.h"

#include "../DesktopEditor/common/Path.h"
#include "../DesktopEditor/graphics/commands/AnnotField.h"

#include "SrcReader/Adaptors.h"
#include "SrcReader/PdfAnnot.h"
#include "SrcReader/PdfFont.h"
#include "SrcReader/RendererOutputDev.h"
#include "lib/xpdf/PDFDoc.h"
#include "lib/xpdf/AcroForm.h"
#include "lib/xpdf/TextString.h"
#include "lib/xpdf/Lexer.h"
#include "lib/xpdf/Parser.h"
#include "lib/xpdf/Outline.h"
#include "lib/xpdf/Link.h"
#include "lib/xpdf/Stream.h"
#include "lib/xpdf/GfxFont.h"

#include "SrcWriter/Catalog.h"
#include "SrcWriter/EncryptDictionary.h"
#include "SrcWriter/Info.h"
#include "SrcWriter/ResourcesDictionary.h"
#include "SrcWriter/Streams.h"
#include "SrcWriter/Destination.h"
#include "SrcWriter/Outline.h"
#include "SrcWriter/GState.h"
#include "SrcWriter/RedactOutputDev.h"
#include "SrcWriter/Image.h"
#include "SrcWriter/Font14.h"

#include <vector>

#define _UC_AddToParent(val)\
{\
	if (pParent->GetType() == PdfWriter::object_type_DICT)\
		dynamic_cast<PdfWriter::CDictObject*>(pParent)->Add(sKey, (val));\
	else if (pParent->GetType() == PdfWriter::object_type_ARRAY)\
		dynamic_cast<PdfWriter::CArrayObject*>(pParent)->Add((val));\
}

namespace
{
bool SplitSkipDict(Object* obj, CObjectsManager* pManager, int nStartRefID)
{
	CObjectGuard oTemp, oTemp2;
	if (obj->dictLookup("Type", oTemp.get())->isName("Page"))
		return true;
	else if (oTemp->isName("Annot") && obj->dictLookupNF("P", oTemp2.get())->isRef())
	{
		// Skip annotations that don't have page created
		PdfWriter::CObjectBase* pObj = pManager->GetObj(oTemp2->getRefNum() + nStartRefID);
		if (!pObj)
			return true;
	}
	oTemp->free(); oTemp2->free();

	if (obj->dictLookup("S", oTemp.get())->isName("GoTo"))
	{
		oTemp->free();
		if (obj->dictLookup("D", oTemp.get())->isArray() && oTemp->arrayGetLength() > 1 && oTemp->arrayGetNF(0, oTemp2.get())->isRef())
		{
			PdfWriter::CObjectBase* pObj = pManager->GetObj(oTemp2->getRefNum() + nStartRefID);
			if (!pObj)
				return true;
		}
	}

	return false;
}
PdfWriter::CAnnotation* CreateAnnot(Object* oAnnot, Object* oType, PdfWriter::CXref* pXref)
{
	PdfWriter::CAnnotation* pAnnot = NULL;
	if (oType->isName("Text"))
		pAnnot = new PdfWriter::CTextAnnotation(pXref);
	else if (oType->isName("Ink"))
		pAnnot = new PdfWriter::CInkAnnotation(pXref);
	else if (oType->isName("Line"))
		pAnnot = new PdfWriter::CLineAnnotation(pXref);
	else if (oType->isName("Highlight") || oType->isName("Underline") || oType->isName("Squiggly") || oType->isName("StrikeOut"))
		pAnnot = new PdfWriter::CTextMarkupAnnotation(pXref);
	else if (oType->isName("Square") || oType->isName("Circle"))
		pAnnot = new PdfWriter::CSquareCircleAnnotation(pXref);
	else if (oType->isName("Polygon") || oType->isName("PolyLine"))
		pAnnot = new PdfWriter::CPolygonLineAnnotation(pXref);
	else if (oType->isName("FreeText"))
		pAnnot = new PdfWriter::CFreeTextAnnotation(pXref);
	else if (oType->isName("Caret"))
		pAnnot = new PdfWriter::CCaretAnnotation(pXref);
	else if (oType->isName("Stamp"))
		pAnnot = new PdfWriter::CStampAnnotation(pXref);
	else if (oType->isName("Redact"))
		pAnnot = new PdfWriter::CRedactAnnotation(pXref);
	else if (oType->isName("Popup"))
		pAnnot = new PdfWriter::CPopupAnnotation(pXref);
	else if (oType->isName("Link"))
		pAnnot = new PdfWriter::CLinkAnnotation(pXref);
	else if (oType->isName("Widget"))
	{
		char* sName = NULL;
		Object oFT;
		if (oAnnot->dictLookup("FT", &oFT)->isName())
			sName = oFT.getName();

		if (!sName)
		{
			Object oParent, oParent2;
			oAnnot->dictLookup("Parent", &oParent);
			while (oParent.isDict())
			{
				if (oParent.dictLookup("FT", &oFT)->isName())
				{
					sName = oFT.getName();
					break;
				}
				oFT.free();
				oParent.dictLookup("Parent", &oParent2);
				oParent.free();
				oParent = oParent2;
			}
			oParent.free();
		}

		if (!sName)
		{
			oFT.free();
			return new PdfWriter::CWidgetAnnotation(pXref, PdfWriter::EAnnotType::AnnotWidget);
		}
		if (strcmp("Btn", sName) == 0)
		{
			bool bPushButton = false;
			oFT.free();
			int nFf = 0;
			if (oAnnot->dictLookup("Ff", &oFT)->isInt())
				nFf = oFT.getInt();
			if (!nFf)
			{
				Object oParent, oParent2;
				oAnnot->dictLookup("Parent", &oParent);
				while (oParent.isDict())
				{
					if (oParent.dictLookup("Ff", &oFT)->isInt())
					{
						nFf = oFT.getInt();
						break;
					}
					oFT.free();
					oParent.dictLookup("Parent", &oParent2);
					oParent.free();
					oParent = oParent2;
				}
				oParent.free();
			}

			bPushButton = (bool)((nFf >> 16) & 1);
			if (bPushButton)
				pAnnot = new PdfWriter::CPushButtonWidget(pXref);
			else
				pAnnot = new PdfWriter::CCheckBoxWidget(pXref);
		}
		else if (strcmp("Tx", sName) == 0)
			pAnnot = new PdfWriter::CTextWidget(pXref);
		else if (strcmp("Ch", sName) == 0)
			pAnnot = new PdfWriter::CChoiceWidget(pXref);
		else if (strcmp("Sig", sName) == 0)
			pAnnot = new PdfWriter::CSignatureWidget(pXref);
		else
			pAnnot = new PdfWriter::CWidgetAnnotation(pXref, PdfWriter::EAnnotType::AnnotWidget);
		oFT.free();
	}
	return pAnnot;
}
PdfWriter::CExtGrState* CreateExtGState(Object* oState)
{
	PdfWriter::CExtGrState* pState = new PdfWriter::CExtGrState(NULL);
	return pState;
}

void AddWidgetParent(PdfWriter::CDocument* pDoc, CObjectsManager* pManager, PdfWriter::CObjectBase* pObj)
{
	if (pObj->GetType() != PdfWriter::object_type_DICT)
		return;
	PdfWriter::CDictObject* pDict = dynamic_cast<PdfWriter::CDictObject*>(pObj);
	if (!pDict)
		return;

	int nID = pManager->FindObj(pObj);
	if (nID < 0)
		return;

	if (pDict->GetDictType() == PdfWriter::dict_type_UNKNOWN)
	{
		if (pDoc->GetParent(nID))
			return;
		pDoc->AddParent(nID, pDict);
	}

	PdfWriter::CObjectBase* pObjParent = pDict->Get("Parent");
	if (pObjParent && pObjParent->GetType() == PdfWriter::object_type_DICT)
		AddWidgetParent(pDoc, pManager, pObjParent);

	if (pDict->GetDictType() != PdfWriter::dict_type_UNKNOWN)
		return;

	PdfWriter::CObjectBase* pObjKids = pDict->Get("Kids");
	if (!pObjKids || pObjKids->GetType() != PdfWriter::object_type_ARRAY)
		return;

	PdfWriter::CArrayObject* pKids = (PdfWriter::CArrayObject*)pObjKids;
	for (int i = 0; i < pKids->GetCount(); ++i)
		AddWidgetParent(pDoc, pManager, pKids->Get(i));
}
PdfWriter::CDictObject* GetWidgetParent(PDFDoc* pdfDoc, PdfWriter::CDocument* pDoc, Object* pParentRef, int nStartRefID)
{
	if (!pParentRef || !pParentRef->isRef() || !pdfDoc)
		return NULL;
	PdfWriter::CDictObject* pParent = pDoc->GetParent(pParentRef->getRefNum() + nStartRefID);
	if (pParent)
		return pParent;

	Object oParent;
	if (!pParentRef->fetch(pdfDoc->getXRef(), &oParent)->isDict())
	{
		oParent.free();
		return pParent;
	}

	PdfWriter::CXref* pXref = NULL;
	pParent = new PdfWriter::CDictObject();
	if (nStartRefID == 0)
	{
		pXref = new PdfWriter::CXref(pDoc, pParentRef->getRefNum());
		pXref->Add(pParent, pParentRef->getRefGen());
	}
	if (!pDoc->EditParent(pXref, pParent, pParentRef->getRefNum() + nStartRefID))
	{
		RELEASEOBJECT(pXref);
		oParent.free();
		return NULL;
	}

	SDictConvertContext ctx;
	for (int i = 0; i < oParent.dictGetLength(); ++i)
	{
		char* chKey = oParent.dictGetKey(i);
		if (strcmp("Parent", chKey) == 0)
		{
			Object oParentRef;
			oParent.dictGetValNF(i, &oParentRef);
			PdfWriter::CDictObject* pParent2 = GetWidgetParent(pdfDoc, pDoc, &oParentRef, nStartRefID);
			if (pParent2)
			{
				pParent->Add("Parent", pParent2);
				oParentRef.free();
				continue;
			}
			oParentRef.free();
		}
		Object oTemp;
		oParent.dictGetValNF(i, &oTemp);
		DictToCDictObjectUnified(&oTemp, ctx, pParent, chKey);
		oTemp.free();
	}

	oParent.free();
	return pParent;
}
// ── ctm ─────────────────────────────────────────────────────────────────────
struct CTMSnapshot
{
	double m[6];
};
void CopyCTM(const double* src, double* dst)
{
	for (int i = 0; i < 6; ++i)
		dst[i] = src[i];
}
void SkipInlineImage(Parser* parser)
{
	Object obj;
	parser->getObj(&obj);
	while (!obj.isEOF() && !obj.isCmd("ID"))
	{
		obj.free();
		parser->getObj(&obj);
	}
	obj.free();

	Stream* str = parser->getStream();

	int c1 = str->getChar();
	int c2 = str->getChar();
	while (true)
	{
		if (c1 == EOF)
			break;

		if (c1 == 'E' && c2 == 'I')
		{
			int c3 = str->lookChar();
			if (c3 == EOF || isspace(c3) || c3 == 0)
			{
				str->getChar();
				break;
			}
		}

		c1 = c2;
		c2 = str->getChar();
	}
}
void GetCTM(XRef* pXref, Object* oPage, double* dCTM)
{
	if (!oPage || !oPage->isDict())
		return;

	Object oContents;
	if (!oPage->dictLookup("Contents", &oContents) || (!oContents.isArray() && !oContents.isStream()))
	{
		oContents.free();
		return;
	}

	dCTM[0] = 1; dCTM[1] = 0;
	dCTM[2] = 0; dCTM[3] = 1;
	dCTM[4] = 0; dCTM[5] = 0;

	Parser* parser = new Parser(pXref, new Lexer(pXref, &oContents), gFalse);

	int nNumArgs = 0;
	Object oObj;
	Object pArgs[maxArgs];

	std::vector<CTMSnapshot> ctmStack;

	parser->getObj(&oObj);
	while (!oObj.isEOF())
	{
		if (oObj.isCmd())
		{
			if (oObj.isCmd("q"))
			{
				CTMSnapshot snap;
				CopyCTM(dCTM, snap.m);
				ctmStack.push_back(snap);
			}
			else if (oObj.isCmd("Q"))
			{
				if (!ctmStack.empty())
				{
					CopyCTM(ctmStack.back().m, dCTM);
					ctmStack.pop_back();
				}
			}
			else if (oObj.isCmd("cm") && nNumArgs > 5)
			{
				double a1 = dCTM[0];
				double b1 = dCTM[1];
				double c1 = dCTM[2];
				double d1 = dCTM[3];

				dCTM[0] = pArgs[0].getNum() * a1 + pArgs[1].getNum() * c1;
				dCTM[1] = pArgs[0].getNum() * b1 + pArgs[1].getNum() * d1;
				dCTM[2] = pArgs[2].getNum() * a1 + pArgs[3].getNum() * c1;
				dCTM[3] = pArgs[2].getNum() * b1 + pArgs[3].getNum() * d1;
				dCTM[4] = pArgs[4].getNum() * a1 + pArgs[5].getNum() * c1 + dCTM[4];
				dCTM[5] = pArgs[4].getNum() * b1 + pArgs[5].getNum() * d1 + dCTM[5];
			}
			else if (oObj.isCmd("BI"))
				SkipInlineImage(parser);

			oObj.free();
			for (int i = 0; i < nNumArgs; ++i)
				pArgs[i].free();
			nNumArgs = 0;
		}
		else if (nNumArgs < maxArgs)
			pArgs[nNumArgs++] = oObj;
		else
			oObj.free();

		parser->getObj(&oObj);
	}
	oObj.free();
	for (int i = 0; i < nNumArgs; ++i)
		pArgs[i].free();

	RELEASEOBJECT(parser);

	oContents.free();
}
void GetInvertCTM(double* dCTM, double* dInvCTM)
{
	double det = dCTM[0] * dCTM[3] - dCTM[1] * dCTM[2];
	if (fabs(det) > 1e-10)
	{
		double invDet = 1.0 / det;
		dInvCTM[0] =  dCTM[3] * invDet;
		dInvCTM[1] = -dCTM[1] * invDet;
		dInvCTM[2] = -dCTM[2] * invDet;
		dInvCTM[3] =  dCTM[0] * invDet;
		dInvCTM[4] = (dCTM[2] * dCTM[5] - dCTM[3] * dCTM[4]) * invDet;
		dInvCTM[5] = (dCTM[1] * dCTM[4] - dCTM[0] * dCTM[5]) * invDet;
	}
}

void DrawAnnot(Object* oAnnot, PdfWriter::CPage* pPage, PdfWriter::CDocument* pDoc, XRef* xref, CObjectsManager* pManager, int nStartRefID)
{
	Object oAP, oAPN, oObj;
	if (!oAnnot->dictLookup("AP", &oAP)->isDict() || !oAP.dictLookup("N", &oAPN)->isStream())
	{
		oAP.free();
		Object oTemp;
		if (!oAPN.isDict() || !oAnnot->dictLookup("AS", &oObj)->isName() || !oAPN.dictLookup(oObj.getName(), &oTemp)->isStream())
		{
			oTemp.free(); oAPN.free(); oObj.free();
			return;
		}
		oAPN.free();
		oAPN = oTemp;
	}
	oAP.free();

	PdfWriter::CXObject* pForm = pDoc->CreateForm();
	pPage->GrSave();
	pPage->ExecuteXObject(pForm);
	pPage->GrRestore();

	Object oTemp;
	double m[6] = { 1, 0, 0, 1, 0, 0 }, bbox[4] = { 0, 0, 0, 0 }, rect[4] = { 0, 0, 0, 0 };
	if (oAnnot->dictLookup("Rect", &oTemp)->isArray() && oTemp.arrayGetLength() == 4)
	{
		for (int j = 0; j < 4; ++j)
		{
			oTemp.arrayGet(j, &oObj);
			rect[j] = oObj.isNum() ? oObj.getNum() : 0;
			oObj.free();
		}

		if (rect[0] > rect[2])
			std::swap(rect[0], rect[2]);
		if (rect[1] > rect[3])
			std::swap(rect[1], rect[3]);
	}
	oTemp.free();

	SDictConvertContext ctx(pDoc, xref, pManager, nStartRefID);
	Dict* pODict = oAPN.streamGetDict();
	for (int nIndex = 0; nIndex < pODict->getLength(); ++nIndex)
	{
		char* chKey = pODict->getKey(nIndex);
		if (strcmp("Length", chKey) == 0 || strcmp("Filter", chKey) == 0)
		{
			oTemp.free();
			continue;
		}
		else if (strcmp("BBox", chKey) == 0 && pODict->getVal(nIndex, &oTemp)->isArray() && oTemp.arrayGetLength() == 4)
		{
			for (int j = 0; j < 4; ++j)
			{
				oTemp.arrayGet(j, &oObj);
				bbox[j] = oObj.isNum() ? oObj.getNum() : 0;
				oObj.free();
			}

			if (bbox[0] > bbox[2])
				std::swap(bbox[0], bbox[2]);
			if (bbox[1] > bbox[3])
				std::swap(bbox[1], bbox[3]);
		}
		else if (strcmp("Matrix", chKey) == 0 && pODict->getVal(nIndex, &oTemp)->isArray() && oTemp.arrayGetLength() == 6)
		{
			for (int j = 0; j < 6; ++j)
			{
				oTemp.arrayGet(j, &oObj);
				m[j] = oObj.getNum();
				oObj.free();
			}
		}
		oTemp.free();
		pODict->getValNF(nIndex, &oTemp);
		PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oTemp, ctx);
		pForm->Add(chKey, pBase);
		oTemp.free();
	}

	double formXMin, formYMin, formXMax, formYMax, x, y, sx, sy;
	x = bbox[0] * m[0] + bbox[1] * m[2] + m[4];
	y = bbox[0] * m[1] + bbox[1] * m[3] + m[5];
	formXMin = formXMax = x;
	formYMin = formYMax = y;
	x = bbox[0] * m[0] + bbox[3] * m[2] + m[4];
	y = bbox[0] * m[1] + bbox[3] * m[3] + m[5];
	if (x < formXMin)
		formXMin = x;
	else if (x > formXMax)
		formXMax = x;
	if (y < formYMin)
		formYMin = y;
	else if (y > formYMax)
		formYMax = y;
	x = bbox[2] * m[0] + bbox[1] * m[2] + m[4];
	y = bbox[2] * m[1] + bbox[1] * m[3] + m[5];
	if (x < formXMin)
		formXMin = x;
	else if (x > formXMax)
		formXMax = x;
	if (y < formYMin)
		formYMin = y;
	else if (y > formYMax)
		formYMax = y;
	x = bbox[2] * m[0] + bbox[3] * m[2] + m[4];
	y = bbox[2] * m[1] + bbox[3] * m[3] + m[5];
	if (x < formXMin)
		formXMin = x;
	else if (x > formXMax)
		formXMax = x;
	if (y < formYMin)
		formYMin = y;
	else if (y > formYMax)
		formYMax = y;

	if (formXMin == formXMax)
		sx = 1;
	else
		sx = (rect[2] - rect[0]) / (formXMax - formXMin);
	if (formYMin == formYMax)
		sy = 1;
	else
		sy = (rect[3] - rect[1]) / (formYMax - formYMin);
	double tx = -formXMin * sx + rect[0];
	double ty = -formYMin * sy + rect[1];

	m[0] *= sx;
	m[1] *= sy;
	m[2] *= sx;
	m[3] *= sy;
	m[4] = m[4] * sx + tx;
	m[5] = m[5] * sy + ty;

	pForm->Add("Matrix", PdfWriter::CArrayObject::CreateMatrix(m));

	PdfWriter::CStream* pStream = pForm->GetStream();
	Stream* pOStream = oAPN.getStream();
	pOStream->reset();
	int nChar = pOStream->getChar();
	while (nChar != EOF)
	{
		pStream->WriteChar(nChar);
		nChar = pOStream->getChar();
	}

	oAPN.free();
}

PdfWriter::CObjectBase* FillEncryptDictFromXRef(XRef* xref, Object* pTrailerDict, PdfWriter::CEncryptDict* pEncryptDict, const SDictConvertContext& ctx)
{
	PdfWriter::CObjectBase* pID = NULL;

	xref->offEncrypted();
	{
		CObjectGuard encrypt;
		if (pTrailerDict->dictLookup("Encrypt", encrypt.get()) && encrypt->isDict())
		{
			for (int i = 0; i < encrypt->dictGetLength(); ++i)
			{
				CObjectGuard oTemp;
				char* chKey = encrypt->dictGetKey(i);
				encrypt->dictGetValNF(i, oTemp.get());
				pEncryptDict->Add(chKey, DictToCDictObjectUnified(oTemp.get(), ctx));
			}
		}

		if (!pEncryptDict->Get("Length"))
			pEncryptDict->Add("Length", 40);

		CObjectGuard ID;
		if (pTrailerDict->dictLookup("ID", ID.get()) && ID->isArray())
		{
			CObjectGuard ID1;
			if (ID->arrayGet(0, ID1.get()) && ID1->isString())
			{
				pID = DictToCDictObjectUnified(ID1.get(), ctx);
				pEncryptDict->Add("ID", pID);
			}
		}
	}
	xref->onEncrypted();

	return pID;
}
bool MakeDocWritable(PDFDoc* pPDFDocument, const wchar_t* wsPassword, GBool bWritable)
{
	GString* owner_pswd = wsPassword ? NSStrings::CreateString(wsPassword) : NULL;
	GString* user_pswd  = wsPassword ? NSStrings::CreateString(wsPassword) : NULL;
	GBool    bOk        = pPDFDocument->makeWritable(bWritable, owner_pswd, user_pswd);
	RELEASEOBJECT(owner_pswd);
	RELEASEOBJECT(user_pswd);
	return bOk == gTrue;
}

struct SFontScanContext
{
	PDFDoc*                  pPDFDocument;
	NSFonts::IFontManager*   pFontManager;
	PdfWriter::CDocument*    pDoc;
	CPdfWriter*              pWriter;
	CObjectsManager*         pObjManager;
	PdfReader::CPdfFontList* pFontList;
	int                      nStartRefID;
};
bool VisitAndEmbedFont(const SFontScanContext& ctx, const PdfReader::SFontVisitContext& oFontCtx)
{
	XRef* xref = ctx.pPDFDocument->getXRef();

	if (!oFontCtx.pFontRefNF->isRef())
		return false;

	Ref oFontRef = oFontCtx.pFontRefNF->getRef();

	GfxFont* gfxFont = GfxFont::makeFont(xref, "F", oFontRef, oFontCtx.pFontDict);
	if (!gfxFont)
		return false;

	Ref oEmbRef;
	const std::wstring wsFontBaseName = NSStrings::GetStringFromUTF32(gfxFont->getName());
	GfxFontLoc* pFontLoc = NULL;
	const bool bLocatable = gfxFont->getEmbeddedFontID(&oEmbRef) ||
		PdfReader::IsBaseFont(wsFontBaseName) ||
		((pFontLoc = gfxFont->locateFont(xref, false)) && NSStrings::GetStringFromUTF32(pFontLoc->path).length());

	if (bLocatable)
	{
		std::wstring wsFileName, wsFontName;
		PdfReader::GetFont(xref, ctx.pFontManager, ctx.pFontList, gfxFont, wsFileName, wsFontName);

		if (!PdfReader::IsBaseFont(wsFontBaseName))
		{
			PdfReader::TFontEntry pFontEntry;
			if (ctx.pFontList->GetFont(&oFontRef, &pFontEntry))
			{
				PdfWriter::CFontEmbedded* pFont = ctx.pDoc->FindFontEmbedded(wsFileName, 0);
				if (pFont)
				{
					pFont->UpdateKey(oFontCtx.sFontKey);
				}
				else
				{
					CObjectGuard oFontResolved;
					xref->fetch(oFontRef.num, oFontRef.gen, oFontResolved.get());

					std::map<unsigned int, unsigned int> mCodeToWidth, mCodeToUnicode, mCodeToGID;
					const int nDW = PdfReader::CollectFontWidths(oFontCtx.pFontDict, mCodeToWidth);

					for (int j = 0; j < (int)pFontEntry.unLenUnicode; ++j)
						if (pFontEntry.pCodeToUnicode[j])
							mCodeToUnicode[j] = pFontEntry.pCodeToUnicode[j];

					for (int j = 0; j < (int)pFontEntry.unLenGID; ++j)
						if (pFontEntry.pCodeToGID[j])
							mCodeToGID[j] = pFontEntry.pCodeToGID[j];

					ctx.pWriter->AddFont(L"Embedded: " + wsFontName, false, false, wsFileName, 0);

					PdfWriter::CObjectBase* pObj = ctx.pObjManager->GetObj(oFontRef.num + ctx.nStartRefID);
					if (!pObj)
					{
						pObj = new PdfWriter::CObjectBase();
						pObj->SetRef(oFontRef.num, oFontRef.gen);
					}

					pFont = ctx.pDoc->CreateFontEmbedded(wsFileName, 0, oFontCtx.sFontKey,
						static_cast<PdfWriter::EFontType>(gfxFont->getType()), pObj,
						mCodeToWidth, mCodeToUnicode, mCodeToGID);
					pFont->SetDW(nDW);
				}
			}
		}
	}

	RELEASEOBJECT(pFontLoc);
	RELEASEOBJECT(gfxFont);
	return false;
}
void ScanFonts(PDFDoc* pPDFDocument, Dict* pResources, CPdfWriter* pWriter, CObjectsManager* pObjManager, CPdfReader* pReader)
{
	if (!pResources)
		return;

	SFontScanContext ctx;
	ctx.pPDFDocument = pPDFDocument;
	ctx.pFontManager = pReader->GetFontManager();
	ctx.pDoc         = pWriter->GetDocument();
	ctx.pWriter      = pWriter;
	ctx.pObjManager  = pObjManager;
	ctx.pFontList    = pReader->GetFontList(pPDFDocument);
	ctx.nStartRefID  = pReader->GetStartRefID(pPDFDocument);

	auto fnVisit = [&ctx](const PdfReader::SFontVisitContext& oFontCtx) -> bool
	{
		return VisitAndEmbedFont(ctx, oFontCtx);
	};

	std::vector<int> arrUniqueResources;
	PdfReader::ScanResourceFonts(pResources, 0, arrUniqueResources, fnVisit);

	pReader->SetFonts(ctx.pFontList);
}

void CreateOutlines(PDFDoc* pdfDoc, PdfWriter::CDocument* pDoc, OutlineItem* pOutlineItem, PdfWriter::COutline* pParent)
{
	std::string sTitle = NSStringExt::CConverter::GetUtf8FromUTF32(pOutlineItem->getTitle(), pOutlineItem->getTitleLength());
	PdfWriter::COutline* pOutline = pDoc->CreateOutline(pParent, sTitle.c_str());

	PdfWriter::CDestination* pDest = NULL;
	LinkAction* pLinkAction = pOutlineItem->getAction();
	if (pLinkAction && pLinkAction->getKind() == actionGoTo)
	{
		GString* str = ((LinkGoTo*)pLinkAction)->getNamedDest();
		LinkDest* pLinkDest = str ? pdfDoc->findDest(str) : ((LinkGoTo*)pLinkAction)->getDest();
		if (pLinkDest)
		{
			int pg;
			if (pLinkDest->isPageRef())
			{
				Ref pageRef = pLinkDest->getPageRef();
				pg = pdfDoc->findPage(pageRef.num, pageRef.gen);
			}
			else
				pg = pLinkDest->getPageNum();
			if (pg == 0)
				pg = 1;

			Ref* pPageRef = pdfDoc->getCatalog()->getPageRef(pg);
			PdfWriter::CObjectBase* pPageD = pDoc->GetEditPage(--pg);
			PdfWriter::CStructureTreeRoot* pSTR = pDoc->GetStructureTreeRoot();
			if (pPageD && pPageD->GetType() == PdfWriter::object_type_DICT)
			{
				if (pSTR)
				{
					PdfWriter::CDictObject* pStructAnchor = pSTR->CreateDestAnchor((PdfWriter::CDictObject*)pPageD, ((PdfWriter::CPage*)pPageD)->GetOrCreateDestAnchorMCID());
					pDest = pDoc->CreateDestination(pStructAnchor, true);
				}
				else
					pDest = pDoc->CreateDestination(pPageD, true);
			}
			else if (!pPageD && pPageRef->num > 0)
			{
				PdfWriter::CObjectBase* pBase = new PdfWriter::CObjectBase();
				pBase->SetRef(pPageRef->num, pPageRef->gen);
				pPageD = new PdfWriter::CProxyObject(pBase, true);
				pDest = pDoc->CreateDestination(pPageD, true);
			}	

			switch (pLinkDest->getKind())
			{
			case destXYZ:
			{
				pDest->SetXYZ(pLinkDest->getLeft(), pLinkDest->getTop(), pLinkDest->getZoom());
				break;
			}
			case destFit:
			{
				pDest->SetFit();
				break;
			}
			case destFitH:
			{
				pDest->SetFitH(pLinkDest->getTop());
				break;
			}
			case destFitV:
			{
				pDest->SetFitV(pLinkDest->getLeft());
				break;
			}
			case destFitR:
			{
				pDest->SetFitR(pLinkDest->getLeft(), pLinkDest->getBottom(), pLinkDest->getRight(), pLinkDest->getTop());
				break;
			}
			case destFitB:
			{
				pDest->SetFitB();
				break;
			}
			case destFitBH:
			{
				pDest->SetFitBH(pLinkDest->getTop());
				break;
			}
			case destFitBV:
			{
				pDest->SetFitBV(pLinkDest->getLeft());
				break;
			}
			}
		}
		if (str)
			RELEASEOBJECT(pLinkDest);
	}
	if (pDest)
		pOutline->SetDestination(pDest);

	pOutlineItem->open();
	GList* pList = pOutlineItem->getKids();
	if (!pList)
	{
		pOutlineItem->close();
		return;
	}
	for (int i = 0, num = pList->getLength(); i < num; i++)
	{
		OutlineItem* pOutlineItemKid = (OutlineItem*)pList->get(i);
		if (pOutlineItemKid)
			CreateOutlines(pdfDoc, pDoc, pOutlineItemKid, pOutline);
	}
	pOutlineItem->close();
}
}

HRESULT _ChangePassword(const std::wstring& wsPath, const std::wstring& wsPassword, CPdfReader* _pReader, CPdfWriter* _pWriter)
{
	if (!_pReader || !_pWriter)
		return S_FALSE;
	PDFDoc* pPDFDocument = _pReader->GetPDFDocument(0);
	if (!pPDFDocument)
		return S_FALSE;
	XRef* xref = pPDFDocument->getXRef();
	if (!xref)
		return S_FALSE;
	Object* trailerDict = xref->getTrailerDict();
	if (!trailerDict)
		return S_FALSE;

	PdfWriter::CDocument* pDoc = _pWriter->GetDocument();
	PdfWriter::CXref* pXref   = new PdfWriter::CXref(pDoc, 0);
	PdfWriter::CXref* m_pXref = new PdfWriter::CXref(pDoc, xref->getNumObjects()); // For new objects
	if (!xref || !pDoc || !pXref || !m_pXref)
	{
		RELEASEOBJECT(pXref);
		RELEASEOBJECT(m_pXref);
		return S_FALSE;
	}
	pXref->SetPrev(m_pXref);

	SDictConvertContext ctx;
	for (int i = 0; i < xref->getSize(); ++i)
	{
		XRefEntry* pEntry = xref->getEntry(i);
		if (pEntry->type == xrefEntryFree)
			continue;

		if (i != pXref->GetSizeXRef())
		{
			PdfWriter::CXref* pXref2 = new PdfWriter::CXref(pDoc, i);
			pXref2->SetPrev(pXref);
			pXref = pXref2;
		}

		Object oTemp;
		xref->fetch(i, pEntry->type == xrefEntryCompressed ? 0 : pEntry->gen, &oTemp);
		PdfWriter::CObjectBase* pObj = NULL;

		switch (oTemp.getType())
		{
		case objBool:
		{
			pObj = new PdfWriter::CBoolObject(oTemp.getBool());
			break;
		}
		case objInt:
		{
			pObj = new PdfWriter::CNumberObject(oTemp.getInt());
			break;
		}
		case objReal:
		{
			pObj = new PdfWriter::CRealObject(oTemp.getReal());
			break;
		}
		case objString:
		{
			GString* str = oTemp.getString();
			if (str->isBinary())
			{
				int nLength = str->getLength();
				BYTE* arrId = new BYTE[nLength];
				for (int nIndex = 0; nIndex < nLength; ++nIndex)
					arrId[nIndex] = str->getChar(nIndex);
				pObj = new PdfWriter::CBinaryObject(arrId, nLength, false);
			}
			else
			{
				TextString* s = new TextString(str);
				std::string sValue = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
				pObj = new PdfWriter::CStringObject(sValue.c_str(), !s->isPDFDocEncoding());
				delete s;
			}
			break;
		}
		case objName:
		{
			pObj = new PdfWriter::CNameObject(oTemp.getName());
			break;
		}
		case objNull:
		{
			pObj = new PdfWriter::CNullObject();
			break;
		}
		case objArray:
		{
			pObj = new PdfWriter::CArrayObject();

			for (int nIndex = 0; nIndex < oTemp.arrayGetLength(); ++nIndex)
			{
				Object oT;
				oTemp.arrayGetNF(nIndex, &oT);
				DictToCDictObjectUnified(&oT, ctx, pObj);
				oT.free();
			}
			break;
		}
		case objDict:
		{
			pObj = new PdfWriter::CDictObject();

			for (int nIndex = 0; nIndex < oTemp.dictGetLength(); ++nIndex)
			{
				Object oT;
				char* chKey = oTemp.dictGetKey(nIndex);
				oTemp.dictGetValNF(nIndex, &oT);
				DictToCDictObjectUnified(&oT, ctx, pObj, chKey);
				oT.free();
			}
			break;
		}
		case objRef:
		{
			PdfWriter::CObjectBase* pBase = new PdfWriter::CObjectBase();
			pBase->SetRef(oTemp.getRefNum(), oTemp.getRefGen());
			pObj = new PdfWriter::CProxyObject(pBase, true);
			break;
		}
		case objStream:
		{
			Dict* pDict = oTemp.streamGetDict();
			Object oObjStm;
			if (pDict->lookup("Type", &oObjStm)->isName("ObjStm"))
			{
				oObjStm.free();
				break;
			}
			oObjStm.free();

			PdfWriter::CDictObject* pDObj = new PdfWriter::CDictObject();
			pObj = pDObj;

			int nLength = 0;
			for (int nIndex = 0; nIndex < pDict->getLength(); ++nIndex)
			{
				Object oT;
				char* chKey = pDict->getKey(nIndex);
				if (strcmp("Length", chKey) == 0)
				{
					Object oLength;
					nLength = pDict->getVal(nIndex, &oLength)->isInt() ? oLength.getInt() : 0;
					oLength.free();
					continue;
				}
				pDict->getValNF(nIndex, &oT);
				DictToCDictObjectUnified(&oT, ctx, pObj, chKey);
				oT.free();
			}

			PdfWriter::CStream* pStream = new PdfWriter::CMemoryStream();
			pDObj->SetStream(m_pXref, pStream, false);

			Stream* pImage = oTemp.getStream()->getUndecodedStream();
			pImage->reset();
			for (int nI = 0; nI < nLength; ++nI)
				pStream->WriteChar(pImage->getChar());
			break;
		}
		case objNone:
		case objCmd:
		case objError:
		case objEOF:
		default:
			break;
		}
		oTemp.free();

		if (pObj)
			pXref->Add(pObj);
	}

	PdfWriter::CDictObject* pTrailer = pXref->GetTrailer();
	for (int nIndex = 0; nIndex < trailerDict->dictGetLength(); ++nIndex)
	{
		Object oTemp;
		char* chKey = trailerDict->dictGetKey(nIndex);
		if (strcmp("Root", chKey) == 0 || strcmp("Info", chKey) == 0)
		{
			trailerDict->dictGetValNF(nIndex, &oTemp);
			DictToCDictObjectUnified(&oTemp, ctx, pTrailer, chKey);
		}
		oTemp.free();
	}

	bool bRes = pDoc->SaveNewWithPassword(pXref, m_pXref, wsPath, wsPassword, wsPassword, pTrailer);

	RELEASEOBJECT(pXref);

	return bRes ? S_OK : S_FALSE;
}
PdfWriter::CObjectBase* DictToCDictObjectUnified(Object* obj, SDictConvertContext ctx, PdfWriter::CObjectBase* pParent, const std::string& sKey)
{
	PdfWriter::CObjectBase* pBase = NULL;
	Object oTemp;

	switch (obj->getType())
	{
	// ------------------------------------------------------------------ scalar
	case objBool:
	{
		bool b = obj->getBool() == gTrue;
		if (pParent) { _UC_AddToParent(b); return NULL; }
		pBase = new PdfWriter::CBoolObject(b);
		break;
	}
	case objInt:
	{
		int n = obj->getInt();
		if (pParent) { _UC_AddToParent(n); return NULL; }
		pBase = new PdfWriter::CNumberObject(n);
		break;
	}
	case objReal:
	{
		double d = obj->getReal();
		if (pParent) { _UC_AddToParent(d); return NULL; }
		pBase = new PdfWriter::CRealObject(d);
		break;
	}
	case objName:
	{
		char* c = obj->getName();
		if (pParent) { _UC_AddToParent(c); return NULL; }
		pBase = new PdfWriter::CNameObject(c);
		break;
	}
	case objNone:
	{
		const char* c = "None";
		if (pParent) { _UC_AddToParent(c); return NULL; }
		pBase = new PdfWriter::CNameObject(c);
		break;
	}
	case objNull:
	{
		PdfWriter::CNullObject* p = new PdfWriter::CNullObject();
		if (pParent) { _UC_AddToParent(p); return NULL; }
		pBase = p;
		break;
	}
	// ------------------------------------------------------------------ string
	case objString:
	{
		GString* str = obj->getString();
		PdfWriter::CObjectBase* pStr = NULL;
		if (str->isBinary() || ctx.bMakeBinary)
		{
			int nLen = str->getLength();
			BYTE* arr = new BYTE[nLen];
			for (int i = 0; i < nLen; ++i)
				arr[i] = str->getChar(i);
			pStr = new PdfWriter::CBinaryObject(arr, nLen, false);
		}
		else
		{
			TextString* s = new TextString(str);
			if (s->isPDFDocEncoding())
				pStr = new PdfWriter::CStringObject(str->getCString(), false);
			else
			{
				std::string sValue = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
				pStr = new PdfWriter::CStringObject(sValue.c_str(), true);
			}
			delete s;
		}
		if (pParent) { _UC_AddToParent(pStr); return NULL; }
		pBase = pStr;
		break;
	}
	// ------------------------------------------------------------------ array
	case objArray:
	{
		PdfWriter::CArrayObject* pArray = new PdfWriter::CArrayObject();
		if (ctx.Has(EDictConvertFlags::eDCF_RegisterObjs) && ctx.nAddObjToXRef > 0)
		{
			ctx.pDoc->AddObject(pArray);
			ctx.pManager->AddObj(ctx.nAddObjToXRef + ctx.nStartRefID, pArray);
			ctx.nAddObjToXRef = 0;
		}
		if (pParent) _UC_AddToParent(pArray);

		SDictConvertContext childCtx = ctx;
		childCtx.nAddObjToXRef = 0;
		for (int i = 0; i < obj->arrayGetLength(); ++i)
		{
			obj->arrayGetNF(i, &oTemp);
			PdfWriter::CObjectBase* pElem = DictToCDictObjectUnified(&oTemp, childCtx);
			pArray->Add(pElem);
			oTemp.free();
		}
		pBase = pArray;
		break;
	}
	// ------------------------------------------------------------------ dict
	case objDict:
	{
		if (ctx.Has(EDictConvertFlags::eDCF_RegisterObjs) && SplitSkipDict(obj, ctx.pManager, ctx.nStartRefID))
			return NULL;

		PdfWriter::CDictObject* pDict = NULL;

		if (ctx.Has(EDictConvertFlags::eDCF_HandleAnnots))
		{
			Object oSubtype;
			if (obj->dictLookup("Subtype", &oSubtype)->isName())
			{
				PdfWriter::CAnnotation* pAnnot = CreateAnnot(obj, &oSubtype, NULL);
				if (pAnnot)
				{
					ctx.pDoc->AddAnnotation(ctx.nAddObjToXRef + ctx.nStartRefID, pAnnot);
					pDict = pAnnot;
				}
			}
			oSubtype.free();

			Object oType;
			if (obj->dictLookup("Type", &oType)->isName("ExtGState"))
			{
				PdfWriter::CExtGrState* pState = CreateExtGState(obj);
				if (pState)
				{
					ctx.pDoc->AddExtGState(pState);
					pDict = pState;
				}
			}
			oType.free();
		}

		if (!pDict)
			pDict = new PdfWriter::CDictObject();

		if (ctx.Has(EDictConvertFlags::eDCF_RegisterObjs) && ctx.nAddObjToXRef > 0)
		{
			ctx.pDoc->AddObject(pDict);
			ctx.pManager->AddObj(ctx.nAddObjToXRef + ctx.nStartRefID, pDict);
			ctx.nAddObjToXRef = 0;
		}
		if (pParent) _UC_AddToParent(pDict);

		SDictConvertContext childCtx = ctx;
		childCtx.nAddObjToXRef = 0;

		for (int i = 0; i < obj->dictGetLength(); ++i)
		{
			char* chKey = obj->dictGetKey(i);

			if (ctx.Has(EDictConvertFlags::eDCF_HandleResources) && strcmp("Resources", chKey) == 0)
			{
				Ref oResRef = { -1, -1 };
				if (obj->dictGetValNF(i, &oTemp)->isRef())
					oResRef = oTemp.getRef();
				oTemp.free();

				PdfWriter::CObjectBase* pExisting = oResRef.num > 0 ? ctx.pManager->GetObj(oResRef.num + ctx.nStartRefID) : NULL;
				if (pExisting)
				{
					pDict->Add(chKey, pExisting);
					ctx.pManager->IncRefCount(oResRef.num + ctx.nStartRefID);
					continue;
				}

				if (obj->dictGetVal(i, &oTemp)->isDict())
				{
					PdfWriter::CResourcesDict* pResDict = ctx.pDoc->CreateResourcesDict(oResRef.num < 0, false);
					if (oResRef.num > 0)
						ctx.pManager->AddObj(oResRef.num + ctx.nStartRefID, pResDict);
					pDict->Add(chKey, pResDict);

					for (int j = 0; j < oTemp.dictGetLength(); ++j)
					{
						Object oRes;
						char* chKey2 = oTemp.dictGetKey(j);
						oTemp.dictGetValNF(j, &oRes);
						PdfWriter::CObjectBase* pResItem = DictToCDictObjectUnified(&oRes, childCtx);
						pResDict->Add(chKey2, pResItem);
						oRes.free();
					}
					pResDict->Fix();
					oTemp.free();
					continue;
				}
				else
				{
					oTemp.free();
					obj->dictGetValNF(i, &oTemp);
				}
			}
			else
				obj->dictGetValNF(i, &oTemp);

			if (pParent)
				DictToCDictObjectUnified(&oTemp, childCtx, pDict, chKey);
			else
			{
				PdfWriter::CObjectBase* pVal = DictToCDictObjectUnified(&oTemp, childCtx);
				pDict->Add(chKey, pVal);
			}
			oTemp.free();
		}
		pBase = pDict;
		break;
	}
	// ------------------------------------------------------------------ ref
	case objRef:
	{
		if (!ctx.Has(EDictConvertFlags::eDCF_ResolveRefs))
		{
			PdfWriter::CObjectBase* pRefBase = new PdfWriter::CObjectBase();
			pRefBase->SetRef(obj->getRefNum(), obj->getRefGen());
			PdfWriter::CProxyObject* pProxy = new PdfWriter::CProxyObject(pRefBase, true);
			if (pParent) _UC_AddToParent(pProxy);
			return pProxy;
		}

		int nObjNum = obj->getRefNum();
		PdfWriter::CObjectBase* pCached = ctx.pManager->GetObj(nObjNum + ctx.nStartRefID);
		if (pCached)
		{
			ctx.pManager->IncRefCount(nObjNum + ctx.nStartRefID);
			return pCached;
		}

		std::vector<int>::iterator it = std::find(ctx.pManager->m_arrRedactFormFieldsID.begin(), ctx.pManager->m_arrRedactFormFieldsID.end(), nObjNum);
		if (it != ctx.pManager->m_arrRedactFormFieldsID.end())
			return NULL;

		SDictConvertContext refCtx = ctx;
		refCtx.nAddObjToXRef = nObjNum;
		obj->fetch(ctx.xref, &oTemp);
		pBase = DictToCDictObjectUnified(&oTemp, refCtx);
		oTemp.free();
		break;
	}
	// ------------------------------------------------------------------ stream
	case objStream:
	{
		if (!ctx.Has(EDictConvertFlags::eDCF_HandleStreams))
			break;

		PdfWriter::CDictObject* pDict = new PdfWriter::CDictObject();
		if (ctx.nAddObjToXRef > 0)
		{
			ctx.pDoc->AddObject(pDict);
			ctx.pManager->AddObj(ctx.nAddObjToXRef + ctx.nStartRefID, pDict);
			ctx.nAddObjToXRef = 0;
		}

		SDictConvertContext childCtx = ctx;
		childCtx.nAddObjToXRef = 0;

		Dict* pODict = obj->streamGetDict();
		int nLength = 0;
		for (int i = 0; i < pODict->getLength(); ++i)
		{
			char* chKey = pODict->getKey(i);
			if (strcmp("Length", chKey) == 0)
			{
				pODict->getVal(i, &oTemp);
				nLength = oTemp.getNum();
				PdfWriter::CNumberObject* pLenObj = new PdfWriter::CNumberObject(nLength);
				ctx.pDoc->AddObject(pLenObj);
				pDict->Add(chKey, pLenObj);
				oTemp.free();
				continue;
			}
			else if (ctx.Has(EDictConvertFlags::eDCF_HandleResources) && strcmp("Resources", chKey) == 0)
			{
				Ref oResRef = { -1, -1 };
				if (pODict->getValNF(i, &oTemp)->isRef())
					oResRef = oTemp.getRef();
				oTemp.free();

				PdfWriter::CObjectBase* pExisting = oResRef.num > 0 ? ctx.pManager->GetObj(oResRef.num + ctx.nStartRefID) : nullptr;
				if (pExisting)
				{
					pDict->Add(chKey, pExisting);
					ctx.pManager->IncRefCount(oResRef.num + ctx.nStartRefID);
					continue;
				}

				if (pODict->getVal(i, &oTemp)->isDict())
				{
					PdfWriter::CResourcesDict* pResDict = ctx.pDoc->CreateResourcesDict(oResRef.num < 0, false);
					if (oResRef.num > 0)
						ctx.pManager->AddObj(oResRef.num + ctx.nStartRefID, pResDict);
					pDict->Add(chKey, pResDict);

					for (int j = 0; j < oTemp.dictGetLength(); ++j)
					{
						Object oRes;
						char* chKey2 = oTemp.dictGetKey(j);
						oTemp.dictGetValNF(j, &oRes);
						PdfWriter::CObjectBase* pResItem = DictToCDictObjectUnified(&oRes, childCtx);
						pResDict->Add(chKey2, pResItem);
						oRes.free();
					}
					pResDict->Fix();
					oTemp.free();
					continue;
				}
				else
				{
					oTemp.free();
					pODict->getValNF(i, &oTemp);
				}
			}
			else
				pODict->getValNF(i, &oTemp);

			pBase = DictToCDictObjectUnified(&oTemp, childCtx);
			pDict->Add(chKey, pBase);
			oTemp.free();
		}
		pBase = pDict;

		PdfWriter::CStream* pStream = new PdfWriter::CMemoryStream(nLength);
		pDict->SetStream(pStream);
		Stream* pOStream = ctx.bUndecodedStream ? obj->getStream()->getUndecodedStream() : obj->getStream();
		pOStream->reset();
		int nChar = pOStream->getChar();
		while (nChar != EOF)
		{
			pStream->WriteChar(nChar);
			nChar = pOStream->getChar();
		}
		break;
	}
	case objCmd:
	case objError:
	case objEOF:
		break;
	}

	if (ctx.Has(EDictConvertFlags::eDCF_RegisterObjs) && ctx.nAddObjToXRef > 0 && pBase)
	{
		ctx.pDoc->AddObject(pBase);
		ctx.pManager->AddObj(ctx.nAddObjToXRef + ctx.nStartRefID, pBase);
	}

	return pBase;
}
// ============================================================
// CObjectsManager
// ============================================================
void CObjectsManager::AddObj(int nID, PdfWriter::CObjectBase* pObj)
{
	if (m_mUniqueRef.find(nID) == m_mUniqueRef.end())
		m_mUniqueRef[nID] = { pObj, 1 };
}
void CObjectsManager::RemoveObj(int nID)
{
	m_mUniqueRef.erase(nID);
}
PdfWriter::CObjectBase* CObjectsManager::GetObj(int nID)
{
	if (m_mUniqueRef.find(nID) != m_mUniqueRef.end())
		return m_mUniqueRef[nID].pObj;
	return NULL;
}
bool CObjectsManager::IncRefCount(int nID)
{
	if (m_mUniqueRef.find(nID) != m_mUniqueRef.end())
	{
		m_mUniqueRef[nID].nRefCount++;
		return true;
	}
	return false;
}
bool CObjectsManager::DecRefCount(int nID)
{
	if (m_mUniqueRef.find(nID) != m_mUniqueRef.end())
	{
		if (m_pDoc)
			m_pDoc->RemoveObj(m_mUniqueRef[nID].pObj);
		else
			m_mUniqueRef[nID].pObj->SetHidden();
		return true;
	}
	return false;
}
int CObjectsManager::FindObj(PdfWriter::CObjectBase* pObj)
{
	std::map<int, CObjectInfo>::iterator it = std::find_if(m_mUniqueRef.begin(), m_mUniqueRef.end(), [pObj](const std::pair<int, CObjectInfo>& t){ return t.second.pObj == pObj; });
	if (it != m_mUniqueRef.end())
		return it->first;
	return -1;
}
void CObjectsManager::DeleteObjTree(Object* obj, XRef* xref, int nStartRefID)
{
	Object oTemp;
	switch (obj->getType())
	{
	case objBool:
	case objInt:
	case objReal:
	case objString:
	case objName:
	case objNull:
	case objNone:
	case objCmd:
	case objError:
	case objEOF:
		break;
	case objArray:
	{
		for (int nIndex = 0; nIndex < obj->arrayGetLength(); ++nIndex)
		{
			obj->arrayGetNF(nIndex, &oTemp);
			DeleteObjTree(&oTemp, xref, nStartRefID);
			oTemp.free();
		}
		break;
	}
	case objDict:
	{
		if (SplitSkipDict(obj, this, nStartRefID))
			return;
		for (int nIndex = 0; nIndex < obj->dictGetLength(); ++nIndex)
		{
			obj->dictGetValNF(nIndex, &oTemp);
			DeleteObjTree(&oTemp, xref, nStartRefID);
			oTemp.free();
		}
		break;
	}
	case objRef:
	{
		int nObjNum = obj->getRefNum();
		PdfWriter::CObjectBase* pObj = GetObj(nObjNum + nStartRefID);
		if (pObj && DecRefCount(nObjNum + nStartRefID))
			return;

		obj->fetch(xref, &oTemp);
		DeleteObjTree(&oTemp, xref, nStartRefID);
		oTemp.free();
		break;
	}
	case objStream:
	{
		Dict* pODict = obj->streamGetDict();
		for (int nIndex = 0; nIndex < pODict->getLength(); ++nIndex)
		{
			pODict->getValNF(nIndex, &oTemp);
			DeleteObjTree(&oTemp, xref, nStartRefID);
			oTemp.free();
		}
		break;
	}
	}
}
void CObjectsManager::SetDoc(PdfWriter::CDocument* pDoc) { m_pDoc = pDoc; }
// ============================================================
// CPdfEditor
// ============================================================
CPdfEditor::CPdfEditor(const std::wstring& _wsSrcFile, const wchar_t* _wsPassword, const std::wstring& _wsDstFile, CPdfReader* _pReader, CPdfWriter* _pWriter, Mode nMode)
{
	m_wsSrcFile  = _wsSrcFile;
	m_wsDstFile  = _wsDstFile;
	m_wsPassword = _wsPassword;
	m_pReader    = _pReader;
	m_pWriter    = _pWriter;
	m_nEditPage  = -1;
	m_nError     = 0;
	m_nMode      = Mode::Unknown;
	m_nRedactInfo = 0;
	m_nOriginIndex = 0;

	PDFDoc*               pPDFDocument = m_pReader->GetPDFDocument(0);
	XRef*                 xref         = pPDFDocument ? pPDFDocument->getXRef() : NULL;
	PdfWriter::CDocument* pDoc         = m_pWriter->GetDocument();
	if (!pPDFDocument || !xref || !pDoc)
	{
		m_nError = 1;
		return;
	}

	m_nOriginIndex = m_pReader->GetNumPages();
	SetMode(nMode);
}
void CPdfEditor::SetMode(Mode nMode)
{
	if (m_nMode != Mode::Unknown)
		return;
	m_nMode = nMode;
	if (m_nMode == Mode::WriteAppend)
	{
		IncrementalUpdates();
		return;
	}
	if (m_nMode == Mode::WriteNew)
	{
		PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
		pDoc->CreateNew();
		PdfWriter::CPageTree* pPageTree = pDoc->GetPageTree();
		m_mObjManager.SetDoc(pDoc);
		pPageTree->CreateFakePages(m_pReader->GetNumPages());
		m_pWriter->SetNeedAddHelvetica(false);

		SyncAcroFormFromReader();
	}
}
void CPdfEditor::RedactInfo(int nFlag, const std::vector<IAdvancedCommand*>& arrForms)
{
	if (m_nMode != Mode::Unknown)
		return;
	m_pWriter->GetDocument()->SetRedactInfo(nFlag);
	m_nRedactInfo = nFlag;
	// 0 - Metadata
	// 1 - FileAttachments
	// 2 - Bookmarks
	// 3 - Embedded search index
	// 4 - Comments and markups
	// 5 - Form Fields
	// 6 - Hidden text
	// 7 - Hidden layers
	// 8 - Deleted or cropped content
	// 9 - Links, actions and JavaScripts
	// 10 - Overlapping Objects

	m_mObjManager.m_arrRedactFormFields = arrForms;
	for (int i = 0; i < arrForms.size(); ++i)
	{
		IAdvancedCommand::AdvancedCommandType nType = arrForms[i]->GetCommandType();
		if (nType == IAdvancedCommand::AdvancedCommandType::Annotation)
		{
			// Annotation - annotation that has changed or been created and needs to be drawn in Contents of page
			CAnnotFieldInfo* command = dynamic_cast<CAnnotFieldInfo*>(arrForms[i]);
			m_mObjManager.m_arrRedactFormFieldsID.push_back(command->GetID());
		}
		else if (nType == IAdvancedCommand::AdvancedCommandType::RedactAnnot)
		{
			// RedactAnnot - annotation that needs to be drawn in Contents of page without changes
			CRedactAnnot* command = dynamic_cast<CRedactAnnot*>(arrForms[i]);
			m_mObjManager.m_arrRedactFormFieldsID.push_back(command->GetID());
		}
	}
}
// ============================================================
// CPdfEditor::IncrementalUpdates
// ============================================================
namespace
{
// Returns the unique 1-based index for the next "SigN" field name
unsigned int ComputeInitialSigFieldIndex(AcroForm* form)
{
	if (!form)
		return 0;

	const int nFormFields   = form->getNumFields();
	unsigned int nFormField = nFormFields + 1;
	std::wstring sSig       = L"Sig" + std::to_wstring(nFormField);

	for (int i = 0; i < nFormFields; )
	{
		int nLength = 0;
		Unicode* uName = form->getField(i)->getName(&nLength);
		std::wstring sName = NSStringExt::CConverter::GetUnicodeFromUTF32(uName, nLength);
		RELEASEMEM(uName);

		if (sName == sSig)
		{
			++nFormField;
			sSig = L"Sig" + std::to_wstring(nFormField);
			i    = 0; // restart scan
		}
		else
			++i;
	}
	return nFormField - 1;
}
// Builds a PdfWriter::CDictObject for AcroForm, also creates DR xref/dict.
PdfWriter::CDictObject* BuildAcroForm(
	Object&               oAcroForm,
	XRef*                 xref,
	PdfWriter::CDocument* pDoc,
	SDictConvertContext&  ctx,
	PdfWriter::CXref**    ppDRXref,   // out
	PdfWriter::CResourcesDict** ppDR) // out
{
	*ppDRXref = NULL;
	*ppDR     = NULL;

	PdfWriter::CDictObject* pAcroForm = new(std::nothrow) PdfWriter::CDictObject();
	if (!pAcroForm)
		return NULL;

	for (int i = 0, nLen = oAcroForm.dictGetLength(); i < nLen; ++i)
	{
		char* chKey = oAcroForm.dictGetKey(i);

		if (strcmp("DR", chKey) == 0)
		{
			CObjectGuard oTemp2, oDR;
			if (!oAcroForm.dictGetVal(i, oTemp2.get())->isDict())
				continue;

			oAcroForm.dictGetValNF(i, oDR.get());
			const int nDRNum = oDR->isRef() ? oDR->getRefNum() : xref->getNumObjects();
			const int nDRGen = oDR->isRef() ? oDR->getRefGen() : 0;

			PdfWriter::CXref*          pDRXref = new(std::nothrow) PdfWriter::CXref(pDoc, nDRNum);
			PdfWriter::CResourcesDict* pDR     = new(std::nothrow) PdfWriter::CResourcesDict(NULL, true, false);
			if (!pDRXref || !pDR)
			{
				RELEASEOBJECT(pDRXref);
				RELEASEOBJECT(pDR);
				RELEASEOBJECT(pAcroForm);
				return NULL;
			}

			pDRXref->Add(pDR, nDRGen);
			pAcroForm->Add(chKey, pDR);

			for (int j = 0; j < oTemp2->dictGetLength(); ++j)
			{
				CObjectGuard oEntry;
				char* chKey2 = oTemp2->dictGetKey(j);
				oTemp2->dictGetVal(j, oEntry.get());
				DictToCDictObjectUnified(oEntry.get(), ctx, pDR, chKey2);
			}
			pDR->Fix();

			*ppDRXref = pDRXref;
			*ppDR     = pDR;
			continue;
		}

		CObjectGuard oTemp2;
		if (strcmp("NeedAppearances", chKey) == 0)
			continue;
		else if (strcmp("Fields", chKey) == 0)
			oAcroForm.dictGetVal(i, oTemp2.get());
		else
			oAcroForm.dictGetValNF(i, oTemp2.get());

		DictToCDictObjectUnified(oTemp2.get(), ctx, pAcroForm, chKey);
	}

	if (!pAcroForm->Get("Fields"))
		pAcroForm->Add("Fields", new PdfWriter::CArrayObject());

	return pAcroForm;
}
// Builds writer catalog + xref from the reader's catalog dict.
// Returns false on failure; ppDRXref/ppDR may be null if no DR present.
bool BuildCatalogFromReader(
	Object&               catDict,
	XRef*                 xref,
	PdfWriter::CDocument* pDoc,
	const Ref&            catRef,
	PdfWriter::CXref**    ppXref,     // out
	PdfWriter::CCatalog** ppCatalog,  // out
	PdfWriter::CXref**    ppDRXref,   // out
	PdfWriter::CResourcesDict** ppDR) // out
{
	*ppXref    = NULL;
	*ppCatalog = NULL;
	*ppDRXref  = NULL;
	*ppDR      = NULL;

	PdfWriter::CXref*    pXref    = new(std::nothrow) PdfWriter::CXref(pDoc, catRef.num);
	PdfWriter::CCatalog* pCatalog = new(std::nothrow) PdfWriter::CCatalog();
	if (!pXref || !pCatalog)
	{
		RELEASEOBJECT(pXref);
		RELEASEOBJECT(pCatalog);
		return false;
	}
	pXref->Add(pCatalog, catRef.gen);

	SDictConvertContext ctx;
	for (int i = 0, nLen = catDict.dictGetLength(); i < nLen; ++i)
	{
		char* chKey = catDict.dictGetKey(i);

		if (strcmp("AcroForm", chKey) == 0)
		{
			CObjectGuard oAcroForm;
			catDict.dictGetVal(i, oAcroForm.get());

			PdfWriter::CXref*          pDRXref = NULL;
			PdfWriter::CResourcesDict* pDR     = NULL;
			PdfWriter::CDictObject* pAcroFormDict = BuildAcroForm(*oAcroForm.get(), xref, pDoc, ctx, &pDRXref, &pDR);

			if (!pAcroFormDict)
			{
				RELEASEOBJECT(pXref);
				RELEASEOBJECT(pCatalog);
				return false;
			}
			pCatalog->Add(chKey, pAcroFormDict);
			*ppDRXref = pDRXref;
			*ppDR     = pDR;
			continue;
		}

		CObjectGuard oVal;
		catDict.dictGetValNF(i, oVal.get());
		DictToCDictObjectUnified(oVal.get(), ctx, pCatalog, chKey);
	}

	*ppXref    = pXref;
	*ppCatalog = pCatalog;
	return true;
}
// Reads encryption parameters from xref and builds a CEncryptDict for writer.
// Returns nullptr if the PDF is not encrypted or on allocation failure.
PdfWriter::CEncryptDict* GetEncryptionFromReader(XRef* xref, const wchar_t* wsPassword, int& nCryptAlgorithm) // out
{
	nCryptAlgorithm = -1;
	if (!xref->isEncrypted())
		return NULL;

	Object* pTrailerDict = xref->getTrailerDict();
	if (!pTrailerDict)
		return NULL;

	CryptAlgorithm encAlgorithm;
	GBool          ownerPasswordOk;
	int            permFlags, keyLength, encVersion;
	xref->getEncryption(&permFlags, &ownerPasswordOk, &keyLength, &encVersion, &encAlgorithm);
	nCryptAlgorithm = encAlgorithm;

	PdfWriter::CEncryptDict* pEncryptDict = new(std::nothrow) PdfWriter::CEncryptDict();
	if (!pEncryptDict)
		return NULL;

	SDictConvertContext ctx;
	ctx.bMakeBinary = true;
	FillEncryptDictFromXRef(xref, pTrailerDict, pEncryptDict, ctx);

	pEncryptDict->SetRef(0, 0);
	pEncryptDict->Fix();

	std::wstring sPassword = wsPassword ? wsPassword : L"";
	pEncryptDict->SetPasswords(sPassword, sPassword);

	if (!pEncryptDict->UpdateKey(nCryptAlgorithm))
	{
		RELEASEOBJECT(pEncryptDict);
		return NULL; // caller checks nullptr as failure signal
	}
	return pEncryptDict;
}
}
bool CPdfEditor::IncrementalUpdates()
{
	if (m_nMode != Mode::Unknown)
		return true;
	m_nMode = Mode::WriteAppend;

	// ── acquire reader state ─────────────────────────────────────────────────
	PDFDoc*                  pPDFDocument = NULL;
	PdfReader::CPdfFontList* pFontList    = NULL;
	int                      nStartRefID  = 0;
	m_pReader->GetPageIndex(0, &pPDFDocument, &pFontList, &nStartRefID);

	XRef*                 xref = pPDFDocument->getXRef();
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	pDoc->CreateNew();

	// ── make dst file writable ───────────────────────────────────────────────
	const std::string sPathUtf8New = U_TO_UTF8(m_wsDstFile);
	const std::string sPathUtf8Old = U_TO_UTF8(m_wsSrcFile);
	const bool bSameFile = sPathUtf8Old == sPathUtf8New || NSSystemPath::NormalizePath(sPathUtf8Old) == NSSystemPath::NormalizePath(sPathUtf8New);

	if (bSameFile)
	{
		if (!MakeDocWritable(pPDFDocument, m_wsPassword, true))
			return false;
	}
	else
	{
		if (!NSFile::CFileBinary::Copy(m_wsSrcFile, m_wsDstFile))
			return false;
		NSFile::CFileBinary oFile;
		if (!oFile.OpenFile(m_wsDstFile, true))
			return false;
		oFile.CloseFile();
	}

	// ── read catalog ref and pages ref from xref ─────────────────────────────
	CObjectGuard catDict, catRefObj, pagesRefObj;

	if (!xref->getCatalog(catDict.get())->isDict() || !catDict->dictLookupNF("Pages", pagesRefObj.get()))
		return false;

	Object* trailer = xref->getTrailerDict();
	if (!trailer || !trailer->isDict() || !trailer->dictLookupNF("Root", catRefObj.get())->isRef())
		return false;

	const Ref catRef = catRefObj->getRef();
	// catRefObj goes out of scope at end of block; guards cover cleanup

	// ── build writer catalog ─────────────────────────────────────────────────
	PdfWriter::CXref*          pXref    = NULL;
	PdfWriter::CCatalog*       pCatalog = NULL;
	PdfWriter::CXref*          pDRXref  = NULL;
	PdfWriter::CResourcesDict* pDR      = NULL;

	if (!BuildCatalogFromReader(*catDict.get(), xref, pDoc, catRef, &pXref, &pCatalog, &pDRXref, &pDR))
		return false;

	// ── sig field index ──────────────────────────────────────────────────────
	AcroForm*    form       = pPDFDocument->getCatalog()->getForm();
	unsigned int nFormField = ComputeInitialSigFieldIndex(form);

	// ── encryption ───────────────────────────────────────────────────────────
	int                      nCryptAlgorithm = -1;
	PdfWriter::CEncryptDict* pEncryptDict    = GetEncryptionFromReader(xref, m_wsPassword, nCryptAlgorithm);

	// GetEncryptionFromReader returns NULL both for "not encrypted" and
	// for "UpdateKey failed". Distinguish the two:
	if (xref->isEncrypted() && !pEncryptDict)
	{
		RELEASEOBJECT(pXref);
		RELEASEOBJECT(pDRXref);
		return false;
	}

	// ── apply to writer ──────────────────────────────────────────────────────
	bool bRes = pDoc->EditPdf(xref->getLastXRefPos(), xref->getNumObjects() + 1, pXref, pCatalog, pEncryptDict, nFormField);
	if (bRes)
	{
		GetPageTree(xref, pagesRefObj.get());

		if (pDR && pDRXref)
			bRes = pDoc->EditResources(pDRXref, pDR);
	}

	// ── scan fonts from AcroForm DR ──────────────────────────────────────────
	if (form)
	{
		CObjectGuard oDR;
		Object* oAcroForm = form->getAcroFormObj();
		if (oAcroForm->dictLookup("DR", oDR.get())->isDict())
			ScanFonts(pPDFDocument, oDR->getDict(), m_pWriter, &m_mObjManager, m_pReader);
	}

	return bRes;
}
// ============================================================
// CPdfEditor::SyncAcroFormFromReader
// ============================================================
namespace
{
// Copies an indirect array (Fields or CO) from reader into pAcroForm.
// If the array already exists in pAcroForm, merges elements; otherwise creates it.
// For Fields only: calls AddWidgetParent on each element.
void MergeRefArray(
	Object&               oAcroForm,
	int                   nIndex,
	const char*           chKey,
	PdfWriter::CDocument* pDoc,
	CObjectsManager&      mObjManager,
	int                   nStartRefID,
	bool                  bIsFields)
{
	PdfWriter::CDictObject* pAcroForm = pDoc->GetAcroForm();
	CObjectGuard oRef;
	Ref oArrayRef = { -1, -1 };
	if (oAcroForm.dictGetValNF(nIndex, oRef.get())->isRef())
		oArrayRef = oRef->getRef();

	PdfWriter::CArrayObject* pArray = dynamic_cast<PdfWriter::CArrayObject*>(pAcroForm->Get(chKey));
	if (!pArray)
	{
		// Reuse already-converted object if available
		PdfWriter::CObjectBase* pObj = oArrayRef.num > 0 ? mObjManager.GetObj(oArrayRef.num + nStartRefID) : NULL;
		if (pObj)
		{
			pAcroForm->Add(chKey, pObj);
			mObjManager.IncRefCount(oArrayRef.num + nStartRefID);
			return;
		}
	}

	CObjectGuard oTemp;
	if (!oAcroForm.dictGetVal(nIndex, oTemp.get())->isArray())
		return;

	if (!pArray)
	{
		pArray = new PdfWriter::CArrayObject();
		if (oArrayRef.num > 0)
		{
			pDoc->AddObject(pArray);
			mObjManager.AddObj(oArrayRef.num + nStartRefID, pArray);
		}
		pAcroForm->Add(chKey, pArray);
	}

	for (int i = 0; i < oTemp->arrayGetLength(); ++i)
	{
		CObjectGuard oRes;
		if (!oTemp->arrayGetNF(i, oRes.get())->isRef())
			continue;

		PdfWriter::CObjectBase* pObj = mObjManager.GetObj(oRes->getRefNum() + nStartRefID);
		if (!pObj)
			continue;

		pArray->Add(pObj);
		mObjManager.IncRefCount(oRes->getRefNum() + nStartRefID);
		if (bIsFields)
			AddWidgetParent(pDoc, &mObjManager, pObj);
	}
}
// Merges DR dict from reader AcroForm into pDoc's CResourcesDict.
// Adds only keys not already present; scans fonts afterwards.
void MergeDRDict(
	Object&               oAcroForm,
	int                   nIndex,
	PdfWriter::CDocument* pDoc,
	PDFDoc*               pPDFDocument,
	CObjectsManager&      mObjManager,
	int                   nStartRefID,
	const SDictConvertContext& ctx,
	CPdfWriter*           pWriter,
	CPdfReader*           pReader)
{
	PdfWriter::CResourcesDict* pDR = pDoc->GetFieldsResources();
	if (!pDR)
		return;

	CObjectGuard oTemp;
	if (!oAcroForm.dictGetVal(nIndex, oTemp.get())->isDict())
		return;

	for (int j = 0; j < oTemp->dictGetLength(); ++j)
	{
		char* chKey2 = oTemp->dictGetKey(j);
		if (strcmp("ProcSet", chKey2) == 0)
			continue;

		CObjectGuard oTemp2;
		if (!oTemp->dictGetVal(j, oTemp2.get())->isDict())
			continue;

		PdfWriter::CDictObject* pDict = dynamic_cast<PdfWriter::CDictObject*>(pDR->Get(chKey2));

		if (!pDict)
		{
			// Sub-dict not present — add whole entry
			CObjectGuard oTempRef;
			if (oTemp->dictGetValNF(j, oTempRef.get())->isRef())
			{
				PdfWriter::CObjectBase* pObj = mObjManager.GetObj(oTempRef->getRefNum() + nStartRefID);
				if (pObj)
				{
					pDR->Add(chKey2, pObj);
					mObjManager.IncRefCount(oTempRef->getRefNum() + nStartRefID);
					continue;
				}
			}
			PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(oTemp2.get(), ctx);
			if (oTempRef->isRef())
				pDoc->AddObject(pBase);
			pDR->Add(chKey2, pBase);
		}
		else
		{
			// Sub-dict present — merge missing keys only
			for (int k = 0; k < oTemp2->dictGetLength(); ++k)
			{
				char* chKey3 = oTemp2->dictGetKey(k);
				if (pDict->Get(chKey3))
					continue;

				CObjectGuard oTempRef;
				if (oTemp2->dictGetValNF(k, oTempRef.get())->isRef())
				{
					PdfWriter::CObjectBase* pObj = mObjManager.GetObj(oTempRef->getRefNum() + nStartRefID);
					if (pObj)
					{
						pDict->Add(chKey3, pObj);
						mObjManager.IncRefCount(oTempRef->getRefNum() + nStartRefID);
						continue;
					}
				}
				PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(oTemp2.get(), ctx);
				if (oTempRef->isRef())
					pDoc->AddObject(pBase);
				pDict->Add(chKey3, pBase);
			}
		}
	}

	pDR->Fix();
	ScanFonts(pPDFDocument, oTemp->getDict(), pWriter, &mObjManager, pReader);
}
}
// Copies/merges AcroForm from reader into writer document.
// Called twice: at the start of WriteNew editing (pDoc is empty — builds skeleton)
// and at the end (pDoc has edits — fills in missing Fields/CO/DR entries).
void CPdfEditor::SyncAcroFormFromReader()
{
	PdfWriter::CDocument*    pDoc         = m_pWriter->GetDocument();
	PDFDoc*                  pPDFDocument = NULL;
	PdfReader::CPdfFontList* pFontList    = NULL;
	int                      nStartRefID  = 0;
	m_pReader->GetPageIndex(0, &pPDFDocument, &pFontList, &nStartRefID);
	XRef* xref = pPDFDocument->getXRef();

	CObjectGuard oCatalog;
	if (!xref->getCatalog(oCatalog.get())->isDict())
		return;

	CObjectGuard oAcroFormNF;
	if (!oCatalog->dictLookupNF("AcroForm", oAcroFormNF.get())->isRef() && !oAcroFormNF->isDict())
		return;

	// Resolve indirect ref if needed
	CObjectGuard oAcroFormResolved;
	Object* oAcroForm = oAcroFormNF.get();
	if (oAcroFormNF->isRef())
	{
		if (!oCatalog->dictLookup("AcroForm", oAcroFormResolved.get())->isDict())
			return;
		oAcroForm = oAcroFormResolved.get();
	}

	// Get or create AcroForm in writer
	PdfWriter::CDictObject* pAcroForm = pDoc->GetAcroForm();
	if (!pAcroForm)
	{
		pAcroForm = new PdfWriter::CDictObject();
		if (oAcroFormNF->isRef())
			pDoc->AddObject(pAcroForm);
		pDoc->SetAcroForm(pAcroForm);
	}
	else
		pAcroForm->Remove("NeedAppearances");

	SDictConvertContext ctx(pDoc, xref, &m_mObjManager, nStartRefID);

	for (int i = 0; i < oAcroForm->dictGetLength(); ++i)
	{
		char* chKey = oAcroForm->dictGetKey(i);
		if (strcmp("Fields", chKey) == 0)
		{
			MergeRefArray(*oAcroForm, i, chKey, pDoc, m_mObjManager, nStartRefID, true);
			continue;
		}
		if (strcmp("CO", chKey) == 0)
		{
			MergeRefArray(*oAcroForm, i, chKey, pDoc, m_mObjManager, nStartRefID, false);
			continue;
		}
		if (strcmp("DR", chKey) == 0)
		{
			MergeDRDict(*oAcroForm, i, pDoc, pPDFDocument, m_mObjManager, nStartRefID, ctx, m_pWriter, m_pReader);
			// Fall through to default only if DR was not a dict (handled inside)
			continue;
		}
		if (strcmp("SigFlags",        chKey) == 0 ||
			strcmp("XFA",             chKey) == 0 ||
			strcmp("DA",              chKey) == 0 ||
			strcmp("NeedAppearances", chKey) == 0)
			continue;

		// Default: copy value as-is
		CObjectGuard oTemp;
		oAcroForm->dictGetValNF(i, oTemp.get());
		pAcroForm->Add(chKey, DictToCDictObjectUnified(oTemp.get(), ctx));
	}
}
// ============================================================
// CPdfEditor::Close
// ============================================================
namespace
{
PdfWriter::CAnnotAppearanceObject* GetAnnotAppearanceObject(PdfWriter::CAnnotation* pAnnot)
{
	PdfWriter::CDictObject* pAP = dynamic_cast<PdfWriter::CDictObject*>(pAnnot->Get("AP"));
	if (!pAP)
		return NULL;

	PdfWriter::CDictObject* pAP_N = dynamic_cast<PdfWriter::CDictObject*>(pAP->Get("N"));
	if (!pAP_N)
		return NULL;

	PdfWriter::CAnnotAppearanceObject* pAPN = dynamic_cast<PdfWriter::CAnnotAppearanceObject*>(pAP_N);
	if (pAPN)
		return pAPN;

	const char* sName = "Off";
	PdfWriter::CNameObject* pAPNName = dynamic_cast<PdfWriter::CNameObject*>(pAnnot->Get("AS"));
	if (pAPNName)
		sName = pAPNName->Get();

	return dynamic_cast<PdfWriter::CAnnotAppearanceObject*>(pAP_N->Get(sName));
}
// ─── WriteNew helpers ────────────────────────────────────────────────────────
void CloseWriteNew_FlushAnnotations(CPdfReader* pReader, CObjectsManager* mObjManager, PdfWriter::CDocument* pDoc)
{
	for (size_t i = 0; i < mObjManager->m_arrRedactFormFields.size(); ++i)
	{
		IAdvancedCommand* pCmd = mObjManager->m_arrRedactFormFields[i];
		IAdvancedCommand::AdvancedCommandType nType = pCmd->GetCommandType();

		if (nType == IAdvancedCommand::AdvancedCommandType::Annotation)
		{
			// Annotation - annotation that has changed or been created and needs to be drawn in Contents of page
			CAnnotFieldInfo* command = dynamic_cast<CAnnotFieldInfo*>(pCmd);
			CAnnotFieldInfo& oInfo   = *command;
			if (!oInfo.IsWidget())
				continue;

			PdfWriter::CPage* pPage = pDoc->GetEditPage(oInfo.GetPage());
			if (!pPage)
				pPage = pDoc->GetPage(oInfo.GetPage());
			if (!pPage)
				continue;

			PdfWriter::CAnnotation* pAnnot = pDoc->GetAnnot(oInfo.GetID());
			if (!pAnnot)
				continue;

			PdfWriter::CAnnotAppearanceObject* pAPN = GetAnnotAppearanceObject(pAnnot);
			if (!pAPN)
			{
				pDoc->RemoveObj(pAnnot);
				continue;
			}

			double m[6] = { 1, 0, 0, 1, 0, 0 };
			pAPN->GetMatrix(m);
			m[4] += pAnnot->GetRect().fLeft;
			m[5] += pAnnot->GetRect().fBottom;
			pAPN->AddMatrix(m[0], m[1], m[2], m[3], m[4], m[5]);

			PdfWriter::CResourcesDict* pResources = pPage->GetResourcesItem();
			if (!pResources)
			{
				pPage->AddResource();
				pResources = pPage->GetResourcesItem();
			}

			pPage->GrSave();
			pPage->ExecuteXObject(pResources->GetXObjectName(pAPN));
			pPage->GrRestore();

			pDoc->RemoveObj(pAnnot);
		}
		else if (nType == IAdvancedCommand::AdvancedCommandType::RedactAnnot)
		{
			// RedactAnnot - annotation that needs to be drawn in Contents of page without changes
			CRedactAnnot* command = dynamic_cast<CRedactAnnot*>(pCmd);

			PDFDoc* pPDFDocument = NULL;
			int     nStartRefID  = 0;
			int     nRefID       = pReader->FindRefNum(command->GetID(), &pPDFDocument, &nStartRefID);
			if (nRefID < 0)
				continue;

			XRef*      xref   = pPDFDocument->getXRef();
			XRefEntry* pEntry = xref->getEntry(nRefID);

			CObjectGuard oAnnotRef, oAnnot;
			oAnnotRef->initRef(nRefID, pEntry->type == xrefEntryCompressed ? 0 : pEntry->gen);
			if (oAnnotRef->fetch(xref, oAnnot.get())->isNull() || !oAnnot->isDict())
				continue;

			CObjectGuard oP;
			if (!oAnnot->dictLookupNF("P", oP.get())->isRef())
				continue;

			PdfWriter::CPage* pPage = dynamic_cast<PdfWriter::CPage*>(mObjManager->GetObj(oP->getRefNum() + nStartRefID));
			if (!pPage)
				continue;

			DrawAnnot(oAnnot.get(), pPage, pDoc, xref, mObjManager, nStartRefID);
		}
	}
}
void CloseWriteNew_ApplyEncryption(const wchar_t* wsPassword, CObjectsManager* mObjManager, PDFDoc* pPDFDocument, PdfWriter::CDocument* pDoc)
{
	XRef* xref = pPDFDocument->getXRef();
	if (!xref->isEncrypted() || !xref->getTrailerDict())
		return;

	CryptAlgorithm encAlgorithm;
	GBool          ownerPasswordOk;
	int            permFlags, keyLength, encVersion;
	xref->getEncryption(&permFlags, &ownerPasswordOk, &keyLength, &encVersion, &encAlgorithm);
	const int nCryptAlgorithm = encAlgorithm;

	std::wstring sPassword = wsPassword ? wsPassword : L"";
	pDoc->SetPasswords(sPassword, sPassword);
	PdfWriter::CEncryptDict* pEncryptDict = pDoc->GetEncrypt();

	SDictConvertContext ctx(pDoc, xref, mObjManager, 0);
	ctx.bMakeBinary = true;
	PdfWriter::CObjectBase* pID = FillEncryptDictFromXRef(xref, xref->getTrailerDict(), pEncryptDict, ctx);

	pDoc->SetEncryption(pEncryptDict, pID);
	pEncryptDict->Fix();
	pEncryptDict->UpdateKey(nCryptAlgorithm);
}
void CloseWriteNew_ApplyNameTree(PDFDoc* pPDFDocument, PdfWriter::CDocument* pDoc)
{
	Object* pNameTree = pPDFDocument->getCatalog()->getNameTree();
	if (!pNameTree || !pNameTree->isDict())
		return;

	CObjectGuard oNames;
	if (!pNameTree->dictLookup("Names", oNames.get())->isArray())
		return;

	for (int i = 0, nLen = oNames->arrayGetLength(); i < nLen; i += 2)
	{
		CObjectGuard oName, oDest;
		if (!oNames->arrayGet(i, oName.get())->isString() || !oNames->arrayGet(i + 1, oDest.get())->isArray())
			continue;

		TextString* s = new TextString(oName->getString());
		std::string sName = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		PdfWriter::CStringObject* pName = new PdfWriter::CStringObject(sName.c_str(), !s->isPDFDocEncoding());
		delete s;

		PdfWriter::CDestination* pDest     = NULL;
		LinkDest*                pLinkDest = new LinkDest(oDest->getArray());
		if (pLinkDest && pLinkDest->isOk())
		{
			int nPage = 0;
			if (pLinkDest->isPageRef())
			{
				Ref pageRef = pLinkDest->getPageRef();
				nPage = pPDFDocument->findPage(pageRef.num, pageRef.gen);
			}
			else
				nPage = pLinkDest->getPageNum();

			pDest = pDoc->CreateDestination(pDoc->GetEditPage(nPage - 1), true);
		}
		delete pLinkDest;

		if (pName && pDest)
			pDoc->AddNameTree(pName, pDest);
		else
		{
			delete pName;
			delete pDest;
		}
	}
}
// ─── WriteAppend helpers ─────────────────────────────────────────────────────
// Returns false on allocation failure; pXref and pTrailer are always set on success.
bool CloseWriteAppend_BuildTrailer(
	PDFDoc*                  pPDFDocument,
	PdfWriter::CDocument*    pDoc,
	PdfWriter::CXref**       ppXref,     // out
	PdfWriter::CDictObject** ppTrailer)  // out
{
	*ppXref    = NULL;
	*ppTrailer = NULL;

	PdfWriter::CXref* pXref = new(std::nothrow) PdfWriter::CXref(pDoc, 0, 65535);
	if (!pXref)
		return false;

	Object* trailerDict = pPDFDocument->getXRef()->getTrailerDict();
	if (trailerDict)
	{
		PdfWriter::CDictObject* pTrailer = pXref->GetTrailer();
		SDictConvertContext ctx;
		for (int i = 0; i < trailerDict->dictGetLength(); ++i)
		{
			CObjectGuard oTemp;
			char* chKey = trailerDict->dictGetKey(i);
			trailerDict->dictGetValNF(i, oTemp.get());
			DictToCDictObjectUnified(oTemp.get(), ctx, pTrailer, chKey);
		}
		*ppTrailer = pTrailer;
	}

	*ppXref = pXref;
	return true;
}
// Returns false on allocation failure.
bool CloseWriteAppend_UpdateInfo(
	PDFDoc*                  pPDFDocument,
	PdfWriter::CDocument*    pDoc,
	PdfWriter::CXref*        pXref,
	PdfWriter::CDictObject*  pTrailer,
	PdfWriter::CXref**       ppInfoXref,  // out
	PdfWriter::CInfoDict**   ppInfoDict)  // out
{
	*ppInfoXref = NULL;
	*ppInfoDict = NULL;

	CObjectGuard info;
	pPDFDocument->getDocInfo(info.get());
	if (!info->isDict())
		return true;

	PdfWriter::CObjectBase* pInfo     = pTrailer ? pTrailer->Get("Info") : NULL;
	PdfWriter::CXref*       pInfoXref = new(std::nothrow) PdfWriter::CXref(pDoc, pInfo ? pInfo->GetObjId() : 0);
	if (!pInfoXref)
		return false;

	PdfWriter::CInfoDict* pInfoDict = new(std::nothrow) PdfWriter::CInfoDict();
	if (!pInfoDict)
	{
		RELEASEOBJECT(pInfoXref);
		return false;
	}
	pInfoXref->Add(pInfoDict, pInfo ? pInfo->GetGenNo() : 0);

	SDictConvertContext ctx;
	for (int i = 0; i < info->dictGetLength(); ++i)
	{
		CObjectGuard oTemp;
		char* chKey = info->dictGetKey(i);
		info->dictGetValNF(i, oTemp.get());
		DictToCDictObjectUnified(oTemp.get(), ctx, pInfoDict, chKey);
	}
	pInfoDict->SetTime(PdfWriter::InfoModaDate);

	*ppInfoXref = pInfoXref;
	*ppInfoDict = pInfoDict;
	return true;
}
void CloseWriteAppend_FinalizeSource(CPdfReader* pReader, const wchar_t* wsPassword, const std::wstring& wsDstFile, const std::wstring& wsSrcFile)
{
	PDFDoc* pPDFDocument = pReader->GetPDFDocument(0);
	const std::string sPathUtf8New = U_TO_UTF8(wsDstFile);
	const std::string sPathUtf8Old = U_TO_UTF8(wsSrcFile);
	const bool bSameFile = sPathUtf8Old == sPathUtf8New || NSSystemPath::NormalizePath(sPathUtf8Old) == NSSystemPath::NormalizePath(sPathUtf8New);

	if (!bSameFile)
		return;

	MakeDocWritable(pPDFDocument, wsPassword, false);

	NSFile::CFileBinary oFile;
	if (oFile.OpenFile(wsSrcFile))
	{
		pReader->ChangeLength(oFile.GetFileSize());
		oFile.CloseFile();
	}
}
}
void CPdfEditor::Close()
{
	if (m_wsDstFile.empty() && m_nMode != Mode::WriteNew)
		return;

	if (m_nMode == Mode::Unknown)
	{
		if (m_wsDstFile != m_wsSrcFile && NSSystemPath::NormalizePath(m_wsDstFile) != NSSystemPath::NormalizePath(m_wsSrcFile))
			NSFile::CFileBinary::Copy(m_wsSrcFile, m_wsDstFile);
		return;
	}

	if (m_nMode == Mode::Split)
	{
		m_pWriter->EditClose();
		m_pWriter->SaveToFile(m_wsDstFile);
		return;
	}

	if (m_nMode == Mode::WriteNew)
	{
		PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();

		PdfWriter::CPageTree* pPageTree = pDoc->GetPageTree();
		const int nPages = pPageTree->GetCount();
		for (int i = 0; i < nPages; ++i)
		{
			PdfWriter::CObjectBase* pObj = pPageTree->GetObj(i);
			if (pObj && pObj->GetType() != PdfWriter::object_type_DICT)
				EditPage(i, false, true);
		}

		CloseWriteNew_FlushAnnotations(m_pReader, &m_mObjManager, pDoc);

		SyncAcroFormFromReader();

		PDFDoc* pPDFDocument = NULL;
		PdfReader::CPdfFontList* pFontList = NULL;
		int nStartRefID = 0;
		m_pReader->GetPageIndex(0, &pPDFDocument, &pFontList, &nStartRefID);
		CloseWriteNew_ApplyEncryption(m_wsPassword, &m_mObjManager, pPDFDocument, pDoc);

		if (~m_nRedactInfo & (1 << 1))
		{
			XRef* xref = pPDFDocument->getXRef();
			CObjectGuard catDict;
			xref->getCatalog(catDict.get());
			CObjectGuard pNames, oEF, oNames;
			if (catDict->dictLookup("Names", pNames.get())->isDict() && pNames->dictLookup("EmbeddedFiles", oEF.get())->isDict())
			{
				SDictConvertContext ctx(pDoc, xref, &m_mObjManager, nStartRefID);
				PdfWriter::CCatalog* pCatalog = pDoc->GetCatalog();
				PdfWriter::CDictObject* pNames = pCatalog->GetNames();
				pNames->Add("EmbeddedFiles", DictToCDictObjectUnified(oEF.get(), ctx));
			}
		}
		if (~m_nRedactInfo & (1 << 2))
		{
			PDFDoc* pDocumentFirst = m_pReader->GetPDFDocument(0);
			Outline* pOutlineOld = pDocumentFirst->getOutline();
			GList* pListOld = NULL;
			if (pOutlineOld)
				pListOld = pOutlineOld->getItems();
			if (!pDoc->GetOutlines() && pListOld)
			{
				for (int i = 0, num = pListOld->getLength(); i < num; i++)
				{
					OutlineItem* pOutlineItem = (OutlineItem*)pListOld->get(i);
					if (pOutlineItem)
						CreateOutlines(pDocumentFirst, pDoc, pOutlineItem, NULL);
				}
			}
		}
		if (~m_nRedactInfo & (1 << 3))
			CloseWriteNew_ApplyNameTree(pPDFDocument, pDoc);

		m_pWriter->EditClose();
		m_pWriter->SaveToFile(m_wsDstFile);
		return;
	}

	// m_nMode == Mode::WriteAppend
	PDFDoc*               pPDFDocument = m_pReader->GetPDFDocument(0);
	PdfWriter::CDocument* pDoc         = m_pWriter->GetDocument();

	PdfWriter::CXref*       pXref    = NULL;
	PdfWriter::CDictObject* pTrailer = NULL;
	if (!CloseWriteAppend_BuildTrailer(pPDFDocument, pDoc, &pXref, &pTrailer))
		return;

	PdfWriter::CXref*     pInfoXref = NULL;
	PdfWriter::CInfoDict* pInfoDict = NULL;
	if (!CloseWriteAppend_UpdateInfo(pPDFDocument, pDoc, pXref, pTrailer, &pInfoXref, &pInfoDict))
	{
		RELEASEOBJECT(pXref);
		return;
	}

	if (!m_pWriter->EditClose() || !pDoc->AddToFile(m_wsDstFile, pXref, pTrailer, pInfoXref, pInfoDict))
	{
		RELEASEOBJECT(pXref);
		return;
	}

	CloseWriteAppend_FinalizeSource(m_pReader, m_wsPassword, m_wsDstFile, m_wsSrcFile);

	m_pReader   = NULL;
	m_pWriter   = NULL;
	m_nEditPage = -1;
}

int CPdfEditor::GetError()
{
	return m_nError;
}
void CPdfEditor::GetPageTree(XRef* xref, Object* pPagesRefObj, PdfWriter::CPageTree* pPageParent)
{
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	if (!pPagesRefObj || !xref || !pDoc)
		return;

	Object pagesObj;
	if (!pPagesRefObj->isRef() || !pPagesRefObj->fetch(xref, &pagesObj)->isDict("Pages"))
	{
		pagesObj.free();
		return;
	}

	Ref topPagesRef = pPagesRefObj->getRef();
	PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, topPagesRef.num);
	if (!pXref)
	{
		pagesObj.free();
		return;
	}

	PdfWriter::CPageTree* pPageT = new PdfWriter::CPageTree();
	if (!pPageT)
	{
		pagesObj.free();
		RELEASEOBJECT(pXref);
		return;
	}
	pXref->Add(pPageT, topPagesRef.gen);
	SDictConvertContext ctx;
	for (int nIndex = 0; nIndex < pagesObj.dictGetLength(); ++nIndex)
	{
		Object oTemp;
		char* chKey = pagesObj.dictGetKey(nIndex);
		if (strcmp("Resources", chKey) == 0)
		{
			if (pagesObj.dictGetVal(nIndex, &oTemp)->isDict())
			{
				PdfWriter::CResourcesDict* pDict = new PdfWriter::CResourcesDict(NULL, true, false);
				pPageT->Add("Resources", pDict);
				for (int nIndex = 0; nIndex < oTemp.dictGetLength(); ++nIndex)
				{
					Object oRes;
					char* chKey2 = oTemp.dictGetKey(nIndex);
					if (strcmp("Font", chKey2) == 0 || strcmp("ExtGState", chKey2) == 0 || strcmp("XObject", chKey2) == 0 || strcmp("Shading", chKey2) == 0 || strcmp("Pattern", chKey2) == 0)
						oTemp.dictGetVal(nIndex, &oRes);
					else
						oTemp.dictGetValNF(nIndex, &oRes);
					DictToCDictObjectUnified(&oRes, ctx, pDict, chKey2);
					oRes.free();
				}

				oTemp.free();
				continue;
			}
			else
			{
				oTemp.free();
				pagesObj.dictGetValNF(nIndex, &oTemp);
			}
		}
		else if (strcmp("Parent", chKey) == 0 && pPageParent)
		{
			pPageT->Add("Parent", pPageParent);
			continue;
		}
		else
			pagesObj.dictGetValNF(nIndex, &oTemp);
		DictToCDictObjectUnified(&oTemp, ctx, pPageT, chKey);
		oTemp.free();
	}
	pDoc->CreatePageTree(pXref, pPageT);
	pPageT->Fix();

	Object kidsArrObj;
	if (!pagesObj.dictLookup("Kids", &kidsArrObj)->isArray())
	{
		pagesObj.free();
		kidsArrObj.free();
		return;
	}
	pagesObj.free();

	for (int i = 0, count = kidsArrObj.arrayGetLength(); i < count; ++i)
	{
		Object kidRefObj;
		if (kidsArrObj.arrayGetNF(i, &kidRefObj))
			GetPageTree(xref, &kidRefObj, pPageT);
		kidRefObj.free();
	}
	kidsArrObj.free();
}
bool CPdfEditor::EditPage(int _nPageIndex, bool bSet, bool bActualPos)
{
	if (m_nMode == Mode::Unknown && !IncrementalUpdates())
		return false;

	WriteRedact({});
	m_arrRedact.clear();

	PDFDoc* pPDFDocument = NULL;
	PdfReader::CPdfFontList* pFontList = NULL;
	int nStartRefID = 0;
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	if (!pDoc)
		return false;

	PdfWriter::CPageTree* pPageTree = pDoc->GetPageTree();
	PdfWriter::CPage* pEditPage = NULL;
	pEditPage = bActualPos ? pDoc->GetPage(_nPageIndex) : pDoc->GetEditPage(_nPageIndex);
	if (m_nMode == Mode::Split && !pEditPage)
		return false;
	if (pEditPage)
	{
		if (bSet)
		{
			pDoc->SetCurPage(pEditPage);
			m_pWriter->EditPage(pEditPage);
			m_nEditPage = _nPageIndex;
		}
		return true;
	}

	int nOriginIndex = _nPageIndex;
	if (m_nMode == Mode::WriteNew)
	{
		PdfWriter::CObjectBase* pObj = pPageTree->GetObj(_nPageIndex);
		PdfWriter::CFakePage* pFakePage = NULL;
		if (pObj)
			pFakePage = dynamic_cast<PdfWriter::CFakePage*>(pObj);
		if (pFakePage)
			nOriginIndex = pFakePage->GetOriginIndex();
	}
	int nPageIndex = m_pReader->GetPageIndex(nOriginIndex, &pPDFDocument, &pFontList, &nStartRefID);
	if (nPageIndex < 0 || !pPDFDocument)
		return NULL;

	if (m_nMode == Mode::WriteNew)
	{
		PdfWriter::CPage* pPage = new PdfWriter::CPage(pDoc);
		pDoc->AddObject(pPage);
		pPageTree->ReplacePage(_nPageIndex, pPage);
		pDoc->AddEditPage(pPage, _nPageIndex);

		// Get page object
		Catalog* pCatalog = pPDFDocument->getCatalog();
		XRef* xref = pPDFDocument->getXRef();
		Ref* pPageRef = pCatalog->getPageRef(nPageIndex);
		Object pageRefObj, pageObj;
		pageRefObj.initRef(pPageRef->num, pPageRef->gen);
		if (!pageRefObj.fetch(xref, &pageObj)->isDict())
		{
			pageObj.free(); pageRefObj.free();
			return false;
		}
		m_mObjManager.AddObj(pPageRef->num + nStartRefID, pPage);
		pageRefObj.free();

		SDictConvertContext ctx(pDoc, xref, &m_mObjManager, nStartRefID);

		bool bResources = false, bMediaBox = false, bCropBox = false, bRotate = false;
		for (int nIndex = 0; nIndex < pageObj.dictGetLength(); ++nIndex)
		{
			Object oTemp;
			char* chKey = pageObj.dictGetKey(nIndex);
			if (strcmp("Resources", chKey) == 0)
			{
				bResources = true;
				Ref oResourcesRef = { -1, -1 };
				if (pageObj.dictGetValNF(nIndex, &oTemp)->isRef())
					oResourcesRef = oTemp.getRef();
				oTemp.free();

				PdfWriter::CObjectBase* pObj = oResourcesRef.num > 0 ? m_mObjManager.GetObj(oResourcesRef.num + nStartRefID) : NULL;
				if (pObj)
				{
					pPage->Add(chKey, pObj);
					m_mObjManager.IncRefCount(oResourcesRef.num + nStartRefID);
					continue;
				}

				if (pageObj.dictGetVal(nIndex, &oTemp)->isDict())
				{
					PdfWriter::CResourcesDict* pDict = pDoc->CreateResourcesDict(oResourcesRef.num < 0, false);
					if (oResourcesRef.num > 0)
						m_mObjManager.AddObj(oResourcesRef.num + nStartRefID, pDict);
					pPage->Add(chKey, pDict);
					for (int nIndex = 0; nIndex < oTemp.dictGetLength(); ++nIndex)
					{
						Object oRes;
						char* chKey2 = oTemp.dictGetKey(nIndex);
						oTemp.dictGetValNF(nIndex, &oRes);
						PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oRes, ctx);
						pDict->Add(chKey2, pBase);
						oRes.free();
					}

					oTemp.free();
					continue;
				}
				else
				{
					oTemp.free();
					pageObj.dictGetValNF(nIndex, &oTemp);
				}
			}
			else if (strcmp("Parent", chKey) == 0)
			{
				// Fields from Parent pages are transferred to the pages themselves
				oTemp.free();
				continue;
			}
			else if (strcmp("MediaBox", chKey) == 0)
			{
				bMediaBox = true;
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
			else if (strcmp("CropBox", chKey) == 0)
			{
				bCropBox = true;
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
			else if (strcmp("Rotate", chKey) == 0)
			{
				bRotate = true;
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
			else if (strcmp("Contents", chKey) == 0)
			{
				pageObj.dictGetValNF(nIndex, &oTemp);
				ctx.bUndecodedStream = false;
				PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oTemp, ctx);
				ctx.bUndecodedStream = true;
				pPage->Add(chKey, pBase);
				continue;
			}
			else
				pageObj.dictGetValNF(nIndex, &oTemp);
			PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oTemp, ctx);
			pPage->Add(chKey, pBase);
			oTemp.free();
		}
		if (!bResources || !bMediaBox || !bCropBox || !bRotate)
		{
			Page* pOPage = pCatalog->getPage(nPageIndex);
			if (!bMediaBox)
			{
				PDFRectangle* pRect = pOPage->getMediaBox();
				pPage->Add("MediaBox", PdfWriter::CArrayObject::CreateBox(pRect->x1, pRect->y1, pRect->x2, pRect->y2));
			}
			if (!bCropBox && pOPage->isCropped())
			{
				PDFRectangle* pRect = pOPage->getCropBox();
				pPage->Add("CropBox", PdfWriter::CArrayObject::CreateBox(pRect->x1, pRect->y1, pRect->x2, pRect->y2));
			}
			if (!bRotate)
				pPage->Add("Rotate", pOPage->getRotate());
			if (!bResources)
			{
				Dict* pResources = pOPage->getResourceDict();
				PdfWriter::CResourcesDict* pDict = pDoc->CreateResourcesDict(true, false);
				pPage->Add("Resources", pDict);
				for (int nIndex = 0; nIndex < pResources->getLength(); ++nIndex)
				{
					Object oRes;
					char* chKey2 = pResources->getKey(nIndex);
					pResources->getValNF(nIndex, &oRes);
					PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oRes, ctx);
					pDict->Add(chKey2, pBase);
					oRes.free();
				}
			}
		}
		pPage->Fix();
		pDoc->FixEditPage(pPage);

		if (m_nRedactInfo & (1 << 7))
		{
			int nEP = m_nEditPage;
			m_nEditPage = _nPageIndex;
			Redact(NULL);
			m_nEditPage = nEP;
		}

		if (bSet)
		{
			pDoc->SetCurPage(pPage);
			m_pWriter->EditPage(pPage);
			m_nEditPage = _nPageIndex;

			if (bCropBox)
			{
				Page* pOPage = pCatalog->getPage(nPageIndex);
				if (pOPage->isCropped())
				{
					PDFRectangle* pCropBox = pOPage->getCropBox();
					PdfWriter::CStream* pStream = pPage->GetStream();
					pStream->WriteStr("1 0 0 1 ");
					pStream->WriteReal(pCropBox->x1);
					pStream->WriteChar(' ');
					pStream->WriteReal(pCropBox->y2 - pOPage->getMediaBox()->y2);
					pStream->WriteStr(" cm\012");
				}
			}

			double dCTM[6] = { 1, 0, 0, 1, 0, 0 };
			double dInvCTM[6] = { 1, 0, 0, 1, 0, 0 };
			GetCTM(xref, &pageObj, dCTM);
			GetInvertCTM(dCTM, dInvCTM);
			pageObj.free();

			pPage->Transform(dInvCTM[0], dInvCTM[1], dInvCTM[2], dInvCTM[3], dInvCTM[4], dInvCTM[5]);
			pPage->SetStrokeColor(0, 0, 0);
			pPage->SetFillColor(0, 0, 0);
			pPage->SetExtGrState(pDoc->GetExtGState(255, 255));
			pPage->BeginText();
			pPage->SetCharSpace(0);
			pPage->SetTextRenderingMode(PdfWriter::textrenderingmode_Fill);
			pPage->SetHorizontalScaling(100);
			pPage->EndText();
		}
		return true;
	}

	XRef* xref = pPDFDocument->getXRef();
	Catalog* pCatalog = pPDFDocument->getCatalog();
	if (!xref || !pCatalog)
		return false;
	std::pair<int, int> pPageRef = std::make_pair(0, 0);
	if (bActualPos)
		pPageRef = pDoc->GetPageRef(_nPageIndex);
	else
	{
		Ref* pRef = pPDFDocument->getCatalog()->getPageRef(nPageIndex);
		if (pRef && pRef->num != 0)
			pPageRef = std::make_pair(pRef->num, pRef->gen);
	}
	if (pPageRef.first == 0)
		return false;

	// Get page object
	Object pageRefObj, pageObj;
	pageRefObj.initRef(pPageRef.first, pPageRef.second);
	if (!pageRefObj.fetch(xref, &pageObj) || !pageObj.isDict())
	{
		pageObj.free();
		pageRefObj.free();
		return false;
	}
	pageRefObj.free();

	// Reproduce page dictionary from reader for writer
	PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, pPageRef.first);
	if (!pXref)
	{
		pageObj.free();
		return false;
	}
	PdfWriter::CPage* pPage = new PdfWriter::CPage(pDoc);
	if (!pPage)
	{
		pageObj.free();
		RELEASEOBJECT(pXref);
		return false;
	}
	pXref->Add(pPage, pPageRef.second);
	bool bResources = false, bMediaBox = false, bCropBox = false, bRotate = false;
	SDictConvertContext ctx;
	for (int nIndex = 0; nIndex < pageObj.dictGetLength(); ++nIndex)
	{
		Object oTemp;
		char* chKey = pageObj.dictGetKey(nIndex);
		if (strcmp("Resources", chKey) == 0)
		{
			bResources = true;
			if (pageObj.dictGetVal(nIndex, &oTemp)->isDict())
			{
				PdfWriter::CResourcesDict* pDict = new PdfWriter::CResourcesDict(NULL, true, false);
				pPage->Add("Resources", pDict);
				for (int nIndex = 0; nIndex < oTemp.dictGetLength(); ++nIndex)
				{
					Object oRes;
					char* chKey2 = oTemp.dictGetKey(nIndex);
					if (strcmp("Font", chKey2) == 0 || strcmp("ExtGState", chKey2) == 0 || strcmp("XObject", chKey2) == 0 || strcmp("Shading", chKey2) == 0 || strcmp("Pattern", chKey2) == 0)
						oTemp.dictGetVal(nIndex, &oRes);
					else
						oTemp.dictGetValNF(nIndex, &oRes);
					DictToCDictObjectUnified(&oRes, ctx, pDict, chKey2);
					oRes.free();
				}

				oTemp.free();
				continue;
			}
			else
			{
				oTemp.free();
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
		}
		else if (strcmp("Annots", chKey) == 0)
		{
			if (pageObj.dictGetVal(nIndex, &oTemp)->isArray())
			{
				PdfWriter::CArrayObject* pArray = new PdfWriter::CArrayObject();
				pPage->Add("Annots", pArray);
				for (int nIndex = 0; nIndex < oTemp.arrayGetLength(); ++nIndex)
				{
					Object oAnnot;
					oTemp.arrayGetNF(nIndex, &oAnnot);
					DictToCDictObjectUnified(&oAnnot, ctx, pArray);
					oAnnot.free();
				}
				oTemp.free();
				continue;
			}
			else
			{
				oTemp.free();
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
		}
		else if (strcmp("Contents", chKey) == 0)
		{
			if (pageObj.dictGetVal(nIndex, &oTemp)->isArray())
			{
				DictToCDictObjectUnified(&oTemp, ctx, pPage, chKey);
				oTemp.free();
				continue;
			}
			else
			{
				oTemp.free();
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
		}
		else if (strcmp("Parent", chKey) == 0)
		{
			pageObj.dictGetValNF(nIndex, &oTemp);
		}
		else if (strcmp("MediaBox", chKey) == 0)
		{
			bMediaBox = true;
			pageObj.dictGetValNF(nIndex, &oTemp);
		}
		else if (strcmp("CropBox", chKey) == 0)
		{
			bCropBox = true;
			pageObj.dictGetValNF(nIndex, &oTemp);
		}
		else if (strcmp("Rotate", chKey) == 0)
		{
			bRotate = true;
			pageObj.dictGetValNF(nIndex, &oTemp);
		}
		else
			pageObj.dictGetValNF(nIndex, &oTemp);
		DictToCDictObjectUnified(&oTemp, ctx, pPage, chKey);
		oTemp.free();
	}
	if (!bResources || !bMediaBox || !bCropBox || !bRotate)
	{
		Page* pOPage = pCatalog->getPage(nPageIndex);
		if (!bMediaBox)
		{
			PDFRectangle* pRect = pOPage->getMediaBox();
			pPage->Add("MediaBox", PdfWriter::CArrayObject::CreateBox(pRect->x1, pRect->y1, pRect->x2, pRect->y2));
		}

		if (!bCropBox && pOPage->isCropped())
		{
			PDFRectangle* pRect = pOPage->getCropBox();
			pPage->Add("CropBox", PdfWriter::CArrayObject::CreateBox(pRect->x1, pRect->y1, pRect->x2, pRect->y2));
		}

		if (!bRotate)
			pPage->Add("Rotate", pOPage->getRotate());

		if (!bResources)
		{
			Dict* pResources = pOPage->getResourceDict();
			PdfWriter::CResourcesDict* pDict = pDoc->CreateResourcesDict(true, false);
			pPage->Add("Resources", pDict);

			SDictConvertContext ctx;
			ctx.bMakeBinary = false;
			for (int nIndex = 0; nIndex < pResources->getLength(); ++nIndex)
			{
				Object oRes;
				char* chKey2 = pResources->getKey(nIndex);
				if (strcmp("Font", chKey2) == 0 || strcmp("ExtGState", chKey2) == 0 || strcmp("XObject", chKey2) == 0 || strcmp("Shading", chKey2) == 0 || strcmp("Pattern", chKey2) == 0)
					pResources->getVal(nIndex, &oRes);
				else
					pResources->getValNF(nIndex, &oRes);
				DictToCDictObjectUnified(&oRes, ctx, pDict, chKey2);
			}
		}
	}
	pPage->Fix();

	// Apply page editing for writer
	if (pDoc->EditPage(pXref, pPage, _nPageIndex))
	{
		if (bSet)
		{
			m_pWriter->EditPage(pPage);
			m_nEditPage = _nPageIndex;
		}
		if (bCropBox)
		{
			Page* pOPage = pCatalog->getPage(nPageIndex);
			if (pOPage->isCropped())
			{
				PDFRectangle* pCropBox = pOPage->getCropBox();
				PdfWriter::CStream* pStream = pPage->GetStream();
				pStream->WriteStr("1 0 0 1 ");
				pStream->WriteReal(pCropBox->x1);
				pStream->WriteChar(' ');
				pStream->WriteReal(pCropBox->y2 - pOPage->getMediaBox()->y2);
				pStream->WriteStr(" cm\012");
			}
		}
		double dCTM[6] = { 1, 0, 0, 1, 0, 0 };
		double dInvCTM[6] = { 1, 0, 0, 1, 0, 0 };
		GetCTM(xref, &pageObj, dCTM);
		GetInvertCTM(dCTM, dInvCTM);
		pageObj.free();

		pPage->Transform(dInvCTM[0], dInvCTM[1], dInvCTM[2], dInvCTM[3], dInvCTM[4], dInvCTM[5]);
		pPage->SetStrokeColor(0, 0, 0);
		pPage->SetFillColor(0, 0, 0);
		pPage->SetExtGrState(pDoc->GetExtGState(255, 255));
		pPage->BeginText();
		pPage->SetCharSpace(0);
		pPage->SetTextRenderingMode(PdfWriter::textrenderingmode_Fill);
		pPage->SetHorizontalScaling(100);
		pPage->EndText();
		return true;
	}

	pageObj.free();
	RELEASEOBJECT(pXref);
	return false;
}
bool CPdfEditor::SplitPages(const int* arrPageIndex, unsigned int unLength, PDFDoc* _pDoc, int nStartRefID)
{
	if (m_nMode == Mode::Unknown)
		return false;
	PDFDoc* pPDFDocument = _pDoc;
	XRef* xref = pPDFDocument->getXRef();
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	PdfWriter::CPageTree* pPageTree = pDoc->GetPageTree();
	int nPagesBefore = m_pReader->GetNumPagesBefore(pPDFDocument);

	if (unLength == 0)
		unLength = pPDFDocument->getNumPages();

	SDictConvertContext ctx(pDoc, xref, &m_mObjManager, nStartRefID);
	// Pages must be created in advance to reference them
	Catalog* pCatalog = pPDFDocument->getCatalog();
	for (unsigned int i = 0; i < unLength; ++i)
	{
		Ref* pPageRef = pCatalog->getPageRef((arrPageIndex ? arrPageIndex[i] : i) + 1);
		if (pPageRef->num == 0)
			return false;

		PdfWriter::CPage* pPage = new PdfWriter::CPage(pDoc);
		pDoc->AddObject(pPage);
		if (m_nMode == Mode::WriteAppend || m_nMode == Mode::WriteNew)
			pDoc->AddPage(pDoc->GetPagesCount(), pPage);
		else
			pPageTree->ReplacePage(nPagesBefore + (arrPageIndex ? arrPageIndex[i] : i), pPage);
		pDoc->AddEditPage(pPage, nPagesBefore + (arrPageIndex ? arrPageIndex[i] : i));

		// Get page object
		Object pageRefObj, pageObj;
		pageRefObj.initRef(pPageRef->num, pPageRef->gen);
		if (!pageRefObj.fetch(xref, &pageObj)->isDict())
		{
			pageObj.free(); pageRefObj.free();
			return false;
		}
		m_mObjManager.AddObj(pPageRef->num + nStartRefID, pPage);
		pageObj.free(); pageRefObj.free();
	}

	for (unsigned int i = 0; i < unLength; ++i)
	{
		Ref* pPageRef = pCatalog->getPageRef((arrPageIndex ? arrPageIndex[i] : i) + 1);
		if (pPageRef->num == 0)
			return false;

		// Get page object
		PdfWriter::CPage* pPage = (PdfWriter::CPage*)m_mObjManager.GetObj(pPageRef->num + nStartRefID);
		Object pageRefObj, pageObj;
		pageRefObj.initRef(pPageRef->num, pPageRef->gen);
		if (!pageRefObj.fetch(xref, &pageObj)->isDict())
		{
			pageObj.free();
			pageRefObj.free();
			return false;
		}
		pageRefObj.free();

		bool bResources = false, bMediaBox = false, bCropBox = false, bRotate = false;
		// Copy page with all resources from reader for writer
		for (int nIndex = 0; nIndex < pageObj.dictGetLength(); ++nIndex)
		{
			Object oTemp;
			char* chKey = pageObj.dictGetKey(nIndex);
			if (strcmp("Resources", chKey) == 0)
			{
				bResources = true;
				Ref oResourcesRef = { -1, -1 };
				if (pageObj.dictGetValNF(nIndex, &oTemp)->isRef())
					oResourcesRef = oTemp.getRef();
				oTemp.free();

				PdfWriter::CObjectBase* pObj = oResourcesRef.num > 0 ? m_mObjManager.GetObj(oResourcesRef.num + nStartRefID) : NULL;
				if (pObj)
				{
					pPage->Add(chKey, pObj);
					m_mObjManager.IncRefCount(oResourcesRef.num + nStartRefID);
					continue;
				}

				if (pageObj.dictGetVal(nIndex, &oTemp)->isDict())
				{
					PdfWriter::CResourcesDict* pDict = pDoc->CreateResourcesDict(oResourcesRef.num < 0, false);
					if (oResourcesRef.num > 0)
						m_mObjManager.AddObj(oResourcesRef.num + nStartRefID, pDict);
					pPage->Add(chKey, pDict);
					for (int nIndex2 = 0; nIndex2 < oTemp.dictGetLength(); ++nIndex2)
					{
						Object oRes;
						char* chKey2 = oTemp.dictGetKey(nIndex2);
						oTemp.dictGetValNF(nIndex2, &oRes);
						PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oRes, ctx);
						pDict->Add(chKey2, pBase);
						oRes.free();
					}

					oTemp.free();
					continue;
				}
				else
				{
					oTemp.free();
					pageObj.dictGetValNF(nIndex, &oTemp);
				}
			}
			else if (strcmp("Parent", chKey) == 0)
			{
				oTemp.free();
				continue;
			}
			else if (strcmp("MediaBox", chKey) == 0)
			{
				bMediaBox = true;
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
			else if (strcmp("CropBox", chKey) == 0)
			{
				bCropBox = true;
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
			else if (strcmp("Rotate", chKey) == 0)
			{
				bRotate = true;
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
			else
				pageObj.dictGetValNF(nIndex, &oTemp);
			PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oTemp, ctx);
			pPage->Add(chKey, pBase);
			if (strcmp("Contents", chKey) == 0)
			{
				if (pBase->GetType() == PdfWriter::object_type_ARRAY)
				{
					PdfWriter::CArrayObject* pArr = (PdfWriter::CArrayObject*)pBase;
					for (int j = 0; j < pArr->GetCount(); ++j)
					{
						pBase = pArr->Get(j);
						if (pBase->GetType() == PdfWriter::object_type_DICT)
						{
							PdfWriter::CDictObject* pDict = (PdfWriter::CDictObject*)pBase;
							if (pDict->Get("Filter"))
								pDict->SetFilter(STREAM_FILTER_ALREADY_DECODE);
						}
					}
				}
				else if (pBase->GetType() == PdfWriter::object_type_DICT)
				{
					PdfWriter::CDictObject* pDict = (PdfWriter::CDictObject*)pBase;
					if (pDict->Get("Filter"))
						pDict->SetFilter(STREAM_FILTER_ALREADY_DECODE);
				}
			}
			oTemp.free();
		}
		if (!bResources || !bMediaBox || !bCropBox || !bRotate)
		{
			Page* pOPage = pCatalog->getPage((arrPageIndex ? arrPageIndex[i] : i) + 1);
			if (!bMediaBox)
			{
				PDFRectangle* pRect = pOPage->getMediaBox();
				pPage->Add("MediaBox", PdfWriter::CArrayObject::CreateBox(pRect->x1, pRect->y1, pRect->x2, pRect->y2));
			}
			if (!bCropBox && pOPage->isCropped())
			{
				PDFRectangle* pRect = pOPage->getCropBox();
				pPage->Add("CropBox", PdfWriter::CArrayObject::CreateBox(pRect->x1, pRect->y1, pRect->x2, pRect->y2));
			}
			if (!bRotate)
				pPage->Add("Rotate", pOPage->getRotate());
			if (!bResources)
			{
				Dict* pResources = pOPage->getResourceDict();
				PdfWriter::CResourcesDict* pDict = pDoc->CreateResourcesDict(true, false);
				pPage->Add("Resources", pDict);
				for (int nIndex = 0; nIndex < pResources->getLength(); ++nIndex)
				{
					Object oRes;
					char* chKey2 = pResources->getKey(nIndex);
					pResources->getValNF(nIndex, &oRes);
					PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oRes, ctx);
					pDict->Add(chKey2, pBase);
					oRes.free();
				}
			}
		}
		pPage->Fix();
		if (m_nMode == Mode::WriteAppend || m_nMode == Mode::WriteNew)
		{
			pDoc->FixEditPage(pPage);

			double dCTM[6] = { 1, 0, 0, 1, 0, 0 };
			double dInvCTM[6] = { 1, 0, 0, 1, 0, 0 };
			GetCTM(xref, &pageObj, dCTM);
			GetInvertCTM(dCTM, dInvCTM);

			pPage->Transform(dInvCTM[0], dInvCTM[1], dInvCTM[2], dInvCTM[3], dInvCTM[4], dInvCTM[5]);
			pPage->SetStrokeColor(0, 0, 0);
			pPage->SetFillColor(0, 0, 0);
			pPage->SetExtGrState(pDoc->GetExtGState(255, 255));
			pPage->BeginText();
			pPage->SetCharSpace(0);
			pPage->SetTextRenderingMode(PdfWriter::textrenderingmode_Fill);
			pPage->SetHorizontalScaling(100);
			pPage->EndText();
		}
		else
			m_pWriter->SetNeedAddHelvetica(false); // TODO adds font for proper Adobe pdf editing without text. Remove when implementing font map
		pageObj.free();
	}

	Object oCatalog;
	if (!xref->getCatalog(&oCatalog)->isDict())
	{
		oCatalog.free();
		return false;
	}

	Object oAcroForm;
	if (oCatalog.dictLookupNF("AcroForm", &oAcroForm)->isRef() || oAcroForm.isDict())
	{
		PdfWriter::CDictObject* pAcroForm = pDoc->GetAcroForm();
		if (!pAcroForm)
		{
			pAcroForm = new PdfWriter::CDictObject();
			if (oAcroForm.isRef())
				pDoc->AddObject(pAcroForm);
			pDoc->SetAcroForm(pAcroForm);
		}
		else
			pAcroForm->Remove("NeedAppearances");

		if (oAcroForm.isRef())
		{
			oAcroForm.free();
			if (!oCatalog.dictLookup("AcroForm", &oAcroForm)->isDict())
			{
				oAcroForm.free(); oCatalog.free();
				return false;
			}
		}

		for (int nIndex = 0; nIndex < oAcroForm.dictGetLength(); ++nIndex)
		{
			Object oTemp;
			char* chKey = oAcroForm.dictGetKey(nIndex);
			if (strcmp("Fields", chKey) == 0)
			{
				Ref oFieldsRef = { -1, -1 };
				if (oAcroForm.dictGetValNF(nIndex, &oTemp)->isRef())
					oFieldsRef = oTemp.getRef();
				oTemp.free();

				PdfWriter::CArrayObject* pFields = dynamic_cast<PdfWriter::CArrayObject*>(pAcroForm->Get("Fields"));
				if (!pFields)
				{
					PdfWriter::CObjectBase* pObj = oFieldsRef.num > 0 ? m_mObjManager.GetObj(oFieldsRef.num + nStartRefID) : NULL;
					if (pObj)
					{
						pAcroForm->Add(chKey, pObj);
						m_mObjManager.IncRefCount(oFieldsRef.num + nStartRefID);
						continue;
					}
				}

				if (oAcroForm.dictGetVal(nIndex, &oTemp)->isArray())
				{
					if (!pFields)
					{
						pFields = new PdfWriter::CArrayObject();
						if (oFieldsRef.num > 0)
						{
							pDoc->AddObject(pFields);
							m_mObjManager.AddObj(oFieldsRef.num + nStartRefID, pFields);
						}
						pAcroForm->Add(chKey, pFields);
					}

					for (int nIndex = 0; nIndex < oTemp.arrayGetLength(); ++nIndex)
					{
						Object oRes;
						PdfWriter::CObjectBase* pObj = NULL;
						if (oTemp.arrayGetNF(nIndex, &oRes)->isRef())
							pObj = m_mObjManager.GetObj(oRes.getRefNum() + nStartRefID);
						if (pObj)
						{
							pFields->Add(pObj);
							m_mObjManager.IncRefCount(oRes.getRefNum() + nStartRefID);
							AddWidgetParent(pDoc, &m_mObjManager, pObj);
							oRes.free();
							continue;
						}
						oRes.free();
					}
					oTemp.free();
					continue;
				}
				else if (!pFields)
				{
					oTemp.free();
					oAcroForm.dictGetValNF(nIndex, &oTemp);
				}
				else
				{
					oTemp.free();
					continue;
				}
			}
			else if (strcmp("SigFlags", chKey) == 0 || strcmp("XFA", chKey) == 0 || (strcmp("DA", chKey) == 0 && pAcroForm->Get("DA")) || strcmp("NeedAppearances", chKey) == 0)
			{ // Cannot guarantee their execution
				oTemp.free();
				continue;
			}
			else if (strcmp("DR", chKey) == 0)
			{ // Add only unique keys
				PdfWriter::CDictObject* pDR = dynamic_cast<PdfWriter::CDictObject*>(pAcroForm->Get("DR"));
				if (!pDR)
				{
					pDR = new PdfWriter::CDictObject();
					pDoc->AddObject(pDR);
					pAcroForm->Add(chKey, pDR);
				}

				PdfWriter::CArrayObject* pProcset = new PdfWriter::CArrayObject();
				pDR->Add("ProcSet", pProcset);
				pProcset->Add(new PdfWriter::CNameObject("PDF"));
				pProcset->Add(new PdfWriter::CNameObject("Text"));
				pProcset->Add(new PdfWriter::CNameObject("ImageB"));
				pProcset->Add(new PdfWriter::CNameObject("ImageC"));
				pProcset->Add(new PdfWriter::CNameObject("ImageI"));

				if (oAcroForm.dictGetVal(nIndex, &oTemp)->isDict())
				{
					Object oTemp2;
					for (int nIndex2 = 0; nIndex2 < oTemp.dictGetLength(); ++nIndex2)
					{
						char* chKey2 = oTemp.dictGetKey(nIndex2);
						if (strcmp("ProcSet", chKey2) == 0 || !oTemp.dictGetVal(nIndex2, &oTemp2)->isDict())
						{
							oTemp2.free();
							continue;
						}
						PdfWriter::CDictObject* pDict = dynamic_cast<PdfWriter::CDictObject*>(pDR->Get(chKey2));
						if (!pDict)
						{
							Object oTempRef;
							if (oTemp.dictGetValNF(nIndex2, &oTempRef)->isRef())
							{
								PdfWriter::CObjectBase* pObj = m_mObjManager.GetObj(oTempRef.getRefNum() + nStartRefID);
								if (pObj)
								{
									pDR->Add(chKey2, pObj);
									m_mObjManager.IncRefCount(oTempRef.getRefNum() + nStartRefID);
									oTemp2.free(); oTempRef.free();
									continue;
								}
							}
							PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oTemp2, ctx);
							if (oTempRef.isRef())
								pDoc->AddObject(pBase);
							pDR->Add(chKey2, pBase);
							oTemp2.free(); oTempRef.free();
							continue;
						}
						else
						{
							for (int nIndex3 = 0; nIndex3 < oTemp2.dictGetLength(); ++nIndex3)
							{
								char* chKey3 = oTemp2.dictGetKey(nIndex3);
								if (pDict->Get(chKey3))
									continue;
								Object oTempRef;
								if (oTemp2.dictGetValNF(nIndex3, &oTempRef)->isRef())
								{
									PdfWriter::CObjectBase* pObj = m_mObjManager.GetObj(oTempRef.getRefNum() + nStartRefID);
									if (pObj)
									{
										pDict->Add(chKey3, pObj);
										m_mObjManager.IncRefCount(oTempRef.getRefNum() + nStartRefID);
										oTemp2.free(); oTempRef.free();
										continue;
									}
								}
								PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oTemp2, ctx);
								if (oTempRef.isRef())
									pDoc->AddObject(pBase);
								pDict->Add(chKey3, pBase);
								oTemp2.free(); oTempRef.free();
								continue;
							}
						}
					}
					oTemp2.free();

					Dict* pResources = oTemp.getDict();
					ScanFonts(pPDFDocument, pResources, m_pWriter, &m_mObjManager, m_pReader);
					oTemp.free();
					continue;
				}
				else
				{
					oTemp.free();
					oAcroForm.dictGetValNF(nIndex, &oTemp);
				}
			}
			else
				oAcroForm.dictGetValNF(nIndex, &oTemp);
			PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oTemp, ctx);
			pAcroForm->Add(chKey, pBase);
			oTemp.free();
		}
	}
	oAcroForm.free(); oCatalog.free();

	// Renaming fields
	std::string sPrefix = m_pReader->GetPrefixForm(pPDFDocument);
	if (!sPrefix.empty())
	{
		sPrefix = "_" + sPrefix;
		std::vector<int> arrRename; // Vector of renamed fields
		std::map<int, PdfWriter::CAnnotation*> mAnnots = m_pWriter->GetDocument()->GetAnnots();
		for (auto it = mAnnots.begin(); it != mAnnots.end(); it++)
		{
			PdfWriter::CAnnotation* pAnnot = it->second;
			if (pAnnot->GetAnnotationType() != PdfWriter::AnnotWidget || it->first < nStartRefID)
				continue;

			std::vector<int>::iterator it2 = std::find(arrRename.begin(), arrRename.end(), it->first);
			if (it2 != arrRename.end())
				continue;

			PdfWriter::CObjectBase* pObjBase = pAnnot->Get("Parent");
			if (!pObjBase || !ChangeFullNameParent(m_mObjManager.FindObj(pObjBase), sPrefix, arrRename))
			{
				pObjBase = pAnnot->Get("T");
				if (pObjBase && pObjBase->GetType() == PdfWriter::object_type_STRING)
				{
					PdfWriter::CStringObject* pStr = (PdfWriter::CStringObject*)pObjBase;
					pStr->Add(sPrefix.c_str());
				}
				else if (pObjBase && pObjBase->GetType() == PdfWriter::object_type_BINARY)
				{
					PdfWriter::CBinaryObject* pBin = (PdfWriter::CBinaryObject*)pObjBase;
					pBin->Add((BYTE*)sPrefix.c_str(), sPrefix.length());
				}
			}
			arrRename.push_back(it->first);
		}
	}

	return true;
}
bool CPdfEditor::SplitPages(const int* arrPageIndex, unsigned int unLength)
{
	if (m_nMode != Mode::Split)
		return true;
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	if (!pDoc)
		return false;
	PdfWriter::CPageTree* pPageTree = pDoc->GetPageTree();
	if (!pPageTree)
		return false;

	m_mObjManager.SetDoc(pDoc);
	int nPages = m_pReader->GetNumPages();
	pPageTree->CreateFakePages(nPages);

	int nTotalPages = 0;
	int nPDFIndex = 0;
	std::map<int, std::vector<int>> mFileToPages;
	PDFDoc* pPDFDocument = m_pReader->GetPDFDocument(nPDFIndex);
	nPages = pPDFDocument->getNumPages();

	if (!arrPageIndex || unLength == 0)
	{
		unLength = m_pReader->GetNumPages();
		arrPageIndex = NULL;
	}

	for (unsigned int i = 0; i < unLength; ++i)
	{
		int nPageIndex = arrPageIndex ? arrPageIndex[i] : i;
		if (nPageIndex < nTotalPages + nPages)
			mFileToPages[nPDFIndex].push_back(nPageIndex - nTotalPages);
		else
		{
			pPDFDocument = m_pReader->GetPDFDocument(++nPDFIndex);
			if (!pPDFDocument)
			{
				m_mObjManager.m_arrSplitAddPages.push_back(nPageIndex);
				--nPDFIndex;
				continue;
			}
			nTotalPages += nPages;
			nPages = pPDFDocument->getNumPages();
			--i;
		}
	}

	for (const std::pair<const int, std::vector<int>>& it : mFileToPages)
	{
		pPDFDocument = m_pReader->GetPDFDocument(it.first);
		if (!SplitPages(it.second.data(), it.second.size(), pPDFDocument, m_pReader->GetStartRefID(pPDFDocument)))
			return false;
	}
	return true;
}
void CPdfEditor::AfterSplitPages()
{
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	if (!pDoc)
		return;
	PdfWriter::CPageTree* pPageTree = pDoc->GetPageTree();
	if (!pPageTree)
		return;
	pPageTree->ClearFakePages();
}
bool CPdfEditor::ChangeFullNameParent(int nParent, const std::string& sPrefix, std::vector<int>& arrRename)
{
	std::vector<int>::const_iterator it2 = std::find(arrRename.begin(), arrRename.end(), nParent);
	if (it2 != arrRename.end())
		return true;

	PdfWriter::CObjectBase* pObjBase = m_mObjManager.GetObj(nParent);
	if (!pObjBase || pObjBase->GetType() != PdfWriter::object_type_DICT)
		return false;

	PdfWriter::CDictObject* pDict = (PdfWriter::CDictObject*)pObjBase;
	pObjBase = pDict->Get("Parent");
	if (!pObjBase || !ChangeFullNameParent(m_mObjManager.FindObj(pObjBase), sPrefix, arrRename))
	{
		pObjBase = pDict->Get("T");
		if (pObjBase && pObjBase->GetType() == PdfWriter::object_type_STRING)
		{
			PdfWriter::CStringObject* pStr = (PdfWriter::CStringObject*)pObjBase;
			pStr->Add(sPrefix.c_str());
		}
		else if (pObjBase && pObjBase->GetType() == PdfWriter::object_type_BINARY)
		{
			PdfWriter::CBinaryObject* pBin = (PdfWriter::CBinaryObject*)pObjBase;
			pBin->Add((BYTE*)sPrefix.c_str(), sPrefix.length());
		}
	}
	arrRename.push_back(nParent);

	return true;
}
bool CPdfEditor::MergePages(const std::string& sOutlineTitle)
{
	if (m_nMode == Mode::Unknown && !IncrementalUpdates())
		return false;
	m_nOriginIndex = m_pReader->GetNumPages();
	PDFDoc* pDocument = m_pReader->GetLastPDFDocument();
	int nStartRefID = m_pReader->GetStartRefID(pDocument);
	m_mObjManager.SetDoc(m_pWriter->GetDocument());
	bool bRes = SplitPages(NULL, 0, pDocument, nStartRefID);
	if (!bRes)
		return false;

	Outline* pOutlineAdd = pDocument->getOutline();
	GList* pListAdd = NULL;
	if (pOutlineAdd)
		pListAdd = pOutlineAdd->getItems();
	if (!pListAdd)
		return bRes;

	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	PDFDoc* pDocumentFirst = m_pReader->GetPDFDocument(0);
	Outline* pOutlineOld = pDocumentFirst->getOutline();
	GList* pListOld = NULL;
	if (pOutlineOld)
		pListOld = pOutlineOld->getItems();
	if (!pDoc->GetOutlines() && pListOld)
	{
		for (int i = 0, num = pListOld->getLength(); i < num; i++)
		{
			OutlineItem* pOutlineItem = (OutlineItem*)pListOld->get(i);
			if (pOutlineItem)
				CreateOutlines(pDocumentFirst, pDoc, pOutlineItem, NULL);
		}
	}

	PdfWriter::COutline* pOutline = pDoc->CreateOutline(NULL, sOutlineTitle.c_str());
	for (int i = 0, num = pListAdd->getLength(); i < num; i++)
	{
		OutlineItem* pOutlineItem = (OutlineItem*)pListAdd->get(i);
		if (pOutlineItem)
			CreateOutlines(pDocumentFirst, pDoc, pOutlineItem, pOutline);
	}

	return bRes;
}
bool CPdfEditor::PrintPages(const std::vector<bool>& arrPages, int nFlag)
{
	if (m_nMode != Mode::Unknown)
		return false;

	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	PdfWriter::CPageTree* pPageTree = pDoc->GetPageTree();
	m_mObjManager.SetDoc(pDoc);

	for (int i = 0; i < arrPages.size(); ++i)
	{
		if (!arrPages[i])
			continue;

		PdfWriter::CPage* pPage = new PdfWriter::CPage(pDoc);
		pDoc->AddObject(pPage);
		pPageTree->AddPage(pPage);
		pDoc->AddEditPage(pPage, i);

		PDFDoc* pPDFDocument = NULL;
		PdfReader::CPdfFontList* pFontList = NULL;
		int nStartRefID = 0;
		int nPageIndex = m_pReader->GetPageIndex(i, &pPDFDocument, &pFontList, &nStartRefID);

		Catalog* pCatalog = pPDFDocument->getCatalog();
		XRef* xref = pPDFDocument->getXRef();
		Ref* pPageRef = pCatalog->getPageRef(nPageIndex);
		Object pageRefObj, pageObj;
		pageRefObj.initRef(pPageRef->num, pPageRef->gen);
		if (!pageRefObj.fetch(xref, &pageObj)->isDict())
		{
			pageObj.free(); pageRefObj.free();
			continue;
		}
		m_mObjManager.AddObj(pPageRef->num + nStartRefID, pPage);
		pageRefObj.free();

		SDictConvertContext ctx(pDoc, xref, &m_mObjManager, nStartRefID);

		bool bResources = false, bMediaBox = false, bCropBox = false, bRotate = false;
		for (int nIndex = 0; nIndex < pageObj.dictGetLength(); ++nIndex)
		{
			Object oTemp;
			char* chKey = pageObj.dictGetKey(nIndex);
			if (strcmp("Resources", chKey) == 0)
			{
				bResources = true;
				Ref oResourcesRef = { -1, -1 };
				if (pageObj.dictGetValNF(nIndex, &oTemp)->isRef())
					oResourcesRef = oTemp.getRef();
				oTemp.free();

				PdfWriter::CObjectBase* pObj = oResourcesRef.num > 0 ? m_mObjManager.GetObj(oResourcesRef.num + nStartRefID) : NULL;
				if (pObj)
				{
					pPage->Add(chKey, pObj);
					m_mObjManager.IncRefCount(oResourcesRef.num + nStartRefID);
					continue;
				}

				if (pageObj.dictGetVal(nIndex, &oTemp)->isDict())
				{
					PdfWriter::CResourcesDict* pDict = pDoc->CreateResourcesDict(oResourcesRef.num < 0, false);
					if (oResourcesRef.num > 0)
						m_mObjManager.AddObj(oResourcesRef.num + nStartRefID, pDict);
					pPage->Add(chKey, pDict);
					for (int nIndex = 0; nIndex < oTemp.dictGetLength(); ++nIndex)
					{
						Object oRes;
						char* chKey2 = oTemp.dictGetKey(nIndex);
						oTemp.dictGetValNF(nIndex, &oRes);
						PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oRes, ctx);
						pDict->Add(chKey2, pBase);
						oRes.free();
					}

					oTemp.free();
					continue;
				}
				else
				{
					oTemp.free();
					pageObj.dictGetValNF(nIndex, &oTemp);
				}
			}
			else if (strcmp("Parent", chKey) == 0)
			{
				// Fields from Parent pages are transferred to the pages themselves
				oTemp.free();
				continue;
			}
			else if (strcmp("MediaBox", chKey) == 0)
			{
				bMediaBox = true;
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
			else if (strcmp("CropBox", chKey) == 0)
			{
				bCropBox = true;
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
			else if (strcmp("Rotate", chKey) == 0)
			{
				bRotate = true;
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
			else if (strcmp("Contents", chKey) == 0)
			{
				if (nFlag != 3)
				{
					pageObj.dictGetValNF(nIndex, &oTemp);
					ctx.bUndecodedStream = false;
					PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oTemp, ctx);
					ctx.bUndecodedStream = true;
					pPage->Add(chKey, pBase);
				}
				continue;
			}
			else if (strcmp("Annots", chKey) == 0)
			{
				oTemp.free();
				continue;
			}
			else
				pageObj.dictGetValNF(nIndex, &oTemp);
			PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oTemp, ctx);
			pPage->Add(chKey, pBase);
			oTemp.free();
		}
		if (!bResources || !bMediaBox || !bCropBox || !bRotate)
		{
			Page* pOPage = pCatalog->getPage(nPageIndex);
			if (!bMediaBox)
			{
				PDFRectangle* pRect = pOPage->getMediaBox();
				pPage->Add("MediaBox", PdfWriter::CArrayObject::CreateBox(pRect->x1, pRect->y1, pRect->x2, pRect->y2));
			}
			if (!bCropBox && pOPage->isCropped())
			{
				PDFRectangle* pRect = pOPage->getCropBox();
				pPage->Add("CropBox", PdfWriter::CArrayObject::CreateBox(pRect->x1, pRect->y1, pRect->x2, pRect->y2));
			}
			if (!bRotate)
				pPage->Add("Rotate", pOPage->getRotate());
			if (!bResources)
			{
				Dict* pResources = pOPage->getResourceDict();
				PdfWriter::CResourcesDict* pDict = pDoc->CreateResourcesDict(true, false);
				pPage->Add("Resources", pDict);
				for (int nIndex = 0; nIndex < pResources->getLength(); ++nIndex)
				{
					Object oRes;
					char* chKey2 = pResources->getKey(nIndex);
					pResources->getValNF(nIndex, &oRes);
					PdfWriter::CObjectBase* pBase = DictToCDictObjectUnified(&oRes, ctx);
					pDict->Add(chKey2, pBase);
					oRes.free();
				}
			}
		}
		pPage->Fix();
		pDoc->FixEditPage(pPage);

		double dCTM[6] = { 1, 0, 0, 1, 0, 0 };
		double dInvCTM[6] = { 1, 0, 0, 1, 0, 0 };
		GetCTM(xref, &pageObj, dCTM);
		GetInvertCTM(dCTM, dInvCTM);

		pPage->Transform(dInvCTM[0], dInvCTM[1], dInvCTM[2], dInvCTM[3], dInvCTM[4], dInvCTM[5]);
		pPage->SetStrokeColor(0, 0, 0);
		pPage->SetFillColor(0, 0, 0);
		pPage->SetExtGrState(pDoc->GetExtGState(255, 255));
		pPage->BeginText();
		pPage->SetCharSpace(0);
		pPage->SetTextRenderingMode(PdfWriter::textrenderingmode_Fill);
		pPage->SetHorizontalScaling(100);
		pPage->EndText();

		Object oAnnots;
		if (!pageObj.dictLookup("Annots", &oAnnots)->isArray())
		{
			pageObj.free();
			continue;
		}
		for (int i = 0; i < oAnnots.arrayGetLength(); ++i)
		{
			Object oAnnot, oType, oObj;
			if (!oAnnots.arrayGet(i, &oAnnot)->isDict() || !oAnnot.dictLookup("Subtype", &oType)->isName() || !oAnnot.dictLookup("F", &oObj)->isInt())
			{
				oAnnot.free(); oType.free(); oObj.free();
				continue;
			}

			int nFlags = oObj.getInt();
			oObj.free();
			// if Invisible || Hidden || !Print
			if ((nFlags & (1 << 0)) || (nFlags & (1 << 1)) || !(nFlags & (1 << 2)))
			{
				oAnnot.free(); oType.free();
				continue;
			}

			char* sType = oType.getName();
			if (((nFlag == 0 || nFlag == 3) && strcmp(sType, "Widget")) || (nFlag == 2 && strcmp(sType, "Stamp") && strcmp(sType, "Widget")))
			{
				oType.free(); oAnnot.free();
				continue;
			}
			oType.free();

			// TODO Should we generate appearance for those who don't have it
			DrawAnnot(&oAnnot, pPage, pDoc, xref, &m_mObjManager, nStartRefID);
			oAnnot.free();
		}
		oAnnots.free();

		pageObj.free();
	}

	return true;
}
bool CPdfEditor::DeletePage(int nPageIndex)
{
	if (m_nMode == Mode::Unknown && !IncrementalUpdates())
		return false;

	WriteRedact({});
	m_arrRedact.clear();

	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	PdfWriter::CPage* pPage = pDoc->GetPage(nPageIndex);
	int nObjID = m_mObjManager.FindObj(pPage);
	bool bRes = pDoc->DeletePage(nPageIndex);
	if (bRes && nObjID > 0)
	{
		m_mObjManager.RemoveObj(nObjID);
		pDoc->RemoveObj(pPage);
	}
	return bRes;
}
bool CPdfEditor::AddPage(int nPageIndex)
{
	if (m_nMode == Mode::Unknown && !IncrementalUpdates())
		return false;

	WriteRedact({});
	m_arrRedact.clear();

	if (m_nMode == Mode::Split)
	{
		std::vector<int>::iterator it = std::find(m_mObjManager.m_arrSplitAddPages.begin(), m_mObjManager.m_arrSplitAddPages.end(), m_nOriginIndex++);
		if (it == m_mObjManager.m_arrSplitAddPages.end())
		{
			PdfWriter::CPageTree* pPageTree = m_pWriter->GetDocument()->GetPageTree();
			pPageTree->CreateFakePages(1, nPageIndex);
			return false;
		}
		m_mObjManager.m_arrSplitAddPages.erase(it);
	}

	// Mode WriteNew & WriteAppend
	m_nEditPage = -1;
	// Apply page addition for writer
	if (!m_pWriter->AddPage(nPageIndex))
		return false;
	// By default, first page dimensions are set for new page, they can be changed later
	double dPageDpiX, dPageDpiY;
	double dWidth, dHeight;
	m_pReader->GetPageInfo(0, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

	dWidth  *= 25.4 / dPageDpiX;
	dHeight *= 25.4 / dPageDpiY;

	m_pWriter->put_Width(dWidth);
	m_pWriter->put_Height(dHeight);
	return true;
}
bool CPdfEditor::MovePage(int nPageIndex, int nPos)
{
	WriteRedact({});
	m_arrRedact.clear();

	if (m_nMode == Mode::Split || m_nMode == Mode::WriteNew || EditPage(nPageIndex, true, true))
		return m_pWriter->GetDocument()->MovePage(nPageIndex, nPos);
	return false;
}
bool CPdfEditor::EditAnnot(int _nPageIndex, int nID)
{
	PdfWriter::CObjectBase* pObj = m_mObjManager.GetObj(nID);
	if (pObj)
		return true;

	PDFDoc* pPDFDocument = NULL;
	PdfReader::CPdfFontList* pFontList = NULL;
	int nStartRefID = 0;
	int nPageIndex = m_pReader->GetPageIndex(_nPageIndex, &pPDFDocument, &pFontList, &nStartRefID);
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	if (nPageIndex < 0 || !pPDFDocument || !pDoc)
		return false;

	if (pDoc->GetAnnot(nID))
		return true;

	if (m_nMode == Mode::Split || m_nMode == Mode::WriteNew)
		return false;

	XRef* xref = pPDFDocument->getXRef();
	Ref* pPageRef = pPDFDocument->getCatalog()->getPageRef(nPageIndex);
	if (!xref || !pPageRef || pPageRef->num == 0)
		return false;

	// Get annotation object
	Object pageRefObj, pageObj, oAnnots;
	pageRefObj.initRef(pPageRef->num, pPageRef->gen);
	if (!pageRefObj.fetch(xref, &pageObj)->isDict() || !pageObj.dictLookup("Annots", &oAnnots)->isArray())
	{
		pageRefObj.free(); pageObj.free(); oAnnots.free();
		return false;
	}
	pageRefObj.free(); pageObj.free();

	Object oAnnotRef, oAnnot, oType;
	for (int i = 0; i < oAnnots.arrayGetLength(); ++i)
	{
		if (oAnnots.arrayGetNF(i, &oAnnotRef)->isRef() && oAnnotRef.getRefNum() + nStartRefID == nID)
			break;
		oAnnotRef.free();
	}
	oAnnots.free();
	if (!oAnnotRef.isRef() || !oAnnotRef.fetch(xref, &oAnnot)->isDict() || !oAnnot.dictLookup("Subtype", &oType)->isName())
	{
		oAnnotRef.free(); oAnnot.free(); oType.free();
		return false;
	}

	if (!pDoc->GetEditPage(_nPageIndex))
		EditPage(_nPageIndex, false);

	// Reproduce annotation dictionary from reader for writer
	PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, oAnnotRef.getRefNum());
	if (!pXref)
	{
		oAnnotRef.free(); oAnnot.free(); oType.free();
		return false;
	}

	bool bIsWidget = false;
	PdfWriter::CAnnotation* pAnnot = NULL;
	if (oType.isName("Text"))
		pAnnot = new PdfWriter::CTextAnnotation(pXref);
	else if (oType.isName("Ink"))
		pAnnot = new PdfWriter::CInkAnnotation(pXref);
	else if (oType.isName("Line"))
		pAnnot = new PdfWriter::CLineAnnotation(pXref);
	else if (oType.isName("Highlight") || oType.isName("Underline") || oType.isName("Squiggly") || oType.isName("StrikeOut"))
		pAnnot = new PdfWriter::CTextMarkupAnnotation(pXref);
	else if (oType.isName("Square") || oType.isName("Circle"))
		pAnnot = new PdfWriter::CSquareCircleAnnotation(pXref);
	else if (oType.isName("Polygon") || oType.isName("PolyLine"))
		pAnnot = new PdfWriter::CPolygonLineAnnotation(pXref);
	else if (oType.isName("FreeText"))
	{
		std::vector<Ref> arrFont = PdfReader::GetFreeTextFonts(pPDFDocument, m_pReader->GetFontManager(), pFontList, &oAnnotRef);
		for (int i = 0; i < arrFont.size(); ++i)
		{
			Ref oFontInfo = arrFont[i];
			PdfReader::TFontEntry pEntry;
			pFontList->GetFont(&oFontInfo, &pEntry);

			m_mFonts.insert(std::make_pair(pEntry.wsFontName, pEntry.wsFilePath));
		}
		pAnnot = new PdfWriter::CFreeTextAnnotation(pXref);
	}
	else if (oType.isName("Caret"))
		pAnnot = new PdfWriter::CCaretAnnotation(pXref);
	else if (oType.isName("Stamp"))
		pAnnot = new PdfWriter::CStampAnnotation(pXref);
	else if (oType.isName("Redact"))
		pAnnot = new PdfWriter::CRedactAnnotation(pXref);
	else if (oType.isName("Popup"))
		pAnnot = new PdfWriter::CPopupAnnotation(pXref);
	else if (oType.isName("Link"))
		pAnnot = new PdfWriter::CLinkAnnotation(pXref);
	else if (oType.isName("Widget"))
	{
		bIsWidget = true;
		char* sName = NULL;
		Object oFT;
		if (oAnnot.dictLookup("FT", &oFT)->isName())
			sName = oFT.getName();

		if (!sName)
		{
			Object oParent, oParent2;
			oAnnot.dictLookup("Parent", &oParent);
			while (oParent.isDict())
			{
				if (oParent.dictLookup("FT", &oFT)->isName())
				{
					sName = oFT.getName();
					break;
				}
				oFT.free();
				oParent.dictLookup("Parent", &oParent2);
				oParent.free();
				oParent = oParent2;
			}
			oParent.free();
		}

		if (sName)
		{
			if (strcmp("Btn", sName) == 0)
			{
				bool bPushButton = false;
				oFT.free();
				int nFf = 0;
				if (oAnnot.dictLookup("Ff", &oFT)->isInt())
					nFf = oFT.getInt();
				if (!nFf)
				{
					Object oParent, oParent2;
					oAnnot.dictLookup("Parent", &oParent);
					while (oParent.isDict())
					{
						if (oParent.dictLookup("Ff", &oFT)->isInt())
						{
							nFf = oFT.getInt();
							break;
						}
						oFT.free();
						oParent.dictLookup("Parent", &oParent2);
						oParent.free();
						oParent = oParent2;
					}
					oParent.free();
				}

				bPushButton = (bool)((nFf >> 16) & 1);
				bool bRadiobutton = (bool)((nFf >> 15) & 1);
				if (bPushButton)
					pAnnot = new PdfWriter::CPushButtonWidget(pXref);
				else
					pAnnot = new PdfWriter::CCheckBoxWidget(pXref, bRadiobutton ? PdfWriter::WidgetRadiobutton : PdfWriter::WidgetCheckbox);
			}
			else if (strcmp("Tx", sName) == 0)
				pAnnot = new PdfWriter::CTextWidget(pXref);
			else if (strcmp("Ch", sName) == 0)
				pAnnot = new PdfWriter::CChoiceWidget(pXref);
			else if (strcmp("Sig", sName) == 0)
				pAnnot = new PdfWriter::CSignatureWidget(pXref);
			else
				pAnnot = new PdfWriter::CWidgetAnnotation(pXref, PdfWriter::EAnnotType::AnnotWidget);
		}
		oFT.free();
	}

	if (!pAnnot)
	{
		oAnnotRef.free(); oAnnot.free(); oType.free();
		RELEASEOBJECT(pXref);
		return false;
	}
	pXref->Add(pAnnot, oAnnotRef.getRefGen());

	SDictConvertContext ctx;
	for (int nIndex = 0; nIndex < oAnnot.dictGetLength(); ++nIndex)
	{
		char* chKey = oAnnot.dictGetKey(nIndex);
		if (!strcmp("Popup", chKey))
		{
			Object oPopupRef;
			if (oAnnot.dictGetValNF(nIndex, &oPopupRef)->isRef() && EditAnnot(nPageIndex, oPopupRef.getRefNum() + nStartRefID))
			{
				PdfWriter::CAnnotation* pPopup = pDoc->GetAnnot(oPopupRef.getRefNum() + nStartRefID);
				if (pPopup)
				{
					pAnnot->Add("Popup", pPopup);
					pPopup->Add("Parent", pAnnot);
				}
			}
			continue;
		}
		else if (!strcmp("Parent", chKey) && bIsWidget)
		{
			Object oParentRef;
			oAnnot.dictGetValNF(nIndex, &oParentRef);
			PdfWriter::CDictObject* pParent = GetWidgetParent(pPDFDocument, pDoc, &oParentRef, nStartRefID);

			if (!pParent)
			{
				oParentRef.free();
				continue;
			}

			((PdfWriter::CWidgetAnnotation*)pAnnot)->SetParent(pParent);
			PdfWriter::CArrayObject* pKids = dynamic_cast<PdfWriter::CArrayObject*>(pParent->Get("Kids"));
			if (!pKids)
			{
				oParentRef.free();
				continue;
			}

			for (int i = 0; i < pKids->GetCount(); ++i)
			{
				PdfWriter::CObjectBase* pKid = pKids->Get(i);
				if (pKid->GetObjId() == oAnnotRef.getRefNum())
				{
					pKids->Insert(pKid, pAnnot, true);
					break;
				}
			}
			oParentRef.free();
			continue;
		}
		else if (!strcmp("AP", chKey) && pAnnot->GetAnnotationType() == PdfWriter::AnnotWidget)
		{
			PdfWriter::EWidgetType nType = ((PdfWriter::CWidgetAnnotation*)pAnnot)->GetWidgetType();
			if (nType == PdfWriter::WidgetRadiobutton || nType == PdfWriter::WidgetCheckbox)
			{
				PdfWriter::CCheckBoxWidget* pCAnnot = dynamic_cast<PdfWriter::CCheckBoxWidget*>(pAnnot);

				Object oAP, oN;
				if (oAnnot.dictGetVal(nIndex, &oAP)->isDict() && oAP.dictLookup("N", &oN)->isDict())
				{
					for (int j = 0, nNormLength = oN.dictGetLength(); j < nNormLength; ++j)
					{
						std::string sNormName(oN.dictGetKey(j));
						if (sNormName != "Off")
						{
							pCAnnot->SetAP_N_Yes(UTF8_TO_U(sNormName));
							break;
						}
					}
				}
			}
		}
		Object oTemp;
		oAnnot.dictGetValNF(nIndex, &oTemp);
		DictToCDictObjectUnified(&oTemp, ctx, pAnnot, chKey);
		oTemp.free();
	}

	if (oType.isName("Stamp"))
	{
		Object oAP, oAPN;
		if (oAnnot.dictLookup("AP", &oAP)->isDict() && oAP.dictLookup("N", &oAPN)->isStream())
		{
			Object oAPNRef;
			oAP.dictLookupNF("N", &oAPNRef);
			PdfWriter::CXref* pXRef = new PdfWriter::CXref(pDoc, oAPNRef.getRefNum());
			pDoc->EditXref(pXRef);

			PdfWriter::CDictObject* pAPN = new PdfWriter::CDictObject();
			pXRef->Add(pAPN, oAPNRef.getRefGen());
			((PdfWriter::CStampAnnotation*)pAnnot)->SetAPStream(pAPN);
			oAPNRef.free();

			Object oTemp;
			SDictConvertContext ctx;
			Dict* pODict = oAPN.streamGetDict();
			for (int nIndex = 0; nIndex < pODict->getLength(); ++nIndex)
			{
				char* chKey = pODict->getKey(nIndex);
				pODict->getValNF(nIndex, &oTemp);
				DictToCDictObjectUnified(&oTemp, ctx, pAPN, chKey);
				oTemp.free();
			}

			int nLength = 0;
			if (pODict->lookup("Length", &oTemp)->isInt())
				nLength = oTemp.getInt();
			PdfWriter::CStream* pStream = new PdfWriter::CMemoryStream(nLength);
			pAPN->SetStream(pStream);
			pAPN->Add("Length", nLength);
			if (pAPN->Get("Filter"))
				pAPN->SetFilter(STREAM_FILTER_ALREADY_DECODE);

			Stream* pOStream = oAPN.getStream()->getUndecodedStream();
			pOStream->reset();
			int nChar = pOStream->getChar();
			while (nChar != EOF)
			{
				pStream->WriteChar(nChar);
				nChar = pOStream->getChar();
			}
		}
		oAP.free(); oAPN.free();
	}
	oAnnotRef.free(); oAnnot.free(); oType.free();

	if (pDoc->EditAnnot(pXref, pAnnot, nID))
		return true;

	RELEASEOBJECT(pXref);
	return false;
}
bool CPdfEditor::DeleteAnnot(int nID, Object* oAnnots)
{
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	if (!pDoc)
		return false;

	PdfWriter::CObjectBase* pObj = m_mObjManager.GetObj(nID);
	if (pObj)
	{
		if (pObj->GetType() == PdfWriter::object_type_DICT)
		{
			PdfWriter::CObjectBase* pPopup = ((PdfWriter::CDictObject*)pObj)->Get("Popup");
			int nIDPopup = m_mObjManager.FindObj(pPopup);
			m_mObjManager.RemoveObj(nIDPopup);
			pDoc->RemoveObj(pPopup);
		}
		m_mObjManager.RemoveObj(nID);
		pDoc->RemoveObj(pObj);
		return true;
	}
	if (m_nMode == Mode::Split || m_nMode == Mode::WriteNew)
		return true;

	PDFDoc* pPDFDocument = NULL;
	int nPageIndex = m_pReader->GetPageIndex(m_nEditPage, &pPDFDocument);
	if (nPageIndex < 0 || !pPDFDocument)
		return false;

	XRef* xref = pPDFDocument->getXRef();
	bool bClear = false;
	if (!oAnnots)
	{
		PdfWriter::CPage* pPage = pDoc->GetCurPage();
		std::pair<int, int> pPageRef = { pPage->GetObjId(), pPage->GetGenNo() };
		if (pPageRef.first == 0)
			return false;

		oAnnots = new Object();
		bClear = true;

		// Get annotation object
		Object pageRefObj, pageObj;
		pageRefObj.initRef(pPageRef.first, pPageRef.second);
		if (!pageRefObj.fetch(xref, &pageObj)->isDict() || !pageObj.dictLookup("Annots", oAnnots)->isArray())
		{
			pageRefObj.free(); pageObj.free(); oAnnots->free();
			RELEASEOBJECT(oAnnots);
			return false;
		}
		pageRefObj.free(); pageObj.free();
	}

	bool bRes = false;
	for (int i = 0; i < oAnnots->arrayGetLength(); ++i)
	{
		Object oAnnotRef, oAnnot;
		if (oAnnots->arrayGetNF(i, &oAnnotRef)->isRef() && oAnnotRef.getRefNum() == nID)
		{
			bool bNeed = false;
			if (oAnnotRef.fetch(xref, &oAnnot)->isDict())
			{
				Object oType;
				if (oAnnot.dictLookup("Subtype", &oType)->isName("Widget"))
				{
					char* sName = NULL;
					Object oFT;
					if (oAnnot.dictLookup("FT", &oFT)->isName())
						sName = oFT.getName();

					if (!sName)
					{
						Object oParent, oParent2;
						oAnnot.dictLookup("Parent", &oParent);
						while (oParent.isDict())
						{
							if (oParent.dictLookup("FT", &oFT)->isName())
							{
								sName = oFT.getName();
								break;
							}
							oFT.free();
							oParent.dictLookup("Parent", &oParent2);
							oParent.free();
							oParent = oParent2;
						}
						oParent.free();
					}

					if (sName && strcmp("Btn", sName) == 0)
					{
						bool bPushButton = false;
						oFT.free();
						int nFf = 0;
						if (oAnnot.dictLookup("Ff", &oFT)->isInt())
							nFf = oFT.getInt();
						if (!nFf)
						{
							Object oParent, oParent2;
							oAnnot.dictLookup("Parent", &oParent);
							while (oParent.isDict())
							{
								if (oParent.dictLookup("Ff", &oFT)->isInt())
								{
									nFf = oFT.getInt();
									break;
								}
								oFT.free();
								oParent.dictLookup("Parent", &oParent2);
								oParent.free();
								oParent = oParent2;
							}
							oParent.free();
						}

						bPushButton = (bool)((nFf >> 16) & 1);
						bool bRadiosInUnison = (bool)(nFf & (1 << 25));
						if (!bPushButton)
						{
							oFT.free();
							bNeed = oAnnot.dictLookup("Opt", &oFT)->isArray() == gTrue;
							if (!bNeed)
							{
								Object oParent, oParent2;
								oAnnot.dictLookup("Parent", &oParent);
								while (oParent.isDict())
								{
									if (oParent.dictLookup("Opt", &oFT)->isArray())
									{
										bNeed = true;
										break;
									}
									oFT.free();
									oParent.dictLookup("Parent", &oParent2);
									oParent.free();
									oParent = oParent2;
								}
								oParent.free();
							}
							if (bNeed && EditAnnot(m_nEditPage, nID))
							{
								PdfWriter::CAnnotation* pAnnot = pDoc->GetAnnot(nID);
								if (pAnnot)
								{
									pAnnot->SetHidden();

									PdfWriter::CObjectBase* pObj = pAnnot->Get("Parent");
									PdfWriter::CDictObject* pParent = NULL;
									if (pObj && pObj->GetType() == PdfWriter::object_type_DICT)
										pParent = (PdfWriter::CDictObject*)pObj;
									PdfWriter::CArrayObject* pOpt = NULL, *pKids = NULL;
									if (pParent)
									{
										pObj = pParent->Get("Kids");
										if (pObj && pObj->GetType() == PdfWriter::object_type_ARRAY)
											pKids = (PdfWriter::CArrayObject*)pObj;
										pObj = pParent->Get("Opt");
										if (pObj && pObj->GetType() == PdfWriter::object_type_ARRAY)
											pOpt = (PdfWriter::CArrayObject*)pObj;
									}
									std::map<std::wstring, std::wstring> mNameAP_N_Yes;
									if (pKids && pOpt && pKids->GetCount() == pOpt->GetCount())
									{
										for (int i = 0; i < pKids->GetCount(); ++i)
										{
											pObj = pKids->Get(i);
											if (pObj == pAnnot)
											{
												pObj = pKids->Remove(i);
												delete pObj;
												pObj = pOpt->Remove(i);
												delete pObj;
												--i;
											}
											else
											{
												pObj = pOpt->Get(i);
												if (pObj->GetType() == PdfWriter::object_type_ARRAY && ((PdfWriter::CArrayObject*)pObj)->GetCount() > 0)
													pObj = ((PdfWriter::CArrayObject*)pObj)->Get(0);
												std::wstring sNameOpt;
												if (pObj->GetType() == PdfWriter::object_type_STRING)
												{
													PdfWriter::CStringObject* pStr = (PdfWriter::CStringObject*)pObj;
													sNameOpt = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pStr->GetString(), pStr->GetLength());
													if (mNameAP_N_Yes.find(sNameOpt) == mNameAP_N_Yes.end())
														mNameAP_N_Yes[sNameOpt] = std::to_wstring(i);
												}
												pObj = pKids->Get(i);
												Object oAnnot, oSubtype, oPageRef;
												if (xref->fetch(pObj->GetObjId(), pObj->GetGenNo(), &oAnnot)->isDict("Annot") && oAnnot.dictLookup("Subtype", &oSubtype)->isName("Widget") &&
													oAnnot.dictLookupNF("P", &oPageRef)->isRef())
												{
													int nPage = pPDFDocument->findPage(oPageRef.getRefNum(), oPageRef.getRefGen()) - 1;
													PdfWriter::CCheckBoxWidget* pKidAnnot = NULL;
													int nObjId = pObj->GetObjId();
													if (nPage >= 0 && EditAnnot(nPage, nObjId))
														pKidAnnot = dynamic_cast<PdfWriter::CCheckBoxWidget*>(pDoc->GetAnnot(nObjId));
													if (pKidAnnot && !sNameOpt.empty())
														pKidAnnot->RenameAP_N_Yes((bRadiosInUnison || pKidAnnot->GetWidgetType() == PdfWriter::WidgetCheckbox) ? mNameAP_N_Yes[sNameOpt] : std::to_wstring(i));
												}
												oAnnot.free(); oSubtype.free(); oPageRef.free();
											}
										}
									}

									Object oPopupRef;
									if (oAnnot.dictLookupNF("Popup", &oPopupRef)->isRef())
									{
										pAnnot = pDoc->GetAnnot(oPopupRef.getRefNum());
										if (pAnnot)
											pAnnot->SetHidden();
									}
									oPopupRef.free();
								}
							}
						}
					}
					oFT.free();
				}
				oType.free();

				if (!bNeed)
				{
					Object oPopupRef;
					if (oAnnot.dictLookupNF("Popup", &oPopupRef)->isRef())
						pDoc->DeleteAnnot(oPopupRef.getRefNum(), oPopupRef.getRefGen());
					oPopupRef.free();
				}
			}
			if (!bNeed)
				bRes = pDoc->DeleteAnnot(oAnnotRef.getRefNum(), oAnnotRef.getRefGen());
		}
		else if (oAnnots->arrayGet(i, &oAnnot)->isDict())
		{
			Object oIRTRef;
			if (oAnnot.dictLookupNF("IRT", &oIRTRef)->isRef() && oIRTRef.getRefNum() == nID)
				DeleteAnnot(oAnnotRef.getRefNum(), oAnnots);
			oIRTRef.free();
		}
		oAnnotRef.free(); oAnnot.free();
	}

	if (bClear)
	{
		oAnnots->free();
		RELEASEOBJECT(oAnnots);
	}

	return bRes;
}
bool CPdfEditor::EditWidgets(IAdvancedCommand* pCommand)
{
	if (m_nMode == Mode::Unknown && !IncrementalUpdates())
		return false;

	WriteRedact({});
	m_arrRedact.clear();

	CWidgetsInfo* pFieldInfo = (CWidgetsInfo*)pCommand;
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();

	for (int i = 0; i < m_mObjManager.m_arrRedactFormFields.size(); ++i)
	{
		IAdvancedCommand::AdvancedCommandType nType = m_mObjManager.m_arrRedactFormFields[i]->GetCommandType();
		if (nType == IAdvancedCommand::AdvancedCommandType::Annotation)
		{
			// Annotation - annotation that has changed or been created and needs to be drawn in Contents of page
			CAnnotFieldInfo* command = dynamic_cast<CAnnotFieldInfo*>(m_mObjManager.m_arrRedactFormFields[i]);
			CAnnotFieldInfo& oInfo = *command;
			if (!oInfo.IsWidget())
				continue;

			int nPage = oInfo.GetPage();
			PdfWriter::CPage* pPage = pDoc->GetEditPage(nPage);
			if (!pPage)
			{
				EditPage(nPage, false);
				pPage = pDoc->GetEditPage(nPage);
			}
			if (!pPage)
				pPage = pDoc->GetPage(nPage);
			if (!pPage)
				continue;

			m_pWriter->SetPage(pPage);
			pDoc->SetCurPage(pPage);

			m_pWriter->AddAnnotField(NULL, command, true);
		}
	}

	const std::vector< std::pair<int, int> >& arrCO = pFieldInfo->GetCO();
	for (int i = 0; i < arrCO.size(); ++i)
	{
		int nObjNum = arrCO[i].first;
		if (pDoc->GetParent(nObjNum))
			continue;
		if (pDoc->GetAnnot(nObjNum))
			continue;

		PDFDoc* pPDFDocument = NULL;
		int nStartRefID = 0;
		int nRefID = m_pReader->FindRefNum(nObjNum, &pPDFDocument, &nStartRefID);
		if (nRefID < 0)
			continue;

		XRefEntry* pEntry = pPDFDocument->getXRef()->getEntry(nRefID);
		pFieldInfo->ChangeCO(i, nRefID, pEntry->type == xrefEntryCompressed ? 0 : pEntry->gen);
	}

	const std::vector<CWidgetsInfo::CParent*>& arrParents = pFieldInfo->GetParents();
	for (CWidgetsInfo::CParent* pParent : arrParents)
	{
		PdfWriter::CDictObject* pDParent = pDoc->GetParent(pParent->nID);
		if (pDParent)
			continue;

		PDFDoc* pPDFDocument = NULL;
		int nStartRefID = 0;
		int nRefID = m_pReader->FindRefNum(pParent->nID, &pPDFDocument, &nStartRefID);
		if (nRefID < 0)
			continue;

		XRefEntry* pEntry = pPDFDocument->getXRef()->getEntry(nRefID);
		Object oParentRef;
		oParentRef.initRef(nRefID, pEntry->type == xrefEntryCompressed ? 0 : pEntry->gen);
		GetWidgetParent(pPDFDocument, pDoc, &oParentRef, nStartRefID);
		// TODO re-set children
		oParentRef.free();
	}
	return true;
}
int  CPdfEditor::GetPagesCount()
{
	return m_pWriter->GetDocument()->GetPagesCount();
}
void CPdfEditor::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	PdfWriter::CPage* pPage = m_pWriter->GetDocument()->GetPage(nPageIndex);
	if (!pPage)
		return;

	int nRotate = pPage->GetRotate();
	if (nRotate % 180 == 0)
	{
		*pdWidth  = pPage->GetWidth();
		*pdHeight = pPage->GetHeight();
	}
	else
	{
		*pdWidth  = pPage->GetHeight();
		*pdHeight = pPage->GetWidth();
	}

	*pdDpiX = 72.0;
	*pdDpiY = 72.0;
}
int  CPdfEditor::GetRotate(int nPageIndex)
{
	PdfWriter::CPage* pPage = m_pWriter->GetDocument()->GetPage(nPageIndex);
	if (!pPage)
		return 0;
	return pPage->GetRotate();
}
bool CPdfEditor::IsEditPage()
{
	return m_nEditPage >= 0;
}
void CPdfEditor::ClearPage()
{
	PDFDoc* pPDFDocument = NULL;
	PdfReader::CPdfFontList* pFontList = NULL;
	int nStartRefID = 0;
	int nPageIndex = m_pReader->GetPageIndex(m_nEditPage, &pPDFDocument, &pFontList, &nStartRefID);
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	if (nPageIndex < 0 || !pPDFDocument || !pDoc)
		return;
	XRef* xref = pPDFDocument->getXRef();
	Ref* pPageRef = pPDFDocument->getCatalog()->getPageRef(nPageIndex);

	// Get page object
	Object pageRefObj, pageObj;
	pageRefObj.initRef(pPageRef->num, pPageRef->gen);
	if (!pageRefObj.fetch(xref, &pageObj)->isDict())
	{
		pageObj.free(); pageRefObj.free();
		return;
	}
	pageRefObj.free();

	Object oAnnots;
	// TEMPORARILY delete Link annotations when editing
	if (pageObj.dictLookup("Annots", &oAnnots)->isArray())
	{
		for (int nIndex = 0; nIndex < oAnnots.arrayGetLength(); ++nIndex)
		{
			Object oAnnot, oSubtype, oAnnotRef;
			if (oAnnots.arrayGet(nIndex, &oAnnot)->isDict("Annot") && oAnnot.dictLookup("Subtype", &oSubtype)->isName("Link"))
			{
				oAnnots.arrayGetNF(nIndex, &oAnnotRef);
				DeleteAnnot(oAnnotRef.getRefNum(), &oAnnots);
			}
			oAnnot.free(); oSubtype.free(); oAnnotRef.free();
		}
	}
	pageObj.free();

	if (m_nMode == Mode::Split || m_nMode == Mode::WriteNew)
	{
		PdfWriter::CPage* pPage = pDoc->GetCurPage();
		auto removeContentObj = [&](PdfWriter::CObjectBase* pObj)
		{
			if (pObj->GetType() == PdfWriter::object_type_DICT)
			{
				PdfWriter::CObjectBase* pLength = ((PdfWriter::CDictObject*)pObj)->Get("Length");
				if (pLength)
				{
					int nLengthID = m_mObjManager.FindObj(pLength);
					m_mObjManager.RemoveObj(nLengthID);
				}
			}
			int nObjID = m_mObjManager.FindObj(pObj);
			m_mObjManager.RemoveObj(nObjID);
		};

		PdfWriter::CObjectBase* pObjContents = pPage->Get("Contents");
		if (pObjContents && pObjContents->GetType() == PdfWriter::object_type_ARRAY)
		{
			PdfWriter::CArrayObject* pContents = (PdfWriter::CArrayObject*)pObjContents;
			for (int i = 0; i < pContents->GetCount(); ++i)
				removeContentObj(pContents->Get(i));
		}
		else if (pObjContents)
			removeContentObj(pObjContents);

		pDoc->ClearPageFull();
	}
	else
		pDoc->ClearPage();

	Page* pOPage = pPDFDocument->getCatalog()->getPage(nPageIndex);
	if (pOPage->isCropped())
	{
		PDFRectangle* pCropBox = pOPage->getCropBox();
		PdfWriter::CStream* pStream = pDoc->GetCurPage()->GetStream();
		pStream->WriteStr("1 0 0 1 ");
		pStream->WriteReal(pCropBox->x1);
		pStream->WriteChar(' ');
		pStream->WriteReal(pCropBox->y2 - pOPage->getMediaBox()->y2);
		pStream->WriteStr(" cm\012");
	}

	if (m_nMode == Mode::Split)
	{
		oAnnots.free();
		return;
	}

	// Mode::WriteAppend && Mode::WriteNew
	// Reading and processing fonts from page
	Dict* pResources = pOPage->getResourceDict();
	ScanFonts(pPDFDocument, pResources, m_pWriter, &m_mObjManager, m_pReader);
	oAnnots.free();
}
void CPdfEditor::AddShapeXML(const std::string& sXML)
{
	return m_pWriter->GetDocument()->AddShapeXML(sXML);
}
void CPdfEditor::EndMarkedContent()
{
	m_pWriter->GetDocument()->EndShapeXML();
}
bool CPdfEditor::IsBase14(const std::wstring& wsFontName, bool& bBold, bool& bItalic, std::wstring& wsFontPath)
{
	std::map<std::wstring, std::wstring>::const_iterator it = m_mFonts.find(wsFontName);
	if (it != m_mFonts.end())
		wsFontPath = it->second;
	if (wsFontPath.empty())
	{
		const std::map<std::wstring, std::wstring>& mFonts = m_pReader->GetFonts();
		std::map<std::wstring, std::wstring>::const_iterator it2 = mFonts.find(wsFontName);
		if (it2 != mFonts.end())
			wsFontPath = it2->second;
	}
	if (wsFontPath.empty())
		return false;
	if (wsFontName == L"Helvetica")
		return true;
	if (wsFontName == L"Helvetica-Bold")
	{
		bBold = true;
		return true;
	}
	if (wsFontName == L"Helvetica-Oblique")
	{
		bItalic = true;
		return true;
	}
	if (wsFontName == L"Helvetice-BoldOblique")
	{
		bBold = true;
		bItalic = true;
		return true;
	}
	if (wsFontName == L"Courier")
		return true;
	if (wsFontName == L"Courier-Bold")
	{
		bBold = true;
		return true;
	}
	if (wsFontName == L"Courier-Oblique")
	{
		bItalic = true;
		return true;
	}
	if (wsFontName == L"Courier-BoldOblique")
	{
		bBold = true;
		bItalic = true;
		return true;
	}
	if (wsFontName == L"Times" || wsFontName == L"Times-Roman")
		return true;
	if (wsFontName == L"Times-Bold")
	{
		bBold = true;
		return true;
	}
	if (wsFontName == L"Times-Oblique")
	{
		bItalic = true;
		return true;
	}
	if (wsFontName == L"Times-BoldOblique")
	{
		bBold = true;
		bItalic = true;
		return true;
	}
	if (wsFontName == L"Symbol")
		return true;
	if (wsFontName == L"ZapfDingbats")
		return true;
	return false;
}
void CPdfEditor::Redact(IAdvancedCommand* _pCommand)
{
	PDFDoc* pPDFDocument = NULL;
	PdfReader::CPdfFontList* pFontList = NULL;
	int nStartRefID = 0;
	PDFRectangle* cropBox = NULL;
	int nPageIndex = -1;
	Page* pPage = NULL;
	bool bEditPage = IsEditPage();
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	if (bEditPage)
	{
		nPageIndex = m_pReader->GetPageIndex(m_nEditPage, &pPDFDocument, &pFontList, &nStartRefID);
		if (nPageIndex < 0 || !pPDFDocument)
			return;
		pPage = pPDFDocument->getCatalog()->getPage(nPageIndex);
		cropBox = pPage->getCropBox();
	}
	else
	{
		cropBox = new PDFRectangle();
		PdfWriter::CPage* pWPage = pDoc->GetCurPage();
		PdfWriter::TBox oBox = pWPage->GetBox("CropBox");
		if (oBox.IsEmpty())
			oBox = pWPage->GetBox("MediaBox");
		cropBox = new PDFRectangle(oBox.fLeft, oBox.fBottom, oBox.fRight, oBox.fTop);
	}

	std::vector<double> arrAllQuads;
	if (_pCommand)
	{
		CRedact* pCommand = (CRedact*)_pCommand;
		std::vector<CRedact::SRedact*> arrRedacts = pCommand->GetRedact();
		for (CRedact::SRedact* pRedact : arrRedacts)
		{
			m_arrRedact.push_back(CRedactData());
			m_arrRedact.back().sID = pRedact->sID;
			m_arrRedact.back().arrQuads = pRedact->arrQuadPoints;
			for (int i = 0; i < pRedact->arrQuadPoints.size(); i += 2)
			{
				arrAllQuads.push_back(pRedact->arrQuadPoints[i + 0]);
				arrAllQuads.push_back(cropBox->y2 - pRedact->arrQuadPoints[i + 1]);
			}
			int nFlags = pRedact->nFlag;
			if (nFlags & (1 << 0))
			{
				m_arrRedact.back().pRender = pRedact->pRender;
				m_arrRedact.back().nLenRender = pRedact->nRenderLen;
			}
		}
	}

	if (bEditPage)
	{
		PdfWriter::RedactOutputDev oRedactOut(m_pWriter, &m_mObjManager, nStartRefID);
		oRedactOut.NewPDF(pPDFDocument->getXRef());
		oRedactOut.SetRedact(arrAllQuads);

		Object oContents;
		pPage->getContents(&oContents);
		PDFRectangle* box = pPage->getMediaBox();
		Gfx* gfx = new Gfx(pPDFDocument, &oRedactOut, m_nEditPage, pPage->getResourceDict(), 72.0, 72.0, box, NULL, 0);
		gfx->saveState();
		gfx->display(&oContents);
		gfx->endOfPage();
		oContents.free();
		RELEASEOBJECT(gfx);

		PdfWriter::CPage* pWPage = pDoc->GetCurPage();
		pWPage->SetFontType(PdfWriter::EFontType::fontUnknownType);
	}
	else
	{
		RELEASEOBJECT(cropBox);
	}
}
std::vector<double> CPdfEditor::WriteRedact(const std::vector<std::wstring>& arrID)
{
	std::wstring sID;
	if (!arrID.empty())
		sID = arrID[0];
	std::vector<double> arrRes;
	for (int i = 0; i < m_arrRedact.size(); ++i)
	{
		if (m_arrRedact[i].sID == sID)
		{
			for (int j = i; j < m_arrRedact.size(); ++j)
				arrRes.insert(arrRes.end(), m_arrRedact[j].arrQuads.begin(), m_arrRedact[j].arrQuads.end());
			return arrRes;
		}

		CRedactData oRedact = m_arrRedact[i];
		if (oRedact.bDraw || !oRedact.pRender || oRedact.nLenRender != oRedact.arrQuads.size() / 8 * 12)
			continue;

		BYTE* pMemory = oRedact.pRender;
		m_pWriter->AddRedact({});
		double dM1, dM2, dM3, dM4, dM5, dM6;
		m_pWriter->GetTransform(&dM1, &dM2, &dM3, &dM4, &dM5, &dM6);
		LONG lType, lColorB, lAlpha1, lAlpha2;
		m_pWriter->get_BrushType(&lType);
		m_pWriter->get_BrushColor1(&lColorB);
		m_pWriter->get_BrushAlpha1(&lAlpha1);
		m_pWriter->get_BrushAlpha2(&lAlpha2);

		m_pWriter->SetTransform(1, 0, 0, 1, 0, 0);
		m_pWriter->PathCommandEnd();
		m_pWriter->put_BrushType(c_BrushTypeSolid);
		m_pWriter->put_BrushAlpha1(255);
		m_pWriter->put_BrushAlpha2(255);

		for (int i = 0; i < oRedact.arrQuads.size(); i += 8)
		{
			int ret = *((int*)pMemory);
			pMemory += 4;
			LONG R = ret;
			ret = *((int*)pMemory);
			pMemory += 4;
			LONG G = ret;
			ret = *((int*)pMemory);
			pMemory += 4;
			LONG B = ret;
			LONG lColor = (LONG)(R | (G << 8) | (B << 16) | ((LONG)255 << 24));

			m_pWriter->put_BrushColor1(lColor);
			m_pWriter->PathCommandMoveTo(PdfReader::PDFCoordsToMM(oRedact.arrQuads[i + 0]), PdfReader::PDFCoordsToMM(oRedact.arrQuads[i + 1]));
			m_pWriter->PathCommandLineTo(PdfReader::PDFCoordsToMM(oRedact.arrQuads[i + 2]), PdfReader::PDFCoordsToMM(oRedact.arrQuads[i + 3]));
			m_pWriter->PathCommandLineTo(PdfReader::PDFCoordsToMM(oRedact.arrQuads[i + 4]), PdfReader::PDFCoordsToMM(oRedact.arrQuads[i + 5]));
			m_pWriter->PathCommandLineTo(PdfReader::PDFCoordsToMM(oRedact.arrQuads[i + 6]), PdfReader::PDFCoordsToMM(oRedact.arrQuads[i + 7]));
			m_pWriter->PathCommandClose();
			m_pWriter->DrawPath(NULL, L"", c_nWindingFillMode);
			m_pWriter->PathCommandEnd();
		}

		m_pWriter->SetTransform(dM1, dM2, dM3, dM4, dM5, dM6);
		m_pWriter->put_BrushType(lType);
		m_pWriter->put_BrushColor1(lColorB);
		m_pWriter->put_BrushAlpha1(lAlpha1);
		m_pWriter->put_BrushAlpha2(lAlpha2);

		// TODO Redact render should intersect with all subsequent Redacts
		// TODO actually there should be render of Redact commands
		/*
		PdfWriter::CPage* pCurPage = m_pWriter->GetPage();
		pDoc->FixEditPage(pCurPage);
		PdfWriter::CPage* pFakePage = new PdfWriter::CPage(pDoc);
		m_pWriter->SetPage(pFakePage);
		pDoc->SetCurPage(pFakePage);

		// TODO Need to reset current matrix to identity, then shift by CropBox

		pFakePage->SetStream(pCurPage->GetStream());
		pFakePage->Add("Resources", pCurPage->Get("Resources"));

		IMetafileToRenderter* pCorrector = new IMetafileToRenderter(m_pWriter->GetRenderer());
		NSOnlineOfficeBinToPdf::ConvertBufferToRenderer(pRender, nLenRender, pCorrector);
		RELEASEOBJECT(pCorrector);

		m_pWriter->SetPage(pCurPage);
		pDoc->SetCurPage(pCurPage);
		RELEASEOBJECT(pFakePage);
		*/

		m_arrRedact[i].bDraw = true;
	}
	return arrRes;
}
