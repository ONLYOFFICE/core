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

#include "../../DesktopEditor/common/Types.h"
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
                double dTop = pdfDoc->getPageCropHeight(ppRes->unPage + 1) - pLinkDest->getTop();
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
            double dTop = pdfDoc->getPageCropHeight(ppRes->unPage + 1) - pLinkDest->getTop();
            ppRes->pRect[1] = (dTop < 0 ? 0.0 : dTop);
            ppRes->pRect[2] = pLinkDest->getRight();
            dTop = pdfDoc->getPageCropHeight(ppRes->unPage + 1) - pLinkDest->getBottom();
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
    if (oAction->dictLookup("Next", &oNextAction)->isDict())
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
    if (!oFieldRef->isRef() || std::find_if(arrParents.begin(), arrParents.end(), [oFieldRef] (CAnnotParent* pAP) { return oFieldRef->getRefNum() == pAP->unRefNum; }) != arrParents.end() || !oFieldRef->fetch(xref, &oField)->isDict())
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
            m_nStyle = (oType == acroFormFieldRadioButton ? 3 : 0);
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
                Object oObj1;
                m_unIFFlag |= (1 << 3);
                m_dA1 = oObj.arrayGet(0, &oObj1)->isNum() ? oObj1.getNum() : 0.5;
                oObj1.free();
                m_dA2 = oObj.arrayGet(1, &oObj1)->isNum() ? oObj1.getNum() : 0.5;
                oObj1.free();
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
        CAction* pA = getAction(pdfDoc, &oAction);
        pA->sType = sAA;
    }
    oAction.free();
}

//------------------------------------------------------------------------
// Annots
//------------------------------------------------------------------------

CAnnots::CAnnots(PDFDoc* pdfDoc)
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

        CAnnot* pAnnot = NULL;
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
            m_arrAnnots.push_back(pAnnot);
    }
}

CAnnots::~CAnnots()
{
    for (int i = 0; i < m_arrParents.size(); ++i)
        RELEASEOBJECT(m_arrParents[i]);

    for (int i = 0; i < m_arrAnnots.size(); ++i)
        RELEASEOBJECT(m_arrAnnots[i]);
}

CAnnot::CAnnot(PDFDoc* pdfDoc, AcroFormField* pField)
{
    m_pBorder = NULL;

    Object oObj, oField;
    XRef* xref = pdfDoc->getXRef();
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
    double dHeight = pdfDoc->getPageCropHeight(m_unPage + 1);
    double dTemp = m_pRect[1];
    m_pRect[1] = dHeight - m_pRect[3];
    m_pRect[3] = dHeight - dTemp;

    m_unAFlags = 0;

    // 3 - Эффекты границы - BE
    Object oBorderBE, oBorderBEI;
    if (pField->fieldLookup("BE", &oObj)->isDict() && oObj.dictLookup("S", &oBorderBE)->isName("C") && oObj.dictLookup("I", &oBorderBEI)->isNum())
    {
        m_unAFlags |= (1 << 2);
        m_dBE = oBorderBEI.getNum();
    }
    oObj.free(); oBorderBE.free(); oBorderBEI.free();


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

CAnnot::~CAnnot()
{
    RELEASEOBJECT(m_pBorder);
}

//------------------------------------------------------------------------
// ToWASM
//------------------------------------------------------------------------

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

void CAnnotParent::ToWASM(NSWasm::CData& oRes)
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
    if (m_unAFlags & (1 << 2))
        oRes.AddDouble(m_dBE);
    if (m_pBorder && (m_unAFlags & (1 << 4)))
        m_pBorder->ToWASM(oRes);
    if (m_unAFlags & (1 << 17))
        oRes.AddInt(m_unRefNumParent);
}

void CBorderType::ToWASM(NSWasm::CData& oRes)
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
    CAnnot::ToWASM(oRes);

    oRes.AddInt(m_arrTC.size());
    for (int i = 0; i < m_arrTC.size(); ++i)
        oRes.AddDouble(m_arrTC[i]);
    oRes.WriteBYTE(m_nQ);
    oRes.WriteBYTE(m_nType);
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
    if (m_unFlags & (1 << 15))
        oRes.WriteString(m_sContents);
    if (m_unFlags & (1 << 16))
    {
        oRes.AddInt(m_arrC.size());
        for (int i = 0; i < m_arrC.size(); ++i)
            oRes.AddDouble(m_arrC[i]);
    }
    if (m_unFlags & (1 << 18))
        oRes.WriteString(m_sT);
    oRes.AddInt(m_arrAction.size());
    for (int i = 0; i < m_arrAction.size(); ++i)
        m_arrAction[i]->ToWASM(oRes);
}

void CActionGoTo::ToWASM(NSWasm::CData& oRes)
{
    oRes.WriteString(sType);
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
    oRes.WriteString(sType);
    oRes.WriteBYTE(6);
    oRes.WriteString(sURI);

    CAction::ToWASM(oRes);
}

void CActionNamed::ToWASM(NSWasm::CData& oRes)
{
    oRes.WriteString(sType);
    oRes.WriteBYTE(10);
    oRes.WriteString(sNamed);

    CAction::ToWASM(oRes);
}

void CActionJavaScript::ToWASM(NSWasm::CData& oRes)
{
    oRes.WriteString(sType);
    oRes.WriteBYTE(14);
    oRes.WriteString(sJavaScript);

    CAction::ToWASM(oRes);
}

void CActionHide::ToWASM(NSWasm::CData& oRes)
{
    oRes.WriteString(sType);
    oRes.WriteBYTE(9);
    oRes.WriteBYTE(bHideFlag ? 1 : 0);
    oRes.AddInt(arrAnnotName.size());
    for (int i = 0; i < arrAnnotName.size(); ++i)
        oRes.WriteString(arrAnnotName[i]);

    CAction::ToWASM(oRes);
}

void CActionResetForm::ToWASM(NSWasm::CData& oRes)
{
    oRes.WriteString(sType);
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
    if (m_nType == 1)
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

}
