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

namespace PdfReader
{

#define DICT_LOOKUP_STRING(func, sName, byte, put) \
{\
if (func(sName, &oObj)->isString())\
{\
	m_unFlags |= (1 << byte);\
	TextString* s = new TextString(oObj.getString());\
	put = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());\
	delete s;\
}\
oObj.free();\
}
#define ARR_GET_NUM(take, get, put) \
{\
	if (take.arrayGet(get, &oObj2)->isNum())\
		put = oObj2.getNum();\
	oObj2.free();\
}
#define WRITE_EF(EF) \
{ \
	oRes.AddInt(EF->nLength); \
	unsigned long long npSubMatrix = (unsigned long long)EF->pFile; \
	unsigned int npSubMatrix1 = npSubMatrix & 0xFFFFFFFF; \
	oRes.AddInt(npSubMatrix1); \
	oRes.AddInt(npSubMatrix >> 32); \
	EF->bFree = false; \
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
			ppRes->unPage = pdfDoc->findPage(pageRef.num, pageRef.gen) - 1;
		}
		else
			ppRes->unPage = pLinkDest->getPageNum() - 1;
		ppRes->nKind = pLinkDest->getKind();
		double dHeight = pdfDoc->getPageCropHeight(ppRes->unPage + 1);
		switch (ppRes->nKind)
		{
		case destXYZ:
		case destFitH:
		case destFitBH:
		case destFitV:
		case destFitBV:
		{
			ppRes->unKindFlag = 0;
			// 1 - left
			if (pLinkDest->getChangeLeft())
			{
				ppRes->unKindFlag |= (1 << 0);
				ppRes->pRect[0] = pLinkDest->getLeft();
			}
			// 2 - top
			if (pLinkDest->getChangeTop())
			{
				ppRes->unKindFlag |= (1 << 1);
				double dTop = dHeight - pLinkDest->getTop();
				ppRes->pRect[1] = (dTop < 0 ? 0.0 : dTop);
			}
			// 3 - zoom
			if (pLinkDest->getChangeZoom() && pLinkDest->getZoom())
			{
				ppRes->unKindFlag |= (1 << 2);
				ppRes->pRect[2] = pLinkDest->getZoom();
			}
			break;
		}
		case destFitR:
		{

			ppRes->pRect[0] = pLinkDest->getLeft();
			double dTop = dHeight - pLinkDest->getTop();
			ppRes->pRect[1] = (dTop < 0 ? 0.0 : dTop);
			ppRes->pRect[2] = pLinkDest->getRight();
			dTop = dHeight - pLinkDest->getBottom();
			ppRes->pRect[3] = (dTop < 0 ? 0.0 : dTop);
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

	// Значение поля - V
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
		// 11 - Заголовок - СА
		// 12 - Заголовок прокрутки - RC
		// 13 - Альтернативный заголовок - AC
		if (oType == acroFormFieldPushbutton)
		{
			DICT_LOOKUP_STRING(oMK.dictLookup, "CA", 10, m_sCA);
			DICT_LOOKUP_STRING(oMK.dictLookup, "RC", 11, m_sRC);
			DICT_LOOKUP_STRING(oMK.dictLookup, "AC", 12, m_sAC);
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

		// 14 - Положение заголовка - TP
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
			// 2 - Масштабирование - SW
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
			// 3 - Тип масштабирования - S
			if (oIF.dictLookup("S", &oObj)->isName())
			{
				m_unIFFlag |= (1 << 2);
				std::string sName(oObj.getName());
				m_nS = 0; // Default: P
				if (sName == "A")
					m_nS = 1;
			}
			oObj.free();
			// 4 - Смещение - A
			if (oIF.dictLookup("A", &oObj)->isArray())
			{
				Object oObj2;
				m_unIFFlag |= (1 << 3);
				m_dA1 = 0.5; m_dA2 = 0.5;
				ARR_GET_NUM(oObj, 0, m_dA1);
				ARR_GET_NUM(oObj, 1, m_dA2);
			}
			oObj.free();
			// 5 - Полное соответствие - FB
			if (oIF.dictLookup("FB", &oObj)->isBool() && oObj.getBool())
				m_unIFFlag |= (1 << 4);
			oObj.free();
		}
		oIF.free();
	}
	oMK.free();

	Object oOpt;
	pField->fieldLookup("Opt", &oOpt);

	// 15 - Имя вкл состояния - AP - N - Yes
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

	// 10 - Значение
	// 11 - Максимальное количество символов
	// 12 - Расширенный текст RV
	if (oField.dictLookup("V", &oObj))
	{
		m_sV = getValue(&oObj);
		if (!m_sV.empty())
			m_unFlags |= (1 << 9);
	}
	oObj.free();
	oField.free();

	// Максимальное количество символов в Tx - MaxLen
	int nMaxLen = pField->getMaxLen();
	if (nMaxLen > 0)
	{
		m_unFlags |= (1 << 10);
		m_unMaxLen = nMaxLen;
	}

	// RichText
	if (pField->getFlags() & (1 << 25))
	{
		DICT_LOOKUP_STRING(pField->fieldLookup, "RV", 11, m_sRV);
	}
}

CAnnotWidgetCh::CAnnotWidgetCh(PDFDoc* pdfDoc, AcroFormField* pField) : CAnnotWidget(pdfDoc, pField)
{
	Object oObj;
	Object oFieldRef, oField;
	pField->getFieldRef(&oFieldRef);
	oFieldRef.fetch(pdfDoc->getXRef(), &oField);
	oFieldRef.free();

	// 10 - Значение
	if (oField.dictLookup("V", &oObj))
	{
		m_sV = getValue(&oObj, false);
		if (!m_sV.empty())
			m_unFlags |= (1 << 9);
	}
	oObj.free();

	Object oOpt;
	// 11 - Список значений
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

	// 12 - Индекс верхнего элемента - TI
	if (pField->fieldLookup("TI", &oObj)->isInt())
	{
		m_unFlags |= (1 << 11);
		m_unTI = oObj.getInt();
	}
	oObj.free();

	// 13 - Выбранные индексы - I
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

	// 14 - Массив значений
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

	// 10 - Значение
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

	// Исходное значение флага данных
	m_unFlags = 0;

	// 1 - Альтернативное имя поля, используется во всплывающей подсказке и сообщениях об ошибке - TU
	DICT_LOOKUP_STRING(pField->fieldLookup, "TU", 0, m_sTU);

	// 2 - Строка стиля по умолчанию - DS
	DICT_LOOKUP_STRING(pField->fieldLookup, "DS", 1, m_sDS);

	// 4 - Режим выделения - H
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
		// 6 - Цвет границ - BC. Даже если граница не задана BS/Border, то при наличии BC предоставляется граница по-умолчанию (сплошная, толщиной 1)
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

		// 7 - Поворот аннотации относительно страницы - R
		if (oMK.dictLookup("R", &oObj)->isInt())
		{
			m_unFlags |= (1 << 6);
			m_unR = oObj.getInt();
		}
		oObj.free();

		// 8 - Цвет фона - BG
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

	// 9 - Значение по-умолчанию
	if (oField.dictLookup("DV", &oObj))
	{
		m_sDV = getValue(&oObj);
		if (!m_sDV.empty())
			m_unFlags |= (1 << 8);
	}
	oObj.free();

	// 18 - Родитель - Parent
	if (oField.dictLookupNF("Parent", &oObj)->isRef())
	{
		m_unRefNumParent = oObj.getRefNum();
		m_unFlags |= (1 << 17);
	}
	oObj.free();

	// 19 - Частичное имя поля - T
	DICT_LOOKUP_STRING(oField.dictLookup, "T", 18, m_sT);

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

bool GetFontFromAP(PDFDoc* pdfDoc, AcroFormField* pField, Object* oR, Object* oFonts, Object* oFontRef, std::string& sFontKey)
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
					Object oObj3;
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
			Dict* pNDict = oN.streamGetDict();
			bFindResources = pNDict->lookup("Resources", oR)->isDict() && oR->dictLookup("Font", oFonts)->isDict() && oFonts->dictLookupNF(oObj1.getName(), oFontRef)->isRef();
			sFontKey = oObj1.getName();
		}

		oObj1.free(); oObj2.free(); oObj3.free();
		RELEASEOBJECT(parser);
	}
	oAP.free(); oN.free();

	return bFindResources;
}
std::wstring GetFontData(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CFontList *pFontList, Object* oFonts, Object* oFontRef, int nTypeFonts, std::string& sFontName, std::string& sActualFontName, bool& bBold, bool& bItalic)
{
	XRef* xref = pdfDoc->getXRef();

	GfxFont* gfxFont = NULL;
	GfxFontDict *gfxFontDict = NULL;
	gfxFontDict = new GfxFontDict(xref, NULL, oFonts->getDict());
	gfxFont = gfxFontDict->lookupByRef(oFontRef->getRef());

	bBold = false, bItalic = false;
	if (!gfxFont)
	{
		RELEASEOBJECT(gfxFontDict);
		return L"";
	}

	Ref oEmbRef;
	const unsigned char* pData14 = NULL;
	unsigned int nSize14 = 0;
	std::wstring wsFontBaseName = NSStrings::GetStringFromUTF32(gfxFont->getName());
	std::wstring wsFileName;

	if (((nTypeFonts & 1) && gfxFont->getEmbeddedFontID(&oEmbRef)) || ((nTypeFonts & 2) && GetBaseFont(wsFontBaseName, pData14, nSize14)))
	{
		std::wstring wsFontName;
		GetFont(xref, pFontManager, pFontList, gfxFont, wsFileName, wsFontName);

		sFontName = U_TO_UTF8(wsFontName);
		CheckFontStylePDF(wsFontName, bBold, bItalic);
		if (!bBold)
			bBold = gfxFont->isBold();
		if (!bItalic)
			bItalic = gfxFont->isItalic();
	}
	else if (nTypeFonts & 4)
	{
		std::wstring wsFBN = wsFontBaseName;
		NSFonts::CFontInfo* pFontInfo = GetFontByParams(xref, pFontManager, gfxFont, wsFBN);
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
			if (pFontInfo->m_bBold)
				bBold = true;
			if (pFontInfo->m_bItalic)
				bItalic = true;
		}
	}

	RELEASEOBJECT(gfxFontDict);
	return wsFileName;
}

void CAnnotWidget::SetFont(PDFDoc* pdfDoc, AcroFormField* pField, NSFonts::IFontManager* pFontManager, CFontList *pFontList)
{
	// Шрифт и размер шрифта - из DA
	Ref fontID;
	pField->getFont(&fontID, &m_dFontSize);
	bool bFullFont = true;
	if (fontID.num < 0)
		bFullFont = false;

	Object oR, oFonts, oFontRef;
	bool bFindResources = false;

	if (bFullFont && pField->fieldLookup("DR", &oR)->isDict() && oR.dictLookup("Font", &oFonts)->isDict())
	{
		for (int i = 0; i < oFonts.dictGetLength(); ++i)
		{
			if (oFonts.dictGetValNF(i, &oFontRef)->isRef() && oFontRef.getRef() == fontID)
			{
				m_sFontKey = oFonts.dictGetKey(i);
				bFindResources = true;
				break;
			}
			oFontRef.free();
		}
	}

	if (bFullFont && !bFindResources)
	{
		oR.free(); oFonts.free();
		AcroForm* pAcroForms = pdfDoc->getCatalog()->getForm();
		Object* oAcroForm = pAcroForms->getAcroFormObj();
		if (oAcroForm->isDict() && oAcroForm->dictLookup("DR", &oR)->isDict() && oR.dictLookup("Font", &oFonts)->isDict())
		{
			for (int i = 0; i < oFonts.dictGetLength(); ++i)
			{
				if (oFonts.dictGetValNF(i, &oFontRef)->isRef() && oFontRef.getRef() == fontID)
				{
					m_sFontKey = oFonts.dictGetKey(i);
					bFindResources = true;
					break;
				}
				oFontRef.free();
			}
		}
	}

	if (!bFullFont)
		bFindResources = GetFontFromAP(pdfDoc, pField, &oR, &oFonts, &oFontRef, m_sFontKey);

	bool bBold = false, bItalic = false;
	if (bFindResources)
		GetFontData(pdfDoc, pFontManager, pFontList, &oFonts, &oFontRef, bFullFont ? 7 : 6, m_sFontName, m_sActualFontName, bBold, bItalic);
	oR.free(); oFonts.free(); oFontRef.free();

	// 3 - Актуальный шрифт
	if (!m_sActualFontName.empty())
		m_unFlags |= (1 << 2);

	// 5 - Уникальный идентификатор шрифта
	if (!m_sFontKey.empty())
		m_unFlags |= (1 << 4);

	m_unFontStyle = 0;
	if (bBold)
		m_unFontStyle |= (1 << 0);
	if (bItalic)
		m_unFontStyle |= (1 << 1);
}
void CAnnotWidget::SetButtonFont(PDFDoc* pdfDoc, AcroFormField* pField, NSFonts::IFontManager* pFontManager, CFontList *pFontList)
{
	// Неполноценный шрифт во внешнем виде pushbutton
	Object oR, oFonts, oFontRef;

	if (!GetFontFromAP(pdfDoc, pField, &oR, &oFonts, &oFontRef, m_sFontKey))
	{
		oR.free(); oFonts.free(); oFontRef.free();
		return;
	}

	bool bBold = false, bItalic = false;
	GetFontData(pdfDoc, pFontManager, pFontList, &oFonts, &oFontRef, 3, m_sButtonFontName, m_sButtonFontName, bBold, bItalic);
	if (!m_sButtonFontName.empty())
		m_unFlags |= (1 << 19);

	oR.free(); oFonts.free(); oFontRef.free();
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

	// 1 - Отображаться открытой - Open
	if (oAnnot.dictLookup("Open", &oObj)->isBool() && oObj.getBool())
		m_unFlags |= (1 << 0);
	oObj.free();

	// 2 - Родитель - Parent
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

CAnnotText::CAnnotText(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CMarkupAnnot(pdfDoc, oAnnotRef, nPageIndex)
{
	Object oAnnot, oObj;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// 16 - Отображаться открытой - Open
	if (oAnnot.dictLookup("Open", &oObj)->isBool() && oObj.getBool())
		m_unFlags |= (1 << 15);
	oObj.free();

	// 17 - Иконка - Name
	if (oAnnot.dictLookup("Name", &oObj)->isName())
	{
		m_unFlags |= (1 << 16);
		std::string sName(oObj.getName());
		std::vector<std::string> arrName = {"Check", "Checkmark", "Circle", "Comment", "Cross", "CrossHairs", "Help", "Insert", "Key", "NewParagraph", "Note", "Paragraph", "RightArrow", "RightPointer", "Star", "UpArrow", "UpLeftArrow"};
		m_nName = 10; // Default: Note
		std::vector<std::string>::iterator p = std::find(arrName.begin(), arrName.end(), sName);
		if (p != arrName.end())
			m_nName = p - arrName.begin();
	}
	oObj.free();

	// 18 - Модель состояния - StateModel
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

	// 19 - Состояние - State
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

CAnnotInk::CAnnotInk(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CMarkupAnnot(pdfDoc, oAnnotRef, nPageIndex)
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
						arrLine.push_back(k % 2 == 0 ? oObj1.getNum() : m_dHeight - oObj1.getNum());
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

CAnnotLine::CAnnotLine(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CMarkupAnnot(pdfDoc, oAnnotRef, nPageIndex)
{
	Object oAnnot, oObj, oObj2;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// Координаты линии - L
	if (oAnnot.dictLookup("L", &oObj)->isArray())
	{
		ARR_GET_NUM(oObj, 0, m_pL[0]);
		ARR_GET_NUM(oObj, 1, m_pL[1]);
		ARR_GET_NUM(oObj, 2, m_pL[2]);
		ARR_GET_NUM(oObj, 3, m_pL[3]);
		m_pL[1] = m_dHeight - m_pL[1];
		m_pL[3] = m_dHeight - m_pL[3];
	}
	oObj.free();

	// 16 - Стили окончания линии - LE
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

	// 17 - Цвет окончаний линии - IC
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

	// 18 - Длина линий выноски - LL
	if (oAnnot.dictLookup("LL", &oObj)->isNum())
	{
		m_unFlags |= (1 << 17);
		m_dLL = oObj.getNum();
	}
	oObj.free();

	// 19 - Продолжение линий выноски - LLE
	if (oAnnot.dictLookup("LLE", &oObj)->isNum())
	{
		m_unFlags |= (1 << 18);
		m_dLLE = oObj.getNum();
	}
	oObj.free();

	// 20 - Местоположение заголовка - Cap
	if (oAnnot.dictLookup("Cap", &oObj)->isBool())
		m_unFlags |= (1 << 19);
	oObj.free();

	// 21 - Назначение аннотации - IT
	if (oAnnot.dictLookup("IT", &oObj)->isName())
	{
		m_unFlags |= (1 << 20);
		m_nIT = 0; // LineDimension
		if (oObj.isName("LineArrow"))
			m_nIT = 1;
	}
	oObj.free();

	// 22 - Длина смещения выноски - LLO
	if (oAnnot.dictLookup("LLO", &oObj)->isNum())
	{
		m_unFlags |= (1 << 21);
		m_dLLO = oObj.getNum();
	}
	oObj.free();

	// 23 - Расположение заголовка аннотации - CP
	if (oAnnot.dictLookup("CP", &oObj)->isName())
	{
		m_unFlags |= (1 << 22);
		m_nCP = 0; // Inline
		if (oObj.isName("Top"))
			m_nCP = 1;
	}
	oObj.free();

	// 24 - Смещение текста подписи - CO
	m_pCO[0] = 0.0; m_pCO[1] = 0.0;
	if (oAnnot.dictLookup("CO", &oObj)->isArray())
	{
		m_unFlags |= (1 << 23);
		ARR_GET_NUM(oObj, 0, m_pCO[0]);
		ARR_GET_NUM(oObj, 1, m_pCO[1]);
	}
	oObj.free();

	oAnnot.free();
}

//------------------------------------------------------------------------
// TextMarkup: Highlight, Underline, Squiggly, StrikeOut
//------------------------------------------------------------------------

CAnnotTextMarkup::CAnnotTextMarkup(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CMarkupAnnot(pdfDoc, oAnnotRef, nPageIndex)
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
				m_arrQuadPoints.push_back(i % 2 == 1 ? m_dHeight - oObj2.getNum() : oObj2.getNum());
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

CAnnotSquareCircle::CAnnotSquareCircle(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CMarkupAnnot(pdfDoc, oAnnotRef, nPageIndex)
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

	// 16 - Различия Rect и фактического размера - RD
	if (oAnnot.dictLookup("RD", &oObj)->isArray())
	{
		m_unFlags |= (1 << 15);
		ARR_GET_NUM(oObj, 0, m_pRD[0]);
		ARR_GET_NUM(oObj, 1, m_pRD[3]);
		ARR_GET_NUM(oObj, 2, m_pRD[2]);
		ARR_GET_NUM(oObj, 3, m_pRD[1]);
	}
	oObj.free();

	// 17 - Цвет заполнения - IC
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

CAnnotPolygonLine::CAnnotPolygonLine(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CMarkupAnnot(pdfDoc, oAnnotRef, nPageIndex)
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
				m_arrVertices.push_back(j % 2 == 0 ? oObj2.getNum() : m_dHeight - oObj2.getNum());
			else
				m_arrVertices.push_back(0.0);
			oObj2.free();
		}
	}
	oObj.free();

	// 16 - Стили окончания линии - LE
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

	// 17 - Цвет заполнения - IC
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

	// 21 - Назначение аннотации - IT
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

CAnnotFreeText::CAnnotFreeText(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CMarkupAnnot(pdfDoc, oAnnotRef, nPageIndex)
{
	Object oAnnot, oObj, oObj2;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// Выравнивание текста - Q
	m_nQ = 0;
	if (oAnnot.dictLookup("Q", &oObj)->isInt())
		m_nQ = oObj.getInt();
	oObj.free();

	// 16 - Различия Rect и фактического размера - RD
	if (oAnnot.dictLookup("RD", &oObj)->isArray())
	{
		m_unFlags |= (1 << 15);
		ARR_GET_NUM(oObj, 0, m_pRD[0]);
		ARR_GET_NUM(oObj, 1, m_pRD[3]);
		ARR_GET_NUM(oObj, 2, m_pRD[2]);
		ARR_GET_NUM(oObj, 3, m_pRD[1]);
	}
	oObj.free();

	// 17 - Координаты выноски - CL
	if (oAnnot.dictLookup("CL", &oObj)->isArray())
	{
		m_unFlags |= (1 << 16);
		for (int j = 0; j < oObj.arrayGetLength(); ++j)
		{
			if (oObj.arrayGet(j, &oObj2)->isNum())
				m_arrCL.push_back(j % 2 == 0 ? oObj2.getNum() : m_dHeight - oObj2.getNum());
			else
				m_arrCL.push_back(0.0);
			oObj2.free();
		}
	}
	oObj.free();

	// 18 - Строка стиля по умолчанию - DS
	DICT_LOOKUP_STRING(oAnnot.dictLookup, "DS", 17, m_sDS);

	// 19 - Стили окончания линии - LE
	if (oAnnot.dictLookup("LE", &oObj)->isName())
	{
		m_unFlags |= (1 << 18);
		m_nLE = getLE(&oObj);
	}
	oObj.free();

	// 21 - Назначение аннотации - IT
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

	// 22 - Назначение аннотации - IT
	if (oAnnot.dictLookup("DA", &oObj)->isString())
	{
		m_unFlags |= (1 << 21);
		int nSpace;
		GList *arrColors = new GList();

		// parse the default appearance string
		GList* daToks = tokenize(oObj.getString());
		for (int i = 1; i < daToks->getLength(); ++i) {

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

	oAnnot.free();
}

//------------------------------------------------------------------------
// Caret
//------------------------------------------------------------------------

CAnnotCaret::CAnnotCaret(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CMarkupAnnot(pdfDoc, oAnnotRef, nPageIndex)
{
	Object oAnnot, oObj, oObj2;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// 16 - Различия Rect и фактического размера - RD
	if (oAnnot.dictLookup("RD", &oObj)->isArray() && oObj.arrayGetLength() == 4)
	{
		m_unFlags |= (1 << 15);
		ARR_GET_NUM(oObj, 0, m_pRD[0]);
		ARR_GET_NUM(oObj, 1, m_pRD[3]);
		ARR_GET_NUM(oObj, 2, m_pRD[2]);
		ARR_GET_NUM(oObj, 3, m_pRD[1]);
	}
	oObj.free();

	// 17 - Связанный символ - Sy
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

CAnnotFileAttachment::CAnnotFileAttachment(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CMarkupAnnot(pdfDoc, oAnnotRef, nPageIndex)
{
	m_pEF = NULL;

	Object oAnnot, oObj, oObj2;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// 16 - Иконка - Name
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

	// 17 - Файловая система - FS
	if (oFS.dictLookup("FS", &oObj)->isName())
	{
		m_unFlags |= (1 << 16);
		m_sFS = oObj.getName();
	}
	oObj.free();

	// 18 - Спецификация файла - F
	DICT_LOOKUP_STRING(oFS.dictLookup, "F", 17, m_sF);

	// 19 - Спецификация файла - UF
	DICT_LOOKUP_STRING(oFS.dictLookup, "UF", 18, m_sUF);

	// 20 - Спецификация файла - DOS
	DICT_LOOKUP_STRING(oFS.dictLookup, "DOS", 19, m_sDOS);

	// 21 - Спецификация файла - Mac
	DICT_LOOKUP_STRING(oFS.dictLookup, "Mac", 20, m_sMac);

	// 22 - Спецификация файла - Unix
	DICT_LOOKUP_STRING(oFS.dictLookup, "Unix", 21, m_sUnix);

	// 23 - Идентификатор файла - ID
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

	// 24 - Изменчивость файла - V
	if (oFS.dictLookup("V", &oObj)->isBool() && oObj.getBool())
		m_unFlags |= (1 << 23);
	oObj.free();

	// 25 - Встроенные файловые потоки - EF
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

	// 26 - Встроенные файловые потоки - RF
	Object oRF;
	if (oFS.dictLookup("RF", &oRF)->isDict())
	{
		m_unFlags |= (1 << 25);
	}
	oRF.free();

	// 27 - Описание файла - Desc
	DICT_LOOKUP_STRING(oFS.dictLookup, "Desc", 26, m_sDesc);

	// 28 - Коллекция - Cl
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
// Annots
//------------------------------------------------------------------------

CAnnots::CAnnots(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CFontList *pFontList)
{
	Object oObj1, oObj2;
	XRef* xref = pdfDoc->getXRef();
	AcroForm* pAcroForms = pdfDoc->getCatalog()->getForm();

	// Порядок вычислений - CO
	Object* oAcroForm = pAcroForms->getAcroFormObj();
	if (oAcroForm && oAcroForm->dictLookup("CO", &oObj1)->isArray())
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
}

//------------------------------------------------------------------------
// Markup
//------------------------------------------------------------------------

CMarkupAnnot::CMarkupAnnot(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex) : CAnnot(pdfDoc, oAnnotRef, nPageIndex)
{
	m_unFlags = 0;

	Object oAnnot, oObj;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	// 1 - Всплывающая аннотация - Popup
	if (oAnnot.dictLookupNF("Popup", &oObj)->isRef())
	{
		m_unFlags |= (1 << 0);
		m_unRefNumPopup = oObj.getRefNum();
	}

	// 2 - Текстовая метка пользователя - T
	DICT_LOOKUP_STRING(oAnnot.dictLookup, "T", 1, m_sT);

	// 3 - Значение непрозрачности - CA
	if (oAnnot.dictLookup("CA", &oObj)->isNum())
	{
		m_unFlags |= (1 << 2);
		m_dCA = oObj.getNum();
	}
	oObj.free();

	// 4 - Форматированный текст - RC
	DICT_LOOKUP_STRING(oAnnot.dictLookup, "RC", 3, m_sRC);
	// if (oAnnot.dictLookup("RC", &oObj)->isStream())
	// TODO streamGetBlock

	// 5 - Дата создания - CreationDate
	DICT_LOOKUP_STRING(oAnnot.dictLookup, "CreationDate", 4, m_sCreationDate);

	// 6 - Ссылка на аннотацию ответ - IRT
	if (oAnnot.dictLookupNF("IRT", &oObj)->isRef())
	{
		m_unFlags |= (1 << 5);
		m_unRefNumIRT = oObj.getRefNum();
	}
	oObj.free();

	// 7 - Тип аннотации ответа - RT
	if (oAnnot.dictLookup("RT", &oObj)->isName())
	{
		m_unFlags |= (1 << 6);
		m_nRT = 1; // Group
		if (oObj.isName("R"))
			m_nRT = 0;
	}
	oObj.free();

	// 8 - Краткое описание - Subj
	DICT_LOOKUP_STRING(oAnnot.dictLookup, "Subj", 7, m_sSubj);

	oAnnot.free();
}

//------------------------------------------------------------------------
// Annot
//------------------------------------------------------------------------

CAnnot::CAnnot(PDFDoc* pdfDoc, AcroFormField* pField)
{
	m_pBorder = NULL;
	m_unAnnotFlag = 0;
	m_unAFlags = 0;

	Object oObj;
	pField->getFieldRef(&oObj);
	m_unRefNum = oObj.getRefNum();
	oObj.free();

	// Флаг аннотации - F
	if (pField->fieldLookup("F", &oObj)->isInt())
		m_unAnnotFlag = oObj.getInt();
	oObj.free();

	// Номер страницы - P
	m_unPage = pField->getPageNum() - 1;

	// Координаты - Rect
	pField->getBBox(&m_pRect[0], &m_pRect[1], &m_pRect[2], &m_pRect[3]);
	m_dHeight = pdfDoc->getPageCropHeight(m_unPage + 1);
	double dTemp = m_pRect[1];
	m_pRect[1] = m_dHeight - m_pRect[3];
	m_pRect[3] = m_dHeight - dTemp;

	// 1 - Уникальное имя - NM
	if (pField->fieldLookup("NM", &oObj)->isString())
	{
		m_unAFlags |= (1 << 0);
		TextString* s = new TextString(oObj.getString());
		m_sNM = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();

	// 2 - Альтернативный текст - Contents
	if (pField->fieldLookup("Contents", &oObj)->isString())
	{
		m_unAFlags |= (1 << 1);
		TextString* s = new TextString(oObj.getString());
		m_sContents = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();

	// 3 - Эффекты границы - BE
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

	// 4 - Специальный цвет для аннотации - C
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

	// 5 - Границы и Dash Pattern - Border/BS
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

	// 6 - Дата последнего изменения - M
	if (pField->fieldLookup("M", &oObj)->isString())
	{
		m_unAFlags |= (1 << 5);
		TextString* s = new TextString(oObj.getString());
		m_sM = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();

	// 7 - Наличие/Отсутствие внешнего вида
	if (pField->fieldLookup("AP", &oObj)->isDict() && oObj.dictGetLength())
		m_unAFlags |= (1 << 6);
	oObj.free();
}

CAnnot::CAnnot(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex)
{
	m_pBorder = NULL;
	m_unAnnotFlag = 0;
	m_unAFlags = 0;

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

	// Координаты - Rect
	m_pRect[0] = 0.0, m_pRect[1] = 0.0, m_pRect[2] = 0.0, m_pRect[3] = 0.0;
	if (oAnnot.dictLookup("Rect", &oObj)->isArray() && oObj.arrayGetLength() == 4)
	{
		ARR_GET_NUM(oObj, 0, m_pRect[0]);
		ARR_GET_NUM(oObj, 1, m_pRect[1]);
		ARR_GET_NUM(oObj, 2, m_pRect[2]);
		ARR_GET_NUM(oObj, 3, m_pRect[3]);

		double dTemp;
		m_dHeight = pdfDoc->getPageCropHeight(nPageIndex + 1);
		if (m_pRect[0] > m_pRect[2])
		{
			dTemp = m_pRect[0]; m_pRect[0] = m_pRect[2]; m_pRect[2] = dTemp;
		}
		if (m_pRect[1] > m_pRect[3])
		{
			dTemp = m_pRect[1]; m_pRect[1] = m_pRect[3]; m_pRect[3] = dTemp;
		}
		dTemp = m_pRect[1]; m_pRect[1] = m_dHeight - m_pRect[3]; m_pRect[3] = m_dHeight - dTemp;
	}
	oObj.free();

	// 1 - Уникальное имя - NM
	if (oAnnot.dictLookup("NM", &oObj)->isString())
	{
		m_unAFlags |= (1 << 0);
		TextString* s = new TextString(oObj.getString());
		m_sNM = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();

	// 2 - Альтернативный текст - Contents
	if (oAnnot.dictLookup("Contents", &oObj)->isString())
	{
		m_unAFlags |= (1 << 1);
		TextString* s = new TextString(oObj.getString());
		m_sContents = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();

	// 3 - Эффекты границы - BE
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

	// 4 - Цвет - C
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

	// 5 - Границы и Dash Pattern - Border/BS
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

	// 6 - Дата последнего изменения - M
	if (oAnnot.dictLookup("M", &oObj)->isString())
	{
		m_unAFlags |= (1 << 5);
		TextString* s = new TextString(oObj.getString());
		m_sM = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		delete s;
	}
	oObj.free();

	// 7 - Наличие/Отсутствие внешнего вида
	if (oAnnot.dictLookup("AP", &oObj)->isDict() && oObj.dictGetLength())
		m_unAFlags |= (1 << 6);
	oObj.free();

	oAnnot.free();
}

CAnnot::~CAnnot()
{
	RELEASEOBJECT(m_pBorder);
}

CAnnot::CBorderType* CAnnot::getBorder(Object* oBorder, bool bBSorBorder)
{
	// Границы и Dash Pattern - Border/BS
	CBorderType* pBorderType = new CBorderType();
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
		Object oObj2;
		pBorderType->dWidth = 1.0;
		ARR_GET_NUM((*oBorder), 2, pBorderType->dWidth);

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
// AP
//------------------------------------------------------------------------

CAnnotAP::CAnnotAP(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CFontList*  pFontList, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, const char* sView, const char* sButtonView, AcroFormField* pField)
{
	m_gfx = NULL;
	m_pFrame = NULL;
	m_pRendererOut = NULL;
	m_pRenderer = NULL;

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

CAnnotAP::CAnnotAP(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CFontList*  pFontList, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, const char* sView, Object* oAnnotRef)
{
	m_gfx = NULL;
	m_pFrame = NULL;
	m_pRendererOut = NULL;
	m_pRenderer = NULL;

	Object oAnnot, oAP;
	XRef* xref = pdfDoc->getXRef();
	oAnnotRef->fetch(xref, &oAnnot);
	if (oAnnot.dictLookup("AP", &oAP)->isDict())
	{
		m_unRefNum = oAnnotRef->getRefNum();

		Init(&oAnnot);
		Init(pdfDoc, pFontManager, pFontList, nRasterW, nRasterH, nBackgroundColor, nPageIndex);
		Draw(pdfDoc, &oAP, nRasterH, nBackgroundColor, oAnnotRef, sView);
	}
	oAP.free(); oAnnot.free();

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

void CAnnotAP::Init(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CFontList*  pFontList, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex)
{
	Page* pPage = pdfDoc->getCatalog()->getPage(nPageIndex + 1);

	double dWidth  = pdfDoc->getPageCropWidth(nPageIndex + 1);
	double dHeight = pdfDoc->getPageCropHeight(nPageIndex + 1);

	m_dWScale = (double)nRasterW / dWidth;
	m_dHScale = (double)nRasterH / dHeight;
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
	m_pRenderer->put_Width ((m_dx2 - m_dx1 + (2 + m_dWTale) * dWidth  / (double)nRasterW) * 25.4 / 72.0);
	m_pRenderer->put_Height((m_dy2 - m_dy1 + (2 + m_dHTale) * dHeight / (double)nRasterH) * 25.4 / 72.0);
	if (nBackgroundColor != 0xFFFFFF)
		m_pRenderer->CommandLong(c_nDarkMode, 1);

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
	m_nRx1 = (int)round(m_dx1 * m_dWScale) - 1;
	m_nRy1 = nRasterH - (int)round(m_dy2 * m_dHScale) - 1;
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
		ARR_GET_NUM(oObj, 0, m_dx1);
		ARR_GET_NUM(oObj, 1, m_dy1);
		ARR_GET_NUM(oObj, 2, m_dx2);
		ARR_GET_NUM(oObj, 3, m_dy2);

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
				m_pRenderer->SetCoordTransformOffset(-m_dx1 * m_dWScale + 1 + m_dWTale / 2, m_dy2 * m_dHScale - nRasterH + 1 + m_dHTale / 2);
				DrawAppearance(pdfDoc, nPageIndex + 1, pField, m_gfx, arrAPName[j], pView->sASName.c_str());
				WriteAppearance(nBackgroundColor, pView);
				pView->nBlendMode = GetBlendMode();
				m_arrAP.push_back(pView);
			}
		}
		else if (!oObj.isNull())
		{
			CAnnotAPView* pView = new CAnnotAPView();
			pView->sAPName = arrAPName[j];
			m_pRenderer->SetCoordTransformOffset(-m_dx1 * m_dWScale + 1 + m_dWTale / 2, m_dy2 * m_dHScale - nRasterH + 1 + m_dHTale / 2);
			DrawAppearance(pdfDoc, nPageIndex + 1, pField, m_gfx, arrAPName[j], NULL);
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

	std::vector<const char*> arrAPName { "N", "D", "R" };
	for (unsigned int j = 0; j < arrAPName.size(); ++j)
	{
		if (sView && strcmp(sView, arrAPName[j]) != 0)
			continue;
		CAnnotAPView* pView = NULL;
		Object oObj;
		if (oAP->dictLookup(arrAPName[j], &oObj)->isStream())
		{
			pView = new CAnnotAPView();
			pView->sAPName = arrAPName[j];

			m_pRenderer->SetCoordTransformOffset(-m_dx1 * m_dWScale + 1 + m_dWTale / 2, m_dy2 * m_dHScale - nRasterH + 1 + m_dHTale / 2);

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
	oRes.AddInt(m_nRx1);
	oRes.AddInt(m_nRy1);
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
	oRes.AddInt(m_unPage);
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

void CMarkupAnnot::ToWASM(NSWasm::CData& oRes)
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
		oRes.WriteString(m_sRC);
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

	CMarkupAnnot::ToWASM(oRes);

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

	CMarkupAnnot::ToWASM(oRes);

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

	CMarkupAnnot::ToWASM(oRes);

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

	CMarkupAnnot::ToWASM(oRes);

	oRes.AddInt((unsigned int)m_arrQuadPoints.size());
	for (int i = 0; i < m_arrQuadPoints.size(); ++i)
		oRes.AddDouble(m_arrQuadPoints[i]);
}

void CAnnotSquareCircle::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(m_nSubtype); // Square, Circle

	CMarkupAnnot::ToWASM(oRes);

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

	CMarkupAnnot::ToWASM(oRes);

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

	CMarkupAnnot::ToWASM(oRes);

	oRes.WriteBYTE(m_nQ);
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

	CMarkupAnnot::ToWASM(oRes);

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

	CMarkupAnnot::ToWASM(oRes);

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
		// Освобождение памяти необходимо вызвать с js стороны
		if (m_pEF->m_pF)
		{
			nFlag |= (1 << 0);
			WRITE_EF(m_pEF->m_pF);
		}
		if (m_pEF->m_pUF)
		{
			nFlag |= (1 << 1);
			WRITE_EF(m_pEF->m_pUF);
		}
		if (m_pEF->m_pDOS)
		{
			nFlag |= (1 << 2);
			WRITE_EF(m_pEF->m_pDOS);
		}
		if (m_pEF->m_pMac)
		{
			nFlag |= (1 << 3);
			WRITE_EF(m_pEF->m_pMac);
		}
		if (m_pEF->m_pUnix)
		{
			nFlag |= (1 << 4);
			WRITE_EF(m_pEF->m_pUnix);
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
}
