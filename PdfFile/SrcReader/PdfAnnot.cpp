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
#include "../lib/goo/GList.h"

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
std::string getValue(Object* oV)
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
		else if (oV->isArray())
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

//------------------------------------------------------------------------
// Widget
//------------------------------------------------------------------------

CAnnotWidgetBtn::CAnnotWidgetBtn(PDFDoc* pdfDoc, AcroFormField* pField) : CAnnotWidget(pdfDoc, pField)
{
	m_unIFFlag = 0;

	Object oObj;

	// Значение поля - V
	int nValueLength;
	Unicode* pValue = pField->getValue(&nValueLength);
	std::string sValue = NSStringExt::CConverter::GetUtf8FromUTF32(pValue, nValueLength);
	gfree(pValue);

	if (pField->fieldLookup("AS", &oObj)->isName())
		sValue = oObj.getName();
	oObj.free();

	// 10 - Включено
	if (sValue != "Off")
		m_unFlags |= (1 << 9);

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
				break;
			}
		}
	}
	oNorm.free(); oObj.free();
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
		m_sV = getValue(&oObj);
		if (!m_sV.empty())
			m_unFlags |= (1 << 9);
	}
	oObj.free();
	oField.free();

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
	if (pField->fieldLookup("AA", &oAA)->isDict())
	{
		for (int j = 0; j < oAA.dictGetLength(); ++j)
		{
			if (oAA.dictGetVal(j, &oAction)->isDict())
			{
				std::string sAA(oAA.dictGetKey(j));
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
	oField.free();
}

CAnnotWidget::~CAnnotWidget()
{
	for (int i = 0; i < m_arrAction.size(); ++i)
		RELEASEOBJECT(m_arrAction[i]);
}

void CAnnotWidget::SetFont(PDFDoc* pdfDoc, AcroFormField* pField, NSFonts::IFontManager* pFontManager, CFontList *pFontList)
{
	GfxFont* gfxFont = NULL;
	GfxFontDict *gfxFontDict = NULL;

	// Шрифт и размер шрифта - из DA
	Ref fontID;
	pField->getFont(&fontID, &m_dFontSize);
	if (fontID.num > 0)
	{
		Object oObj, oField, oFont;
		XRef* xref = pdfDoc->getXRef();
		pField->getFieldRef(&oObj);
		oObj.fetch(xref, &oField);
		oObj.free();

		bool bFindResources = false;

		if (oField.dictLookup("DR", &oObj)->isDict() && oObj.dictLookup("Font", &oFont)->isDict())
		{
			for (int i = 0; i < oFont.dictGetLength(); ++i)
			{
				Object oFontRef;
				if (oFont.dictGetValNF(i, &oFontRef)->isRef() && oFontRef.getRef() == fontID)
				{
					bFindResources = true;
					oFontRef.free();
					break;
				}
				oFontRef.free();
			}
		}
		oFont.free(); oField.free();

		if (!bFindResources)
		{
			oObj.free();
			AcroForm* pAcroForms = pdfDoc->getCatalog()->getForm();
			Object* oAcroForm = pAcroForms->getAcroFormObj();
			if (oAcroForm->isDict() && oAcroForm->dictLookup("DR", &oObj)->isDict() && oObj.dictLookup("Font", &oFont)->isDict())
			{
				for (int i = 0; i < oFont.dictGetLength(); ++i)
				{
					Object oFontRef;
					if (oFont.dictGetValNF(i, &oFontRef)->isRef() && oFontRef.getRef() == fontID)
					{
						bFindResources = true;
						oFontRef.free();
						break;
					}
					oFontRef.free();
				}
			}
			oFont.free();
		}

		if (bFindResources)
		{
			Object oFontRef;
			if (oObj.dictLookupNF("Font", &oFontRef)->isRef())
			{
				if (oFontRef.fetch(xref, &oFont)->isDict())
				{
					Ref r = oFontRef.getRef();
					gfxFontDict = new GfxFontDict(pdfDoc->getXRef(), &r, oFont.getDict());
					gfxFont = gfxFontDict->lookupByRef(fontID);
				}
				oFont.free();
			}
			else if (oFontRef.isDict())
			{
				gfxFontDict = new GfxFontDict(pdfDoc->getXRef(), NULL, oFontRef.getDict());
				gfxFont = gfxFontDict->lookupByRef(fontID);
			}
			oFontRef.free();
		}
		oObj.free();
	}

	std::wstring wsFileName, wsFontName;
	if (gfxFont)
		GetFont(pdfDoc->getXRef(), pFontManager, pFontList, gfxFont, wsFileName, wsFontName);

	m_sFontName = U_TO_UTF8(wsFileName);

	RELEASEOBJECT(gfxFontDict);
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
		ARR_GET_NUM(oObj, 1, m_pRD[1]);
		ARR_GET_NUM(oObj, 2, m_pRD[2]);
		ARR_GET_NUM(oObj, 3, m_pRD[3]);
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
		ARR_GET_NUM(oObj, 1, m_pRD[1]);
		ARR_GET_NUM(oObj, 2, m_pRD[2]);
		ARR_GET_NUM(oObj, 3, m_pRD[3]);
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
	if (oAnnot.dictLookup("RD", &oObj)->isArray())
	{
		m_unFlags |= (1 << 15);
		ARR_GET_NUM(oObj, 0, m_pRD[0]);
		ARR_GET_NUM(oObj, 1, m_pRD[1]);
		ARR_GET_NUM(oObj, 2, m_pRD[2]);
		ARR_GET_NUM(oObj, 3, m_pRD[3]);
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
			oObj1.arrayGet(j, &oObj2);
			TextString* s = getName(&oObj2);
			if (s)
			{
				std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
				m_arrCO.push_back(sStr);
				delete s;
			}
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
		return;
	}

	pAnnotParent->unRefNum = oFieldRef->getRefNum();

	Object oT;
	if (oField.dictLookup("T", &oT)->isString())
	{
		TextString* s = new TextString(oT.getString());
		std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
		pAnnotParent->unFlags |= (1 << 0);
		pAnnotParent->sT = sStr;
		delete s;
	}
	oT.free();

	Object oV;
	if (oField.dictLookup("V", &oV))
	{
		pAnnotParent->sV = getValue(&oV);
		if (!pAnnotParent->sV.empty())
			pAnnotParent->unFlags |= (1 << 1);
	}
	oV.free();

	Object oDV;
	if (oField.dictLookup("DV", &oDV))
	{
		pAnnotParent->sDV = getValue(&oDV);
		if (!pAnnotParent->sDV.empty())
			pAnnotParent->unFlags |= (1 << 2);
	}
	oDV.free();

	m_arrParents.push_back(pAnnotParent);

	Object oParentRefObj;
	if (oField.dictLookupNF("Parent", &oParentRefObj)->isRef())
	{
		pAnnotParent->unFlags |= (1 << 3);
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
			Object oObj2;
			ARR_GET_NUM(oV, 0, pBorderType->dDashesAlternating);
			pBorderType->dGaps = pBorderType->dDashesAlternating;
			ARR_GET_NUM(oV, 1, pBorderType->dGaps);
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
			ARR_GET_NUM(oObj, 0, pBorderType->dDashesAlternating);
			ARR_GET_NUM(oObj, 1, pBorderType->dGaps);
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
		CAnnotAPView* pView = NULL;
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
				pView = new CAnnotAPView();
				pView->sAPName = arrAPName[j];
				pView->sASName = oObj.dictGetKey(k);
				if ((oType == acroFormFieldRadioButton || oType == acroFormFieldCheckbox) && pView->sASName != "Off")
					pView->sASName = "Yes";
				m_pRenderer->SetCoordTransformOffset(-m_dx1 * m_dWScale + 1 + m_dWTale / 2, m_dy2 * m_dHScale - nRasterH + 1 + m_dHTale / 2);
				DrawAppearance(pdfDoc, nPageIndex + 1, pField, m_gfx, arrAPName[j], pView->sASName.c_str());
				WriteAppearance(nBackgroundColor, pView);
			}
		}
		else if (!oObj.isNull())
		{
			pView = new CAnnotAPView();
			pView->sAPName = arrAPName[j];
			m_pRenderer->SetCoordTransformOffset(-m_dx1 * m_dWScale + 1 + m_dWTale / 2, m_dy2 * m_dHScale - nRasterH + 1 + m_dHTale / 2);
			DrawAppearance(pdfDoc, nPageIndex + 1, pField, m_gfx, arrAPName[j], NULL);
			WriteAppearance(nBackgroundColor, pView);
		}
		oObj.free();

		if (pView)
			m_arrAP.push_back(pView);
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
		}
		oObj.free();

		if (pView)
			m_arrAP.push_back(pView);
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
	}
}

void CAnnots::ToWASM(NSWasm::CData& oRes)
{
	// Порядок вычислений - CO
	oRes.AddInt(m_arrCO.size());
	for (int i = 0; i < m_arrCO.size(); ++i)
		oRes.WriteString(m_arrCO[i]);

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
		oRes.AddInt(unRefNumParent);
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
			oRes.AddDouble(m_arrC[i]);
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
		oRes.AddDouble(dDashesAlternating);
		oRes.AddDouble(dGaps);
	}
}

void CAnnotWidget::ToWASM(NSWasm::CData& oRes)
{
	oRes.WriteBYTE(m_nType);

	CAnnot::ToWASM(oRes);

	oRes.WriteString(m_sFontName);
	oRes.AddDouble(m_dFontSize);
	oRes.AddInt(m_arrTC.size());
	for (int i = 0; i < m_arrTC.size(); ++i)
		oRes.AddDouble(m_arrTC[i]);
	oRes.WriteBYTE(m_nQ);
	oRes.AddInt(m_unFieldFlag);
	oRes.AddInt(m_unFlags);
	if (m_unFlags & (1 << 0))
		oRes.WriteString(m_sTU);
	if (m_unFlags & (1 << 1))
		oRes.WriteString(m_sDS);
	if (m_unFlags & (1 << 3))
		oRes.WriteBYTE(m_nH);
	if (m_unFlags & (1 << 5))
	{
		oRes.AddInt(m_arrBC.size());
		for (int i = 0; i < m_arrBC.size(); ++i)
			oRes.AddDouble(m_arrBC[i]);
	}
	if (m_unFlags & (1 << 6))
		oRes.AddInt(m_unR);
	if (m_unFlags & (1 << 7))
	{
		oRes.AddInt(m_arrBG.size());
		for (int i = 0; i < m_arrBG.size(); ++i)
			oRes.AddDouble(m_arrBG[i]);
	}
	if (m_unFlags & (1 << 8))
		oRes.WriteString(m_sDV);
	if (m_unFlags & (1 << 17))
		oRes.AddInt(m_unRefNumParent);
	if (m_unFlags & (1 << 18))
		oRes.WriteString(m_sT);
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

	oRes.AddInt(m_unIFFlag);
	if (m_nType == 27)
	{
		if (m_unFlags & (1 << 10))
			oRes.WriteString(m_sCA);
		if (m_unFlags & (1 << 11))
			oRes.WriteString(m_sRC);
		if (m_unFlags & (1 << 12))
			oRes.WriteString(m_sAC);
	}
	else
		oRes.WriteBYTE(m_nStyle);
	if (m_unFlags & (1 << 13))
		oRes.WriteBYTE(m_nTP);
	if (m_unIFFlag & (1 << 1))
		oRes.WriteBYTE(m_nSW);
	if (m_unIFFlag & (1 << 2))
		oRes.WriteBYTE(m_nS);
	if (m_unIFFlag & (1 << 3))
	{
		oRes.AddDouble(m_dA1);
		oRes.AddDouble(m_dA2);
	}
	if (m_unFlags & (1 << 14))
		oRes.WriteString(m_sAP_N_Yes);
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
			oRes.AddDouble(m_arrIC[i]);
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
			oRes.AddDouble(m_arrIC[i]);
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
			oRes.AddDouble(m_arrIC[i]);
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
}
