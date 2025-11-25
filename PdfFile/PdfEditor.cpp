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
#include "PdfEditor.h"

#include "../DesktopEditor/common/Path.h"
#include "../DesktopEditor/graphics/commands/AnnotField.h"

#include "SrcReader/Adaptors.h"
#include "SrcReader/PdfAnnot.h"
#include "lib/xpdf/PDFDoc.h"
#include "lib/xpdf/AcroForm.h"
#include "lib/xpdf/TextString.h"
#include "lib/xpdf/Lexer.h"
#include "lib/xpdf/Parser.h"
#include "lib/xpdf/Outline.h"
#include "lib/xpdf/Link.h"
#include "lib/xpdf/Stream.h"

#include "SrcWriter/Catalog.h"
#include "SrcWriter/EncryptDictionary.h"
#include "SrcWriter/Info.h"
#include "SrcWriter/ResourcesDictionary.h"
#include "SrcWriter/Streams.h"
#include "SrcWriter/Destination.h"
#include "SrcWriter/Outline.h"
#include "SrcWriter/GState.h"
#include "SrcWriter/RedactOutputDev.h"

#define AddToObject(oVal)\
{\
	if (pObj->GetType() == PdfWriter::object_type_DICT)\
	((PdfWriter::CDictObject*)pObj)->Add(sKey, oVal);\
	else if (pObj->GetType() == PdfWriter::object_type_ARRAY)\
	((PdfWriter::CArrayObject*)pObj)->Add(oVal);\
}

void DictToCDictObject(Object* obj, PdfWriter::CObjectBase* pObj, const std::string& sKey, bool bMakeBinary = false)
{
	Object oTemp;
	switch (obj->getType())
	{
	case objBool:
	{
		bool b = obj->getBool() == gTrue;
		AddToObject(b)
		break;
	}
	case objInt:
	{
		AddToObject(obj->getInt())
		break;
	}
	case objReal:
	{
		AddToObject(obj->getReal())
		break;
	}
	case objString:
	{
		GString* str = obj->getString();
		if (str->isBinary() || bMakeBinary)
		{
			int nLength = str->getLength();
			BYTE* arrId = new BYTE[nLength];
			for (int nIndex = 0; nIndex < nLength; ++nIndex)
				arrId[nIndex] = str->getChar(nIndex);
			AddToObject(new PdfWriter::CBinaryObject(arrId, nLength, false));
		}
		else
		{
			TextString* s = new TextString(str);
			std::string sValue = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
			AddToObject(new PdfWriter::CStringObject(sValue.c_str(), !s->isPDFDocEncoding()))
			delete s;
		}
		break;
	}
	case objName:
	{
		AddToObject(obj->getName())
		break;
	}
	case objNull:
	{
		AddToObject(new PdfWriter::CNullObject())
		break;
	}
	case objArray:
	{
		PdfWriter::CArrayObject* pArray = new PdfWriter::CArrayObject();
		AddToObject(pArray)
		for (int nIndex = 0; nIndex < obj->arrayGetLength(); ++nIndex)
		{
			obj->arrayGetNF(nIndex, &oTemp);
			DictToCDictObject(&oTemp, pArray, "");
			oTemp.free();
		}
		break;
	}
	case objDict:
	{
		PdfWriter::CDictObject* pDict = new PdfWriter::CDictObject();
		AddToObject(pDict);
		for (int nIndex = 0; nIndex < obj->dictGetLength(); ++nIndex)
		{
			char* chKey = obj->dictGetKey(nIndex);
			obj->dictGetValNF(nIndex, &oTemp);
			DictToCDictObject(&oTemp, pDict, chKey);
			oTemp.free();
		}
		break;
	}
	case objRef:
	{
		PdfWriter::CObjectBase* pBase = new PdfWriter::CObjectBase();
		pBase->SetRef(obj->getRefNum(), obj->getRefGen());
		AddToObject(new PdfWriter::CProxyObject(pBase, true))
		break;
	}
	case objNone:
	{
		AddToObject("None")
		break;
	}
	case objStream:
	case objCmd:
	case objError:
	case objEOF:
		break;
	}
}
bool SplitSkipDict(Object* obj, CObjectsManager* pManager, int nStartRefID)
{
	Object oTemp, oTemp2;
	if (obj->dictLookup("Type", &oTemp)->isName("Page"))
	{
		oTemp.free();
		return true;
	}
	else if (oTemp.isName("Annot") && obj->dictLookupNF("P", &oTemp2)->isRef())
	{
		PdfWriter::CObjectBase* pObj = pManager->GetObj(oTemp2.getRefNum() + nStartRefID);
		if (!pObj)
		{
			oTemp.free(); oTemp2.free();
			return true;
		}
	}
	oTemp.free(); oTemp2.free();

	if (obj->dictLookup("S", &oTemp)->isName("GoTo"))
	{
		oTemp.free();
		if (obj->dictLookup("D", &oTemp)->isArray() && oTemp.arrayGetLength() > 1 && oTemp.arrayGetNF(0, &oTemp2)->isRef())
		{
			PdfWriter::CObjectBase* pObj = pManager->GetObj(oTemp2.getRefNum() + nStartRefID);
			if (!pObj)
			{
				oTemp.free(); oTemp2.free();
				return true;
			}
		}
		oTemp2.free();
	}
	oTemp.free();

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
PdfWriter::CObjectBase* DictToCDictObject2(Object* obj, PdfWriter::CDocument* pDoc, XRef* xref, CObjectsManager* pManager, int nStartRefID, int nAddObjToXRef = 0, bool bUndecodedStream = true, bool bMakeBinary = false)
{
	PdfWriter::CObjectBase* pBase = NULL;
	Object oTemp;
	switch (obj->getType())
	{
	case objBool:
	{
		pBase = new PdfWriter::CBoolObject(obj->getBool() == gTrue);
		break;
	}
	case objInt:
	{
		pBase = new PdfWriter::CNumberObject(obj->getInt());
		break;
	}
	case objReal:
	{
		pBase = new PdfWriter::CRealObject(obj->getReal());
		break;
	}
	case objString:
	{
		GString* str = obj->getString();
		if (str->isBinary() || bMakeBinary)
		{
			int nLength = str->getLength();
			BYTE* arrId = new BYTE[nLength];
			for (int nIndex = 0; nIndex < nLength; ++nIndex)
				arrId[nIndex] = str->getChar(nIndex);
			pBase = new PdfWriter::CBinaryObject(arrId, nLength, false);
		}
		else
		{
			TextString* s = new TextString(str);
			std::string sValue = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
			pBase = new PdfWriter::CStringObject(sValue.c_str(), !s->isPDFDocEncoding());
			delete s;
		}
		break;
	}
	case objName:
	{
		pBase = new PdfWriter::CNameObject(obj->getName());
		break;
	}
	case objNull:
	{
		pBase = new PdfWriter::CNullObject();
		break;
	}
	case objArray:
	{
		PdfWriter::CArrayObject* pArray = new PdfWriter::CArrayObject();
		if (nAddObjToXRef > 0)
		{
			pDoc->AddObject(pArray);
			pManager->AddObj(nAddObjToXRef + nStartRefID, pArray);
			nAddObjToXRef = 0;
		}
		for (int nIndex = 0; nIndex < obj->arrayGetLength(); ++nIndex)
		{
			obj->arrayGetNF(nIndex, &oTemp);
			pBase = DictToCDictObject2(&oTemp, pDoc, xref, pManager, nStartRefID, 0, bUndecodedStream);
			pArray->Add(pBase);
			oTemp.free();
		}
		pBase = pArray;
		break;
	}
	case objDict:
	{
		if (SplitSkipDict(obj, pManager, nStartRefID))
			return NULL;

		Object oType, oSubtype;
		PdfWriter::CDictObject* pDict = NULL;
		if (obj->dictLookup("Subtype", &oSubtype)->isName())
		{
			PdfWriter::CAnnotation* pAnnot = CreateAnnot(obj, &oSubtype, NULL);
			if (pAnnot)
			{
				pDoc->AddAnnotation(nAddObjToXRef + nStartRefID, pAnnot);
				pDict = pAnnot;
			}
		}
		oSubtype.free();

		if (obj->dictLookup("Type", &oType)->isName("ExtGState"))
		{
			PdfWriter::CExtGrState* pState = CreateExtGState(obj);
			if (pState)
			{
				pDoc->AddExtGState(pState);
				pDict = pState;
			}
		}
		oType.free();

		if (!pDict)
			pDict = new PdfWriter::CDictObject();
		if (nAddObjToXRef > 0)
		{
			pDoc->AddObject(pDict);
			pManager->AddObj(nAddObjToXRef + nStartRefID, pDict);
			nAddObjToXRef = 0;
		}
		for (int nIndex = 0; nIndex < obj->dictGetLength(); ++nIndex)
		{
			char* chKey = obj->dictGetKey(nIndex);
			obj->dictGetValNF(nIndex, &oTemp);
			pBase = DictToCDictObject2(&oTemp, pDoc, xref, pManager, nStartRefID, 0, bUndecodedStream);
			pDict->Add(chKey, pBase);
			oTemp.free();
		}
		pBase = pDict;
		break;
	}
	case objRef:
	{
		int nObjNum = obj->getRefNum();
		PdfWriter::CObjectBase* pObj = pManager->GetObj(nObjNum + nStartRefID);
		if (pObj)
		{
			pManager->IncRefCount(nObjNum + nStartRefID);
			return pObj;
		}

		obj->fetch(xref, &oTemp);
		pBase = DictToCDictObject2(&oTemp, pDoc, xref, pManager, nStartRefID, nObjNum, bUndecodedStream);
		oTemp.free();
		break;
	}
	case objNone:
	{
		pBase = new PdfWriter::CNameObject("None");
		break;
	}
	case objStream:
	{
		PdfWriter::CDictObject* pDict = new PdfWriter::CDictObject();
		if (nAddObjToXRef > 0)
		{
			pDoc->AddObject(pDict);
			pManager->AddObj(nAddObjToXRef + nStartRefID, pDict);
			nAddObjToXRef = 0;
		}

		Dict* pODict = obj->streamGetDict();
		int nLength = 0;
		for (int nIndex = 0; nIndex < pODict->getLength(); ++nIndex)
		{
			char* chKey = pODict->getKey(nIndex);
			if (strcmp("Length", chKey) == 0)
			{
				pODict->getVal(nIndex, &oTemp);
				nLength = oTemp.getNum();

				pBase = new PdfWriter::CNumberObject(nLength);
				pDoc->AddObject(pBase);
				pDict->Add(chKey, pBase);
				oTemp.free();
				continue;
			}
			pODict->getValNF(nIndex, &oTemp);
			pBase = DictToCDictObject2(&oTemp, pDoc, xref, pManager, nStartRefID);
			pDict->Add(chKey, pBase);
			oTemp.free();
		}
		pBase = pDict;

		PdfWriter::CStream* pStream = new PdfWriter::CMemoryStream(nLength);
		pDict->SetStream(pStream);
		Stream* pOStream = bUndecodedStream ? obj->getStream()->getUndecodedStream() : obj->getStream();
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
	if (nAddObjToXRef > 0)
	{
		pDoc->AddObject(pBase);
		pManager->AddObj(nAddObjToXRef + nStartRefID, pBase);
	}

	return pBase;
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
		DictToCDictObject(&oTemp, pParent, chKey);
		oTemp.free();
	}

	oParent.free();
	return pParent;
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
	PdfWriter::CXref* m_pXref = new PdfWriter::CXref(pDoc, xref->getNumObjects()); // Для новых объектов
	if (!xref || !pDoc || !pXref || !m_pXref)
	{
		RELEASEOBJECT(pXref);
		RELEASEOBJECT(m_pXref);
		return S_FALSE;
	}
	pXref->SetPrev(m_pXref);

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
				DictToCDictObject(&oT, pObj, "");
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
				DictToCDictObject(&oT, pObj, chKey);
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
				DictToCDictObject(&oT, pObj, chKey);
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
			DictToCDictObject(&oTemp, pTrailer, chKey);
		}
		oTemp.free();
	}

	bool bRes = pDoc->SaveNewWithPassword(pXref, m_pXref, wsPath, wsPassword, wsPassword, pTrailer);

	RELEASEOBJECT(pXref);

	return bRes ? S_OK : S_FALSE;
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

	Parser* parser = new Parser(pXref, new Lexer(pXref, &oContents), gFalse);

	int nNumArgs = 0;
	Object oObj;
	Object pArgs[maxArgs];

	parser->getObj(&oObj);
	while (!oObj.isEOF())
	{
		if (oObj.isCmd())
		{
			if (oObj.isCmd("q"))
			{
				Object obj;
				parser->getObj(&obj);
				while (!obj.isEOF() && !obj.isCmd("Q"))
				{
					obj.free();
					parser->getObj(&obj);
				}
				obj.free();
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
			oObj.free();
			for (int i = 0; i < nNumArgs; ++i)
				pArgs[i].free();
			nNumArgs = 0;
		}
		else if (nNumArgs < maxArgs)
			pArgs[nNumArgs++] = oObj;

		parser->getObj(&oObj);
	}
	oObj.free();
	for (int i = 0; i < nNumArgs; ++i)
		pArgs[i].free();
	RELEASEOBJECT(parser);

	oContents.free();
}

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

CPdfEditor::CPdfEditor(const std::wstring& _wsSrcFile, const std::wstring& _wsPassword, const std::wstring& _wsDstFile, CPdfReader* _pReader, CPdfWriter* _pWriter, Mode nMode)
{
	m_wsSrcFile  = _wsSrcFile;
	m_wsDstFile  = _wsDstFile;
	m_wsPassword = _wsPassword;
	m_pReader    = _pReader;
	m_pWriter    = _pWriter;
	m_nEditPage  = -1;
	m_nError     = 0;
	m_nMode      = Mode::Unknown;

	PDFDoc* pPDFDocument = m_pReader->GetPDFDocument(0);
	if (!pPDFDocument)
	{
		m_nError = 1;
		return;
	}

	XRef* xref = pPDFDocument->getXRef();
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	if (!xref || !pDoc)
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
		IncrementalUpdates();
	else if (m_nMode == Mode::WriteNew)
	{
		PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
		PdfWriter::CPageTree* pPageTree = pDoc->GetPageTree();
		m_mObjManager.SetDoc(pDoc);
		int nPages = m_pReader->GetNumPages();
		pPageTree->CreateFakePages(nPages);
		m_pWriter->SetNeedAddHelvetica(false);
	}
}
bool CPdfEditor::IncrementalUpdates()
{
	if (m_nMode != Mode::Unknown)
		return true;

	m_nMode = Mode::WriteAppend;
	PDFDoc* pPDFDocument = m_pReader->GetPDFDocument(0);
	XRef* xref = pPDFDocument->getXRef();
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();

	std::string sPathUtf8New = U_TO_UTF8(m_wsDstFile);
	std::string sPathUtf8Old = U_TO_UTF8(m_wsSrcFile);
	if (sPathUtf8Old == sPathUtf8New || NSSystemPath::NormalizePath(sPathUtf8Old) == NSSystemPath::NormalizePath(sPathUtf8New))
	{
		GString* owner_pswd = NSStrings::CreateString(m_wsPassword);
		GString* user_pswd  = NSStrings::CreateString(m_wsPassword);
		GBool bRes = pPDFDocument->makeWritable(true, owner_pswd, user_pswd);
		delete owner_pswd;
		delete user_pswd;
		if (!bRes)
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

	// Получение каталога и дерева страниц из reader
	Object catDict, catRefObj, pagesRefObj;
	if (!xref->getCatalog(&catDict)->isDict() || !catDict.dictLookupNF("Pages", &pagesRefObj))
	{
		pagesRefObj.free(); catDict.free();
		return false;
	}
	Object* trailer = xref->getTrailerDict();
	if (!trailer || !trailer->isDict() || !trailer->dictLookupNF("Root", &catRefObj)->isRef())
	{
		pagesRefObj.free(); catDict.free(); catRefObj.free();
		return false;
	}
	Ref catRef = catRefObj.getRef();
	catRefObj.free();

	// Создание каталога для writer
	PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, catRef.num);
	if (!pXref)
	{
		pagesRefObj.free(); catDict.free();
		return false;
	}
	PdfWriter::CCatalog* pCatalog = new PdfWriter::CCatalog();
	if (!pCatalog)
	{
		pagesRefObj.free(); catDict.free(); RELEASEOBJECT(pXref);
		return false;
	}
	pXref->Add(pCatalog, catRef.gen);
	PdfWriter::CResourcesDict* pDR = NULL;
	PdfWriter::CXref* pDRXref = NULL;
	for (int nIndex = 0; nIndex < catDict.dictGetLength(); ++nIndex)
	{
		Object oAcroForm;
		char* chKey = catDict.dictGetKey(nIndex);
		if (strcmp("AcroForm", chKey) == 0)
		{
			catDict.dictGetVal(nIndex, &oAcroForm);
			PdfWriter::CDictObject* pAcroForm = new PdfWriter::CDictObject();

			for (int nIndex = 0; nIndex < oAcroForm.dictGetLength(); ++nIndex)
			{
				Object oTemp2;
				char* chKey = oAcroForm.dictGetKey(nIndex);
				if (strcmp("DR", chKey) == 0)
				{
					if (!oAcroForm.dictGetVal(nIndex, &oTemp2)->isDict())
					{
						oTemp2.free();
						continue;
					}

					Object oDR;
					oAcroForm.dictGetValNF(nIndex, &oDR);
					int nDRxrefNum = oDR.isRef() ? oDR.getRefNum() : xref->getNumObjects();
					int nDRxrefGen = oDR.isRef() ? oDR.getRefGen() : 0;
					oDR.free();
					pDRXref = new PdfWriter::CXref(pDoc, nDRxrefNum);

					pDR = new PdfWriter::CResourcesDict(NULL, true, false);
					pDRXref->Add(pDR, nDRxrefGen);

					pAcroForm->Add(chKey, pDR);
					for (int nIndex2 = 0; nIndex2 < oTemp2.dictGetLength(); ++nIndex2)
					{
						Object oTemp;
						char* chKey2 = oTemp2.dictGetKey(nIndex2);
						oTemp2.dictGetVal(nIndex2, &oTemp);
						DictToCDictObject(&oTemp, pDR, chKey2);
						oTemp.free();
					}
					oTemp2.free();

					pDR->Fix();
					continue;
				}
				else if (strcmp("Fields", chKey) == 0)
					oAcroForm.dictGetVal(nIndex, &oTemp2);
				else if (strcmp("NeedAppearances", chKey) == 0)
				{
					oTemp2.free();
					continue;
				}
				else
					oAcroForm.dictGetValNF(nIndex, &oTemp2);
				DictToCDictObject(&oTemp2, pAcroForm, chKey);
				oTemp2.free();
			}

			if (!pAcroForm->Get("Fields"))
				pAcroForm->Add("Fields", new PdfWriter::CArrayObject());

			oAcroForm.free();
			pCatalog->Add(chKey, pAcroForm);
			continue;
		}
		else
			catDict.dictGetValNF(nIndex, &oAcroForm);
		DictToCDictObject(&oAcroForm, pCatalog, chKey);
		oAcroForm.free();
	}
	catDict.free();

	// Проверка уникальности имён текущих цифровых подписей pdf
	unsigned int nFormField = 0;
	AcroForm* form = pPDFDocument->getCatalog()->getForm();
	if (form)
	{
		nFormField = form->getNumFields() + 1;
		std::wstring sSig = L"Sig" + std::to_wstring(nFormField);
		int i = 0, nFormFields = form->getNumFields();
		while (i < nFormFields)
		{
			int nLength;
			Unicode* uName = form->getField(i)->getName(&nLength);
			std::wstring sName = NSStringExt::CConverter::GetUnicodeFromUTF32(uName, nLength);
			RELEASEMEM(uName);
			if (sName == sSig)
			{
				i = 0;
				nFormField++;
				sSig = L"Sig" + std::to_wstring(nFormField);
			}
			else
				i++;
		}
		nFormField--;
	}

	// Получение шифрования из reader и применения для writer
	int nCryptAlgorithm = -1;
	PdfWriter::CEncryptDict* pEncryptDict = NULL;
	if (xref->isEncrypted())
	{
		CryptAlgorithm encAlgorithm;
		GBool ownerPasswordOk;
		int permFlags, keyLength, encVersion;
		xref->getEncryption(&permFlags, &ownerPasswordOk, &keyLength, &encVersion, &encAlgorithm);
		nCryptAlgorithm = encAlgorithm;

		Object* pTrailerDict = xref->getTrailerDict();
		if (pTrailerDict)
		{
			pEncryptDict = new PdfWriter::CEncryptDict();

			// Нужно получить словарь Encrypt БЕЗ дешифровки, поэтому времено отключаем encrypted в xref
			xref->offEncrypted();

			Object encrypt, ID, ID1;
			if (pTrailerDict->dictLookup("Encrypt", &encrypt) && encrypt.isDict())
			{
				for (int nIndex = 0; nIndex < encrypt.dictGetLength(); ++nIndex)
				{
					Object oTemp;
					char* chKey = encrypt.dictGetKey(nIndex);
					encrypt.dictGetValNF(nIndex, &oTemp);
					DictToCDictObject(&oTemp, pEncryptDict, chKey, true);
					oTemp.free();
				}
			}

			if (!pEncryptDict->Get("Length"))
				pEncryptDict->Add("Length", 40);

			encrypt.free();

			if (pTrailerDict->dictLookup("ID", &ID) && ID.isArray() && ID.arrayGet(0, &ID1) && ID1.isString())
				DictToCDictObject(&ID1, pEncryptDict, "ID", true);
			ID.free(); ID1.free();

			xref->onEncrypted();

			pEncryptDict->SetRef(0, 0);
			pEncryptDict->Fix();

			pEncryptDict->SetPasswords(m_wsPassword, m_wsPassword);
			if (!pEncryptDict->UpdateKey(nCryptAlgorithm))
			{
				pagesRefObj.free();
				RELEASEOBJECT(pXref);
				RELEASEOBJECT(pDRXref);
				return false;
			}
		}
	}

	// Применение редактирования для writer
	bool bRes = pDoc->EditPdf(xref->getLastXRefPos(), xref->getNumObjects() + 1, pXref, pCatalog, pEncryptDict, nFormField);
	if (bRes)
	{
		// Воспроизведение дерева страниц во writer
		GetPageTree(xref, &pagesRefObj);

		if (pDR && pDRXref)
			bRes = pDoc->EditResources(pDRXref, pDR);
	}
	pagesRefObj.free();
	return bRes;
}
void CPdfEditor::Close()
{
	if (m_wsDstFile.empty())
		 return;

	if (m_nMode == Mode::Unknown)
	{
		if (m_wsDstFile != m_wsSrcFile && NSSystemPath::NormalizePath(m_wsDstFile) != NSSystemPath::NormalizePath(m_wsSrcFile))
			NSFile::CFileBinary::Copy(m_wsSrcFile, m_wsDstFile);
		return;
	}
	if (m_nMode == Mode::Split)
	{
		m_pWriter->SaveToFile(m_wsDstFile);
		return;
	}
	if (m_nMode == Mode::WriteNew)
	{
		PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
		PdfWriter::CPageTree* pPageTree = pDoc->GetPageTree();
		int nPages = pPageTree->GetCount();
		for (int i = 0; i < nPages; ++i)
		{
			PdfWriter::CObjectBase* pObj = pPageTree->GetObj(i);
			if (pObj && pObj->GetType() != PdfWriter::object_type_DICT)
				EditPage(i, false, true);
		}

		Object oCatalog;
		PDFDoc* pPDFDocument = m_pReader->GetPDFDocument(0);
		int nStartRefID = 0;
		XRef* xref = pPDFDocument->getXRef();
		if (!xref->getCatalog(&oCatalog)->isDict())
		{
			oCatalog.free();
			return;
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
					return;
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
				{ // Нельзя гарантировать их выполнение
					oTemp.free();
					continue;
				}
				else if (strcmp("DR", chKey) == 0)
				{ // Добавляем только уникальные ключи
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
								PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oTemp2, pDoc, xref, &m_mObjManager, nStartRefID);
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
									PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oTemp2, pDoc, xref, &m_mObjManager, nStartRefID);
									if (oTempRef.isRef())
										pDoc->AddObject(pBase);
									pDict->Add(chKey3, pBase);
									oTemp2.free(); oTempRef.free();
									continue;
								}
							}
						}
						oTemp2.free(); oTemp.free();
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
				PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oTemp, pDoc, xref, &m_mObjManager, nStartRefID);
				pAcroForm->Add(chKey, pBase);
				oTemp.free();
			}
		}
		oAcroForm.free(); oCatalog.free();

		if (xref->isEncrypted() && xref->getTrailerDict())
		{
			CryptAlgorithm encAlgorithm;
			GBool ownerPasswordOk;
			int permFlags, keyLength, encVersion;
			xref->getEncryption(&permFlags, &ownerPasswordOk, &keyLength, &encVersion, &encAlgorithm);
			int nCryptAlgorithm = encAlgorithm;

			Object* pTrailerDict = xref->getTrailerDict();
			pDoc->SetPasswords(m_wsPassword, m_wsPassword);
			PdfWriter::CEncryptDict* pEncryptDict = pDoc->GetEncrypt();

			// Нужно получить словарь Encrypt БЕЗ дешифровки, поэтому времено отключаем encrypted в xref
			xref->offEncrypted();

			Object encrypt, ID, ID1;
			if (pTrailerDict->dictLookup("Encrypt", &encrypt) && encrypt.isDict())
			{
				for (int nIndex = 0; nIndex < encrypt.dictGetLength(); ++nIndex)
				{
					Object oTemp;
					char* chKey = encrypt.dictGetKey(nIndex);
					encrypt.dictGetValNF(nIndex, &oTemp);
					PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oTemp, pDoc, xref, &m_mObjManager, nStartRefID, 0, true, true);
					pEncryptDict->Add(chKey, pBase);
					oTemp.free();
				}
			}

			if (!pEncryptDict->Get("Length"))
				pEncryptDict->Add("Length", 40);

			encrypt.free();

			PdfWriter::CObjectBase* pID = NULL;
			if (pTrailerDict->dictLookup("ID", &ID) && ID.isArray() && ID.arrayGet(0, &ID1) && ID1.isString())
			{
				pID = DictToCDictObject2(&ID1, pDoc, xref, &m_mObjManager, nStartRefID, 0, true, true);
				pEncryptDict->Add("ID", pID);
			}
			ID.free(); ID1.free();

			xref->onEncrypted();

			pDoc->SetEncryption(pEncryptDict, pID);
			pEncryptDict->Fix();
			pEncryptDict->UpdateKey(nCryptAlgorithm);
		}

		m_pWriter->SaveToFile(m_wsDstFile);
		return;
	}

	// m_nMode == Mode::WriteAppend
	PDFDoc* pPDFDocument = m_pReader->GetPDFDocument(0);
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	XRef* xref = pPDFDocument->getXRef();

	// Добавляем первый элемент в таблицу xref
	// он должен иметь вид 0000000000 65535 f
	PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, 0, 65535);
	if (!pXref)
		return;

	PdfWriter::CDictObject* pTrailer = NULL;
	Object* trailerDict = xref->getTrailerDict();
	if (trailerDict)
	{
		pTrailer = pXref->GetTrailer();

		for (int nIndex = 0; nIndex < trailerDict->dictGetLength(); ++nIndex)
		{
			Object oTemp;
			char* chKey = trailerDict->dictGetKey(nIndex);
			trailerDict->dictGetValNF(nIndex, &oTemp);
			DictToCDictObject(&oTemp, pTrailer, chKey);
			oTemp.free();
		}
	}

	Object info;
	pPDFDocument->getDocInfo(&info);
	PdfWriter::CXref* pInfoXref = NULL;
	PdfWriter::CInfoDict* pInfoDict = NULL;
	if (info.isDict())
	{
		// Обновление Info
		PdfWriter::CObjectBase* pInfo = pTrailer->Get("Info");
		pInfoXref = new PdfWriter::CXref(pDoc, pInfo ? pInfo->GetObjId() : 0);
		if (!pInfoXref)
		{
			RELEASEOBJECT(pXref);
			return;
		}
		pInfoDict = new PdfWriter::CInfoDict();
		if (!pInfoDict)
		{
			RELEASEOBJECT(pXref);
			RELEASEOBJECT(pInfoXref);
			return;
		}
		pInfoXref->Add(pInfoDict, pInfo ? pInfo->GetGenNo() : 0);

		for (int nIndex = 0; nIndex < info.dictGetLength(); ++nIndex)
		{
			Object oTemp;
			char* chKey = info.dictGetKey(nIndex);
			info.dictGetValNF(nIndex, &oTemp);
			DictToCDictObject(&oTemp, pInfoDict, chKey);
			oTemp.free();
		}
		pInfoDict->SetTime(PdfWriter::InfoModaDate);
	}
	info.free();

	if (!m_pWriter->EditClose() || !pDoc->AddToFile(m_wsDstFile, pXref, pTrailer, pInfoXref, pInfoDict))
	{
		RELEASEOBJECT(pXref);
		return;
	}

	std::string sPathUtf8New = U_TO_UTF8(m_wsDstFile);
	std::string sPathUtf8Old = U_TO_UTF8(m_wsSrcFile);
	if (sPathUtf8Old == sPathUtf8New || NSSystemPath::NormalizePath(sPathUtf8Old) == NSSystemPath::NormalizePath(sPathUtf8New))
	{
		GString* owner_pswd = NSStrings::CreateString(m_wsPassword);
		GString* user_pswd  = NSStrings::CreateString(m_wsPassword);
		pPDFDocument->makeWritable(false, owner_pswd, user_pswd);
		delete owner_pswd;
		delete user_pswd;

		NSFile::CFileBinary oFile;
		if (oFile.OpenFile(m_wsSrcFile))
		{
			m_pReader->ChangeLength(oFile.GetFileSize());
			oFile.CloseFile();
		}
	}

	m_pReader = NULL;
	m_pWriter = NULL;
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
					DictToCDictObject(&oRes, pDict, chKey2);
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
		DictToCDictObject(&oTemp, pPageT, chKey);
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

		// Получение объекта страницы
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
						PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oRes, pDoc, xref, &m_mObjManager, nStartRefID);
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
				// Поля родителей страниц переносятся к самим страницам
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
				PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oTemp, pDoc, xref, &m_mObjManager, nStartRefID, 0, false);
				pPage->Add(chKey, pBase);
				continue;
			}
			else
				pageObj.dictGetValNF(nIndex, &oTemp);
			PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oTemp, pDoc, xref, &m_mObjManager, nStartRefID);
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
					PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oRes, pDoc, xref, &m_mObjManager, nStartRefID);
					pDict->Add(chKey2, pBase);
					oRes.free();
				}
			}
		}
		pPage->Fix();
		pDoc->FixEditPage(pPage);
		double dCTM[6] = { 1, 0, 0, 1, 0, 0 };
		GetCTM(xref, &pageObj, dCTM);
		pageObj.free();

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

			pPage->StartTransform(dCTM[0], dCTM[1], dCTM[2], dCTM[3], dCTM[4], dCTM[5]);
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

	// Получение объекта страницы
	Object pageRefObj, pageObj;
	pageRefObj.initRef(pPageRef.first, pPageRef.second);
	if (!pageRefObj.fetch(xref, &pageObj) || !pageObj.isDict())
	{
		pageObj.free();
		pageRefObj.free();
		return false;
	}
	pageRefObj.free();

	// Воспроизведение словаря страницы из reader для writer
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
					DictToCDictObject(&oRes, pDict, chKey2);
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
					DictToCDictObject(&oAnnot, pArray, "");
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
				DictToCDictObject(&oTemp, pPage, chKey);
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
		DictToCDictObject(&oTemp, pPage, chKey);
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
				if (strcmp("Font", chKey2) == 0 || strcmp("ExtGState", chKey2) == 0 || strcmp("XObject", chKey2) == 0 || strcmp("Shading", chKey2) == 0 || strcmp("Pattern", chKey2) == 0)
					pResources->getVal(nIndex, &oRes);
				else
					pResources->getValNF(nIndex, &oRes);
				DictToCDictObject(&oRes, pDict, chKey2);
			}
		}
	}
	pPage->Fix();
	double dCTM[6] = { 1, 0, 0, 1, 0, 0 };
	GetCTM(xref, &pageObj, dCTM);
	pageObj.free();

	// Применение редактирования страницы для writer
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
		pPage->StartTransform(dCTM[0], dCTM[1], dCTM[2], dCTM[3], dCTM[4], dCTM[5]);
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

	// Страницы должны быть созданы заранее для ссылки на них
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

		// Получение объекта страницы
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

		// Получение объекта страницы
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
		// Копирование страницы со всеми ресурсами из reader для writer
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
						PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oRes, pDoc, xref, &m_mObjManager, nStartRefID);
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
			PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oTemp, pDoc, xref, &m_mObjManager, nStartRefID);
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
					PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oRes, pDoc, xref, &m_mObjManager, nStartRefID);
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
			GetCTM(xref, &pageObj, dCTM);
			pPage->StartTransform(dCTM[0], dCTM[1], dCTM[2], dCTM[3], dCTM[4], dCTM[5]);
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
			m_pWriter->SetNeedAddHelvetica(false); // TODO дописывает шрифт для адекватного редактирования Adobe pdf без текст. Убрать при реализации map шрифтов
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
			{ // Нельзя гарантировать их выполнение
				oTemp.free();
				continue;
			}
			else if (strcmp("DR", chKey) == 0)
			{ // Добавляем только уникальные ключи
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
							PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oTemp2, pDoc, xref, &m_mObjManager, nStartRefID);
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
								PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oTemp2, pDoc, xref, &m_mObjManager, nStartRefID);
								if (oTempRef.isRef())
									pDoc->AddObject(pBase);
								pDict->Add(chKey3, pBase);
								oTemp2.free(); oTempRef.free();
								continue;
							}
						}
					}
					oTemp2.free(); oTemp.free();
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
			PdfWriter::CObjectBase* pBase = DictToCDictObject2(&oTemp, pDoc, xref, &m_mObjManager, nStartRefID);
			pAcroForm->Add(chKey, pBase);
			oTemp.free();
		}
	}
	oAcroForm.free(); oCatalog.free();

	// Переименование полей
	std::string sPrefix = m_pReader->GetPrefixForm(pPDFDocument);
	if (!sPrefix.empty())
	{
		sPrefix = "_" + sPrefix;
		std::vector<int> arrRename; // Вектор переименованных полей
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
		return false;
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
	for (unsigned int i = 0; i < unLength; ++i)
	{
		if (arrPageIndex[i] < nTotalPages + nPages)
			mFileToPages[nPDFIndex].push_back(arrPageIndex[i] - nTotalPages);
		else
		{
			pPDFDocument = m_pReader->GetPDFDocument(++nPDFIndex);
			if (!pPDFDocument)
			{
				m_mObjManager.m_arrSplitAddPages.push_back(arrPageIndex[i]);
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
			if (!pPageD && pPageRef->num > 0)
			{
				PdfWriter::CObjectBase* pBase = new PdfWriter::CObjectBase();
				pBase->SetRef(pPageRef->num, pPageRef->gen);
				pPageD = new PdfWriter::CProxyObject(pBase, true);
			}
			pDest = pDoc->CreateDestination(pPageD, true);

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
bool CPdfEditor::MergePages(const std::wstring& wsPath)
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

	std::wstring wsFileName = NSFile::GetFileName(wsPath);
	std::string sFileName = U_TO_UTF8(wsFileName);
	PdfWriter::COutline* pOutline = pDoc->CreateOutline(NULL, sFileName.c_str());
	for (int i = 0, num = pListAdd->getLength(); i < num; i++)
	{
		OutlineItem* pOutlineItem = (OutlineItem*)pListAdd->get(i);
		if (pOutlineItem)
			CreateOutlines(pDocumentFirst, pDoc, pOutlineItem, pOutline);
	}

	return bRes;
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
	// Применение добавления страницы для writer
	if (!m_pWriter->AddPage(nPageIndex))
		return false;
	// По умолчанию выставляются размеры первой страницы, в дальнейшем размеры можно изменить
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

	// Получение объекта аннотации
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

	// Воспроизведение словаря аннотации из reader для writer
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
		std::map<std::wstring, std::wstring> mapFont = PdfReader::CAnnotFonts::GetAnnotFont(pPDFDocument, m_pReader->GetFontManager(), pFontList, &oAnnotRef);
		m_mFonts.insert(mapFont.begin(), mapFont.end());
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
		DictToCDictObject(&oTemp, pAnnot, chKey);
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
			Dict* pODict = oAPN.streamGetDict();
			for (int nIndex = 0; nIndex < pODict->getLength(); ++nIndex)
			{
				char* chKey = pODict->getKey(nIndex);
				pODict->getValNF(nIndex, &oTemp);
				DictToCDictObject(&oTemp, pAPN, chKey);
				oTemp.free();
			}
			int nLength = 0;
			if (pODict->lookup("Length", &oTemp)->isInt())
				nLength = oTemp.getInt();
			PdfWriter::CStream* pStream = new PdfWriter::CMemoryStream(nLength);
			pAPN->SetStream(pStream);
			pAPN->Add("Length", nLength);
			Stream* pOStream = oAPN.getStream()->getUndecodedStream();
			pOStream->reset();
			for (int nI = 0; nI < nLength; ++nI)
				pStream->WriteChar(pOStream->getChar());
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

		// Получение объекта аннотации
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
		// TODO перевыставить детей
		oParentRef.free();
	}
	return true;
}
int CPdfEditor::GetPagesCount()
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
int CPdfEditor::GetRotate(int nPageIndex)
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
	int nPageIndex = m_pReader->GetPageIndex(m_nEditPage, &pPDFDocument);
	PdfWriter::CDocument* pDoc = m_pWriter->GetDocument();
	if (nPageIndex < 0 || !pPDFDocument || !pDoc)
		return;
	XRef* xref = pPDFDocument->getXRef();
	Ref* pPageRef = pPDFDocument->getCatalog()->getPageRef(nPageIndex);

	// Получение объекта страницы
	Object pageRefObj, pageObj;
	pageRefObj.initRef(pPageRef->num, pPageRef->gen);
	if (!pageRefObj.fetch(xref, &pageObj)->isDict())
	{
		pageObj.free(); pageRefObj.free();
		return;
	}
	pageRefObj.free();

	Object oAnnots;
	// ВРЕМЕННО удаление Link аннотаций при редактировании
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
		pDoc->ClearPageFull();
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
		cropBox->x2 = pWPage->GetWidth();
		cropBox->y2 = pWPage->GetHeight();
	}

	std::vector<double> arrAllQuads;
	CRedact* pCommand = (CRedact*)_pCommand;
	std::vector<CRedact::SRedact*> arrRedacts = pCommand->GetRedact();
	for (CRedact::SRedact* pRedact : arrRedacts)
	{
		m_arrRedact.push_back(CRedactData());
		m_arrRedact.back().sID = pRedact->sID;
		m_arrRedact.back().arrQuads = pRedact->arrQuadPoints;
		for (int i = 0; i < pRedact->arrQuadPoints.size(); i += 2)
		{
			arrAllQuads.push_back(pRedact->arrQuadPoints[i + 0] + cropBox->x1);
			arrAllQuads.push_back(cropBox->y2 - pRedact->arrQuadPoints[i + 1]);
		}
		int nFlags = pRedact->nFlag;
		if (nFlags & (1 << 0))
		{
			m_arrRedact.back().pRender = pRedact->pRender;
			m_arrRedact.back().nLenRender = pRedact->nRenderLen;
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

		// TODO рендер редакта должен быть пересечён со всеми последующими редактами
		// TODO на самом деле должен быть рендер команд редакта
		/*
		PdfWriter::CPage* pCurPage = m_pWriter->GetPage();
		pDoc->FixEditPage(pCurPage);
		PdfWriter::CPage* pFakePage = new PdfWriter::CPage(pDoc);
		m_pWriter->SetPage(pFakePage);
		pDoc->SetCurPage(pFakePage);

		// TODO Нужно нивелировать текущую матрицу до единичной, а потом сместить ещё на CropBox

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
