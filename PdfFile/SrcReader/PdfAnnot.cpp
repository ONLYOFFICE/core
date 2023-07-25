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
#include "../lib/xpdf/TextString.h"
#include "../lib/xpdf/Link.h"
#include "../lib/xpdf/Annot.h"
#include "../lib/goo/GList.h"

#include "../../DesktopEditor/common/StringExt.h"

namespace PdfReader
{

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
    CAction* pRes = new CAction();
    Object oActType;
    std::string sSName;
    if (oAction->dictLookup("S", &oActType)->isName())
    {
        sSName = oActType.getName();
        pRes->nA = 0; // Default: Unknown
        {
        if (sSName == "GoTo")
            pRes->nA = 1;
        else if (sSName == "GoToR")
            pRes->nA = 2;
        else if (sSName == "GoToE")
            pRes->nA = 3;
        else if (sSName == "Launch")
            pRes->nA = 4;
        else if (sSName == "Thread")
            pRes->nA = 5;
        else if (sSName == "URI")
            pRes->nA = 6;
        else if (sSName == "Sound")
            pRes->nA = 7;
        else if (sSName == "Movie")
            pRes->nA = 8;
        else if (sSName == "Hide")
            pRes->nA = 9;
        else if (sSName == "Named")
            pRes->nA = 10;
        else if (sSName == "SubmitForm")
            pRes->nA = 11;
        else if (sSName == "ResetForm")
            pRes->nA = 12;
        else if (sSName == "ImportData")
            pRes->nA = 13;
        else if (sSName == "JavaScript")
            pRes->nA = 14;
        else if (sSName == "SetOCGState")
            pRes->nA = 15;
        else if (sSName == "Rendition")
            pRes->nA = 16;
        else if (sSName == "Trans")
            pRes->nA = 17;
        else if (sSName == "GoTo3DView")
            pRes->nA = 18;
        }
    }
    oActType.free();

    LinkAction* oAct = LinkAction::parseAction(oAction);
    if (!oAct)
        return pRes;

    LinkActionKind kind = oAct->getKind();
    switch (kind)
    {
    // Переход внутри файла
    case actionGoTo:
    {
        GString* str = ((LinkGoTo*)oAct)->getNamedDest();
        LinkDest* pLinkDest = str ? pdfDoc->findDest(str) : ((LinkGoTo*)oAct)->getDest();
        if (!pLinkDest)
        {
            oRes.AddInt(0);
            oRes.WriteBYTE(8);
            break;
        }
        int pg;
        if (pLinkDest->isPageRef())
        {
            Ref pageRef = pLinkDest->getPageRef();
            pg = pdfDoc->findPage(pageRef.num, pageRef.gen);
        }
        else
            pg = pLinkDest->getPageNum();
        LinkDestKind nKind = pLinkDest->getKind();
        oRes.AddInt(pg - 1);
        oRes.WriteBYTE(nKind);
        switch (nKind)
        {
        case destXYZ:
        case destFitH:
        case destFitBH:
        case destFitV:
        case destFitBV:
        {
            size_t nKindPos = oRes.GetSize();
            int nKindFlag = 0;
            oRes.WriteBYTE(nKindFlag);
            // 1 - left
            if (pLinkDest->getChangeLeft())
            {
                nKindFlag |= (1 << 0);
                oRes.AddDouble(pLinkDest->getLeft());
            }
            // 2 - top
            if (pLinkDest->getChangeTop())
            {
                nKindFlag |= (1 << 1);
                double dTop = pdfDoc->getPageCropHeight(pg) - pLinkDest->getTop();
                oRes.AddDouble(dTop < 0 ? 0.0 : dTop);
            }
            // 3 - zoom
            if (pLinkDest->getChangeZoom() && pLinkDest->getZoom())
            {
                nKindFlag |= (1 << 2);
                oRes.AddDouble(pLinkDest->getZoom());
            }
            oRes.WriteBYTE(nKindFlag, nKindPos);
            break;
        }
        case destFitR:
        {
            oRes.AddDouble(pLinkDest->getLeft());
            double dTop = pdfDoc->getPageCropHeight(pg) - pLinkDest->getTop();
            oRes.AddDouble(dTop < 0 ? 0.0 : dTop);
            oRes.AddDouble(pLinkDest->getRight());
            dTop = pdfDoc->getPageCropHeight(pg) - pLinkDest->getBottom();
            oRes.AddDouble(dTop < 0 ? 0.0 : dTop);
            break;
        }
        case destFit:
        case destFitB:
        default:
            break;
        }
        if (str)
            RELEASEOBJECT(pLinkDest);
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
        TextString* s = new TextString(((LinkURI*)oAct)->getURI());
        std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
        oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
        delete s;
        break;
    }
    // Нестандартно именованные действия
    case actionNamed:
    {
        TextString* s = new TextString(((LinkNamed*)oAct)->getName());
        std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
        oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
        delete s;
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
        TextString* s = new TextString(((LinkJavaScript*)oAct)->getJS());
        std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
        oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
        delete s;
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
        oRes.AddInt(((LinkHide*)oAct)->getHideFlag());
        Object* oHideObj = ((LinkHide*)oAct)->getFields();
        int nHide = 1, k = 0;
        if (oHideObj->isArray())
            nHide = oHideObj->arrayGetLength();
        int nFieldsPos = oRes.GetSize();
        int nFields = 0;
        oRes.AddInt(nFields);

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
                nFields++;
                std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
                oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
                delete s;
            }
            k++;
        } while (k < nHide);
        oHide.free();

        oRes.AddInt(nFields, nFieldsPos);
        break;
    }
    // Неизвестное действие
    case actionUnknown:
    default:
    {
        if (sSName == "ResetForm")
        {
            Object oObj;
            if (oAction->dictLookup("Flags", &oObj)->isInt())
                oRes.AddInt(oObj.getInt());
            else
                oRes.AddInt(0);
            oObj.free();

            if (oAction->dictLookup("Fields", &oObj)->isArray())
            {
                int nFieldsPos = oRes.GetSize();
                int nFields = 0;
                oRes.AddInt(nFields);
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
                        nFields++;
                        std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
                        oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
                        delete s;
                    }
                    oField.free();
                }
                oRes.AddInt(nFields, nFieldsPos);
            }
            else
                oRes.AddInt(0);
            oObj.free();
        }
        break;
    }
    }

    Object oNextAction;
    if (oAction->dictLookup("Next", &oNextAction)->isDict())
    {
        oRes.WriteBYTE(1);
        getAction(pdfDoc, oRes, &oNextAction, nAnnot);
    }
    else
        oRes.WriteBYTE(0);

    RELEASEOBJECT(oAct);
};

std::string getValue(Object* oV)
{
    std::string sRes;
    if (oV->isName())
        sRes = oV->getName();
    else if (oV->isString() || oV->isDict())
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
        if (s)
        {
            sRes = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
            delete s;
        }
    }
    return sRes;
}
void getParents(XRef* xref, Object* oFieldRef, std::vector<CAnnotParent*>& arrParents)
{
    if (!oFieldRef || !xref)
        return;

    Object oField;
    if (!oFieldRef->isRef() || std::find(arrParents.begin(), arrParents.end(), oFieldRef->getRefNum()) != arrParents.end() || !oFieldRef->fetch(xref, &oField)->isDict())
    {
        oField.free();
        return;
    }

    CAnnotParent* pAnnotParent = new CAnnotParent();
    if (!pAnnotParent)
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

    arrParents.push_back(pAnnotParent);

    Object oParentRefObj;
    if (oField.dictLookupNF("Parent", &oParentRefObj)->isRef())
    {
        pAnnotParent->unFlags |= (1 << 3);
        pAnnotParent->unRefNumParent = oParentRefObj.getRefNum();
        getParents(xref, &oParentRefObj, arrParents);
    }
    oParentRefObj.free();
}
CBorderType* getBorder(Object* oBorder, bool bBSorBorder)
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
            Object oV1;
            if (oV.arrayGet(0, &oV1)->isNum())
                pBorderType->dDashesAlternating = oV1.getNum();
            oV1.free();
            if (oV.arrayGet(1, &oV1)->isNum())
                pBorderType->dGaps = oV1.getNum();
            else
                pBorderType->dGaps = pBorderType->dDashesAlternating;
            oV1.free();
        }
        oV.free();
    }
    else
    {
        pBorderType->nType = annotBorderSolid;
        Object oV;
        if (oBorder->arrayGet(2, &oV) && oV.isNum())
            pBorderType->dWidth = oV.getNum();
        oV.free();

        if (oBorder->arrayGetLength() > 3 && oBorder->arrayGet(3, &oV)->isArray() && oV.arrayGetLength() > 1)
        {
            pBorderType->nType = annotBorderDashed;
            Object oV1;
            if (oV.arrayGet(0, &oV1)->isNum())
                pBorderType->dDashesAlternating = oV1.getNum();
            oV1.free();
            if (oV.arrayGet(1, &oV1)->isNum())
                pBorderType->dGaps = oV1.getNum();
            oV1.free();
        }
        oV.free();
    }

    return pBorderType;
}

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

CAnnot::CAnnot(XRef* xref, AcroFormField* pField)
{
    Object oObj, oField;
    pField->getFieldRef(&oObj);
    oObj.fetch(xref, &oField);
    m_unRefNum = oObj.getRefNum();
    oObj.free();

    // Флаг аннотации - F
    m_unAnnotFlag = 0;
    if (pField->fieldLookup("F", &oObj)->isInt())
        m_unAnnotFlag = oObj.getInt();
    oObj.free();

    // Номер страницы - P
    m_unPage = pField->getPageNum() - 1;


    // Координаты - Rect
    pField->getBBox(&m_pRect[0], &m_pRect[1], &m_pRect[2], &m_pRect[3]);
    // TODO при записи передать dHeight
    // double dTemp = m_pRect[1];
    // m_pRect[1] = dHeight - m_pRect[3];
    // m_pRect[3] = dHeight - dTemp;

    m_unAFlags = 0;

    // 5 - Границы и Dash Pattern - Border/BS
    Object oBorder;
    m_pBorder = NULL;
    if (pField->fieldLookup("BS", &oBorder)->isDict())
        m_pBorder = getBorder(&oBorder, true);
    else
    {
        oBorder.free();
        if (pField->fieldLookup("Border", &oBorder)->isArray() && oBorder.arrayGetLength() > 2)
            m_pBorder = getBorder(&oBorder, false);
    }
    oBorder.free();
    if (m_pBorder && m_pBorder->nType != 5)
        m_unAFlags |= (1 << 4);

    // 18 - Родитель - Parent
    if (oField.dictLookupNF("Parent", &oObj)->isRef())
    {
        m_unRefNumParent = oObj.getRefNum();
        m_unAFlags |= (1 << 17);
    }
    oObj.free();
}

CWidgetAnnot::CWidgetAnnot(XRef* xref, AcroFormField* pField) : CAnnot(xref, pField)
{
    Object oObj, oField;
    pField->getFieldRef(&oObj);
    oObj.fetch(xref, &oField);
    oObj.free();

    // Цветовое пространство - из DA
    int nSpace;
    GList *arrColors = pField->getColorSpace(&nSpace);
    for (int j = 0; j < nSpace; ++j)
        m_arrColorSpace.push_back(*(double*)arrColors->get(j));
    deleteGList(arrColors, double);

    // Выравнивание текста - Q
    m_nQ = 0;
    if (pField->fieldLookup("Q", &oObj)->isInt())
        m_nQ = oObj.getInt();
    oObj.free();

    // Тип - FT + флаги
    AcroFormFieldType oType = pField->getAcroFormFieldType();
    m_nType = 0; // Unknown
    switch (oType)
    {
    case acroFormFieldPushbutton:    m_nType = 1;/*sType = "button";*/             break;
    case acroFormFieldRadioButton:   m_nType = 2;/*sType = "radiobutton";*/        break;
    case acroFormFieldCheckbox:      m_nType = 3;/*sType = "checkbox";*/           break;
    case acroFormFieldFileSelect:    m_nType = 4;/*sType = "text""fileselect"*/    break;
    case acroFormFieldMultilineText: m_nType = 4;/*sType = "text""multilinetext"*/ break;
    case acroFormFieldText:          m_nType = 4;/*sType = "text";*/               break;
    case acroFormFieldBarcode:       m_nType = 4;/*sType = "text""barcode"*/       break;
    case acroFormFieldComboBox:      m_nType = 5;/*sType = "combobox";*/           break;
    case acroFormFieldListBox:       m_nType = 6;/*sType = "listbox";*/            break;
    case acroFormFieldSignature:     m_nType = 7;/*sType = "signature";*/          break;
    default:                         m_nType = 0;/*sType = "";*/                   break;
    }

    // Флаг - Ff
    m_unFieldFlag = pField->getFlags();

    // Исходное значение флага данных
    m_unFlags = 0;

    // 1 - Альтернативное имя поля, используется во всплывающей подсказке и сообщениях об ошибке - TU
    DICT_LOOKUP_STRING(pField->fieldLookup, "TU", 0, m_sTU);

    // 2 - Строка стиля по умолчанию - DS
    DICT_LOOKUP_STRING(pField->fieldLookup, "DS", 1, m_sDS);

    // 3 - Эффекты границы - BE
    Object oBorderBE, oBorderBEI;
    if (pField->fieldLookup("BE", &oObj)->isDict() && oObj.dictLookup("S", &oBorderBE)->isName("C") && oObj.dictLookup("I", &oBorderBEI)->isNum())
    {
        m_unFlags |= (1 << 2);
        m_dBE = oBorderBEI.getNum();
    }
    oObj.free(); oBorderBE.free(); oBorderBEI.free();

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
            n_unR = oObj.getInt();
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

    // 16 - Альтернативный текст - Contents
    DICT_LOOKUP_STRING(pField->fieldLookup, "Contents", 15, m_sContents);

    // 17 - Специальный цвет для аннотации - C
    if (pField->fieldLookup("C", &oObj)->isArray())
    {
        m_unFlags |= (1 << 16);
        int nCLength = oObj.arrayGetLength();
        for (int j = 0; j < nCLength; ++j)
        {
            Object oCj;
            m_arrC.push_back(oObj.arrayGet(j, &oCj)->isNum() ? oCj.getNum() : 0.0);
            oCj.free();
        }
    }
    oObj.free();

    // 19 - Частичное имя поля - T
    DICT_LOOKUP_STRING(oField.dictLookup, "T", 18, m_sT);

    // Action - A
    Object oAction;
    if (pField->fieldLookup("A", &oAction)->isDict())
    {
        std::string sAA = "A";
        CAction* pA = getAction(m_pPDFDocument, &oAction);
        pA->sType = sAA;
    }
    oAction.free();
}


CAnnots::CAnnots(XRef* xref, AcroForm* pAcroForms)
{
    Object oObj1, oObj2;

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

    // Родительские Fields
    for (int i = 0, nNum = pAcroForms->getNumFields(); i < nNum; ++i)
    {
        AcroFormField* pField = pAcroForms->getField(i);
        Object oFieldRef, oField;
        if (!pField->getFieldRef(&oFieldRef)->isRef() || !oFieldRef.fetch(xref, &oField)->isDict())
        {
            oFieldRef.free();
            continue;
        }
        Object oParentRefObj;
        if (oField.dictLookupNF("Parent", &oParentRefObj)->isRef())
            getParents(xref, &oParentRefObj, m_arrParents);
        oParentRefObj.free();
    }

    for (int i = 0, nNum = pAcroForms->getNumFields(); i < nNum; ++i)
    {
        AcroFormField* pField = pAcroForms->getField(i);
        if (!pField)
            continue;

        CAnnot* pAnnot = new CWidgetAnnot(xref, pField);
        if (pAnnot)
            m_arrAnnots.push_back(pAnnot);
    }
}

}
