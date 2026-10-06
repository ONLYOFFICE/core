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
#include "../DesktopEditor/graphics/pro/Graphics.h"

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
#include "lib/goo/GString.h"

#include "SrcWriter/Catalog.h"
#include "SrcWriter/EncryptDictionary.h"
#include "SrcWriter/Info.h"
#include "SrcWriter/ResourcesDictionary.h"
#include "SrcWriter/Streams.h"
#include "SrcWriter/Destination.h"
#include "SrcWriter/Outline.h"
#include "SrcWriter/GState.h"
#include "SrcWriter/RedactOutputDev.h"
#include "SrcWriter/OverlapOutputDev.h"
#include "SrcWriter/Image.h"
#include "SrcWriter/Font14.h"
#include "SrcWriter/FontCidTT.h"

#include <vector>
#include <climits>

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
			for (int nDepth = 0; oParent.isDict() && nDepth < 50; ++nDepth) // a broken file may loop the Parent chain
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
				for (int nDepth = 0; oParent.isDict() && nDepth < 50; ++nDepth) // a broken file may loop the Parent chain
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

void AddWidgetParent(PdfWriter::CDocument* pDoc, CObjectsManager* pManager, PdfWriter::CObjectBase* pObj, int nDepth = 0)
{
	// NULL - a deleted kid (a cleared proxy). nDepth: a widget is not registered,
	// so GetParent does not stop a Parent loop through widgets
	if (!pObj || pObj->GetType() != PdfWriter::object_type_DICT || nDepth > 50)
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
		AddWidgetParent(pDoc, pManager, pObjParent, nDepth + 1);

	if (pDict->GetDictType() != PdfWriter::dict_type_UNKNOWN)
		return;

	PdfWriter::CObjectBase* pObjKids = pDict->Get("Kids");
	if (!pObjKids || pObjKids->GetType() != PdfWriter::object_type_ARRAY)
		return;

	PdfWriter::CArrayObject* pKids = (PdfWriter::CArrayObject*)pObjKids;
	for (int i = 0; i < pKids->GetCount(); ++i)
		AddWidgetParent(pDoc, pManager, pKids->Get(i), nDepth + 1);
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

	PdfWriter::CXObject* pForm = pDoc->CreateForm();
	pPage->GrSave();
	pPage->ExecuteXObject(pForm);
	pPage->GrRestore();

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
void CObjectBaseToObject(PdfWriter::CObjectBase* pObj, Object* pOut)
{
	if (pObj && pObj->GetType() == PdfWriter::object_type_PROXY)
		pObj = ((PdfWriter::CProxyObject*)pObj)->Get();

	if (!pObj)
	{
		pOut->initNull();
		return;
	}

	switch (pObj->GetType())
	{
	case PdfWriter::object_type_BOOLEAN:
		pOut->initBool(((PdfWriter::CBoolObject*)pObj)->Get() ? gTrue : gFalse);
		break;
	case PdfWriter::object_type_NUMBER:
		pOut->initInt(((PdfWriter::CNumberObject*)pObj)->Get());
		break;
	case PdfWriter::object_type_REAL:
		pOut->initReal(((PdfWriter::CRealObject*)pObj)->Get());
		break;
	case PdfWriter::object_type_NAME:
		pOut->initName(((PdfWriter::CNameObject*)pObj)->Get());
		break;
	case PdfWriter::object_type_STRING:
	{
		PdfWriter::CStringObject* pStr = (PdfWriter::CStringObject*)pObj;
		pOut->initString(new GString((const char*)pStr->GetString(), (int)pStr->GetLength(), true));
		break;
	}
	case PdfWriter::object_type_BINARY:
	{
		PdfWriter::CBinaryObject* pBin = (PdfWriter::CBinaryObject*)pObj;
		pOut->initString(new GString((const char*)pBin->GetValue(), (int)pBin->GetLength(), true));
		break;
	}
	case PdfWriter::object_type_ARRAY:
	{
		PdfWriter::CArrayObject* pArr = (PdfWriter::CArrayObject*)pObj;
		pOut->initArray(NULL);
		for (int i = 0; i < pArr->GetCount(); ++i)
		{
			Object oElem;
			CObjectBaseToObject(pArr->Get(i), &oElem);
			pOut->arrayAdd(&oElem);
		}
		break;
	}
	case PdfWriter::object_type_DICT:
	{
		PdfWriter::CDictObject* pDict = dynamic_cast<PdfWriter::CDictObject*>(pObj);
		if (!pDict)
		{
			pOut->initNull();
			break;
		}
		pDict->BeforeWrite();

		Object oDict;
		oDict.initDict((XRef*)NULL);
		const std::map<std::string, PdfWriter::CObjectBase*>& mEntries = pDict->GetDict();
		for (std::map<std::string, PdfWriter::CObjectBase*>::const_iterator it = mEntries.begin(); it != mEntries.end(); ++it)
		{
			Object oVal;
			CObjectBaseToObject(it->second, &oVal);
			oDict.dictAdd(copyString(it->first.c_str()), &oVal);
		}

		PdfWriter::CStream* pWStream = pDict->GetStream();
		if (!pWStream)
		{
			*pOut = oDict;
			break;
		}

		// MemStream frees the buffer itself (needFree=true below), so it must come from xpdf's
		// own allocator (gmalloc/gfree), not new[]/delete[].
		unsigned int unSize = pWStream->Size();
		BYTE* pBuf = unSize ? (BYTE*)gmalloc((int)unSize) : NULL;
		if (pBuf)
		{
			pWStream->Seek(0, PdfWriter::SeekSet);
			unsigned int unRead = unSize;
			pWStream->Read(pBuf, &unRead);
		}
		pOut->initStream(new MemStream((char*)pBuf, 0, unSize, &oDict, gTrue));
		break;
	}
	default:
		pOut->initNull();
		break;
	}
}
void DrawFormThroughRedact(Object* pReaderAppearance, const double rect[4], PdfWriter::CXObject* pForm,
						   PdfWriter::CPage* pPage, CPdfWriter* pWriter, PDFDoc* pPDFDocument, CObjectsManager* pManager,
						   int nStartRefID, int nRedactInfo, const std::vector<double>& arrRedact, bool bExecuteOnPage = true)
{
	PdfWriter::CDocument* pDoc = pWriter->GetDocument();
	XRef* xref = pPDFDocument->getXRef();
	PdfWriter::CResourcesDict* pFormResources = dynamic_cast<PdfWriter::CResourcesDict*>(pForm->Get("Resources"));

	PdfWriter::CPage* pRendererPrevPage = pWriter->GetPage();
	PdfWriter::CPage* pDocPrevPage      = pDoc->GetCurPage();

	PdfWriter::CPage* pFakePage = new PdfWriter::CPage(pDoc);
	pFakePage->SetStream(pForm->GetStream());

	PdfWriter::RedactOutputDev oRedactOut(pWriter, pManager, nStartRefID, nRedactInfo);
	oRedactOut.NewPDF(xref);
	oRedactOut.SetRedact(arrRedact);
	oRedactOut.SetOverlapInfo({}, NULL, NULL, {});
	oRedactOut.StartOnPage(pFakePage, pFormResources);
	double arrIdentity[6] = { 1, 0, 0, 1, 0, 0 };
	oRedactOut.setDefaultCTM(arrIdentity);

	PDFRectangle oBox(0, 0, pPage->GetWidth(), pPage->GetHeight());
	Gfx* gfx = new Gfx(pPDFDocument, &oRedactOut, NULL, &oBox, NULL);
	gfx->saveState();
	gfx->drawAnnot(pReaderAppearance, NULL, rect[0], rect[1], rect[2], rect[3]);
	gfx->endOfPage();
	RELEASEOBJECT(gfx);

	pWriter->EditPage(pRendererPrevPage);
	pDoc->SetCurPage(pDocPrevPage);
	RELEASEOBJECT(pFakePage);

	if (!bExecuteOnPage)
		return;

	PdfWriter::CResourcesDict* pPageResources = pPage->GetResourcesItem();
	if (!pPageResources)
	{
		pPage->AddResource();
		pPageResources = pPage->GetResourcesItem();
	}
	pPage->GrSave();
	pPage->ExecuteXObject(pPageResources->GetXObjectName(pForm));
	pPage->GrRestore();
}
void DrawAnnotRedacted(Object* oAnnot, PdfWriter::CPage* pPage, PdfWriter::CDocument* pDoc, CPdfWriter* pWriter, PDFDoc* pPDFDocument,
					   XRef* xref, CObjectsManager* pManager, int nStartRefID, int nRedactInfo, const std::vector<double>& arrRedact)
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

	Object oTemp;
	double rect[4] = { 0, 0, 0, 0 };
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

	if (rect[0] == rect[2] || rect[1] == rect[3])
	{
		oAPN.free();
		return;
	}

	std::vector<PdfWriter::CPoint> arrRectPolygon =
	{
		PdfWriter::CPoint(rect[0], rect[1]), PdfWriter::CPoint(rect[0], rect[3]),
		PdfWriter::CPoint(rect[2], rect[3]), PdfWriter::CPoint(rect[2], rect[1])
	};

	if (!arrRedact.empty() && PdfWriter::CheckFullRedact(arrRedact, arrRectPolygon))
	{
		oAPN.free();
		return;
	}

	PdfWriter::CXObject* pForm = pDoc->CreateForm();
	pForm->Add("Type", "XObject");
	pForm->Add("Subtype", "Form");
	pForm->Add("FormType", 1);
	pForm->Add("BBox", PdfWriter::CArrayObject::CreateBox(rect[0], rect[1], rect[2], rect[3]));

	PdfWriter::CResourcesDict* pFormResources = pDoc->CreateResourcesDict(true, false);
	Object oAPResources;
	if (oAPN.streamGetDict()->lookup("Resources", &oAPResources)->isDict())
	{
		SDictConvertContext resCtx(pDoc, xref, pManager, nStartRefID);
		for (int j = 0; j < oAPResources.dictGetLength(); ++j)
		{
			Object oResItem;
			char* chKey = oAPResources.dictGetKey(j);
			oAPResources.dictGetValNF(j, &oResItem);
			pFormResources->Add(chKey, DictToCDictObjectUnified(&oResItem, resCtx));
			oResItem.free();
		}
		pFormResources->Fix();
	}
	oAPResources.free();
	pForm->Add("Resources", pFormResources);

	DrawFormThroughRedact(&oAPN, rect, pForm, pPage, pWriter, pPDFDocument, pManager, nStartRefID, nRedactInfo, arrRedact);

	oAPN.free();
}
void DrawAnnotAppearanceRedacted(PdfWriter::CAnnotAppearanceObject* pAPN, const double rect[4], PdfWriter::CPage* pPage,
								  PdfWriter::CDocument* pDoc, CPdfWriter* pWriter, PDFDoc* pPDFDocument,
								  CObjectsManager* pManager, int nRedactInfo, const std::vector<double>& arrRedact,
								  PdfWriter::CXObject* pExistingForm = NULL)
{
	if (rect[0] == rect[2] || rect[1] == rect[3])
		return;

	PdfWriter::CXObject* pForm = pExistingForm ? pExistingForm : pDoc->CreateForm();
	pForm->Add("Type", "XObject");
	pForm->Add("Subtype", "Form");
	pForm->Add("FormType", 1);
	pForm->Add("BBox", PdfWriter::CArrayObject::CreateBox(rect[0], rect[1], rect[2], rect[3]));
	pForm->Add("Resources", pAPN->Get("Resources"));

	Object oReaderAPN;
	CObjectBaseToObject(pAPN, &oReaderAPN);

	DrawFormThroughRedact(&oReaderAPN, rect, pForm, pPage, pWriter, pPDFDocument, pManager, 0, nRedactInfo, arrRedact, pExistingForm == NULL);

	oReaderAPN.free();
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

		bool bAnnot = false;
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
					bAnnot = true;
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
			if (ctx.bIgnoreOO && bAnnot && strcmp("OO", chKey) == 0)
			{
				oTemp.free();
				continue;
			}

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

		// Deleted (m_setDeletedIDs): a later reference does not convert it from the reader again
		if (ctx.pManager->m_setDeletedIDs.count(nObjNum + ctx.nStartRefID))
			return NULL;

		// the editor's IDs, like the manager's, carry nStartRefID
		std::vector<int>::iterator it = std::find(ctx.pManager->m_arrRedactFormFieldsID.begin(), ctx.pManager->m_arrRedactFormFieldsID.end(), nObjNum + ctx.nStartRefID);
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
		pDoc->CreateNew(false);
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
// The gen to fetch reader object nNum with (nNum < getNumObjects()): 0 for an object in an
// object stream, whose xref entry gen is its index in that stream
int FetchGen(XRef* xref, int nNum)
{
	XRefEntry* pEntry = xref->getEntry(nNum);
	return pEntry->type == xrefEntryCompressed ? 0 : pEntry->gen;
}
bool IsButtonField(PdfWriter::CDictObject* pField, PdfWriter::CDictObject* pKid);
// The bytes of an Opt entry's export value (the first item of [export display]); entries with
// equal bytes are one value. false - not a string
bool OptValueBytes(PdfWriter::CObjectBase* pObj, std::string& sBytes)
{
	if (pObj && pObj->GetType() == PdfWriter::object_type_ARRAY && ((PdfWriter::CArrayObject*)pObj)->GetCount() > 0)
		pObj = ((PdfWriter::CArrayObject*)pObj)->Get(0);
	if (pObj && pObj->GetType() == PdfWriter::object_type_STRING)
		sBytes.assign((const char*)((PdfWriter::CStringObject*)pObj)->GetString(), ((PdfWriter::CStringObject*)pObj)->GetLength());
	else if (pObj && pObj->GetType() == PdfWriter::object_type_BINARY)
		sBytes.assign((const char*)((PdfWriter::CBinaryObject*)pObj)->GetValue(), ((PdfWriter::CBinaryObject*)pObj)->GetLength());
	else
		return false;
	return true;
}
// Ff of a field: its own, or inherited
int FieldFlags(PdfWriter::CDictObject* pField)
{
	for (int nDepth = 0; pField && nDepth < 32; ++nDepth)
	{
		PdfWriter::CObjectBase* pObj = pField->Get("Ff");
		if (pObj && pObj->GetType() == PdfWriter::object_type_NUMBER)
			return ((PdfWriter::CNumberObject*)pObj)->Get();
		pObj = pField->Get("Parent");
		pField = (pObj && pObj->GetType() == PdfWriter::object_type_DICT) ? (PdfWriter::CDictObject*)pObj : NULL;
	}
	return 0;
}
// A kid named after the first index of its export value: a checkbox, or a radio in unison.
// A kid converted from the reader is typed checkbox (CreateAnnot): Ff of the field tells a radio too
bool IsUnisonKid(int nFf, PdfWriter::CCheckBoxWidget* pKid)
{
	bool bRadio = (nFf & (1 << 15)) || pKid->GetWidgetType() == PdfWriter::WidgetRadiobutton;
	return !bRadio || (nFf & (1 << 25));
}
// The on state of a kid as it is: its name, else the one of its appearance. "" - none
std::string KidOnState(PdfWriter::CCheckBoxWidget* pKid)
{
	return pKid->NeedAP_N_Yes() ? pKid->GetAPOnState() : pKid->GetAP_N_Yes();
}
// The Opt index V holds; -1 - V is no index (Off, a state name)
int OptIndexOfV(const std::string& sV)
{
	if (sV.empty() || sV.length() >= 10)
		return -1;
	for (char c : sV)
	{
		if (!isdigit((unsigned char)c))
			return -1;
	}
	return std::stoi(sV);
}
// Names the kids of a checkbox or radio group after their Opt index (IsUnisonKid: the first index of the
// same export value). arrKids pairs with Opt by index (NULL - not a button widget). A kid is on when its AS
// is (under any name: the old one, or Yes it was drawn with); none when V is Off. A kid with no AS is on
// as V reads: an Opt index names pVKid; a state name - the kids with that state when they share one
// export value, else the kids whose export value it is (the editor drew them Yes, it reads V so).
// V names the first kid on, AS follows V
void RenameKids(PdfWriter::CDictObject* pParent, PdfWriter::CArrayObject* pOpt, const std::vector<PdfWriter::CCheckBoxWidget*>& arrKids, PdfWriter::CObjectBase* pVKid)
{
	int nFf = FieldFlags(pParent);
	PdfWriter::CObjectBase* pObj = pParent->Get("V");
	std::string sOldV = pObj && pObj->GetType() == PdfWriter::object_type_NAME ? ((PdfWriter::CNameObject*)pObj)->Get() : "";

	std::vector<std::wstring> arrNames(arrKids.size());
	std::vector<std::string> arrValues(arrKids.size());
	std::map<std::string, std::wstring> mNameAP_N_Yes;
	for (int i = 0; i < (int)arrKids.size(); ++i)
	{
		arrNames[i] = std::to_wstring(i);
		if (OptValueBytes(pOpt->Get(i), arrValues[i]))
			arrNames[i] = mNameAP_N_Yes.emplace(arrValues[i], arrNames[i]).first->second;
		else
			arrValues[i] = "#" + std::to_string(i); // no export value: a value of its own
	}

	// the kids with no AS that are on
	std::set<PdfWriter::CCheckBoxWidget*> setOn;
	if (!sOldV.empty() && sOldV != "Off")
	{
		if (OptIndexOfV(sOldV) >= 0)
			setOn.insert(dynamic_cast<PdfWriter::CCheckBoxWidget*>(pVKid));
		else
		{
			std::set<std::string> setValues;
			for (int i = 0; i < (int)arrKids.size(); ++i)
			{
				if (arrKids[i] && KidOnState(arrKids[i]) == sOldV)
				{
					setOn.insert(arrKids[i]);
					setValues.insert(arrValues[i]);
				}
			}
			if (setValues.size() != 1)
			{
				setOn.clear();
				for (int i = 0; i < (int)arrKids.size(); ++i)
				{
					if (arrKids[i] && arrValues[i] == sOldV)
						setOn.insert(arrKids[i]);
				}
			}
		}
	}

	std::string sV = "Off";
	for (int i = 0; i < (int)arrKids.size(); ++i)
	{
		PdfWriter::CCheckBoxWidget* pKid = arrKids[i];
		if (!pKid)
			continue;
		PdfWriter::CObjectBase* pAS = pKid->Get("AS");
		std::string sAS = pAS && pAS->GetType() == PdfWriter::object_type_NAME ? ((PdfWriter::CNameObject*)pAS)->Get() : "";
		bool bOn = sOldV != "Off" && (sAS.empty() ? setOn.count(pKid) > 0 : sAS != "Off");
		pKid->RenameAP_N_Yes(IsUnisonKid(nFf, pKid) ? arrNames[i] : std::to_wstring(i));
		if (bOn && sV == "Off")
			sV = pKid->GetAP_N_Yes();
	}
	for (PdfWriter::CCheckBoxWidget* pKid : arrKids)
	{
		if (pKid)
			pKid->Add("AS", pKid->GetAP_N_Yes() == sV ? sV.c_str() : "Off");
	}
	pParent->Add("V", sV.c_str());
}
// RenameKids for the kids in pParent's Kids. pVKids - the Kids V's Opt index counts: as they were before
// a cut; NULL - Kids
void RenameButtonKids(PdfWriter::CDictObject* pParent, const std::vector<PdfWriter::CObjectBase*>* pVKids = NULL)
{
	PdfWriter::CObjectBase* pObj = pParent->Get("Kids");
	PdfWriter::CArrayObject* pKids = (pObj && pObj->GetType() == PdfWriter::object_type_ARRAY) ? (PdfWriter::CArrayObject*)pObj : NULL;
	pObj = pParent->Get("Opt");
	PdfWriter::CArrayObject* pOpt = (pObj && pObj->GetType() == PdfWriter::object_type_ARRAY) ? (PdfWriter::CArrayObject*)pObj : NULL;
	if (!pKids || !pOpt || pKids->GetCount() != pOpt->GetCount())
		return;

	std::vector<PdfWriter::CCheckBoxWidget*> arrKids;
	for (int i = 0; i < pKids->GetCount(); ++i)
		arrKids.push_back(dynamic_cast<PdfWriter::CCheckBoxWidget*>(pKids->Get(i)));

	pObj = pParent->Get("V");
	int nV = OptIndexOfV(pObj && pObj->GetType() == PdfWriter::object_type_NAME ? ((PdfWriter::CNameObject*)pObj)->Get() : "");
	PdfWriter::CObjectBase* pVKid = NULL;
	if (nV >= 0 && pVKids)
		pVKid = nV < (int)pVKids->size() ? (*pVKids)[nV] : NULL;
	else if (nV >= 0)
		pVKid = nV < pKids->GetCount() ? pKids->Get(nV) : NULL;
	RenameKids(pParent, pOpt, arrKids, pVKid);
}
// Kids of a converted field and of the fields under it: every converted kid the reader lists, in the
// reader's order (a field converted before its kids' pages lacks them - SplitSkipDict), then the kids
// the reader does not list (the editor's new widgets). Opt of a button group is cut to the kids kept
void AddMissingFieldKids(Object* pNode, int nNodeID, XRef* xref, CObjectsManager& mObjManager, int nStartRefID, int nDepth, std::set<int>& setVisited)
{
	// each node once: Kids may list a node again, or its own ancestor
	CObjectGuard oKids;
	if (nDepth > 32 || !setVisited.insert(nNodeID).second || !pNode->dictLookup("Kids", oKids.get())->isArray())
		return;

	PdfWriter::CArrayObject* pKids = NULL;
	PdfWriter::CObjectBase* pField = mObjManager.GetObj(nNodeID);
	if (pField && pField->GetType() == PdfWriter::object_type_DICT)
	{
		PdfWriter::CObjectBase* pKidsObj = ((PdfWriter::CDictObject*)pField)->Get("Kids");
		if (pKidsObj && pKidsObj->GetType() == PdfWriter::object_type_ARRAY)
			pKids = (PdfWriter::CArrayObject*)pKidsObj;
		else if (!pKidsObj)
		{
			pKids = new PdfWriter::CArrayObject();
			((PdfWriter::CDictObject*)pField)->Add("Kids", pKids);
		}
	}

	std::vector<PdfWriter::CObjectBase*> arrKids;
	for (int i = 0; i < oKids->arrayGetLength(); ++i)
	{
		CObjectGuard oKidRef, oKid;
		if (!oKids->arrayGetNF(i, oKidRef.get())->isRef())
			continue;
		int nKidID = oKidRef->getRefNum() + nStartRefID;

		// NULL - deleted, or not in the output
		PdfWriter::CObjectBase* pKid = mObjManager.GetObj(nKidID);
		if (pKid && std::find(arrKids.begin(), arrKids.end(), pKid) == arrKids.end())
			arrKids.push_back(pKid);

		if (oKidRef->fetch(xref, oKid.get())->isDict())
			AddMissingFieldKids(oKid.get(), nKidID, xref, mObjManager, nStartRefID, nDepth + 1, setVisited);
	}
	if (!pKids)
		return;

	for (int j = 0; j < pKids->GetCount(); ++j)
	{
		PdfWriter::CObjectBase* pKid = pKids->Get(j);
		if (pKid && std::find(arrKids.begin(), arrKids.end(), pKid) == arrKids.end())
			arrKids.push_back(pKid);
	}
	bool bSame = pKids->GetCount() == arrKids.size();
	for (int j = 0; bSame && j < arrKids.size(); ++j)
		bSame = pKids->Get(j) == arrKids[j];
	// rebuilt only when every item is a proxy to a converted kid: Clear deletes a direct item
	// and the placeholder of a raw reference
	for (int j = 0; !bSame && j < pKids->GetCount(); ++j)
	{
		PdfWriter::CObjectBase* pItem = pKids->Get(j, false);
		PdfWriter::CObjectBase* pKid = pKids->Get(j);
		if (pItem && (pItem->GetType() != PdfWriter::object_type_PROXY || (pKid && pKid->GetType() == PdfWriter::object_type_UNKNOWN)))
			bSame = true;
	}
	if (!bSame)
	{
		// the proxies are released, the kids stay
		pKids->Clear();
		for (PdfWriter::CObjectBase* pKid : arrKids)
			pKids->Add(pKid);
	}

	// Opt of a checkbox or radio group pairs with Kids by index: only the entries of the kids in Kids stay
	PdfWriter::CDictObject* pDict = (PdfWriter::CDictObject*)pField;
	PdfWriter::CObjectBase* pOptObj = pDict->Get("Opt");
	CObjectGuard oOpt;
	if (!pOptObj || pOptObj->GetType() != PdfWriter::object_type_ARRAY || ((PdfWriter::CArrayObject*)pOptObj)->GetCount() == pKids->GetCount() ||
		!pNode->dictLookup("Opt", oOpt.get())->isArray() || oOpt->arrayGetLength() != oKids->arrayGetLength() || !IsButtonField(pDict, NULL))
		return;
	PdfWriter::CArrayObject* pOpt = new PdfWriter::CArrayObject();
	// the reader's Kids, which V's Opt index counts (NULL - a kid not in the output)
	std::vector<PdfWriter::CObjectBase*> arrVKids;
	for (int i = 0; i < oKids->arrayGetLength(); ++i)
	{
		CObjectGuard oKidRef, oItem;
		if (!oKids->arrayGetNF(i, oKidRef.get())->isRef())
		{
			arrVKids.push_back(NULL);
			continue;
		}
		PdfWriter::CObjectBase* pKid = mObjManager.GetObj(oKidRef->getRefNum() + nStartRefID);
		arrVKids.push_back(pKid);
		bool bIn = false;
		for (int j = 0; pKid && j < pKids->GetCount() && !bIn; ++j)
			bIn = pKids->Get(j) == pKid;
		if (bIn)
		{
			oOpt->arrayGet(i, oItem.get());
			pOpt->Add(DictToCDictObjectUnified(oItem.get(), SDictConvertContext()));
		}
	}
	// the kids the reader does not list have no entry to take
	if (pOpt->GetCount() != pKids->GetCount())
	{
		RELEASEOBJECT(pOpt);
		return;
	}
	pDict->Add("Opt", pOpt);
	RenameButtonKids(pDict, &arrVKids);
}
// AddMissingFieldKids for every field tree of the reader, from the roots of AcroForm Fields
// (Fields may list widgets: their roots are reached through Parent)
void SyncFieldKids(Object* oAcroForm, XRef* xref, CObjectsManager& mObjManager, int nStartRefID)
{
	CObjectGuard oFields;
	if (!oAcroForm->dictLookup("Fields", oFields.get())->isArray())
		return;

	std::set<int> setRoots;
	for (int i = 0; i < oFields->arrayGetLength(); ++i)
	{
		CObjectGuard oRef;
		if (!oFields->arrayGetNF(i, oRef.get())->isRef())
			continue;
		Ref oRoot = oRef->getRef();
		Object oNode;
		oRef->fetch(xref, &oNode);
		for (int nDepth = 0; nDepth < 32 && oNode.isDict(); ++nDepth)
		{
			Object oParentRef;
			if (!oNode.dictLookupNF("Parent", &oParentRef)->isRef())
			{
				oParentRef.free();
				break;
			}
			oRoot = oParentRef.getRef();
			oNode.free();
			oParentRef.fetch(xref, &oNode);
			oParentRef.free();
		}
		oNode.free();
		setRoots.insert(oRoot.num);
	}

	std::set<int> setVisited;
	for (int nRoot : setRoots)
	{
		if (nRoot < 0 || nRoot >= xref->getNumObjects())
			continue;
		CObjectGuard oRootRef, oRoot;
		oRootRef->initRef(nRoot, FetchGen(xref, nRoot));
		if (oRootRef->fetch(xref, oRoot.get())->isDict())
			AddMissingFieldKids(oRoot.get(), nRoot + nStartRefID, xref, mObjManager, nStartRefID, 0, setVisited);
	}
}
// Copies an indirect array (Fields or CO) from reader into pAcroForm.
// If the array already exists in pAcroForm, merges elements; otherwise creates it.
// For Fields only: registers each field tree (AddWidgetParent) and lists its root, once.
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

		if (bIsFields)
		{
			AddWidgetParent(pDoc, &mObjManager, pObj);
			// Fields lists root fields: an entry with a Parent is replaced by its root.
			// A field's Parent is a field: a widget or a page there is a broken file's
			for (int nDepth = 0; nDepth < 32 && pObj->GetType() == PdfWriter::object_type_DICT; ++nDepth)
			{
				PdfWriter::CObjectBase* pParent = ((PdfWriter::CDictObject*)pObj)->Get("Parent");
				if (!pParent || pParent->GetType() != PdfWriter::object_type_DICT || ((PdfWriter::CDictObject*)pParent)->GetDictType() != PdfWriter::dict_type_UNKNOWN)
					break;
				pObj = pParent;
			}
		}

		// listed already: by EditCO, EditWidgetParents, or as the root of an earlier entry
		bool bHas = false;
		for (int j = 0; j < pArray->GetCount() && !bHas; ++j)
			bHas = pArray->Get(j) == pObj;
		if (bHas)
			continue;

		pArray->Add(pObj);
		mObjManager.IncRefCount(oRes->getRefNum() + nStartRefID);
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

	// Kids the page-by-page conversion missed: a kid converted after its field
	SyncFieldKids(oAcroForm, xref, m_mObjManager, nStartRefID);

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
void RasterizePageOverContent(NSFonts::IFontManager* pFontManager, PdfReader::CPdfFontList* pFontList, PDFDoc* pPDFDocument, int nPageIndex, Object* pContents, Dict* pResourcesDict, PDFRectangle* box, PdfWriter::CDocument* pDoc, PdfWriter::CPage* pPage)
{
	double dPageW = box->x2 - box->x1;
	double dPageH = box->y2 - box->y1;
	if (dPageW <= 0 || dPageH <= 0)
		return;

	double dDpi = 144.0;
	double dMaxSidePx = 5000.0;
	double dMaxSide = (dPageW > dPageH ? dPageW : dPageH) * dDpi / 72.0;
	if (dMaxSide > dMaxSidePx)
		dDpi *= dMaxSidePx / dMaxSide;

	int nRasterW = (int)(dPageW * dDpi / 72.0 + 0.5);
	int nRasterH = (int)(dPageH * dDpi / 72.0 + 0.5);
	if (nRasterW <= 0 || nRasterH <= 0)
		return;

	BYTE* pBgraData = new(std::nothrow) BYTE[(size_t)nRasterW * nRasterH * 4];
	if (!pBgraData)
		return;

	memset(pBgraData, 0xFF, (size_t)nRasterW * nRasterH * 4);

	CBgraFrame oFrame;
	oFrame.put_Data(pBgraData);
	oFrame.put_Width(nRasterW);
	oFrame.put_Height(nRasterH);
	oFrame.put_Stride(4 * nRasterW);

	NSGraphics::IGraphicsRenderer* pRenderer = NSGraphics::Create();
	pRenderer->SetFontManager(pFontManager);
	pRenderer->CreateFromBgraFrame(&oFrame);
	pRenderer->SetSwapRGB(true);
	pRenderer->put_Width (dPageW * 25.4 / 72.0);
	pRenderer->put_Height(dPageH * 25.4 / 72.0);

	PdfReader::RendererOutputDev oRendererOut(pRenderer, pFontManager, pFontList);
	oRendererOut.NewPDF(pPDFDocument->getXRef());

	Gfx* gfx = new Gfx(pPDFDocument, &oRendererOut, nPageIndex, pResourcesDict, 72.0, 72.0, box, NULL, 0);
	gfx->saveState();
	gfx->display(pContents);
	gfx->endOfPage();
	delete gfx;

	delete pRenderer;

	PdfWriter::CImageDict* pImageDict = pDoc->CreateImage();
	pImageDict->LoadRaw(pBgraData, (unsigned int)nRasterW, (unsigned int)nRasterH);

	PdfWriter::CPage* pPrevCurPage = pDoc->GetCurPage();
	pDoc->SetCurPage(pPage);
	pDoc->ClearPageFull();
	pPage->DrawImage(pImageDict, box->x1, box->y1, dPageW, dPageH);
	pDoc->SetCurPage(pPrevCurPage);
}
void CloseWriteNew_ApplyOverlappingObjects(PdfWriter::CPage* pPage, NSFonts::IFontManager* pFontManager, PdfReader::CPdfFontList* pFontList, PDFDoc* pPDFDocument, int nPageIndex, PdfWriter::CDocument* pDoc)
{
	PdfWriter::CObjectBase* pContentsObj = pPage->Get("Contents");
	if (!pContentsObj)
		return;

	Object oContents;
	CObjectBaseToObject(pContentsObj, &oContents);

	PdfWriter::CResourcesDict* pResources = pPage->GetResourcesItem();
	Object oResources;
	CObjectBaseToObject(pResources, &oResources);
	Dict* pResourcesDict = oResources.isDict() ? oResources.getDict() : NULL;

	PdfWriter::TBox oMediaBox = pPage->GetBox("MediaBox");
	PDFRectangle box(oMediaBox.fLeft, oMediaBox.fBottom, oMediaBox.fRight, oMediaBox.fTop);

	RasterizePageOverContent(pFontManager, pFontList, pPDFDocument, nPageIndex, &oContents, pResourcesDict, &box, pDoc, pPage);

	oContents.free();
	oResources.free();
}
void CloseWriteNew_FlushAnnotations(CPdfReader* pReader, CObjectsManager* mObjManager, PdfWriter::CDocument* pDoc, CPdfWriter* pWriter, int nRedactInfo, IAdvancedCommand* pCommand = NULL, const std::vector<double>& arrRedact = {})
{
	std::vector<int> arrWidget;
	if (pCommand)
		arrWidget = ((CRedactMeta*)pCommand)->GetWidgetID();

	std::vector<IAdvancedCommand*> arrProcessed;
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

			PdfWriter::CPage* pPage = NULL;
			if (pCommand)
			{
				int nID = oInfo.GetID();
				std::vector<int>::iterator it = std::find(arrWidget.begin(), arrWidget.end(), nID);
				if (it == arrWidget.end())
					continue;
				pPage = pDoc->GetCurPage();
				arrProcessed.push_back(pCmd);
			}
			else
			{
				int nPage = oInfo.GetPage(); // actual page number
				pPage = pDoc->GetPage(nPage);
				if (!pPage)
					pPage = pDoc->GetEditPage(nPage);
				if (!pPage)
					continue;
			}

			PdfWriter::CAnnotation* pAnnot = pDoc->GetAnnot(oInfo.GetID());
			if (!pAnnot)
				continue;

			if (pCommand)
			{
				PdfWriter::CXObject* pPlaceholder = pDoc->CreateForm();
				PdfWriter::TRect& oRect = pAnnot->GetRect();
				pPlaceholder->Add("Type", "XObject");
				pPlaceholder->Add("Subtype", "Form");
				pPlaceholder->Add("FormType", 1);
				pPlaceholder->Add("BBox", PdfWriter::CArrayObject::CreateBox(oRect.fLeft, oRect.fBottom, oRect.fRight, oRect.fTop));

				PdfWriter::CResourcesDict* pPlaceholderRes = pPage->GetResourcesItem();
				if (!pPlaceholderRes)
				{
					pPage->AddResource();
					pPlaceholderRes = pPage->GetResourcesItem();
				}

				pPage->GrSave();
				pPage->ExecuteXObject(pPlaceholderRes->GetXObjectName(pPlaceholder));
				pPage->GrRestore();

				mObjManager->m_arrPendingWidgetStamps.push_back({ pAnnot, pPlaceholder, pPage, arrRedact });
				continue;
			}

			PdfWriter::CAnnotAppearanceObject* pAPN = GetAnnotAppearanceObject(pAnnot);
			if (!pAPN)
			{
				pDoc->RemoveObj(pAnnot);
				continue;
			}

			std::vector<double> arrAnnotRedact = pCommand ? arrRedact : std::vector<double>();
			PdfWriter::TRect& oAnnotRect = pAnnot->GetRect();
			std::vector<PdfWriter::CPoint> arrRectPolygon =
			{
				PdfWriter::CPoint(oAnnotRect.fLeft, oAnnotRect.fBottom), PdfWriter::CPoint(oAnnotRect.fLeft, oAnnotRect.fTop),
				PdfWriter::CPoint(oAnnotRect.fRight, oAnnotRect.fTop), PdfWriter::CPoint(oAnnotRect.fRight, oAnnotRect.fBottom)
			};

			if (!arrAnnotRedact.empty() && PdfWriter::CheckFullRedact(arrAnnotRedact, arrRectPolygon))
			{
				pDoc->RemoveObj(pAnnot);
				continue;
			}

			bool bPartialRedact = !arrAnnotRedact.empty() && PdfWriter::CheckPartialRedact(arrAnnotRedact, arrRectPolygon);
			PDFDoc* pPDFDocument = bPartialRedact ? pReader->GetPDFDocument(0) : NULL;

			if (bPartialRedact && pPDFDocument)
			{
				double rect[4] = { oAnnotRect.fLeft, oAnnotRect.fBottom, oAnnotRect.fRight, oAnnotRect.fTop };
				DrawAnnotAppearanceRedacted(pAPN, rect, pPage, pDoc, pWriter, pPDFDocument, mObjManager, nRedactInfo, arrAnnotRedact);
			}
			else
			{
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
			}

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

			PdfWriter::CPage* pPage = NULL;
			if (pCommand)
			{
				std::vector<int>::iterator it = std::find(arrWidget.begin(), arrWidget.end(), nRefID);
				if (it == arrWidget.end())
					continue;
				pPage = pDoc->GetCurPage();
				arrProcessed.push_back(pCmd);
			}
			else
				pPage = dynamic_cast<PdfWriter::CPage*>(mObjManager->GetObj(oP->getRefNum() + nStartRefID));

			if (!pPage)
				continue;
			DrawAnnotRedacted(oAnnot.get(), pPage, pDoc, pWriter, pPDFDocument, xref, mObjManager, nStartRefID, nRedactInfo, pCommand ? arrRedact : std::vector<double>());
		}
	}

	if (!arrProcessed.empty())
	{
		std::vector<IAdvancedCommand*>& arrFields = mObjManager->m_arrRedactFormFields;
		arrFields.erase(std::remove_if(arrFields.begin(), arrFields.end(), [&arrProcessed](IAdvancedCommand* pCmd)
		{
			return std::find(arrProcessed.begin(), arrProcessed.end(), pCmd) != arrProcessed.end();
		}), arrFields.end());
	}
}
void CloseWriteNew_FinalizeWidgetStamps(CPdfReader* pReader, CObjectsManager* mObjManager, PdfWriter::CDocument* pDoc, CPdfWriter* pWriter, int nRedactInfo)
{
	std::vector<SPendingWidgetStamp> arrPending;
	arrPending.swap(mObjManager->m_arrPendingWidgetStamps);

	for (size_t i = 0; i < arrPending.size(); ++i)
	{
		SPendingWidgetStamp& oStamp = arrPending[i];
		PdfWriter::CAnnotation* pAnnot = oStamp.pAnnot;

		PdfWriter::CAnnotAppearanceObject* pAPN = GetAnnotAppearanceObject(pAnnot);
		if (!pAPN)
		{
			pDoc->RemoveObj(pAnnot);
			continue;
		}

		PdfWriter::TRect& oAnnotRect = pAnnot->GetRect();
		std::vector<PdfWriter::CPoint> arrRectPolygon =
		{
			PdfWriter::CPoint(oAnnotRect.fLeft, oAnnotRect.fBottom), PdfWriter::CPoint(oAnnotRect.fLeft, oAnnotRect.fTop),
			PdfWriter::CPoint(oAnnotRect.fRight, oAnnotRect.fTop), PdfWriter::CPoint(oAnnotRect.fRight, oAnnotRect.fBottom)
		};

		if (!oStamp.arrAnnotRedact.empty() && PdfWriter::CheckFullRedact(oStamp.arrAnnotRedact, arrRectPolygon))
		{
			pDoc->RemoveObj(pAnnot);
			continue;
		}

		bool bPartialRedact = !oStamp.arrAnnotRedact.empty() && PdfWriter::CheckPartialRedact(oStamp.arrAnnotRedact, arrRectPolygon);
		PDFDoc* pPDFDocument = bPartialRedact ? pReader->GetPDFDocument(0) : NULL;

		if (bPartialRedact && pPDFDocument)
		{
			double rect[4] = { oAnnotRect.fLeft, oAnnotRect.fBottom, oAnnotRect.fRight, oAnnotRect.fTop };
			DrawAnnotAppearanceRedacted(pAPN, rect, oStamp.pPage, pDoc, pWriter, pPDFDocument, mObjManager, nRedactInfo, oStamp.arrAnnotRedact, oStamp.pPlaceholder);
		}
		else
		{
			double m[6] = { 1, 0, 0, 1, 0, 0 };
			pAPN->GetMatrix(m);
			m[4] += oAnnotRect.fLeft;
			m[5] += oAnnotRect.fBottom;
			pAPN->AddMatrix(m[0], m[1], m[2], m[3], m[4], m[5]);

			PdfWriter::CResourcesDict* pFormResources = pDoc->CreateResourcesDict(true, false);
			oStamp.pPlaceholder->Add("Resources", pFormResources);
			const char* sXObjectName = pFormResources->GetXObjectName(pAPN);

			PdfWriter::CPage* pFakePage = new PdfWriter::CPage(pDoc);
			pFakePage->SetStream(oStamp.pPlaceholder->GetStream());
			pFakePage->GrSave();
			pFakePage->ExecuteXObject(sXObjectName);
			pFakePage->GrRestore();
			RELEASEOBJECT(pFakePage);
		}

		pDoc->RemoveObj(pAnnot);
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
// ─── Remove hidden layers ────────────────────────────────────────────────────
bool ContainsRef(const std::vector<Ref>& refs, const Ref& ref)
{
	for (size_t i = 0; i < refs.size(); ++i)
		if (refs[i].num == ref.num && refs[i].gen == ref.gen)
			return true;
	return false;
}
void CollectRefs(Object* pArr, std::vector<Ref>& out)
{
	if (!pArr->isArray())
		return;
	for (int i = 0; i < pArr->arrayGetLength(); ++i)
	{
		CObjectGuard oElem;
		if (pArr->arrayGetNF(i, oElem.get())->isRef())
			out.push_back(oElem.get()->getRef());
	}
}
void CollectHiddenOCGRefs(Object* pOCProperties, std::vector<Ref>& hiddenRefs)
{
	CObjectGuard oD;
	if (!pOCProperties->dictLookup("D", oD.get())->isDict())
		return;

	std::vector<Ref> allRefs, onRefs, offRefs;

	CObjectGuard oOCGs;
	if (pOCProperties->dictLookup("OCGs", oOCGs.get())->isArray())
		CollectRefs(oOCGs.get(), allRefs);

	CObjectGuard oOn;
	if (oD.get()->dictLookupNF("ON", oOn.get())->isArray())
		CollectRefs(oOn.get(), onRefs);

	CObjectGuard oOff;
	if (oD.get()->dictLookupNF("OFF", oOff.get())->isArray())
		CollectRefs(oOff.get(), offRefs);

	for (size_t i = 0; i < offRefs.size(); ++i)
		if (!ContainsRef(hiddenRefs, offRefs[i]))
			hiddenRefs.push_back(offRefs[i]);

	CObjectGuard oBaseState;
	if (oD.get()->dictLookup("BaseState", oBaseState.get())->isName() && !strcmp(oBaseState.get()->getName(), "OFF"))
	{
		for (size_t i = 0; i < allRefs.size(); ++i)
			if (!ContainsRef(onRefs, allRefs[i]) && !ContainsRef(hiddenRefs, allRefs[i]))
				hiddenRefs.push_back(allRefs[i]);
	}
}
void FilterRefArray(Object* pSrc, const std::vector<Ref>& hiddenRefs, XRef* xref, Object* pDst)
{
	pDst->initArray(xref);
	if (!pSrc->isArray())
		return;
	for (int i = 0; i < pSrc->arrayGetLength(); ++i)
	{
		CObjectGuard oElem;
		if (pSrc->arrayGetNF(i, oElem.get())->isRef() && ContainsRef(hiddenRefs, oElem.get()->getRef()))
			continue;
		Object oCopy;
		oElem.get()->copy(&oCopy);
		pDst->arrayAdd(&oCopy);
	}
}
void FilterOrderArray(Object* pSrc, const std::vector<Ref>& hiddenRefs, XRef* xref, Object* pDst)
{
	pDst->initArray(xref);
	if (!pSrc->isArray())
		return;
	for (int i = 0; i < pSrc->arrayGetLength(); ++i)
	{
		CObjectGuard oElem;
		pSrc->arrayGetNF(i, oElem.get());

		if (oElem.get()->isRef())
		{
			if (ContainsRef(hiddenRefs, oElem.get()->getRef()))
				continue;
			Object oCopy;
			oElem.get()->copy(&oCopy);
			pDst->arrayAdd(&oCopy);
		}
		else if (oElem.get()->isArray())
		{
			Object oNested;
			FilterOrderArray(oElem.get(), hiddenRefs, xref, &oNested);
			if (oNested.arrayGetLength() > 0)
				pDst->arrayAdd(&oNested);
			else
				oNested.free();
		}
		else
		{
			Object oCopy;
			oElem.get()->copy(&oCopy);
			pDst->arrayAdd(&oCopy);
		}
	}
}
void FilterRBGroups(Object* pSrc, const std::vector<Ref>& hiddenRefs, XRef* xref, Object* pDst)
{
	pDst->initArray(xref);
	if (!pSrc->isArray())
		return;
	for (int i = 0; i < pSrc->arrayGetLength(); ++i)
	{
		CObjectGuard oGroup;
		if (!pSrc->arrayGet(i, oGroup.get())->isArray())
			continue;

		Object oFiltered;
		FilterRefArray(oGroup.get(), hiddenRefs, xref, &oFiltered);

		if (oFiltered.arrayGetLength() > 1)
			pDst->arrayAdd(&oFiltered);
		else
			oFiltered.free();
	}
}
void FilterOCConfig(Object* pSrcConfig, const std::vector<Ref>& hiddenRefs, XRef* xref, Object* pDstConfig)
{
	pDstConfig->initDict(xref);
	if (!pSrcConfig->isDict())
		return;

	for (int i = 0; i < pSrcConfig->dictGetLength(); ++i)
	{
		char* key = pSrcConfig->dictGetKey(i);

		CObjectGuard oSrcVal;
		pSrcConfig->dictGetValNF(i, oSrcVal.get());

		if (!strcmp(key, "ON") || !strcmp(key, "OFF") || !strcmp(key, "Locked"))
		{
			Object oDstArr;
			FilterRefArray(oSrcVal.get(), hiddenRefs, xref, &oDstArr);
			pDstConfig->dictAdd(copyString(key), &oDstArr);
		}
		else if (!strcmp(key, "Order"))
		{
			Object oDstArr;
			FilterOrderArray(oSrcVal.get(), hiddenRefs, xref, &oDstArr);
			pDstConfig->dictAdd(copyString(key), &oDstArr);
		}
		else if (!strcmp(key, "RBGroups"))
		{
			Object oDstArr;
			FilterRBGroups(oSrcVal.get(), hiddenRefs, xref, &oDstArr);
			pDstConfig->dictAdd(copyString(key), &oDstArr);
		}
		else
		{
			// BaseState, Name, Creator, Intent, AS
			Object oCopy;
			oSrcVal.get()->copy(&oCopy);
			pDstConfig->dictAdd(copyString(key), &oCopy);
		}
	}
}
void RemoveHiddenOCGs(Object* pOCProperties, XRef* xref)
{
	if (!pOCProperties->isDict())
		return;

	std::vector<Ref> hiddenRefs;
	CollectHiddenOCGRefs(pOCProperties, hiddenRefs);
	if (hiddenRefs.empty())
		return;

	Object oNewOCProps;
	oNewOCProps.initDict(xref);

	for (int i = 0; i < pOCProperties->dictGetLength(); ++i)
	{
		char* key = pOCProperties->dictGetKey(i);

		CObjectGuard oSrcVal;
		pOCProperties->dictGetValNF(i, oSrcVal.get());

		if (!strcmp(key, "OCGs"))
		{
			Object oDstArr;
			FilterRefArray(oSrcVal.get(), hiddenRefs, xref, &oDstArr);
			oNewOCProps.dictAdd(copyString(key), &oDstArr);
		}
		else if (!strcmp(key, "D"))
		{
			Object oDstD;
			FilterOCConfig(oSrcVal.get(), hiddenRefs, xref, &oDstD);
			oNewOCProps.dictAdd(copyString(key), &oDstD);
		}
		else if (!strcmp(key, "Configs"))
		{
			Object oDstConfigs;
			oDstConfigs.initArray(xref);

			if (oSrcVal.get()->isArray())
			{
				for (int j = 0; j < oSrcVal.get()->arrayGetLength(); ++j)
				{
					CObjectGuard oCfg;
					oSrcVal.get()->arrayGet(j, oCfg.get());

					Object oDstCfg;
					FilterOCConfig(oCfg.get(), hiddenRefs, xref, &oDstCfg);
					oDstConfigs.arrayAdd(&oDstCfg);
				}
			}
			oNewOCProps.dictAdd(copyString(key), &oDstConfigs);
		}
		else
		{
			Object oCopy;
			oSrcVal.get()->copy(&oCopy);
			oNewOCProps.dictAdd(copyString(key), &oCopy);
		}
	}

	pOCProperties->free();
	oNewOCProps.copy(pOCProperties);
	oNewOCProps.free();
}
}
namespace
{
void PruneSplitFields(CObjectsManager* pManager, PdfWriter::CDocument* pDoc);
void DeleteEmptyAppendFields(CObjectsManager* pManager, PdfWriter::CDocument* pDoc, const std::set<int>& setFields);
// A checkbox or radio group with Opt whose on states disagree with it - one name for kids of different
// values (a kid the editor sent without its state name is drawn as Yes), or AS naming a state the kid
// lacks - is renamed after Opt. A group with a kid not converted or with no state dict is left as it is
void RenameButtonKidsYes(PdfWriter::CDocument* pDoc)
{
	const std::map<int, PdfWriter::CDictObject*>& mParents = pDoc->GetParents();
	for (std::map<int, PdfWriter::CDictObject*>::const_iterator it = mParents.begin(); it != mParents.end(); ++it)
	{
		PdfWriter::CDictObject* pParent = it->second;
		PdfWriter::CObjectBase* pKidsObj = pParent ? pParent->Get("Kids") : NULL;
		PdfWriter::CObjectBase* pOptObj = pParent ? pParent->Get("Opt") : NULL;
		if (!pKidsObj || !pOptObj || pKidsObj->GetType() != PdfWriter::object_type_ARRAY || pOptObj->GetType() != PdfWriter::object_type_ARRAY ||
			((PdfWriter::CArrayObject*)pKidsObj)->GetCount() != ((PdfWriter::CArrayObject*)pOptObj)->GetCount() || !IsButtonField(pParent, NULL))
			continue;
		PdfWriter::CArrayObject* pKids = (PdfWriter::CArrayObject*)pKidsObj;
		PdfWriter::CArrayObject* pOpt = (PdfWriter::CArrayObject*)pOptObj;

		bool bAll = true, bBroken = false;
		std::map<std::string, std::string> mStateValue; // on state -> the kid's export value (Opt bytes)
		for (int i = 0; i < pKids->GetCount() && bAll; ++i)
		{
			PdfWriter::CCheckBoxWidget* pKid = dynamic_cast<PdfWriter::CCheckBoxWidget*>(pKids->Get(i));
			PdfWriter::CObjectBase* pAP = pKid ? pKid->Get("AP") : NULL;
			PdfWriter::CObjectBase* pAPN = pAP && pAP->GetType() == PdfWriter::object_type_DICT ? ((PdfWriter::CDictObject*)pAP)->Get("N") : NULL;
			// an appearance stream, not a state dict: not renamed
			if (!pKid || !pAPN || pAPN->GetType() != PdfWriter::object_type_DICT || ((PdfWriter::CDictObject*)pAPN)->GetStream())
			{
				bAll = false;
				break;
			}
			PdfWriter::CDictObject* pDictAPN = (PdfWriter::CDictObject*)pAPN;
			std::string sOn;
			for (const std::pair<const std::string, PdfWriter::CObjectBase*>& itState : pDictAPN->GetDict())
			{
				if (itState.first != "Off")
				{
					sOn = itState.first;
					break;
				}
			}
			PdfWriter::CObjectBase* pAS = pKid->Get("AS");
			if (pAS && pAS->GetType() == PdfWriter::object_type_NAME && strcmp(((PdfWriter::CNameObject*)pAS)->Get(), "Off") != 0 && !pDictAPN->Get(((PdfWriter::CNameObject*)pAS)->Get()))
				bBroken = true;
			// by value: radios not in unison may share a state for one export value
			std::string sValue = std::to_string(i);
			std::string sBytes;
			if (OptValueBytes(pOpt->Get(i), sBytes))
				sValue = sBytes;
			if (!sOn.empty() && mStateValue.emplace(sOn, sValue).first->second != sValue)
				bBroken = true;
		}
		if (bAll && bBroken)
			RenameButtonKids(pParent);
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
		PruneSplitFields(&m_mObjManager, m_pWriter->GetDocument());
		m_pWriter->EditClose();
		m_pWriter->SaveToFile(m_wsDstFile);
		return;
	}

	if (m_nMode == Mode::WriteNew)
	{
		PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();

		PDFDoc* pPDFDocument = NULL;
		PdfReader::CPdfFontList* pFontList = NULL;
		int nStartRefID = 0;
		m_pReader->GetPageIndex(0, &pPDFDocument, &pFontList, &nStartRefID);

		PdfWriter::CPageTree* pPageTree = pDoc->GetPageTree();
		const int nPages = pPageTree->GetCount();
		for (int i = 0; i < nPages; ++i)
		{
			PdfWriter::CObjectBase* pObj = pPageTree->GetObj(i);
			if (!pObj)
				continue;
			if (pObj->GetType() != PdfWriter::object_type_DICT)
			{
				EditPage(i, false, true);
			}
			/*
			else if (bOverlappingObjects)
			{
				PdfWriter::CPage* pEditedPage = dynamic_cast<PdfWriter::CPage*>(pObj);
				if (pEditedPage)
					CloseWriteNew_ApplyOverlappingObjects(pEditedPage, m_pReader->GetFontManager(), pFontList, pPDFDocument, i, pDoc);
			}
			*/
		}

		CloseWriteNew_FlushAnnotations(m_pReader, &m_mObjManager, pDoc, m_pWriter, m_nRedactInfo);

		SyncAcroFormFromReader();
		RenameButtonKidsYes(pDoc);

		if (!pDoc->GetEncrypt())
			CloseWriteNew_ApplyEncryption(m_wsPassword, &m_mObjManager, pPDFDocument, pDoc);

		XRef* xref = pPDFDocument->getXRef();
		CObjectGuard catDict;
		xref->getCatalog(catDict.get());
		SDictConvertContext ctx(pDoc, xref, &m_mObjManager, nStartRefID);
		PdfWriter::CCatalog* pCatalog = pDoc->GetCatalog();

		if (~m_nRedactInfo & (1 << 1))
		{
			CObjectGuard pNames, oEF, oNames;
			if (catDict->dictLookup("Names", pNames.get())->isDict() && pNames->dictLookup("EmbeddedFiles", oEF.get())->isDict())
			{
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
		// Layers, Optional content
		CObjectGuard oOC;
		if (catDict->dictLookup("OCProperties", oOC.get())->isDict())
		{
			if (m_nRedactInfo & (1 << 7))
				RemoveHiddenOCGs(oOC.get(), xref);
			pCatalog->Add("OCProperties", DictToCDictObjectUnified(oOC.get(), ctx));
		}
		if (m_nRedactInfo & (1 << 8))
			pDoc->RemoveUnreachableObjects();
		if (~m_nRedactInfo & (1 << 9))
		{
			CObjectGuard pNames, oEF, oNames;
			if (catDict->dictLookup("Names", pNames.get())->isDict() && pNames->dictLookup("EmbeddedFiles", oEF.get())->isDict())
			{
				PdfWriter::CDictObject* pNames = pCatalog->GetNames();
				pNames->Add("JavaScript", DictToCDictObjectUnified(oEF.get(), ctx));
			}
		}

		m_pWriter->EditClose();
		m_pWriter->SaveToFile(m_wsDstFile);
		return;
	}

	// m_nMode == Mode::WriteAppend
	PDFDoc*               pPDFDocument = m_pReader->GetPDFDocument(0);
	PdfWriter::CDocument* pDoc         = m_pWriter->GetDocument();
	DeleteEmptyAppendFields(&m_mObjManager, pDoc, m_setLostKidFields);
	RenameButtonKidsYes(pDoc);

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

		bool bHiddenText              = (m_nRedactInfo & (1 << 6)) != 0;
		bool bHiddenLayers            = (m_nRedactInfo & (1 << 7)) != 0;
		bool bDeletedOrCroppedContent = (m_nRedactInfo & (1 << 8)) != 0;
		bool bOverlappingObjects      = (m_nRedactInfo & (1 << 10)) != 0;
		if (bOverlappingObjects)
		{
			Page* pOPage = pCatalog->getPage(nPageIndex);
			PDFRectangle* box = pOPage->getMediaBox();
			Object oContents;
			pOPage->getContents(&oContents);
			RasterizePageOverContent(m_pReader->GetFontManager(), pFontList, pPDFDocument, _nPageIndex, &oContents, pOPage->getResourceDict(), box, pDoc, pPage);
			oContents.free();
		}
		else if (bHiddenText || bHiddenLayers || bDeletedOrCroppedContent)
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
namespace
{
void RemoveOffPageWidgets(CObjectsManager* pManager, PdfWriter::CDocument* pDoc, int nStartRefID, int nEndRefID, const std::vector<PdfWriter::CPage*>& arrPages);
}
bool CPdfEditor::SplitPages(const int* arrPageIndex, unsigned int unLength, PDFDoc* _pDoc, int nStartRefID, bool bIgnoreOO)
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
	ctx.bIgnoreOO = bIgnoreOO;
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

	std::vector<PdfWriter::CPage*> arrPages;
	for (unsigned int i = 0; i < unLength; ++i)
	{
		Ref* pPageRef = pCatalog->getPageRef((arrPageIndex ? arrPageIndex[i] : i) + 1);
		if (pPageRef->num == 0)
			return false;

		// Get page object
		PdfWriter::CPage* pPage = (PdfWriter::CPage*)m_mObjManager.GetObj(pPageRef->num + nStartRefID);
		arrPages.push_back(pPage);
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
	// off-page widgets go before SyncFieldKids and the Fields below take them in
	RemoveOffPageWidgets(&m_mObjManager, pDoc, nStartRefID, nStartRefID + xref->getNumObjects(), arrPages);

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

		// Kids in the reader's order; Opt of a button group is cut to the kids of the pages taken
		SyncFieldKids(&oAcroForm, xref, m_mObjManager, nStartRefID);

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
			else if (strcmp("CO", chKey) == 0)
			{
				// only the fields the pages brought in: a field of a page split away would come with its V
				MergeRefArray(oAcroForm, nIndex, chKey, pDoc, m_mObjManager, nStartRefID, false);
				continue;
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
		if (!SplitPages(it.second.data(), it.second.size(), pPDFDocument, m_pReader->GetStartRefID(pPDFDocument), false))
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

	// The editor's changes are in: a field converted for them (EditWidgets) brings its kids with no P,
	// those of the pages split away too; then a field with no widget left, and states named Yes
	std::vector<PdfWriter::CPage*> arrPages;
	for (unsigned int i = 0; i < pPageTree->GetCount(); ++i)
		arrPages.push_back(pPageTree->GetPage(i)); // NULL - a page not in the part
	RemoveOffPageWidgets(&m_mObjManager, pDoc, 0, INT_MAX, arrPages);
	PruneSplitFields(&m_mObjManager, pDoc);
	RenameButtonKidsYes(pDoc);
	pPageTree->ClearFakePages();
	// what nothing in the part refers to leaves it: the appearance streams of the widgets the editor sent
	// (AddAnnotField removes them: they show the old values), the objects of what was taken out
	pDoc->RemoveUnreachableObjects();
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
	// marked before the recursion: a Parent chain that loops back stops here
	arrRename.push_back(nParent);
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
	bool bRes = SplitPages(NULL, 0, pDocument, nStartRefID, true);
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
namespace
{
bool RemoveParentKid(PdfWriter::CDictObject* pParent, PdfWriter::CDictObject* pKid, int nKidID);
int RemoveArrayItem(PdfWriter::CArrayObject* pArray, PdfWriter::CObjectBase* pKid, int nKidID);
bool HasButtonOpt(PdfWriter::CDictObject* pField, PdfWriter::CDictObject* pKid);
// WriteAppend: the widgets of a deleted page of the original file leave Fields and their field's Kids
// (setLostKid gets the field, setDeleted the widget). A checkbox or radio group with Opt keeps its kids
// (HasButtonOpt): the editor's Opt and V index still count them
void RemovePageWidgets(PDFDoc* pPDFDocument, PdfWriter::CDocument* pDoc, PdfWriter::CObjectBase* pPageObj, std::set<int>& setLostKid, std::set<int>& setDeleted)
{
	PdfWriter::CDictObject* pAcroForm = pDoc->GetAcroForm();
	PdfWriter::CObjectBase* pFieldsObj = pAcroForm ? pAcroForm->Get("Fields") : NULL;
	if (!pPDFDocument || !pPageObj || !pPageObj->GetObjId() || !pFieldsObj || pFieldsObj->GetType() != PdfWriter::object_type_ARRAY)
		return;
	PdfWriter::CArrayObject* pFields = (PdfWriter::CArrayObject*)pFieldsObj;
	XRef* xref = pPDFDocument->getXRef();

	CObjectGuard oPage, oAnnots;
	if (!xref->fetch(pPageObj->GetObjId(), pPageObj->GetGenNo(), oPage.get())->isDict() || !oPage->dictLookup("Annots", oAnnots.get())->isArray())
		return;
	for (int i = 0; i < oAnnots->arrayGetLength(); ++i)
	{
		CObjectGuard oAnnotRef, oNode, oType;
		if (!oAnnots->arrayGetNF(i, oAnnotRef.get())->isRef() || !oAnnotRef->fetch(xref, oNode.get())->isDict() || !oNode->dictLookup("Subtype", oType.get())->isName("Widget"))
			continue;
		int nKid = oAnnotRef->getRefNum();
		RemoveArrayItem(pFields, NULL, nKid);
		CObjectGuard oParentRef;
		PdfWriter::CDictObject* pParent = oNode->dictLookupNF("Parent", oParentRef.get())->isRef() ? GetWidgetParent(pPDFDocument, pDoc, oParentRef.get(), 0) : NULL;
		if (pParent && HasButtonOpt(pParent, NULL))
			continue;
		if (pParent)
		{
			RemoveParentKid(pParent, NULL, nKid);
			setLostKid.insert(oParentRef->getRefNum());
		}
		setDeleted.insert(nKid);
	}
}
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
	if (pPage && nObjID > 0)
	{
		// A converted page (a merged file's; WriteNew, Split) takes its widgets out of the field tree:
		// else their field's Kids keeps them, on no page
		std::vector<int> arrWidgets;
		PdfWriter::CObjectBase* pAnnotsObj = pPage->Get("Annots");
		if (pAnnotsObj && pAnnotsObj->GetType() == PdfWriter::object_type_ARRAY)
		{
			PdfWriter::CArrayObject* pAnnots = (PdfWriter::CArrayObject*)pAnnotsObj;
			for (int i = 0; i < pAnnots->GetCount(); ++i)
			{
				PdfWriter::CAnnotation* pAnnot = dynamic_cast<PdfWriter::CAnnotation*>(pAnnots->Get(i));
				int nAnnotID = (pAnnot && pAnnot->GetAnnotationType() == PdfWriter::AnnotWidget) ? m_mObjManager.FindObj(pAnnot) : -1;
				if (nAnnotID >= 0)
					arrWidgets.push_back(nAnnotID);
			}
		}
		for (int nWidgetID : arrWidgets)
		{
			// still converted only: DeleteAnnot looks any other ID up on the edited page
			PdfWriter::CObjectBase* pWidget = m_mObjManager.GetObj(nWidgetID);
			if (!pWidget || pWidget->GetType() != PdfWriter::object_type_DICT)
				continue;
			// a kid of a checkbox or radio group with Opt stays, as in RemovePageWidgets: the editor's Opt
			// and V index still count it. Split cuts it, EditWidgetParents finds the kid on by export value
			PdfWriter::CObjectBase* pParent = ((PdfWriter::CDictObject*)pWidget)->Get("Parent");
			if (m_nMode != Mode::Split && pParent && pParent->GetType() == PdfWriter::object_type_DICT &&
				HasButtonOpt((PdfWriter::CDictObject*)pParent, (PdfWriter::CDictObject*)pWidget))
				continue;
			DeleteAnnot(nWidgetID, NULL);
		}
	}
	else if (m_nMode == Mode::WriteAppend)
		RemovePageWidgets(m_pReader->GetPDFDocument(0), pDoc, pDoc->GetPageObj(nPageIndex), m_setLostKidFields, m_mObjManager.m_setDeletedIDs);
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
			for (int nDepth = 0; oParent.isDict() && nDepth < 50; ++nDepth) // a broken file may loop the Parent chain
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
					for (int nDepth = 0; oParent.isDict() && nDepth < 50; ++nDepth) // a broken file may loop the Parent chain
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
				if (pKid && pKid->GetObjId() == oAnnotRef.getRefNum())
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
namespace
{
void RemoveObj(CObjectsManager* pManager, PdfWriter::CDocument* pDoc, PdfWriter::CObjectBase* pObj)
{
	std::vector<PdfWriter::CObjectBase*> arrRemoved;
	pDoc->RemoveObj(pObj, true, &arrRemoved);

	for (size_t i = 0; i < arrRemoved.size(); ++i)
	{
		int nIDR = pManager->FindObj(arrRemoved[i]);
		pManager->RemoveObj(nIDR);
	}
}
// true for a button field (FT Btn on the field, an ancestor, else the kid): its Opt pairs with Kids
// by index, a choice field's Opt is its list of items
bool IsButtonField(PdfWriter::CDictObject* pField, PdfWriter::CDictObject* pKid)
{
	for (int nDepth = 0; pField && nDepth < 32; ++nDepth)
	{
		PdfWriter::CObjectBase* pFT = pField->Get("FT");
		if (pFT && pFT->GetType() == PdfWriter::object_type_NAME)
			return strcmp(((PdfWriter::CNameObject*)pFT)->Get(), "Btn") == 0;
		PdfWriter::CObjectBase* pParent = pField->Get("Parent");
		pField = (pParent && pParent->GetType() == PdfWriter::object_type_DICT) ? (PdfWriter::CDictObject*)pParent : NULL;
	}
	PdfWriter::CObjectBase* pFT = pKid ? pKid->Get("FT") : NULL;
	return pFT && pFT->GetType() == PdfWriter::object_type_NAME && strcmp(((PdfWriter::CNameObject*)pFT)->Get(), "Btn") == 0;
}
// A checkbox or radio group with Opt (a WriteAppend copy keeps an indirect Opt as a raw reference)
bool HasButtonOpt(PdfWriter::CDictObject* pField, PdfWriter::CDictObject* pKid)
{
	PdfWriter::CObjectBase* pOpt = pField ? pField->Get("Opt") : NULL;
	return pOpt && (pOpt->GetType() == PdfWriter::object_type_ARRAY || pOpt->GetType() == PdfWriter::object_type_UNKNOWN) && IsButtonField(pField, pKid);
}
// The item of pKid (the writer's object), or of object nKidID when pKid is NULL, leaves the array;
// only its proxy is deleted (CArrayObject::Remove returns the target). Its index, -1 - not there
int RemoveArrayItem(PdfWriter::CArrayObject* pArray, PdfWriter::CObjectBase* pKid, int nKidID)
{
	for (int i = 0; i < pArray->GetCount(); ++i)
	{
		PdfWriter::CObjectBase* pItem = pArray->Get(i);
		if (!pItem || (pKid ? pItem != pKid : (int)pItem->GetObjId() != nKidID))
			continue;
		PdfWriter::CObjectBase* pProxy = pArray->Get(i, false);
		pArray->Remove(i);
		if (pProxy != pItem)
			delete pProxy;
		return i;
	}
	return -1;
}
// The kid leaves the parent's Kids; in a checkbox or radio group whose Opt pairs with Kids, its Opt entry
// too (the editor writes Opt without it). pKid - the writer's kid; NULL - the item of object nKidID
// (WriteAppend: not converted). true - an Opt entry left with it
bool RemoveParentKid(PdfWriter::CDictObject* pParent, PdfWriter::CDictObject* pKid, int nKidID)
{
	PdfWriter::CObjectBase* pKidsObj = pParent->Get("Kids");
	if (!pKidsObj || pKidsObj->GetType() != PdfWriter::object_type_ARRAY)
		return false;

	PdfWriter::CArrayObject* pKids = (PdfWriter::CArrayObject*)pKidsObj;
	PdfWriter::CObjectBase* pOptObj = pParent->Get("Opt");
	PdfWriter::CArrayObject* pOpt = (pOptObj && pOptObj->GetType() == PdfWriter::object_type_ARRAY) ? (PdfWriter::CArrayObject*)pOptObj : NULL;
	if (pOpt && (pOpt->GetCount() != pKids->GetCount() || !IsButtonField(pParent, pKid)))
		pOpt = NULL;
	int i = RemoveArrayItem(pKids, pKid, nKidID);
	if (i < 0 || !pOpt || !pOpt->Get(i))
		return false;
	// the entry itself, or the proxy of an indirect one
	PdfWriter::CObjectBase* pOptItem = pOpt->Get(i, false);
	pOpt->Remove(i);
	RELEASEOBJECT(pOptItem);
	return true;
}
// pDict leaves its parent's Kids (RemoveParentKid); when its Opt entry goes too, the kids left are
// renamed after their new indexes (V's index counts the Kids before)
void RemoveFromParentKids(PdfWriter::CDictObject* pDict)
{
	PdfWriter::CObjectBase* pParent = pDict->Get("Parent");
	if (!pParent || pParent->GetType() != PdfWriter::object_type_DICT)
		return;
	std::vector<PdfWriter::CObjectBase*> arrVKids;
	PdfWriter::CObjectBase* pKids = ((PdfWriter::CDictObject*)pParent)->Get("Kids");
	for (int i = 0; pKids && pKids->GetType() == PdfWriter::object_type_ARRAY && i < ((PdfWriter::CArrayObject*)pKids)->GetCount(); ++i)
		arrVKids.push_back(((PdfWriter::CArrayObject*)pKids)->Get(i));
	if (RemoveParentKid((PdfWriter::CDictObject*)pParent, pDict, 0))
		RenameButtonKids((PdfWriter::CDictObject*)pParent, &arrVKids);
}
// A field whose kids are all deleted is deleted too, up the Parent chain - else its parent's Kids keeps it,
// with no widget and the deleted kids' V. Decided by the reader's Kids: the writer's lacks the kids not converted yet
void DeleteEmptyParents(CPdfReader* pReader, CObjectsManager* pManager, PdfWriter::CDocument* pDoc, int nID)
{
	PDFDoc* pPDFDocument = NULL;
	int nStartRefID = 0;
	int nRefNum = pReader->FindRefNum(nID, &pPDFDocument, &nStartRefID);
	if (nRefNum < 0 || !pPDFDocument)
		return;
	XRef* xref = pPDFDocument->getXRef();

	Object oNode, oType;
	if (!xref->fetch(nRefNum, FetchGen(xref, nRefNum), &oNode)->isDict() || !oNode.dictLookup("Subtype", &oType)->isName("Widget"))
	{
		oNode.free(); oType.free();
		return;
	}
	oType.free();

	for (int nDepth = 0; nDepth < 32; ++nDepth)
	{
		Object oParentRef, oParent, oKids;
		if (!oNode.dictLookupNF("Parent", &oParentRef)->isRef() || !oParentRef.fetch(xref, &oParent)->isDict() ||
			!oParent.dictLookup("Kids", &oKids)->isArray())
		{
			oParentRef.free(); oParent.free(); oKids.free();
			break;
		}

		bool bLive = false;
		for (int i = 0; i < oKids.arrayGetLength() && !bLive; ++i)
		{
			Object oKid;
			bLive = !oKids.arrayGetNF(i, &oKid)->isRef() || !pManager->m_setDeletedIDs.count(oKid.getRefNum() + nStartRefID);
			oKid.free();
		}
		oKids.free();
		if (bLive)
		{
			oParentRef.free(); oParent.free();
			break;
		}

		int nParentID = oParentRef.getRefNum() + nStartRefID;
		oParentRef.free();
		pManager->m_setDeletedIDs.insert(nParentID);
		PdfWriter::CObjectBase* pObj = pManager->GetObj(nParentID);
		if (pObj && pObj->GetType() == PdfWriter::object_type_DICT)
		{
			RemoveFromParentKids((PdfWriter::CDictObject*)pObj);
			RemoveObj(pManager, pDoc, pObj);
		}

		oNode.free();
		oNode = oParent;
	}
	oNode.free();
}
// WriteAppend: a field whose kids all left in this update (DeleteAnnot, RemovePageWidgets: setFields) leaves
// it, up the Parent chain: out of Fields, CO and its parent's Kids, its object freed - else Fields lists it with
// Kids [] and its V. A field the editor gave a new widget (EditWidgetParents) has a kid again and stays
void DeleteEmptyAppendFields(CObjectsManager* pManager, PdfWriter::CDocument* pDoc, const std::set<int>& setFields)
{
	PdfWriter::CDictObject* pAcroForm = pDoc->GetAcroForm();
	PdfWriter::CArrayObject* pFields = pAcroForm ? dynamic_cast<PdfWriter::CArrayObject*>(pAcroForm->Get("Fields")) : NULL;
	PdfWriter::CArrayObject* pCO = pAcroForm ? dynamic_cast<PdfWriter::CArrayObject*>(pAcroForm->Get("CO")) : NULL;
	for (int nID : setFields)
	{
		PdfWriter::CDictObject* pField = pDoc->GetParent(nID);
		for (int nDepth = 0; pField && nDepth < 32; ++nDepth)
		{
			PdfWriter::CArrayObject* pKids = dynamic_cast<PdfWriter::CArrayObject*>(pField->Get("Kids"));
			bool bEmpty = pKids != NULL;
			for (int i = 0; bEmpty && i < pKids->GetCount(); ++i)
				bEmpty = !pKids->Get(i);
			// only a field copied into the update under its number (GetWidgetParent)
			int nFieldID = pField->GetObjId();
			if (!bEmpty || nFieldID <= 0 || pDoc->GetParent(nFieldID) != pField)
				break;

			PdfWriter::CObjectBase* pUp = pField->Get("Parent");
			PdfWriter::CDictObject* pUpField = pUp && pUp->GetType() == PdfWriter::object_type_DICT ? (PdfWriter::CDictObject*)pUp : NULL;
			if (pFields)
				RemoveArrayItem(pFields, NULL, nFieldID);
			if (pCO)
				RemoveArrayItem(pCO, NULL, nFieldID);
			if (pUpField)
				RemoveParentKid(pUpField, NULL, nFieldID);
			pManager->m_setDeletedIDs.insert(nFieldID);
			pDoc->RemoveObj(pField, false); // out of the parents EditWidgetParents walks
			PdfWriter::CXref::Free(pField);
			pField = pUpField;
		}
	}
}
// Split: a widget SplitSkipDict does not hold back - one with no P or no Type - comes in with its field's
// Kids even when its page is left out. Removed when no page of arrPages lists it in Annots.
// [nStartRefID, nEndRefID) - the manager IDs of the widgets checked
void RemoveOffPageWidgets(CObjectsManager* pManager, PdfWriter::CDocument* pDoc, int nStartRefID, int nEndRefID, const std::vector<PdfWriter::CPage*>& arrPages)
{
	std::set<PdfWriter::CObjectBase*> setOnPage;
	for (PdfWriter::CPage* pPage : arrPages)
	{
		PdfWriter::CObjectBase* pAnnotsObj = pPage ? pPage->Get("Annots") : NULL;
		if (!pAnnotsObj || pAnnotsObj->GetType() != PdfWriter::object_type_ARRAY)
			continue;
		PdfWriter::CArrayObject* pAnnots = (PdfWriter::CArrayObject*)pAnnotsObj;
		for (int i = 0; i < pAnnots->GetCount(); ++i)
			setOnPage.insert(pAnnots->Get(i));
	}

	std::vector<int> arrOff;
	const std::map<int, PdfWriter::CAnnotation*>& mAnnots = pDoc->GetAnnots();
	for (std::map<int, PdfWriter::CAnnotation*>::const_iterator it = mAnnots.begin(); it != mAnnots.end(); ++it)
	{
		if (it->first >= nStartRefID && it->first < nEndRefID && it->second->GetAnnotationType() == PdfWriter::AnnotWidget && !setOnPage.count(it->second))
			arrOff.push_back(it->first);
	}
	for (int nID : arrOff)
	{
		PdfWriter::CObjectBase* pObj = pManager->GetObj(nID);
		if (!pObj || pObj->GetType() != PdfWriter::object_type_DICT)
			continue;
		pManager->m_setDeletedIDs.insert(nID);
		RemoveFromParentKids((PdfWriter::CDictObject*)pObj);
		RemoveObj(pManager, pDoc, pObj);
	}
}
// Split: a field with no widget under it (its widgets split away, or deleted) leaves the part, its V with it.
// false - pNode has none: the caller removes it
bool PruneSplitFields(CObjectsManager* pManager, PdfWriter::CDocument* pDoc, PdfWriter::CObjectBase* pNode, std::set<PdfWriter::CObjectBase*>& setSeen, int nDepth)
{
	// a widget, or anything that is not a field dict, stays
	if (!pNode || pNode->GetType() != PdfWriter::object_type_DICT || ((PdfWriter::CDictObject*)pNode)->GetDictType() != PdfWriter::dict_type_UNKNOWN)
		return true;
	// a loop in the field tree is left as it is
	if (nDepth > 50 || !setSeen.insert(pNode).second)
		return true;
	PdfWriter::CObjectBase* pKidsObj = ((PdfWriter::CDictObject*)pNode)->Get("Kids");
	if (!pKidsObj || pKidsObj->GetType() != PdfWriter::object_type_ARRAY)
		return true;

	PdfWriter::CArrayObject* pKids = (PdfWriter::CArrayObject*)pKidsObj;
	bool bLive = false;
	for (int i = 0; i < pKids->GetCount(); ++i)
	{
		PdfWriter::CObjectBase* pKid = pKids->Get(i);
		if (!pKid)
			continue; // deleted
		if (PruneSplitFields(pManager, pDoc, pKid, setSeen, nDepth + 1))
		{
			bLive = true;
			continue;
		}
		RemoveArrayItem(pKids, pKid, 0);
		RemoveObj(pManager, pDoc, pKid);
		--i;
	}
	return bLive;
}
void PruneSplitFields(CObjectsManager* pManager, PdfWriter::CDocument* pDoc)
{
	PdfWriter::CDictObject* pAcroForm = pDoc->GetAcroForm();
	PdfWriter::CObjectBase* pFieldsObj = pAcroForm ? pAcroForm->Get("Fields") : NULL;
	if (!pFieldsObj || pFieldsObj->GetType() != PdfWriter::object_type_ARRAY)
		return;

	PdfWriter::CArrayObject* pFields = (PdfWriter::CArrayObject*)pFieldsObj;
	std::set<PdfWriter::CObjectBase*> setSeen;
	for (int i = 0; i < pFields->GetCount(); ++i)
	{
		PdfWriter::CObjectBase* pField = pFields->Get(i);
		if (!pField || PruneSplitFields(pManager, pDoc, pField, setSeen, 0))
			continue;
		RemoveArrayItem(pFields, pField, 0);
		RemoveObj(pManager, pDoc, pField);
		--i;
	}
}
PdfWriter::CDictObject* GetRebuildParent(CPdfReader* pReader, CObjectsManager* pManager, PdfWriter::CDocument* pDoc, int nID);
// A button kid that is on leaves its field: true when the field's V names the kid's state and no
// other kid the reader lists has that state on - V is Off then. pParentID - the field's manager ID
bool IsValueOfDeletedKid(CPdfReader* pReader, CObjectsManager* pManager, int nID, int* pParentID = NULL)
{
	PDFDoc* pPDFDocument = NULL;
	int nStartRefID = 0;
	int nRefNum = pReader->FindRefNum(nID, &pPDFDocument, &nStartRefID);
	if (nRefNum < 0 || !pPDFDocument)
		return false;
	XRef* xref = pPDFDocument->getXRef();

	CObjectGuard oKid, oAS, oParentRef, oParent, oV, oKids;
	if (!xref->fetch(nRefNum, FetchGen(xref, nRefNum), oKid.get())->isDict() || !oKid->dictLookup("AS", oAS.get())->isName() || oAS->isName("Off") ||
		!oKid->dictLookupNF("Parent", oParentRef.get())->isRef() || !oParentRef->fetch(xref, oParent.get())->isDict() ||
		!oParent->dictLookup("V", oV.get())->isName(oAS->getName()))
		return false;
	if (oParent->dictLookup("Kids", oKids.get())->isArray())
	{
		for (int i = 0; i < oKids->arrayGetLength(); ++i)
		{
			CObjectGuard oRef, oOther, oOtherAS;
			if (!oKids->arrayGetNF(i, oRef.get())->isRef() || oRef->getRefNum() == nRefNum || pManager->m_setDeletedIDs.count(oRef->getRefNum() + nStartRefID))
				continue;
			if (oRef->fetch(xref, oOther.get())->isDict() && oOther->dictLookup("AS", oOtherAS.get())->isName(oAS->getName()))
				return false;
		}
	}
	if (pParentID)
		*pParentID = oParentRef->getRefNum() + nStartRefID;
	return true;
}
}
bool CPdfEditor::DeleteAnnot(int nID, Object* oAnnots)
{
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	if (!pDoc)
		return false;

	// Marked deleted when converted (any mode; in WriteAppend a merged file's annotation), and always in
	// Split and WriteNew, which convert page by page: a later reference must not convert it again
	bool bRebuild = m_nMode == Mode::Split || m_nMode == Mode::WriteNew;
	PdfWriter::CObjectBase* pObj = m_mObjManager.GetObj(nID);
	if (pObj || bRebuild)
		m_mObjManager.m_setDeletedIDs.insert(nID);

	if (pObj)
	{
		if (pObj->GetType() == PdfWriter::object_type_DICT)
		{
			PdfWriter::CObjectBase* pParentObj = ((PdfWriter::CDictObject*)pObj)->Get("Parent");
			if (pParentObj && pParentObj->GetType() == PdfWriter::object_type_DICT && IsValueOfDeletedKid(m_pReader, &m_mObjManager, nID))
				((PdfWriter::CDictObject*)pParentObj)->Add("V", "Off");
			// before the Popup, whose cascade may free the annotation (Popup /Parent)
			RemoveFromParentKids((PdfWriter::CDictObject*)pObj);
			PdfWriter::CObjectBase* pObjR = ((PdfWriter::CDictObject*)pObj)->Get("Popup");
			if (pObjR)
			{
				int nPopupID = m_mObjManager.FindObj(pObjR);
				if (nPopupID >= 0)
					m_mObjManager.m_setDeletedIDs.insert(nPopupID);
				RemoveObj(&m_mObjManager, pDoc, pObjR);
			}
		}

		// freed already by the Popup's cascade: the manager no longer lists it
		if (m_mObjManager.GetObj(nID) == pObj)
			RemoveObj(&m_mObjManager, pDoc, pObj);
		DeleteEmptyParents(m_pReader, &m_mObjManager, pDoc, nID);
		return true;
	}
	if (bRebuild)
	{
		// Not converted yet: its Popup is in the page's Annots too and would be converted alone
		PDFDoc* pPDFDocument = NULL;
		int nStartRefID = 0;
		int nRefNum = m_pReader->FindRefNum(nID, &pPDFDocument, &nStartRefID);
		if (nRefNum >= 0 && pPDFDocument)
		{
			XRef* xref = pPDFDocument->getXRef();
			CObjectGuard oAnnot, oPopupRef;
			if (xref->fetch(nRefNum, FetchGen(xref, nRefNum), oAnnot.get())->isDict() && oAnnot->dictLookupNF("Popup", oPopupRef.get())->isRef())
				m_mObjManager.m_setDeletedIDs.insert(oPopupRef->getRefNum() + nStartRefID);
		}
		DeleteEmptyParents(m_pReader, &m_mObjManager, pDoc, nID);
		// WriteNew converts the field from the reader with its V; a split part has it already, or not at all
		int nParentID = 0;
		if (IsValueOfDeletedKid(m_pReader, &m_mObjManager, nID, &nParentID))
		{
			PdfWriter::CObjectBase* pParent = m_nMode == Mode::WriteNew ? GetRebuildParent(m_pReader, &m_mObjManager, pDoc, nParentID) : m_mObjManager.GetObj(nParentID);
			if (pParent && pParent->GetType() == PdfWriter::object_type_DICT)
				((PdfWriter::CDictObject*)pParent)->Add("V", "Off");
		}
		return true;
	}

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
						for (int nDepth = 0; oParent.isDict() && nDepth < 50; ++nDepth) // a broken file may loop the Parent chain
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
							for (int nDepth = 0; oParent.isDict() && nDepth < 50; ++nDepth) // a broken file may loop the Parent chain
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
						if (!bPushButton)
						{
							oFT.free();
							bNeed = oAnnot.dictLookup("Opt", &oFT)->isArray() == gTrue;
							if (!bNeed)
							{
								Object oParent, oParent2;
								oAnnot.dictLookup("Parent", &oParent);
								for (int nDepth = 0; oParent.isDict() && nDepth < 50; ++nDepth) // a broken file may loop the Parent chain
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
							PdfWriter::CAnnotation* pAnnot = bNeed && EditAnnot(m_nEditPage, nID) ? pDoc->GetAnnot(nID) : NULL;
							if (pAnnot)
							{
								// restored below: EditAnnot of a kid on another page makes that page current
								PdfWriter::CPage* pPage = pDoc->GetCurPage();

								PdfWriter::CObjectBase* pObj = pAnnot->Get("Parent");
								PdfWriter::CDictObject* pParent = NULL;
								if (pObj && pObj->GetType() == PdfWriter::object_type_DICT)
									pParent = (PdfWriter::CDictObject*)pObj;
								PdfWriter::CArrayObject* pOpt = NULL, *pKids = NULL;
								// the Kids V's Opt index counts: before this kid leaves
								std::vector<int> arrVKidIDs;
								if (pParent)
								{
									pObj = pParent->Get("Kids");
									if (pObj && pObj->GetType() == PdfWriter::object_type_ARRAY)
										pKids = (PdfWriter::CArrayObject*)pObj;
									for (int i = 0; pKids && i < pKids->GetCount(); ++i)
									{
										PdfWriter::CObjectBase* pItem = pKids->Get(i);
										arrVKidIDs.push_back(pItem ? (int)pItem->GetObjId() : 0);
									}
									pObj = pParent->Get("Opt");
									if (pObj && pObj->GetType() == PdfWriter::object_type_ARRAY)
										pOpt = (PdfWriter::CArrayObject*)pObj;
									// only the proxy goes: pAnnot is still in the page's Annots, its own xref and the document's annotations
									RemoveParentKid(pParent, pAnnot, 0);
								}
								// Fields may list widgets (CDocument::DeleteAnnot takes them out too)
								PdfWriter::CDictObject* pAcroForm = pDoc->GetAcroForm();
								pObj = pAcroForm ? pAcroForm->Get("Fields") : NULL;
								if (pObj && pObj->GetType() == PdfWriter::object_type_ARRAY)
									RemoveArrayItem((PdfWriter::CArrayObject*)pObj, NULL, nID);

								// The kids left are renamed after their new Opt indexes, the field's value follows the kid that is on
								if (pKids && pOpt && pKids->GetCount() == pOpt->GetCount())
								{
									std::vector<PdfWriter::CCheckBoxWidget*> arrKids;
									for (int i = 0; i < pKids->GetCount(); ++i)
									{
										pObj = pKids->Get(i);
										PdfWriter::CCheckBoxWidget* pKidAnnot = NULL;
										Object oKid, oSubtype, oPageRef;
										if (pObj && xref->fetch(pObj->GetObjId(), pObj->GetGenNo(), &oKid)->isDict("Annot") && oKid.dictLookup("Subtype", &oSubtype)->isName("Widget") &&
											oKid.dictLookupNF("P", &oPageRef)->isRef())
										{
											int nPage = pPDFDocument->findPage(oPageRef.getRefNum(), oPageRef.getRefGen()) - 1;
											int nObjId = pObj->GetObjId();
											if (nPage >= 0 && EditAnnot(nPage, nObjId))
												pKidAnnot = dynamic_cast<PdfWriter::CCheckBoxWidget*>(pDoc->GetAnnot(nObjId));
										}
										oKid.free(); oSubtype.free(); oPageRef.free();
										arrKids.push_back(pKidAnnot);
									}
									pObj = pParent->Get("V");
									int nV = OptIndexOfV(pObj && pObj->GetType() == PdfWriter::object_type_NAME ? ((PdfWriter::CNameObject*)pObj)->Get() : "");
									PdfWriter::CObjectBase* pVKid = nV >= 0 && nV < (int)arrVKidIDs.size() && arrVKidIDs[nV] > 0 ? pDoc->GetAnnot(arrVKidIDs[nV]) : NULL;
									RenameKids(pParent, pOpt, arrKids, pVKid);
								}
								pDoc->SetCurPage(pPage);

								// Out of the page's Annots and the document's annotations, which EditWidgetParents walks.
								// Not freed: EditAnnot gave it an xref entry in use, a free entry would duplicate it.
								// Unreferenced, it stays in the update, like an edited page that is deleted (CDocument::DeletePage)
								Object oPopupRef;
								if (oAnnot.dictLookupNF("Popup", &oPopupRef)->isRef())
								{
									PdfWriter::CAnnotation* pPopup = pDoc->GetAnnot(oPopupRef.getRefNum());
									if (pPopup)
									{
										if (pPage)
											pPage->DeleteAnnotation(oPopupRef.getRefNum());
										pDoc->RemoveObj(pPopup);
									}
									else
										pDoc->DeleteAnnot(oPopupRef.getRefNum(), oPopupRef.getRefGen());
								}
								oPopupRef.free();
								bRes = pPage && pPage->DeleteAnnotation(nID);
								// In WriteAppend pAnnot is in its own xref, not the document's one: RemoveObj does not delete it
								pDoc->RemoveObj(pAnnot);
							}
							else
								bNeed = false; // not edited: deleted like any other widget
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
			{
				// The widget's field is copied now and written without it: a copy made later (EditWidgets,
				// EditAnnot of another kid) would still list the freed widget
				Object oParentRef, oSubtype;
				if (oAnnot.isDict() && oAnnot.dictLookup("Subtype", &oSubtype)->isName("Widget") && oAnnot.dictLookupNF("Parent", &oParentRef)->isRef())
				{
					PdfWriter::CDictObject* pParent = GetWidgetParent(pPDFDocument, pDoc, &oParentRef, 0);
					if (pParent)
					{
						RemoveParentKid(pParent, NULL, nID);
						if (IsValueOfDeletedKid(m_pReader, &m_mObjManager, nID))
							pParent->Add("V", "Off");
						m_setLostKidFields.insert(oParentRef.getRefNum());
					}
				}
				oParentRef.free(); oSubtype.free();
				bRes = pDoc->DeleteAnnot(oAnnotRef.getRefNum(), oAnnotRef.getRefGen());
				// a CO entry of it is left out (EditWidgets)
				if (bRes)
					m_mObjManager.m_setDeletedIDs.insert(nID);
			}
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
void CPdfEditor::CheckMode()
{
	if (m_nMode == Mode::Unknown)
		IncrementalUpdates();
}
namespace
{
// Split, WriteNew: field nID as the manager converted it (with a page, or here from the reader), registered
// with its ancestors and the fields under it - not a GetWidgetParent copy, which keeps the reader number.
// NULL - a new field, a widget, a deleted field
PdfWriter::CDictObject* GetRebuildParent(CPdfReader* pReader, CObjectsManager* pManager, PdfWriter::CDocument* pDoc, int nID)
{
	PdfWriter::CDictObject* pParent = pDoc->GetParent(nID);
	if (pParent)
		return pParent;
	// Deleted with all its kids - not converted again
	if (pManager->m_setDeletedIDs.count(nID))
		return NULL;

	PdfWriter::CObjectBase* pObj = pManager->GetObj(nID);
	if (!pObj)
	{
		PDFDoc* pPDFDocument = NULL;
		int nStartRefID = 0;
		int nRefNum = pReader->FindRefNum(nID, &pPDFDocument, &nStartRefID);
		if (nRefNum < 0 || !pPDFDocument)
			return NULL; // a new field
		XRef* xref = pPDFDocument->getXRef();

		Object oRef, oField, oType;
		oRef.initRef(nRefNum, FetchGen(xref, nRefNum));
		// a field, not a widget: a widget is converted with its page
		bool bField = oRef.fetch(xref, &oField)->isDict() && !oField.dictLookup("Subtype", &oType)->isName("Widget");
		oField.free(); oType.free();
		// registered under nID; WriteNew adds the kids of the pages converted later in SyncAcroFormFromReader
		if (bField)
			pObj = DictToCDictObjectUnified(&oRef, SDictConvertContext(pDoc, xref, pManager, nStartRefID));
		oRef.free();
	}
	if (!pObj || pObj->GetType() != PdfWriter::object_type_DICT || ((PdfWriter::CDictObject*)pObj)->GetDictType() != PdfWriter::dict_type_UNKNOWN)
		return NULL;

	// the field, its ancestors and the fields under it
	AddWidgetParent(pDoc, pManager, pObj);
	return pDoc->GetParent(nID);
}
// The parent of a field: as the JS sends it, as converted, or as the reader has it. 0 - a root
int GetFieldParentID(CPdfReader* pReader, CObjectsManager* pManager, const std::map<int, int>& mJSParent, int nID)
{
	std::map<int, int>::const_iterator it = mJSParent.find(nID);
	if (it != mJSParent.end())
		return it->second;

	PdfWriter::CObjectBase* pObj = pManager->GetObj(nID);
	if (pObj && pObj->GetType() == PdfWriter::object_type_DICT)
	{
		PdfWriter::CObjectBase* pParent = ((PdfWriter::CDictObject*)pObj)->Get("Parent");
		int nParentID = pParent ? pManager->FindObj(pParent) : -1;
		return nParentID > 0 ? nParentID : 0;
	}

	PDFDoc* pPDFDocument = NULL;
	int nStartRefID = 0;
	int nRefNum = pReader->FindRefNum(nID, &pPDFDocument, &nStartRefID);
	if (nRefNum < 0 || !pPDFDocument)
		return 0;
	XRef* xref = pPDFDocument->getXRef();

	int nParentID = 0;
	Object oNode, oParentRef;
	if (xref->fetch(nRefNum, FetchGen(xref, nRefNum), &oNode)->isDict() && oNode.dictLookupNF("Parent", &oParentRef)->isRef())
		nParentID = oParentRef.getRefNum() + nStartRefID;
	oNode.free(); oParentRef.free();
	return nParentID;
}
// Split: the fields a widget of the part lies under, up to the root. The editor sends the changed
// fields of all pages; a field not in this set is not in the part
std::set<int> GetSplitLiveFields(CPdfReader* pReader, CObjectsManager* pManager, PdfWriter::CDocument* pDoc, const std::vector<CWidgetsInfo::CParent*>& arrParents)
{
	std::map<int, int> mJSParent;
	for (CWidgetsInfo::CParent* pParent : arrParents)
		mJSParent[pParent->nID] = (pParent->nFlags & (1 << 4)) ? pParent->nParentID : 0;

	std::set<int> setLive;
	const std::map<int, PdfWriter::CAnnotation*>& mAnnots = pDoc->GetAnnots();
	for (std::map<int, PdfWriter::CAnnotation*>::const_iterator it = mAnnots.begin(); it != mAnnots.end(); ++it)
	{
		if (!it->second || it->second->GetAnnotationType() != PdfWriter::AnnotWidget)
			continue;
		PdfWriter::CWidgetAnnotation* pWidget = (PdfWriter::CWidgetAnnotation*)it->second;
		// written by the JS, or converted with a page split out
		int nID = pWidget->GetParentID();
		if (nID <= 0)
		{
			PdfWriter::CObjectBase* pParent = pWidget->Get("Parent");
			nID = pParent ? pManager->FindObj(pParent) : -1;
		}
		for (int nDepth = 0; nID > 0 && nDepth < 32 && setLive.insert(nID).second; ++nDepth)
			nID = GetFieldParentID(pReader, pManager, mJSParent, nID);
	}
	return setLive;
}
// The pages (origin indexes) of the widget nID, or of the widgets under the field nID, deleted ones aside
void GetWidgetPages(CPdfReader* pReader, CObjectsManager* pManager, int nID, std::set<int>& setPages)
{
	PDFDoc* pPDFDocument = NULL;
	int nStartRefID = 0;
	int nRefNum = pReader->FindRefNum(nID, &pPDFDocument, &nStartRefID);
	if (nRefNum < 0 || !pPDFDocument)
		return;
	XRef* xref = pPDFDocument->getXRef();
	int nPagesBefore = pReader->GetNumPagesBefore(pPDFDocument);

	std::set<int> setSeen;
	std::vector< std::pair<Ref, int> > arrStack;
	Ref oRoot = { nRefNum, FetchGen(xref, nRefNum) };
	arrStack.push_back(std::make_pair(oRoot, 0));
	while (!arrStack.empty())
	{
		Ref oRef = arrStack.back().first;
		int nDepth = arrStack.back().second;
		arrStack.pop_back();

		CObjectGuard oNode, oPageRef, oKids;
		if (!setSeen.insert(oRef.num).second || pManager->m_setDeletedIDs.count(oRef.num + nStartRefID) || !xref->fetch(oRef.num, oRef.gen, oNode.get())->isDict())
			continue;
		if (oNode->dictLookupNF("P", oPageRef.get())->isRef())
		{
			int nPage = pPDFDocument->findPage(oPageRef->getRefNum(), oPageRef->getRefGen());
			if (nPage > 0)
				setPages.insert(nPagesBefore + nPage - 1);
		}
		if (nDepth >= 32 || !oNode->dictLookup("Kids", oKids.get())->isArray())
			continue;
		for (int i = 0; i < oKids->arrayGetLength(); ++i)
		{
			CObjectGuard oKidRef;
			if (oKids->arrayGetNF(i, oKidRef.get())->isRef())
				arrStack.push_back(std::make_pair(oKidRef->getRef(), nDepth + 1));
		}
	}
}
// Kids of the field nID and of the fields under it: every converted kid, in the reader's order
void SyncFieldKidsOf(CPdfReader* pReader, CObjectsManager* pManager, int nID)
{
	PDFDoc* pPDFDocument = NULL;
	int nStartRefID = 0;
	int nRefNum = pReader->FindRefNum(nID, &pPDFDocument, &nStartRefID);
	if (nRefNum < 0 || !pPDFDocument)
		return;
	XRef* xref = pPDFDocument->getXRef();

	CObjectGuard oNode;
	std::set<int> setVisited;
	if (xref->fetch(nRefNum, FetchGen(xref, nRefNum), oNode.get())->isDict())
		AddMissingFieldKids(oNode.get(), nID, xref, *pManager, nStartRefID, 0, setVisited);
}
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

			int nPage = oInfo.GetPage(); // actual page number
			PdfWriter::CPage* pPage = pDoc->GetPage(nPage);
			if (!pPage)
			{
				EditPage(nPage, false, true);
				pPage = pDoc->GetPage(nPage);
			}
			if (!pPage)
				pPage = pDoc->GetEditPage(nPage);
			if (!pPage)
				continue;

			m_pWriter->SetPage(pPage);
			pDoc->SetCurPage(pPage);

			m_pWriter->AddAnnotField(NULL, command, true);
		}
	}

	const std::vector< std::pair<int, int> >& arrCO = pFieldInfo->GetCO();
	const std::vector<CWidgetsInfo::CParent*>& arrParents = pFieldInfo->GetParents();
	// Split and WriteNew renumber the reader: no raw reader reference (ChangeCO),
	// no parent copied with its reader number (GetWidgetParent)
	bool bRebuild = m_nMode == Mode::Split || m_nMode == Mode::WriteNew;
	std::set<int> setSplitLive;
	if (m_nMode == Mode::Split && (!arrCO.empty() || !arrParents.empty()))
		setSplitLive = GetSplitLiveFields(m_pReader, &m_mObjManager, pDoc, arrParents);
	// WriteNew converts the pages not edited only in Close, after this command. The pages of a CO widget
	// and of a changed field's widgets are converted here: EditCO lists the widget in the editor's order,
	// EditWidgetParents gets every kid in Kids (Opt pairs with it by index)
	std::map<int, int> mFakePages; // origin index -> page index, the pages not converted yet
	bool bFakePages = false;
	auto EditWidgetPages = [this, pDoc, &mFakePages, &bFakePages](int nID)
	{
		std::set<int> setPages;
		GetWidgetPages(m_pReader, &m_mObjManager, nID, setPages);
		if (setPages.empty())
			return;
		// listed once: CPageTree::GetObj walks the tree
		if (!bFakePages)
		{
			bFakePages = true;
			PdfWriter::CPageTree* pPageTree = pDoc->GetPageTree();
			for (int nPage = 0; nPage < (int)pPageTree->GetCount(); ++nPage)
			{
				PdfWriter::CFakePage* pFakePage = dynamic_cast<PdfWriter::CFakePage*>(pPageTree->GetObj(nPage));
				if (pFakePage)
					mFakePages.emplace(pFakePage->GetOriginIndex(), nPage);
			}
		}
		for (int nOrigin : setPages)
		{
			std::map<int, int>::iterator it = mFakePages.find(nOrigin);
			if (it == mFakePages.end())
				continue;
			int nPage = it->second;
			mFakePages.erase(it);
			// the converted page takes the place of the fake one, the indexes stay
			EditPage(nPage, false, true);
		}
	};

	for (int i = 0; i < arrCO.size(); ++i)
	{
		int nObjNum = arrCO[i].first;
		if (pDoc->GetParent(nObjNum))
			continue;
		if (pDoc->GetAnnot(nObjNum))
			continue;

		if (bRebuild)
		{
			// EditCO finds a field registered as a parent, a widget among the annotations,
			// and leaves out the rest: its gen stays -1
			if (m_mObjManager.m_setDeletedIDs.count(nObjNum) || (m_nMode == Mode::Split && !setSplitLive.count(nObjNum)))
				continue;
			// a widget (a field of one widget) of a page not edited
			if (!GetRebuildParent(m_pReader, &m_mObjManager, pDoc, nObjNum) && m_nMode == Mode::WriteNew)
				EditWidgetPages(nObjNum);
			continue;
		}

		// a widget deleted in this update is left out (EditCO: its gen stays -1)
		if (m_mObjManager.m_setDeletedIDs.count(nObjNum))
			continue;
		PDFDoc* pPDFDocument = NULL;
		int nStartRefID = 0;
		int nRefID = m_pReader->FindRefNum(nObjNum, &pPDFDocument, &nStartRefID);
		if (nRefID < 0)
			continue;

		XRefEntry* pEntry = pPDFDocument->getXRef()->getEntry(nRefID);
		pFieldInfo->ChangeCO(i, nRefID, pEntry->type == xrefEntryCompressed ? 0 : pEntry->gen);
	}

	for (CWidgetsInfo::CParent* pParent : arrParents)
	{
		if (bRebuild)
		{
			// The changed field and its parent are registered here for EditWidgetParents.
			// Split: one no widget of the part lies under is not, and EditWidgetParents skips it
			int arrID[2] = { pParent->nID, (pParent->nFlags & (1 << 4)) ? pParent->nParentID : -1 };
			for (int nID : arrID)
			{
				if (nID < 0 || (m_nMode == Mode::Split && !setSplitLive.count(nID)))
					continue;
				// The changed field gets its kids in the reader's order and Opt cut to them: WriteNew all of them
				// (their pages converted here), Split the ones of the part (a field converted here, after
				// SplitPages synced the others)
				bool bSync = nID == pParent->nID && !m_mObjManager.m_setDeletedIDs.count(nID);
				if (bSync && m_nMode == Mode::WriteNew)
					EditWidgetPages(nID);
				if (GetRebuildParent(m_pReader, &m_mObjManager, pDoc, nID))
				{
					if (bSync)
						SyncFieldKidsOf(m_pReader, &m_mObjManager, nID);
				}
				// a new field, or a deleted one the JS has a new widget under: it keeps its name only,
				// nothing of what was deleted (a redacted value)
				else if (m_pReader->FindRefNum(nID, NULL, NULL) < 0 || m_mObjManager.m_setDeletedIDs.count(nID))
				{
					PdfWriter::CDictObject* pNew = pDoc->CreateParent(nID);
					PDFDoc* pPDFDocument = NULL;
					int nRefNum = m_pReader->FindRefNum(nID, &pPDFDocument, NULL);
					CObjectGuard oField, oT;
					if (nRefNum >= 0 && pPDFDocument && pPDFDocument->getXRef()->fetch(nRefNum, FetchGen(pPDFDocument->getXRef(), nRefNum), oField.get())->isDict() &&
						oField->dictLookup("T", oT.get())->isString())
						pNew->Add("T", DictToCDictObjectUnified(oT.get(), SDictConvertContext()));
				}
			}
			continue;
		}

		PdfWriter::CDictObject* pDParent = pDoc->GetParent(pParent->nID);
		if (pDParent)
			continue;

		PDFDoc* pPDFDocument = NULL;
		int nStartRefID = 0;
		int nRefID = m_pReader->FindRefNum(pParent->nID, &pPDFDocument, &nStartRefID);
		if (nRefID < 0)
			continue;
		// a merged file's field is converted (renumbered) with its pages, not copied under its reader number
		if (nStartRefID)
		{
			GetRebuildParent(m_pReader, &m_mObjManager, pDoc, pParent->nID);
			continue;
		}

		XRefEntry* pEntry = pPDFDocument->getXRef()->getEntry(nRefID);
		Object oParentRef;
		oParentRef.initRef(nRefID, pEntry->type == xrefEntryCompressed ? 0 : pEntry->gen);
		GetWidgetParent(pPDFDocument, pDoc, &oParentRef, nStartRefID);
		// TODO re-set children
		oParentRef.free();
	}
	// A widget the editor adds to a field it does not send as changed names the field only by its ParentID,
	// and EditWidgetParents links only the fields sent: such a field is linked here
	std::set<int> setSent, setOrphan;
	for (CWidgetsInfo::CParent* pParent : arrParents)
		setSent.insert(pParent->nID);
	const std::map<int, PdfWriter::CAnnotation*>& mAnnots = pDoc->GetAnnots();
	for (std::map<int, PdfWriter::CAnnotation*>::const_iterator it = mAnnots.begin(); it != mAnnots.end(); ++it)
	{
		if (!it->second || it->second->GetAnnotationType() != PdfWriter::AnnotWidget)
			continue;
		PdfWriter::CWidgetAnnotation* pWidget = (PdfWriter::CWidgetAnnotation*)it->second;
		int nParentID = pWidget->GetParentID();
		if (nParentID > 0 && !pWidget->Get("Parent") && !setSent.count(nParentID) && !m_mObjManager.m_setDeletedIDs.count(nParentID))
			setOrphan.insert(nParentID);
	}
	for (int nID : setOrphan)
	{
		PdfWriter::CDictObject* pParent = pDoc->GetParent(nID);
		if (!pParent && bRebuild)
			pParent = GetRebuildParent(m_pReader, &m_mObjManager, pDoc, nID);
		else if (!pParent)
		{
			PDFDoc* pPDFDocument = NULL;
			int nStartRefID = 0;
			int nRefID = m_pReader->FindRefNum(nID, &pPDFDocument, &nStartRefID);
			if (nRefID < 0 || !pPDFDocument)
				continue;
			// a merged file's field is converted (renumbered) with its pages: a copy would keep its reader numbers
			if (nStartRefID)
				pParent = GetRebuildParent(m_pReader, &m_mObjManager, pDoc, nID);
			else
			{
				Object oParentRef;
				oParentRef.initRef(nRefID, FetchGen(pPDFDocument->getXRef(), nRefID));
				pParent = GetWidgetParent(pPDFDocument, pDoc, &oParentRef, nStartRefID);
				oParentRef.free();
			}
		}
		// Opt of a checkbox or radio group pairs with Kids by index: the editor sends no entry for the kid
		if (pParent && !HasButtonOpt(pParent, NULL))
			pDoc->SetParentKids(nID);
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
				// DeleteAnnot takes the manager ID, the page may come from a merged file
				DeleteAnnot(oAnnotRef.getRefNum() + nStartRefID, &oAnnots);
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
		std::set<int> arrOverlapHiddenTextIndices;
		std::vector<int> arrOverlapFormTextCounts, arrOverlapFormGraphicsCounts;
		std::vector<PdfWriter::SOverlapGraphicsResult> arrOverlapGraphicsResults;
		bool bTextOnly = (m_nRedactInfo & (1 << 6)) != 0;
		if (bTextOnly)
		{
			PdfWriter::OverlapOutputDev oOverlapOut(bTextOnly);
			oOverlapOut.NewPDF(pPDFDocument->getXRef());

			Object oContents;
			pPage->getContents(&oContents);
			PDFRectangle* box = pPage->getMediaBox();
			Gfx* gfx = new Gfx(pPDFDocument, &oOverlapOut, m_nEditPage, pPage->getResourceDict(), 72.0, 72.0, box, NULL, 0);
			gfx->saveState();
			gfx->display(&oContents);
			gfx->endOfPage();
			oContents.free();
			RELEASEOBJECT(gfx);

			arrOverlapHiddenTextIndices  = oOverlapOut.GetHiddenText();
			arrOverlapFormTextCounts     = oOverlapOut.GetFormTextCounts();
			arrOverlapFormGraphicsCounts = oOverlapOut.GetFormGraphicsCounts();
			arrOverlapGraphicsResults    = oOverlapOut.GetHiddenGraphics();
		}

		PdfWriter::RedactOutputDev oRedactOut(m_pWriter, &m_mObjManager, nStartRefID, m_nRedactInfo);
		oRedactOut.NewPDF(pPDFDocument->getXRef());
		oRedactOut.SetRedact(arrAllQuads);
		oRedactOut.SetOverlapInfo(arrOverlapHiddenTextIndices, &arrOverlapFormTextCounts, &arrOverlapFormGraphicsCounts, std::move(arrOverlapGraphicsResults));

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
void CPdfEditor::RedactMeta(IAdvancedCommand* pCommand, const std::vector<double>& arrRedact)
{
	CRedactMeta* pRedactMeta = (CRedactMeta*)pCommand;
	std::vector<int> arrWidget = pRedactMeta->GetWidgetID();

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

			int nID = oInfo.GetID();
			std::vector<int>::iterator it = std::find(arrWidget.begin(), arrWidget.end(), nID);
			if (it == arrWidget.end())
				continue;

			m_pWriter->AddAnnotField(NULL, command, true);
		}
	}

	CloseWriteNew_FlushAnnotations(m_pReader, &m_mObjManager, m_pWriter->GetDocument(), m_pWriter, m_nRedactInfo, pCommand, arrRedact);
}
void CPdfEditor::FinalizeWidgetRedactStamps()
{
	CloseWriteNew_FinalizeWidgetStamps(m_pReader, &m_mObjManager, m_pWriter->GetDocument(), m_pWriter, m_nRedactInfo);
}
