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

#include "PdfAnnot.h"
#include "RendererOutputDev.h"
#include "Adaptors.h"

#include "../lib/xpdf/TextString.h"
#include "../lib/xpdf/Link.h"
#include "../lib/xpdf/Annot.h"
#include "../lib/xpdf/GfxFont.h"
#include "../lib/xpdf/Lexer.h"
#include "../lib/xpdf/Parser.h"
#include "../lib/goo/GList.h"
#include "../Resources/BaseFonts.h"

#include "../../DesktopEditor/common/Types.h"
#include "../../DesktopEditor/common/StringExt.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"

#include <map>

namespace PdfReader
{

double ArrGetNum(Object* pArr, int nI)
{
	double dRes = 0.0;
	Object oObj;
	if (pArr->arrayGet(nI, &oObj)->isNum())
		dRes = oObj.getNum();
	oObj.free();
	return dRes;
}
TextString* getName(Object* oField)
{
	TextString* sResName = NULL;

	if (!oField || !oField->isDict())
		return sResName;

	Object oName;
	if (oField->dictLookup("T", &oName)->isString())
		sResName = new TextString(oName.getString());
	else
	{
		oName.free();
		return sResName;
	}
	oName.free();

	Object oParent, oParent2;
	oField->dictLookup("Parent", &oParent);
	int nDepth = 0;
	while (oParent.isDict() && nDepth < 50)
	{
		if (oParent.dictLookup("T", &oName)->isString())
		{
			if (sResName->getLength())
				sResName->insert(0, (Unicode)'.');
			sResName->insert(0, oName.getString());
		}
		oName.free();

		oParent.dictLookup("Parent", &oParent2);
		oParent.free();
		oParent = oParent2;

		++nDepth;
	}
	oParent.free();

	return sResName;
}
CAction* getAction(PDFDoc* pdfDoc, Object* oAction)
{
	Object oActType;
	std::string sSName;
	if (oAction->dictLookup("S", &oActType)->isName())
		sSName = oActType.getName();
	oActType.free();

	LinkAction* oAct = LinkAction::parseAction(oAction);
	if (!oAct)
		return NULL;

	CAction* pRes = NULL;
	LinkActionKind kind = oAct->getKind();
	switch (kind)
	{
	// Переход внутри файла
	case actionGoTo:
	{
		GString* str = ((LinkGoTo*)oAct)->getNamedDest();
		LinkDest* pLinkDest = str ? pdfDoc->findDest(str) : ((LinkGoTo*)oAct)->getDest();
		if (!pLinkDest)
			break;
		CActionGoTo* ppRes = new CActionGoTo();
		if (pLinkDest->isPageRef())
		{
			Ref pageRef = pLinkDest->getPageRef();
			ppRes->unPage = pdfDoc->findPage(pageRef.num, pageRef.gen);
		}
		else
			ppRes->unPage = pLinkDest->getPageNum();

		if (ppRes->unPage > 0)
			--ppRes->unPage;
		ppRes->nKind = pLinkDest->getKind();

		PDFRectangle* pCropBox = pdfDoc->getCatalog()->getPage(ppRes->unPage + 1)->getCropBox();
		double dHeight = pCropBox->y2;
		double dX = pCropBox->x1;
		switch (ppRes->nKind)
		{
		case destXYZ:
		case destFitH:
		case destFitBH:
		case destFitV:
		case destFitBV:
		{
			ppRes->unKindFlag = 0;
			// 0 - left
			if (pLinkDest->getChangeLeft())
			{
				ppRes->unKindFlag |= (1 << 0);
				ppRes->pRect[0] = pLinkDest->getLeft() - dX;
			}
			// 1 - top
			if (pLinkDest->getChangeTop())
			{
				ppRes->unKindFlag |= (1 << 1);
				ppRes->pRect[1] = dHeight - pLinkDest->getTop();
			}
			// 2 - zoom
			if (pLinkDest->getChangeZoom() && pLinkDest->getZoom())
			{
				ppRes->unKindFlag |= (1 << 2);
				ppRes->pRect[2] = pLinkDest->getZoom();
			}
			break;
		}
		case destFitR:
		{
			ppRes->pRect[0] = pLinkDest->getLeft() - dX;
			ppRes->pRect[1] = dHeight - pLinkDest->getTop();
			ppRes->pRect[2] = pLinkDest->getRight() - dX;
			ppRes->pRect[3] = dHeight - pLinkDest->getBottom();
			break;
		}
		case destFit:
		case destFitB:
		default:
			break;
		}
		if (str)
			RELEASEOBJECT(pLinkDest);
		pRes = ppRes;
		break;
	}
	// Переход к внешнему файлу
	case actionGoToR:
	{
		break;
	}
	// Запуск стороннего приложения
	case actionLaunch:
	{
		break;
	}
	// Внешняя ссылка
	case actionURI:
	{
		CActionURI* ppRes = new CActionURI();
		TextString* s = new TextString(((LinkURI*)oAct)->getURI());
		ppRes->sURI = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
		pRes = ppRes;
		break;
	}
	// Нестандартно именованные действия
	case actionNamed:
	{
		CActionNamed* ppRes = new CActionNamed();
		TextString* s = new TextString(((LinkNamed*)oAct)->getName());
		ppRes->sNamed = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
		pRes = ppRes;
		break;
	}
	// Воспроизведение фильма
	case actionMovie:
	{
		break;
	}
	// JavaScript
	case actionJavaScript:
	{
		CActionJavaScript* ppRes = new CActionJavaScript();
		TextString* s = new TextString(((LinkJavaScript*)oAct)->getJS());
		ppRes->sJavaScript = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
		pRes = ppRes;
		break;
	}
	// Отправка формы
	case actionSubmitForm:
	{
		break;
	}
	// Скрытие аннотаций
	case actionHide:
	{
		CActionHide* ppRes = new CActionHide();
		ppRes->bHideFlag = ((LinkHide*)oAct)->getHideFlag();
		Object* oHideObj = ((LinkHide*)oAct)->getFields();
		int nHide = 1, k = 0;
		if (oHideObj->isArray())
			nHide = oHideObj->arrayGetLength();

		Object oHide;
		oHideObj->copy(&oHide);
		do
		{
			TextString* s = NULL;
			if (oHideObj->isArray())
			{
				oHide.free();
				oHideObj->arrayGet(k, &oHide);
			}
			if (oHide.isString())
				s = new TextString(oHide.getString());
			else if (oHide.isDict())
				s = getName(&oHide);

			if (s)
			{
				std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
				ppRes->arrAnnotName.push_back(sStr);
				delete s;
			}
			k++;
		} while (k < nHide);
		oHide.free();
		pRes = ppRes;
		break;
	}
	// Неизвестное действие
	case actionUnknown:
	default:
	{
		if (sSName == "ResetForm")
		{
			CActionResetForm* ppRes = new CActionResetForm();
			Object oObj;
			if (oAction->dictLookup("Flags", &oObj)->isInt())
				ppRes->unFlags = oObj.getInt();
			oObj.free();

			if (oAction->dictLookup("Fields", &oObj)->isArray())
			{
				for (int j = 0; j < oObj.arrayGetLength(); ++j)
				{
					Object oField;
					oObj.arrayGet(j, &oField);
					TextString* s = NULL;
					if (oField.isString())
						s = new TextString(oField.getString());
					else if (oField.isDict())
						s = getName(&oField);

					if (s)
					{
						std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
						ppRes->arrAnnotName.push_back(sStr);
						delete s;
					}
					oField.free();
				}
			}
			oObj.free();
			pRes = ppRes;
		}
		break;
	}
	}

	if (pRes)
	{
		pRes->pNext = NULL;
	}

	Object oNextAction;
	if (pRes && oAction->dictLookup("Next", &oNextAction)->isDict())
		pRes->pNext = getAction(pdfDoc, &oNextAction);
	oNextAction.free();

	RELEASEOBJECT(oAct);
	return pRes;
};
std::string getValue(Object* oV, bool bArray = true)
{
	std::string sRes;
	if (oV->isName())
		sRes = oV->getName();
	else if (oV->isString() || oV->isDict() || oV->isArray())
	{
		TextString* s = NULL;
		if (oV->isString())
			s = new TextString(oV->getString());
		else if (oV->isDict())
		{
			Object oContents;
			if (oV->dictLookup("Contents", &oContents)->isString())
			{
				s = new TextString(oContents.getString());
			}
			oContents.free();
		}
		else if (bArray && oV->isArray())
		{
			Object oContents;
			if (oV->arrayGet(0, &oContents)->isString())
			{
				s = new TextString(oContents.getString());
			}
			oContents.free();
		}
		if (s)
		{
			sRes = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
			delete s;
		}
	}
	return sRes;
}
void DrawAppearance(PDFDoc* pdfDoc, int nPage, AcroFormField* pField, Gfx* gfx, const char* sAPName, const char* sASName)
{
	XRef* xref = pdfDoc->getXRef();
	Object oFieldRef, oField;
	pField->getFieldRef(&oFieldRef);
	oFieldRef.fetch(xref, &oField);

	Object kidsObj, annotRef, annotObj;
	if (oField.dictLookup("Kids", &kidsObj)->isArray())
	{
		for (int j = 0; j < kidsObj.arrayGetLength(); ++j)
		{
			kidsObj.arrayGetNF(j, &annotRef);
			annotRef.fetch(xref, &annotObj);
			pField->drawAnnot(nPage, gfx, gFalse, &annotRef, &annotObj, sAPName, sASName, gFalse);
			annotObj.free();
			annotRef.free();
		}
	}
	else
		pField->drawAnnot(nPage, gfx, gFalse, &oFieldRef, &oField, sAPName, sASName, gFalse);
	kidsObj.free();

	oFieldRef.free(); oField.free();
}
BYTE getLE(Object* oObj)
{
	BYTE nLE = 5; // None

	if (oObj->isName("Square"))
		nLE = 0;
	else if (oObj->isName("Circle"))
		nLE = 1;
	else if (oObj->isName("Diamond"))
		nLE = 2;
	else if (oObj->isName("OpenArrow"))
		nLE = 3;
	else if (oObj->isName("ClosedArrow"))
		nLE = 4;
	else if (oObj->isName("Butt"))
		nLE = 6;
	else if (oObj->isName("ROpenArrow"))
		nLE = 7;
	else if (oObj->isName("RClosedArrow"))
		nLE = 8;
	else if (oObj->isName("Slash"))
		nLE = 9;

	return nLE;
}
CAnnotFileAttachment::CEmbeddedFile* getEF(Object* oObj)
{
	CAnnotFileAttachment::CEmbeddedFile* pRes = new CAnnotFileAttachment::CEmbeddedFile();

	Object oObj2;
	Dict* pImDict = oObj->streamGetDict();
	if (pImDict->lookup("Length", &oObj2)->isInt())
		pRes->nLength = oObj2.getInt();
	oObj2.free();
	if (pImDict->lookup("DL", &oObj2)->isInt())
		pRes->nLength = oObj2.getInt();
	oObj2.free();

	Stream* pImage = oObj->getStream();
	pImage->reset();
	pRes->pFile = new BYTE[pRes->nLength];
	BYTE* pBufferPtr = pRes->pFile;
	for (int nI = 0; nI < pRes->nLength; ++nI)
		*pBufferPtr++ = (BYTE)pImage->getChar();

	return pRes;
}
GList* tokenize(GString *s)
{
  GList *toks;
  int i, j;

  toks = new GList();
  i = 0;
  while (i < s->getLength()) {
	while (i < s->getLength() && Lexer::isSpace(s->getChar(i))) {
	  ++i;
	}
	if (i < s->getLength()) {
	  for (j = i + 1;
	   j < s->getLength() && !Lexer::isSpace(s->getChar(j));
	   ++j) ;
	  toks->append(new GString(s, i, j - i));
	  i = j;
	}
  }
  return toks;
}
CAnnot::CBorderType* getBorder(Object* oBorder, bool bBSorBorder)
{
	// Границы и Dash Pattern - Border/BS
	CAnnot::CBorderType* pBorderType = new CAnnot::CBorderType();
	if (!oBorder)
		return pBorderType;
	if (bBSorBorder)
	{
		pBorderType->nType = annotBorderSolid;
		Object oV;
		if (oBorder->dictLookup("S", &oV)->isName())
		{
			if (oV.isName("S"))
				pBorderType->nType = annotBorderSolid;
			else if (oV.isName("D"))
				pBorderType->nType = annotBorderDashed;
			else if (oV.isName("B"))
				pBorderType->nType = annotBorderBeveled;
			else if (oV.isName("I"))
				pBorderType->nType = annotBorderInset;
			else if (oV.isName("U"))
				pBorderType->nType = annotBorderUnderlined;
		}
		oV.free();
		if (oBorder->dictLookup("W", &oV)->isNum())
			pBorderType->dWidth = oV.getNum();
		oV.free();
		if (oBorder->dictLookup("D", &oV)->isArray())
		{
			for (int j = 0; j < oV.arrayGetLength(); ++j)
			{
				Object oObj2;
				if (oV.arrayGet(j, &oObj2)->isNum())
					pBorderType->arrDash.push_back(oObj2.getNum());
				oObj2.free();
			}
		}
		oV.free();
	}
	else
	{
		pBorderType->nType = annotBorderSolid;
		pBorderType->dWidth = ArrGetNum(oBorder, 2);
		if (!pBorderType->dWidth)
			pBorderType->dWidth = 1.0;

		Object oObj;
		if (oBorder->arrayGetLength() > 3 && oBorder->arrayGet(3, &oObj)->isArray() && oObj.arrayGetLength() > 1)
		{
			pBorderType->nType = annotBorderDashed;
			for (int j = 0; j < oObj.arrayGetLength(); ++j)
			{
				Object oObj2;
				if (oObj.arrayGet(j, &oObj2)->isNum())
					pBorderType->arrDash.push_back(oObj2.getNum());
				oObj2.free();
			}
		}
		oObj.free();
	}

	return pBorderType;
}

//------------------------------------------------------------------------
// Fonts
//------------------------------------------------------------------------

bool CAnnotFonts::IsBaseFont(const std::wstring& wsName)
{
	return wsName == L"Courier" || wsName == L"Courier-Bold" || wsName == L"Courier-BoldOblique" || wsName == L"Courier-Oblique" ||
		   wsName == L"Helvetica" || wsName == L"Helvetica-Bold" || wsName == L"Helvetica-BoldOblique" ||
		   wsName == L"Helvetica-Oblique" || wsName == L"Symbol" || wsName == L"Times-Bold" || wsName == L"Times-BoldItalic" ||
		   wsName == L"Times-Italic" || wsName == L"Times-Roman" || wsName == L"ZapfDingbats";
}
std::map<std::wstring, std::wstring> CAnnotFonts::GetAllFonts(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList)
{
	std::map<std::wstring, std::wstring> mFonts;

	AcroForm* pAcroForms = pdfDoc->getCatalog()->getForm();
	if (pAcroForms)
	{
		std::vector<int> arrUniqueFontsRef;
		for (int nField = 0, nNum = pAcroForms->getNumFields(); nField < nNum; ++nField)
		{
			AcroFormField* pField = pAcroForms->getField(nField);
			if (!pField)
				continue;

			// Шрифт и размер шрифта - из DA
			Ref fontID;
			double dFontSize = 0;
			pField->getFont(&fontID, &dFontSize);

			Object oFontRef;
			if (fontID.num < 0)
			{
				std::string sFontKey;
				if (!GetFontFromAP(pdfDoc, pField, &oFontRef, sFontKey))
				{
					oFontRef.free();
					continue;
				}
			}
			else
				oFontRef.initRef(fontID.num, fontID.gen);

			if (std::find(arrUniqueFontsRef.begin(), arrUniqueFontsRef.end(), oFontRef.getRefNum()) != arrUniqueFontsRef.end())
			{
				oFontRef.free();
				continue;
			}

			std::string sFontName;
			std::string sActualFontName;
			std::wstring wsFileName;
			bool bBold = false, bItalic = false;
			wsFileName = GetFontData(pdfDoc, pFontManager, pFontList, &oFontRef, sFontName, sActualFontName, bBold, bItalic);

			if (!sActualFontName.empty())
			{
				oFontRef.free();
				continue;
			}

			if (!sFontName.empty())
			{
				std::wstring wsFontName = UTF8_TO_U(sFontName);
				if (mFonts.find(wsFontName) == mFonts.end())
				{
					arrUniqueFontsRef.push_back(oFontRef.getRefNum());
					mFonts[wsFontName] = wsFileName;
				}
			}
			oFontRef.free();

			if (pField->getAcroFormFieldType() == acroFormFieldPushbutton && fontID.num >= 0)
			{
				std::string sFontKey;
				if (GetFontFromAP(pdfDoc, pField, &oFontRef, sFontKey) && std::find(arrUniqueFontsRef.begin(), arrUniqueFontsRef.end(), oFontRef.getRefNum()) == arrUniqueFontsRef.end())
				{
					wsFileName = GetFontData(pdfDoc, pFontManager, pFontList, &oFontRef, sFontName, sActualFontName, bBold, bItalic);

					std::wstring wsFontName = UTF8_TO_U(sFontName);
					if (sActualFontName.empty() && mFonts.find(wsFontName) == mFonts.end())
					{
						arrUniqueFontsRef.push_back(oFontRef.getRefNum());
						mFonts[wsFontName] = wsFileName;
					}
				}
			}
			oFontRef.free();
		}
	}

	for (int nPage = 0, nLastPage = pdfDoc->getNumPages(); nPage < nLastPage; ++nPage)
	{
		Page* pPage = pdfDoc->getCatalog()->getPage(nPage + 1);
		if (!pPage)
			continue;

		Object oAnnots;
		if (!pPage->getAnnots(&oAnnots)->isArray())
		{
			oAnnots.free();
			continue;
		}

		for (int i = 0, nNum = oAnnots.arrayGetLength(); i < nNum; ++i)
		{
			Object oAnnot;
			if (!oAnnots.arrayGet(i, &oAnnot)->isDict())
			{
				oAnnot.free();
				continue;
			}

			Object oSubtype;
			if (!oAnnot.dictLookup("Subtype", &oSubtype)->isName("FreeText"))
			{
				oSubtype.free(); oAnnot.free();
				continue;
			}
			oSubtype.free();

			Object oObj;
			if (!oAnnot.dictLookup("RC", &oObj)->isString())
			{
				oObj.free();
				if (oAnnot.dictLookup("AP", &oObj)->isNull() && oAnnot.dictLookup("Contents", &oObj)->isString() && oObj.getString()->getLength())
				{
					const unsigned char* pData14 = NULL;
					unsigned int nSize14 = 0;
					std::wstring wsFontName = L"Helvetica";
					NSFonts::IFontsMemoryStorage* pMemoryStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
					if (pMemoryStorage && !pMemoryStorage->Get(wsFontName) && GetBaseFont(wsFontName, pData14, nSize14))
						pMemoryStorage->Add(wsFontName, (BYTE*)pData14, nSize14, false);
					mFonts[L"Helvetica"] = L"Helvetica";
				}
				oAnnot.free(); oObj.free();
				continue;
			}
			oAnnot.free();

			TextString* s = new TextString(oObj.getString());
			std::string sRC = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
			delete s;
			oObj.free();

			Object oAnnotRef;
			oAnnots.arrayGetNF(i, &oAnnotRef);
			std::vector<PdfReader::CAnnotMarkup::CFontData*> arrRC = CAnnotMarkup::ReadRC(sRC);
			std::map<std::wstring, std::wstring> mFreeText = GetFreeTextFont(pdfDoc, pFontManager, pFontList, &oAnnotRef, arrRC);
			for (std::map<std::wstring, std::wstring>::iterator it = mFreeText.begin(); it != mFreeText.end(); ++it)
			{
				if (mFonts.find(it->first) != mFonts.end())
					continue;
				mFonts[it->first] = it->second;
			}
			oAnnotRef.free();
			for (int j = 0; j < arrRC.size(); ++j)
				RELEASEOBJECT(arrRC[j]);
		}
		oAnnots.free();
	}

	return mFonts;
}
std::wstring CAnnotFonts::GetFontData(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList, Object* oFontRef, std::string& sFontName, std::string& sActualFontName, bool& bBold, bool& bItalic)
{
	bBold = false, bItalic = false;
	XRef* xref = pdfDoc->getXRef();

	Object oFont;
	if (!xref->fetch(oFontRef->getRefNum(), oFontRef->getRefGen(), &oFont)->isDict())
	{
		oFont.free();
		return L"";
	}

	GfxFont* gfxFont = GfxFont::makeFont(xref, "F", oFontRef->getRef(), oFont.getDict());
	oFont.free();
	if (!gfxFont)
		return L"";

	Ref oEmbRef;
	std::wstring wsFontBaseName = NSStrings::GetStringFromUTF32(gfxFont->getName());
	std::wstring wsFileName;

	if (gfxFont->getEmbeddedFontID(&oEmbRef) || IsBaseFont(wsFontBaseName))
	{
		std::wstring wsFontName;
		RendererOutputDev::GetFont(xref, pFontManager, pFontList, gfxFont, wsFileName, wsFontName);

		sFontName = U_TO_UTF8(wsFontName);
		RendererOutputDev::CheckFontStylePDF(wsFontName, bBold, bItalic);
		if (!bBold)
			bBold = gfxFont->isBold();
		if (!bItalic)
			bItalic = gfxFont->isItalic();
	}
	else
	{
		std::wstring wsFBN = wsFontBaseName;
		NSFonts::CFontInfo* pFontInfo = RendererOutputDev::GetFontByParams(xref, pFontManager, gfxFont, wsFBN);
		if (pFontInfo && !pFontInfo->m_wsFontPath.empty())
		{
			if (wsFontBaseName.length() > 7 && wsFontBaseName.at(6) == '+')
			{
				bool bIsRemove = true;
				for (int nIndex = 0; nIndex < 6; nIndex++)
				{
					wchar_t nChar = wsFontBaseName.at(nIndex);
					if (nChar < 'A' || nChar > 'Z')
					{
						bIsRemove = false;
						break;
					}
				}
				if (bIsRemove)
					wsFontBaseName.erase(0, 7);
			}

			wsFileName = pFontInfo->m_wsFontPath;
			sFontName  = U_TO_UTF8(wsFontBaseName);
			sActualFontName = U_TO_UTF8(pFontInfo->m_wsFontName);
			bBold = pFontInfo->m_bBold;
			bItalic = pFontInfo->m_bItalic;
		}
	}

	RELEASEOBJECT(gfxFont);
	return wsFileName;
}
bool CAnnotFonts::GetFontFromAP(PDFDoc* pdfDoc, AcroFormField* pField, Object* oFontRef, std::string& sFontKey)
{
	bool bFindResources = false;

	Object oAP, oN;
	XRef* xref = pdfDoc->getXRef();
	if (pField->fieldLookup("AP", &oAP)->isDict() && oAP.dictLookup("N", &oN)->isStream())
	{
		Parser* parser = new Parser(xref, new Lexer(xref, &oN), gFalse);

		bool bFindFont = false;
		Object oObj1, oObj2, oObj3;
		parser->getObj(&oObj1);
		while (!oObj1.isEOF())
		{
			if (oObj1.isName())
			{
				parser->getObj(&oObj2);
				if (oObj2.isEOF())
					break;
				if (oObj2.isNum())
				{
					parser->getObj(&oObj3);
					if (oObj3.isEOF())
						break;
					if (oObj3.isCmd("Tf"))
					{
						bFindFont = true;
						break;
					}
				}
			}
			if (oObj2.isName())
			{
				oObj1.free();
				oObj2.copy(&oObj1);
				oObj2.free(); oObj3.free();
				continue;
			}
			if (oObj3.isName())
			{
				oObj1.free();
				oObj3.copy(&oObj1);
				oObj3.free(); oObj2.free();
				continue;
			}
			oObj1.free(); oObj2.free(); oObj3.free();

			parser->getObj(&oObj1);
		}

		if (bFindFont && oObj1.isName())
		{
			Object oR, oFonts;
			bFindResources = oN.streamGetDict()->lookup("Resources", &oR)->isDict() && oR.dictLookup("Font", &oFonts)->isDict() && oFonts.dictLookupNF(oObj1.getName(), oFontRef)->isRef();
			sFontKey = oObj1.getName();
			oR.free(); oFonts.free();
		}

		oObj1.free(); oObj2.free(); oObj3.free();
		RELEASEOBJECT(parser);
	}
	oAP.free(); oN.free();

	return bFindResources;
}
bool CAnnotFonts::FindFonts(Object* oStream, int nDepth, Object* oResFonts)
{
	if (nDepth > 5)
		return false;

	Object oResources;
	if (!oStream->streamGetDict()->lookup("Resources", &oResources)->isDict())
	{
		oResources.free();
		return false;
	}

	if (oResources.dictLookup("Font", oResFonts)->isDict())
	{
		oResources.free();
		return true;
	}

	Object oXObject;
	if (oResources.dictLookup("XObject", &oXObject)->isDict())
	{
		for (int i = 0, nLength = oXObject.dictGetLength(); i < nLength; ++i)
		{
			Object oXObj;
			if (!oXObject.dictGetVal(i, &oXObj)->isStream())
			{
				oXObj.free();
				continue;
			}
			if (FindFonts(&oXObj, nDepth + 1, oResFonts))
			{
				oXObj.free(); oXObject.free(); oResources.free();
				return true;
			}
			oXObj.free();
		}
	}
	oXObject.free(); oResources.free();
	return false;
}
std::map<std::wstring, std::wstring> CAnnotFonts::GetAnnotFont(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList, Object* oAnnotRef)
{
	Object oAnnot, oObj;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);
	std::map<std::wstring, std::wstring> mFontFreeText;

	Object oAP, oN;
	if (!oAnnot.dictLookup("AP", &oAP)->isDict() || !oAP.dictLookup("N", &oN)->isStream())
	{
		oAP.free(); oN.free(); oAnnot.free();
		return mFontFreeText;
	}
	oAP.free();

	Object oFonts;
	if (!FindFonts(&oN, 0, &oFonts))
	{
		oN.free(); oFonts.free(); oAnnot.free();
		return mFontFreeText;
	}
	oN.free();

	CFontList* pAppFontList = (CFontList*)pFontManager->GetApplication()->GetList();
	NSFonts::IFontsMemoryStorage* pMemoryStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();

	for (int i = 0, nFonts = oFonts.dictGetLength(); i < nFonts; ++i)
	{
		Object oFontRef;
		if (!oFonts.dictGetValNF(i, &oFontRef)->isRef())
		{
			oFontRef.free();
			continue;
		}

		std::string sFontName, sActualFontName;
		bool bBold = false, bItalic = false;
		std::wstring sFontPath = GetFontData(pdfDoc, pFontManager, pFontList, &oFontRef, sFontName, sActualFontName, bBold, bItalic);
		oFontRef.free();
		if (sFontPath.empty() || IsBaseFont(sFontPath) || !sActualFontName.empty())
			continue;

		std::wstring wsFontName = UTF8_TO_U(sFontName);
		NSFonts::IFontStream* pFontStream = pMemoryStorage ? (NSFonts::IFontStream*)pMemoryStorage->Get(sFontPath) : NULL;
		if (pFontStream)
		{
			bool bNew = true;
			std::vector<NSFonts::CFontInfo*>* arrFontList = pAppFontList->GetFonts();
			for (int nIndex = 0; nIndex < arrFontList->size(); ++nIndex)
			{
				if (((*arrFontList)[nIndex]->m_wsFontPath == sFontPath ||
					 (*arrFontList)[nIndex]->m_wsFontName == wsFontName) &&
					 (*arrFontList)[nIndex]->m_bBold      == (bBold ? 1 : 0) &&
					 (*arrFontList)[nIndex]->m_bItalic    == (bItalic ? 1 : 0))
				{
					bNew = false;
					break;
				}
			}
			if (bNew)
				pAppFontList->Add(sFontPath, pFontStream);
		}
		mFontFreeText[wsFontName] = sFontPath;
	}

	oFonts.free(); oAnnot.free();
	return mFontFreeText;
}
std::map<std::wstring, std::wstring> CAnnotFonts::GetFreeTextFont(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, Object* oAnnotRef, std::vector<CAnnotMarkup::CFontData*>& arrRC)
{
	std::map<std::wstring, std::wstring> mRes;

	std::map<std::wstring, std::wstring> mFontFreeText = GetAnnotFont(pdfDoc, pFontManager, pFontList, oAnnotRef);
	CFontList* pAppFontList = (CFontList*)pFontManager->GetApplication()->GetList();
	for (int i = 0; i < arrRC.size(); ++i)
	{
		if (arrRC[i]->bFind)
			continue;

		std::string sFontName = arrRC[i]->sFontFamily;
		std::wstring wsFontName = UTF8_TO_U(sFontName);
		bool bBold = (bool)((arrRC[i]->unFontFlags >> 0) & 1);
		bool bItalic = (bool)((arrRC[i]->unFontFlags >> 1) & 1);
		if (IsBaseFont(wsFontName))
		{
			if (sFontName == "Times-Roman")
			{
				if (bBold && bItalic)
					sFontName = "Times-BoldItalic";
				else if (bBold)
					sFontName = "Times-Bold";
				else if (bItalic)
					sFontName = "Times-Italic";
			}
			else if (sFontName == "Courier" || sFontName == "Helvetica")
			{
				if (bBold && bItalic)
					sFontName += "-BoldOblique";
				else if (bBold)
					sFontName += "-Bold";
				else if (bItalic)
					sFontName += "-Oblique";
			}
			wsFontName = UTF8_TO_U(sFontName);

			const unsigned char* pData14 = NULL;
			unsigned int nSize14 = 0;
			NSFonts::IFontsMemoryStorage* pMemoryStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
			if (pMemoryStorage && !pMemoryStorage->Get(wsFontName) && GetBaseFont(wsFontName, pData14, nSize14))
				pMemoryStorage->Add(wsFontName, (BYTE*)pData14, nSize14, false);

			std::string sFontNameBefore = arrRC[i]->sFontFamily;
			arrRC[i]->sFontFamily = sFontName;
			arrRC[i]->bFind = true;
			mRes[wsFontName] = wsFontName;

			for (int j = i; j < arrRC.size(); ++j)
			{
				if (arrRC[j]->sFontFamily == sFontNameBefore && bBold == (bool)((arrRC[j]->unFontFlags >> 0) & 1) && bItalic == (bool)((arrRC[j]->unFontFlags >> 1) & 1))
				{
					arrRC[j]->sFontFamily = sFontName;
					arrRC[j]->bFind = true;
				}
			}
		}
		else
		{
			NSFonts::CFontSelectFormat oFontSelect;
			if (bBold)
				oFontSelect.bBold = new INT(1);
			if (bItalic)
				oFontSelect.bItalic = new INT(1);
			oFontSelect.wsName = new std::wstring(wsFontName);

			NSFonts::CFontInfo* pFontInfo = pAppFontList->GetByParams(oFontSelect);
			if (pFontInfo && !pFontInfo->m_wsFontPath.empty())
			{
				std::wstring sFontPath = pFontInfo->m_wsFontPath;
				bool bFindFreeText = false;
				for (std::map<std::wstring, std::wstring>::iterator it = mFontFreeText.begin(); it != mFontFreeText.end(); ++it)
				{
					if (it->second == sFontPath)
					{
						bFindFreeText = true;
						break;
					}
				}
				std::wstring wsFontBaseName = pFontInfo->m_wsFontName;
				if (wsFontBaseName.length() > 7 && wsFontBaseName.at(6) == '+')
				{
					bool bIsRemove = true;
					for (int nIndex = 0; nIndex < 6; nIndex++)
					{
						wchar_t nChar = wsFontBaseName.at(nIndex);
						if (nChar < 'A' || nChar > 'Z')
						{
							bIsRemove = false;
							break;
						}
					}
					if (bIsRemove)
						wsFontBaseName.erase(0, 7);
				}

				if (bFindFreeText)
				{
					arrRC[i]->sFontFamily = U_TO_UTF8(wsFontBaseName);
					mRes[wsFontBaseName] = pFontInfo->m_wsFontPath;
				}
				else
				{
					arrRC[i]->unFontFlags |= (1 << 6);
					arrRC[i]->sActualFont = U_TO_UTF8(wsFontBaseName);
				}
				arrRC[i]->bFind = true;

				std::string sFontNameNew = bFindFreeText ? arrRC[i]->sFontFamily : arrRC[i]->sActualFont;
				for (int j = i; j < arrRC.size(); ++j)
				{
					if (arrRC[j]->sFontFamily == sFontName && bBold == (bool)((arrRC[j]->unFontFlags >> 0) & 1) && bItalic == (bool)((arrRC[j]->unFontFlags >> 1) & 1))
					{
						if (bFindFreeText)
							arrRC[j]->sFontFamily = sFontNameNew;
						else
						{
							arrRC[j]->unFontFlags |= (1 << 6);
							arrRC[j]->sActualFont = sFontNameNew;
						}
						arrRC[j]->bFind = true;
					}
				}
			}
		}
	}

	return mRes;
}

//------------------------------------------------------------------------
// Widget
//------------------------------------------------------------------------

CAnnotWidgetBtn::CAnnotWidgetBtn(PDFDoc* pdfDoc, AcroFormField* pField) : CAnnotWidget(pdfDoc, pField)
{
	m_unIFFlag = 0;

	Object oObj;
	Object oFieldRef, oField;
	pField->getFieldRef(&oFieldRef);
	oFieldRef.fetch(pdfDoc->getXRef(), &oField);
	oFieldRef.free();

	// 9 - Значение поля - V
	if (oField.dictLookup("V", &oObj))
	{
		m_sV = getValue(&oObj);
		if (!m_sV.empty())
			m_unFlags |= (1 << 9);
	}
	oObj.free();
	oField.free();

	if (pField->fieldLookup("AS", &oObj)->isName())
		m_sV = oObj.getName();
	oObj.free();

	Object oMK;
	AcroFormFieldType oType = pField->getAcroFormFieldType();
	m_nStyle = (oType == acroFormFieldRadioButton ? 3 : 0);
	if (pField->fieldLookup("MK", &oMK)->isDict())
	{
		if (oType == acroFormFieldPushbutton)
		{
			// 10 - Заголовок - СА
			m_sCA = DictLookupString(&oMK, "CA", 10);
			// 11 - Заголовок прокрутки - RC
			m_sRC = DictLookupString(&oMK, "RC", 11);
			// 12 - Альтернативный заголовок - AC
			m_sAC = DictLookupString(&oMK, "AC", 12);
		}
		else
		{
			if (oMK.dictLookup("CA", &oObj)->isString())
			{
				std::string sCA(oObj.getString()->getCString());

				if (sCA == "l") // кружок
					m_nStyle = 3;
				else if (sCA == "4") // галка
					m_nStyle = 0;
				else if (sCA == "8") // крест
					m_nStyle = 1;
				else if (sCA == "u") // ромб
					m_nStyle = 2;
				else if (sCA == "n") // квадрат
					m_nStyle = 5;
				else if (sCA == "H") // звезда
					m_nStyle = 4;
			}
			oObj.free();
		}

		// 13 - Положение заголовка - TP
		if (oMK.dictLookup("TP", &oObj)->isInt())
		{
			m_nTP = oObj.getInt();
			m_unFlags |= (1 << 13);
		}
		oObj.free();

		Object oIF;
		if (oMK.dictLookup("IF", &oIF)->isDict())
		{
			m_unIFFlag = 1;
			// 1 - Масштабирование - SW
			if (oIF.dictLookup("SW", &oObj)->isName())
			{
				m_unIFFlag |= (1 << 1);
				std::string sName(oObj.getName());
				m_nSW = 0; // Default: A
				if (sName == "B")
					m_nSW = 2;
				else if (sName == "S")
					m_nSW = 3;
				else if (sName == "N")
					m_nSW = 1;
			}
			oObj.free();
			// 2 - Тип масштабирования - S
			if (oIF.dictLookup("S", &oObj)->isName())
			{
				m_unIFFlag |= (1 << 2);
				std::string sName(oObj.getName());
				m_nS = 0; // Default: P
				if (sName == "A")
					m_nS = 1;
			}
			oObj.free();
			// 3 - Смещение - A
			if (oIF.dictLookup("A", &oObj)->isArray())
			{
				m_dA1 = 0.5, m_dA2 = 0.5;
				Object oObj2;
				m_unIFFlag |= (1 << 3);
				m_dA1 = ArrGetNum(&oObj, 0);
				m_dA2 = ArrGetNum(&oObj, 1);
			}
			oObj.free();
			// 4 - Полное соответствие - FB
			if (oIF.dictLookup("FB", &oObj)->isBool() && oObj.getBool())
				m_unIFFlag |= (1 << 4);
			oObj.free();
		}
		oIF.free();
	}
	oMK.free();

	Object oOpt;
	pField->fieldLookup("Opt", &oOpt);

	// 14 - Имя вкл состояния - AP - N - Yes
	Object oNorm;
	if (pField->fieldLookup("AP", &oObj)->isDict() && oObj.dictLookup("N", &oNorm)->isDict())
	{
		for (int j = 0, nNormLength = oNorm.dictGetLength(); j < nNormLength; ++j)
		{
			std::string sNormName(oNorm.dictGetKey(j));
			if (sNormName != "Off")
			{
				m_unFlags |= (1 << 14);
				m_sAP_N_Yes = sNormName;

				int nOptI;
				if (oOpt.isArray() && isdigit(sNormName[0]) && (nOptI = std::stoi(sNormName)) >= 0 && nOptI < oOpt.arrayGetLength())
				{
					Object oOptJ;
					if (!oOpt.arrayGet(nOptI, &oOptJ) || !(oOptJ.isString() || oOptJ.isArray()))
					{
						oOptJ.free();
						break;
					}

					if (oOptJ.isString())
					{
						TextString* s = new TextString(oOptJ.getString());
						m_sAP_N_Yes = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
						delete s;
					}
					else if (oOptJ.isArray() && oOptJ.arrayGetLength() > 0)
					{
						Object oOptJ2;
						if (oOptJ.arrayGet(0, &oOptJ2)->isString())
						{
							TextString* s = new TextString(oOptJ2.getString());
							m_sAP_N_Yes = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
							delete s;
						}
						oOptJ2.free();
					}
					oOptJ.free();
				}
				break;
			}
		}
	}
	oNorm.free(); oObj.free(); oOpt.free();
}
CAnnotWidgetTx::CAnnotWidgetTx(PDFDoc* pdfDoc, AcroFormField* pField) : CAnnotWidget(pdfDoc, pField)
{
	Object oObj;
	Object oFieldRef, oField;
	pField->getFieldRef(&oFieldRef);
	oFieldRef.fetch(pdfDoc->getXRef(), &oField);
	oFieldRef.free();

	// 9 - Значение - V
	if (oField.dictLookup("V", &oObj))
	{
		m_sV = getValue(&oObj);
		if (!m_sV.empty())
			m_unFlags |= (1 << 9);
	}
	oObj.free();
	oField.free();

	// 10 - Максимальное количество символов в Tx - MaxLen
	int nMaxLen = pField->getMaxLen();
	if (nMaxLen > 0)
	{
		m_unFlags |= (1 << 10);
		m_unMaxLen = nMaxLen;
	}

	// 11 - Расширенный текст RV - RichText
	if (pField->getFlags() & (1 << 25))
		m_sRV = FieldLookupString(pField, "RV", 11);
}
CAnnotWidgetCh::CAnnotWidgetCh(PDFDoc* pdfDoc, AcroFormField* pField) : CAnnotWidget(pdfDoc, pField)
{
	Object oObj;
	Object oFieldRef, oField;
	pField->getFieldRef(&oFieldRef);
	oFieldRef.fetch(pdfDoc->getXRef(), &oField);
	oFieldRef.free();

	// 9 - Значение
	if (oField.dictLookup("V", &oObj))
	{
		m_sV = getValue(&oObj, false);
		if (!m_sV.empty())
			m_unFlags |= (1 << 9);
	}
	oObj.free();

	Object oOpt;
	// 10 - Список значений
	if (pField->fieldLookup("Opt", &oOpt)->isArray())
	{
		m_unFlags |= (1 << 10);
		int nOptLength = oOpt.arrayGetLength();
		for (int j = 0; j < nOptLength; ++j)
		{
			Object oOptJ;
			if (!oOpt.arrayGet(j, &oOptJ) || !(oOptJ.isString() || oOptJ.isArray()))
			{
				oOptJ.free();
				continue;
			}

			std::string sOpt1, sOpt2;
			if (oOptJ.isArray() && oOptJ.arrayGetLength() > 1)
			{
				Object oOptJ2;
				if (oOptJ.arrayGet(0, &oOptJ2)->isString())
				{
					TextString* s = new TextString(oOptJ2.getString());
					sOpt1 = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
					delete s;
				}
				oOptJ2.free();
				if (oOptJ.arrayGet(1, &oOptJ2)->isString())
				{
					TextString* s = new TextString(oOptJ2.getString());
					sOpt2 = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
					delete s;
				}
				oOptJ2.free();
			}
			else if (oOptJ.isString())
			{
				TextString* s = new TextString(oOptJ.getString());
				sOpt2 = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
				delete s;
			}
			m_arrOpt.push_back(std::make_pair(sOpt1, sOpt2));
			oOptJ.free();
		}
	}
	oOpt.free();

	// 11 - Индекс верхнего элемента - TI
	if (pField->fieldLookup("TI", &oObj)->isInt())
	{
		m_unFlags |= (1 << 11);
		m_unTI = oObj.getInt();
	}
	oObj.free();

	// 12 - Выбранные индексы - I
	if (oField.dictLookup("I", &oOpt)->isArray())
	{
		m_unFlags |= (1 << 12);
		int nILength = oOpt.arrayGetLength();
		for (int j = 0; j < nILength; ++j)
		{
			if (oOpt.arrayGet(j, &oObj)->isInt())
				m_arrI.push_back(oObj.getInt());
			oObj.free();
		}
	}
	oOpt.free();

	// 13 - Массив значений
	if (oField.dictLookup("V", &oOpt)->isArray())
	{
		m_unFlags |= (1 << 13);
		int nVLength = oOpt.arrayGetLength();
		for (int j = 0; j < nVLength; ++j)
		{
			if (oOpt.arrayGet(j, &oObj)->isString())
			{
				TextString* s = new TextString(oObj.getString());
				m_arrV.push_back(NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength()));
				delete s;
			}
			oObj.free();
		}
	}
	oOpt.free();

	oField.free();
}
CAnnotWidgetSig::CAnnotWidgetSig(PDFDoc* pdfDoc, AcroFormField* pField) : CAnnotWidget(pdfDoc, pField)
{
	Object oObj;
	Object oFieldRef, oField;
	pField->getFieldRef(&oFieldRef);
	oFieldRef.fetch(pdfDoc->getXRef(), &oField);
	oFieldRef.free();

	// 9 - Значение
	if (oField.dictLookup("V", &oObj)->isDict())
		m_unFlags |= (1 << 9);
	oObj.free();

	oField.free();
}
CAnnotWidget::CAnnotWidget(PDFDoc* pdfDoc, AcroFormField* pField) : CAnnot(pdfDoc, pField)
{
	Object oObj, oField;
	XRef* xref = pdfDoc->getXRef();
	pField->getFieldRef(&oObj);
	oObj.fetch(xref, &oField);
	oObj.free();

	m_dFontSize = 0.0;
	m_unFontStyle = 0;

	// Цвет текста - из DA
	int nSpace;
	GList *arrColors = pField->getColorSpace(&nSpace);
	for (int j = 0; j < nSpace; ++j)
		m_arrTC.push_back(*(double*)arrColors->get(j));
	deleteGList(arrColors, double);

	// Выравнивание текста - Q
	m_nQ = 0;
	if (pField->fieldLookup("Q", &oObj)->isInt())
		m_nQ = oObj.getInt();
	oObj.free();

	// Тип - FT + флаги
	AcroFormFieldType oType = pField->getAcroFormFieldType();
	m_nType = 26; // Unknown
	switch (oType)
	{
	case acroFormFieldPushbutton:    m_nType = 27; break;
	case acroFormFieldRadioButton:   m_nType = 28; break;
	case acroFormFieldCheckbox:      m_nType = 29; break;
	case acroFormFieldFileSelect:    m_nType = 30; break;
	case acroFormFieldMultilineText: m_nType = 30; break;
	case acroFormFieldText:          m_nType = 30; break;
	case acroFormFieldBarcode:       m_nType = 30; break;
	case acroFormFieldComboBox:      m_nType = 31; break;
	case acroFormFieldListBox:       m_nType = 32; break;
	case acroFormFieldSignature:     m_nType = 33; break;
	default:                         m_nType = 26; break;
	}

	// Флаг - Ff
	m_unFieldFlag = pField->getFlags();

	// 0 - Альтернативное имя поля, используется во всплывающей подсказке и сообщениях об ошибке - TU
	m_sTU = FieldLookupString(pField, "TU", 0);

	// 1 - Строка стиля по умолчанию - DS
	m_sDS = FieldLookupString(pField, "DS", 1);

	// 3 - Режим выделения - H
	if (pField->fieldLookup("H", &oObj)->isName())
	{
		m_unFlags |= (1 << 3);
		std::string sName(oObj.getName());
		m_nH = 1; // Default: I
		if (sName == "N")
			m_nH = 0;
		else if (sName == "O")
			m_nH = 3;
		else if (sName == "P" || sName == "T")
			m_nH = 2;
	}
	oObj.free();

	Object oMK;
	if (pField->fieldLookup("MK", &oMK)->isDict())
	{
		// 5 - Цвет границ - BC. Даже если граница не задана BS/Border, то при наличии BC предоставляется граница по-умолчанию (сплошная, толщиной 1)
		if (oMK.dictLookup("BC", &oObj)->isArray())
		{
			m_unFlags |= (1 << 5);
			int nBCLength = oObj.arrayGetLength();
			for (int j = 0; j < nBCLength; ++j)
			{
				Object oBCj;
				m_arrBC.push_back(oObj.arrayGet(j, &oBCj)->isNum() ? oBCj.getNum() : 0.0);
				oBCj.free();
			}
		}
		oObj.free();

		// 6 - Поворот аннотации относительно страницы - R
		if (oMK.dictLookup("R", &oObj)->isInt())
		{
			m_unFlags |= (1 << 6);
			m_unR = oObj.getInt();
		}
		oObj.free();

		// 7 - Цвет фона - BG
		if (oMK.dictLookup("BG", &oObj)->isArray())
		{
			m_unFlags |= (1 << 7);
			int nBGLength = oObj.arrayGetLength();
			for (int j = 0; j < nBGLength; ++j)
			{
				Object oBGj;
				m_arrBG.push_back(oObj.arrayGet(j, &oBGj)->isNum() ? oBGj.getNum() : 0.0);
				oBGj.free();
			}
		}
		oObj.free();
	}
	oMK.free();

	// 8 - Значение по-умолчанию
	if (oField.dictLookup("DV", &oObj))
	{
		m_sDV = getValue(&oObj);
		if (!m_sDV.empty())
			m_unFlags |= (1 << 8);
	}
	oObj.free();

	// 17 - Родитель - Parent
	if (oField.dictLookupNF("Parent", &oObj)->isRef())
	{
		m_unRefNumParent = oObj.getRefNum();
		m_unFlags |= (1 << 17);
	}
	oObj.free();

	// 18 - Частичное имя поля - T
	m_sT = DictLookupString(&oField, "T", 18);

	// Action - A
	Object oAction;
	if (pField->fieldLookup("A", &oAction)->isDict())
	{
		std::string sAA = "A";
		CAction* pA = getAction(pdfDoc, &oAction);
		if (pA)
		{
			pA->sType = sAA;
			m_arrAction.push_back(pA);
		}
	}
	oAction.free();

	// Actions - AA
	Object oAA;
	Object parent, parent2;
	bool bParent = oField.dictLookup("Parent", &parent)->isDict();
	bool bAA = bParent && parent.dictLookup("AA", &oAA)->isDict();
	oAA.free();

	if (oField.dictLookup("AA", &oAA)->isDict())
	{
		for (int j = 0; j < oAA.dictGetLength(); ++j)
		{
			if (oAA.dictGetVal(j, &oAction)->isDict())
			{
				std::string sAA(oAA.dictGetKey(j));
				if (bAA && (sAA == "K" || sAA == "F" || sAA == "V" || sAA == "C"))
					continue;
				CAction* pA = getAction(pdfDoc, &oAction);
				if (pA)
				{
					pA->sType = sAA;
					m_arrAction.push_back(pA);
				}
			}
			oAction.free();
		}
	}
	oAA.free();

	if (bParent)
	{
		int depth = 0;
		while (parent.isDict() && depth < 50)
		{
			if (parent.dictLookup("AA", &oAA)->isDict())
			{
				for (int j = 0; j < oAA.dictGetLength(); ++j)
				{
					if (oAA.dictGetVal(j, &oAction)->isDict())
					{
						std::string sAA(oAA.dictGetKey(j));
						if (sAA == "E" || sAA == "X" || sAA == "D" || sAA == "U" || sAA == "Fo" || sAA == "Bl" || sAA == "PO" || sAA == "PC" || sAA == "PV" || sAA == "PI")
							continue;
						CAction* pA = getAction(pdfDoc, &oAction);
						if (pA)
						{
							pA->sType = sAA;
							m_arrAction.push_back(pA);
						}
					}
					oAction.free();
				}
			}
			oAA.free();
			parent.dictLookup("Parent", &parent2);
			parent.free();
			parent = parent2;
			++depth;
		}
	}
	parent.free();

	oField.free();
}
CAnnotWidget::~CAnnotWidget()
{
	for (int i = 0; i < m_arrAction.size(); ++i)
		RELEASEOBJECT(m_arrAction[i]);
}
std::string CAnnotWidget::FieldLookupString(AcroFormField* pField, const char* sName, int nByte)
{
	std::string sRes;
	Object oObj;
	if (pField->fieldLookup(sName, &oObj)->isString())
	{
		m_unFlags |= (1 << nByte);
		TextString* s = new TextString(oObj.getString());
		sRes = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();
	return sRes;
}
void CAnnotWidget::SetFont(PDFDoc* pdfDoc, AcroFormField* pField, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList)
{
	// Шрифт и размер шрифта - из DA
	Ref fontID;
	pField->getFont(&fontID, &m_dFontSize);
	m_unFontStyle = 0;

	Object oFontRef;
	if (fontID.num < 0)
	{
		if (!CAnnotFonts::GetFontFromAP(pdfDoc, pField, &oFontRef, m_sFontKey))
		{
			oFontRef.free();
			return;
		}
	}
	else
		oFontRef.initRef(fontID.num, fontID.gen);

	bool bBold = false, bItalic = false;
	CAnnotFonts::GetFontData(pdfDoc, pFontManager, pFontList, &oFontRef, m_sFontName, m_sActualFontName, bBold, bItalic);
	oFontRef.free();

	// 2 - Актуальный шрифт
	if (!m_sActualFontName.empty())
		m_unFlags |= (1 << 2);

	// 4 - Уникальный идентификатор шрифта
	if (!m_sFontKey.empty())
		m_unFlags |= (1 << 4);

	if (bBold)
		m_unFontStyle |= (1 << 0);
	if (bItalic)
		m_unFontStyle |= (1 << 1);
}
void CAnnotWidget::SetButtonFont(PDFDoc* pdfDoc, AcroFormField* pField, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList)
{
	// Неполный шрифт во внешнем виде pushbutton
	Object oFontRef;
	if (!CAnnotFonts::GetFontFromAP(pdfDoc, pField, &oFontRef, m_sFontKey))
	{
		oFontRef.free();
		return;
	}

	bool bBold = false, bItalic = false;
	CAnnotFonts::GetFontData(pdfDoc, pFontManager, pFontList, &oFontRef, m_sButtonFontName, m_sButtonFontName, bBold, bItalic);
	if (!m_sButtonFontName.empty())
		m_unFlags |= (1 << 19);

	oFontRef.free();
}

//------------------------------------------------------------------------
// Popup
//------------------------------------------------------------------------

CAnnotPopup::CAnnotPopup(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CAnnot(pdfDoc, oAnnotRef, nPageIndex)
{
	m_unFlags = 0;

	Object oAnnot, oObj;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// 0 - Отображаться открытой - Open
	if (oAnnot.dictLookup("Open", &oObj)->isBool() && oObj.getBool())
		m_unFlags |= (1 << 0);
	oObj.free();

	// 1 - Родитель - Parent
	if (oAnnot.dictLookupNF("Parent", &oObj)->isRef())
	{
		m_unFlags |= (1 << 1);
		m_unRefNumParent = oObj.getRefNum();
	}
	oObj.free();
	oAnnot.free();
}

//------------------------------------------------------------------------
// Text
//------------------------------------------------------------------------

CAnnotText::CAnnotText(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CAnnotMarkup(pdfDoc, oAnnotRef, nPageIndex)
{
	Object oAnnot, oObj;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// 15 - Отображаться открытой - Open
	if (oAnnot.dictLookup("Open", &oObj)->isBool() && oObj.getBool())
		m_unFlags |= (1 << 15);
	oObj.free();

	// 16 - Иконка - Name
	m_unFlags |= (1 << 16);
	m_nName = 10; // Default: Note
	if (oAnnot.dictLookup("Name", &oObj)->isName())
	{
		std::string sName(oObj.getName());
		std::vector<std::string> arrName = {"Check", "Checkmark", "Circle", "Comment", "Cross", "CrossHairs", "Help", "Insert", "Key", "NewParagraph", "Note", "Paragraph", "RightArrow", "RightPointer", "Star", "UpArrow", "UpLeftArrow"};
		std::vector<std::string>::iterator p = std::find(arrName.begin(), arrName.end(), sName);
		if (p != arrName.end())
			m_nName = p - arrName.begin();
	}
	oObj.free();

	// 17 - Модель состояния - StateModel
	if (oAnnot.dictLookup("StateModel", &oObj)->isString())
	{
		m_unFlags |= (1 << 17);
		TextString* s = new TextString(oObj.getString());
		std::string sStateModel = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
		m_nStateModel = 1; // Review
		if (sStateModel == "Marked")
			m_nStateModel = 0;
	}
	oObj.free();

	// 18 - Состояние - State
	if (oAnnot.dictLookup("State", &oObj)->isString())
	{
		m_unFlags |= (1 << 18);
		TextString* s = new TextString(oObj.getString());
		std::string sState = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
		m_nState = 6; // None
		if (sState == "Marked")
			m_nState = 0;
		else if (sState == "Unmarked")
			m_nState = 1;
		else if (sState == "Accepted")
			m_nState = 2;
		else if (sState == "Rejected")
			m_nState = 3;
		else if (sState == "Cancelled")
			m_nState = 4;
		else if (sState == "Completed")
			m_nState = 5;
	}
	oObj.free();
	oAnnot.free();
}

//------------------------------------------------------------------------
// Ink
//------------------------------------------------------------------------

CAnnotInk::CAnnotInk(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CAnnotMarkup(pdfDoc, oAnnotRef, nPageIndex)
{
	Object oAnnot, oObj, oObj2;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// Путь - InkList
	if (oAnnot.dictLookup("InkList", &oObj)->isArray())
	{
		for (int j = 0; j < oObj.arrayGetLength(); ++j)
		{
			if (oObj.arrayGet(j, &oObj2)->isArray())
			{
				std::vector<double> arrLine;
				for (int k = 0; k < oObj2.arrayGetLength(); ++k)
				{
					Object oObj1;
					if (oObj2.arrayGet(k, &oObj1)->isNum())
						arrLine.push_back(k % 2 == 0 ? oObj1.getNum() - m_dX : m_dHeight - oObj1.getNum());
					else
						arrLine.push_back(0.0);
					oObj1.free();
				}
				if (!arrLine.empty())
					m_arrInkList.push_back(arrLine);
			}
		}
	}
	oObj.free();
	oAnnot.free();
}

//------------------------------------------------------------------------
// Line
//------------------------------------------------------------------------

CAnnotLine::CAnnotLine(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CAnnotMarkup(pdfDoc, oAnnotRef, nPageIndex)
{
	Object oAnnot, oObj, oObj2;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// Координаты линии - L
	if (oAnnot.dictLookup("L", &oObj)->isArray())
	{
		m_pL[0] = ArrGetNum(&oObj, 0) - m_dX;
		m_pL[1] = m_dHeight - ArrGetNum(&oObj, 1);
		m_pL[2] = ArrGetNum(&oObj, 2) - m_dX;
		m_pL[3] = m_dHeight - ArrGetNum(&oObj, 3);
	}
	oObj.free();

	// 15 - Стили окончания линии - LE
	if (oAnnot.dictLookup("LE", &oObj)->isArray())
	{
		m_unFlags |= (1 << 15);
		m_nLE[0] = 5; m_nLE[1] = 5; // None
		for (int i = 0; i < oObj.arrayGetLength() && i < 2; ++i)
		{
			if (oObj.arrayGet(i, &oObj2)->isName())
				m_nLE[i] = getLE(&oObj2);
			oObj2.free();
		}
	}
	oObj.free();

	// 16 - Цвет окончаний линии - IC
	if (oAnnot.dictLookup("IC", &oObj)->isArray())
	{
		m_unFlags |= (1 << 16);
		for (int j = 0; j < oObj.arrayGetLength(); ++j)
		{
			m_arrIC.push_back(oObj.arrayGet(j, &oObj2)->isNum() ? oObj2.getNum() : 0.0);
			oObj2.free();
		}
	}
	oObj.free();

	// 17 - Длина линий выноски - LL
	if (oAnnot.dictLookup("LL", &oObj)->isNum())
	{
		m_unFlags |= (1 << 17);
		m_dLL = oObj.getNum();
	}
	oObj.free();

	// 18 - Продолжение линий выноски - LLE
	if (oAnnot.dictLookup("LLE", &oObj)->isNum())
	{
		m_unFlags |= (1 << 18);
		m_dLLE = oObj.getNum();
	}
	oObj.free();

	// 19 - Местоположение заголовка - Cap
	if (oAnnot.dictLookup("Cap", &oObj)->isBool())
		m_unFlags |= (1 << 19);
	oObj.free();

	// 20 - Назначение аннотации - IT
	if (oAnnot.dictLookup("IT", &oObj)->isName())
	{
		m_unFlags |= (1 << 20);
		m_nIT = 0; // LineDimension
		if (oObj.isName("LineArrow"))
			m_nIT = 1;
	}
	oObj.free();

	// 21 - Длина смещения выноски - LLO
	if (oAnnot.dictLookup("LLO", &oObj)->isNum())
	{
		m_unFlags |= (1 << 21);
		m_dLLO = oObj.getNum();
	}
	oObj.free();

	// 22 - Расположение заголовка аннотации - CP
	if (oAnnot.dictLookup("CP", &oObj)->isName())
	{
		m_unFlags |= (1 << 22);
		m_nCP = 0; // Inline
		if (oObj.isName("Top"))
			m_nCP = 1;
	}
	oObj.free();

	// 23 - Смещение текста подписи - CO
	m_pCO[0] = 0.0; m_pCO[1] = 0.0;
	if (oAnnot.dictLookup("CO", &oObj)->isArray())
	{
		m_unFlags |= (1 << 23);
		m_pCO[0] = ArrGetNum(&oObj, 0);
		m_pCO[1] = ArrGetNum(&oObj, 1);
	}
	oObj.free();

	oAnnot.free();
}

//------------------------------------------------------------------------
// TextMarkup: Highlight, Underline, Squiggly, StrikeOut
//------------------------------------------------------------------------

CAnnotTextMarkup::CAnnotTextMarkup(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CAnnotMarkup(pdfDoc, oAnnotRef, nPageIndex)
{
	Object oAnnot, oObj, oObj2;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// Координаты - QuadPoints
	if (oAnnot.dictLookup("QuadPoints", &oObj)->isArray())
	{
		for (int i = 0; i < oObj.arrayGetLength(); ++i)
		{
			if (oObj.arrayGet(i, &oObj2)->isNum())
				m_arrQuadPoints.push_back(i % 2 == 0 ? oObj2.getNum() - m_dX : m_dHeight - oObj2.getNum());
			oObj2.free();
		}
	}
	oObj.free();

	// Подтип - Subtype
	std::string sType;
	if (oAnnot.dictLookup("Subtype", &oObj)->isName())
		sType = oObj.getName();
	oObj.free();

	if (sType == "Highlight")
		m_nSubtype = 8;
	else if (sType == "Underline")
		m_nSubtype = 9;
	else if (sType == "Squiggly")
		m_nSubtype = 10;
	else if (sType == "StrikeOut")
		m_nSubtype = 11;

	oAnnot.free();
}

//------------------------------------------------------------------------
// Square, Circle
//------------------------------------------------------------------------

CAnnotSquareCircle::CAnnotSquareCircle(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CAnnotMarkup(pdfDoc, oAnnotRef, nPageIndex)
{
	Object oAnnot, oObj, oObj2;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// Подтип - Subtype
	std::string sType;
	if (oAnnot.dictLookup("Subtype", &oObj)->isName())
		sType = oObj.getName();
	oObj.free();

	if (sType == "Square")
		m_nSubtype = 4;
	else if (sType == "Circle")
		m_nSubtype = 5;

	// 15 - Различия Rect и фактического размера - RD
	if (oAnnot.dictLookup("RD", &oObj)->isArray())
	{
		m_unFlags |= (1 << 15);
		m_pRD[0] = ArrGetNum(&oObj, 0);
		m_pRD[3] = ArrGetNum(&oObj, 1);
		m_pRD[2] = ArrGetNum(&oObj, 2);
		m_pRD[1] = ArrGetNum(&oObj, 3);
	}
	oObj.free();

	// 16 - Цвет заполнения - IC
	if (oAnnot.dictLookup("IC", &oObj)->isArray())
	{
		m_unFlags |= (1 << 16);
		for (int j = 0; j < oObj.arrayGetLength(); ++j)
		{
			m_arrIC.push_back(oObj.arrayGet(j, &oObj2)->isNum() ? oObj2.getNum() : 0.0);
			oObj2.free();
		}
	}
	oObj.free();

	oAnnot.free();
}

//------------------------------------------------------------------------
// Polygon, PolyLine
//------------------------------------------------------------------------

CAnnotPolygonLine::CAnnotPolygonLine(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CAnnotMarkup(pdfDoc, oAnnotRef, nPageIndex)
{
	Object oAnnot, oObj, oObj2;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// Подтип - Subtype
	std::string sType;
	if (oAnnot.dictLookup("Subtype", &oObj)->isName())
		sType = oObj.getName();
	oObj.free();

	if (sType == "Polygon")
		m_nSubtype = 6;
	else if (sType == "PolyLine")
		m_nSubtype = 7;

	// Координаты вершин - Vertices
	if (oAnnot.dictLookup("Vertices", &oObj)->isArray())
	{
		for (int j = 0; j < oObj.arrayGetLength(); ++j)
		{
			if (oObj.arrayGet(j, &oObj2)->isNum())
				m_arrVertices.push_back(j % 2 == 0 ? oObj2.getNum() - m_dX : m_dHeight - oObj2.getNum());
			else
				m_arrVertices.push_back(0.0);
			oObj2.free();
		}
	}
	oObj.free();

	// 15 - Стили окончания линии - LE
	if (oAnnot.dictLookup("LE", &oObj)->isArray())
	{
		m_unFlags |= (1 << 15);
		m_nLE[0] = 5; m_nLE[1] = 5; // None
		for (int i = 0; i < oObj.arrayGetLength() && i < 2; ++i)
		{
			if (oObj.arrayGet(i, &oObj2)->isName())
				m_nLE[i] = getLE(&oObj2);
			oObj2.free();
		}
	}
	oObj.free();

	// 16 - Цвет заполнения - IC
	if (oAnnot.dictLookup("IC", &oObj)->isArray())
	{
		m_unFlags |= (1 << 16);
		for (int j = 0; j < oObj.arrayGetLength(); ++j)
		{
			m_arrIC.push_back(oObj.arrayGet(j, &oObj2)->isNum() ? oObj2.getNum() : 0.0);
			oObj2.free();
		}
	}
	oObj.free();

	// 20 - Назначение аннотации - IT
	if (oAnnot.dictLookup("IT", &oObj)->isName())
	{
		m_unFlags |= (1 << 20);
		m_nIT = 0; // PolygonCloud
		if (oObj.isName("PolyLineDimension"))
			m_nIT = 1;
		else if (oObj.isName("PolygonDimension"))
			m_nIT = 2;
	}
	oObj.free();

	oAnnot.free();
}

//------------------------------------------------------------------------
// FreeText
//------------------------------------------------------------------------

CAnnotFreeText::CAnnotFreeText(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CAnnotMarkup(pdfDoc, oAnnotRef, nPageIndex)
{
	Object oAnnot, oObj, oObj2;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// Выравнивание текста - Q
	m_nQ = 0;
	if (oAnnot.dictLookup("Q", &oObj)->isInt())
		m_nQ = oObj.getInt();
	oObj.free();

	m_nRotate = 0;
	if (oAnnot.dictLookup("Rotate", &oObj)->isInt())
		m_nRotate = oObj.getInt();
	oObj.free();

	// 15 - Различия Rect и фактического размера - RD
	if (oAnnot.dictLookup("RD", &oObj)->isArray())
	{
		m_unFlags |= (1 << 15);
		m_pRD[0] = ArrGetNum(&oObj, 0);
		m_pRD[3] = ArrGetNum(&oObj, 1);
		m_pRD[2] = ArrGetNum(&oObj, 2);
		m_pRD[1] = ArrGetNum(&oObj, 3);
	}
	oObj.free();

	// 16 - Координаты выноски - CL
	if (oAnnot.dictLookup("CL", &oObj)->isArray())
	{
		m_unFlags |= (1 << 16);
		for (int j = 0; j < oObj.arrayGetLength(); ++j)
		{
			if (oObj.arrayGet(j, &oObj2)->isNum())
				m_arrCL.push_back(j % 2 == 0 ? oObj2.getNum() - m_dX : m_dHeight - oObj2.getNum());
			else
				m_arrCL.push_back(0.0);
			oObj2.free();
		}
	}
	oObj.free();

	// 17 - Строка стиля по умолчанию - DS
	m_sDS = DictLookupString(&oAnnot, "DS", 17);

	// 18 - Стили окончания линии - LE
	if (oAnnot.dictLookup("LE", &oObj)->isName())
	{
		m_unFlags |= (1 << 18);
		m_nLE = getLE(&oObj);
	}
	oObj.free();

	// 20 - Назначение аннотации - IT
	if (oAnnot.dictLookup("IT", &oObj)->isName())
	{
		m_unFlags |= (1 << 20);
		m_nIT = 0; // FreeText
		if (oObj.isName("FreeTextCallout"))
			m_nIT = 1;
		else if (oObj.isName("FreeTextTypeWriter"))
			m_nIT = 2;
	}
	oObj.free();

	// 21 - Цвет границы - color from DA
	if (oAnnot.dictLookup("DA", &oObj)->isString())
	{
		m_unFlags |= (1 << 21);
		int nSpace;
		GList *arrColors = new GList();

		// parse the default appearance string
		GList* daToks = tokenize(oObj.getString());
		for (int i = daToks->getLength() - 1; i > 0; --i) {

		  // handle the g operator
		  if (!((GString *)daToks->get(i))->cmp("g")) {
		arrColors->append(new double(atof(((GString *)daToks->get(i - 1))->getCString())));
		break;

		  // handle the rg operator
		  } else if (i >= 3 && !((GString *)daToks->get(i))->cmp("rg")) {
		arrColors->append(new double(atof(((GString *)daToks->get(i - 3))->getCString())));
		arrColors->append(new double(atof(((GString *)daToks->get(i - 2))->getCString())));
		arrColors->append(new double(atof(((GString *)daToks->get(i - 1))->getCString())));
		break;
		  } else if (i >= 4 && !((GString *)daToks->get(i))->cmp("k")) {
		arrColors->append(new double(atof(((GString *)daToks->get(i - 4))->getCString())));
		arrColors->append(new double(atof(((GString *)daToks->get(i - 3))->getCString())));
		arrColors->append(new double(atof(((GString *)daToks->get(i - 2))->getCString())));
		arrColors->append(new double(atof(((GString *)daToks->get(i - 1))->getCString())));
		break;
		  }
		}
		deleteGList(daToks, GString);

		nSpace = arrColors->getLength();

		for (int j = 0; j < nSpace; ++j)
			m_arrCFromDA.push_back(*(double*)arrColors->get(j));
		deleteGList(arrColors, double);
	}
	oObj.free();

	if (oAnnot.dictLookup("AP", &oObj)->isNull() && oAnnot.dictLookup("RC", &oObj2)->isNull() && oAnnot.dictLookup("Contents", &oObj)->isString() && oObj.getString()->getLength())
	{
		NSStringUtils::CStringBuilder oRC;

		oRC += L"<?xml version=\"1.0\"?><body xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:xfa=\"http://www.xfa.org/schema/xfa-data/1.0/\" xfa:APIVersion=\"Acrobat:23.8.0\"  xfa:spec=\"2.0.2\"><p dir=\"ltr\"><span style=\"font-size:14.0pt;font-family:Helvetica;text-align:left;color:";
		if (m_arrCFromDA.size() == 3)
			oRC.WriteHexColor3((unsigned char)(m_arrCFromDA[0] * 255.0),
							   (unsigned char)(m_arrCFromDA[1] * 255.0),
							   (unsigned char)(m_arrCFromDA[2] * 255.0));
		else
			oRC += L"#000000";

		oRC += L"\">";
		TextString* s = new TextString(oObj.getString());
		std::wstring wsContents = NSStringExt::CConverter::GetUnicodeFromUTF32(s->getUnicode(), s->getLength());
		delete s;
		oRC.WriteEncodeXmlString(wsContents);
		oRC += L"</span></p></body>";

		std::wstring wsRC = oRC.GetData();
		m_arrRC = CAnnotMarkup::ReadRC(U_TO_UTF8(wsRC));
		if (m_arrRC.empty())
			m_unFlags &= ~(1 << 3);
		else
			m_unFlags |= (1 << 3);
	}
	oObj.free(); oObj2.free();

	oAnnot.free();
}

//------------------------------------------------------------------------
// Caret
//------------------------------------------------------------------------

CAnnotCaret::CAnnotCaret(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CAnnotMarkup(pdfDoc, oAnnotRef, nPageIndex)
{
	Object oAnnot, oObj, oObj2;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// 15 - Различия Rect и фактического размера - RD
	if (oAnnot.dictLookup("RD", &oObj)->isArray() && oObj.arrayGetLength() == 4)
	{
		m_unFlags |= (1 << 15);
		m_pRD[0] = ArrGetNum(&oObj, 0);
		m_pRD[3] = ArrGetNum(&oObj, 1);
		m_pRD[2] = ArrGetNum(&oObj, 2);
		m_pRD[1] = ArrGetNum(&oObj, 3);
	}
	oObj.free();

	// 16 - Связанный символ - Sy
	if (oAnnot.dictLookup("Sy", &oObj)->isName())
	{
		m_unFlags |= (1 << 16);
		m_nSy = 0; // None
		if (oObj.isName("P"))
			m_nSy = 1;
		if (oObj.isName("S"))
			m_nSy = 2;
	}
	oObj.free();

	oAnnot.free();
}

//------------------------------------------------------------------------
// FileAttachment
//------------------------------------------------------------------------

CAnnotFileAttachment::CAnnotFileAttachment(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CAnnotMarkup(pdfDoc, oAnnotRef, nPageIndex)
{
	m_pEF = NULL;

	Object oAnnot, oObj, oObj2;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// 15 - Иконка - Name
	if (oAnnot.dictLookup("Name", &oObj)->isName())
	{
		m_unFlags |= (1 << 15);
		m_sName = oObj.getName();
	}
	oObj.free();

	Object oFS;
	if (!oAnnot.dictLookup("FS", &oFS)->isDict())
	{
		oFS.free(); oAnnot.free();
		return;
	}

	// 16 - Файловая система - FS
	if (oFS.dictLookup("FS", &oObj)->isName())
	{
		m_unFlags |= (1 << 16);
		m_sFS = oObj.getName();
	}
	oObj.free();

	// 17 - Спецификация файла - F
	m_sF = DictLookupString(&oFS, "F", 17);

	// 18 - Спецификация файла - UF
	m_sUF = DictLookupString(&oFS, "UF", 18);

	// 19 - Спецификация файла - DOS
	m_sDOS = DictLookupString(&oFS, "DOS", 19);

	// 20 - Спецификация файла - Mac
	m_sMac = DictLookupString(&oFS, "Mac", 20);

	// 21 - Спецификация файла - Unix
	m_sUnix = DictLookupString(&oFS, "Unix", 21);

	// 22 - Идентификатор файла - ID
	if (oFS.dictLookup("ID", &oObj)->isArray() && oObj.arrayGetLength() == 2)
	{
		m_unFlags |= (1 << 22);
		if (oObj.arrayGet(0, &oObj2)->isString())
		{
			TextString* s = new TextString(oObj2.getString());
			m_sID.first = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
			delete s;
		}
		oObj2.free();
		if (oObj.arrayGet(1, &oObj2)->isString())
		{
			TextString* s = new TextString(oObj2.getString());
			m_sID.second = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
			delete s;
		}
		oObj2.free();
	}
	oObj.free();

	// 23 - Изменчивость файла - V
	if (oFS.dictLookup("V", &oObj)->isBool() && oObj.getBool())
		m_unFlags |= (1 << 23);
	oObj.free();

	// 24 - Встроенные файловые потоки - EF
	Object oEF;
	if (oFS.dictLookup("EF", &oEF)->isDict())
	{
		m_unFlags |= (1 << 24);
		m_pEF = new CEmbeddedFiles();
		if (oEF.dictLookup("F", &oObj)->isStream())
			m_pEF->m_pF = getEF(&oObj);
		oObj.free();
		if (oEF.dictLookup("UF", &oObj)->isStream())
			m_pEF->m_pUF = getEF(&oObj);
		oObj.free();
		if (oEF.dictLookup("DOS", &oObj)->isStream())
			m_pEF->m_pDOS = getEF(&oObj);
		oObj.free();
		if (oEF.dictLookup("Mac", &oObj)->isStream())
			m_pEF->m_pMac = getEF(&oObj);
		oObj.free();
		if (oEF.dictLookup("Unix", &oObj)->isStream())
			m_pEF->m_pUnix = getEF(&oObj);
		oObj.free();
	}
	oEF.free();

	// 25 - Встроенные файловые потоки - RF
	Object oRF;
	if (oFS.dictLookup("RF", &oRF)->isDict())
	{
		m_unFlags |= (1 << 25);
	}
	oRF.free();

	// 26 - Описание файла - Desc
	m_sDesc = DictLookupString(&oFS, "Desc", 26);

	// 27 - Коллекция - Cl
	if (oFS.dictLookup("Cl", &oObj)->isDict())
	{
		m_unFlags |= (1 << 27);
	}
	oObj.free();

	oFS.free(); oAnnot.free();
}
CAnnotFileAttachment::~CAnnotFileAttachment()
{
	RELEASEOBJECT(m_pEF);
}

//------------------------------------------------------------------------
// Stamp
//------------------------------------------------------------------------

CAnnotStamp::CAnnotStamp(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CAnnotMarkup(pdfDoc, oAnnotRef, nPageIndex)
{
	Object oAnnot, oObj;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// Иконка - Name
	if (oAnnot.dictLookup("Name", &oObj)->isName())
	{
		m_sName = oObj.getName();
	}
	oObj.free();

	m_dRotate = 0;
	if (oAnnot.dictLookup("Rotate", &oObj)->isNum())
		m_dRotate = oObj.getNum();
	oObj.free();

	double m[6] = { 1, 0, 0, 1, 0, 0 }, bbox[4] = { 0, 0, 0, 0 };

	Object oAP, oObj2;
	if (oAnnot.dictLookup("AP", &oAP)->isDict() && oAP.dictLookup("N", &oObj2)->isStream())
	{
		Object oObj1;
		if (oObj2.streamGetDict()->lookup("BBox", &oObj)->isArray() && oObj.arrayGetLength() == 4)
		{
			for (int i = 0; i < 4; ++i)
			{
				oObj.arrayGet(i, &oObj1);
				bbox[i] = oObj1.isNum() ? oObj1.getNum() : 0;
				oObj1.free();
			}
		}

		oObj.free();
		if (oObj2.streamGetDict()->lookup("Matrix", &oObj)->isArray() && oObj.arrayGetLength() == 6)
		{
			for (int i = 0; i < 6; ++i)
			{
				oObj.arrayGet(i, &oObj1);
				m[i] = oObj1.getNum();
				oObj1.free();
			}
		}
	}
	oAP.free(); oObj2.free(); oObj.free();

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
		sx = (m_pRect[2] - m_pRect[0]) / (formXMax - formXMin);
	if (formYMin == formYMax)
		sy = 1;
	else
		sy = (m_pRect[3] - m_pRect[1]) / (formYMax - formYMin);
	double tx = -formXMin * sx + m_pRect[0];
	double ty = -formYMin * sy + m_pRect[1];

	m[0] *= sx;
	m[1] *= sy;
	m[2] *= sx;
	m[3] *= sy;
	m[4] = m[4] * sx + tx;
	m[5] = m[5] * sy + ty;

	m_dX1 = bbox[0] * m[0] + bbox[1] * m[2] + m[4];
	m_dY1 = bbox[0] * m[1] + bbox[1] * m[3] + m[5];

	m_dX2 = bbox[0] * m[0] + bbox[3] * m[2] + m[4];
	m_dY2 = bbox[0] * m[1] + bbox[3] * m[3] + m[5];

	m_dX3 = bbox[2] * m[0] + bbox[3] * m[2] + m[4];
	m_dY3 = bbox[2] * m[1] + bbox[3] * m[3] + m[5];

	m_dX4 = bbox[2] * m[0] + bbox[1] * m[2] + m[4];
	m_dY4 = bbox[2] * m[1] + bbox[1] * m[3] + m[5];

	oAnnot.free();
}

//------------------------------------------------------------------------
// Annots
//------------------------------------------------------------------------

CAnnots::CAnnots(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList, int nStartPage)
{
	Object oObj1, oObj2;
	XRef* xref = pdfDoc->getXRef();
	AcroForm* pAcroForms = pdfDoc->getCatalog()->getForm();

	// Порядок вычислений - CO
	Object* oAcroForm = pAcroForms->getAcroFormObj();
	if (oAcroForm->dictLookup("CO", &oObj1)->isArray())
	{
		for (int j = 0; j < oObj1.arrayGetLength(); ++j)
		{
			if (oObj1.arrayGetNF(j, &oObj2)->isRef())
				m_arrCO.push_back(oObj2.getRefNum());
			oObj2.free();
		}
	}
	oObj1.free();

	// Fields
	for (int i = 0, nNum = pAcroForms->getNumFields(); i < nNum; ++i)
	{
		AcroFormField* pField = pAcroForms->getField(i);
		Object oFieldRef, oField;
		if (!pField || !pField->getFieldRef(&oFieldRef)->isRef() || !oFieldRef.fetch(xref, &oField)->isDict())
		{
			oField.free(); oFieldRef.free();
			continue;
		}

		if (pField->getPageNum() < 1)
		{
			oField.free(); oFieldRef.free();
			std::vector<int>::iterator it = std::find(m_arrCO.begin(), m_arrCO.end(), oFieldRef.getRefNum());
			if (it != m_arrCO.end())
				m_arrCO.erase(it);
			continue;
		}

		// Родители
		Object oParentRefObj;
		if (oField.dictLookupNF("Parent", &oParentRefObj)->isRef())
			getParents(xref, &oParentRefObj);
		oParentRefObj.free();
		oField.free(); oFieldRef.free();

		CAnnotWidget* pAnnot = NULL;
		AcroFormFieldType oType = pField->getAcroFormFieldType();
		switch (oType)
		{
		case acroFormFieldPushbutton:
		case acroFormFieldRadioButton:
		case acroFormFieldCheckbox:
		{
			pAnnot = new CAnnotWidgetBtn(pdfDoc, pField);
			break;
		}
		case acroFormFieldFileSelect:
		case acroFormFieldMultilineText:
		case acroFormFieldText:
		case acroFormFieldBarcode:
		{
			pAnnot = new CAnnotWidgetTx(pdfDoc, pField);
			break;
		}
		case acroFormFieldComboBox:
		case acroFormFieldListBox:
		{
			pAnnot = new CAnnotWidgetCh(pdfDoc, pField);
			break;
		}
		case acroFormFieldSignature:
		{
			pAnnot = new CAnnotWidgetSig(pdfDoc, pField);
			break;
		}
		default:
			break;
		}
		if (pAnnot)
		{
			pAnnot->SetFont(pdfDoc, pField, pFontManager, pFontList);
			if (pField->getAcroFormFieldType() == acroFormFieldPushbutton)
				pAnnot->SetButtonFont(pdfDoc, pField, pFontManager, pFontList);
			pAnnot->SetPage(nStartPage + pField->getPageNum());
			m_arrAnnots.push_back(pAnnot);
		}
	}
}
CAnnots::~CAnnots()
{
	for (int i = 0; i < m_arrParents.size(); ++i)
		RELEASEOBJECT(m_arrParents[i]);

	for (int i = 0; i < m_arrAnnots.size(); ++i)
		RELEASEOBJECT(m_arrAnnots[i]);
}
void CAnnots::getParents(XRef* xref, Object* oFieldRef)
{
	if (!oFieldRef || !xref || !oFieldRef->isRef() ||
		std::find_if(m_arrParents.begin(), m_arrParents.end(), [oFieldRef] (CAnnotParent* pAP) { return oFieldRef->getRefNum() == pAP->unRefNum; }) != m_arrParents.end())
		return;

	Object oField;
	CAnnotParent* pAnnotParent = new CAnnotParent();
	if (!pAnnotParent || !oFieldRef->fetch(xref, &oField)->isDict())
	{
		oField.free();
		RELEASEOBJECT(pAnnotParent);
		return;
	}

	pAnnotParent->unRefNum = oFieldRef->getRefNum();

	Object oObj;
	if (oField.dictLookup("T", &oObj)->isString())
	{
		TextString* s = new TextString(oObj.getString());
		std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		pAnnotParent->unFlags |= (1 << 0);
		pAnnotParent->sT = sStr;
		delete s;
	}
	oObj.free();

	if (oField.dictLookup("V", &oObj))
	{
		pAnnotParent->sV = getValue(&oObj, false);
		if (!pAnnotParent->sV.empty())
			pAnnotParent->unFlags |= (1 << 1);

		if (oObj.isArray())
		{
			pAnnotParent->unFlags |= (1 << 5);
			int nVLength = oObj.arrayGetLength();
			for (int j = 0; j < nVLength; ++j)
			{
				Object oObj2;
				if (oObj.arrayGet(j, &oObj2)->isString())
				{
					TextString* s = new TextString(oObj2.getString());
					pAnnotParent->arrV.push_back(NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength()));
					delete s;
				}
				oObj2.free();
			}
		}
	}
	oObj.free();

	if (oField.dictLookup("DV", &oObj))
	{
		pAnnotParent->sDV = getValue(&oObj);
		if (!pAnnotParent->sDV.empty())
			pAnnotParent->unFlags |= (1 << 2);
	}
	oObj.free();

	Object oI;
	if (oField.dictLookup("I", &oI)->isArray())
	{
		int nILength = oI.arrayGetLength();
		for (int j = 0; j < nILength; ++j)
		{
			if (oI.arrayGet(j, &oObj)->isInt())
				pAnnotParent->arrI.push_back(oObj.getInt());
			oObj.free();
		}
		if (!pAnnotParent->arrI.empty())
			pAnnotParent->unFlags |= (1 << 3);
	}
	oI.free();

	Object oOpt;
	// 6 - Opt
	if (oField.dictLookup("Opt", &oOpt)->isArray())
	{
		int nOptLength = oOpt.arrayGetLength();
		for (int j = 0; j < nOptLength; ++j)
		{
			Object oOptJ;
			if (!oOpt.arrayGet(j, &oOptJ) || !oOptJ.isString())
			{
				oOptJ.free();
				continue;
			}

			TextString* s = new TextString(oOptJ.getString());
			pAnnotParent->arrOpt.push_back(NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength()));
			delete s;
			oOptJ.free();
		}
		if (!pAnnotParent->arrOpt.empty())
			pAnnotParent->unFlags |= (1 << 6);
	}
	oOpt.free();

	m_arrParents.push_back(pAnnotParent);

	Object oParentRefObj;
	if (oField.dictLookupNF("Parent", &oParentRefObj)->isRef())
	{
		pAnnotParent->unFlags |= (1 << 4);
		pAnnotParent->unRefNumParent = oParentRefObj.getRefNum();
		getParents(xref, &oParentRefObj);
	}
	oParentRefObj.free();

	oField.free();
}

//------------------------------------------------------------------------
// Markup
//------------------------------------------------------------------------

CAnnotMarkup::CAnnotMarkup(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CAnnot(pdfDoc, oAnnotRef, nPageIndex)
{
	m_unFlags = 0;

	Object oAnnot, oObj;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// 0 - Всплывающая аннотация - Popup
	if (oAnnot.dictLookupNF("Popup", &oObj)->isRef())
	{
		m_unFlags |= (1 << 0);
		m_unRefNumPopup = oObj.getRefNum();
	}

	// 1 - Текстовая метка пользователя - T
	m_sT = DictLookupString(&oAnnot, "T", 1);

	// 2 - Значение непрозрачности - CA
	if (oAnnot.dictLookup("CA", &oObj)->isNum())
	{
		m_unFlags |= (1 << 2);
		m_dCA = oObj.getNum();
	}
	oObj.free();

	// 3 - Форматированный текст - RC
	std::string sRC = DictLookupString(&oAnnot, "RC", 3);
	// std::cout << sRC << std::endl;
	// if (oAnnot.dictLookup("RC", &oObj)->isStream())
	// TODO streamGetBlock
	m_arrRC = CAnnotMarkup::ReadRC(sRC);
	if (m_arrRC.empty())
		m_unFlags &= ~(1 << 3);
	else
		m_unFlags |= (1 << 3);

	// 4 - Дата создания - CreationDate
	m_sCreationDate = DictLookupString(&oAnnot, "CreationDate", 4);

	// 5 - Ссылка на аннотацию ответ - IRT
	if (oAnnot.dictLookupNF("IRT", &oObj)->isRef())
	{
		m_unFlags |= (1 << 5);
		m_unRefNumIRT = oObj.getRefNum();
	}
	oObj.free();

	// 6 - Тип аннотации ответа - RT
	if (oAnnot.dictLookup("RT", &oObj)->isName())
	{
		m_unFlags |= (1 << 6);
		m_nRT = 1; // Group
		if (oObj.isName("R"))
			m_nRT = 0;
	}
	oObj.free();

	// 7 - Краткое описание - Subj
	m_sSubj = DictLookupString(&oAnnot, "Subj", 7);

	oAnnot.free();
}
CAnnotMarkup::~CAnnotMarkup()
{
	for (int i = 0; i < m_arrRC.size(); ++i)
		RELEASEOBJECT(m_arrRC[i]);
}
void ReadFontData(const std::string& sData, CAnnotMarkup::CFontData* pFont)
{
	size_t nSemicolon = 0;
	size_t nColon = sData.find(':');
	while (nColon != std::string::npos && nColon > nSemicolon)
	{
		std::string sProperty = sData.substr(nSemicolon, nColon - nSemicolon);
		nSemicolon = sData.find(';', nSemicolon);
		nColon++;
		std::string sValue = sData.substr(nColon, nSemicolon - nColon);
		nColon = sData.find(':', nSemicolon);
		nSemicolon++;

		if (sProperty == "font-size")
			pFont->dFontSise = std::stod(sValue);
		else if (sProperty == "text-align")
		{
			// 0 start / left
			if (sValue == "center" || sValue == "middle")
				pFont->nAlign = 1;
			else if (sValue == "right" || sValue == "end")
				pFont->nAlign = 2;
			else if (sValue == "justify")
				pFont->nAlign = 3;
		}
		else if (sProperty == "color")
		{
			if (sValue[0] == '#')
			{
				sValue = sValue.substr(1);
				BYTE nColor1 = 0, nColor2 = 0, nColor3 = 0;
				if (sValue.length() == 6)
					sscanf(sValue.c_str(), "%2hhx%2hhx%2hhx", &nColor1, &nColor2, &nColor3);
				else if (sValue.length() == 3)
				{
					sscanf(sValue.c_str(), "%1hhx%1hhx%1hhx", &nColor1, &nColor2, &nColor3);
					nColor1 *= 17;
					nColor2 *= 17;
					nColor3 *= 17;
				}

				pFont->dColor[0] = (double)nColor1 / 255.0;
				pFont->dColor[1] = (double)nColor2 / 255.0;
				pFont->dColor[2] = (double)nColor3 / 255.0;
			}
		}
		else if (sProperty == "font-weight")
		{
			// 0 normal / 300 / 400 / 500
			if (sValue == "normal" || sValue == "300" || sValue == "400" || sValue == "500")
				pFont->unFontFlags &= ~(1 << 0);
			else if (sValue == "bold" || sValue == "bolder" || sValue == "600" || sValue == "700" || sValue == "800" || sValue == "900")
				pFont->unFontFlags |= (1 << 0);
		}
		else if (sProperty == "font-style")
		{
			// 0 normal
			if (sValue == "normal")
				pFont->unFontFlags &= ~(1 << 1);
			else if (sValue == "italic" || sValue.find("oblique") != std::string::npos)
				pFont->unFontFlags |= (1 << 1);
		}
		else if (sProperty == "font-family")
			pFont->sFontFamily = sValue[0] == '\'' ? sValue.substr(1, sValue.length() - 2) : sValue;
		else if (sProperty == "text-decoration")
		{
			if (sValue.find("line-through") != std::string::npos)
				pFont->unFontFlags |= (1 << 3);
			if (sValue.find("word") != std::string::npos || sValue.find("underline") != std::string::npos)
				pFont->unFontFlags |= (1 << 4);
			if (sValue.find("none") != std::string::npos)
			{
				pFont->unFontFlags &= ~(1 << 3);
				pFont->unFontFlags &= ~(1 << 4);
			}
		}
		else if (sProperty == "vertical-align")
		{
			pFont->unFontFlags |= (1 << 5);
			pFont->dVAlign = std::stod(sValue);
			if (pFont->dVAlign == 0 && sValue[0] == '-')
				pFont->dVAlign = -0.01;
		}
		// font-stretch
	}
}
std::vector<CAnnotMarkup::CFontData*> CAnnotMarkup::ReadRC(const std::string& sRC)
{
	std::vector<CAnnotMarkup::CFontData*> arrRC;

	XmlUtils::CXmlLiteReader oLightReader;
	if (sRC.empty() || !oLightReader.FromStringA(sRC) || !oLightReader.ReadNextNode() || oLightReader.GetNameA() != "body")
		return arrRC;

	CAnnotMarkup::CFontData oFontBase;
	while (oLightReader.MoveToNextAttribute())
	{
		if (oLightReader.GetNameA() == "style")
		{
			ReadFontData(oLightReader.GetTextA(), &oFontBase);
			break;
		}
	}
	oLightReader.MoveToElement();

	int nDepthP = oLightReader.GetDepth();
	while (oLightReader.ReadNextSiblingNode2(nDepthP))
	{
		if (oLightReader.GetNameA() != "p")
			continue;

		int nDepthSpan = oLightReader.GetDepth();
		if (oLightReader.IsEmptyNode() || !oLightReader.ReadNextSiblingNode2(nDepthSpan))
			continue;

		do
		{
			std::string sName = oLightReader.GetNameA();
			if (sName == "span")
			{
				CAnnotMarkup::CFontData* pFont = new CAnnotMarkup::CFontData(oFontBase);
				while (oLightReader.MoveToNextAttribute())
				{
					if (oLightReader.GetNameA() == "style")
					{
						ReadFontData(oLightReader.GetTextA(), pFont);
						break;
					}
				}
				oLightReader.MoveToElement();

				pFont->sText = oLightReader.GetText2A();
				arrRC.push_back(pFont);
			}
			else if (sName == "#text")
			{
				CAnnotMarkup::CFontData* pFont = new CAnnotMarkup::CFontData(oFontBase);
				pFont->sText = oLightReader.GetTextA();
				arrRC.push_back(pFont);
			}
		} while (oLightReader.ReadNextSiblingNode2(nDepthSpan));
	}

	return arrRC;
}
void CAnnotMarkup::SetFont(PDFDoc* pdfDoc, Object* oAnnotRef, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList)
{
	CAnnotFonts::GetFreeTextFont(pdfDoc, pFontManager, pFontList, oAnnotRef, m_arrRC);
}

//------------------------------------------------------------------------
// Annot
//------------------------------------------------------------------------

CAnnot::CAnnot(PDFDoc* pdfDoc, AcroFormField* pField)
{
	m_pBorder = NULL;
	m_unAnnotFlag = 0;
	m_unAFlags = 0;
	m_unFlags = 0;

	Object oObj;
	pField->getFieldRef(&oObj);
	m_unRefNum = oObj.getRefNum();
	oObj.free();

	// Флаг аннотации - F
	if (pField->fieldLookup("F", &oObj)->isInt())
		m_unAnnotFlag = oObj.getInt();
	oObj.free();

	// Номер страницы - P
	m_unPage = pField->getPageNum();

	// Координаты - Rect
	pField->getBBox(&m_pRect[0], &m_pRect[1], &m_pRect[2], &m_pRect[3]);
	PDFRectangle* pCropBox = pdfDoc->getCatalog()->getPage(m_unPage)->getCropBox();
	m_dHeight = pCropBox->y2;
	m_dX = pCropBox->x1;
	double dTemp = m_pRect[1];
	m_pRect[0] = m_pRect[0] - m_dX;
	m_pRect[1] = m_dHeight - m_pRect[3];
	m_pRect[2] = m_pRect[2] - m_dX;
	m_pRect[3] = m_dHeight - dTemp;

	// 0 - Уникальное имя - NM
	if (pField->fieldLookup("NM", &oObj)->isString())
	{
		m_unAFlags |= (1 << 0);
		TextString* s = new TextString(oObj.getString());
		m_sNM = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();

	// 1 - Альтернативный текст - Contents
	if (pField->fieldLookup("Contents", &oObj)->isString())
	{
		m_unAFlags |= (1 << 1);
		TextString* s = new TextString(oObj.getString());
		m_sContents = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();

	// 2 - Эффекты границы - BE
	if (pField->fieldLookup("BE", &oObj)->isDict())
	{
		Object oBorderBE;
		m_unAFlags |= (1 << 2);

		m_pBE.first = 0;
		if (oObj.dictLookup("S", &oBorderBE)->isName("C"))
			m_pBE.first = 1;
		oBorderBE.free();

		m_pBE.second = 0;
		if (oObj.dictLookup("I", &oBorderBE)->isNum())
			m_pBE.second = oBorderBE.getNum();
		oBorderBE.free();
	}
	oObj.free();

	// 3 - Специальный цвет для аннотации - C
	if (pField->fieldLookup("C", &oObj)->isArray())
	{
		m_unAFlags |= (1 << 3);
		int nCLength = oObj.arrayGetLength();
		for (int j = 0; j < nCLength; ++j)
		{
			Object oCj;
			m_arrC.push_back(oObj.arrayGet(j, &oCj)->isNum() ? oCj.getNum() : 0.0);
			oCj.free();
		}
	}
	oObj.free();

	// 4 - Границы и Dash Pattern - Border/BS
	m_pBorder = NULL;
	if (pField->fieldLookup("BS", &oObj)->isDict())
		m_pBorder = getBorder(&oObj, true);
	else
	{
		oObj.free();
		if (pField->fieldLookup("Border", &oObj)->isArray() && oObj.arrayGetLength() > 2)
			m_pBorder = getBorder(&oObj, false);
	}
	oObj.free();
	if (m_pBorder && m_pBorder->nType != 5)
		m_unAFlags |= (1 << 4);

	// 5 - Дата последнего изменения - M
	if (pField->fieldLookup("M", &oObj)->isString())
	{
		m_unAFlags |= (1 << 5);
		TextString* s = new TextString(oObj.getString());
		m_sM = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();

	// 6 - Наличие/Отсутствие внешнего вида
	if (pField->fieldLookup("AP", &oObj)->isDict() && oObj.dictGetLength())
		m_unAFlags |= (1 << 6);
	oObj.free();

	// 7 - User ID
	if (pField->fieldLookup("OUserID", &oObj)->isString())
	{
		m_unAFlags |= (1 << 7);
		TextString* s = new TextString(oObj.getString());
		m_sOUserID = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();
}
CAnnot::CAnnot(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex)
{
	m_pBorder = NULL;
	m_unAnnotFlag = 0;
	m_unAFlags = 0;
	m_unFlags = 0;

	Object oAnnot, oObj, oObj2;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// Номер объекта аннотации
	m_unRefNum = oAnnotRef->getRefNum();

	// Флаг аннотации - F
	if (oAnnot.dictLookup("F", &oObj)->isInt())
		m_unAnnotFlag = oObj.getInt();
	oObj.free();

	// Номер страницы - P
	m_unPage = nPageIndex;
	PDFRectangle* pCropBox = pdfDoc->getCatalog()->getPage(m_unPage)->getCropBox();
	m_dHeight = pCropBox->y2;
	m_dX = pCropBox->x1;

	// Координаты - Rect
	m_pRect[0] = 0.0, m_pRect[1] = 0.0, m_pRect[2] = 0.0, m_pRect[3] = 0.0;
	if (oAnnot.dictLookup("Rect", &oObj)->isArray() && oObj.arrayGetLength() == 4)
	{
		m_pRect[0] = ArrGetNum(&oObj, 0) - m_dX;
		m_pRect[1] = m_dHeight - ArrGetNum(&oObj, 3);
		m_pRect[2] = ArrGetNum(&oObj, 2) - m_dX;
		m_pRect[3] = m_dHeight - ArrGetNum(&oObj, 1);
	}
	oObj.free();

	// 0 - Уникальное имя - NM
	if (oAnnot.dictLookup("NM", &oObj)->isString())
	{
		m_unAFlags |= (1 << 0);
		TextString* s = new TextString(oObj.getString());
		m_sNM = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();

	// 1 - Альтернативный текст - Contents
	if (oAnnot.dictLookup("Contents", &oObj)->isString())
	{
		m_unAFlags |= (1 << 1);
		TextString* s = new TextString(oObj.getString());
		m_sContents = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();

	// 2 - Эффекты границы - BE
	if (oAnnot.dictLookup("BE", &oObj)->isDict())
	{
		Object oBorderBE;
		m_unAFlags |= (1 << 2);

		m_pBE.first = 0;
		if (oObj.dictLookup("S", &oBorderBE)->isName("C"))
			m_pBE.first = 1;
		oBorderBE.free();

		m_pBE.second = 0;
		if (oObj.dictLookup("I", &oBorderBE)->isNum())
			m_pBE.second = oBorderBE.getNum();
		oBorderBE.free();
	}
	oObj.free();

	// 3 - Цвет - C
	if (oAnnot.dictLookup("C", &oObj)->isArray())
	{
		m_unAFlags |= (1 << 3);
		int nBCLength = oObj.arrayGetLength();
		for (int j = 0; j < nBCLength; ++j)
		{
			m_arrC.push_back(oObj.arrayGet(j, &oObj2)->isNum() ? oObj2.getNum() : 0.0);
			oObj2.free();
		}
	}
	oObj.free();

	// 4 - Границы и Dash Pattern - Border/BS
	m_pBorder = NULL;
	if (oAnnot.dictLookup("BS", &oObj)->isDict())
		m_pBorder = getBorder(&oObj, true);
	else
	{
		oObj.free();
		if (oAnnot.dictLookup("Border", &oObj)->isArray() && oObj.arrayGetLength() > 2)
			m_pBorder = getBorder(&oObj, false);
	}
	oObj.free();
	if (m_pBorder && m_pBorder->nType != 5)
		m_unAFlags |= (1 << 4);

	// 5 - Дата последнего изменения - M
	if (oAnnot.dictLookup("M", &oObj)->isString())
	{
		m_unAFlags |= (1 << 5);
		TextString* s = new TextString(oObj.getString());
		m_sM = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();

	// 6 - Наличие/Отсутствие внешнего вида
	if (oAnnot.dictLookup("AP", &oObj)->isDict() && oObj.dictGetLength())
		m_unAFlags |= (1 << 6);
	oObj.free();

	// 7 - User ID
	if (oAnnot.dictLookup("OUserID", &oObj)->isString())
	{
		m_unAFlags |= (1 << 7);
		TextString* s = new TextString(oObj.getString());
		m_sOUserID = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();

	oAnnot.free();
}
CAnnot::~CAnnot()
{
	RELEASEOBJECT(m_pBorder);
}
std::string CAnnot::DictLookupString(Object* pObj, const char* sName, int nByte)
{
	std::string sRes;
	Object oObj;
	if (pObj->dictLookup(sName, &oObj)->isString())
	{
		m_unFlags |= (1 << nByte);
		TextString* s = new TextString(oObj.getString());
		sRes = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();
	return sRes;
}

//------------------------------------------------------------------------
// AP
//------------------------------------------------------------------------

CAnnotAP::CAnnotAP(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList*  pFontList, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, const char* sView, const char* sButtonView, AcroFormField* pField)
{
	m_gfx = NULL;
	m_pFrame = NULL;
	m_pRendererOut = NULL;
	m_pRenderer = NULL;
	m_dRWScale = 1;
	m_dRHScale = 1;
	m_bIsStamp = false;

	Object oAP;
	if (pField->fieldLookup("AP", &oAP)->isDict() && oAP.dictGetLength())
	{
		Init(pField);
		Init(pdfDoc, pFontManager, pFontList, nRasterW, nRasterH, nBackgroundColor, nPageIndex);
		Draw(pdfDoc, &oAP, nRasterH, nBackgroundColor, nPageIndex, pField, sView, sButtonView);
	}
	oAP.free();

	Clear();
}
CAnnotAP::CAnnotAP(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList*  pFontList, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, const char* sView, Object* oAnnotRef)
{
	m_gfx = NULL;
	m_pFrame = NULL;
	m_pRendererOut = NULL;
	m_pRenderer = NULL;
	m_dRWScale = 1;
	m_dRHScale = 1;

	Object oAnnot, oAP, oSubtype;
	XRef* xref = pdfDoc->getXRef();
	oAnnotRef->fetch(xref, &oAnnot);
	m_bIsStamp = oAnnot.dictLookup("Subtype", &oSubtype)->isName("Stamp") == gTrue;
	if (oAnnot.dictLookup("AP", &oAP)->isDict())
	{
		m_unRefNum = oAnnotRef->getRefNum();

		Init(&oAnnot);
		Init(pdfDoc, pFontManager, pFontList, nRasterW, nRasterH, nBackgroundColor, nPageIndex);
		Draw(pdfDoc, &oAP, nRasterH, nBackgroundColor, oAnnotRef, sView);
	}
	oAP.free(); oAnnot.free(); oSubtype.free();

	Clear();
}
CAnnotAP::~CAnnotAP()
{
	Clear();

	for (int i = 0; i < m_arrAP.size(); ++i)
	{
		RELEASEOBJECT(m_arrAP[i]);
	}
}
void CAnnotAP::Clear()
{
	RELEASEOBJECT(m_gfx);
	RELEASEOBJECT(m_pFrame);
	RELEASEOBJECT(m_pRendererOut);
	RELEASEOBJECT(m_pRenderer);
}
void CAnnotAP::Init(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList*  pFontList, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex)
{
	Page* pPage = pdfDoc->getCatalog()->getPage(nPageIndex + 1);
	PDFRectangle* pCropBox = pPage->getCropBox();
	m_dCropX = pCropBox->x1;
	m_dCropY = pCropBox->y1;

	double dWidth  = round(pdfDoc->getPageCropWidth(nPageIndex + 1));
	double dHeight = round(pdfDoc->getPageCropHeight(nPageIndex + 1));
	double dRasterW = (double)nRasterW * m_dRWScale;
	double dRasterH = (double)nRasterH * m_dRHScale;

	m_dWScale = dRasterW / dWidth;
	m_dHScale = dRasterH / dHeight;
	m_nWidth  = (int)round((m_dx2 - m_dx1) * m_dWScale);
	m_nHeight = (int)round((m_dy2 - m_dy1) * m_dHScale);
	m_dWTale = m_nWidth  - (m_dx2 - m_dx1) * m_dWScale;
	m_dHTale = m_nHeight - (m_dy2 - m_dy1) * m_dHScale;

	m_nWidth  += 2;
	m_nHeight += 2;

	// Отрисовка на прозрачном холсте с заданым цветом фона
	BYTE* pBgraData = new BYTE[m_nWidth * m_nHeight * 4];
	unsigned int nColor = (unsigned int)nBackgroundColor;
	unsigned int nSize = (unsigned int)(m_nWidth * m_nHeight);
	unsigned int* pTemp = (unsigned int*)pBgraData;
	for (unsigned int i = 0; i < nSize; ++i)
		*pTemp++ = nColor;

	m_pFrame = new CBgraFrame();
	m_pFrame->put_Data(pBgraData);
	m_pFrame->put_Width(m_nWidth);
	m_pFrame->put_Height(m_nHeight);
	m_pFrame->put_Stride(4 * m_nWidth);

	m_pRenderer = NSGraphics::Create();
	m_pRenderer->SetFontManager(pFontManager);
	m_pRenderer->CreateFromBgraFrame(m_pFrame);
	m_pRenderer->SetSwapRGB(true);
	m_pRenderer->put_Width ((m_dx2 - m_dx1 + (2 + m_dWTale) * dWidth  / dRasterW) * 25.4 / 72.0);
	m_pRenderer->put_Height((m_dy2 - m_dy1 + (2 + m_dHTale) * dHeight / dRasterH) * 25.4 / 72.0);
	if (nBackgroundColor != 0xFFFFFF)
		m_pRenderer->CommandLong(c_nDarkMode, 1);
	m_pRenderer->CommandLong(c_nPenWidth0As1px, 1);

	m_pRendererOut = new RendererOutputDev(m_pRenderer, pFontManager, pFontList);
	m_pRendererOut->NewPDF(pdfDoc->getXRef());

	// Создание Gfx
	GBool crop = gTrue;
	PDFRectangle box;
	// Поворот не требуется
	pPage->makeBox(72.0, 72.0, 0, gFalse, m_pRendererOut->upsideDown(), -1, -1, -1, -1, &box, &crop);
	PDFRectangle* cropBox = pPage->getCropBox();

	m_gfx = new Gfx(pdfDoc, m_pRendererOut, nPageIndex + 1, pPage->getAttrs()->getResourceDict(), 72.0, 72.0, &box, crop ? cropBox : (PDFRectangle *)NULL, 0, NULL, NULL);

	// Координаты внешнего вида
	m_dRx1 = (m_dx1 - m_dCropX) * m_dWScale - 1;
	m_dRy1 = (pdfDoc->getPageCropHeight(nPageIndex + 1) - m_dy2 + m_dCropY) * m_dHScale - 1;
}
void CAnnotAP::Init(AcroFormField* pField)
{
	// Номер аннотации для сопоставления с AP
	Object oRef;
	pField->getFieldRef(&oRef);
	m_unRefNum = oRef.getRefNum();
	oRef.free();

	// Координаты - BBox
	pField->getBBox(&m_dx1, &m_dy1, &m_dx2, &m_dy2);
}
void CAnnotAP::Init(Object* oAnnot)
{
	Object oObj, oObj2;
	if (oAnnot->dictLookup("Rect", &oObj)->isArray() && oObj.arrayGetLength() == 4)
	{
		m_dx1 = ArrGetNum(&oObj, 0);
		m_dy1 = ArrGetNum(&oObj, 1);
		m_dx2 = ArrGetNum(&oObj, 2);
		m_dy2 = ArrGetNum(&oObj, 3);

		if (m_bIsStamp)
		{
			double m[6] = { 1, 0, 0, 1, 0, 0 }, bbox[4] = { m_dx1, m_dy1, m_dx2, m_dy2 };

			oObj.free();
			Object oAP;
			if (oAnnot->dictLookup("AP", &oAP)->isDict() && oAP.dictLookup("N", &oObj2)->isStream())
			{
				Object oObj1;
				if (oObj2.streamGetDict()->lookup("BBox", &oObj)->isArray() && oObj.arrayGetLength() == 4)
				{
					for (int i = 0; i < 4; ++i)
					{
						oObj.arrayGet(i, &oObj1);
						bbox[i] = oObj1.isNum() ? oObj1.getNum() : 0;
						oObj1.free();
					}
				}

				oObj.free();
				if (oObj2.streamGetDict()->lookup("Matrix", &oObj)->isArray() && oObj.arrayGetLength() == 6)
				{
					for (int i = 0; i < 6; ++i)
					{
						oObj.arrayGet(i, &oObj1);
						m[i] = oObj1.getNum();
						oObj1.free();
					}
				}
			}
			oAP.free(); oObj2.free();

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
				sx = (m_dx2 - m_dx1) / (formXMax - formXMin);
			if (formYMin == formYMax)
				sy = 1;
			else
				sy = (m_dy2 - m_dy1) / (formYMax - formYMin);

			m_dx1 = bbox[0];
			m_dy1 = bbox[1];
			m_dx2 = bbox[2];
			m_dy2 = bbox[3];
			m_dRWScale = sx;
			m_dRHScale = sy;
		}

		double dTemp;
		if (m_dx1 > m_dx2)
		{
			dTemp = m_dx1; m_dx1 = m_dx2; m_dx2 = dTemp;
		}
		if (m_dy1 > m_dy2)
		{
			dTemp = m_dy1; m_dy1 = m_dy2; m_dy2 = dTemp;
		}
	}
	oObj.free();
}
void CAnnotAP::Draw(PDFDoc* pdfDoc, Object* oAP, int nRasterH, int nBackgroundColor, int nPageIndex, AcroFormField* pField, const char* sView, const char* sButtonView)
{
	// Отрисовка внешних видов аннотации
	AcroFormFieldType oType = pField->getAcroFormFieldType();
	((GlobalParamsAdaptor*)globalParams)->setDrawFormField(true);

	double dOffsetX = -(m_dx1 - m_dCropX) * m_dWScale + 1 + m_dWTale / 2;
	double dOffsetY = (m_dy2 - m_dCropY) * m_dHScale - nRasterH + 1 + m_dHTale / 2;
	nPageIndex++;

	std::vector<const char*> arrAPName { "N", "D", "R" };
	for (unsigned int j = 0; j < arrAPName.size(); ++j)
	{
		if (sView && strcmp(sView, arrAPName[j]) != 0)
			continue;

		Object oObj;
		if (oAP->dictLookup(arrAPName[j], &oObj)->isDict())
		{
			for (int k = 0; k < oObj.dictGetLength(); ++k)
			{
				if (sButtonView)
				{
					if (strcmp(sButtonView, "Off") == 0 && strcmp(oObj.dictGetKey(k), "Off") != 0)
						continue;
					if (strcmp(sButtonView, "Yes") == 0 && strcmp(oObj.dictGetKey(k), "Off") == 0)
						continue;
				}
				CAnnotAPView* pView = new CAnnotAPView();
				pView->sAPName = arrAPName[j];
				pView->sASName = oObj.dictGetKey(k);
				if ((oType == acroFormFieldRadioButton || oType == acroFormFieldCheckbox) && pView->sASName != "Off")
					pView->sASName = "Yes";
				m_pRenderer->SetCoordTransformOffset(dOffsetX, dOffsetY);
				DrawAppearance(pdfDoc, nPageIndex, pField, m_gfx, arrAPName[j], pView->sASName.c_str());
				WriteAppearance(nBackgroundColor, pView);
				pView->nBlendMode = GetBlendMode();
				m_arrAP.push_back(pView);
			}
		}
		else if (!oObj.isNull())
		{
			CAnnotAPView* pView = new CAnnotAPView();
			pView->sAPName = arrAPName[j];
			m_pRenderer->SetCoordTransformOffset(dOffsetX, dOffsetY);
			DrawAppearance(pdfDoc, nPageIndex, pField, m_gfx, arrAPName[j], NULL);
			WriteAppearance(nBackgroundColor, pView);
			pView->nBlendMode = GetBlendMode();
			m_arrAP.push_back(pView);
		}
		oObj.free();
	}

	((GlobalParamsAdaptor*)globalParams)->setDrawFormField(false);
}
void CAnnotAP::Draw(PDFDoc* pdfDoc, Object* oAP, int nRasterH, int nBackgroundColor, Object* oAnnotRef, const char* sView)
{
	((GlobalParamsAdaptor*)globalParams)->setDrawFormField(true);
	// Отрисовка внешних видов аннотации
	Object oAnnot;
	XRef* xref = pdfDoc->getXRef();
	oAnnotRef->fetch(xref, &oAnnot);

	double dOffsetX = -(m_dx1 - m_dCropX) * m_dWScale + 1 + m_dWTale / 2;
	double dOffsetY = (m_dy2 - m_dCropY) * m_dHScale - (double)nRasterH * m_dRHScale + 1 + m_dHTale / 2;

	std::vector<const char*> arrAPName { "N", "D", "R" };
	for (unsigned int j = 0; j < arrAPName.size(); ++j)
	{
		if (sView && strcmp(sView, arrAPName[j]) != 0)
			continue;
		Object oObj;
		if (oAP->dictLookup(arrAPName[j], &oObj)->isStream())
		{
			CAnnotAPView* pView = new CAnnotAPView();
			pView->sAPName = arrAPName[j];

			m_pRenderer->SetCoordTransformOffset(dOffsetX, dOffsetY);

			Ref ref = oAnnotRef->getRef();
			Annot* annot = new Annot(pdfDoc, oAnnot.getDict(), &ref, arrAPName[j]);
			if (annot)
			{
				annot->generateAnnotAppearance();
				annot->draw(m_gfx, gFalse);
			}
			RELEASEOBJECT(annot);

			WriteAppearance(nBackgroundColor, pView);
			pView->nBlendMode = GetBlendMode();
			m_arrAP.push_back(pView);
		}
		oObj.free();
	}
	oAnnot.free();

	((GlobalParamsAdaptor*)globalParams)->setDrawFormField(false);
}
void CAnnotAP::WriteAppearance(unsigned int nColor, CAnnotAPView* pView)
{
	BYTE* pSubMatrix = new BYTE[m_nWidth * m_nHeight * 4];
	int p = 0;
	unsigned int* pTemp = (unsigned int*)m_pFrame->get_Data();
	unsigned int* pSubTemp = (unsigned int*)pSubMatrix;
	for (int y = 0; y < m_nHeight; ++y)
	{
		for (int x = 0; x < m_nWidth; ++x)
		{
			pSubTemp[p++] = pTemp[y * m_nWidth + x];
			pTemp[y * m_nWidth + x] = nColor;
		}
	}

	pView->pAP = pSubMatrix;
}
BYTE CAnnotAP::GetBlendMode()
{
	return 0;
}

//------------------------------------------------------------------------
// ToWASM
//------------------------------------------------------------------------

void CAnnotAP::ToWASM(NSWasm::CData& oRes)
{
	if (m_arrAP.empty())
		return;

	oRes.AddInt(m_unRefNum);
	oRes.AddDouble(m_dRx1);
	oRes.AddDouble(m_dRy1);
	oRes.AddInt(m_nWidth);
	oRes.AddInt(m_nHeight);
	oRes.AddInt((unsigned int)m_arrAP.size());
	for (int i = 0; i < m_arrAP.size(); ++i)
	{
		oRes.WriteString(m_arrAP[i]->sAPName);
		oRes.WriteString(m_arrAP[i]->sASName);

		unsigned long long npSubMatrix = (unsigned long long)m_arrAP[i]->pAP;
		unsigned int npSubMatrix1 = npSubMatrix & 0xFFFFFFFF;
		oRes.AddInt(npSubMatrix1);
		oRes.AddInt(npSubMatrix >> 32);

		oRes.WriteBYTE(m_arrAP[i]->nBlendMode);
	}
}
void CAnnots::ToWASM(NSWasm::CData& oRes)
{
	// Порядок вычислений - CO
	oRes.AddInt(m_arrCO.size());
	for (int i = 0; i < m_arrCO.size(); ++i)
		oRes.AddInt(m_arrCO[i]);

	// Родительские Fields
	oRes.AddInt(m_arrParents.size());
	for (int i = 0; i < m_arrParents.size(); ++i)
		m_arrParents[i]->ToWASM(oRes);

	oRes.AddInt(m_arrAnnots.size());
	for (int i = 0; i < m_arrAnnots.size(); ++i)
		m_arrAnnots[i]->ToWASM(oRes);
}
void CAnnots::CAnnotParent::ToWASM(NSWasm::CData& oRes)
{
	oRes.AddInt(unRefNum);
	oRes.AddInt(unFlags);
	if (unFlags & (1 << 0))
		oRes.WriteString(sT);
	if (unFlags & (1 << 1))
		oRes.WriteString(sV);
	if (unFlags & (1 << 2))
		oRes.WriteString(sDV);
	if (unFlags & (1 << 3))
	{
		oRes.AddInt((unsigned int)arrI.size());
		for (int i = 0; i < arrI.size(); ++i)
			oRes.AddInt(arrI[i]);
	}
	if (unFlags & (1 << 4))
		oRes.AddInt(unRefNumParent);
	if (unFlags & (1 << 5))
	{
		oRes.AddInt((unsigned int)arrV.size());
		for (int i = 0; i < arrV.size(); ++i)
			oRes.WriteString(arrV[i]);
	}
	if (unFlags & (1 << 6))
	{
		oRes.AddInt((unsigned int)arrOpt.size());
		for (int i = 0; i < arrOpt.size(); ++i)
			oRes.WriteString(arrOpt[i]);
	}
}
void CAnnot::ToWASM(NSWasm::CData& oRes)
{
	oRes.AddInt(m_unRefNum);
	oRes.AddInt(m_unAnnotFlag);
	oRes.AddInt(m_unPage - 1);
	for (int i = 0; i < 4; ++i)
		oRes.WriteDouble(m_pRect[i]);
	oRes.AddInt(m_unAFlags);
	if (m_unAFlags & (1 << 0))
		oRes.WriteString(m_sNM);
	if (m_unAFlags & (1 << 1))
		oRes.WriteString(m_sContents);
	if (m_unAFlags & (1 << 2))
	{
		oRes.WriteBYTE(m_pBE.first);
		oRes.AddDouble(m_pBE.second);
	}
	if (m_unAFlags & (1 << 3))
	{
		oRes.AddInt((unsigned int)m_arrC.size());
		for (int i = 0; i < m_arrC.size(); ++i)
			oRes.WriteDouble(m_arrC[i]);
	}
	if (m_pBorder && (m_unAFlags & (1 << 4)))
		m_pBorder->ToWASM(oRes);
	if (m_unAFlags & (1 << 5))
		oRes.WriteString(m_sM);
	if (m_unAFlags & (1 << 7))
		oRes.WriteString(m_sOUserID);
}
void CAnnot::CBorderType::ToWASM(NSWasm::CData& oRes)
{
	BYTE nBP = nType;
	if (nBP == 1)
		nBP = 2;
	else if (nBP == 2)
		nBP = 1;

	oRes.WriteBYTE(nBP);
	oRes.AddDouble(dWidth);
	if (nType == annotBorderDashed)
	{
		oRes.AddInt(arrDash.size());
		for (int i = 0; i < arrDash.size(); ++i)
			oRes.AddDouble(arrDash[i]);
	}
}
void CAnnotWidget::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(m_nType);

	CAnnot::ToWASM(oRes);

	oRes.WriteString(m_sFontName);
	oRes.AddDouble(m_dFontSize);
	oRes.AddInt(m_unFontStyle);
	oRes.AddInt(m_arrTC.size());
	for (int i = 0; i < m_arrTC.size(); ++i)
		oRes.WriteDouble(m_arrTC[i]);
	oRes.WriteBYTE(m_nQ);
	oRes.AddInt(m_unFieldFlag);
	oRes.AddInt(m_unFlags);
	if (m_unFlags & (1 << 0))
		oRes.WriteString(m_sTU);
	if (m_unFlags & (1 << 1))
		oRes.WriteString(m_sDS);
	if (m_unFlags & (1 << 2))
		oRes.WriteString(m_sActualFontName);
	if (m_unFlags & (1 << 3))
		oRes.WriteBYTE(m_nH);
	if (m_unFlags & (1 << 4))
		oRes.WriteString(m_sFontKey);
	if (m_unFlags & (1 << 5))
	{
		oRes.AddInt(m_arrBC.size());
		for (int i = 0; i < m_arrBC.size(); ++i)
			oRes.WriteDouble(m_arrBC[i]);
	}
	if (m_unFlags & (1 << 6))
		oRes.AddInt(m_unR);
	if (m_unFlags & (1 << 7))
	{
		oRes.AddInt(m_arrBG.size());
		for (int i = 0; i < m_arrBG.size(); ++i)
			oRes.WriteDouble(m_arrBG[i]);
	}
	if (m_unFlags & (1 << 8))
		oRes.WriteString(m_sDV);
	if (m_unFlags & (1 << 17))
		oRes.AddInt(m_unRefNumParent);
	if (m_unFlags & (1 << 18))
		oRes.WriteString(m_sT);
	if (m_unFlags & (1 << 19))
		oRes.WriteString(m_sButtonFontName);
	oRes.AddInt(m_arrAction.size());
	for (int i = 0; i < m_arrAction.size(); ++i)
	{
		oRes.WriteString(m_arrAction[i]->sType);
		m_arrAction[i]->ToWASM(oRes);
	}
}
void CActionGoTo::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(1);
	oRes.AddInt(unPage);
	oRes.WriteBYTE(nKind);
	switch (nKind)
	{
	case destXYZ:
	case destFitH:
	case destFitBH:
	case destFitV:
	case destFitBV:
	{
		oRes.WriteBYTE(unKindFlag);
		if (unKindFlag & (1 << 0))
			oRes.AddDouble(pRect[0]);
		if (unKindFlag & (1 << 1))
			oRes.AddDouble(pRect[1]);
		if (unKindFlag & (1 << 2))
			oRes.AddDouble(pRect[3]);
		break;
	}
	case destFitR:
	{
		oRes.AddDouble(pRect[0]);
		oRes.AddDouble(pRect[1]);
		oRes.AddDouble(pRect[2]);
		oRes.AddDouble(pRect[3]);
		break;
	}
	case destFit:
	case destFitB:
	default:
		break;
	}

	CAction::ToWASM(oRes);
}
void CActionURI::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(6);
	oRes.WriteString(sURI);

	CAction::ToWASM(oRes);
}
void CActionNamed::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(10);
	oRes.WriteString(sNamed);

	CAction::ToWASM(oRes);
}
void CActionJavaScript::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(14);
	oRes.WriteString(sJavaScript);

	CAction::ToWASM(oRes);
}
void CActionHide::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(9);
	oRes.WriteBYTE(bHideFlag ? 1 : 0);
	oRes.AddInt(arrAnnotName.size());
	for (int i = 0; i < arrAnnotName.size(); ++i)
		oRes.WriteString(arrAnnotName[i]);

	CAction::ToWASM(oRes);
}
void CActionResetForm::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(12);
	oRes.AddInt(unFlags);
	oRes.AddInt(arrAnnotName.size());
	for (int i = 0; i < arrAnnotName.size(); ++i)
		oRes.WriteString(arrAnnotName[i]);

	CAction::ToWASM(oRes);
}
void CAction::ToWASM(NSWasm::CData& oRes)
{
	if (pNext)
	{
		oRes.WriteBYTE(1);
		pNext->ToWASM(oRes);
	}
	else
		oRes.WriteBYTE(0);
}
void CAnnotWidgetBtn::ToWASM(NSWasm::CData& oRes)
{
	CAnnotWidget::ToWASM(oRes);

	if (m_unFlags & (1 << 9))
		oRes.WriteString(m_sV);
	if (m_nType == 27)
	{
		oRes.AddInt(m_unIFFlag);
		if (m_unFlags & (1 << 10))
			oRes.WriteString(m_sCA);
		if (m_unFlags & (1 << 11))
			oRes.WriteString(m_sRC);
		if (m_unFlags & (1 << 12))
			oRes.WriteString(m_sAC);
		if (m_unFlags & (1 << 13))
			oRes.WriteBYTE(m_nTP);
		if (m_unIFFlag & (1 << 0))
		{
			if (m_unIFFlag & (1 << 1))
				oRes.WriteBYTE(m_nSW);
			if (m_unIFFlag & (1 << 2))
				oRes.WriteBYTE(m_nS);
			if (m_unIFFlag & (1 << 3))
			{
				oRes.AddDouble(m_dA1);
				oRes.AddDouble(m_dA2);
			}
		}
	}
	else
	{
		oRes.WriteBYTE(m_nStyle);
		if (m_unFlags & (1 << 14))
			oRes.WriteString(m_sAP_N_Yes);
	}
}
void CAnnotWidgetTx::ToWASM(NSWasm::CData& oRes)
{
	CAnnotWidget::ToWASM(oRes);

	if (m_unFlags & (1 << 9))
		oRes.WriteString(m_sV);
	if (m_unFlags & (1 << 10))
		oRes.AddInt(m_unMaxLen);
	if (m_unFieldFlag & (1 << 25))
		oRes.WriteString(m_sRV);
}
void CAnnotWidgetCh::ToWASM(NSWasm::CData& oRes)
{
	CAnnotWidget::ToWASM(oRes);

	if (m_unFlags & (1 << 9))
		oRes.WriteString(m_sV);
	if (m_unFlags & (1 << 10))
	{
		oRes.AddInt(m_arrOpt.size());
		for (int i = 0; i < m_arrOpt.size(); ++i)
		{
			oRes.WriteString(m_arrOpt[i].first);
			oRes.WriteString(m_arrOpt[i].second);
		}
	}
	if (m_unFlags & (1 << 11))
		oRes.AddInt(m_unTI);
	if (m_unFlags & (1 << 12))
	{
		oRes.AddInt(m_arrI.size());
		for (int i = 0; i < m_arrI.size(); ++i)
			oRes.AddInt(m_arrI[i]);
	}
	if (m_unFlags & (1 << 13))
	{
		oRes.AddInt(m_arrV.size());
		for (int i = 0; i < m_arrV.size(); ++i)
			oRes.WriteString(m_arrV[i]);
	}
}
void CAnnotWidgetSig::ToWASM(NSWasm::CData& oRes)
{
	CAnnotWidget::ToWASM(oRes);
}
void CAnnotMarkup::ToWASM(NSWasm::CData& oRes)
{
	CAnnot::ToWASM(oRes);

	oRes.AddInt(m_unFlags);
	if (m_unFlags & (1 << 0))
		oRes.AddInt(m_unRefNumPopup);
	if (m_unFlags & (1 << 1))
		oRes.WriteString(m_sT);
	if (m_unFlags & (1 << 2))
		oRes.AddDouble(m_dCA);
	if (m_unFlags & (1 << 3))
	{
		oRes.AddInt(m_arrRC.size());
		for (int i = 0; i < m_arrRC.size(); ++i)
		{
			oRes.WriteBYTE(m_arrRC[i]->nAlign);
			oRes.AddInt(m_arrRC[i]->unFontFlags);
			if (m_arrRC[i]->unFontFlags & (1 << 5))
				oRes.AddDouble(m_arrRC[i]->dVAlign);
			if (m_arrRC[i]->unFontFlags & (1 << 6))
				oRes.WriteString(m_arrRC[i]->sActualFont);
			oRes.AddDouble(m_arrRC[i]->dFontSise);
			oRes.WriteDouble(m_arrRC[i]->dColor[0]);
			oRes.WriteDouble(m_arrRC[i]->dColor[1]);
			oRes.WriteDouble(m_arrRC[i]->dColor[2]);
			oRes.WriteString(m_arrRC[i]->sFontFamily);
			oRes.WriteString(m_arrRC[i]->sText);
		}
	}
	if (m_unFlags & (1 << 4))
		oRes.WriteString(m_sCreationDate);
	if (m_unFlags & (1 << 5))
		oRes.AddInt(m_unRefNumIRT);
	if (m_unFlags & (1 << 6))
		oRes.WriteBYTE(m_nRT);
	if (m_unFlags & (1 << 7))
		oRes.WriteString(m_sSubj);
}
void CAnnotText::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(0); // Text

	CAnnotMarkup::ToWASM(oRes);

	if (m_unFlags & (1 << 16))
		oRes.WriteBYTE(m_nName);
	if (m_unFlags & (1 << 17))
		oRes.WriteBYTE(m_nStateModel);
	if (m_unFlags & (1 << 18))
		oRes.WriteBYTE(m_nState);
}
void CAnnotPopup::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(15); // Popup

	CAnnot::ToWASM(oRes);

	oRes.AddInt(m_unFlags);
	if (m_unFlags & (1 << 1))
		oRes.AddInt(m_unRefNumParent);
}
void CAnnotInk::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(14); // Ink

	CAnnotMarkup::ToWASM(oRes);

	oRes.AddInt(m_arrInkList.size());
	for (int i = 0; i < m_arrInkList.size(); ++i)
	{
		oRes.AddInt(m_arrInkList[i].size());
		for (int j = 0; j < m_arrInkList[i].size(); ++j)
			oRes.AddDouble(m_arrInkList[i][j]);
	}
}
void CAnnotLine::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(3); // Line

	CAnnotMarkup::ToWASM(oRes);

	for (int i = 0; i < 4; ++i)
		oRes.AddDouble(m_pL[i]);

	if (m_unFlags & (1 << 15))
	{
		oRes.WriteBYTE(m_nLE[0]);
		oRes.WriteBYTE(m_nLE[1]);
	}
	if (m_unFlags & (1 << 16))
	{
		oRes.AddInt((unsigned int)m_arrIC.size());
		for (int i = 0; i < m_arrIC.size(); ++i)
			oRes.WriteDouble(m_arrIC[i]);
	}
	if (m_unFlags & (1 << 17))
		oRes.AddDouble(m_dLL);
	if (m_unFlags & (1 << 18))
		oRes.AddDouble(m_dLLE);
	if (m_unFlags & (1 << 20))
		oRes.WriteBYTE(m_nIT);
	if (m_unFlags & (1 << 21))
		oRes.AddDouble(m_dLLO);
	if (m_unFlags & (1 << 22))
		oRes.WriteBYTE(m_nCP);
	if (m_unFlags & (1 << 23))
	{
		oRes.AddDouble(m_pCO[0]);
		oRes.AddDouble(m_pCO[1]);
	}
}
void CAnnotTextMarkup::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(m_nSubtype); // Highlight, Underline, Squiggly, StrikeOut

	CAnnotMarkup::ToWASM(oRes);

	oRes.AddInt((unsigned int)m_arrQuadPoints.size());
	for (int i = 0; i < m_arrQuadPoints.size(); ++i)
		oRes.AddDouble(m_arrQuadPoints[i]);
}
void CAnnotSquareCircle::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(m_nSubtype); // Square, Circle

	CAnnotMarkup::ToWASM(oRes);

	if (m_unFlags & (1 << 15))
	{
		for (int i = 0; i < 4; ++i)
			oRes.AddDouble(m_pRD[i]);
	}
	if (m_unFlags & (1 << 16))
	{
		oRes.AddInt((unsigned int)m_arrIC.size());
		for (int i = 0; i < m_arrIC.size(); ++i)
			oRes.WriteDouble(m_arrIC[i]);
	}
}
void CAnnotPolygonLine::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(m_nSubtype); // Polygon, PolyLine

	CAnnotMarkup::ToWASM(oRes);

	oRes.AddInt((unsigned int)m_arrVertices.size());
	for (int i = 0; i < m_arrVertices.size(); ++i)
		oRes.AddDouble(m_arrVertices[i]);

	if (m_unFlags & (1 << 15))
	{
		oRes.WriteBYTE(m_nLE[0]);
		oRes.WriteBYTE(m_nLE[1]);
	}
	if (m_unFlags & (1 << 16))
	{
		oRes.AddInt((unsigned int)m_arrIC.size());
		for (int i = 0; i < m_arrIC.size(); ++i)
			oRes.WriteDouble(m_arrIC[i]);
	}
	if (m_unFlags & (1 << 20))
		oRes.WriteBYTE(m_nIT);
}
void CAnnotFreeText::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(2); // FreeText

	CAnnotMarkup::ToWASM(oRes);

	oRes.WriteBYTE(m_nQ);
	oRes.AddInt(m_nRotate);
	if (m_unFlags & (1 << 15))
	{
		for (int i = 0; i < 4; ++i)
			oRes.AddDouble(m_pRD[i]);
	}
	if (m_unFlags & (1 << 16))
	{
		oRes.AddInt((unsigned int)m_arrCL.size());
		for (int i = 0; i < m_arrCL.size(); ++i)
			oRes.AddDouble(m_arrCL[i]);
	}
	if (m_unFlags & (1 << 17))
		oRes.WriteString(m_sDS);
	if (m_unFlags & (1 << 18))
		oRes.WriteBYTE(m_nLE);
	if (m_unFlags & (1 << 20))
		oRes.WriteBYTE(m_nIT);
	if (m_unFlags & (1 << 21))
	{
		oRes.AddInt((unsigned int)m_arrCFromDA.size());
		for (int i = 0; i < m_arrCFromDA.size(); ++i)
			oRes.WriteDouble(m_arrCFromDA[i]);
	}
}
void CAnnotCaret::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(13); // Caret

	CAnnotMarkup::ToWASM(oRes);

	if (m_unFlags & (1 << 15))
	{
		for (int i = 0; i < 4; ++i)
			oRes.AddDouble(m_pRD[i]);
	}
	if (m_unFlags & (1 << 16))
		oRes.WriteBYTE(m_nSy);
}
void CAnnotFileAttachment::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(16); // FileAttachment

	CAnnotMarkup::ToWASM(oRes);

	if (m_unFlags & (1 << 15))
		oRes.WriteString(m_sName);
	if (m_unFlags & (1 << 16))
		oRes.WriteString(m_sFS);
	if (m_unFlags & (1 << 17))
		oRes.WriteString(m_sF);
	if (m_unFlags & (1 << 18))
		oRes.WriteString(m_sUF);
	if (m_unFlags & (1 << 19))
		oRes.WriteString(m_sDOS);
	if (m_unFlags & (1 << 20))
		oRes.WriteString(m_sMac);
	if (m_unFlags & (1 << 21))
		oRes.WriteString(m_sUnix);
	if (m_unFlags & (1 << 22))
	{
		oRes.WriteString(m_sID.first);
		oRes.WriteString(m_sID.second);
	}
	if (m_unFlags & (1 << 24))
	{
		int nFlag = 0;
		int nPos = oRes.GetSize();
		oRes.AddInt(nFlag);

		auto fWriteEF = [&oRes](CAnnotFileAttachment::CEmbeddedFile* pEF)
		{
			oRes.AddInt(pEF->nLength);
			unsigned long long npSubMatrix = (unsigned long long)pEF->pFile;
			unsigned int npSubMatrix1 = npSubMatrix & 0xFFFFFFFF;
			oRes.AddInt(npSubMatrix1);
			oRes.AddInt(npSubMatrix >> 32);
			pEF->bFree = false;
		};

		// Освобождение памяти необходимо вызвать с js стороны
		if (m_pEF->m_pF)
		{
			nFlag |= (1 << 0);
			fWriteEF(m_pEF->m_pF);
		}
		if (m_pEF->m_pUF)
		{
			nFlag |= (1 << 1);
			fWriteEF(m_pEF->m_pUF);
		}
		if (m_pEF->m_pDOS)
		{
			nFlag |= (1 << 2);
			fWriteEF(m_pEF->m_pDOS);
		}
		if (m_pEF->m_pMac)
		{
			nFlag |= (1 << 3);
			fWriteEF(m_pEF->m_pMac);
		}
		if (m_pEF->m_pUnix)
		{
			nFlag |= (1 << 4);
			fWriteEF(m_pEF->m_pUnix);
		}
		oRes.AddInt(nFlag, nPos);
	}
	if (m_unFlags & (1 << 25))
	{
	}
	if (m_unFlags & (1 << 26))
		oRes.WriteString(m_sDesc);
	if (m_unFlags & (1 << 27))
	{
	}
}
void CAnnotStamp::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(12); // Stamp

	CAnnotMarkup::ToWASM(oRes);

	oRes.WriteString(m_sName);
	oRes.WriteDouble(m_dRotate);
	oRes.WriteDouble(m_dX1);
	oRes.WriteDouble(m_dY1);
	oRes.WriteDouble(m_dX2);
	oRes.WriteDouble(m_dY2);
	oRes.WriteDouble(m_dX3);
	oRes.WriteDouble(m_dY3);
	oRes.WriteDouble(m_dX4);
	oRes.WriteDouble(m_dY4);
}
}
